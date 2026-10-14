# 📖 UPL SYNTAX & GRAMMAR REFERENCE MANUAL
## Official Specification of Keywords, Data Types, and Directives

---

### 1. Bilingual Keywords Equivalence

All keywords can be written in English or Spanish interchangeably:

| English | Spanish | Description |
|---|---|---|
| `fn` | `funcion` | Function declaration |
| `let` | `sea` / `variable` | Immutable local variable declaration |
| `mut` | `mutable` | Mutability modifier |
| `const` | `constante` | Compile-time constant |
| `struct` | `estructura` | Packed data structure declaration |
| `if` / `else` | `si` / `sino` | Conditional branching |
| `while` | `mientras` | Conditional loop |
| `for` ... `in` | `para` ... `en` | Range loop |
| `return` | `retornar` | Function return |
| `pub` | `publico` | Public visibility export |
| `module` | `modulo` | Namespace identifier |
| `as` | `como` | Explicit type cast |
| `asm` | `asm` | Inline CPU assembly block |
| `embed` | `embed` | Polyglot embedded block |
| `and` / `&&` | `and` / `&&` | Logical AND |
| `or` / `\|\|` | `or` / `\|\|` | Logical OR |

---

### 2. Primitive Data Types

* **Unsigned Integers:** `u8` (byte), `u16` (I/O ports), `u32` (standard 32-bit uint), `u64` (64-bit uint/address), `usize` (native pointer width).
* **Signed Integers:** `i8`, `i16`, `i32`, `i64`.
* **Floating-point:** `f32`, `f64`.
* **Logical & Text:** `bool` (`true`/`false`), `str` (UTF-8 string), `void`.
* **NeuroCOBOL Exact Math:** `comp3` / `decimal` (Packed BCD decimal for financial precision).
* **Pointers & Static Arrays:** `*u32` (raw memory pointer), `[u32; 16]` (fixed static array).

---

### 3. Silicon Directives

* `@baremetal`: Disables host OS runtime libraries; enables privileged CPU instructions (`cli`, `sti`, `inb`, `outb`, `hlt`).
* `@snapshot`: Activates atomic state checkpoints and fast landing-pad rollbacks.
* `@dvtrgas30` / `@vectorial`: Applies 52.80 GB/s vector optimization and canonical `0xAA30` parity seals.
* `@guarded`: Enforces static bounds verification to eliminate buffer overflow bugs.
* `@pure`: Declares deterministic side-effect-free functions.
