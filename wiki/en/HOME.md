# 🚀 UPL PROGRAMMING LANGUAGE - QUICKSTART
## The Next-Generation Sovereign Systems Language

Welcome to the official documentation of **UPL (Universal Polyglot Layer)**. UPL is a **brand-new, independent, first-class systems programming language** designed from the ground up for extreme hardware performance, bilingual sovereignty, and self-healing resilience superior to Rust and C++.

---

### 🌟 Why is UPL a Completely New Language?

1. **Dedicated Native Compiler (`uplc`):** Built with its own lexical scanner, recursive-descent parser, typed Abstract Syntax Tree (AST), and dual code generators for ANSI C99 and x86_64 NASM Long Mode.
2. **First-Class Dual Syntax:** The first language worldwide with full native syntax parity between Spanish and English.
3. **Silicon Landing-Pads (DVTRGAS-30 0xAA30):** Automatic boundary guard pages with canonical parity checks that prevent buffer overflows before execution touches adjacent memory.
4. **Self-Healing Resilience (`@snapshot`):** Unlike Rust’s process-killing `panic!`, UPL captures atomic state checkpoints and rolls back execution in nanoseconds without service downtime.
5. **Exact Decimal Math (`comp3`):** Native BCD packed decimals that completely eliminate floating-point IEEE-754 rounding inaccuracies in financial and critical computations.

---

### 📦 Your First UPL Program

Save as `hello_sovereign.upl`:

```rust
module demo;

pub fn main() -> i32 {
    let mut counter: i32 = 0;
    while counter < 5 {
        counter = counter + 1;
    }
    return counter;
}
```

---

### ⚡ Running & Compiling

From the root directory:

```bash
# Direct one-step native execution (Windows: upl.bat / Linux: ./upl.sh)
upl run hello_sovereign.upl

# Compile to optimized C99 source code
upl build hello_sovereign.upl -t c -o output.c

# Compile to pure x86_64 Long Mode Assembly for Ring 0 (Bare-Metal / Firmware)
upl build hello_sovereign.upl -t asm --baremetal -o output.asm
```

---

### 📚 Wiki Sections
* **[Syntax & Grammar Reference](SYNTAX_AND_GRAMMAR.md)**
* **[Compiler Pipeline & Architecture](COMPILER_ARCHITECTURE.md)**
* **[Multi-OS Error Dictionary (57 Languages)](ERROR_DICTIONARY.md)**
* **[Code Examples Gallery](EXAMPLES.md)**
