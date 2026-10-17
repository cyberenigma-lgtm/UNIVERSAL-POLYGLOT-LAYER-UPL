#!/usr/bin/env bash
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BIN_DIR="$SCRIPT_DIR/bin"
COMPILER_DIR="$SCRIPT_DIR/compiler"
UPLC_BIN="$BIN_DIR/uplc"

mkdir -p "$BIN_DIR"

if [ ! -f "$UPLC_BIN" ]; then
    echo "[UPLC] Compilando compilador nativo UPL+ (Arquitectura Dragon Book C99)..."
    if command -v gcc >/dev/null 2>&1; then
        gcc -O2 "$COMPILER_DIR/upl_arena.c" "$COMPILER_DIR/lexer.c" "$COMPILER_DIR/upl_ast.c" "$COMPILER_DIR/upl_parser.c" "$COMPILER_DIR/upl_codegen.c" "$COMPILER_DIR/runtime/upl_comp3.c" "$COMPILER_DIR/main.c" -I "$COMPILER_DIR" -I "$COMPILER_DIR/runtime" -o "$UPLC_BIN"
    elif command -v clang >/dev/null 2>&1; then
        clang -O2 "$COMPILER_DIR/upl_arena.c" "$COMPILER_DIR/lexer.c" "$COMPILER_DIR/upl_ast.c" "$COMPILER_DIR/upl_parser.c" "$COMPILER_DIR/upl_codegen.c" "$COMPILER_DIR/runtime/upl_comp3.c" "$COMPILER_DIR/main.c" -I "$COMPILER_DIR" -I "$COMPILER_DIR/runtime" -o "$UPLC_BIN"
    else
        echo "[ERROR] No se detecto un compilador C nativo (gcc o clang)."
        exit 1
    fi
fi

chmod +x "$UPLC_BIN"
exec "$UPLC_BIN" "$@"
