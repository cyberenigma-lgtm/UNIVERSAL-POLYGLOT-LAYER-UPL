// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

using System;
using System.Diagnostics;
using System.Reflection;
using System.Runtime.CompilerServices;
using Xunit;

namespace Mono.Runtime.Tests
{
    public class DelegateVirtualInvokeMemoryLeakTest
    {
        public interface IPropertyAccessor
        {
            int Value { get; set; }
        }

        public class PropertyHolder : IPropertyAccessor
        {
            public int Value { get; set; }
        }

        /// <summary>
        /// Regression test for dotnet/runtime Issue #136284:
        /// Ensures open-instance delegates to virtual methods do not leak native MonoMethodSignature
        /// structures on invoke wrapper cache hits.
        /// </summary>
        [Fact]
        public static void VirtualDelegateInvoke_DoesNotLeakNativeMemoryOnCacheHit()
        {
            MethodInfo setter = typeof(PropertyHolder).GetProperty(nameof(IPropertyAccessor.Value))!.GetSetMethod()!;
            var instance = new PropertyHolder();

            // Step 1: Warm-up to guarantee the virtual delegate invoke wrapper is cached
            for (int i = 0; i < 500; i++)
            {
                var warmUpDelegate = (Action<PropertyHolder, int>)Delegate.CreateDelegate(
                    typeof(Action<PropertyHolder, int>), null, setter);
                warmUpDelegate(instance, i);
            }

            GC.Collect();
            GC.WaitForPendingFinalizers();
            GC.Collect();

            long initialNativeMemory = GetNativeProcessMemory();

            // Step 2: Stress invocation with 100,000 newly created open-instance virtual delegates
            const int IterationCount = 100_000;
            for (int i = 0; i < IterationCount; i++)
            {
                var action = (Action<PropertyHolder, int>)Delegate.CreateDelegate(
                    typeof(Action<PropertyHolder, int>), null, setter);
                action(instance, i);
            }

            GC.Collect();
            GC.WaitForPendingFinalizers();
            GC.Collect();

            long finalNativeMemory = GetNativeProcessMemory();
            long nativeMemoryGrowth = finalNativeMemory - initialNativeMemory;

            // In the unfixed runtime, 100,000 iterations leak ~8.0 MB of unmanaged native heap.
            // With the fix applied, native heap growth on cache hits is 0 (accounting for minor jitter < 512 KB).
            const long MaxTolerableNativeGrowthBytes = 512 * 1024; // 512 KB threshold
            Assert.True(
                nativeMemoryGrowth < MaxTolerableNativeGrowthBytes,
                $"Native memory leak detected in virtual delegate invoke! Growth: {nativeMemoryGrowth} bytes (~{(nativeMemoryGrowth / 1024.0):F2} KB) over {IterationCount} iterations."
            );
        }

        [MethodImpl(MethodImplOptions.NoInlining)]
        private static long GetNativeProcessMemory()
        {
            using (var process = Process.GetCurrentProcess())
            {
                process.Refresh();
                return process.WorkingSet64;
            }
        }
    }
}
