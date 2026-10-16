# Regla de Arquitectura y Desarrollo en UPL (Universal Polyglot Layer)

Esta regla rige el desarrollo, edición y verificación de archivos `.upl` y la toolchain nativa en este repositorio.

## 1. Principios de Ingeniería y Compilación
- **Toolchain Nativa Autónoma:** Todo el ecosistema compila mediante el ejecutable C99 nativo (`bin/uplc.exe` en Windows, `./bin/uplc` en POSIX). Cero dependencias externas de ejecución.
- **Sintaxis Bilingüe Equitativa:** El compilador reconoce palabras clave tanto en inglés como en español:
  - Declaraciones: `fn` / `funcion`, `let` / `sea`, `mut`, `const` / `constante`, `struct` / `estructura`, `module` / `modulo`, `import` / `importar`.
  - Control de flujo: `if` / `si`, `else` / `sino`, `while` / `mientras`, `for` / `para`, `return` / `retornar`.
  - Ensamblador: `asm` / `ensamblador`.
- **Decoradores de Sistemas:**
  - `@snapshot`: Activa la protección canaria en cabecera y cola de estructuras para mitigar corrupción de memoria.
  - `@baremetal`: Deshabilita dependencias de entorno hospedado y optimiza para silicio directo.

## 2. Sistema de Módulos
Los módulos estándar se importan con `import <nombre>;` o `importar <nombre>;`. UPLC los resuelve automáticamente desde:
- `modules/urep/urep_guard.upl` (CFI y canarios de pila)
- `modules/urep/urep_arch.upl` (Punteros multiarquitectura y segmentación)
- `modules/urep/urep_hw.upl` (MMIO, VGA, HPET y periféricos)
- `modules/multilang_asm/masm_table.upl` (Normalización de mnemónicos a x86_64)
- `modules/emulator/cpu_runner.upl` (Emulador y máquina de estados de registros)
- `modules/compute/vector_ops.upl` (Álgebra vectorial 4D)

## 3. Estándar de Comunicación
- Toda la documentación, comentarios y salidas de CLI deben utilizar terminología técnica estándar de ciencias de la computación. Evitar cualquier jerga esotérica.
