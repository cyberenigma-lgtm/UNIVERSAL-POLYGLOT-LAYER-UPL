@echo off
echo ====================================================================
echo  INSTALADOR DE EXTENSION UPL+ PARA EDITORES E IDEs
echo ====================================================================
echo.

set EXT_SRC=%~dp0upl-vscode
set VSCODE_EXT_DIR=%USERPROFILE%\.vscode\extensions\upl-lang-1.5.0
set ANTIGRAVITY_EXT_DIR=%USERPROFILE%\.antigravity\extensions\upl-lang-1.5.0

echo [1/3] Instalando extension para VS Code / Cursor / VSCodium...
if exist "%USERPROFILE%\.vscode\extensions" (
    xcopy /E /I /Y "%EXT_SRC%" "%VSCODE_EXT_DIR%" >nul
    echo   [OK] Extension UPL instalada exitosamente en VS Code.
) else (
    echo   [OMITIDO] No se detecto directorio .vscode/extensions.
)

echo.
echo [2/3] Instalando extension para Antigravity IDE...
if exist "%USERPROFILE%\.antigravity\extensions" (
    xcopy /E /I /Y "%EXT_SRC%" "%ANTIGRAVITY_EXT_DIR%" >nul
    echo   [OK] Extension UPL instalada exitosamente en Antigravity.
) else (
    echo   [INFO] Creando directorio y copiando a Antigravity...
    mkdir "%ANTIGRAVITY_EXT_DIR%" 2>nul
    xcopy /E /I /Y "%EXT_SRC%" "%ANTIGRAVITY_EXT_DIR%" >nul
    echo   [OK] Extension UPL instalada en Antigravity.
)

echo.
echo [3/3] Soporte para Notepad++:
echo   El archivo de definicion de lenguaje se encuentra en:
echo   %~dp0notepadpp\upl_notepadpp.xml
echo   Para activarlo: Abre Notepad++ -> Idioma -> Definir idioma -> Importar -> Selecciona upl_notepadpp.xml
echo.
echo ====================================================================
echo  INSTALACION COMPLETADA CON EXITO.
echo  Reinicia tu editor para disfrutar de resaltado de sintaxis en .upl
echo ====================================================================
pause
