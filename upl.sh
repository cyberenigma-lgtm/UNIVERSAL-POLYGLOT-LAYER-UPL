#!/usr/bin/env bash
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BIN_DIR="$SCRIPT_DIR/bin"
COMPILER_DIR="$SCRIPT_DIR/compiler"
UPLC_BIN="$BIN_DIR/uplc"

mkdir -p "$BIN_DIR"

if [ ! -f "$UPLC_BIN" ]; then
    echo "[UPLC] Compilando herramienta nativa soberana para POSIX/Linux..."
    if command -v gcc >/dev/null 2>&1; then
        gcc -O2 "$COMPILER_DIR/uplc.c" -o "$UPLC_BIN"
    elif command -v clang >/dev/null 2>&1; then
        clang -O2 "$COMPILER_DIR/uplc.c" -o "$UPLC_BIN"
    else
        echo "[ERROR] No se detectó un compilador nativo (gcc o clang)."
        exit 1
    fi
fi

chmod +x "$UPLC_BIN"
exec "$UPLC_BIN" "$@"
