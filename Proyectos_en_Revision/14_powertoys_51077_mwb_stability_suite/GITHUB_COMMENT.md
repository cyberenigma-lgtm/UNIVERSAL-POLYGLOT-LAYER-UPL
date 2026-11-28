Thanks for putting #51077 together — grouping these by failure mode makes total sense. I dug into the confirmed causes against `4e6b250` and found that five of the highest-impact issues boil down to three underlying patterns:

**1. Socket & reconnection state leaks (Causes 1 & 12)**
When the cursor hides, unhandled WinRT `InputInjector.InjectMouseInput` exceptions (`COMException 0x8007007A` / `UnauthorizedAccessException`) bubble straight up into `MainTCPRoutine` and kill the active socket. On top of that, `ScheduleReopenSocketsDueToNetworkChanges` zeroes out `PleaseReopenSocket` on the lock screen, and `SystemEvents_PowerModeChanged` never re-arms it on `PowerModes.Resume`. That's why connections drop mid-typing or over video and refuse to reconnect after sleep/lock until you manually hit "Refresh connections". Falling back to `NativeMethods.SendInput64` / `SendInput` in `InputSimulation.SendInputEx` and re-arming `PleaseReopenSocket = 10` on resume fixes both.

**2. Duplicate service launches & handle churn (Cause 4)**
In `Worker.StartAsync`, both `isLoggingOff` and `isByWindows` are hardcoded to `false`, so execution always falls through to the `else` branch and calls `RunMeUnderSystemAccount` a second time right after startup. This causes extra short-lived `SYSTEM` processes, handle/CPU churn, and `WSAEADDRINUSE` port-bind retries. Removing that dead `else` branch cleans it up.

**3. Parser type-coercion & stale UI tiles (Causes 6 & 14)**
`SocketStuff.StartNewTcpClient` concatenates `Name2IpPolicyList + Separator + Name2IP` where `Separator` is a `string[]` (producing `"System.String[]"`), which silently breaks single-line IP mappings. Meanwhile, in Settings, `MouseWithoutBordersViewModel` skips machines missing from the 500 ms status reply, leaving tiles stuck on orange/red/purple long after the socket is gone. Switching to `string.Join(Separator[0], ...)` (when either list is non-empty) and resetting omitted machines to `SocketStatus.NA` resolves both.

**Files touched:**
- `src/modules/MouseWithoutBorders/App/Core/InitAndCleanup.cs` (`SystemEvents_PowerModeChanged`)
- `src/modules/MouseWithoutBorders/App/Class/InputSimulation.cs` (`SendInputEx`)
- `src/modules/MouseWithoutBorders/App/Service/Worker.cs` (`StartAsync`)
- `src/modules/MouseWithoutBorders/App/Class/SocketStuff.cs` (`StartNewTcpClient`)
- `src/settings-ui/Settings.UI/ViewModels/MouseWithoutBordersViewModel.cs` (status polling loop)

I put together a clean patch, MSTest regression tests, and the UPL+ verification suite over here if anyone wants to review or pull from it:
https://github.com/cyberenigma-lgtm/UNIVERSAL-POLYGLOT-LAYER-UPL/tree/main/Proyectos_en_Revision/14_powertoys_51077_mwb_stability_suite
