## Free CoTaskMem in AsAnyMarshaler.LayoutImplementation if conversion throws

### Summary
Fixes https://github.com/dotnet/runtime/issues/126908

In `AsAnyMarshaler.LayoutImplementation.ConvertToNative`, unmanaged memory is allocated via `Marshal.AllocCoTaskMem` before calling `MngdLayoutTypeConvertToUnmanaged`. If an exception occurs during this conversion, control exits before returning the native pointer to the caller stub, bypassing `ClearNative` and permanently leaking the allocated CoTaskMem.

### Fix
Wrap the unmanaged conversion call in a `try / catch` block to free `pNative` via `Marshal.FreeCoTaskMem` and reset `m_pNative` if an exception occurs.
Add regression test `AsAnyMarshalerExceptionLeakTest.cs`.

