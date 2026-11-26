Thanks for opening this issue @MihaZupan.

We investigated the shared singleton pattern in `HttpGeneralHeaders.TransferEncodingChunked` (delegated from `HttpRequestHeaders` and `HttpResponseHeaders`) and observed how mutability on `TransferCodingHeaderValue.Parameters` causes cross-request contamination.

### Root Cause Analysis

In `HttpGeneralHeaders.cs`, when `TransferEncodingChunked = true` is assigned:
```csharp
if (value == true)
{
    TransferEncoding.Add(HeaderUtilities.TransferEncodingChunked);
}
```
The static instance `HeaderUtilities.TransferEncodingChunked` is added by reference directly to the request's `TransferEncoding` collection. 

Because `TransferCodingHeaderValue.Parameters` is a mutable `ObjectCollection<NameValueHeaderValue>` without a read-only lock or copy-on-write semantics:
1. If client code or middleware adds chunk extensions (`request1.Headers.TransferEncoding.First().Parameters.Add(...)`), it directly mutates `HeaderUtilities.TransferEncodingChunked.Parameters`.
2. Any subsequent request across any thread that sets `TransferEncodingChunked = true` inherits those foreign parameters.
3. Concurrent additions or iterations over `Parameters` during serialization can throw `InvalidOperationException: Collection was modified`.

### Minimal Surgical Fix

Allocating a fresh, request-scoped instance when `TransferEncodingChunked` is set in `HttpGeneralHeaders.cs` ensures strict isolation between independent requests:

```diff
diff --git a/src/libraries/System.Net.Http/src/System/Net/Http/Headers/HttpGeneralHeaders.cs b/src/libraries/System.Net.Http/src/System/Net/Http/Headers/HttpGeneralHeaders.cs
index 9e4f1a2..d8c32b5 100644
--- a/src/libraries/System.Net.Http/src/System/Net/Http/Headers/HttpGeneralHeaders.cs
+++ b/src/libraries/System.Net.Http/src/System/Net/Http/Headers/HttpGeneralHeaders.cs
@@ -82,7 +82,7 @@ public bool? TransferEncodingChunked
                 {
                     if (value == true)
                     {
-                        TransferEncoding.Add(HeaderUtilities.TransferEncodingChunked);
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

This eliminates cross-request parameter leakage and multithreaded collection corruption while preserving standard chunked semantics across both requests and responses.
