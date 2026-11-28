# UPL+ 语法和语法规范（v2.3.0）

> **语言 / Language:** 🇨🇳 中文 | [🇬🇧 English](../en/SYNTAX_AND_GRAMMAR.md) | [🇪🇸 Español](../SPEC_GRAMATICA_Y_SISTEMA_TIPOS.md) | [🇫🇷 Français](../fr/SYNTAX_AND_GRAMMAR.md) | [🇵🇹 Português](../pt/SYNTAX_AND_GRAMMAR.md)

**作者：** José Manuel Moreno (Noxferion)  
**分类：** 系统编程语言 / 类型化 C99 转译

---

## 1. 双语关键字对照表

词法分析器将英语和西班牙语关键字确定性地映射到相同的令牌标识符（`UPLTokenType`）：

| 词法令牌 | 关键字（英语） | 关键字（西班牙语） | 语义 |
|---------|-------------|----------------|------|
| `TOK_MODULE` | `module` | `modulo` | 编译单元命名空间声明 |
| `TOK_IMPORT` | `import` | `importar` | 外部模块或单元包含 |
| `TOK_STRUCT` | `struct` | `estructura` | 聚合数据结构定义 |
| `TOK_PUB` | `pub` | `publico` | 可导出可见性修饰符 |
| `TOK_FN` | `fn` | `funcion` | 子程序声明或定义 |
| `TOK_LET` | `let` | `sea` | 默认不可变变量声明 |
| `TOK_MUT` | `mut` | `mut` | 变量可变性修饰符 |
| `TOK_CONST` | `const` | `constante` | 编译时不可变常量 |
| `TOK_IF` | `if` | `si` | 条件分支 |
| `TOK_ELSE` | `else` | `sino` | 条件分支的替代分支 |
| `TOK_WHILE` | `while` | `mientras` | 前测条件迭代循环 |
| `TOK_FOR` | `for` | `para` | 结构化迭代循环 |
| `TOK_RETURN` | `return` | `retornar` | 从函数返回控制和值 |
| `TOK_AS` | `as` | `como` | 显式类型转换（强制类型转换） |
| `TOK_EMBED` | `embed` | `incrustar` | 字面量原生代码块（C 或宿主语言） |
| `TOK_ASM` | `asm` | `ensamblador` | 内联汇编指令块 |

---

## 2. 类型系统

### 2.1 基本类型

| 类型 | 大小 | 描述 |
|------|------|------|
| `i8` | 1 字节 | 有符号 8 位整数 |
| `i16` | 2 字节 | 有符号 16 位整数 |
| `i32` | 4 字节 | 有符号 32 位整数 |
| `i64` | 8 字节 | 有符号 64 位整数 |
| `u8` | 1 字节 | 无符号 8 位整数 |
| `u64` | 8 字节 | 无符号 64 位整数 |
| `f32` | 4 字节 | IEEE 754 单精度浮点数 |
| `f64` | 8 字节 | IEEE 754 双精度浮点数 |
| `bool` | 1 字节 | 布尔值（`true` / `false`） |
| `str` | 指针 + 长度 | UTF-8 字符串切片 |
| `void` | — | 无值（函数返回） |

### 2.2 所有权和 `defer`

```upl
fn process() -> i64 {
    let handle = open_resource()
    defer close_resource(handle)   // 作用域退出时保证释放
    // ... 使用 handle ...
    return 0
}
```

---

## 3. 语法示例

```upl
module example

import std/io

// 函数声明（英语/西班牙语等效）
fn add(a: i64, b: i64) -> i64 {
    return a + b
}

funcion multiplicar(a: i64, b: i64) -> i64 {
    retornar a * b
}

fn main() -> i64 {
    let result = add(10, 20)
    sea producto = multiplicar(3, 7)

    si result > 0 {
        io::println("结果为正数: ", result)
    } sino {
        io::println("结果为零或负数")
    }

    sea mut contador: i64 = 0
    mientras contador < 5 {
        io::println("计数: ", contador)
        contador = contador + 1
    }

    return 0
}
```

---

## 4. 原生 C99 互操作性

```upl
module interop

fn native_example() -> i64 {
    embed "c" {
        #include <stdio.h>
        printf("来自 C99 的 UPL+\n");
        return 0LL;
    }
}
```

---

## 5. 参见

- [快速入门指南](QUICKSTART.md)
- [编译器架构](COMPILER_ARCHITECTURE.md)
- [文档总索引](../INDEX.md)
