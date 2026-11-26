## Detach HttpListenerContext upon WebSocket protocol upgrade on Unix

### Summary
Fixes https://github.com/dotnet/runtime/issues/101022

On Linux (managed `HttpListener`), upgrading an incoming connection to WebSocket via `AcceptWebSocketAsync` leaves `HttpConnection._context` holding a strong reference to `HttpListenerContext`. This prevents the garbage collector from reclaiming the transient HTTP 1.1 request/response graph (`HttpListenerRequest`, `HttpListenerResponse`, `HttpResponseStream`, `CookieCollection`), causing steady multi-gigabyte Gen 2 memory accumulation under high-frequency WebSocket workloads.

### Fix
- Add `HttpConnection.DetachContextForWebSocketUpgrade()` and invoke it upon successful completion of the `101 Switching Protocols` handshake in `HttpListenerWebSocketContext`.
- Add regression test `HttpListenerWebSocketContextLeakTest.cs`.
