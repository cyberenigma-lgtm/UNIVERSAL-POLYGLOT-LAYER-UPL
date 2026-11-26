// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

using System.Linq;
using System.Net.Http.Headers;
using System.Threading.Tasks;
using Xunit;

namespace System.Net.Http.Functional.Tests
{
    public class TransferEncodingChunkedTests
    {
        [Fact]
        public void TransferEncodingChunked_MutatingParameters_DoesNotPolluteSubsequentRequests()
        {
            using var request1 = new HttpRequestMessage(HttpMethod.Post, "https://example.org");
            request1.Headers.TransferEncodingChunked = true;

            TransferCodingHeaderValue header1 = request1.Headers.TransferEncoding.First();
            Assert.Equal("chunked", header1.Value, StringComparer.OrdinalIgnoreCase);
            Assert.Empty(header1.Parameters);

            // Mutate parameters on the first request
            header1.Parameters.Add(new NameValueHeaderValue("custom-ext", "secret-value"));
            Assert.Single(header1.Parameters);

            // Create a completely separate second request
            using var request2 = new HttpRequestMessage(HttpMethod.Post, "https://example.org");
            request2.Headers.TransferEncodingChunked = true;

            TransferCodingHeaderValue header2 = request2.Headers.TransferEncoding.First();
            Assert.Equal("chunked", header2.Value, StringComparer.OrdinalIgnoreCase);

            // In the unpatched baseline, header2 points to the same shared mutable instance,
            // resulting in parameter leakage from request1.
            Assert.Empty(header2.Parameters);
            Assert.NotSame(header1, header2);
        }

        [Fact]
        public async Task TransferEncodingChunked_ConcurrentMutations_ThreadSafeIsolation()
        {
            const int iterationCount = 1000;

            await Parallel.ForEachAsync(Enumerable.Range(0, iterationCount), async (i, ct) =>
            {
                using var request = new HttpRequestMessage(HttpMethod.Post, "https://example.org");
                request.Headers.TransferEncodingChunked = true;

                TransferCodingHeaderValue header = request.Headers.TransferEncoding.First();
                Assert.Equal("chunked", header.Value, StringComparer.OrdinalIgnoreCase);

                if (i % 2 == 0)
                {
                    header.Parameters.Add(new NameValueHeaderValue($"ext-{i}", $"val-{i}"));
                    Assert.Single(header.Parameters);
                }
                else
                {
                    // Unparameterized requests must remain pristine and unpolluted
                    Assert.Empty(header.Parameters);
                }

                await Task.Yield();
            });
        }
    }
}

