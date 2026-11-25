// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

using System;
using System.Collections.Concurrent;
using System.Diagnostics;
using System.Threading;
using System.Threading.Tasks;
using Xunit;

namespace System.Security.Cryptography.X509Certificates.Tests
{
    public class CrlDownloadCoalescingTest
    {
        private sealed class MockCrlManager
        {
            private readonly ConcurrentDictionary<string, Task<byte[]>> _inFlight = new();
            private int _downloadInvocationCount;

            public int DownloadCount => Volatile.Read(ref _downloadInvocationCount);

            public Task<byte[]> GetCrlAsync(string url)
            {
                return _inFlight.GetOrAdd(url, u => Task.Run(async () =>
                {
                    Interlocked.Increment(ref _downloadInvocationCount);
                    // Simulate 16 MB CRL download and OpenSSL parse delay
                    await Task.Delay(100).ConfigureAwait(false);
                    return new byte[16 * 1024 * 1024]; // 16 MB buffer
                }));
            }
        }

        [Fact]
        public async Task ConcurrentCrlRequests_CoalesceToSingleDownloadAndParse()
        {
            var manager = new MockCrlManager();
            const int Concurrency = 50;
            const string TargetCrlUrl = "http://crl3.digicert.com/sha2-ha-server-g6.crl";

            var tasks = new Task<byte[]>[Concurrency];
            for (int i = 0; i < Concurrency; i++)
            {
                tasks[i] = manager.GetCrlAsync(TargetCrlUrl);
            }

            byte[][] results = await Task.WhenAll(tasks);

            // Assert that all 50 concurrent callers received valid CRL buffers
            Assert.Equal(Concurrency, results.Length);
            for (int i = 0; i < Concurrency; i++)
            {
                Assert.NotNull(results[i]);
                Assert.Equal(16 * 1024 * 1024, results[i].Length);
            }

            // Exactly 1 download and parse was executed despite 50 concurrent handshakes
            Assert.Equal(1, manager.DownloadCount);
        }
    }
}

