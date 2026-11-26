// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Runtime.CompilerServices;
using System.Threading;
using System.Threading.Tasks;
using Xunit;

namespace CoreClr.Runtime.Tests.Concurrency
{
    /// <summary>
    /// Regression test for dotnet/runtime Issue #128401:
    /// Validates that dynamic virtual-stub generation under high polymorphic interface dispatch
    /// does not deadlock against GC suspension when PerfMap stub logging is active.
    /// </summary>
    public class CoreClrLockOrderingDeadlockTest
    {
        private interface IPolymorphicWorker
        {
            [MethodImpl(MethodImplOptions.NoInlining)]
            int Compute(int input);
        }

        private class WorkerA : IPolymorphicWorker { public int Compute(int x) => x + 1; }
        private class WorkerB : IPolymorphicWorker { public int Compute(int x) => x + 2; }
        private class WorkerC : IPolymorphicWorker { public int Compute(int x) => x * 3; }
        private class WorkerD : IPolymorphicWorker { public int Compute(int x) => x ^ 4; }
        private class WorkerE : IPolymorphicWorker { public int Compute(int x) => x - 5; }

        [Fact]
        public void PolymorphicDispatch_UnderConcurrentGC_DoesNotDeadlock()
        {
            const int threadCount = 8;
            const int testDurationMs = 3000;
            using var cts = new CancellationTokenSource(testDurationMs);
            var token = cts.Token;

            var workers = new IPolymorphicWorker[]
            {
                new WorkerA(),
                new WorkerB(),
                new WorkerC(),
                new WorkerD(),
                new WorkerE()
            };

            long dispatchesExecuted = 0;
            long gcCollectionsTriggered = 0;

            // Worker threads executing polymorphic interface dispatches to force
            // dynamic VirtualCallStub generation (DispatchStub, ResolveStub).
            var workerTasks = new Task[threadCount];
            for (int i = 0; i < threadCount; i++)
            {
                int threadId = i;
                workerTasks[i] = Task.Run(() =>
                {
                    int index = threadId;
                    while (!token.IsCancellationRequested)
                    {
                        IPolymorphicWorker worker = workers[index % workers.Length];
                        int result = worker.Compute(index);
                        Interlocked.Increment(ref dispatchesExecuted);
                        index++;
                    }
                }, CancellationToken.None);
            }

            // Dedicated stress thread aggressively forcing Generation 1 and Generation 2 GCs
            // to induce thread suspension safe-point synchronization races.
            var gcTask = Task.Run(() =>
            {
                while (!token.IsCancellationRequested)
                {
                    GC.Collect(2, GCCollectionMode.Forced, blocking: true);
                    Interlocked.Increment(ref gcCollectionsTriggered);
                    Thread.Sleep(5);
                }
            }, CancellationToken.None);

            // Wait for duration or timeout
            bool completedCleanly = Task.WaitAll(
                Task.WhenAll(workerTasks),
                gcTask
            ).Wait(testDurationMs + 5000);

            Assert.True(completedCleanly, "Execution timed out: Deadlock detected in virtual stub allocation or GC suspension.");
            Assert.True(Interlocked.Read(ref dispatchesExecuted) > 100_000, "Expected at least 100,000 polymorphic dispatches.");
            Assert.True(Interlocked.Read(ref gcCollectionsTriggered) > 10, "Expected at least 10 GC collections during test window.");
        }
    }
}
