# UPL+ 30-Second Quickstart

> **Language / Idioma:** 🇬🇧 English | [🇪🇸 Español](../GUIA_OFICIAL_SINTAXIS_Y_COMANDOS_UPL.md) | [🇨🇳 中文](../zh/QUICKSTART.md) | [🇫🇷 Français](../fr/QUICKSTART.md) | [🇵🇹 Português](../pt/QUICKSTART.md)

**Author:** José Manuel Moreno (Noxferion)  
**Toolchain:** UPL+ v2.3.0 — Zero-dependency ANSI C99 compiler  
**Prerequisites:** `gcc` (MinGW-w64 on Windows, system `gcc`/`clang` on Linux/macOS)

---

## 1. Install

**Windows (PowerShell):**
```powershell
# Download and extract the release package
Invoke-WebRequest -Uri https://github.com/cyberenigma-lgtm/UNIVERSAL-POLYGLOT-LAYER-UPL/releases/download/v2.3.0/uplc-v2.3.0-windows-x64.zip -OutFile uplc.zip
Expand-Archive uplc.zip -DestinationPath uplc
# Add uplc.exe to your PATH or run from the extracted folder
```

**Linux / macOS (build from source):**
```bash
git clone https://github.com/cyberenigma-lgtm/UNIVERSAL-POLYGLOT-LAYER-UPL.git
cd UNIVERSAL-POLYGLOT-LAYER-UPL
bash upl.sh   # builds bin/uplc
export PATH="$PWD/bin:$PATH"
```

---

## 2. Your First Program (Zero-Leak Kernel Telemetry)

Save the following as `examples/quickstart_30s.upl`:

```upl
// quickstart_30s.upl — UPL+ Zero-Leak Kernel Telemetry Demo
// Bilingual syntax: English and Spanish keywords are equivalent

module quickstart

import std/io
import std/kernel_telemetry   // Native OS handle telemetry

fn main() -> i64 {
    // Open a file handle using the kernel API
    let handle = open_file("test.txt")

    // Audit: verify the handle is tracked before any operation
    kernel_telemetry::audit_handle(handle, "pre-read")

    // Read 128 bytes from the handle
    let data = read_bytes(handle, 128)
    io::println("Read bytes: ", data.len)

    // Deterministic cleanup — no GC, no leaks
    defer close_file(handle)

    // Post-operation audit: confirms handle will be released
    kernel_telemetry::audit_handle(handle, "post-read")

    return 0
}
```

**Compile and run:**
```bash
uplc build examples/quickstart_30s.upl -o quickstart
./quickstart          # Linux / macOS
quickstart.exe        # Windows
```

**Audit memory and handles:**
```bash
uplc audit examples/quickstart_30s.upl
```

---

## 3. Key CLI Commands

| Command | Description |
|---------|-------------|
| `uplc build <file.upl> -o <out>` | Compile `.upl` → native binary via C99 |
| `uplc audit <file.upl>` | Analyze handles, memory ownership, and lock ranks |
| `uplc audit <file.upl> --format sarif` | Output SARIF v2.1.0 for GitHub Code Scanning |
| `uplc test <file.upl>` | Run `test { }` / `prueba { }` blocks inline |
| `uplc --emit-c <file.upl>` | Emit intermediate C99 without compiling |
| `uplc --dump-ast <file.upl>` | Print the Abstract Syntax Tree |
| `uplc --version` | Show toolchain version |

---

## 4. Bilingual Syntax Cheat Sheet

```upl
// Declarations (English / Spanish — both valid)
fn greet(name: str) -> str { ... }
funcion saludar(nombre: str) -> str { ... }

// Variables
let x: i64 = 42
sea y: i64 = 42

// Mutable variables
let mut counter: i64 = 0
sea mut contador: i64 = 0

// Structures
struct Point { x: f64, y: f64 }
estructura Punto { x: f64, y: f64 }

// Control flow
if x > 0 { ... } else { ... }
si x > 0 { ... } sino { ... }

while running { ... }
mientras corriendo { ... }

// Native C99 inline blocks
embed "c" {
    #include <stdio.h>
    printf("Hello from C99!\n");
}
```

---

## 5. Next Steps

- **Compiler Architecture →** [COMPILER_ARCHITECTURE.md](COMPILER_ARCHITECTURE.md)
- **Full Syntax Reference →** [SYNTAX_AND_GRAMMAR.md](SYNTAX_AND_GRAMMAR.md)
- **MCP Server →** [../MCP_SERVER.md](../MCP_SERVER.md)
- **v2.4.0 Roadmap →** [../ROADMAP_v2.4.0.md](../ROADMAP_v2.4.0.md)
- **Master Index →** [../INDEX.md](../INDEX.md)
