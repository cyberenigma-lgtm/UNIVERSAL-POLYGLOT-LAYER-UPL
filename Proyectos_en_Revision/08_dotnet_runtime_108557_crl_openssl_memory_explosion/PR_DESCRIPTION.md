## Coalesce in-flight CRL downloads with WeakReference caching in OpenSslCrlCache

### Summary
Fixes https://github.com/dotnet/runtime/issues/108557

On Linux, establishing concurrent TLS handshakes against servers without OCSP stapling causes each connection to independently download and parse large Certificate Revocation Lists (e.g. DigiCert CRLs sized 10–16 MB). Under concurrent workloads, this causes explosive multi-gigabyte native RSS memory spikes (from redundant OpenSSL `X509_REVOKED` tree allocations and glibc arena fragmentation), leading to container OOMKills in Kubernetes.

### Fix
- In `OpenSslCrlCache.cs`, introduce in-flight task coalescing (`ConcurrentDictionary<string, Task<SafeX509CrlHandle>>`) to eliminate redundant downloads and OpenSSL parses.
- Store the active handle in a `WeakReference<SafeX509CrlHandle>` completion cache to permit immediate native deallocation when chain validations complete, avoiding static retention.
- Add regression unit test `CrlDownloadCoalescingTest.cs`.
