# Sintaxe e Gramática UPL+ — Especificação (v2.3.0)

> **Idioma / Language:** 🇵🇹 Português | [🇬🇧 English](../en/SYNTAX_AND_GRAMMAR.md) | [🇪🇸 Español](../SPEC_GRAMATICA_Y_SISTEMA_TIPOS.md) | [🇨🇳 中文](../zh/SYNTAX_AND_GRAMMAR.md) | [🇫🇷 Français](../fr/SYNTAX_AND_GRAMMAR.md)

**Autor:** José Manuel Moreno (Noxferion)  
**Classificação:** Linguagem de programação de sistemas / Transpilação tipada C99

---

## 1. Tabela de Equivalência de Palavras-chave Bilíngues

O analisador léxico mapeia de forma determinística palavras-chave em inglês e espanhol para o mesmo identificador de token (`UPLTokenType`):

| Token Léxico | Palavra-chave (EN) | Palavra-chave (ES) | Semântica |
|---|---|---|---|
| `TOK_MODULE` | `module` | `modulo` | Declaração do espaço de nomes da unidade de compilação |
| `TOK_IMPORT` | `import` | `importar` | Inclusão de módulo ou unidade externa |
| `TOK_STRUCT` | `struct` | `estructura` | Definição de estrutura de dados agregada |
| `TOK_PUB` | `pub` | `publico` | Modificador de visibilidade exportável |
| `TOK_FN` | `fn` | `funcion` | Declaração ou definição de sub-rotina |
| `TOK_LET` | `let` | `sea` | Declaração de variável imutável por padrão |
| `TOK_MUT` | `mut` | `mut` | Modificador de mutabilidade para variáveis |
| `TOK_CONST` | `const` | `constante` | Constante imutável avaliável em tempo de compilação |
| `TOK_IF` | `if` | `si` | Ramificação condicional |
| `TOK_ELSE` | `else` | `sino` | Ramo alternativo do condicional |
| `TOK_WHILE` | `while` | `mientras` | Loop de iteração condicional pré-teste |
| `TOK_FOR` | `for` | `para` | Loop de iteração estruturado |
| `TOK_RETURN` | `return` | `retornar` | Retorno de controle e valor de uma função |
| `TOK_AS` | `as` | `como` | Conversão de tipo explícita (cast) |
| `TOK_EMBED` | `embed` | `incrustar` | Bloco de código nativo literal (C ou linguagem hospedeira) |
| `TOK_ASM` | `asm` | `ensamblador` | Bloco de instruções assembly inline |

---

## 2. Sistema de Tipos

### 2.1 Tipos Primitivos

| Tipo | Tamanho | Descrição |
|------|---------|-----------|
| `i8` | 1 byte | Inteiro com sinal de 8 bits |
| `i16` | 2 bytes | Inteiro com sinal de 16 bits |
| `i32` | 4 bytes | Inteiro com sinal de 32 bits |
| `i64` | 8 bytes | Inteiro com sinal de 64 bits |
| `u8` | 1 byte | Inteiro sem sinal de 8 bits |
| `u64` | 8 bytes | Inteiro sem sinal de 64 bits |
| `f32` | 4 bytes | Ponto flutuante IEEE 754 de precisão simples |
| `f64` | 8 bytes | Ponto flutuante IEEE 754 de precisão dupla |
| `bool` | 1 byte | Booleano (`true` / `false`) |
| `str` | ponteiro + comprimento | Fatia de string UTF-8 |
| `void` | — | Sem valor (retorno de função) |

### 2.2 Propriedade e `defer`

```upl
fn process() -> i64 {
    let handle = open_resource()
    defer close_resource(handle)   // Liberação garantida na saída do escopo
    return 0
}
```

---

## 3. Exemplo de Sintaxe

```upl
module exemplo

import std/io

fn somar(a: i64, b: i64) -> i64 {
    return a + b
}

funcion multiplicar(a: i64, b: i64) -> i64 {
    retornar a * b
}

fn main() -> i64 {
    let resultado = somar(10, 20)

    si resultado > 0 {
        io::println("Resultado positivo: ", resultado)
    } sino {
        io::println("Resultado zero ou negativo")
    }

    sea mut contador: i64 = 0
    mientras contador < 5 {
        io::println("Contagem: ", contador)
        contador = contador + 1
    }

    return 0
}
```

---

## 4. Interoperabilidade Nativa C99

```upl
module interop

fn exemplo_nativo() -> i64 {
    embed "c" {
        #include <stdio.h>
        printf("UPL+ a partir de C99!\n");
        return 0LL;
    }
}
```

---

## 5. Ver Também

- [Guia de Início Rápido](QUICKSTART.md)
- [Arquitetura do Compilador](COMPILER_ARCHITECTURE.md)
- [Índice Principal da Documentação](../INDEX.md)
