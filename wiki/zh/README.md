# 🚀 UPL+ 编程语言快速入门指南
## 高性能确定性系统编程语言与原生 C99 独立编译器

欢迎查阅 **UPL+ (Universal Polyglot Layer Plus)** 官方技术文档。UPL+ 是一门**编译型、强类型、零依赖的自主系统编程语言**，专为极端性能、底层硅片交互、多语言无缝融合以及绝对精确的金融十进制计算（COMP-3 BCD）而打造。

---

### 🌟 UPL+ 核心架构优势

1. **完全自包含编译器 (`uplc`)：** 基于模块化 ANSI C99 标准打造，完全脱离 Python、LLVM 或大型运行时的捆绑，编译极速（通常小于 50 毫秒）。
2. **多语言与跨国原生支持：** 词法分析器与语法树（AST）原生支持全球 150 种语言关键词等效编译。
3. **高精度财务十进制类型 (`comp3`)：** 专为金融核心设计的软件打包 BCD 类型，彻底消除 IEEE-754 浮点舍入误差。
4. **硅片级嵌入式混合 (`embed` 与 `asm`)：** 原生支持嵌入 C99 代码块和 x86_64 汇编指令，零 FFI 调用损耗。
5. **高性能竞技场内存分配 (Arena Allocator)：** 64 KB 分块确定性管理，彻底告别内存碎片化，实现 $O(1)$ 瞬时回收。

---

### 📦 编写第一个 UPL+ 程序

```upl
module example;

pub fn main() -> i32 {
    let mut total: i32 = 0;
    let base: i32 = 1000;
    let tax: i32 = 13;

    total = base + (base * tax / 100);
    return 0;
}
```

---

### ⚡ 编译与直接运行

```bash
# 一键编译并执行：
bin/uplc.exe run program.upl

# 生成原生 64 位可执行文件：
bin/uplc.exe build program.upl -o bin/program.exe
```

---
*Universal Polyglot Layer (UPL) — 知识产权受 SafeCreative 法律保护。*
