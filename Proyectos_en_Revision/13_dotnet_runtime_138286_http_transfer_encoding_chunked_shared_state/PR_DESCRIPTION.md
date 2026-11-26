## Description
Fixes #138286.

Setting `HttpRequestHeaders.TransferEncodingChunked = true` previously reused a shared static instance `s_chunkedHeaderValue`. Because `TransferCodingHeaderValue.Parameters` is a mutable collection without a read-only lock, mutating parameters on one request permanently altered the shared instance for subsequent requests across all threads, leading to cross-request parameter contamination and concurrent collection modification exceptions.

This PR ensures that `TransferEncodingChunked = true` allocates an isolated `TransferCodingHeaderValue(HeaderUtilities.Chunked)` per request, preventing cross-request state leakage.

## Customer Impact
- Prevents cross-request header leakage where chunk extensions added on one request contaminate unrelated requests across different threads or tenants.
- Prevents multithreaded `InvalidOperationException: Collection was modified` crashes during header serialization when concurrent requests access or mutate `TransferEncoding.Parameters`.

## Regression Tests
- Added unit test `TransferEncodingChunked_MutatingParameters_DoesNotPolluteSubsequentRequests` verifying that mutations to `.Parameters` on one request do not leak into another request.
- Added concurrent stress test `TransferEncodingChunked_ConcurrentMutations_ThreadSafeIsolation` verifying thread-safety under parallel requests.

## Risk
Extremely low. Allocates a single 48-byte header object only when chunked transfer encoding is explicitly requested on an HTTP request.
