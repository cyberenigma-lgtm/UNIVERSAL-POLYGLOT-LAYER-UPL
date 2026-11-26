// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

using System;
using System.Diagnostics;
using System.IO;
using System.Runtime;
using System.Runtime.InteropServices;
using System.Threading;
using Xunit;

namespace CoreClr.Runtime.Tests.GC
{
    /// <summary>
    /// Regression test for dotnet/runtime Issue #131659:
    /// Validates that under container-constrained memory configurations,
    /// triggering a Gen 2 GC actively reduces the process Working Set / cgroups memory footprint
    /// via madvise(MADV_DONTNEED) decommit rather than retaining stale committed segment pages.
    /// </summary>
    public class CgroupsMemoryDecommitTest
    {
        [Fact]
        public void ContainerConstrainedGC_DecommitsFreePages_ReducesWorkingSet()
        {
            if (!RuntimeInformation.IsOSPlatform(OSPlatform.Linux))
            {
                // Linux cgroups memory.current validation target
                return;
            }

            Process currentProcess = Process.GetCurrentProcess();
            currentProcess.Refresh();
            long initialWorkingSet = currentProcess.WorkingSet64;

            // Step 1: Allocate a temporary burst of byte arrays to expand GC segments
            const int allocationBlocks = 200;
            const int blockSizeBytes = 1024 * 1024; // 1 MB
            byte[][] buffers = new byte[allocationBlocks][];

            for (int i = 0; i < allocationBlocks; i++)
            {
                buffers[i] = new byte[blockSizeBytes];
                // Touch memory to ensure physical page frame commit
                buffers[i][0] = 0xAA;
                buffers[i][blockSizeBytes - 1] = 0xBB;
            }

            currentProcess.Refresh();
            long peakWorkingSet = currentProcess.WorkingSet64;
            Assert.True(peakWorkingSet > initialWorkingSet + (150 * 1024 * 1024), 
                $"Expected peak working set expansion, got: {peakWorkingSet / (1024 * 1024)} MB");

            // Step 2: Release managed object references
            for (int i = 0; i < allocationBlocks; i++)
            {
                buffers[i] = null;
            }
            buffers = null;

            // Step 3: Trigger full blocking Gen 2 collection with compacting decommit
            GCSettings.LargeObjectHeapCompactionMode = GCLargeObjectHeapCompactionMode.CompactOnce;
            System.GC.Collect(2, GCCollectionMode.Forced, blocking: true, compacting: true);
            System.GC.WaitForPendingFinalizers();
            System.GC.Collect(2, GCCollectionMode.Forced, blocking: true, compacting: true);

            // Step 4: Verify that resident memory (Working Set) drops significantly
            currentProcess.Refresh();
            long postGcWorkingSet = currentProcess.WorkingSet64;
            long memoryReclaimed = peakWorkingSet - postGcWorkingSet;

            // In an unpatched runtime retaining segments, memoryReclaimed is negligible (~0-10 MB).
            // With eager decommit via madvise(MADV_DONTNEED), reclaimed memory exceeds 100 MB.
            Assert.True(memoryReclaimed > (80 * 1024 * 1024), 
                $"Expected at least 80 MB decommitted to OS, but only reclaimed {memoryReclaimed / (1024 * 1024)} MB. Post-GC Working Set: {postGcWorkingSet / (1024 * 1024)} MB");
        }
    }
}
