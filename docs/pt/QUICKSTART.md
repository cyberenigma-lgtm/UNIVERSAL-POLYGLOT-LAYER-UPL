# UPL+ Início Rápido em 30 Segundos

> **Idioma / Language:** 🇵🇹 Português | [🇬🇧 English](../en/QUICKSTART.md) | [🇪🇸 Español](../GUIA_OFICIAL_SINTAXIS_Y_COMANDOS_UPL.md) | [🇨🇳 中文](../zh/QUICKSTART.md) | [🇫🇷 Français](../fr/QUICKSTART.md)

**Autor:** José Manuel Moreno (Noxferion)  
**Toolchain:** UPL+ v2.3.0 — Compilador ANSI C99 sem dependências  
**Pré-requisitos:** `gcc` (MinGW-w64 no Windows, `gcc`/`clang` do sistema no Linux/macOS)

---

## 1. Instalação

**Windows (PowerShell):**
```powershell
Invoke-WebRequest -Uri https://github.com/cyberenigma-lgtm/UNIVERSAL-POLYGLOT-LAYER-UPL/releases/download/v2.3.0/uplc-v2.3.0-windows-x64.zip -OutFile uplc.zip
Expand-Archive uplc.zip -DestinationPath uplc
```

**Linux / macOS (compilar a partir do código-fonte):**
```bash
git clone https://github.com/cyberenigma-lgtm/UNIVERSAL-POLYGLOT-LAYER-UPL.git
cd UNIVERSAL-POLYGLOT-LAYER-UPL
bash upl.sh
export PATH="$PWD/bin:$PATH"
```

---

## 2. Primeiro Programa (Telemetria de Kernel Sem Vazamentos)

Salve o seguinte como `examples/quickstart_30s.upl`:

```upl
// quickstart_30s.upl — Demo de telemetria de kernel sem vazamentos UPL+
// Sintaxe bilíngue: palavras-chave em inglês e espanhol são equivalentes

module quickstart

import std/io
import std/kernel_telemetry

fn main() -> i64 {
    let handle = open_file("test.txt")
    kernel_telemetry::audit_handle(handle, "pre-read")

    let data = read_bytes(handle, 128)
    io::println("Bytes lidos: ", data.len)

    defer close_file(handle)   // Limpeza determinística — sem GC, sem vazamentos
    kernel_telemetry::audit_handle(handle, "post-read")

    return 0
}
```

**Compilar e executar:**
```bash
uplc build examples/quickstart_30s.upl -o quickstart
./quickstart          # Linux / macOS
quickstart.exe        # Windows
```

**Auditar memória e handles:**
```bash
uplc audit examples/quickstart_30s.upl
```

---

## 3. Comandos CLI Essenciais

| Comando | Descrição |
|---------|-----------|
| `uplc build <arquivo.upl> -o <saída>` | Compila `.upl` → binário nativo via C99 |
| `uplc audit <arquivo.upl>` | Analisa handles, propriedade de memória e ordens de bloqueio |
| `uplc audit <arquivo.upl> --format sarif` | Saída SARIF v2.1.0 para GitHub Code Scanning |
| `uplc test <arquivo.upl>` | Executa blocos `test { }` / `prueba { }` inline |
| `uplc --emit-c <arquivo.upl>` | Gera apenas o código C99 intermediário |
| `uplc --dump-ast <arquivo.upl>` | Imprime a Árvore de Sintaxe Abstrata |
| `uplc --version` | Exibe a versão da toolchain |

---

## 4. Folha de Referência da Sintaxe Bilíngue

```upl
// Declarações de função (inglês / espanhol — ambos válidos)
fn greet(name: str) -> str { ... }
funcion saludar(nombre: str) -> str { ... }

// Variáveis imutáveis
let x: i64 = 42
sea y: i64 = 42

// Variáveis mutáveis
let mut counter: i64 = 0
sea mut contador: i64 = 0

// Estruturas
struct Point { x: f64, y: f64 }
estructura Punto { x: f64, y: f64 }

// Controle de fluxo
if x > 0 { ... } else { ... }
si x > 0 { ... } sino { ... }

while running { ... }
mientras corriendo { ... }

// Bloco nativo C99
embed "c" {
    #include <stdio.h>
    printf("Olá do C99!\n");
}
```

---

## 5. Próximos Passos

- **Arquitetura do compilador →** [COMPILER_ARCHITECTURE.md](COMPILER_ARCHITECTURE.md)
- **Referência completa de sintaxe →** [SYNTAX_AND_GRAMMAR.md](SYNTAX_AND_GRAMMAR.md)
- **Servidor MCP →** [../MCP_SERVER.md](../MCP_SERVER.md)
- **Roteiro v2.4.0 →** [../ROADMAP_v2.4.0.md](../ROADMAP_v2.4.0.md)
- **Índice principal →** [../INDEX.md](../INDEX.md)
