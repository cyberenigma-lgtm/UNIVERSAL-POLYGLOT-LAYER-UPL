# UPL+ Syntax and Grammar Specification (v2.3.0)

> **Language / Idioma:** 🇬🇧 English | [🇪🇸 Español](../SPEC_GRAMATICA_Y_SISTEMA_TIPOS.md) | [🇨🇳 中文](../zh/SYNTAX_AND_GRAMMAR.md) | [🇫🇷 Français](../fr/SYNTAX_AND_GRAMMAR.md) | [🇵🇹 Português](../pt/SYNTAX_AND_GRAMMAR.md)

**Author:** José Manuel Moreno (Noxferion)  
**Classification:** Systems Programming Language / Typed C99 Transpilation

---

## 1. Formal Lexical Grammar (EBNF)

### 1.1 Characters and Whitespace

```ebnf
source_char     = ? Any UTF-8 Unicode code point ? ;
whitespace      = ' ' | '\t' | '\r' | '\n' ;
newline         = '\n' | '\r\n' ;

line_comment    = '//' , { source_char - newline } , newline ;
block_comment   = '/*' , { source_char } , '*/' ;
line_directive  = '#' , { source_char - newline } , newline ;
```

### 1.2 Identifiers and Bilingual Keywords

Identifiers start with an ASCII letter or underscore (`_`), followed by alphanumeric characters or underscore.

```ebnf
letter          = 'a'..'z' | 'A'..'Z' | '_' ;
digit           = '0'..'9' ;
hex_digit       = digit | 'a'..'f' | 'A'..'F' ;
bin_digit       = '0' | '1' ;
identifier      = letter , { letter | digit } ;
```

#### Bilingual Keyword Equivalence Table

The lexer deterministically maps English and Spanish keywords to the same token identifier (`UPLTokenType`):

| Lexical Token | Keyword (EN) | Keyword (ES) | Semantics |
|---|---|---|---|
| `TOK_MODULE` | `module` | `modulo` | Compilation unit namespace declaration |
| `TOK_IMPORT` | `import` | `importar` | External module or unit inclusion |
| `TOK_STRUCT` | `struct` | `estructura` | Aggregate data structure definition |
| `TOK_PUB` | `pub` | `publico` | Exportable visibility modifier |
| `TOK_FN` | `fn` | `funcion` | Subroutine declaration or definition |
| `TOK_LET` | `let` | `sea` | Immutable-by-default variable declaration |
| `TOK_MUT` | `mut` | `mut` | Mutability modifier for variables |
| `TOK_CONST` | `const` | `constante` | Compile-time immutable constant |
| `TOK_IF` | `if` | `si` | Conditional branch |
| `TOK_ELSE` | `else` | `sino` | Alternative branch of conditional |
| `TOK_WHILE` | `while` | `mientras` | Pre-test conditional iteration loop |
| `TOK_FOR` | `for` | `para` | Structured iteration loop |
| `TOK_RETURN` | `return` | `retornar` | Return control and value from a function |
| `TOK_AS` | `as` | `como` | Explicit type conversion (cast) |
| `TOK_EMBED` | `embed` | `incrustar` | Literal native code block (C or host language) |
| `TOK_ASM` | `asm` | `ensamblador` | Inline assembly instruction block |

### 1.3 Literals

```ebnf
int_literal     = digit , { digit }
                | '0x' , hex_digit , { hex_digit }
                | '0b' , bin_digit , { bin_digit } ;

float_literal   = digit , { digit } , '.' , digit , { digit } ,
                  [ ( 'e' | 'E' ) , [ '+' | '-' ] , digit , { digit } ] ;

escape_char     = '\\' , ( 'n' | 'r' | 't' | '\\' | '"' | "'" | '0' |
                  ( 'x' , hex_digit , hex_digit ) ) ;
string_literal  = '"' , { ( source_char - ( '"' | '\\' ) ) | escape_char } , '"' ;
```

### 1.4 Operators and Delimiters

```ebnf
assign_op       = '=' | '+=' | '-=' | '*=' | '/=' ;
arith_op        = '+' | '-' | '*' | '/' | '%' ;
bitwise_op      = '&' | '|' | '^' | '~' | '<<' | '>>' ;
compare_op      = '==' | '!=' | '<' | '<=' | '>' | '>=' ;
logical_op      = '&&' | '||' | '!' ;
arrow_op        = '->' ;
delimiter       = '(' | ')' | '{' | '}' | '[' | ']' | ':' | ';' | ',' | '.' ;
```

---

## 2. Syntactic Grammar (EBNF)

### 2.1 Program and Module

```ebnf
program         = module_decl , { import_decl } , { top_level_decl } ;
module_decl     = ( 'module' | 'modulo' ) , identifier , ';' ;
import_decl     = ( 'import' | 'importar' ) , module_path , ';' ;
module_path     = identifier , { '/' , identifier } ;
top_level_decl  = fn_decl | struct_decl | var_decl ;
```

### 2.2 Function Declarations

```ebnf
fn_decl         = [ 'pub' | 'publico' ] , ( 'fn' | 'funcion' ) ,
                  identifier , '(' , param_list , ')' ,
                  [ '->' , type_expr ] , block_stmt ;
param_list      = [ param , { ',' , param } ] ;
param           = identifier , ':' , type_expr ;
```

### 2.3 Statements

```ebnf
statement       = var_decl_stmt
                | assign_stmt
                | if_stmt
                | while_stmt
                | return_stmt
                | embed_block
                | expr_stmt ;

var_decl_stmt   = ( 'let' | 'sea' ) , [ 'mut' ] , identifier ,
                  [ ':' , type_expr ] , '=' , expression , ';' ;

if_stmt         = ( 'if' | 'si' ) , expression , block_stmt ,
                  [ ( 'else' | 'sino' ) , block_stmt ] ;

while_stmt      = ( 'while' | 'mientras' ) , expression , block_stmt ;

return_stmt     = ( 'return' | 'retornar' ) , [ expression ] , ';' ;

embed_block     = ( 'embed' | 'incrustar' ) , string_literal , '{' ,
                  raw_code , '}' ;

block_stmt      = '{' , { statement } , '}' ;
```

### 2.4 Expressions (Pratt precedence, lowest to highest)

```ebnf
expression      = assignment_expr ;
assignment_expr = logic_or_expr , [ assign_op , assignment_expr ] ;
logic_or_expr   = logic_and_expr , { '||' , logic_and_expr } ;
logic_and_expr  = compare_expr , { '&&' , compare_expr } ;
compare_expr    = add_expr , { compare_op , add_expr } ;
add_expr        = mul_expr , { ( '+' | '-' ) , mul_expr } ;
mul_expr        = unary_expr , { ( '*' | '/' | '%' ) , unary_expr } ;
unary_expr      = ( '!' | '-' ) , unary_expr | postfix_expr ;
postfix_expr    = primary_expr , { call_suffix | field_access } ;
call_suffix     = '(' , [ expression , { ',' , expression } ] , ')' ;
field_access    = '.' , identifier ;
primary_expr    = identifier | int_literal | float_literal |
                  string_literal | 'true' | 'false' |
                  '(' , expression , ')' ;
```

---

## 3. Type System

### 3.1 Primitive Types

| Type | Size | Description |
|------|------|-------------|
| `i8` | 1 byte | Signed 8-bit integer |
| `i16` | 2 bytes | Signed 16-bit integer |
| `i32` | 4 bytes | Signed 32-bit integer |
| `i64` | 8 bytes | Signed 64-bit integer |
| `u8` | 1 byte | Unsigned 8-bit integer |
| `u16` | 2 bytes | Unsigned 16-bit integer |
| `u32` | 4 bytes | Unsigned 32-bit integer |
| `u64` | 8 bytes | Unsigned 64-bit integer |
| `f32` | 4 bytes | IEEE 754 single-precision float |
| `f64` | 8 bytes | IEEE 754 double-precision float |
| `bool` | 1 byte | Boolean (`true` / `false`) |
| `str` | pointer + length | UTF-8 string slice |
| `void` | — | No value (function return) |

### 3.2 Compound Types

```upl
// Structures / Estructuras
struct Vector3 {
    x: f64,
    y: f64,
    z: f64,
}

// Arrays / Arreglos
let buffer: [u8; 1024]

// Pointers / Punteros
let raw_ptr: *u8
let const_ptr: *const u8
```

### 3.3 Ownership and `defer`

```upl
fn process() -> i64 {
    let handle = open_resource()
    defer close_resource(handle)   // Guaranteed release on scope exit
    // ... use handle ...
    return 0
}
```

---

## 4. Native Interoperability (`embed`)

```upl
module interop

fn native_example() -> i64 {
    embed "c" {
        #include <stdio.h>
        #include <stdlib.h>

        printf("Native C99 from UPL+\n");
        int result = abs(-42);
        return (long long)result;
    }
}
```

---

## 5. See Also

- [Quickstart Guide](QUICKSTART.md)
- [Compiler Architecture](COMPILER_ARCHITECTURE.md)
- [Master Documentation Index](../INDEX.md)
