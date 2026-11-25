# Issue #136152: [wasm][mono] GC Major Heap Blocks Doubling in Linear Memory

**Target Repository:** [dotnet/runtime](https://github.com/dotnet/runtime)  
**Target Issue:** [#136152](https://github.com/dotnet/runtime/issues/136152)  
**Component:** Mono SGen GC Mark-Sweep Allocator (`src/mono/mono/sgen`)  
**Formal Verification Engine:** [UPL+ (Universal Polyglot Layer)](https://github.com/cyberenigma-lgtm/UNIVERSAL-POLYGLOT-LAYER-UPL) v2.2.3  

---

## Overview

This directory contains the formal audit, WebAssembly linear memory allocator simulation, and upstream surgical patch resolving a critical memory inflation bug in browser-wasm Mono. Every 16 KB major GC heap block allocated individually via `posix_memalign` burns ~32 KB of linear memory due to dlmalloc alignment slack, prematurely exhausting the 2 GB WebAssembly hard limit.

---

## Contents of This Directory

1. **[`patch_dotnet_136152.diff`](./patch_dotnet_136152.diff)**
   * Minimal C patch setting `MS_BLOCK_ALLOC_NUM` to 32 on `TARGET_WASM` and removing redundant 16 KB page alignment for LOS objects.
2. **[`PR_DESCRIPTION.md`](./PR_DESCRIPTION.md)**
   * Minimal, concise Pull Request description ready for `dotnet/runtime`.
3. **[`WasmHeapBlockAllocationBenchmarkTest.cs`](./WasmHeapBlockAllocationBenchmarkTest.cs)**
   * Automated xUnit benchmark verifying linear memory growth efficiency.
4. **[`wasm_heap_audit.upl`](./wasm_heap_audit.upl)**
   * Native UPL+ verification program demonstrating the reduction of linear memory waste from 2.0x down to 1.03x.
5. **[`DOCUMENTATION_TECHNICAL_EN.md`](./DOCUMENTATION_TECHNICAL_EN.md)**
   * Comprehensive architectural analysis of Emscripten dlmalloc alignment slack.

---

## How to Verify

### 1. Run the UPL+ Benchmark:
```bash
uplc run wasm_heap_audit.upl
```

### 2. Apply the Patch to `dotnet/runtime`:
```bash
git apply patch_dotnet_136152.diff
```
