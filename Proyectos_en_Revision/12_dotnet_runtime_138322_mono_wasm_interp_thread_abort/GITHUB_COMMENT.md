@ottoman52 

The abort at `interp.c:8299` (`g_assert (context)`) occurs because a thread executing purely AOT-compiled WebAssembly code has not previously entered the interpreter loop (`interp_exec_method`), leaving its thread-local `InterpContext` unallocated (`context == NULL`).

### Root Cause Analysis
1. In mixed-mode WASM (AOT + Interpreter), methods compiled Ahead-of-Time execute natively as WebAssembly functions.
2. When an AOT method throws an exception that must be resumed in an interpreted catch clause, the native unwinder invokes `interp_set_resume_state` to transition execution into the interpreter.
3. At line 8299, `interp_set_resume_state` currently queries `mono_tls_get_interp_context()` and immediately asserts that `context` is non-NULL:
   ```c
   InterpContext *context = mono_tls_get_interp_context ();
   g_assert (context); // Aborts here when context is NULL
   ```
4. On a thread that has only executed AOT code, this fast TLS read returns `NULL`.
5. This directly explains why your `Prime()` workaround (executing a one-instruction `DynamicMethod`) prevents the crash: entering the interpreter once triggers `get_context()`, which allocates and registers the `InterpContext` in the thread's TLS for all subsequent catches.

### Proposed Fix
In `src/mono/mono/mini/interp/interp.c` (`interp_set_resume_state`), lazily initialize `InterpContext` via `get_context()` if it is NULL before asserting and assigning the resume frame and clause:

```c
void
interp_set_resume_state (MonoInterpThreadState *state, InterpFrame *frame, MonoJitExceptionInfo *clause)
{
	InterpContext *context = mono_tls_get_interp_context ();

	if (!context) {
		context = get_context ();
	}
	g_assert (context);

	context->resume_frame = frame;
	context->resume_clause = clause;
}
```

This eliminates the need for manual `Prime()` workarounds and allows threads executing purely AOT code to transition cleanly into interpreted catch clauses.

Stand-alone patch, thread lifecycle audit, and regression test available here:
https://github.com/cyberenigma-lgtm/UNIVERSAL-POLYGLOT-LAYER-UPL/tree/main/Proyectos_en_Revision/12_dotnet_runtime_138322_mono_wasm_interp_thread_abort
