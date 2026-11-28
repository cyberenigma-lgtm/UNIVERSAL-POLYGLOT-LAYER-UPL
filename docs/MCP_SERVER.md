# 🔌 UPL+ Native MCP Server (`upl_mcp`)

`upl_mcp` is a standalone, zero-dependency **C99 Model Context Protocol (MCP)** server (`tools/upl_mcp_server.c`) that exposes the UPL+ compiler (`uplc`) and formal static auditor directly to AI coding agents and modern IDEs (Antigravity IDE, VS Code, Cursor, Claude Desktop).

---

## 🛠️ Exposed MCP Tools

| Tool Name | Description | Arguments |
| :--- | :--- | :--- |
| `upl_audit` | Runs formal static lifecycle audit (LIFO `defer`, `lock_rank`, `shared readonly`). Supports `text` or **SARIF v2.1.0** output. | `file_path` (required), `format` (`"text"` \| `"sarif"`) |
| `upl_run` | Compiles and executes a `.upl` program natively in one step. | `file_path` (required) |
| `upl_build` | Compiles `.upl` to native binary (`bin`), ANSI C99 (`c`), WebAssembly (`wasm32`), or MultiLang-ASM (`asm-multilang`). | `file_path` (required), `target`, `output_file` |
| `upl_list_modules` | Lists all registered UPL+ standard & silicon modules. | None |

---

## ⚙️ Configuration

### Antigravity IDE / VS Code / Cursor (`mcp_config.json`)

```json
{
  "mcpServers": {
    "upl-toolchain": {
      "command": "bin/upl_mcp.exe",
      "args": []
    }
  }
}
```

On Linux/macOS, point `"command"` to `./bin/upl_mcp` (or `upl_mcp` if installed on `PATH` via `install.sh`).

---

## 🚀 Build from Source

```bash
# Windows (MinGW / GCC)
gcc -O2 tools/upl_mcp_server.c -o bin/upl_mcp.exe

# Linux / macOS
gcc -O2 tools/upl_mcp_server.c -o bin/upl_mcp
```

---

## 🧪 Quick Smoke Test (JSON-RPC over stdio)

```bash
echo '{"jsonrpc":"2.0","id":1,"method":"tools/list"}' | ./bin/upl_mcp.exe
```

