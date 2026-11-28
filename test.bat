@echo off
setlocal

echo ====================================================================
echo   SUITE MAESTRA DE VERIFICACION INTEGRAL: LENGUAJE UPL+ (C99)
echo ====================================================================

if not exist "%~dp0bin" mkdir "%~dp0bin"

gcc -O2 "%~dp0tests\test_lexer.c" "%~dp0compiler\lexer.c" -I "%~dp0compiler" -o "%~dp0bin\test_lexer.exe"
gcc -O2 "%~dp0tests\test_parser.c" "%~dp0compiler\upl_parser.c" "%~dp0compiler\lexer.c" "%~dp0compiler\upl_ast.c" "%~dp0compiler\upl_arena.c" -I "%~dp0compiler" -o "%~dp0bin\test_parser.exe"
gcc -O2 "%~dp0tests\test_comp3.c" "%~dp0compiler\runtime\upl_comp3.c" -I "%~dp0compiler\runtime" -o "%~dp0bin\test_comp3.exe"
if not exist "%~dp0bin\upl_supervisor.exe" gcc -O2 "%~dp0tools\upl_supervisor.c" -o "%~dp0bin\upl_supervisor.exe"
gcc -O2 "%~dp0compiler\upl_arena.c" "%~dp0compiler\lexer.c" "%~dp0compiler\upl_ast.c" "%~dp0compiler\upl_parser.c" "%~dp0compiler\upl_codegen.c" "%~dp0compiler\runtime\upl_comp3.c" "%~dp0compiler\main.c" -I "%~dp0compiler" -I "%~dp0compiler\runtime" -o "%~dp0bin\uplc.exe"
if not exist "%~dp0bin\c2upl.exe" gcc -std=c99 -O2 "%~dp0tools\c2upl.c" -o "%~dp0bin\c2upl.exe"

echo [1/8] Verificando Analizador Lexico Formal (Lexer)...
"%~dp0bin\test_lexer.exe"
if %ERRORLEVEL% neq 0 (
    echo [ERROR] Fallo en la suite del Lexer.
    exit /b %ERRORLEVEL%
)

echo [2/8] Verificando Analizador Sintactico y AST (Parser)...
"%~dp0bin\test_parser.exe"
if %ERRORLEVEL% neq 0 (
    echo [ERROR] Fallo en la suite del Parser.
    exit /b %ERRORLEVEL%
)

echo [3/8] Verificando Motor Matematico Decimal Exacto (COMP-3 BCD)...
"%~dp0bin\test_comp3.exe"
if %ERRORLEVEL% neq 0 (
    echo [ERROR] Fallo en la suite COMP-3.
    exit /b %ERRORLEVEL%
)

echo [4/8] Verificando Preprocesador y Sanitizador de Codigo Fuente...
"%~dp0bin\upl_supervisor.exe" --test
if %ERRORLEVEL% neq 0 (
    echo [ERROR] Fallo en la suite del Preprocesador.
    exit /b %ERRORLEVEL%
)

echo [5/8] Verificando Compilacion Binaria End-to-End con uplc...
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

echo Verificando Ejecucion Determinista LIFO de posponer / defer...
"%~dp0bin\uplc.exe" "%~dp0tests\test_defer.upl" -o "%~dp0bin\test_defer.exe"
if %ERRORLEVEL% neq 0 (
    echo [ERROR] Fallo en la compilacion de test_defer.upl.
    exit /b %ERRORLEVEL%
)
"%~dp0bin\test_defer.exe"
if %ERRORLEVEL% neq 0 (
    echo [ERROR] Fallo en la ejecucion de test_defer.exe.
    exit /b %ERRORLEVEL%
)

if not exist "%~dp0bin\upl_pm.exe" gcc -O2 "%~dp0tools\upl_pm.c" -o "%~dp0bin\upl_pm.exe"
if not exist "%~dp0bin\test_all_modules.exe" gcc -O2 "%~dp0tests\test_all_modules.c" -o "%~dp0bin\test_all_modules.exe"

echo [6/8] Verificando Transpilador C99 a UPL+ (c2upl)...
"%~dp0bin\c2upl.exe" --test
if %ERRORLEVEL% neq 0 (
    echo [ERROR] Fallo en la suite del transpilador c2upl.
    exit /b %ERRORLEVEL%
)

echo Compilando modulos nativos UPL+ para pruebas de enlace dinamico...
"%~dp0bin\uplc.exe" "%~dp0modules\gxnox\gxnox_dual_parity.upl" -o "%~dp0bin\gxnox_dual_parity.dll"
"%~dp0bin\uplc.exe" "%~dp0modules\neurocobol\neurocobol_core.upl" -o "%~dp0bin\neurocobol_core.dll"
"%~dp0bin\uplc.exe" "%~dp0modules\neuro_gfx\neuro_gfx_core.upl" -o "%~dp0bin\neuro_gfx_core.dll"
"%~dp0bin\uplc.exe" "%~dp0modules\neuro_probe\probe_engine.upl" -o "%~dp0bin\probe_engine.dll"
"%~dp0bin\uplc.exe" "%~dp0modules\nuasm\nuasm_core.upl" -o "%~dp0bin\nuasm_core.dll"
"%~dp0bin\uplc.exe" "%~dp0modules\neurowill\neurowill_core.upl" -o "%~dp0bin\neurowill_core.dll"
"%~dp0bin\uplc.exe" "%~dp0modules\iagrok_core\iagrok_core.upl" -o "%~dp0bin\iagrok_core.dll"
"%~dp0bin\uplc.exe" "%~dp0dvtrgas.upl" -o "%~dp0bin\dvtrgas.dll"

echo [7/8] Verificando Modulos Nativos UPL (28/28 Pruebas Unitarias)...
"%~dp0bin\test_all_modules.exe"
if %ERRORLEVEL% neq 0 (
    echo [ERROR] Fallo en la suite de modulos nativos UPL+.
    exit /b %ERRORLEVEL%
)

echo [8/8] Verificando Caracteristicas UPL+ v2.3.0 Enums Cripto Silicio Canales MPSC Tensores IA Ring Slab Sync...
"%~dp0bin\uplc.exe" run "%~dp0tests\test_enums.upl"
if %ERRORLEVEL% neq 0 (
    echo [ERROR] Fallo en prueba de Enums bilingues.
    exit /b %ERRORLEVEL%
)
"%~dp0bin\uplc.exe" run "%~dp0tests\test_crypto.upl"
if %ERRORLEVEL% neq 0 (
    echo [ERROR] Fallo en prueba de Criptografia SHA-256 y Silicio.
    exit /b %ERRORLEVEL%
)
"%~dp0bin\uplc.exe" run "%~dp0tests\test_channel.upl"
if %ERRORLEVEL% neq 0 (
    echo [ERROR] Fallo en prueba de Canales Lock-Free MPSC.
    exit /b %ERRORLEVEL%
)
"%~dp0bin\uplc.exe" run "%~dp0tests\test_cripto_mpsc.upl"
if %ERRORLEVEL% neq 0 (
    echo [ERROR] Fallo en prueba de Estres Concurrente MPSC + SHA-256.
    exit /b %ERRORLEVEL%
)
"%~dp0bin\uplc.exe" run "%~dp0tests\test_tensor.upl"
if %ERRORLEVEL% neq 0 (
    echo [ERROR] Fallo en prueba de Tensores y GEMM de Silicio.
    exit /b %ERRORLEVEL%
)
"%~dp0bin\uplc.exe" run "%~dp0tests\test_v230_sovereign.upl"
if %ERRORLEVEL% neq 0 (
    echo [ERROR] Fallo en suite soberana v2.3.0 Ring Slab Sync.
    exit /b %ERRORLEVEL%
)
"%~dp0bin\uplc.exe" run "%~dp0tests\test_gh_bot_radar.upl"
if %ERRORLEVEL% neq 0 (
    echo [ERROR] Fallo en suite Anti-Bot Radar sin falsos positivos.
    exit /b %ERRORLEVEL%
)
"%~dp0bin\uplc.exe" run "%~dp0tests\test_calculator_polyglot.upl"
if %ERRORLEVEL% neq 0 (
    echo [ERROR] Fallo en prueba de Calculadora Poliglota Universal Multi-Lenguaje.
    exit /b %ERRORLEVEL%
)
"%~dp0bin\upl_pm.exe" verify
if %ERRORLEVEL% neq 0 (
    echo [ERROR] Fallo en verificacion de paquetes upl_pm.
    exit /b %ERRORLEVEL%
)

if exist "%~dp0extensions\instalar_extension.bat" (
    call "%~dp0extensions\instalar_extension.bat" --auto
)

echo ====================================================================
echo   TODAS LAS PRUEBAS (8/8) SE COMPLETARON CON EXITO (0 FALLOS)
echo ====================================================================
exit /b 0

