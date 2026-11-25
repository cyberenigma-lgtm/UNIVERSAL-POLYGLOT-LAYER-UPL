# Issue #135997: SocketsHttpHandler HTTP/2 Socket & Native TLS State Leak

**Target Repository:** [dotnet/runtime](https://github.com/dotnet/runtime)  
**Target Issue:** [#135997](https://github.com/dotnet/runtime/issues/135997)  
**Component:** `System.Net.Http` SocketsHttpHandler (`src/libraries/System.Net.Http`)  
**Formal Verification Engine:** [UPL+ (Universal Polyglot Layer)](https://github.com/cyberenigma-lgtm/UNIVERSAL-POLYGLOT-LAYER-UPL) v2.2.3  

---

## Overview

This directory contains the formal audit, network channel simulation, regression unit tests, and upstream surgical patch for a critical socket and native TLS leak in the .NET runtime networking stack. When `PooledConnectionLifetime = TimeSpan.Zero` is configured on `SocketsHttpHandler`, disposing the handler leaves HTTP/2 multiplexed connections, TCP sockets, and native TLS states active in memory and open in the OS descriptor table.

---

## Contents of This Directory

1. **[`patch_dotnet_135997.diff`](./patch_dotnet_135997.diff)**
   * Surgical C# patch addressing both `HttpConnectionPool.Http2.cs` and `HttpConnectionPoolManager.cs`.
   * Enforces `AvoidStoringConnections` on HTTP/2 connection pooling and guarantees pool tracking for disposal.
2. **[`PR_DESCRIPTION.md`](./PR_DESCRIPTION.md)**
   * Complete, concise Pull Request description ready for `dotnet/runtime`.
3. **[`SocketsHttpHandlerHttp2DisposeLeakTest.cs`](./SocketsHttpHandlerHttp2DisposeLeakTest.cs)**
   * Automated xUnit test verifying descriptor stability across multiple handler lifecycles.
4. **[`http2_pool_audit.upl`](./http2_pool_audit.upl)**
   * Native UPL+ verification program proving zero-socket and zero-TLS context retention under LIFO defer guards.
5. **[`DOCUMENTATION_TECHNICAL_EN.md`](./DOCUMENTATION_TECHNICAL_EN.md)**
   * In-depth architectural report analyzing the broken pooling invariant.

---

## How to Verify

### 1. Run the UPL+ Benchmark:
```bash
uplc run http2_pool_audit.upl
```

### 2. Apply the Patch to `dotnet/runtime`:
```bash
git apply patch_dotnet_135997.diff
```
