#!/usr/bin/env bash
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BIN_DIR="$SCRIPT_DIR/bin"
COMPILER_DIR="$SCRIPT_DIR/compiler"
TESTS_DIR="$SCRIPT_DIR/tests"
TOOLS_DIR="$SCRIPT_DIR/tools"
EXAMPLES_DIR="$SCRIPT_DIR/examples"

mkdir -p "$BIN_DIR"

CC="gcc"
if ! command -v gcc >/dev/null 2>&1; then
    if command -v clang >/dev/null 2>&1; then
        CC="clang"
    else
        echo "[ERROR] No se detecto un compilador C nativo (gcc o clang)."
        exit 1
    fi
fi

echo "===================================================================="
echo "  SUITE MAESTRA DE VERIFICACION INTEGRAL: LENGUAJE UPL+ (C99)"
echo "===================================================================="

[ ! -f "$BIN_DIR/test_lexer" ] && $CC -O2 "$TESTS_DIR/test_lexer.c" "$COMPILER_DIR/lexer.c" -I "$COMPILER_DIR" -o "$BIN_DIR/test_lexer"
[ ! -f "$BIN_DIR/test_parser" ] && $CC -O2 "$TESTS_DIR/test_parser.c" "$COMPILER_DIR/upl_parser.c" "$COMPILER_DIR/lexer.c" "$COMPILER_DIR/upl_ast.c" "$COMPILER_DIR/upl_arena.c" -I "$COMPILER_DIR" -o "$BIN_DIR/test_parser"
[ ! -f "$BIN_DIR/test_comp3" ] && $CC -O2 "$TESTS_DIR/test_comp3.c" "$COMPILER_DIR/runtime/upl_comp3.c" -I "$COMPILER_DIR/runtime" -o "$BIN_DIR/test_comp3"
[ ! -f "$BIN_DIR/upl_supervisor" ] && $CC -O2 "$TOOLS_DIR/upl_supervisor.c" -o "$BIN_DIR/upl_supervisor"
[ ! -f "$BIN_DIR/uplc" ] && $CC -O2 "$COMPILER_DIR/upl_arena.c" "$COMPILER_DIR/lexer.c" "$COMPILER_DIR/upl_ast.c" "$COMPILER_DIR/upl_parser.c" "$COMPILER_DIR/upl_codegen.c" "$COMPILER_DIR/runtime/upl_comp3.c" "$COMPILER_DIR/main.c" -I "$COMPILER_DIR" -I "$COMPILER_DIR/runtime" -o "$BIN_DIR/uplc"
[ ! -f "$BIN_DIR/c2upl" ] && $CC -std=c99 -O2 "$TOOLS_DIR/c2upl.c" -o "$BIN_DIR/c2upl"

chmod +x "$BIN_DIR"/*

echo "[1/6] Verificando Analizador Lexico Formal (Lexer)..."
"$BIN_DIR/test_lexer"

echo "[2/6] Verificando Analizador Sintactico y AST (Parser)..."
"$BIN_DIR/test_parser"

echo "[3/6] Verificando Motor Matematico Decimal Exacto (COMP-3 BCD)..."
"$BIN_DIR/test_comp3"

echo "[4/6] Verificando Supervisor y Guardian Nativo contra Divagaciones..."
"$BIN_DIR/upl_supervisor" --test

echo "[5/6] Verificando Compilacion Binaria End-to-End con uplc..."
"$BIN_DIR/uplc" "$EXAMPLES_DIR/test_compiler_real.upl" -o "$BIN_DIR/test_compiler_real"
"$BIN_DIR/test_compiler_real"

echo "[6/6] Verificando Transpilador C99 a UPL+ (c2upl)..."
"$BIN_DIR/c2upl" --test

echo "===================================================================="
echo "  TODAS LAS PRUEBAS (6/6) SE COMPLETARON CON EXITO (0 FALLOS)"
echo "===================================================================="
