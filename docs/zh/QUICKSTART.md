# UPL+ 30秒快速入门

> **语言 / Language:** 🇨🇳 中文 | [🇬🇧 English](../en/QUICKSTART.md) | [🇪🇸 Español](../GUIA_OFICIAL_SINTAXIS_Y_COMANDOS_UPL.md) | [🇫🇷 Français](../fr/QUICKSTART.md) | [🇵🇹 Português](../pt/QUICKSTART.md)

**作者：** José Manuel Moreno (Noxferion)  
**工具链：** UPL+ v2.3.0 — 零依赖 ANSI C99 编译器  
**前置条件：** `gcc`（Windows 使用 MinGW-w64，Linux/macOS 使用系统 `gcc`/`clang`）

---

## 1. 安装

**Windows (PowerShell)：**
```powershell
# 下载并解压发行版包
Invoke-WebRequest -Uri https://github.com/cyberenigma-lgtm/UNIVERSAL-POLYGLOT-LAYER-UPL/releases/download/v2.3.0/uplc-v2.3.0-windows-x64.zip -OutFile uplc.zip
Expand-Archive uplc.zip -DestinationPath uplc
# 将 uplc.exe 添加到 PATH，或从解压目录直接运行
```

**Linux / macOS（从源码构建）：**
```bash
git clone https://github.com/cyberenigma-lgtm/UNIVERSAL-POLYGLOT-LAYER-UPL.git
cd UNIVERSAL-POLYGLOT-LAYER-UPL
bash upl.sh   # 构建 bin/uplc
export PATH="$PWD/bin:$PATH"
```

---

## 2. 第一个程序（零泄漏内核遥测）

将以下内容保存为 `examples/quickstart_30s.upl`：

```upl
// quickstart_30s.upl — UPL+ 零泄漏内核遥测演示
// 双语语法：英语和西班牙语关键字等效

module quickstart

import std/io
import std/kernel_telemetry   // 原生操作系统句柄遥测

fn main() -> i64 {
    // 使用内核 API 打开文件句柄
    let handle = open_file("test.txt")

    // 审计：在任何操作之前验证句柄已被跟踪
    kernel_telemetry::audit_handle(handle, "pre-read")

    // 从句柄读取 128 字节
    let data = read_bytes(handle, 128)
    io::println("读取字节数: ", data.len)

    // 确定性清理 — 无 GC，无泄漏
    defer close_file(handle)

    // 操作后审计：确认句柄将被释放
    kernel_telemetry::audit_handle(handle, "post-read")

    return 0
}
```

**编译和运行：**
```bash
uplc build examples/quickstart_30s.upl -o quickstart
./quickstart          # Linux / macOS
quickstart.exe        # Windows
```

**审计内存和句柄：**
```bash
uplc audit examples/quickstart_30s.upl
```

---

## 3. 核心 CLI 命令

| 命令 | 描述 |
|------|------|
| `uplc build <file.upl> -o <out>` | 通过 C99 编译 `.upl` → 原生二进制文件 |
| `uplc audit <file.upl>` | 分析句柄、内存所有权和锁定顺序 |
| `uplc audit <file.upl> --format sarif` | 输出 SARIF v2.1.0 格式（GitHub 代码扫描） |
| `uplc test <file.upl>` | 执行内联的 `test { }` / `prueba { }` 块 |
| `uplc --emit-c <file.upl>` | 仅生成中间 C99 代码，不编译 |
| `uplc --dump-ast <file.upl>` | 打印抽象语法树 |
| `uplc --version` | 显示工具链版本 |

---

## 4. 双语语法速查表

```upl
// 函数声明（英语/西班牙语——两者均有效）
fn greet(name: str) -> str { ... }
funcion saludar(nombre: str) -> str { ... }

// 变量（不可变）
let x: i64 = 42
sea y: i64 = 42

// 可变变量
let mut counter: i64 = 0
sea mut contador: i64 = 0

// 结构体
struct Point { x: f64, y: f64 }
estructura Punto { x: f64, y: f64 }

// 控制流
if x > 0 { ... } else { ... }
si x > 0 { ... } sino { ... }

while running { ... }
mientras corriendo { ... }

// 原生 C99 内联块
embed "c" {
    #include <stdio.h>
    printf("来自 C99 的问候！\n");
}
```

---

## 5. 下一步

- **编译器架构 →** [COMPILER_ARCHITECTURE.md](COMPILER_ARCHITECTURE.md)
- **完整语法参考 →** [SYNTAX_AND_GRAMMAR.md](SYNTAX_AND_GRAMMAR.md)
- **MCP 服务器 →** [../MCP_SERVER.md](../MCP_SERVER.md)
- **v2.4.0 路线图 →** [../ROADMAP_v2.4.0.md](../ROADMAP_v2.4.0.md)
- **文档总索引 →** [../INDEX.md](../INDEX.md)
