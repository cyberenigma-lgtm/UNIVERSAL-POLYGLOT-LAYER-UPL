# Extensión Oficial UPL (Universal Polyglot Layer) para VS Code y Antigravity IDE

Extensión de soporte de lenguaje para **Universal Polyglot Layer (UPL)**. Compatible con **Visual Studio Code**, **VSCodium** y **Antigravity IDE**.

## Características

- 🎨 **Coloreado de Sintaxis Completo:**
  - Soporte de sintaxis bilingüe (palabras clave en Inglés y Español: `fn`/`funcion`, `let`/`sea`, `import`/`importar`, `while`/`mientras`).
  - Resaltado de decoradores de sistemas: `@snapshot`, `@baremetal`, `@pure`, `@hosted`.
  - Tipos nativos de silicio: `i8`, `u8`, `i16`, `u16`, `i32`, `u32`, `i64`, `u64`, `f32`, `f64`, `bool`, `str`, `comp3`.
  - Bloques de ensamblador inline: `asm { ... }`.
- ⚡ **Snippets de Código:**
  - `main` -> Genera la función principal de entrada.
  - `fn` -> Genera una función tipada.
  - `struct` -> Genera una estructura protegida `@snapshot`.
  - `imp` -> Importa módulos del ecosistema.
- ⚙️ **Integración de Tareas de Compilación (`tasks.json`):**
  - `Ctrl + Shift + B` -> Compilar binario nativo optimizado con `bin/uplc.exe`.
  - Tareas para empaquetado directo a `.deb`, `.rpm`, `.app` (macOS) y `.apk` (Android).
  - Tarea para listar módulos disponibles en el sistema.

## Instalación en Visual Studio Code / Antigravity IDE

### Opción 1: Enlace directo en carpeta de extensiones (Recomendada para desarrollo)
Copia o crea un enlace simbólico de la carpeta `extensions/upl-vscode` a la carpeta de extensiones de tu editor:

- **VS Code:** `%USERPROFILE%\.vscode\extensions\upl-lang`
- **Antigravity IDE:** `%USERPROFILE%\.gemini\antigravity-ide\extensions\upl-lang`

### Opción 2: Empaquetar con vsce
```bash
cd extensions/upl-vscode
npx @vscode/vsce package
code --install-extension upl-lang-1.5.0.vsix
```
