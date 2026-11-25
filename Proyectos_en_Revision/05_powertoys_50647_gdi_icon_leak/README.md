# Issue #50647: PowerToys Workspaces GDI Icon Handles & Bitmaps Leak

**Target Repository:** [microsoft/PowerToys](https://github.com/microsoft/PowerToys)  
**Target Issue:** [#50647](https://github.com/microsoft/PowerToys/issues/50647)  
**Component:** Workspaces Editor (`src/modules/Workspaces`)  
**Formal Verification Engine:** [UPL+ (Universal Polyglot Layer)](https://github.com/cyberenigma-lgtm/UNIVERSAL-POLYGLOT-LAYER-UPL) v2.2.3  

---

## Overview

This directory contains the formal audit, live Win32 GDI resource benchmark, automated regression unit test, and upstream surgical patch for a GDI handle leak in Microsoft PowerToys Workspaces. When application icons are rendered in the Workspaces editor, `HICON` handles and GDI bitmaps are leaked without calling `DestroyIcon`, eventually exhausting the Windows 10,000 GDI limit.

---

## Contents of This Directory

1. **[`patch_powertoys_50647.diff`](./patch_powertoys_50647.diff)**
   * Minimal C# patch cloning icons, destroying native `HICON` handles, and disposing bitmaps/encoders.
2. **[`PR_DESCRIPTION.md`](./PR_DESCRIPTION.md)**
   * Concise Pull Request description ready for `microsoft/PowerToys`.
3. **[`WorkspacesGdiIconLeakTest.cs`](./WorkspacesGdiIconLeakTest.cs)**
   * Automated MSTest unit test verifying `GetGuiResources(..., GR_GDIOBJECTS)` stability.
4. **[`gdi_icon_audit.upl`](./gdi_icon_audit.upl)**
   * Native UPL+ verification test checking GDI handle count live against the Windows operating system.
5. **[`DOCUMENTATION_TECHNICAL_EN.md`](./DOCUMENTATION_TECHNICAL_EN.md)**
   * In-depth architectural analysis of Win32 GDI object lifetimes.

---

## How to Verify

### 1. Run the UPL+ Benchmark:
```bash
uplc run gdi_icon_audit.upl
```

### 2. Apply the Patch to PowerToys:
```bash
git apply patch_powertoys_50647.diff
```

