// Copyright (c) Microsoft Corporation
// The Microsoft Corporation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

using System;
using System.Diagnostics;
using System.Threading;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace MouseWithoutBorders.UnitTests
{
    [TestClass]
    public class SystemHandleLeakRegressionTest
    {
        /// <summary>
        /// Regression test for microsoft/PowerToys Issue #51448:
        /// Ensures repeated service impersonation and worker process launches do not leak
        /// Token, Event, Thread, or Process handles into the Windows NT kernel handle table.
        /// </summary>
        [TestMethod]
        public void ServiceMode_ImpersonationPolling_DoesNotLeakKernelHandles()
        {
            using var currentProcess = Process.GetCurrentProcess();
            currentProcess.Refresh();
            int baselineHandles = currentProcess.HandleCount;

            const int IterationCount = 1_000;

            // Execute repeated impersonation cycles simulating service polling
            for (int i = 0; i < IterationCount; i++)
            {
                // Simulating WTSQueryUserToken / DuplicateToken / worker execution
                bool success = Common.ImpersonateLoggedOnUserAndDoSomething(() =>
                {
                    // Minimal probe payload
                    Thread.Yield();
                });
            }

            // Allow kernel asynchronous cleanup
            Thread.Sleep(200);
            currentProcess.Refresh();
            int endHandles = currentProcess.HandleCount;
            int handleDelta = endHandles - baselineHandles;

            // In the unfixed service code, 1,000 iterations leak ~6,000 handles (3:2:1 ratio).
            // With deterministic finally/SafeHandle reclamation, handle delta must remain near zero (threshold: <= 10).
            Assert.IsTrue(
                handleDelta <= 10,
                $"Kernel handle leak detected: Process handle count grew by {handleDelta} handles (Baseline: {baselineHandles}, End: {endHandles}) over {IterationCount} cycles."
            );
        }
    }
}
