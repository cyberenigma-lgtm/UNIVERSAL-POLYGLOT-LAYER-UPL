# Especificación Técnica Formal: Lenguaje UPL (v2.0)
**Estado:** Borrador de Ingeniería Normativo  
**Autor:** José Manuel Moreno (Noxferion)  
**Clasificación:** Lenguaje de Programación de Sistemas / Transpilación Tipada  

---

## 1. Gramática Léxica Formal (EBNF)

### 1.1 Caracteres y Espacio en Blanco
```ebnf
caracter_fuente      = ? Cualquier punto de código Unicode UTF-8 ? ;
espacio_blanco       = ' ' | '\t' | '\r' | '\n' ;
nueva_linea          = '\n' | '\r\n' ;

comentario_linea     = '//' , { caracter_fuente - nueva_linea } , nueva_linea ;
comentario_bloque    = '/*' , { caracter_fuente } , '*/' ;
directiva_linea      = '#' , { caracter_fuente - nueva_linea } , nueva_linea ;
```

### 1.2 Identificadores y Palabras Clave Bilingües
Los identificadores inician con una letra ASCII o guión bajo (`_`), seguidos de caracteres alfanuméricos o guión bajo.
```ebnf
letra                = 'a'..'z' | 'A'..'Z' | '_' ;
digito               = '0'..'9' ;
digito_hex           = digito | 'a'..'f' | 'A'..'F' ;
digito_bin           = '0' | '1' ;
identificador        = letra , { letra | digito } ;
```

#### Tabla de Equivalencia Bilingüe de Palabras Clave
El analizador léxico mapea de forma determinista las palabras clave en inglés y español al mismo identificador de token (`UPLTokenType`):

| Token Léxico | Palabra Clave (EN) | Palabra Clave (ES) | Semántica |
|---|---|---|---|
| `TOK_MODULE` | `module` | `modulo` | Declaración del espacio de nombres de la unidad de compilación |
| `TOK_IMPORT` | `import` | `importar` | Inclusión de módulo o unidad externa |
| `TOK_STRUCT` | `struct` | `estructura` | Definición de estructura de datos agregada |
| `TOK_PUB` | `pub` | `publico` | Modificador de visibilidad exportable |
| `TOK_FN` | `fn` | `funcion` | Declaración o definición de subrutina |
| `TOK_LET` | `let` | `sea` | Declaración de variable inmutable por defecto |
| `TOK_MUT` | `mut` | `mut` | Modificador de mutabilidad para variables |
| `TOK_CONST` | `const` | `constante` | Constante inmutable evaluable en tiempo de compilación |
| `TOK_IF` | `if` | `si` | Bifurcación condicional |
| `TOK_ELSE` | `else` | `sino` | Rama alternativa de bifurcación condicional |
| `TOK_WHILE` | `while` | `mientras` | Bucle de iteración condicional pre-test |
| `TOK_FOR` | `for` | `para` | Bucle de iteración estructurado |
| `TOK_RETURN` | `return` | `retornar` | Retorno de control y valor desde una función |
| `TOK_AS` | `as` | `como` | Conversión explícita de tipos (cast) |
| `TOK_EMBED` | `embed` | `incrustar` | Bloque literal de código nativo (C o lenguaje host) |
| `TOK_ASM` | `asm` | `ensamblador` | Bloque de instrucciones en ensamblador inline |

---

### 1.3 Literales

```ebnf
literal_entero       = digito , { digito }
                     | '0x' , digito_hex , { digito_hex }
                     | '0b' , digito_bin , { digito_bin } ;

literal_flotante     = digito , { digito } , '.' , digito , { digito } , [ ( 'e' | 'E' ) , [ '+' | '-' ] , digito , { digito } ] ;

caracter_escape      = '\\' , ( 'n' | 'r' | 't' | '\\' | '"' | '\'' | '0' | ( 'x' , digito_hex , digito_hex ) ) ;
literal_cadena       = '"' , { ( caracter_fuente - ( '"' | '\\' ) ) | caracter_escape } , '"' ;
```

---

### 1.4 Operadores y Delimitadores

```ebnf
op_asignacion        = '=' | '+=' | '-=' | '*=' | '/=' ;
op_aritmetico        = '+' | '-' | '*' | '/' | '%' ;
op_bit_a_bit         = '&' | '|' | '^' | '~' | '<<' | '>>' ;
op_comparacion       = '==' | '!=' | '<' | '<=' | '>' | '>=' ;
op_logico            = '&&' | '||' | '!' ;
op_flecha            = '->' ;
delimitador          = '(' | ')' | '{' | '}' | '[' | ']' | ':' | ';' | ',' | '.' ;
```

---

## 2. Sistema de Tipos Formal

UPL implementa un sistema de tipos estático y con tipado fuerte. La asignación implícita entre tipos con pérdida de precisión está prohibida y requiere el operador `as` / `como`.

### 2.1 Tipos Primitivos Escalares
* **Enteros con signo:** `i8` (8 bits), `i16` (16 bits), `i32` (32 bits), `i64` (64 bits).
* **Enteros sin signo:** `u8` (8 bits), `u16` (16 bits), `u32` (32 bits), `u64` (64 bits), `usize` (tamaño de puntero según arquitectura: 32 o 64 bits).
* **Coma flotante IEEE 754:** `f32` (precisión simple, 32 bits), `f64` (doble precisión, 64 bits).
* **Booleano:** `bool` (representado como valor de 1 byte, valores literales: `true` / `verdadero`, `false` / `falso`).
* **Cadena inmutable:** `str` (puntero a secuencia de bytes terminada en byte nulo).

### 2.2 Tipos Financieros / Decimales Exactos: `comp3(precision, escala)`
* Implementación binaria de formato Packed Decimal (BCD empaquetado):
  - Formato: 2 dígitos decimales BCD de 4 bits por byte.
  - El último nibble de 4 bits almacena el signo: `0xC` positivo, `0xD` negativo, `0xF` sin signo.
  - Ejemplo: `comp3(5, 2)` almacena valores como `+123.45` en 3 bytes (`0x12 0x34 0x5C`).
  - No utiliza la FPU ni coma flotante; las operaciones aritméticas se computan por software mediante sumadores BCD con corrección decimal.

### 2.3 Tipos Compuestos y Referencias
* **Punteros explícitos:** `*T` (referencia a dirección de memoria del tipo base `T`).
* **Arreglos estáticos:** `[T; N]` (bloque contiguo de $N$ elementos de tipo $T$, asignado en el stack o en segmento de datos).
* **Estructuras:** Declaración explícita de campos con alineación natural según la ABI del procesador host.
