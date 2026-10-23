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
  - `struct` -> Genera una estructura con tipado estático.
  - `net_tcp` -> Plantilla para cliente y conexión de socket TCP nativo.
  - `mutex` -> Bloqueo y sincronización de hilos en hardware.
  - `comp3` -> Aritmética decimal financiera exacta (Packed Decimal BCD).
- ⚙️ **Integración de Tareas de Compilación (`tasks.json`):**
  - `Ctrl + Shift + B` -> Compilar binario nativo optimizado con `bin/uplc.exe`.
  - Soporte integrado para `uplc pm list` (Gestor de módulos de silicio).

## Instalación en Visual Studio Code / Antigravity IDE

### Opción 1: Instalación directa desde archivo VSIX
Ejecuta en tu terminal:
```bash
antigravity-ide --install-extension extensions/upl-lang-1.6.0.vsix
# O en Visual Studio Code:
code --install-extension extensions/upl-lang-1.6.0.vsix
```

### Opción 2: Script de 1 clic (Windows)
Haz doble clic en:
```
extensions/instalar_extension.bat
```
