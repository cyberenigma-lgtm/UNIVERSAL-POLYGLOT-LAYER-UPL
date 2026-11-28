# UPL+ Compiler Internal Architecture (v2.3.0)

> **Language / Idioma:** 🇬🇧 English | [🇪🇸 Español](../ARQUITECTURA_COMPILADOR.md) | [🇨🇳 中文](../zh/COMPILER_ARCHITECTURE.md) | [🇫🇷 Français](../fr/COMPILER_ARCHITECTURE.md) | [🇵🇹 Português](../pt/COMPILER_ARCHITECTURE.md)

**Author:** José Manuel Moreno (Noxferion)  
**Status:** Technical Engineering Specification  
**Implementation Language:** ANSI C99 Standard (ISO/IEC 9899:1999)  
**External Dependencies:** None (0% Python, Zero VMs, Zero external runtimes)

---

## 1. Pipeline Overview

The UPL+ compiler (`compiler/`) transforms `.upl` source code into native binary executables through a structured seven-stage pipeline:

```
[.upl Source Code]
        │
        ▼  (Phase 1: Lexer)
[Typed Token Stream]
        │
        ▼  (Phase 2: Memory Arena + Phase 3 & 4: Pratt Parser / AST)
[Abstract Syntax Tree (AST)]
        │
        ▼  (Phase 5: COMP-3 BCD Arithmetic Engine & Semantics)
[Validated and Typed AST]
        │
        ▼  (Phase 6: C99 CodeGen)
[ANSI C99 Source (.c)]
        │
        ▼  (Phase 7: Backend Linker GCC/Clang)
[Native Executable Binary (.exe / ELF)]
```

---

## 2. Compiler Components

### 2.1 Bilingual Lexical Analyzer (`compiler/lexer.c`, `compiler/lexer.h`)

- **Purpose:** Converts the UTF-8 character stream into a structured sequence of discrete tokens (`Token`).
- **Language Unification:** Directly maps English and Spanish keywords to the same token identifier (`TokenType`), eliminating any need for preprocessing or intermediate translation:
  - `fn` / `funcion` → `TOK_FN`
  - `let` / `sea` → `TOK_LET`
  - `mut` / `mut` → `TOK_MUT`
  - `struct` / `estructura` → `TOK_STRUCT`
  - `if` / `si` → `TOK_IF`
  - `else` / `sino` → `TOK_ELSE`
  - `while` / `mientras` → `TOK_WHILE`
  - `return` / `retornar` → `TOK_RETURN`
  - `embed` / `incrustar` → `TOK_EMBED`
- **Raw Code Blocks (`embed`):** The lexer has a raw-capture mode that extracts the entire body between braces `{ ... }` after the `embed "c"` directive without tokenizing its interior, preserving `#include` preprocessor directives, macros, and native C API calls intact.
- **Precise Diagnostics:** Maintains exact line (`line`) and column (`column`) counters for compile-time error reporting.

### 2.2 Arena Memory Allocator (`compiler/upl_arena.c`, `compiler/upl_arena.h`)

- **Purpose:** Deterministic, high-performance memory management for the compiler.
- **Strategy:** Reserves contiguous 64 KB blocks (`UPL_ARENA_BLOCK_SIZE`). When an allocation exceeds the current block, a new block is chained in a singly-linked list.
- **Advantages over traditional `malloc`/`free`:**
  - Zero heap fragmentation during parsing of large programs.
  - O(1) allocation via pointer bumping (`offset += size`).
  - O(1) instantaneous release of all AST nodes and identifiers (`upl_arena_free`) at end of translation unit.

### 2.3 Abstract Syntax Tree Structures (`compiler/upl_ast.c`, `compiler/upl_ast.h`)

- **Purpose:** In-memory representation of the program's grammatical structure.
- **Node Types:**
  - `UplAstNode`: Polymorphic node discriminated by `UplAstNodeType`.
  - **Declarations:** Modules, imports, functions (`UplFnDecl`), structures (`UplStructDecl`), variables (`UplVarDecl`).
  - **Statements:** Code blocks (`UplBlockStmt`), `if/else` branches (`UplIfStmt`), `while` loops (`UplWhileStmt`), returns (`UplReturnStmt`), isolated expressions, and native `embed` blocks.
  - **Expressions:** Binary operations with precedence (`UplBinaryExpr`), function calls (`UplCallExpr`), assignments (`UplAssignExpr`), integer/float/string/boolean literals.
- **Arena Constructors:** Each node is instantiated via dedicated functions (`upl_ast_create_*`) that allocate memory directly from the arena.

### 2.4 Syntactic Analyzer and Pratt Parsing (`compiler/upl_parser.c`, `compiler/upl_parser.h`)

- **Purpose:** Builds the AST while validating the formal EBNF grammar.
- **Recursive Descent:** Used for the high-level program structure (function declarations, parameters, structures, and control statements).
- **Pratt Parsing (Precedence Climbing):** Used for arithmetic and logical expression analysis, ensuring strict, unambiguous precedence hierarchy:
  ```
  Level 1 (Highest): Grouping (), Function calls f(), Literals
  Level 2: Multiplication (*), Division (/), Modulo (%)
  Level 3: Addition (+), Subtraction (-)
  Level 4: Relational comparisons (<, <=, >, >=)
  Level 5: Equality comparisons (==, !=)
  Level 6: Logical conjunction (&&)
  Level 7: Logical disjunction (||)
  Level 8 (Lowest): Assignment (=)
  ```
- **Error Handling and Synchronization:** On a syntax error, the parser records the message with its exact location (Line:Column) and activates panic mode (`panic_mode`), synchronizing on the next semicolon or structural keyword to avoid cascading false errors.

### 2.5 COMP-3 BCD Decimal Precision Engine (`compiler/runtime/upl_comp3.c`, `compiler/runtime/upl_comp3.h`)

- **Purpose:** Guarantee absolute mathematical precision in financial, accounting, or scientific calculations.
- **Encoding:** Uses Packed Decimal (BCD) conforming to the financial computing industry standard:
  - Each byte stores 2 decimal digits (4 bits/nibble per digit).
  - The last nibble stores the algebraic sign: `0x0C` (positive `+`), `0x0D` (negative `-`), `0x0F` (unsigned).
- **Software Operations:** Includes routines for addition, subtraction, multiplication, truncation/half-up rounding, and bidirectional conversion between ASCII string representations and binary BCD buffers.

### 2.6 C99 Code Generator (`compiler/upl_codegen.c`, `compiler/upl_codegen.h`)

- **Purpose:** Emit clean, readable, ISO/IEC-compliant ANSI C99 source code.
- **Strategy:**
  - Emits standard headers and support injections (`upl_comp3.h` if the program uses decimal arithmetic).
  - Emits structure definitions and function prototypes.
  - Translates AST nodes to equivalent C99 expressions and instructions.
  - Emits `embed` native blocks at the exact position defined by the programmer.
  - Generates the `main` entry point function invoking the module's primary function.

### 2.7 Toolchain Driver (`compiler/main.c`)

- **Purpose:** Command-line interface to orchestrate compilation.
- **Supported Parameters:**
  - `-o <output>`: Name of the final binary executable.
  - `-c`, `--emit-c`: Generate only the intermediate C99 code without invoking the system compiler.
  - `--dump-ast`: Print the structured syntax tree to the console for debugging.
  - `--dump-tokens`: Print the token stream generated by the lexer.
  - `-v`, `--version`: Show the official toolchain version.

---

## 3. Operating System Integration

The compiler interacts with the environment through two native scripts that compile the driver if it does not exist in `bin/`:

- On Windows: [`upl.bat`](../../upl.bat) (invokes `gcc.exe` from MinGW-w64)
- On Linux / macOS: [`upl.sh`](../../upl.sh) (invokes `gcc` or `clang`)

Zero dependencies on Python, Node.js, or external build tools such as CMake or Make for the basic development cycle.

---

## 4. Supported Compilation Targets (v2.4.0 Preview)

| Target Flag | Output | Description |
|-------------|--------|-------------|
| *(default)* | Native binary (.exe / ELF) | Via GCC/Clang C99 backend |
| `--target wasm32` | `.wasm` module | WebAssembly via Emscripten |
| `--target asm-multilang` | Polyglot assembly | MultiLang-ASM 57-language backend |
| `--emit-c` | `.c` file | Intermediate C99 source |

---

## 5. See Also

- [Quickstart Guide](QUICKSTART.md)
- [Syntax and Grammar Specification](SYNTAX_AND_GRAMMAR.md)
- [MCP Server Documentation](../MCP_SERVER.md)
- [Master Documentation Index](../INDEX.md)
