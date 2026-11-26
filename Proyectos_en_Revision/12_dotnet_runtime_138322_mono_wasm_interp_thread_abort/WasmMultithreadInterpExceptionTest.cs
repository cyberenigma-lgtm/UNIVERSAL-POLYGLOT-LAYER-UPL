// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

using System;
using System.Threading;
using System.Threading.Tasks;
using Xunit;

namespace Mono.Interpreter.Tests.Threading
{
    /// <summary>
    /// Regression test for dotnet/runtime Issue #138322:
    /// Validates that a thread started after another thread has exited
    /// handles caught exceptions cleanly in multithreaded browser WebAssembly
    /// without aborting at interp.c:8299.
    /// </summary>
    public class WasmMultithreadInterpExceptionTest
    {
        [Fact]
        public async Task ThreadStartedAfterPreviousThreadExit_HandlesExceptionWithoutAbort()
        {
            bool thread1Executed = false;
            bool thread2HandledException = false;

            // Step 1: Start Thread 1, execute workload, and allow it to exit completely
            var thread1 = new Thread(() =>
            {
                int dummy = 42;
                dummy++;
                thread1Executed = true;
            });

            thread1.Start();
            thread1.Join();
            Assert.True(thread1Executed, "Thread 1 must finish and exit.");

            // Short yield to allow underlying runtime thread detach / Web Worker recycling
            await Task.Delay(50);

            // Step 2: Start Thread 2 (which may reuse the underlying Web Worker / pthread TLS)
            var thread2 = new Thread(() =>
            {
                try
                {
                    // Throw and catch exception inside Thread 2
                    throw new InvalidOperationException("Test exception in recycled worker");
                }
                catch (InvalidOperationException)
                {
                    // Under Issue #138322, interp.c:8299 aborts before reaching this catch block.
                    thread2HandledException = true;
                }
            });

            thread2.Start();
            thread2.Join();

            Assert.True(thread2HandledException, 
                "Thread 2 must catch and handle the exception cleanly without triggering a runtime abort.");
        }

        [Fact]
        public void RapidSequentialThreadCatchCycles_DoNotCorruptInterpreterFrames()
        {
            const int iterations = 50;

            for (int i = 0; i < iterations; i++)
            {
                bool caught = false;
                var t = new Thread(() =>
                {
                    try
                    {
                        throw new ArgumentException("Cycle exception");
                    }
                    catch (ArgumentException)
                    {
                        caught = true;
                    }
                });

                t.Start();
                t.Join();
                Assert.True(caught, $"Cycle {i} failed to catch exception.");
            }
        }
    }
}
