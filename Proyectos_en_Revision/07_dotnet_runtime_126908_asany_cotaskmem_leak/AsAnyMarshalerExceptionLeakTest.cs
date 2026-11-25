// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

using System;
using System.Diagnostics;
using System.Runtime.InteropServices;
using Xunit;

namespace System.Runtime.InteropServices.Tests
{
    public class AsAnyMarshalerExceptionLeakTest
    {
        [StructLayout(LayoutKind.Sequential)]
        private class ValidOuterLayout
        {
            public int Id;
            public InvalidInnerLayout Child;
        }

        [StructLayout(LayoutKind.Explicit)]
        private class InvalidInnerLayout
        {
            // Overlapping managed reference field triggering TypeLoadException / MarshalDirectiveException during layout conversion
            [FieldOffset(0)]
            public string Name;
            [FieldOffset(0)]
            public int Value;
        }

        [DllImport("kernel32.dll", EntryPoint = "SetLastError")]
        private static extern void DummyNativeCall([MarshalAs(UnmanagedType.AsAny)] object obj);

        [Fact]
        public void AsAnyMarshaler_DoesNotLeakCoTaskMem_WhenConversionThrows()
        {
            var invalidInstance = new ValidOuterLayout
            {
                Id = 42,
                Child = new InvalidInnerLayout()
            };

            // Warm-up to load runtime types and JIT stubs
            try
            {
                DummyNativeCall(invalidInstance);
            }
            catch (Exception)
            {
                // Expected conversion failure
            }

            GC.Collect();
            GC.WaitForPendingFinalizers();
            GC.Collect();

            long initialMemory = Process.GetCurrentProcess().WorkingSet64;

            const int Iterations = 5000;
            for (int i = 0; i < Iterations; i++)
            {
                try
                {
                    DummyNativeCall(invalidInstance);
                }
                catch (Exception)
                {
                    // Expected
                }
            }

            GC.Collect();
            GC.WaitForPendingFinalizers();
            GC.Collect();

            long finalMemory = Process.GetCurrentProcess().WorkingSet64;
            long delta = finalMemory - initialMemory;

            // In unpatched runtime, each conversion failure leaks CoTaskMem allocated before the exception.
            // 5000 iterations * ~128 bytes = ~640 KB unmanaged leak accumulating directly in process working set.
            // With fix, delta remains strictly within OS paging noise (< 1 MB).
            const long MaxAcceptableDelta = 1024 * 1024; // 1 MB
            Assert.True(delta < MaxAcceptableDelta,
                $"Native memory leaked {delta / 1024} KB after {Iterations} failed AsAny conversions (Expected < {MaxAcceptableDelta / 1024} KB).");
        }
    }
}

