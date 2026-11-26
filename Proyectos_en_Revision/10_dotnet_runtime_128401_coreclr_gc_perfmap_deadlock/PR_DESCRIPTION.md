## Description

Fixes #128401.

This PR resolves a three-way deadlock in CoreCLR on Linux that occurs when `DOTNET_PerfMapEnabled=1` (or dynamic profiling via `dotnet-trace --enable-perfmap`) is active alongside concurrent Garbage Collection suspension and dynamic virtual-call stub generation.

### Problem Analysis
`CodeFragmentHeap::m_Lock` is configured with `CRST_UNSAFE_ANYMODE`. When a thread in cooperative mode (`GC_COOP`) allocates a dynamic stub (e.g., `DispatchStub`), it acquires `m_Lock` while retaining `GC_COOP`.
Previously, `PerfMap::LogStubs` was invoked within this critical section, acquiring `PerfMap::s_csPerfMap` (preemptive semantics). When restoring cooperative state via `Thread::RareDisablePreemptiveGC()`, the thread blocks if a GC suspension cycle is pending. However, the GC thread is simultaneously waiting for this thread to reach a safe point, while other worker threads block on `m_Lock`, resulting in a permanent application hang.

### Solution
1. **Decouple Allocation from Tracing:**
   - Isolated the critical section of `CodeFragmentHeap::m_Lock` to pure memory block reservation ($O(1)$ pointer math).
   - Captured the allocated `PCODE`, block size, and stub type into a stack-allocated `CodeFragmentAllocation` struct.
   - Released `m_Lock` before calling `PerfMap::LogStubs`.
2. **Safe Point Invariant:**
   - By calling `PerfMap::LogStubs` outside `m_Lock`, any preemption switch or GC wait occurs without holding internal code fragment heap locks.
3. **Defensive Assertion:**
   - Added `_ASSERTE(!pCurThread->HoldingCrstWithGCUnsafe())` in `PerfMap::LogStubs` to prevent future regressions where unsafe locks are held during symbol emission.

### Testing
- Validated via stress test `CoreClrLockOrderingDeadlockTest.cs` running 8 worker threads continuously invoking polymorphic interface dispatches under concurrent forced GC collections (`GC.Collect(2, GCCollectionMode.Forced, blocking: true)`).
- Verified zero deadlocks over 100,000+ dispatches with `DOTNET_PerfMapEnabled=1` and `DOTNET_PerfMapStubGranularity=1`.
- Clean CoreCLR runtime build on Linux x64 and Windows x64.
