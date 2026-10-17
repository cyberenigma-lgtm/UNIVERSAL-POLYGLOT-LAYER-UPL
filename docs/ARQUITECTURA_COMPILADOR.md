# Arquitectura Interna del Compilador UPL+ (v2.0)
## Implementación en ANSI C99 del Pipeline Canónico (Dragon Book)

**Autor:** José Manuel Moreno (Noxferion)  
**Estado:** Especificación Técnica de Ingeniería  
**Lenguaje de Implementación:** ANSI C99 Estándar (ISO/IEC 9899:1999)  
**Dependencias Externas:** Ninguna (0% Python, Cero VMs, Cero runtimes externos)

---

## 1. Visión General del Pipeline

El compilador oficial de UPL+ (`compiler/`) transforma código fuente `.upl` en ejecutables binarios nativos a través de un flujo estructurado de siete etapas:

```
[Código Fuente .upl]
         │
         ▼  (Fase 1: Lexer)
[Flujo de Tokens Tipados]
         │
         ▼  (Fase 2: Memory Arena + Fase 3 & 4: Parser Pratt / AST)
[Árbol de Sintaxis Abstracta (AST)]
         │
         ▼  (Fase 5: Motor Aritmético COMP-3 BCD & Semántica)
[AST Validado y Tipado]
         │
         ▼  (Fase 6: CodeGen C99)
[Código Fuente ANSI C99 (.c)]
         │
         ▼  (Fase 7: Backend Linker GCC/Clang)
[Binario Nativo Ejecutable (.exe / ELF)]
```

---

## 2. Componentes del Compilador

### 2.1 Analizador Léxico Bilingüe (`compiler/lexer.c`, `compiler/lexer.h`)
* **Propósito:** Convierte el flujo de caracteres UTF-8 en una secuencia estructurada de tokens discretos (`Token`).
* **Unificación de Idioma:** Mapea directamente palabras clave en inglés y español al mismo identificador de token (`TokenType`), eliminando cualquier necesidad de preprocesado o traducción intermedia:
  - `fn` / `funcion` ➔ `TOK_FN`
  - `let` / `sea` ➔ `TOK_LET`
  - `mut` / `mut` ➔ `TOK_MUT`
  - `struct` / `estructura` ➔ `TOK_STRUCT`
  - `if` / `si` ➔ `TOK_IF`
  - `else` / `sino` ➔ `TOK_ELSE`
  - `while` / `mientras` ➔ `TOK_WHILE`
  - `return` / `retornar` ➔ `TOK_RETURN`
  - `embed` / `incrustar` ➔ `TOK_EMBED`
* **Bloques Crudos de Código (`embed`):** El analizador léxico cuenta con un modo de captura cruda que extrae íntegramente el cuerpo entre llaves `{ ... }` tras la directiva `embed "c"` sin tokenizar su interior, preservando directivas de preprocesador `#include`, macros y llamadas nativas de la API de C intactas.
* **Diagnósticos Precisos:** Mantiene contadores exactos de número de línea (`line`) y columna (`column`) para el reporte de errores en tiempo de compilación.

### 2.2 Asignador de Memoria por Arenas (`compiler/upl_arena.c`, `compiler/upl_arena.h`)
* **Propósito:** Gestión determinista y de alto rendimiento de la memoria del compilador.
* **Estrategia:** Reserva bloques contiguos de 64 KB (`UPL_ARENA_BLOCK_SIZE`). Cuando una reserva supera el bloque actual, se encadena un nuevo bloque en una lista enlazada simple.
* **Ventajas frente a `malloc`/`free` tradicional:**
  - Cero fragmentación de memoria en el heap durante el parseo de programas grandes.
  - Asignación en tiempo $O(1)$ mediante avance de puntero (`offset += size`).
  - Liberación instantánea $O(1)$ de todos los nodos del AST e identificadores (`upl_arena_free`) al finalizar la unidad de traducción.

### 2.3 Estructuras del Árbol de Sintaxis Abstracta (`compiler/upl_ast.c`, `compiler/upl_ast.h`)
* **Propósito:** Representación en memoria de la estructura gramatical del programa.
* **Tipos de Nodos:**
  - `UplAstNode`: Nodo polimórfico discriminado por `UplAstNodeType`.
  - **Declaraciones:** Módulos, imports, funciones (`UplFnDecl`), estructuras (`UplStructDecl`), variables (`UplVarDecl`).
  - **Sentencias:** Bloques de código (`UplBlockStmt`), bifurcaciones `if/else` (`UplIfStmt`), bucles `while` (`UplWhileStmt`), retornos (`UplReturnStmt`), expresiones aisladas y bloques `embed` nativos.
  - **Expresiones:** Operaciones binarias con precedencia (`UplBinaryExpr`), llamadas a función (`UplCallExpr`), asignaciones (`UplAssignExpr`), literales enteros, flotantes, cadenas y booleanos.
* **Constructores en Arena:** Cada nodo se instancia mediante funciones dedicadas (`upl_ast_create_*`) que reservan la memoria directamente en la arena.

### 2.4 Analizador Sintáctico y Pratt Parsing (`compiler/upl_parser.c`, `compiler/upl_parser.h`)
* **Propósito:** Construye el AST validando la gramática formal EBNF.
* **Descenso Recursivo:** Utilizado para la estructura de alto nivel del programa (declaraciones de funciones, parámetros, estructuras y sentencias de control).
* **Pratt Parsing (Precedence Climbing):** Utilizado para el análisis de expresiones aritméticas y lógicas, garantizando una jerarquía estricta de precedencia sin ambigüedades:
  ```
  Nivel 1 (Mayor): Agrupación (), Llamadas f(), Literales
  Nivel 2: Multiplicación (*), División (/), Módulo (%)
  Nivel 3: Suma (+), Resta (-)
  Nivel 4: Comparaciones relacionales (<, <=, >, >=)
  Nivel 5: Comparaciones de igualdad (==, !=)
  Nivel 6: Conjunción lógica (&&)
  Nivel 7: Disyunción lógica (||)
  Nivel 8 (Menor): Asignación (=)
  ```
* **Manejo de Errores y Sincronización:** Si se produce un error sintáctico, el parser registra el mensaje de error con su ubicación exacta (Línea:Columna) y activa el modo de pánico (`panic_mode`), sincronizándose en el siguiente punto y coma o palabra clave estructural para evitar cascadas de falsos errores.

### 2.5 Motor de Precisión Decimal COMP-3 BCD (`compiler/runtime/upl_comp3.c`, `compiler/runtime/upl_comp3.h`)
* **Propósito:** Garantizar precisión matemática absoluta en cálculos financieros, contables o científicos.
* **Codificación:** Utiliza BCD empaquetado (*Packed Decimal*) conforme al estándar industrial de la computación financiera:
  - Cada byte almacena 2 dígitos decimales (4 bits / nibble por dígito).
  - El último nibble almacena el signo algebraico: `0x0C` (positivo `+`), `0x0D` (negativo `-`), `0x0F` (sin signo).
* **Operaciones por Software:** Incluye rutinas para suma, resta, multiplicación, redondeo por truncamiento o medio-arriba, y conversión bidireccional entre representaciones en cadena ASCII y buffers binarios BCD.

### 2.6 Generador de Código C99 (`compiler/upl_codegen.c`, `compiler/upl_codegen.h`)
* **Propósito:** Emite código fuente ANSI C99 limpio, legible y conforme con las especificaciones ISO/IEC.
* **Estrategia:**
  - Emite encabezados estándar e inyecciones de soporte (`upl_comp3.h` si el programa usa aritmética decimal).
  - Emite definiciones de estructuras y prototipos de funciones.
  - Traduce nodos del AST a expresiones e instrucciones equivalentes en C99.
  - Emite los bloques `embed` nativos en la posición exacta definida por el programador.
  - Genera la función de punto de entrada `main` invocando la función principal del módulo.

### 2.7 Driver de la Toolchain (`compiler/main.c`)
* **Propósito:** Interfaz de línea de comandos para orquestar la compilación.
* **Parámetros Soportados:**
  - `-o <salida>`: Nombre del ejecutable binario final.
  - `-c`, `--emit-c`: Genera únicamente el código C99 intermedio sin invocar el compilador del sistema.
  - `--dump-ast`: Muestra el árbol sintáctico estructurado en consola para depuración.
  - `--dump-tokens`: Muestra el flujo léxico generado por el lexer.
  - `-v`, `--version`: Muestra la versión oficial de la toolchain.

---

## 3. Integración con el Sistema Operativo

El compilador interactúa con el entorno mediante dos scripts nativos que compilan el driver si este no existe en `bin/`:
* En Windows: [`upl.bat`](../upl.bat) (invoca `gcc.exe` de MinGW-w64).
* En Linux / macOS: [`upl.sh`](../upl.sh) (invoca `gcc` o `clang`).

Cero dependencias de Python, Node.js ni herramientas de compilación externas como CMake o Make para el ciclo básico de desarrollo.
