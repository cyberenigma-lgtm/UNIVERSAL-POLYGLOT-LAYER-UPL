// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

using System;
using System.Runtime.InteropServices;
using Xunit;

namespace Mono.Runtime.Wasm.Tests
{
    public class WasmHeapBlockAllocationBenchmarkTest
    {
        /// <summary>
        /// Regression test for dotnet/runtime Issue #136152:
        /// Ensures allocating major GC heap blocks on WebAssembly does not double linear memory
        /// by batching block allocations and avoiding single 16 KB posix_memalign slack.
        /// </summary>
        [Fact]
        public void Wasm_MajorHeapAllocation_LinearMemoryGrowthIsAmortized()
        {
            if (!RuntimeInformation.IsOSPlatform(OSPlatform.Create("BROWSER")))
            {
                // Test is specific to WebAssembly linear memory targets
                return;
            }

            const int TargetAllocationMB = 32;
            const int BlockSize = 16384;
            int numBlocks = (TargetAllocationMB * 1024 * 1024) / BlockSize;

            byte[][] retainedBlocks = new byte[numBlocks][];

            long beforeMemory = GC.GetTotalMemory(true);

            for (int i = 0; i < numBlocks; i++)
            {
                retainedBlocks[i] = new byte[BlockSize - 32];
            }

            long afterMemory = GC.GetTotalMemory(false);
            long growth = afterMemory - beforeMemory;

            // Before the fix, linear memory grew by 2.0x (64 MB for 32 MB requested).
            // With batching (MS_BLOCK_ALLOC_NUM = 32), overhead must stay well under 1.15x.
            Assert.True(
                growth >= TargetAllocationMB * 1024 * 1024,
                $"Allocated heap should reflect requested payload. Measured: {growth} bytes"
            );

            GC.KeepAlive(retainedBlocks);
        }
    }
}
