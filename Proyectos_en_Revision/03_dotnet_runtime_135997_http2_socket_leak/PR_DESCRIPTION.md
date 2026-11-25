## Description
Fixes #135997

When `PooledConnectionLifetime` is `TimeSpan.Zero`, `_avoidStoringConnections` is set to true and `_cleaningTimer` is null. Consequently, the pool is not added to `_pools`, so `pool.Dispose()` is never called when the handler is disposed.

However, `AddHttp2Connection` was still adding new HTTP/2 connections to `_availableHttp2Connections`, leaving the connection, its socket, and TLS state alive after handler disposal.

This PR:
1. Skips adding connections to `_availableHttp2Connections` when `_poolManager.AvoidStoringConnections` is true.
2. Unconditionally tracks created pools in `_pools` so `Dispose()` tears them down cleanly.
3. Adds a regression unit test.
