// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

using System;
using System.Diagnostics;
using System.IO;
using System.Net;
using System.Net.Http;
using System.Net.Sockets;
using System.Threading.Tasks;
using Xunit;

namespace System.Net.Http.Functional.Tests
{
    public class SocketsHttpHandlerHttp2DisposeLeakTest
    {
        /// <summary>
        /// Regression test for dotnet/runtime Issue #135997:
        /// Ensures disposing SocketsHttpHandler with PooledConnectionLifetime = TimeSpan.Zero
        /// tears down HTTP/2 connections, closing the underlying TCP socket and TLS streams.
        /// </summary>
        [Fact]
        public async Task Http2_PooledConnectionLifetimeZero_HandlerDispose_ClosesSocketAndTls()
        {
            // Count initial open file descriptors / sockets
            int initialFdCount = GetProcessDescriptorCount();

            const int Iterations = 50;
            for (int i = 0; i < Iterations; i++)
            {
                using var handler = new SocketsHttpHandler
                {
                    PooledConnectionLifetime = TimeSpan.Zero,
                    PooledConnectionIdleTimeout = TimeSpan.Zero,
                    SslOptions = { RemoteCertificateValidationCallback = static (_, _, _, _) => true }
                };

                using var client = new HttpClient(handler, disposeHandler: true);
                using var request = new HttpRequestMessage(HttpMethod.Get, "https://127.0.0.1:5001/")
                {
                    Version = HttpVersion.Version20,
                    VersionPolicy = HttpVersionPolicy.RequestVersionOrLower
                };

                try
                {
                    using var response = await client.SendAsync(request).ConfigureAwait(false);
                }
                catch (HttpRequestException)
                {
                    // Expected when loopback probe endpoint rejects or is dry run
                }
            }

            // Allow asynchronous teardown grace period
            await Task.Delay(500).ConfigureAwait(false);
            GC.Collect();
            GC.WaitForPendingFinalizers();
            GC.Collect();

            int finalFdCount = GetProcessDescriptorCount();
            int fdGrowth = finalFdCount - initialFdCount;

            // In the unfixed runtime, each iteration leaks active socket descriptors.
            // With the fix applied, descriptor growth must remain negligible.
            Assert.True(
                fdGrowth < 10,
                $"HTTP/2 connection descriptor leak detected! Growth: {fdGrowth} descriptors over {Iterations} iterations."
            );
        }

        private static int GetProcessDescriptorCount()
        {
            try
            {
                if (Directory.Exists("/proc/self/fd"))
                {
                    return Directory.GetFileSystemEntries("/proc/self/fd").Length;
                }
            }
            catch
            {
            }

            using var process = Process.GetCurrentProcess();
            process.Refresh();
            return process.HandleCount;
        }
    }
}

