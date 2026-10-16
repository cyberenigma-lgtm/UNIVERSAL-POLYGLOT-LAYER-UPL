@echo off
setlocal

set "UPLC_EXE=%~dp0bin\uplc.exe"

if not exist "%UPLC_EXE%" (
    echo [UPLC] Compilando compilador nativo UPL+...
    if not exist "%~dp0bin" mkdir "%~dp0bin"
    gcc -O2 "%~dp0compiler\uplc.c" -o "%UPLC_EXE%" -lm
)

"%UPLC_EXE%" %*
