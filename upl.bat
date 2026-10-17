@echo off
setlocal

set "UPLC_EXE=%~dp0bin\uplc.exe"

if not exist "%UPLC_EXE%" (
    echo [UPLC] Compilando compilador nativo UPL+ (Arquitectura Dragon Book C99)...
    if not exist "%~dp0bin" mkdir "%~dp0bin"
    gcc -O2 "%~dp0compiler\upl_arena.c" "%~dp0compiler\lexer.c" "%~dp0compiler\upl_ast.c" "%~dp0compiler\upl_parser.c" "%~dp0compiler\upl_codegen.c" "%~dp0compiler\runtime\upl_comp3.c" "%~dp0compiler\main.c" -I "%~dp0compiler" -I "%~dp0compiler\runtime" -o "%UPLC_EXE%"
)

"%UPLC_EXE%" %*
