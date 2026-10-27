@echo off
setlocal

echo ====================================================================
echo   SUITE MAESTRA DE VERIFICACION INTEGRAL: LENGUAJE UPL+ (C99)
echo ====================================================================

if not exist "%~dp0bin" mkdir "%~dp0bin"

if not exist "%~dp0bin\test_lexer.exe" gcc -O2 "%~dp0tests\test_lexer.c" "%~dp0compiler\lexer.c" -I "%~dp0compiler" -o "%~dp0bin\test_lexer.exe"
if not exist "%~dp0bin\test_parser.exe" gcc -O2 "%~dp0tests\test_parser.c" "%~dp0compiler\upl_parser.c" "%~dp0compiler\lexer.c" "%~dp0compiler\upl_ast.c" "%~dp0compiler\upl_arena.c" -I "%~dp0compiler" -o "%~dp0bin\test_parser.exe"
if not exist "%~dp0bin\test_comp3.exe" gcc -O2 "%~dp0tests\test_comp3.c" "%~dp0compiler\runtime\upl_comp3.c" -I "%~dp0compiler\runtime" -o "%~dp0bin\test_comp3.exe"
if not exist "%~dp0bin\upl_supervisor.exe" gcc -O2 "%~dp0tools\upl_supervisor.c" -o "%~dp0bin\upl_supervisor.exe"
if not exist "%~dp0bin\uplc.exe" gcc -O2 "%~dp0compiler\upl_arena.c" "%~dp0compiler\lexer.c" "%~dp0compiler\upl_ast.c" "%~dp0compiler\upl_parser.c" "%~dp0compiler\upl_codegen.c" "%~dp0compiler\runtime\upl_comp3.c" "%~dp0compiler\main.c" -I "%~dp0compiler" -I "%~dp0compiler\runtime" -o "%~dp0bin\uplc.exe"
if not exist "%~dp0bin\c2upl.exe" gcc -std=c99 -O2 "%~dp0tools\c2upl.c" -o "%~dp0bin\c2upl.exe"

echo [1/6] Verificando Analizador Lexico Formal (Lexer)...
"%~dp0bin\test_lexer.exe"
if %ERRORLEVEL% neq 0 (
    echo [ERROR] Fallo en la suite del Lexer.
    exit /b %ERRORLEVEL%
)

echo [2/6] Verificando Analizador Sintactico y AST (Parser)...
"%~dp0bin\test_parser.exe"
if %ERRORLEVEL% neq 0 (
    echo [ERROR] Fallo en la suite del Parser.
    exit /b %ERRORLEVEL%
)

echo [3/6] Verificando Motor Matematico Decimal Exacto (COMP-3 BCD)...
"%~dp0bin\test_comp3.exe"
if %ERRORLEVEL% neq 0 (
    echo [ERROR] Fallo en la suite COMP-3.
    exit /b %ERRORLEVEL%
)

echo [4/6] Verificando Preprocesador y Sanitizador de Codigo Fuente...
"%~dp0bin\upl_supervisor.exe" --test
if %ERRORLEVEL% neq 0 (
    echo [ERROR] Fallo en la suite del Preprocesador.
    exit /b %ERRORLEVEL%
)

echo [5/6] Verificando Compilacion Binaria End-to-End con uplc...
"%~dp0bin\uplc.exe" "%~dp0examples\test_compiler_real.upl" -o "%~dp0bin\test_compiler_real.exe"
if %ERRORLEVEL% neq 0 (
    echo [ERROR] Fallo en la compilacion end-to-end.
    exit /b %ERRORLEVEL%
)

"%~dp0bin\test_compiler_real.exe"
if %ERRORLEVEL% neq 0 (
    echo [ERROR] Fallo en la ejecucion del binario compilado.
    exit /b %ERRORLEVEL%
)

if not exist "%~dp0bin\upl_pm.exe" gcc -O2 "%~dp0tools\upl_pm.c" -o "%~dp0bin\upl_pm.exe"
if not exist "%~dp0bin\test_all_modules.exe" gcc -O2 "%~dp0tests\test_all_modules.c" -o "%~dp0bin\test_all_modules.exe"

echo [6/7] Verificando Transpilador C99 a UPL+ (c2upl)...
"%~dp0bin\c2upl.exe" --test
if %ERRORLEVEL% neq 0 (
    echo [ERROR] Fallo en la suite del transpilador c2upl.
    exit /b %ERRORLEVEL%
)

echo [7/7] Verificando Modulos Nativos UPL (28/28 Pruebas Unitarias)...
"%~dp0bin\test_all_modules.exe"
if %ERRORLEVEL% neq 0 (
    echo [ERROR] Fallo en la suite de modulos nativos UPL+.
    exit /b %ERRORLEVEL%
)

echo ====================================================================
echo   TODAS LAS PRUEBAS (7/7) SE COMPLETARON CON EXITO (0 FALLOS)
echo ====================================================================
exit /b 0

