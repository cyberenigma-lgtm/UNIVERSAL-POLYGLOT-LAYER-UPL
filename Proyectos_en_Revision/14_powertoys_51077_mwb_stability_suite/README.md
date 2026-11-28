# Issue #51077: PowerToys Mouse Without Borders Multi-Cause Stability Suite

**Target Repository:** [microsoft/PowerToys](https://github.com/microsoft/PowerToys)  
**Target Issue:** [#51077](https://github.com/microsoft/PowerToys/issues/51077) (`[Mouse Without Borders] Current issues grouped by cause (tracking overview)`)  
**Component:** Mouse Without Borders (`src/modules/MouseWithoutBorders`, `src/settings-ui/Settings.UI`)  
**Formal Verification Engine:** [UPL+ (Universal Polyglot Layer)](https://github.com/cyberenigma-lgtm/UNIVERSAL-POLYGLOT-LAYER-UPL)

---

## Overview

Issue #51077 catalogs 15 confirmed root causes behind 266+ open Mouse Without Borders bug reports. This directory delivers the formal UPL+ audit, live Win32 `SendInput` fallback verification, unit regression tests, and a minimal, surgical upstream patch addressing five high-impact confirmed causes without introducing stack corruption, handle leaks, or API regressions:

1. **Cause 1 — Reconnection stops permanently after lock + sleep (`InitAndCleanup.cs`)**: Re-arms `PleaseReopenSocket` on `PowerModes.Resume` when cleared while the desktop was inactive.
2. **Cause 4 — Duplicate service process-pair launch (`Worker.cs`)**: Removes the dead `else` branch that unconditionally called `RunMeUnderSystemAccount` a second time.
3. **Cause 6 — Single-line `Name2IP` and policy-only IP mappings ignored (`SocketStuff.cs`)**: Replaces `Setting.Values.Name2IpPolicyList + Separator + Setting.Values.Name2IP` (which coerced `string[] Separator` to `"System.String[]"`) with `string.Join(Separator[0], ...)`.
4. **Cause 12 — Unhandled `InputInjector.InjectMouseInput` exceptions drop the TCP connection (`InputSimulation.cs`)**: Catches `COMException` (`0x8007007A`) / `UnauthorizedAccessException` when the pointer is hidden and falls back cleanly to `NativeMethods.SendInput64` / `SendInput`.
5. **Cause 14 — Device tiles in Settings retain stale colours (`MouseWithoutBordersViewModel.cs`)**: Resets machines omitted from the 500 ms status reply to `SocketStatus.NA`.

---

## Contents of This Directory

1. **[`patch_powertoys_51077.diff`](./patch_powertoys_51077.diff)** — Minimal, surgical unified diff against `4e6b2506570f459a71bf413d96b96ec471664cf0`.
2. **[`PR_DESCRIPTION.md`](./PR_DESCRIPTION.md)** — Pull Request description ready for `microsoft/PowerToys`.
3. **[`MwbStabilityRegressionTest.cs`](./MwbStabilityRegressionTest.cs)** — MSTest regression suite covering Causes 6, 12, and 14.
4. **[`mwb_stability_audit.upl`](./mwb_stability_audit.upl)** — Native UPL+ verification benchmark with live Win32 `user32.dll` `SendInput` probing.
5. **[`DOCUMENTATION_TECHNICAL_EN.md`](./DOCUMENTATION_TECHNICAL_EN.md)** — Technical root-cause analysis and safety audit.

---

## How to Verify

### 1. Run the UPL+ Native Verification Suite:
```bash
uplc run mwb_stability_audit.upl
```

### 2. Apply the Patch to PowerToys:
```bash
git apply patch_powertoys_51077.diff
```

