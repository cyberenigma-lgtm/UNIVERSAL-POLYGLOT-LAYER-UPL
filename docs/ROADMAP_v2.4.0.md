# 🚧 UPL+ v2.4.0 — Roadmap & Pre-Release Evolution Notes (In Testing)

> **Status:** `In Active Development & Local Testing` — Stable public release remains [**v2.3.0**](https://github.com/cyberenigma-lgtm/UNIVERSAL-POLYGLOT-LAYER-UPL/releases/tag/v2.3.0).

This document tracks the capabilities currently being integrated and validated locally ahead of the upcoming **UPL+ v2.4.0 (Sovereign Autonomous Toolchain Edition)** release.

---

## 🎯 Planned Pillars for v2.4.0

1. **SARIF v2.1.0 CI Auditing (`uplc audit --format sarif`):**
   - Native export of static lifecycle checks (`UPL001` handle/memory leaks, `UPL002` `lock_rank` deadlock hazards, `UPL003` mutable `shared` singletons) for GitHub Code Scanning.
2. **Direct WebAssembly Target (`uplc build --target wasm32`):**
   - Official CLI flag building on `GH-Bot-Radar`'s 807-byte `.wasm` core, with standalone browser demo in [`examples/wasm_hello/`](../examples/wasm_hello/index.html).
3. **Unified Polyglot Backends (`MultiLang-ASM`, `NeuroCOBOL`, `UREP`):**
   - `uplc mlasm` / `--target asm-multilang` (57 natural-language assembly lexicons).
   - `uplc cobol <file.cob|cbl|upl>` (NeuroCOBOL + silicon COMP-3 BCD decimal arithmetic).
   - `uplc urep <file.upl>` (UREP FFI safety bridge; currently fixing lvalue codegen in `examples/test_urep_polyglot.upl`).
4. **Native MCP Server v2.4.0 (`upl_mcp`):**
   - Exposes `upl_audit` (text/SARIF), `upl_run`, `upl_build`, and `upl_list_modules` directly to IDEs and coding agents ([`docs/MCP_SERVER.md`](MCP_SERVER.md)).
5. **One-Line Installers & Multi-OS CI Artifacts:**
   - [`install.sh`](../install.sh) (Linux/macOS) and [`install.ps1`](../install.ps1) (Windows PowerShell).
   - Automated `uplc-linux-x64.tar.gz` and `uplc-windows-x64.zip` builds in [`.github/workflows/ci.yml`](../.github/workflows/ci.yml).

---

## 🧪 Pre-Release Testing Checklist

- [x] `test.bat` 8/8 native C99 test suites passing
- [x] `uplc audit --format sarif` valid SARIF v2.1.0 JSON output
- [x] `uplc test <file.upl>` single-file runner
- [x] `uplc cobol tests/test_legacy.cob` execution verified
- [x] `upl_mcp` JSON-RPC `tools/list` with `upl_audit` verified
- [ ] Resolve `examples/test_urep_polyglot.upl` C99 lvalue codegen for `uplc urep`
- [ ] Cross-test `install.sh` and `uplc-linux-x64.tar.gz` on Linux CI runner
- [ ] End-to-end `clang --target=wasm32` verification on `examples/wasm_hello/hello.upl`
