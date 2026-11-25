// Copyright (c) Microsoft Corporation
// The Microsoft Corporation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

using System;
using System.Diagnostics;
using System.Threading;
using System.Threading.Tasks;
using Xunit;

namespace Microsoft.CmdPal.UI.Tests
{
    public class ExtensionHostRateLimiterTest
    {
        private sealed class TestTokenBucket
        {
            private const int MaxTokens = 50;
            private readonly long _refillTicks = Stopwatch.Frequency; // 1 second
            private long _lastRefillTicks = Stopwatch.GetTimestamp();
            private int _availableTokens = MaxTokens;
            private int _throttledCount;

            public bool TryConsume(out int flushedThrottledCount)
            {
                flushedThrottledCount = 0;
                long now = Stopwatch.GetTimestamp();
                long elapsed = now - Interlocked.Read(ref _lastRefillTicks);

                if (elapsed >= _refillTicks)
                {
                    Interlocked.Exchange(ref _lastRefillTicks, now);
                    Interlocked.Exchange(ref _availableTokens, MaxTokens);
                    flushedThrottledCount = Interlocked.Exchange(ref _throttledCount, 0);
                }

                if (Interlocked.Decrement(ref _availableTokens) >= 0)
                {
                    return true;
                }

                Interlocked.Increment(ref _throttledCount);
                return false;
            }
        }

        [Fact]
        public void BurstLogging_IsBoundedAndDoesNotSaturate()
        {
            var bucket = new TestTokenBucket();
            const int TotalBurstMessages = 10000;

            int allowed = 0;
            int throttled = 0;

            for (int i = 0; i < TotalBurstMessages; i++)
            {
                if (bucket.TryConsume(out _))
                {
                    allowed++;
                }
                else
                {
                    throttled++;
                }
            }

            // Exactly 50 messages allowed within initial 1s window, remainder 9950 throttled
            Assert.Equal(50, allowed);
            Assert.Equal(TotalBurstMessages - 50, throttled);
        }

        [Fact]
        public void MultithreadedBurstLogging_MaintainsTokenIntegrity()
        {
            var bucket = new TestTokenBucket();
            const int Threads = 10;
            const int MessagesPerThread = 1000;

            int allowedTotal = 0;
            int throttledTotal = 0;

            Parallel.For(0, Threads, _ =>
            {
                int localAllowed = 0;
                int localThrottled = 0;

                for (int i = 0; i < MessagesPerThread; i++)
                {
                    if (bucket.TryConsume(out _))
                    {
                        localAllowed++;
                    }
                    else
                    {
                        localThrottled++;
                    }
                }

                Interlocked.Add(ref allowedTotal, localAllowed);
                Interlocked.Add(ref throttledTotal, localThrottled);
            });

            Assert.Equal(50, allowedTotal);
            Assert.Equal((Threads * MessagesPerThread) - 50, throttledTotal);
        }
    }
}
