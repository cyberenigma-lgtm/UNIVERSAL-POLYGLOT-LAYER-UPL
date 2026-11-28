#!/usr/bin/env bash
set -euo pipefail

# UPL+ v2.4.0 One-Line Installer (Linux / macOS)
# Usage: curl -fsSL https://raw.githubusercontent.com/cyberenigma-lgtm/UNIVERSAL-POLYGLOT-LAYER-UPL/main/install.sh | bash

INSTALL_DIR="${UPL_INSTALL_DIR:-$HOME/.local/bin}"
REPO_URL="https://github.com/cyberenigma-lgtm/UNIVERSAL-POLYGLOT-LAYER-UPL"

echo "===================================================================="
echo "  UPL+ Sovereign Toolchain Installer (Linux / macOS)"
echo "===================================================================="

mkdir -p "$INSTALL_DIR"

if [ -f "compiler/main.c" ] && [ -f "tools/upl_pm.c" ]; then
    echo "[*] Local UPL+ source tree detected. Compiling native C99 toolchain..."
    mkdir -p bin
    gcc -O2 compiler/upl_arena.c compiler/lexer.c compiler/upl_ast.c compiler/upl_parser.c compiler/upl_codegen.c compiler/runtime/upl_comp3.c compiler/main.c -I compiler -I compiler/runtime -o bin/uplc
    gcc -O2 tools/upl_pm.c -o bin/upl_pm
    gcc -O2 tools/upl_mcp_server.c -o bin/upl_mcp
    gcc -std=c99 -O2 tools/c2upl.c -o bin/c2upl
    cp bin/uplc bin/upl_pm bin/upl_mcp bin/c2upl "$INSTALL_DIR/"
    chmod +x "$INSTALL_DIR/uplc" "$INSTALL_DIR/upl_pm" "$INSTALL_DIR/upl_mcp" "$INSTALL_DIR/c2upl"
else
    TMP_DIR="$(mktemp -d)"
    trap 'rm -rf "$TMP_DIR"' EXIT
    echo "[*] Cloning UPL+ from $REPO_URL..."
    git clone --depth 1 "$REPO_URL.git" "$TMP_DIR/upl"
    cd "$TMP_DIR/upl"
    mkdir -p bin
    gcc -O2 compiler/upl_arena.c compiler/lexer.c compiler/upl_ast.c compiler/upl_parser.c compiler/upl_codegen.c compiler/runtime/upl_comp3.c compiler/main.c -I compiler -I compiler/runtime -o bin/uplc
    gcc -O2 tools/upl_pm.c -o bin/upl_pm
    gcc -O2 tools/upl_mcp_server.c -o bin/upl_mcp
    gcc -std=c99 -O2 tools/c2upl.c -o bin/c2upl
    cp bin/uplc bin/upl_pm bin/upl_mcp bin/c2upl "$INSTALL_DIR/"
    chmod +x "$INSTALL_DIR/uplc" "$INSTALL_DIR/upl_pm" "$INSTALL_DIR/upl_mcp" "$INSTALL_DIR/c2upl"
fi

echo "[OK] Installed uplc, upl_pm, upl_mcp, and c2upl to: $INSTALL_DIR"
case ":$PATH:" in
    *":$INSTALL_DIR:"*) echo "[OK] $INSTALL_DIR is already in PATH." ;;
    *) echo "[TIP] Add to your shell profile: export PATH=\"$INSTALL_DIR:\$PATH\"" ;;
esac

