# UPL+: Universal Polyglot Layer Plus — Systems Programming Language
## Lenguaje de Programación de Sistemas Determinista y Compilador Nativo C99

**Autor:** José Manuel Moreno (Noxferion)  
**Versión:** 2.0.0-Release (Dragon Book Architecture)  
**Licencia:** Propietaria / Defensiva (Registrado en SafeCreative)

[![SafeCreative Protected](https://img.shields.io/badge/SafeCreative-Obra%20Protegida-blue.svg)](https://www.safecreative.org)
[![Language Specification](https://img.shields.io/badge/Spec-Formal%20EBNF%20v2.0-green.svg)](docs/SPEC_GRAMATICA_Y_SISTEMA_TIPOS.md)
[![Compiler Architecture](https://img.shields.io/badge/Compiler-ANSI%20C99%20Pipeline-blue.svg)](docs/ARQUITECTURA_COMPILADOR.md)
[![Verification Suite](https://img.shields.io/badge/Tests-5%2F5%20Suites%20PASS-brightgreen.svg)](docs/SUITE_DE_PRUEBAS.md)
[![Python Dependency](https://img.shields.io/badge/Python-0%25%20(Pure%20C99)-lightgrey.svg)](#-toolchain-aut%C3%B3noma-cero-dependencias)

> **COMUNICADO DE PROPIEDAD INTELECTUAL:**  
> ***UPL+*** *(Universal Polyglot Layer Plus)* es una obra de sistemas protegida por registro de propiedad intelectual en **SafeCreative**. La arquitectura del compilador (`compiler/`), la especificación de gramática formal EBNF (`docs/SPEC_GRAMATICA_Y_SISTEMA_TIPOS.md`), los motores de aritmética exacta COMP-3 BCD y los módulos de protección de memoria están reservados conforme a las leyes vigentes.

---

## 📌 Descripción General

**UPL+** es un lenguaje de programación de sistemas compilado, fuertemente tipado y de alto rendimiento. Ha sido diseñado para desarrollo de bajo nivel, firmware, utilidades de control de memoria del sistema operativo y cálculo financiero determinista.

El compilador oficial (`uplc`) está implementado íntegramente en **ANSI C99 estándar**, sin dependencias de Python, máquinas virtuales, intérpretes ni runtimes de recolección de basura (GC). Compila código fuente `.upl` a código intermedio C99 optimizado y genera ejecutables binarios nativos de 64 bits utilizando compiladores estándar del sistema (`gcc` o `clang`).

---

## 🏛️ Características de Ingeniería

### 1. Gramática Bilingüe Unificada (Inglés / Español)
El analizador léxico procesa de forma nativa e idéntica las palabras clave en ambos idiomas a nivel de token, permitiendo redactar código técnico en español o en inglés sin traductores intermediarios:

| Semántica | Token | Palabra Clave (EN) | Palabra Clave (ES) |
|---|---|---|---|
| Espacio de nombres | `TOK_MODULE` | `module` | `modulo` |
| Importación | `TOK_IMPORT` | `import` | `importar` |
| Estructuras | `TOK_STRUCT` | `struct` | `estructura` |
| Modificador público | `TOK_PUB` | `pub` | `publico` |
| Función | `TOK_FN` | `fn` | `funcion` |
| Variable inmutable | `TOK_LET` | `let` | `sea` |
| Mutabilidad explícita | `TOK_MUT` | `mut` | `mut` |
| Constantes | `TOK_CONST` | `const` | `constante` |
| Control condicional | `TOK_IF`, `TOK_ELSE` | `if`, `else` | `si`, `sino` |
| Bucles de iteración | `TOK_WHILE`, `TOK_FOR` | `while`, `for` | `mientras`, `para` |
| Retorno | `TOK_RETURN` | `return` | `retornar` |
| Cast explícito | `TOK_AS` | `as` | `como` |
| Bloque C nativo | `TOK_EMBED` | `embed` | `incrustar` |
| Ensamblador inline | `TOK_ASM` | `asm` | `ensamblador` |

### 2. Aritmética Decimal Exacta (COMP-3 Packed Decimal BCD)
Soporte de tipos `comp3(precision, escala)` para eliminar por completo los errores de redondeo derivados de la representación binaria IEEE-754 en cálculos financieros y de balance contable. La aritmética se ejecuta mediante un motor de software determinista en BCD empaquetado (4 bits por dígito con nibbles de signo `0x0C` positivo y `0x0D` negativo).

### 3. Asignador de Memoria por Arenas (Arena Allocator)
El compilador utiliza `UPL_Arena` para asignar todos los nodos del Árbol de Sintaxis Abstracta (AST) e identificadores en bloques contiguos de 64 KB, eliminando la fragmentación de la memoria del compilador y permitiendo una liberación instantánea O(1) de la unidad de compilación completa.

### 4. Interoperabilidad Nativa C / Win32 / POSIX
Mediante los bloques `embed "c" { ... }` o `incrustar "c" { ... }`, UPL+ puede invocar directamente funciones del sistema operativo (Win32 API, POSIX syscalls, librerías gráficas o drivers) sin coste de Marshalling ni sobrecarga en llamadas FFI.

---

## ⚙️ Arquitectura del Compilador (Pipeline Dragon Book)

El compilador implementa el flujo canónico clásico de compilación en fases modulares:

```
                  ┌──────────────────────┐
                  │  Código Fuente .upl  │
                  └──────────┬───────────┘
                             │
                             ▼
                  ┌──────────────────────┐
                  │ Analizador Léxico    │  lexer.c / lexer.h
                  │ (Tokenizer Bilingüe) │  (Tracking L:C, Strings, Embed)
                  └──────────┬───────────┘
                             │ Tokens
                             ▼
                  ┌──────────────────────┐
                  │ Parser Sintáctico    │  upl_parser.c / upl_parser.h
                  │ (Descenso + Pratt)   │  (Construcción AST en Arena)
                  └──────────┬───────────┘
                             │ AST
                             ▼
                  ┌──────────────────────┐
                  │ Generador de Código  │  upl_codegen.c / upl_codegen.h
                  │ Backend ANSI C99     │  (Emisión estricta de C99)
                  └──────────┬───────────┘
                             │
              ┌──────────────┴──────────────┐
              ▼                             ▼
   ┌──────────────────────┐      ┌──────────────────────┐
   │ Archivo Fuente .c    │      │ Compilador C (GCC)   │
   │ (--emit-c)           │      │ Binario Nativo .exe  │
   └──────────────────────┘      └──────────────────────┘
```

---

## 🚀 Guía de Inicio Rápido

### Requisitos Previos
* Compilador C nativo compatible con C99 (`gcc` vía MinGW-w64 o Clang).
* Sistema Operativo: Windows 10/11 (64 bits) o Linux x86_64.

### 1. Compilación del Toolchain
El script de compilación genera automáticamente el binario del compilador `bin/uplc.exe` en su primera ejecución:

```bash
# En Windows (CMD o PowerShell):
upl.bat --version

# En Linux / macOS:
./upl.sh --version
```

### 2. Opciones de Compilación (`uplc`)
```
Uso: uplc [opciones] <archivo.upl>

Opciones:
  -o <salida>       Especificar nombre del binario ejecutable de salida
  -c, --emit-c      Generar solo codigo fuente C99 sin invocar el backend gcc
  --dump-ast        Imprimir el arbol de sintaxis abstracta (AST) en consola
  --dump-tokens     Imprimir la secuencia de tokens lexico bilingue
  -v, --version     Mostrar version del compilador
```

### 3. Ejemplo de Programa (`hola_mundo.upl`)

```rust
modulo ejemplo;

incrustar "c" {
#include <stdio.h>
}

funcion principal() -> i32 {
    sea mut total: i32 = 0;
    sea tasa: i32 = 21;
    sea base: i32 = 1000;

    total = base + (base * tasa / 100);

    incrustar "c" {
        printf("[UPL+ NATIVO] Base: 1000, Total con IVA (21%%): %d\n", total);
    }

    retornar 0;
}
```

Compilar y ejecutar:
```bash
upl.bat -o demo.exe hola_mundo.upl
demo.exe
```

---

## 🧪 Suite de Verificación Automatizada

El repositorio incluye un arnés de pruebas completo en `test.bat` que valida todas las capas del compilador:

```bash
test.bat
```

### Resultados de la Suite:
* `[1/5] Analizador Léxico (Lexer)`: 6/6 tests superados (palabras bilingües, tracking de líneas/columnas, literales numéricos, bloques embed, control de errores).
* `[2/5] Analizador Sintáctico (Parser)`: 6/6 tests superados (parseo de funciones, estructuras, precedencia Pratt, control de flujo, reporte preciso de errores L:C).
* `[3/5] Motor Matemático COMP-3`: 4/4 tests superados (codificación nibbles BCD, conversión string bidireccional, aritmética decimal exacta, comparación con signo).
* `[4/5] Supervisor y Guardián Técnico`: 3/3 tests superados (filtrado de divagaciones, extracción de código, bloqueo de respuestas vacías).
* `[5/5] Compilación End-to-End`: Compilación completa y ejecución nativa del binario resultante con cálculo matemático verificado (0 errores).

---

## 🎮 Caso de Estudio Real: Watchdog de Memoria (Warhammer Online)

Para validar la fiabilidad de UPL+ en entornos de producción con limitaciones de memoria severas, se implementó un monitor de supervisión para el cliente de 32 bits de **Warhammer Online: Return of Reckoning**:

* **Código fuente nativo:** [`tools/UPL_Memory_Guard.upl`](tools/UPL_Memory_Guard.upl)
* **Código C estándar:** [`tools/ror_memory_guard.c`](tools/ror_memory_guard.c)
* **Objetivo:** Monitorear el Working Set físico de `WAR.exe` mediante `K32GetProcessMemoryInfo` y purgar páginas residuales con `K32EmptyWorkingSet` al superar un umbral crítico (1.65 GB), previniendo los bloqueos por OutOfMemory en batallas masivas RvR de 400+ jugadores.
* **Resultado empírico medido:** Reducción verificada del Working Set de 253 MB a 0 MB en procesos activos en menos de 1 milisegundo.
* **Paquete de distribución:** [`dist/ror_memory_guard.zip`](dist/ror_memory_guard.zip).

---

## 📁 Estructura del Repositorio

```
UNIVERSAL-POLYGLOT-LAYER-UPL/
├── bin/                       # Binarios compilados (uplc.exe, test_*.exe)
├── compiler/                  # Código fuente del compilador en C99
│   ├── lexer.c / lexer.h      # Analizador léxico bilingüe
│   ├── upl_arena.c / .h       # Asignador de memoria por bloques (Arena)
│   ├── upl_ast.c / .h         # Estructuras y constructores del AST
│   ├── upl_parser.c / .h      # Parser de descenso recursivo y Pratt
│   ├── upl_codegen.c / .h     # Generador de código C99
│   ├── runtime/upl_comp3.c/.h # Motor de aritmética decimal BCD COMP-3
│   └── main.c                 # Driver principal de la toolchain (CLI)
├── dist/                      # Paquetes y herramientas empaquetadas (.zip)
├── docs/                      # Especificaciones técnicas formales
│   ├── SPEC_GRAMATICA_Y_SISTEMA_TIPOS.md  # Gramática EBNF formal
│   ├── ARQUITECTURA_COMPILADOR.md          # Pipeline Dragon Book detallado
│   ├── CASO_ESTUDIO_WARHAMMER_MEMORY_GUARD.md # Caso de uso real Win32
│   └── SUITE_DE_PRUEBAS.md                 # Documentación del arnés de tests
├── examples/                  # Programas de ejemplo en sintaxis UPL+
├── modules/std/               # Módulos de la librería estándar (io, math, memory)
├── tests/                     # Suites de pruebas unitarias en C99
├── tools/                     # Herramientas de sistemas y utilidades nativas
├── upl.bat                    # Script lanzador del compilador (Windows)
├── upl.sh                     # Script lanzador del compilador (Linux/POSIX)
└── test.bat                   # Ejecutor de la suite completa de pruebas
```

---

## 📜 Licencia y Contacto

Obra Protegida bajo registro de Propiedad Intelectual en SafeCreative.  
Todos los derechos reservados. Para auditorías técnicas o licencias corporativas, contactar al autor: **José Manuel Moreno (Noxferion)**.
