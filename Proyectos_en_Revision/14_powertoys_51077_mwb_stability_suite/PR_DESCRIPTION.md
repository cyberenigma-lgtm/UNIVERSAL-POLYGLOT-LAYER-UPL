# Pull Request: Fix Mouse Without Borders reconnection, Name2IP parsing, InputInjector fallback, duplicate worker launch, and stale UI tiles

**Addresses:** https://github.com/microsoft/PowerToys/issues/51077 (Causes 1, 4, 6, 12, and 14)  
**Area:** Mouse Without Borders (`src/modules/MouseWithoutBorders`, `src/settings-ui/Settings.UI`)

---

### Summary of Fixes

1. **Re-arm reconnection on power resume (Cause 1 — `App/Core/InitAndCleanup.cs`)**
   - When a network change occurs while the lock screen is active (`!WinAPI.IsMyDesktopActive()`), `ScheduleReopenSocketsDueToNetworkChanges` sets `PleaseReopenSocket = 0`.
   - Upon `PowerModes.Resume`, `SystemEvents_PowerModeChanged` now re-arms `PleaseReopenSocket = 10` if it is `0`, preventing permanent disconnection until manual "Refresh connections".
2. **Remove duplicate service-mode launch (Cause 4 — `App/Service/Worker.cs`)**
   - `isLoggingOff` and `isByWindows` are constant `false`, causing the `else` branch to unconditionally invoke `RunMeUnderSystemAccount` a second time immediately after the initial launch. Removed the redundant `else` call.
3. **Fix `Name2IP` `Separator` `string[]` concatenation (Cause 6 — `App/Class/SocketStuff.cs`)**
   - `Setting.Values.Name2IpPolicyList + Separator + Setting.Values.Name2IP` evaluated `Separator.ToString()` (`"System.String[]"`), corrupting the first user IP mapping rule and skipping policy-only lists.
   - Replaced with `string.Join(Separator[0], Setting.Values.Name2IpPolicyList ?? string.Empty, Setting.Values.Name2IP ?? string.Empty)` and guarded on either property being non-empty.
4. **Fallback to `SendInput` on `InputInjector` failure (Cause 12 — `App/Class/InputSimulation.cs`)**
   - `Injector.InjectMouseInput` throws `COMException` (`0x8007007A`) or `UnauthorizedAccessException` when the pointer is hidden (e.g., over video or while typing), propagating unhandled to `MainTCPRoutine` and closing the TCP socket.
   - Wrapped `InjectMouseInput` in a targeted `try/catch (System.Exception ex) when (ex is COMException || ex is System.UnauthorizedAccessException)` that falls back to `NativeMethods.SendInput64` / `SendInput`.
5. **Reset missing device tiles to `SocketStatus.NA` (Cause 14 — `ViewModels/MouseWithoutBordersViewModel.cs`)**
   - Machines absent from the 500 ms `MachineSocketState` reply now update `StatusBrush = StatusColors[SocketStatus.NA]` instead of remaining stuck on orange (`Connecting`) or red/purple.

---

### Testing & Safety

- **Zero stack/memory risks:** All changes are localized, non-blocking, and preserve existing locking (`_machineMatrixStringLock`) and P/Invoke marshaling sizes (`Marshal.SizeOf(input64)` / `Marshal.SizeOf(input)`).
- Added `MwbStabilityRegressionTest.cs` and verified live Win32 `SendInput` fallback via UPL+ (`mwb_stability_audit.upl`).

