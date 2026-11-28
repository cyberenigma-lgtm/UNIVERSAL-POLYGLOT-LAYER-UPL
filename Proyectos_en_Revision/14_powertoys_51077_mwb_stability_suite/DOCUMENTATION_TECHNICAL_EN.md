# Technical Analysis: Mouse Without Borders Stability Fixes (#51077)

## 1. Scope & Target Commit

All code references and unified diff hunks are verified against `microsoft/PowerToys` commit `4e6b2506570f459a71bf413d96b96ec471664cf0`.

## 2. Root Causes & Memory/Stack Safety Audit

| Cause | File | Defect | Safety / Stack Verification |
|---|---|---|---|
| **Cause 1** | `InitAndCleanup.cs:148-153` | `PleaseReopenSocket` stays `0` after sleep while locked | Atomic `int` assignment on power event callback; no new locks or allocations |
| **Cause 4** | `Worker.cs:160-163` | Dead `else` spawns a duplicate `RunMeUnderSystemAccount` pair | Pure removal of redundant call; eliminates extra child process & handle churn |
| **Cause 6** | `SocketStuff.cs:904-907` | `string + string[]` emits `"System.String[]"`, breaking single-line `Name2IP` | `string.Join(Separator[0], ...)` with null-coalescing; bounded `Split` |
| **Cause 12** | `InputSimulation.cs:82-103` | Uncaught `COMException` (`0x8007007A`) / `UnauthorizedAccessException` kills TCP socket | Exception filter catches only expected WinRT injector failures and falls back to existing `SendInput64`/`SendInput` with identical `INPUT64`/`INPUT` marshaling |
| **Cause 14** | `MouseWithoutBordersViewModel.cs:518-531` | Omitted peers keep stale `Connecting`/`Error` `StatusBrush` | Dispatches `StatusColors[SocketStatus.NA]` inside existing `_uiDispatcherQueue.TryEnqueue` |

## 3. UPL+ Live Verification (`mwb_stability_audit.upl`)

Compiled and executed natively via `uplc.exe run mwb_stability_audit.upl`:
- Verifies state transitions for Causes 1, 4, 6, 12, and 14.
- Exercises live `user32.dll` `SendInput(1, &in, sizeof(INPUT))` to confirm the fallback path succeeds without stack corruption or access violations.

