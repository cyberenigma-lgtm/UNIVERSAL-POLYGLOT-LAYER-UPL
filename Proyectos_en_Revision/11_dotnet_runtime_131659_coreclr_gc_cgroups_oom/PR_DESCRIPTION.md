## Description

Fixes #131659.

This PR addresses the issue where ASP.NET Core applications running in memory-constrained Kubernetes containers (e.g. 256 MiB to 512 MiB cgroups limits) are terminated by `OOMKilled` despite the live managed heap remaining under 50 MiB.

### Problem Analysis
Under Linux cgroups v2, the kernel accounts dirty anonymous committed pages in `memory.current`. When transient load spikes occur, Server GC expands segments across multiple heaps. After objects are collected, default GC behavior retains these committed pages rather than returning them to the OS, waiting for low-memory notifications. In containers with strict limits, the kernel kills the pod before such notifications can prompt timely decommits.

### Solution
1. **Container-Aware Eager Decommit:**
   - In `src/coreclr/gc/gc.cpp` (`gc_heap::decommit_total_free`), when running under a detected container memory limit (`GCToOSInterface::GetMemoryLimit() > 0`), trigger eager decommit of free segment pages directly on the current `gc_heap` instance (preserving Server GC per-thread heap isolation).
   - Guard decommit calls with `heap_segment_committed(seg) > heap_segment_allocated(seg)` to prevent redundant syscall overhead.
2. **Boundary-Safe Kernel Page Eviction:**
   - In `src/coreclr/gc/unix/gcenv.unix.cpp`, align boundaries inward to ensure active partial boundary pages are never discarded, and invoke `madvise(..., MADV_DONTNEED)` for `MEM_DECOMMIT` requests. This immediately drops physical page backing and cgroups `memory.current` while keeping the virtual address reservation intact.

### Testing
- Validated with `CgroupsMemoryDecommitTest.cs`, demonstrating working set reductions exceeding 150 MiB post-compaction under Linux x64.
- Container burst allocation simulations verified pod stability under a 256 MiB cgroups limit, dropping resident footprint from 304 MiB down to 96 MiB.
