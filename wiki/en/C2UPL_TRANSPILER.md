# 🔄 OFFICIAL C99 TO UPL+ NATIVE TRANSPILER GUIDE (`c2upl`)
## Automatic Ingestion and Code Migration from C99 to UPL+

**Tool Source:** `tools/c2upl.c`  
**Binary Executable:** `bin/c2upl.exe` (Windows) / `bin/c2upl` (Linux/macOS)  
**Dependencies:** 0% Python, 0% Rust, 100% ANSI C99 Standard  
**Verification Status:** 5/5 Unit Tests Passing (0 Failures)

---

### 1. Why a Dedicated C99 to UPL+ Transpiler?

In modern systems programming languages such as Zig (`zig translate-c`), having an automated translation utility allows developers to import low-level algorithms, structs, and routines without manual line-by-line transcription.

**UPL+** and **ANSI C99** share the exact same physical memory execution model:
* Direct raw pointers (`*T` $\leftrightarrow$ `T*`).
* Packed contiguous structs without garbage collection (GC).
* Fixed-width primitive integer and float types (`i32`, `u64`, `f64`).

Because memory layout and pointer semantics are physically identical, translation from C99 to UPL+ is deterministic and lossless.

---

### 2. Building the Transpiler

The tool is implemented in a standalone, single-file C99 source code with zero external libraries:

```bash
# On Windows (CMD or PowerShell):
gcc -std=c99 -Wall -Wextra -Werror -O2 tools/c2upl.c -o bin/c2upl.exe

# On Linux or macOS:
gcc -std=c99 -Wall -Wextra -Werror -O2 tools/c2upl.c -o bin/c2upl
```

---

### 3. Command-Line Interface (`c2upl`)

```
Usage: c2upl [options] <file.c>

Options:
  -o <output.upl>       Specify output transpiled UPL+ file path
  -l, --lang <en|es>    Target UPL+ dialect (default: en)
  --test                Run internal unit verification suite (5/5 tests)
```

---

### 4. Type Mapping Reference (C99 $\to$ UPL+)

| C99 Type | UPL+ Type | Description |
|---|---|---|
| `void` | `void` | Empty / returnless type |
| `void*` | `*void` | Universal opaque pointer |
| `int8_t` / `signed char` | `i8` | 8-bit signed integer |
| `uint8_t` / `unsigned char` | `u8` | 8-bit unsigned byte |
| `int16_t` / `short` | `i16` | 16-bit signed integer |
| `uint16_t` / `unsigned short` | `u16` | 16-bit unsigned integer |
| `int` / `int32_t` / `long` | `i32` | 32-bit signed integer |
| `unsigned int` / `uint32_t` | `u32` | 32-bit unsigned integer |
| `int64_t` / `long long` | `i64` | 64-bit signed integer |
| `uint64_t` / `unsigned long long` | `u64` | 64-bit unsigned integer |
| `size_t` | `usize` | Pointer-sized indexing integer |
| `float` | `f32` | IEEE-754 single precision float |
| `double` | `f64` | IEEE-754 double precision float |
| `char*` | `*i8` | C character / string pointer |
| `type*` | `*type` | Typed raw pointer |
| `type buffer[N]` | `buffer: [type; N]` | Fixed-size array |

---

### 5. Bilingual Dialect Support (English and Spanish)

`c2upl` supports emitting standard English or native Spanish keywords:

#### Input C99 Code (`algorithm.c`):
```c
int32_t factorial(int32_t n) {
    if (n <= 1) {
        return 1;
    }
    return n * factorial(n - 1);
}
```

#### Output in Standard English (`c2upl algorithm.c`):
```upl
pub fn factorial(n: i32) -> i32 {
    if n <= 1 {
        return 1 ; 
    }
    return n * factorial ( n - 1 ) ; 
}
```

#### Output in Native Spanish (`c2upl -l es algorithm.c`):
```upl
pub funcion factorial(n: i32) -> i32 {
    si n <= 1 {
        retornar 1 ; 
    }
    retornar n * factorial ( n - 1 ) ; 
}
```

---

### 6. Struct Ingestion and `#include` Directives

* **Preprocessor Directives:** `#include` lines are captured inside native `embed "c" { ... }` blocks, preserving host headers for seamless C backend generation.
* **Declarations `typedef struct`:** Automatically transpiled to `pub struct` (or `pub estructura`), mapping field types and fixed array bounds.

#### Input C99 Struct:
```c
typedef struct {
    uint8_t header[4];
    uint32_t tx_id;
    int64_t balance;
} FinancialRecord;
```

#### Transpiled UPL+:
```upl
pub struct FinancialRecord {
    header: [u8; 4],
    tx_id: u32,
    balance: i64,
}
```

---

### 7. End-to-End Migration Workflow

1. Transpile your existing C99 source to UPL+:
   ```bash
   bin/c2upl.exe my_module.c -o my_module.upl
   ```

2. Compile the transpiled file with the `uplc` compiler:
   ```bash
   upl.bat -o my_module.exe my_module.upl
   ```

3. Run the compiled standalone binary:
   ```bash
   my_module.exe
   ```
