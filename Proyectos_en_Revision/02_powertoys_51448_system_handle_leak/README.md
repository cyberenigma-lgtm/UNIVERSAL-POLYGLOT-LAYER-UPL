# Issue #51448: PowerToys Mouse Without Borders SYSTEM Process Kernel Handle Leak

**Target Repository:** [microsoft/PowerToys](https://github.com/microsoft/PowerToys)  
**Target Issue:** [#51448](https://github.com/microsoft/PowerToys/issues/51448)  
**Component:** Mouse Without Borders Service Mode (`src/modules/MouseWithoutBorders`)  
**Formal Verification Engine:** [UPL+ (Universal Polyglot Layer)](https://github.com/cyberenigma-lgtm/UNIVERSAL-POLYGLOT-LAYER-UPL) v2.2.3  

---

## Overview

This directory contains the formal audit, live Windows NT kernel benchmark, unit test suite, and upstream surgical patch for a critical handle leak in Microsoft PowerToys. In Service mode, `PowerToys.MouseWithoutBorders.exe` running as `SYSTEM` leaks ~105 unmanaged handles every 15 minutes (~10,000/day) in a fixed 3:2:1 ratio (Tokens : Events : Threads), eventually exhausting the Windows NT kernel handle table.

---

## Contents of This Directory

1. **[`patch_powertoys_51448.diff`](./patch_powertoys_51448.diff)**
   * Minimal, surgical C# patch for `src/modules/MouseWithoutBorders/App/Service/NativeMethods.cs`.
   * Reclaims `pi.hProcess`, `pi.hThread`, `pEnv`, and token handles inside a deterministic `finally` block.
2. **[`PR_DESCRIPTION.md`](./PR_DESCRIPTION.md)**
   * Complete Pull Request description ready for `microsoft/PowerToys`.
3. **[`SystemHandleLeakRegressionTest.cs`](./SystemHandleLeakRegressionTest.cs)**
   * Automated MSTest/xUnit regression test monitoring `Process.GetCurrentProcess().HandleCount`.
4. **[`mwb_handle_audit.upl`](./mwb_handle_audit.upl)**
   * Native UPL+ verification test checking `GetProcessHandleCount` live against the Windows operating system.
5. **[`DOCUMENTATION_TECHNICAL_EN.md`](./DOCUMENTATION_TECHNICAL_EN.md)**
   * In-depth architectural breakdown of the Win32 handle lifecycle and 3:2:1 ratio analysis.

---

## How to Verify

### 1. Run the UPL+ Windows NT Kernel Benchmark:
```bash
uplc run mwb_handle_audit.upl
```

### 2. Apply the Patch to PowerToys:
```bash
git apply patch_powertoys_51448.diff
```
