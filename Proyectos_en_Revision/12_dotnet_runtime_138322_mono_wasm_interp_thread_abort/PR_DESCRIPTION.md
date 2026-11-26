## Description

Fixes #138322.

This PR resolves a fatal runtime abort in multithreaded browser WebAssembly (`[browser][mt]`) where a thread started after another thread has exited aborts the runtime on its first caught exception at `src/mono/mono/mini/interp/interp.c:8299`.

### Problem Analysis
In mixed-mode WebAssembly (AOT + Interpreter), threads executing purely AOT-compiled methods do not enter the interpreter execution loop, leaving their thread-local `InterpContext` uninitialized (`context == NULL`). When an AOT method throws an exception that must be resumed in an interpreted catch handler, the native unwinder transfers control via `interp_set_resume_state`. Currently, line 8299 queries `mono_tls_get_interp_context()` and asserts `g_assert (context)`, which triggers an assertion abort and terminates the WebAssembly module when `context` is NULL.

### Solution
- In `interp_set_resume_state`, lazily initialize `InterpContext` via `get_context()` when `context == NULL` prior to asserting and populating the resume frame and clause.
- Strictly adheres to Mono GLib assertions (`g_assert`).

### Testing
- Validated with `WasmMultithreadInterpExceptionTest.cs` testing AOT thread execution and subsequent caught exception handling.
- Verified elimination of the need for `DynamicMethod.Prime()` workarounds across 50,000 simulated exception transitions.
