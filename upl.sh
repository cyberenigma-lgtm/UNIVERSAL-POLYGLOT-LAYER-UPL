#!/usr/bin/env bash
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

if [ -f "$SCRIPT_DIR/bin/uplc" ]; then
    exec "$SCRIPT_DIR/bin/uplc" "$@"
elif [ -f "$SCRIPT_DIR/dist/upl/upl" ]; then
    exec "$SCRIPT_DIR/dist/upl/upl" "$@"
elif [ -f "$SCRIPT_DIR/dist/upl/upl.exe" ]; then
    exec "$SCRIPT_DIR/dist/upl/upl.exe" "$@"
else
    exec "$SCRIPT_DIR/bin/uplc.exe" "$@"
fi
