@echo off
setlocal

echo ====================================================================
echo   SUITE MAESTRA DE VERIFICACION INTEGRAL: LENGUAJE UPL+ (C99)
echo ====================================================================

if not exist "%~dp0bin" mkdir "%~dp0bin"

echo [1/5] Verificando Analizador Lexico Formal (Lexer)...
"%~dp0bin\test_lexer.exe"
if %ERRORLEVEL% neq 0 (
    echo [ERROR] Fallo en la suite del Lexer.
    exit /b %ERRORLEVEL%
)

echo [2/5] Verificando Analizador Sintactico y AST (Parser)...
"%~dp0bin\test_parser.exe"
if %ERRORLEVEL% neq 0 (
    echo [ERROR] Fallo en la suite del Parser.
    exit /b %ERRORLEVEL%
)

echo [3/5] Verificando Motor Matematico Decimal Exacto (COMP-3 BCD)...
"%~dp0bin\test_comp3.exe"
if %ERRORLEVEL% neq 0 (
    echo [ERROR] Fallo en la suite COMP-3.
    exit /b %ERRORLEVEL%
)

echo [4/5] Verificando Supervisor y Guardian Nativo contra Divagaciones...
"%~dp0bin\upl_supervisor.exe" --test
if %ERRORLEVEL% neq 0 (
    echo [ERROR] Fallo en la suite del Supervisor.
    exit /b %ERRORLEVEL%
)

echo [5/5] Verificando Compilacion Binaria End-to-End con uplc...
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

echo ====================================================================
echo   TODAS LAS PRUEBAS (5/5) SE COMPLETARON CON EXITO (0 FALLOS)
echo ====================================================================
exit /b 0
