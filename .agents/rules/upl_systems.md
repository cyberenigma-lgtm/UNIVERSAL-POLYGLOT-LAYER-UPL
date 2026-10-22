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

## 2. Sistema de Módulos y Biblioteca Estándar
Los módulos estándar se encuentran en `modules/` y se importan con `import <nombre>;` o `importar <nombre>;`:
- `modules/std/fs.upl` (Sistema de archivos soberano: `fs_abrir`, `fs_guardar`, `fs_cambiar_nombre`, `fs_cambiar_extension`, `fs_crear_carpeta`, `fs_borrar`)
- `modules/std/dict.upl` (Diccionarios y tablas hash O(1) con hash FNV-1a)
- `modules/std/memory.upl` (Operaciones de memoria y canarios de seguridad 0xDEADBEEF)
- `modules/std/math.upl` (Matemáticas de punto fijo, vectores y trigonometría)
- `modules/std/io.upl` (Consola, logs estructurados y redireccionamiento de salida)
- `modules/neurowill/neurowill_core.upl` (Máquina virtual neural y ejecución de bytecode)
- `modules/nuasm/nuasm_core.upl` (JIT y generador de opcodes x86_64 a memoria)
- `modules/urep/` (CFI, segmentación, MMIO y hardware directo)

## 3. Comandos de Compilación y Ejecución
- Compilar: `bin/uplc.exe build <archivo.upl> -o <salida.exe>`
- Compilar y Ejecutar en 1 paso: `bin/uplc.exe run <archivo.upl>`
- Enlazar librerías externas: `bin/uplc.exe <archivo.upl> -lwinmm -lgdi32 -luser32 -o <salida.exe>`
- Transpilar a C99 estándar: `bin/uplc.exe -t c <archivo.upl> -o <salida.c>`
- Suite de pruebas de regresión: `test.bat` (Windows) o `./test.sh` (Linux)

## 4. Estándar de Comunicación y Calidad
- Toda la documentación, comentarios y salidas de CLI deben utilizar terminología técnica estándar de ciencias de la computación. Evitar cualquier jerga esotérica.
- Cada módulo nuevo debe incluir sus pruebas unitarias en `tests/` y mantener el 100% de éxito en la suite de verificación.
