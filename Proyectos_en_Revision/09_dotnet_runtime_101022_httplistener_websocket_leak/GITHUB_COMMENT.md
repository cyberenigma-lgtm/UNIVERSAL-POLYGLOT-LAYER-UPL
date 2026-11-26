The difference in memory behavior between Windows and Linux stems from the platform implementation: on Windows, `HttpListener` is backed by the kernel driver `http.sys`, whereas on Linux it relies on the managed `HttpConnection.cs` state machine.

### Root Cause
When upgrading an incoming connection to WebSocket via `context.AcceptWebSocketAsync()`:
1. `HttpConnection._context` holds a strong reference to the active `HttpListenerContext`.
2. Once the `101 Switching Protocols` handshake is completed and control transitions to `HttpListenerWebSocketContext`, `HttpConnection._context` is **never cleared or detached**.
3. As a result, the full HTTP 1.1 request/response graph (`HttpListenerRequest`, `HttpListenerResponse`, `HttpResponseStream`, `CookieCollection`, and buffers) remains strongly rooted for the entire duration of the WebSocket connection (and socket pooling cycle).
4. In workloads with continuous WebSocket connect/disconnect cycles, thousands of these context instances get promoted to Gen 2, resulting in the steady >1.3 GB memory accumulation reported in the heap dumps.

### Proposed Fix in `HttpConnection.cs`
Upon completing the WebSocket protocol upgrade in `HttpListenerWebSocketContext`, invoke `DetachContextForWebSocketUpgrade()` to clear `_context`:

```csharp
// HttpConnection.cs
internal void DetachContextForWebSocketUpgrade()
{
    if (_context != null)
    {
        _context = null;
    }
}
```

This severs the link between the long-lived physical connection and the transient HTTP 1.1 handshake objects, allowing `HttpListenerContext` to be collected in Gen 0/1 immediately after the upgrade.

Standalone patch, UPL+ lifecycle audit, and xUnit regression test available here:
https://github.com/cyberenigma-lgtm/UNIVERSAL-POLYGLOT-LAYER-UPL/tree/main/Proyectos_en_Revision/09_dotnet_runtime_101022_httplistener_websocket_leak
