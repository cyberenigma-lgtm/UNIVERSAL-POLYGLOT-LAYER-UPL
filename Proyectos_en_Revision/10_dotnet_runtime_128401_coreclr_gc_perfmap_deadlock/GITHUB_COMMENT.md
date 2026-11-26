I investigated the hang under `DOTNET_PerfMapEnabled=1` and identified the lock-ordering inversion between `CodeFragmentHeap::m_Lock` and `PerfMap::s_csPerfMap` during GC suspension.

### Root Cause
1. `CodeFragmentHeap::m_Lock` is initialized with `CRST_UNSAFE_ANYMODE`, meaning acquiring it does not alter the calling thread's GC mode. When dynamic interface stubs (e.g. `DispatchStub`, `ResolveStub`) are generated from cooperative code (`GC_COOP`), the worker thread acquires `m_Lock` while remaining in `GC_COOP`.
2. Inside the critical section of `m_Lock`, `PerfMap::LogStubs` is invoked, which acquires `PerfMap::s_csPerfMap` (using default preemptive semantics). Upon leaving the logging block, it attempts to restore the cooperative state via `Thread::RareDisablePreemptiveGC()`.
3. If an asynchronous GC suspension request (`GCToEEInterface::SuspendEE`) arrives while the worker thread is in this path, the thread blocks inside `RareDisablePreemptiveGC()` waiting for the GC to complete.
4. However, the GC thread is simultaneously blocked waiting for this exact worker thread to reach a safe point. Because the worker thread retains `CodeFragmentHeap::m_Lock`, other threads allocating virtual call stubs also block on `m_Lock`, culminating in a permanent 3-way deadlock.

### Proposed Fix
The allocation critical section can be decoupled from symbol mapping I/O:
* Restrict the critical section under `CodeFragmentHeap::m_Lock` strictly to the $O(1)$ memory slice from the active code block.
* Buffer the allocated stub metadata (`PCODE`, size, and stub type name) in a stack structure.
* Release `m_Lock` immediately via RAII destructor (`CrstHolder`).
* Invoke `PerfMap::LogStubs` **outside** `m_Lock`. If the thread transitions to preemptive mode or waits on a GC cycle, it does not retain any internal unmanaged VM allocator locks, allowing GC suspension and other stub allocations to proceed cleanly.
* Added a defensive checked assert in `PerfMap::LogStubs` (`_ASSERTE(!pCurThread->HoldingCrstWithGCUnsafe())`) to enforce compile/runtime lock-hierarchy compliance.

I have a patch and regression test ready. Let me know if you would like me to open a PR for this.
