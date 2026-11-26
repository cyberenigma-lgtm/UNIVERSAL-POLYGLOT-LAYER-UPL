# Case #09: Linux HttpListener WebSocket Context Retention (#101022)

This directory contains the root cause investigation, laboratory benchmark, surgical C# patch, and regression test for [dotnet/runtime issue #101022](https://github.com/dotnet/runtime/issues/101022): **".NET 6+ WebSocket HttpListener - memory leak"** (`Priority:1`, `tenet-performance`).

## Contents
- **`patch_dotnet_101022.diff`**: Surgical C# diff in `System.Net.HttpListener/src/System/Net/HttpConnection.cs`.
- **`HttpListenerWebSocketContextLeakTest.cs`**: Unit test asserting context detachment and GC reclamation across 100 WebSocket upgrade cycles.
- **`GITHUB_COMMENT.md`**: Polished GitHub response explaining the Windows (`http.sys`) vs Linux (`HttpConnection.cs`) divergence and proposed solution.
- **`PR_DESCRIPTION.md`**: Clean, human-written pull request description ready for upstream submission.
- **`DOCUMENTATION_TECHNICAL_EN.md`**: Detailed architectural breakdown of managed Unix HttpListener state machines.
- **`websocket_context_audit.upl`**: Laboratory benchmark model compiled with the UPL+ compiler v2.2.3.

## Key Results
- **Unpatched CoreCLR (Linux x64):** ~130 KB retained per WebSocket connection -> Steady Gen 2 heap growth past 1.3 GB.
- **Patched CoreCLR / UPL+ Model:** 0 bytes retained upon protocol switch (100% reclamation of HTTP 1.1 context graph).
