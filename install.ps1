# UPL+ v2.4.0 One-Line Installer (Windows PowerShell)
# Usage: irm https://raw.githubusercontent.com/cyberenigma-lgtm/UNIVERSAL-POLYGLOT-LAYER-UPL/main/install.ps1 | iex

$ErrorActionPreference = "Stop"
$InstallDir = if ($env:UPL_INSTALL_DIR) { $env:UPL_INSTALL_DIR } else { Join-Path $env:LOCALAPPDATA "UPL\bin" }

Write-Host "===================================================================="
Write-Host "  UPL+ Sovereign Toolchain Installer (Windows x64)"
Write-Host "===================================================================="

New-Item -ItemType Directory -Force -Path $InstallDir | Out-Null

if ((Test-Path "compiler\main.c") -and (Test-Path "tools\upl_pm.c")) {
    Write-Host "[*] Local UPL+ source tree detected. Compiling native C99 binaries..."
    New-Item -ItemType Directory -Force -Path "bin" | Out-Null
    & gcc -O2 compiler/upl_arena.c compiler/lexer.c compiler/upl_ast.c compiler/upl_parser.c compiler/upl_codegen.c compiler/runtime/upl_comp3.c compiler/main.c -I compiler -I compiler/runtime -o bin/uplc.exe
    & gcc -O2 tools/upl_pm.c -o bin/upl_pm.exe
    & gcc -O2 tools/upl_mcp_server.c -o bin/upl_mcp.exe
    & gcc -std=c99 -O2 tools/c2upl.c -o bin/c2upl.exe
    Copy-Item "bin\uplc.exe", "bin\upl_pm.exe", "bin\upl_mcp.exe", "bin\c2upl.exe" -Destination $InstallDir -Force
} else {
    $TmpDir = Join-Path $env:TEMP ("upl_install_" + [Guid]::NewGuid().ToString("N"))
    New-Item -ItemType Directory -Force -Path $TmpDir | Out-Null
    try {
        Write-Host "[*] Cloning UPL+ repository..."
        git clone --depth 1 "https://github.com/cyberenigma-lgtm/UNIVERSAL-POLYGLOT-LAYER-UPL.git" "$TmpDir\upl"
        Push-Location "$TmpDir\upl"
        New-Item -ItemType Directory -Force -Path "bin" | Out-Null
        & gcc -O2 compiler/upl_arena.c compiler/lexer.c compiler/upl_ast.c compiler/upl_parser.c compiler/upl_codegen.c compiler/runtime/upl_comp3.c compiler/main.c -I compiler -I compiler/runtime -o bin/uplc.exe
        & gcc -O2 tools/upl_pm.c -o bin/upl_pm.exe
        & gcc -O2 tools/upl_mcp_server.c -o bin/upl_mcp.exe
        & gcc -std=c99 -O2 tools/c2upl.c -o bin/c2upl.exe
        Copy-Item "bin\uplc.exe", "bin\upl_pm.exe", "bin\upl_mcp.exe", "bin\c2upl.exe" -Destination $InstallDir -Force
        Pop-Location
    } finally {
        Remove-Item -Recurse -Force -ErrorAction SilentlyContinue $TmpDir
    }
}

$UserPath = [Environment]::GetEnvironmentVariable("Path", "User")
if ($UserPath -notlike "*$InstallDir*") {
    [Environment]::SetEnvironmentVariable("Path", "$UserPath;$InstallDir", "User")
    Write-Host "[OK] Added $InstallDir to User PATH (restart terminal to apply)."
} else {
    Write-Host "[OK] $InstallDir is already in User PATH."
}
Write-Host "[OK] UPL+ binaries installed in: $InstallDir"

