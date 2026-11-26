---
name: System Audit Case Submission
about: Submit a formal audit case or benchmark for tier-1 upstream runtimes
title: '[AUDIT] '
labels: 'audit, upstream-benchmark'
assignees: ''
---

## Upstream Target Reference
- **Upstream Repository:** [e.g., `dotnet/runtime`, `microsoft/PowerToys`, `torvalds/linux`]
- **Issue / Pull Request Link:** [URL to upstream issue]
- **Target Subsystem:** [e.g., Mono WASM JIT, CoreCLR GC, SocketsHttpHandler, Win32 GDI]

## Defect Classification
- [ ] Memory Leak (Native / Managed / Linear Memory)
- [ ] Handle / Kernel Resource Leak (HWND, GDI, Mutex, Socket)
- [ ] Concurrency Deadlock / Race Condition
- [ ] Assertion Failure / Fatal Abort (`g_assert`, `_ASSERTE`)
- [ ] Type Safety / Serialization Invariant Violation

## Failure Mechanism & Trace
Describe the sequence of events leading to the failure or resource degradation.

## UPL+ Formal Verification Model
Link or paste the simulated `.upl` model demonstrating invariant enforcement.

## Upstream Patch Diff
```diff
// Provide the minimal, surgical native patch diff here
```

## Accompanying Test Suite
Link to the standalone reproduction test case (xUnit, C, or integration script).
