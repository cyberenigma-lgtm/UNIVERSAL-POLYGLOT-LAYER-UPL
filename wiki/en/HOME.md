# 🚀 UPL+ PROGRAMMING LANGUAGE — TECHNICAL QUICKSTART
## Deterministic Systems Programming Language & Standalone C99 Native Toolchain

Welcome to the technical documentation of **UPL+ (Universal Polyglot Layer Plus)**. UPL+ is a **compiled, strongly typed systems programming language** designed for low-level application development, memory watchdog utilities, and deterministic fixed-point financial arithmetic.

---

### 🌟 Core Architectural Features

1. **Standalone Native Compiler (`uplc`):** Implemented in ANSI C99 with modular Dragon Book architecture (bilingual lexer, memory arena allocator, typed AST, Pratt recursive-descent parser, and C99 code generator).
2. **First-Class Bilingual Grammar:** Full keyword parity between English (`fn`, `let`, `mut`, `struct`, `if`, `while`, `return`) and Spanish (`funcion`, `sea`, `mut`, `estructura`, `si`, `mientras`, `retornar`) at the lexer level.
3. **Exact Decimal Arithmetic (`comp3`):** Native BCD packed decimal engine that eliminates IEEE-754 binary floating-point drift in accounting and financial operations.
4. **Zero-Overhead Host Interoperability (`embed`):** Direct injection of native C99, Win32 API, or POSIX system calls with zero marshalling overhead.
5. **Memory Arena Allocator:** Monotonic block allocator ensuring $O(1)$ allocation and instant compilation-unit deallocation without heap fragmentation.

---

### 📦 Sample Program (`hello.upl`)

```rust
module demo;

embed "c" {
#include <stdio.h>
}

pub fn main() -> i32 {
    let mut total: i32 = 0;
    let base: i32 = 1000;
    let tax_rate: i32 = 21;

    total = base + (base * tax_rate / 100);

    embed "c" {
        printf("[UPL+ NATIVE] Base: 1000, Total with Tax (21%%): %d\n", total);
    }

    return 0;
}
```

---

### ⚡ Compilation & Execution

From the repository root:

```bash
# 1. Compile directly to a native binary (Windows: upl.bat / POSIX: ./upl.sh):
upl.bat -o hello.exe hello.upl
hello.exe

# 2. Emit ANSI C99 source code without invoking the host compiler:
upl.bat -c hello.upl

# 3. Inspect Abstract Syntax Tree (AST):
upl.bat --dump-ast hello.upl

# 4. Run the comprehensive automated verification suite:
test.bat
```

---

### 📚 Technical Documentation
* **[C99 to UPL+ Native Transpiler Guide (`c2upl`)](C2UPL_TRANSPILER.md)**
* **[Formal Grammar & Type Specification](file:///c:/Users/cyber/OneDrive/Documentos/NeuroOs/UNIVERSAL-POLYGLOT-LAYER-UPL/docs/SPEC_GRAMATICA_Y_SISTEMA_TIPOS.md)**
* **[Compiler Pipeline Architecture](file:///c:/Users/cyber/OneDrive/Documentos/NeuroOs/UNIVERSAL-POLYGLOT-LAYER-UPL/docs/ARQUITECTURA_COMPILADOR.md)**
* **[Verification Test Suite](file:///c:/Users/cyber/OneDrive/Documentos/NeuroOs/UNIVERSAL-POLYGLOT-LAYER-UPL/docs/SUITE_DE_PRUEBAS.md)**
* **[Real-World Case Study: Warhammer Memory Guard](file:///c:/Users/cyber/OneDrive/Documentos/NeuroOs/UNIVERSAL-POLYGLOT-LAYER-UPL/docs/CASO_ESTUDIO_WARHAMMER_MEMORY_GUARD.md)**
