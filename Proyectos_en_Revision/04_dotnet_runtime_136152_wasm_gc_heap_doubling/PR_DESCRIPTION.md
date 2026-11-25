## Description
Fixes #136152

On browser-wasm, allocating major GC heap blocks one by one (`MS_BLOCK_ALLOC_NUM = 1`) via `posix_memalign(16384, 16384)` causes Emscripten's dlmalloc to allocate `size + alignment` (32 KB) per block. The unaligned leading part is freed as slack, but because WebAssembly linear memory cannot shrink, consecutive 16 KB blocks alternate with ~16 KB gaps, doubling the linear memory consumed (2.0x overhead).

Since WebAssembly linear memory has a fixed 2 GB ceiling (`EmccMaximumHeapSize`), this cuts usable managed heap in half, causing early OOM aborts.

This PR:
1. Sets `MS_BLOCK_ALLOC_NUM` to 32 on `TARGET_WASM`, amortizing the 16 KB alignment slack over 512 KB slabs (reducing overhead from 2.0x down to ~1.03x).
2. Removes redundant 16 KB alignment on WASM for LOS objects.
3. Adds regression unit test.

