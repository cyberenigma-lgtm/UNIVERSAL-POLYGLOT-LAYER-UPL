Thanks for opening this issue @MihaZupan.

We investigated the shared singleton pattern in `HttpRequestHeaders.TransferEncodingChunked` and observed how mutability on `TransferCodingHeaderValue.Parameters` causes cross-request contamination.

### Root Cause Analysis

When `TransferEncodingChunked = true` is assigned:
```csharp
if (value == true)
{
    TransferEncoding.Add(s_chunkedHeaderValue);
}
```
The static instance `s_chunkedHeaderValue` is added by reference directly to the request's `TransferEncoding` collection. 

Because `TransferCodingHeaderValue.Parameters` is a mutable `ObjectCollection<NameValueHeaderValue>` without a read-only lock or copy-on-write semantics:
1. If client code or middleware adds chunk extensions (`request1.Headers.TransferEncoding.First().Parameters.Add(...)`), it directly mutates `s_chunkedHeaderValue.Parameters`.
2. Any subsequent request across any thread that sets `TransferEncodingChunked = true` inherits those foreign parameters.
3. Concurrent additions or iterations over `Parameters` during serialization can throw `InvalidOperationException: Collection was modified`.

### Minimal Surgical Fix

Allocating a fresh, request-scoped instance when `TransferEncodingChunked` is set ensures strict isolation between independent requests:

```diff
diff --git a/src/libraries/System.Net.Http/src/System/Net/Http/Headers/HttpRequestHeaders.cs b/src/libraries/System.Net.Http/src/System/Net/Http/Headers/HttpRequestHeaders.cs
index a8e217d..3b4c19f 100644
--- a/src/libraries/System.Net.Http/src/System/Net/Http/Headers/HttpRequestHeaders.cs
+++ b/src/libraries/System.Net.Http/src/System/Net/Http/Headers/HttpRequestHeaders.cs
@@ -74,7 +74,7 @@ public bool? TransferEncodingChunked
                 {
                     if (value == true)
                     {
-                        TransferEncoding.Add(s_chunkedHeaderValue);
+                        TransferEncoding.Add(new TransferCodingHeaderValue(HeaderUtilities.Chunked));
                     }
                 }
                 else
```

### Verification Test

```csharp
[Fact]
public void TransferEncodingChunked_MutatingParameters_DoesNotPolluteSubsequentRequests()
{
    using var request1 = new HttpRequestMessage(HttpMethod.Post, "https://example.org");
    request1.Headers.TransferEncodingChunked = true;

    TransferCodingHeaderValue header1 = request1.Headers.TransferEncoding.First();
    header1.Parameters.Add(new NameValueHeaderValue("ext", "val"));

    using var request2 = new HttpRequestMessage(HttpMethod.Post, "https://example.org");
    request2.Headers.TransferEncodingChunked = true;

    TransferCodingHeaderValue header2 = request2.Headers.TransferEncoding.First();
    Assert.Empty(header2.Parameters);
    Assert.NotSame(header1, header2);
}
```

This eliminates cross-request parameter leakage and multithreaded collection corruption while preserving standard chunked semantics.
