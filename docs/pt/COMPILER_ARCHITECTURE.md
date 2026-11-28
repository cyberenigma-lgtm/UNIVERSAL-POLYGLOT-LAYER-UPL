# Arquitetura Interna do Compilador UPL+ (v2.3.0)

> **Idioma / Language:** 🇵🇹 Português | [🇬🇧 English](../en/COMPILER_ARCHITECTURE.md) | [🇪🇸 Español](../ARQUITECTURA_COMPILADOR.md) | [🇨🇳 中文](../zh/COMPILER_ARCHITECTURE.md) | [🇫🇷 Français](../fr/COMPILER_ARCHITECTURE.md)

**Autor:** José Manuel Moreno (Noxferion)  
**Estado:** Especificação Técnica de Engenharia  
**Linguagem de Implementação:** ANSI C99 Standard (ISO/IEC 9899:1999)  
**Dependências Externas:** Nenhuma (0% Python, Zero VMs, Zero runtimes externos)

---

## 1. Visão Geral do Pipeline

O compilador UPL+ (`compiler/`) transforma código-fonte `.upl` em executáveis binários nativos através de um pipeline estruturado em sete etapas:

```
[Código-fonte .upl]
        │
        ▼  (Fase 1: Lexer)
[Fluxo de tokens tipados]
        │
        ▼  (Fase 2: Arena de memória + Fase 3 & 4: Parser Pratt / AST)
[Árvore de Sintaxe Abstrata (AST)]
        │
        ▼  (Fase 5: Motor Aritmético COMP-3 BCD & Semântica)
[AST validada e tipada]
        │
        ▼  (Fase 6: Geração de código C99)
[Código-fonte ANSI C99 (.c)]
        │
        ▼  (Fase 7: Backend Linker GCC/Clang)
[Executável binário nativo (.exe / ELF)]
```

---

## 2. Componentes do Compilador

### 2.1 Analisador Léxico Bilíngue (`compiler/lexer.c`, `compiler/lexer.h`)

- **Objetivo:** Converte o fluxo de caracteres UTF-8 numa sequência estruturada de tokens discretos (`Token`).
- **Unificação de idioma:** Mapeia diretamente palavras-chave em inglês e espanhol para o mesmo identificador de token (`TokenType`):
  - `fn` / `funcion` → `TOK_FN`
  - `let` / `sea` → `TOK_LET`
  - `struct` / `estructura` → `TOK_STRUCT`
  - `if` / `si` → `TOK_IF`
  - `while` / `mientras` → `TOK_WHILE`
  - `return` / `retornar` → `TOK_RETURN`
  - `embed` / `incrustar` → `TOK_EMBED`
- **Blocos de código brutos (`embed`):** O lexer possui modo de captura bruta que extrai integralmente o corpo entre chaves `{ ... }` após a diretiva `embed "c"` sem tokenizar seu interior, preservando diretivas de pré-processador `#include`, macros e chamadas nativas da API C.
- **Diagnósticos precisos:** Mantém contadores exatos de número de linha (`line`) e coluna (`column`) para reporte de erros em tempo de compilação.

### 2.2 Alocador de Memória por Arenas (`compiler/upl_arena.c`, `compiler/upl_arena.h`)

- **Objetivo:** Gestão determinística e de alto desempenho da memória do compilador.
- **Estratégia:** Reserva blocos contíguos de 64 KB (`UPL_ARENA_BLOCK_SIZE`). Quando uma alocação excede o bloco atual, um novo bloco é encadeado numa lista simplesmente ligada.
- **Vantagens sobre `malloc`/`free` tradicional:**
  - Zero fragmentação do heap durante a análise de programas grandes.
  - Alocação em O(1) por incremento de ponteiro (`offset += size`).
  - Liberação instantânea O(1) de todos os nós AST e identificadores (`upl_arena_free`) ao final da unidade de tradução.

### 2.3 Analisador Sintático e Pratt Parsing (`compiler/upl_parser.c`, `compiler/upl_parser.h`)

- **Objetivo:** Constrói a AST validando a gramática EBNF formal.
- **Descida Recursiva:** Usada para a estrutura de alto nível do programa.
- **Pratt Parsing (Escalada de Precedência):** Usado para análise de expressões aritméticas e lógicas:
  ```
  Nível 1 (Maior): Agrupamento (), Chamadas f(), Literais
  Nível 2: Multiplicação (*), Divisão (/), Módulo (%)
  Nível 3: Adição (+), Subtração (-)
  Nível 4: Comparações relacionais (<, <=, >, >=)
  Nível 5: Comparações de igualdade (==, !=)
  Nível 6: Conjunção lógica (&&)
  Nível 7: Disjunção lógica (||)
  Nível 8 (Menor): Atribuição (=)
  ```

### 2.4 Motor de Precisão Decimal COMP-3 BCD

- **Objetivo:** Garantir precisão matemática absoluta em cálculos financeiros, contábeis ou científicos.
- **Codificação:** Usa BCD compactado (*Packed Decimal*) conforme o padrão industrial da computação financeira.

### 2.5 Gerador de Código C99 (`compiler/upl_codegen.c`, `compiler/upl_codegen.h`)

- **Objetivo:** Emite código-fonte ANSI C99 limpo, legível e conforme ISO/IEC.
- **Estratégia:** Traduz nós AST para expressões e instruções C99 equivalentes, emitindo blocos `embed` na posição exata definida pelo programador.

### 2.6 Driver da Toolchain (`compiler/main.c`)

- **Parâmetros suportados:**
  - `-o <saída>`: Nome do executável binário final.
  - `--emit-c`: Gera apenas o código C99 intermediário.
  - `--dump-ast`: Exibe a árvore sintática para depuração.
  - `--dump-tokens`: Exibe o fluxo léxico gerado pelo lexer.
  - `--version`: Exibe a versão oficial da toolchain.

---

## 3. Integração com o Sistema Operacional

- No Windows: [`upl.bat`](../../upl.bat) (invoca `gcc.exe` do MinGW-w64)
- No Linux / macOS: [`upl.sh`](../../upl.sh) (invoca `gcc` ou `clang`)

Zero dependências de Python, Node.js ou ferramentas de build externas como CMake ou Make.

---

## 4. Ver Também

- [Guia de Início Rápido](QUICKSTART.md)
- [Sintaxe e Gramática](SYNTAX_AND_GRAMMAR.md)
- [Índice Principal da Documentação](../INDEX.md)
