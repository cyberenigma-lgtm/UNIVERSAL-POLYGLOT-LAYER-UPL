# Issue #136284: Mono Native Memory Leak on Open-Instance Virtual Delegate Invocations

**Target Repository:** [dotnet/runtime](https://github.com/dotnet/runtime)  
**Target Issue:** [#136284](https://github.com/dotnet/runtime/issues/136284)  
**Component:** Mono Runtime Metadata Marshaling (`src/mono/mono/metadata/marshal.c`)  
**Formal Verification Engine:** [UPL+ (Universal Polyglot Layer)](https://github.com/Noxferion/UNIVERSAL-POLYGLOT-LAYER-UPL) v2.2.3  

---

## Overview

This directory contains the complete technical audit, formal verification, unit testing, and upstream patch addressing a long-standing unmanaged memory leak in the Mono runtime across .NET 8, .NET 9, and .NET 10.

### Problem Summary
In `mono_marshal_get_delegate_invoke_internal`, invoking an open-instance delegate pointing to a virtual method causes the runtime to duplicate a `MonoMethodSignature` on the unmanaged heap via `mono_metadata_signature_dup_delegate_invoke_to_target`.
When the wrapper cache hits (`check_generic_delegate_wrapper_cache` or `g_hash_table_lookup`), the function returns early without releasing the duplicated signature. In addition, on cache misses, the signature is not reclaimed after intermediate IL emission.
Because this memory resides outside the managed GC heap, managed profilers report 0 leaks while mobile apps (Android/iOS) and WebAssembly workloads terminate unexpectedly due to OS Out-Of-Memory (OOM) conditions.

---

## Contents of This Directory

1. **[`patch_dotnet_136284.diff`](./patch_dotnet_136284.diff)**
   * Minimal, surgical C patch for `src/mono/mono/metadata/marshal.c`.
   * Adds `g_free (target_method_sig);` on early cache-hit exits and at function completion.
2. **[`PR_DESCRIPTION.md`](./PR_DESCRIPTION.md)**
   * Complete, concise Pull Request template ready for submission to `dotnet/runtime`.
3. **[`DelegateVirtualInvokeMemoryLeakTest.cs`](./DelegateVirtualInvokeMemoryLeakTest.cs)**
   * Automated C# xUnit test for the `dotnet/runtime` test suite.
   * Exercises 100,000 virtual delegate invocations and verifies unmanaged process working set stability.
4. **[`mono_delegate_pure_native.upl`](./mono_delegate_pure_native.upl)**
   * Pure native UPL+ verification test using native structs, pointers, and `defer` guards.
   * Compiles with `uplc.exe run mono_delegate_pure_native.upl` to prove 0-leak execution.
5. **[`DOCUMENTATION_TECHNICAL_EN.md`](./DOCUMENTATION_TECHNICAL_EN.md)**
   * Deep-dive architectural report detailing the root cause, lifecycle analysis, and memory semantics.

---

## How to Apply & Verify

### 1. Apply the upstream patch to `dotnet/runtime`:
```bash
git apply patch_dotnet_136284.diff
```

### 2. Run the UPL+ verification proof:
```bash
uplc run mono_delegate_pure_native.upl
```

### 3. Run the C# regression unit test:
```bash
dotnet test --filter FullyQualifiedName~DelegateVirtualInvokeMemoryLeakTest
```

