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
The static instance `HeaderUtilities.TransferEncodingChunked` is added by reference directly to the request's `TransferEncoding` collection (`HttpHeaderValueCollection<TransferCodingHeaderValue>`). 

Because `TransferCodingHeaderValue.Parameters` is a mutable `ObjectCollection<NameValueHeaderValue>` without a read-only lock:
1. If client code or middleware adds chunk extensions (`request1.Headers.TransferEncoding.First().Parameters.Add(...)`), it directly mutates `HeaderUtilities.TransferEncodingChunked.Parameters` in place.
2. Any subsequent request across any thread that sets `TransferEncodingChunked = true` inherits those foreign parameters.
3. Concurrent additions or iterations over `Parameters` during serialization can throw `InvalidOperationException: Collection was modified`.

### Minimal Surgical Fix

Allocating a fresh, request-scoped instance when `TransferEncodingChunked = true` in `HttpGeneralHeaders.cs` ensures strict isolation between independent requests:

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

### Value Equality & Removal Semantics (`value == false`)

Notice that setting `TransferEncodingChunked = false` or querying `get` remains completely compatible:
- `HttpHeaderValueCollection<T>` queries `Contains` and `Remove` via `TransferCodingHeaderValue.Equals`.
- Because `TransferCodingHeaderValue.Equals` performs structural value comparison (`string.Equals(_value, other._value, StringComparison.OrdinalIgnoreCase)` and parameter comparison) rather than reference equality (`object.ReferenceEquals`), removing chunked encoding continues to function seamlessly with isolated instances.

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
