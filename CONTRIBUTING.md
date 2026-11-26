# Contributing to Universal Polyglot Layer (UPL+)

Thank you for your interest in contributing to **Universal Polyglot Layer (UPL+)**! We welcome contributions ranging from compiler optimizations and language grammar enhancements to formal systems audits and tier-1 runtime interoperability bridges.

---

## 1. Ground Rules & Guiding Principles

1. **Deterministic & Sovereign Execution:** All UPL+ code and generated native artifacts must adhere to strict determinism, formal memory isolation, and sovereign compile-time validation.
2. **Code Rules the World:** We prioritize minimal, surgical, and mathematically verifiable solutions backed by automated tests over hypothetical discussions.
3. **Zero Binary Artifacts in Git:** Never commit `.exe`, `.dll`, `.so`, `.o`, `.obj`, or intermediate `.upl.c` files into Git. All binaries must be built on-demand or tested in isolated sandboxes.
4. **Academic & Professional Tone:** All public pull request descriptions, documentation, and technical audit reports must be written in formal computer science English without hyperbole or marketing buzzwords.

---

## 2. Setting Up Your Development Environment

### Prerequisites
* **Windows (x64) or Linux (x86_64):** Standard developer environment.
* **GCC / MinGW-w64:** C compiler toolchain supporting C11/C99 (`gcc`, `make`).
* **UPL Compiler (`uplc`):** Located at `bin/uplc.exe` (Windows) or `bin/uplc` (POSIX).

### Verifying Toolchain Status
```bash
# Verify compiler version
bin/uplc.exe --version

# Run test suite
./test.bat   # Windows
./test.sh    # Linux / macOS
```

---

## 3. Contribution Workflow

### Step 1: Open an Issue or Fork
Before implementing large-scale changes or new language features, open an issue using the appropriate template:
* **Bug Report:** For unexpected compiler or runtime behavior.
* **Feature Request:** For proposed syntax, AST nodes, or backend optimizations.
* **System Audit Case:** For benchmarking and auditing tier-1 upstream runtimes (`dotnet/runtime`, `microsoft/PowerToys`, Linux kernel, etc.).

### Step 2: Create a Feature Branch
```bash
git checkout -b feat/my-compiler-optimization
```

### Step 3: Implement & Verify
1. Ensure new grammar, parser, or code-generator features compile cleanly:
   ```bash
   gcc -O3 -Wall -Wextra -Icompiler -c compiler/*.c
   ```
2. Write unit tests under `tests/` or validation scripts in `.upl`.
3. Verify that zero binary leftovers remain before staging.

### Step 4: Submit a Pull Request
1. Commit your changes using conventional commit formatting (`feat:`, `fix:`, `docs:`, `test:`, `refactor:`).
2. Open a Pull Request against the `main` branch.
3. Fill out the **Pull Request Template** completely.

---

## 4. Coding Standards

* **C Code (Compiler & Runtime):** Follow strict C99/C11 standards with explicit memory management (`free` matching `malloc`). Treat compiler warnings as errors (`-Wall -Wextra`).
* **UPL Code (`.upl`):** Adhere to UPL standard grammar:
  * Module declaration: `module ModuleName;`
  * Strong typing: Explicit annotations for function parameters and return types.
  * Native interoperability: Use `embed "c" { ... }` blocks cleanly when bridging to C runtime.

---

## 5. Security Vulnerabilities

If you discover a security vulnerability within UPL+ or its runtime bridges, please do not file a public issue. Follow our [Security Policy](SECURITY.md) and report it to **security@cyberenigma.io**.
