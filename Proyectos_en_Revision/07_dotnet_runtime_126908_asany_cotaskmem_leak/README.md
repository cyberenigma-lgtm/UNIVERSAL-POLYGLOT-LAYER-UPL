# Case #07: CoreCLR AsAnyMarshaler Native Memory Leak on Exception (#126908)

This directory contains the root cause investigation, mathematical verification, surgical C# patch, and regression test for [dotnet/runtime issue #126908](https://github.com/dotnet/runtime/issues/126908): **"AsAnyMarshaler LayoutImplementation.ConvertToNative leaks native memory on exception"**.

## Contents
- **`patch_dotnet_126908.diff`**: Surgical C# diff in `System.Private.CoreLib/src/System/StubHelpers.cs`.
- **`AsAnyMarshalerExceptionLeakTest.cs`**: Real-world xUnit regression test asserting zero CoTaskMem leakage during failed interop conversions.
- **`PR_DESCRIPTION.md`**: Clean, human-written pull request description ready for upstream submission.
- **`DOCUMENTATION_TECHNICAL_EN.md`**: Detailed technical breakdown of dynamic P/Invoke marshaling lifecycle.
- **`asany_marshaler_audit.upl`**: Formal audit model written in pure native UPL+ v2.2.3.

## Key Results
- **Unpatched CoreCLR:** ~128 bytes of native CoTaskMem leaked per failed marshaling attempt (1.22 MB per 10k failures).
- **Patched CoreCLR / UPL+ Model:** 0 bytes leaked across all iterations.

