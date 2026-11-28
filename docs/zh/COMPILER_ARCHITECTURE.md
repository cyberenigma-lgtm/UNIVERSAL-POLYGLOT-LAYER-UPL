# UPL+ 编译器架构（v2.3.0）

> **语言 / Language:** 🇨🇳 中文 | [🇬🇧 English](../en/COMPILER_ARCHITECTURE.md) | [🇪🇸 Español](../ARQUITECTURA_COMPILADOR.md) | [🇫🇷 Français](../fr/COMPILER_ARCHITECTURE.md) | [🇵🇹 Português](../pt/COMPILER_ARCHITECTURE.md)

**作者：** José Manuel Moreno (Noxferion)  
**状态：** 技术工程规范  
**实现语言：** ANSI C99 标准（ISO/IEC 9899:1999）  
**外部依赖：** 无（0% Python，零虚拟机，零外部运行时）

---

## 1. 编译流水线概览

UPL+ 编译器（`compiler/`）通过七个结构化阶段将 `.upl` 源代码转换为原生二进制可执行文件：

```
[.upl 源代码]
        │
        ▼  （第 1 阶段：词法分析器）
[类型化令牌流]
        │
        ▼  （第 2 阶段：内存竞技场 + 第 3 & 4 阶段：Pratt 解析器 / AST）
[抽象语法树（AST）]
        │
        ▼  （第 5 阶段：COMP-3 BCD 算术引擎 & 语义分析）
[已验证和类型化的 AST]
        │
        ▼  （第 6 阶段：C99 代码生成）
[ANSI C99 源代码（.c）]
        │
        ▼  （第 7 阶段：后端链接器 GCC/Clang）
[原生可执行二进制文件（.exe / ELF）]
```

---

## 2. 编译器组件

### 2.1 双语词法分析器（`compiler/lexer.c`，`compiler/lexer.h`）

- **目的：** 将 UTF-8 字符流转换为离散令牌（`Token`）的结构化序列。
- **语言统一：** 将英语和西班牙语关键字直接映射到相同的令牌标识符（`TokenType`），无需任何预处理或中间翻译：
  - `fn` / `funcion` → `TOK_FN`
  - `let` / `sea` → `TOK_LET`
  - `struct` / `estructura` → `TOK_STRUCT`
  - `if` / `si` → `TOK_IF`
  - `while` / `mientras` → `TOK_WHILE`
  - `return` / `retornar` → `TOK_RETURN`
  - `embed` / `incrustar` → `TOK_EMBED`
- **原始代码块（`embed`）：** 词法分析器具有原始捕获模式，在 `embed "c"` 指令后完整提取花括号 `{ ... }` 之间的正文，不对内部进行令牌化，完整保留 `#include` 预处理器指令、宏和原生 C API 调用。
- **精确诊断：** 维护精确的行号（`line`）和列号（`column`）计数器，用于编译时错误报告。

### 2.2 竞技场内存分配器（`compiler/upl_arena.c`，`compiler/upl_arena.h`）

- **目的：** 编译器的确定性高性能内存管理。
- **策略：** 保留连续的 64 KB 块（`UPL_ARENA_BLOCK_SIZE`）。当分配超过当前块时，新块被链接到单向链表中。
- **相对于传统 `malloc`/`free` 的优势：**
  - 解析大型程序时堆零碎片。
  - 通过指针递增（`offset += size`）实现 O(1) 分配。
  - 在翻译单元结束时 O(1) 瞬时释放所有 AST 节点和标识符（`upl_arena_free`）。

### 2.3 语法分析器和 Pratt 解析（`compiler/upl_parser.c`，`compiler/upl_parser.h`）

- **目的：** 在验证正式 EBNF 语法的同时构建 AST。
- **递归下降：** 用于程序的高级结构（函数声明、参数、结构体和控制语句）。
- **Pratt 解析（优先级攀爬）：** 用于算术和逻辑表达式分析，确保严格、无歧义的优先级层次：
  ```
  级别 1（最高）：分组 ()、函数调用 f()、字面量
  级别 2：乘法 (*)、除法 (/)、取模 (%)
  级别 3：加法 (+)、减法 (-)
  级别 4：关系比较 (<, <=, >, >=)
  级别 5：相等比较 (==, !=)
  级别 6：逻辑与 (&&)
  级别 7：逻辑或 (||)
  级别 8（最低）：赋值 (=)
  ```

### 2.4 COMP-3 BCD 十进制精度引擎

- **目的：** 保证金融、会计或科学计算中的绝对数学精度。
- **编码：** 使用符合金融计算行业标准的压缩 BCD（Packed Decimal）。
- **软件运算：** 包括加减乘法、舍入以及 ASCII 字符串表示与二进制 BCD 缓冲区之间双向转换的例程。

### 2.5 C99 代码生成器（`compiler/upl_codegen.c`，`compiler/upl_codegen.h`）

- **目的：** 生成符合 ISO/IEC 规范的简洁、可读的 ANSI C99 源代码。
- **策略：** 将 AST 节点翻译为等效的 C99 表达式和指令，在程序员定义的确切位置生成 `embed` 原生块。

### 2.6 工具链驱动程序（`compiler/main.c`）

- **支持的参数：**
  - `-o <输出>`: 最终二进制可执行文件的名称。
  - `--emit-c`: 仅生成中间 C99 代码，不调用系统编译器。
  - `--dump-ast`: 将语法树打印到控制台以供调试。
  - `--dump-tokens`: 打印词法分析器生成的令牌流。
  - `--version`: 显示官方工具链版本。

---

## 3. 与操作系统的集成

- Windows: [`upl.bat`](../../upl.bat)（调用 MinGW-w64 的 `gcc.exe`）
- Linux / macOS: [`upl.sh`](../../upl.sh)（调用 `gcc` 或 `clang`）

基本开发周期无需 Python、Node.js 或 CMake、Make 等外部构建工具。

---

## 4. 参见

- [快速入门指南](QUICKSTART.md)
- [语法和语法规范](SYNTAX_AND_GRAMMAR.md)
- [文档总索引](../INDEX.md)
