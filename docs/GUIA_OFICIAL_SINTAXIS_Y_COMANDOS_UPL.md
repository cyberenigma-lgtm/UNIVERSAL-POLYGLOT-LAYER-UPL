# 📖 MANUAL OFICIAL DEL LENGUAJE UPL (UNIVERSAL POLYGLOT LAYER)
### Versión 1.0 Soberana — Arquitectura de Sistemas y Computación Crítica

---

## 1. ¿Qué es UPL y por qué es un Lenguaje Real y no un Script?

Un "script" suele ser un archivo de comandos que simplemente invoca programas de terceros (como un `.bat` o un script de automatización). 

**UPL es un Lenguaje de Programación Autónomo de Primer Nivel** porque cuenta con una arquitectura de compilación completa:

1. **Gramática Formal Propietaria:** Posee su propio conjunto de tokens, reglas sintácticas, inmutabilidad estricta y precedencia de operadores.
2. **Analizador Léxico Propio (Lexer):** Lee el código fuente `.upl` carácter por carácter y clasifica tokens numéricos, identificadores, tipos, directivas y cadenas.
3. **Analizador Sintáctico Propio (Parser & AST):** Construye un Árbol de Sintaxis Abstracta tipado, validando la lógica antes de emitir cualquier instrucción.
4. **Generador de Ensamblador Puro (Backend x86_64 NASM):** Genera código ensamblador en Long Mode de 64 bits directo para el procesador (Ring 0 / Bare-metal), sin runtime ni dependencias externas.
5. **Generador de C99 de Alto Rendimiento:** Emite código C nativo con structs empaquetados e inlining óptimo.
6. **Binarios Nativos Independientes:** Una vez compilado un archivo `.upl`, se genera un ejecutable nativo (`.exe` o `.elf` de Ring 0) que se ejecuta a velocidad de silicio puro, sin necesidad de Python ni de intérpretes externos.

---

## 2. Comandos del Compilador UPL (`upl` / `uplc`)

El compilador de UPL dispone de una interfaz de línea de comandos unificada:

| Comando | Descripción | Ejemplo de Uso |
|---|---|---|
| `upl run <archivo.upl>` | Compila a binario nativo y lo ejecuta inmediatamente en un solo paso. | `upl run mi_programa.upl` |
| `upl build <archivo.upl> -t c` | Compila el archivo UPL y genera código fuente C99 optimizado. | `upl build kernel.upl -t c` |
| `upl build <archivo.upl> -t asm` | Compila el archivo UPL y genera Ensamblador x86_64 puro (NASM). | `upl build kernel.upl -t asm` |
| `upl build <archivo.upl> --baremetal` | Genera código sin librerías de usuario, apto para Ring 0 (Kernel/Boot). | `upl build kernel.upl -t asm --baremetal` |
| `upl repl` | Inicia la Consola Interactiva en vivo de UPL con inspección de código. | `upl repl` |
| `upl diag <codigo> [-l idioma]` | Analiza códigos de error de Windows/Linux/CPU en 57 idiomas. | `upl diag 0xC0000005 -l es` |

### Opciones y Flags Globales:
* `-o <salida>`: Especifica la ruta y nombre del archivo binario o código resultante.
* `-t <c | asm>`: Selecciona el backend objetivo (`c` para ANSI C99, `asm` para NASM x86_64).
* `--baremetal`: Activa el modo de silicio directo (elimina encabezados estándar y llamadas a librerías de usuario).
* `-l, --lang <codigo>`: Establece el idioma de los mensajes y diagnósticos (soporta 57 lenguajes: `es`, `en`, `de`, `fr`, `ja`, `zh`, etc.).

---

## 3. Tipos de Datos Primitivos y Soberanos

| Tipo UPL | Tamaño | Descripción | Rango / Valores |
|---|---|---|---|
| `u8` | 1 byte (8 bits) | Entero sin signo | 0 a 255 |
| `u16` | 2 bytes (16 bits) | Entero sin signo (puertos I/O) | 0 a 65,535 |
| `u32` | 4 bytes (32 bits) | Entero sin signo (RGBA, punteros 32b) | 0 a 4,294,967,295 |
| `u64` | 8 bytes (64 bits) | Entero sin signo de 64 bits (direcciones de memoria) | 0 a 18 trillones |
| `usize` | 8 bytes (64 bits) | Tamaño de índice de memoria de la arquitectura | 0 a $2^{64}-1$ |
| `i8`, `i16`, `i32`, `i64` | 1, 2, 4, 8 bytes | Enteros con signo | Negativos y positivos |
| `f32`, `f64` | 4, 8 bytes | Coma flotante IEEE 754 | Decimales estándar |
| `bool` | 1 byte | Booleano lógico | `true` (`verdadero`) o `false` (`falso`) |
| `str` | Variable | Cadena de texto UTF-8 | `"Texto soberano"` |
| `void` | 0 bytes | Ausencia de valor de retorno | Funciones sin retorno |
| `comp3` / `decimal` | BCD Empaquetado | **Aritmética exacta NeuroCOBOL** sin error de coma flotante | Cálculos financieros y cuánticos |
| `*T` | 8 bytes | Puntero crudo a memoria de tipo T | `*u32`, `*u8` |
| `[T; N]` | N * sizeof(T) | Array estático de tamaño fijo | `[u32; 10]` |

---

## 4. Tabla de Palabras Clave y Sintaxis Bilingüe

UPL es el primer lenguaje del mundo con **soberanía bilingüe nativa**: puedes escribir tu código en inglés universal o en español técnico:

| Inglés Universal | Español Soberano | Significado / Uso |
|---|---|---|
| `let` | `sea` / `variable` | Declara una variable local (inmutable por defecto). |
| `mut` | `mutable` | Permite que una variable pueda modificar su valor. |
| `const` | `constante` | Define una constante inmutable evaluada en tiempo de compilación. |
| `fn` | `funcion` | Declara una función o subrutina. |
| `struct` | `estructura` | Declara una estructura de datos empaquetada. |
| `if` | `si` | Condicional lógico. |
| `else` | `sino` | Rama alternativa del condicional. |
| `while` | `mientras` | Bucle condicional. |
| `for` ... `in` | `para` ... `en` | Bucle de iteración sobre rangos. |
| `return` | `retornar` | Devuelve un valor desde una función. |
| `pub` | `publico` | Exporta la función o estructura fuera del módulo. |
| `module` | `modulo` | Define el espacio de nombres del archivo. |
| `as` | `como` | Conversión explícita de tipos (Casting). |
| `asm` | `asm` | Bloque de instrucciones en Ensamblador inline para la CPU. |
| `embed` | `embed` | Bloque políglota incrustado (C, Python, COBOL, etc.). |
| `and` / `&&` | `and` / `&&` | Operador lógico 'Y'. |
| `or` / `\|\|` | `or` / `\|\|` | Operador lógico 'O'. |

---

## 5. Directivas Especiales para Sistemas Operativos (Silicon Directives)

* `@baremetal`: Indica al compilador que la función se ejecutará en **Ring 0**. Desactiva las llamadas al sistema operativo anfitrión y permite instrucciones privilegiadas de CPU (`cli`, `sti`, `inb`, `outb`, `hlt`, `invlpg`).
* `@snapshot`: Activa la captura de estado atómica y la creación de puntos de control (*Checkpoints*) con recuperación ante fallos.
* `@dvtrgas30` / `@vectorial`: Aplica optimizaciones de vectorización y sella los bloques de memoria con paridad canónica `0xAA30`.

---

## 6. Ejemplos Prácticos de Código UPL

### Ejemplo 1: Sintaxis Estándar y NeuroCOBOL (Modo Usuario)
```rust
module matematicas::finanzas;

// Estructura con tipado exacto
pub struct CuentaBancaria {
    id_cuenta: u32,
    titular: str,
    saldo: comp3(12, 2),
    activa: bool
}

pub fn calcular_interes(monto: comp3(12, 2), tasa_porcentaje: u32) -> comp3(12, 2) {
    let mut resultado: comp3(12, 2) = monto * (tasa_porcentaje as comp3(12, 2)) / 100
    return resultado
}
```

### Ejemplo 2: Programación Soberana en Español
```rust
modulo sistema::reloj;

constante FRECUENCIA_BASE: u32 = 1193182

funcion calcular_divisor(hercios_deseados: u32) -> u16 {
    sea mutable divisor: u32 = FRECUENCIA_BASE / hercios_deseados
    si divisor > 65535 {
        divisor = 65535
    }
    retornar divisor como u16
}
```

### Ejemplo 3: Programación de Kernel y Acceso a Hardware (Ring 0)
```rust
module kernel::controlador_vga;

pub const DIRECCION_VGA_BUFFER: u64 = 0xB8000;

@baremetal
pub fn limpiar_pantalla_vga(caracter: u8, color_atributo: u8) -> void {
    let ptr: *u16 = DIRECCION_VGA_BUFFER as *u16;
    let entrada: u16 = ((color_atributo as u16) << 8) | (caracter as u16);
    
    let mut i: u64 = 0;
    while i < 2000 {
        *(ptr + i) = entrada;
        i = i + 1;
    }

    // Instrucción directa a la CPU
    asm {
        sti
    }
}
```

---

## 7. Comandos de la Consola Interactiva (REPL)

Al ejecutar `upl repl`, entras al entorno interactivo:
* `>>> let x: u32 = 42;` -> Evalúa y guarda en el entorno de sesión.
* `>>> :c` -> Muestra el código C99 emitido para la sesión actual.
* `>>> :asm` -> Muestra el Ensamblador x86_64 NASM generado para la sesión.
* `>>> :lang en` -> Cambia el idioma de los mensajes a inglés.
* `>>> :diag 0xC0000096` -> Muestra el diagnóstico del código de error.
* `>>> :exit` o `exit` -> Sale del REPL.
