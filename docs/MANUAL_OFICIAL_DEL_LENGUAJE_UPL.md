# ============================================================================
# MANUAL OFICIAL DEL LENGUAJE DE PROGRAMACIÓN UPL+
# UNIVERSAL POLYGLOT LAYER (UPL-v5 SPECIFICATION)
# ============================================================================
# Autor Fundador: José Manuel Moreno (Noxferion)
# Arquitectura: C99 Freestanding / Zero-CRT Runtime / Silicio Puro
# Estándar de Compilador: Dragon Book Formal Lexer & Pratt AST Parser
# ============================================================================

---

## ÍNDICE GENERAL

1. [CAPÍTULO 1: INTRODUCCIÓN Y MANIFIESTO SOBERANO](#capítulo-1-introducción-y-manifiesto-soberano)
2. [CAPÍTULO 2: SINTAXIS BILINGÜE Y GRAMÁTICA FORMAL](#capítulo-2-sintaxis-bilingüe-y-gramática-formal)
3. [CAPÍTULO 3: SISTEMA DE TIPOS Y OPERADORES DE SILICIO](#capítulo-3-sistema-de-tipos-y-operadores-de-silicio)
4. [CAPÍTULO 4: LA BIBLIOTECA ESTÁNDAR POR MÓDULOS (STD)](#capítulo-4-la-biblioteca-estándar-por-módulos-std)
   - 4.1. Sistema de Archivos y Carpetas (`std_fs`)
   - 4.2. Diccionarios y Tablas Hash $O(1)$ (`std_dict`)
   - 4.3. Memoria Soberana y Canarios (`std_memory`)
   - 4.4. Matemáticas y Vectores (`std_math`)
   - 4.5. E/S y Redireccionamiento de Flujos (`std_io`)
5. [CAPÍTULO 5: FUSIÓN POLÍGLOTA: ENSAMBLADOR Y C EMBED](#capítulo-5-fusión-políglota-ensamblador-y-c-embed)
6. [CAPÍTULO 6: ENTORNO DE VIRTUALIZACIÓN Y JIT (NEUROWILL / NUASM)](#capítulo-6-entorno-de-virtualización-y-jit-neurowill--nuasm)
7. [CAPÍTULO 7: MODOS DE EMPLEO DEL COMPILADOR (`uplc`)](#capítulo-7-modos-de-empleo-del-compilador-uplc)
8. [CAPÍTULO 8: TABLA MAESTRA DE ERRORES Y SOLUCIONES](#capítulo-8-tabla-maestra-de-errores-y-soluciones)

---

## CAPÍTULO 1: INTRODUCCIÓN Y MANIFIESTO SOBERANO

### 1.1 ¿Qué es UPL+?
**Universal Polyglot Layer (UPL+)** es un lenguaje de programación de sistemas de nueva generación diseñado para garantizar **soberanía tecnológica absoluta, rendimiento a nivel de silicio y cero dependencias de terceros**.

A diferencia de los lenguajes convencionales (C, C++, Rust, Python) que obligan al desarrollador a someterse a monopolios anglosajones, cadenas de montaje gigantescas (Visual Studio de 20 GB, Cargo, LLVM) y capas infinitas de drivers lentos, UPL+ opera como una **navaja suiza autocontenida**.

### 1.2 Principios Fundamentales
1. **Cero Dependencias Prestadas:** El compilador (`uplc.exe`) pesa menos de 300 KB y compila programas nativos ejecutables en **menos de 50 milisegundos**.
2. **Bilingüismo Nativo Real:** Puedes programar indistintamente en español o en inglés dentro del mismo archivo fuente.
3. **Fusión Políglota Sin Coste:** Integra bloques de alto nivel, ensamblador x86_64, código C99 directo y tipos financieros COBOL (`comp3`) sin herramientas externas.
4. **Determinismo y Seguridad de Silicio:** Memoria protegida con canarios criptográficos (`0xDEADBEEF`), tipos estáticos y resolución automática de punteros en tiempo de compilación.

---

## CAPÍTULO 2: SINTAXIS BILINGÜE Y GRAMÁTICA FORMAL

UPL+ reconoce dos dialectos de palabras clave a nivel léxico. Ambos se compilan al mismo Árbol de Sintaxis Abstracta (AST) idéntico.

### Tabla de Equivalencias de Palabras Clave

| Dialecto Español | Dialecto Inglés | Significado Semántico |
| :--- | :--- | :--- |
| `modulo` | `module` | Declara el espacio de nombres del módulo actual |
| `funcion` | `fn` | Declara una función o rutina |
| `publico` / `pub` | `pub` | Exporta la función o struct como símbolo público en DLL/EXE |
| `sea` | `let` | Declara una constante local inmutable |
| `sea mut` | `let mut` | Declara una variable mutable local |
| `si` | `if` | Bifurcación condicional |
| `sino` | `else` | Rama alternativa de la condición |
| `mientras` | `while` | Bucle de repetición condicional |
| `para` | `for` | Bucle iterativo de conteo |
| `retornar` | `return` | Devuelve el control y un valor de la función |
| `romper` | `break` | Interrumpe el bucle actual |
| `continuar` | `continue` | Salta a la siguiente iteración del bucle |
| `estructura` | `struct` | Agregado de datos heterogéneo en memoria |
| `como` | `as` | Conversión explícita de tipos (Casting) |
| `ensamblador` | `asm` | Bloque de instrucciones de silicio ensamblador |
| `incrustar` | `embed` | Bloque de código C99 nativo directo |

---

## CAPÍTULO 3: SISTEMA DE TIPOS Y OPERADORES DE SILICIO

### 3.1 Tipos Primitivos y Escalares

| Tipo UPL | Equivalente C99 | Tamaño en Bits | Rango de Valores |
| :--- | :--- | :--- | :--- |
| `u8` | `uint8_t` | 8 bits | $0$ a $255$ |
| `u16` | `uint16_t` | 16 bits | $0$ a $65,535$ |
| `u32` | `uint32_t` | 32 bits | $0$ a $4,294,967,295$ |
| `u64` | `uint64_t` | 64 bits | $0$ a $18,446,744,073,709,551,615$ |
| `i8` | `int8_t` | 8 bits | $-128$ a $+127$ |
| `i16` | `int16_t` | 16 bits | $-32,768$ a $+32,767$ |
| `i32` | `int32_t` | 32 bits | $-2,147,483,648$ a $+2,147,483,647$ |
| `i64` | `int64_t` | 64 bits | $-9.22 \times 10^{18}$ a $+9.22 \times 10^{18}$ |
| `usize` | `size_t` | 64 bits (x64) | Puntero entero de silicio |
| `f32` | `float` | 32 bits | Coma flotante IEEE-754 simple |
| `f64` | `double` | 64 bits | Coma flotante IEEE-754 doble |
| `bool` | `bool` | 8 bits | `true` (1) o `false` (0) |
| `str` | `const char*` | 64 bits | Puntero a cadena inmutable terminada en cero |
| `*T` | `T*` | 64 bits | Puntero directo a dirección de memoria de tipo `T` |
| `[T; N]` | `T[N]` | $N \times sizeof(T)$ | Array estático contiguo de longitud fija $N$ |
| `comp3(P, S)`| `uint8_t[(P/2)+1]`| Variable | Decimal empaquetado BCD financiero exacto |

### 3.2 Operadores Aritméticos, Lógicos y de Bits

UPL+ posee soporte formal para todos los operadores de cálculo a nivel de silicio:

| Operador | Significado | Ejemplo |
| :---: | :--- | :--- |
| `+` | Suma aritmética | `let x: i32 = a + b;` |
| `-` | Resta aritmética | `let x: i32 = a - b;` |
| `*` | Multiplicación | `let x: i32 = a * b;` |
| `/` | División entera o flotante | `let x: i32 = a / b;` |
| `%` | **Módulo / Resto de división** | `let resto: i32 = 100 % 7;` (Resultado: 2) |
| `<<` | **Desplazamiento a la izquierda** | `let val: u32 = 1 << 8;` (Resultado: 256) |
| `>>` | **Desplazamiento a la derecha** | `let val: u32 = 256 >> 3;` (Resultado: 32) |
| `~` | **Bitwise NOT (Inversión de bits)**| `let mask: u8 = ~0;` (Resultado: 255) |
| `&` | **Bitwise AND / Dirección de memoria**| `let p: *i32 = &val;` |
| `\|` | **Bitwise OR** | `let flags: u32 = a \| b;` |
| `^` | **Bitwise XOR** | `let diff: u32 = a ^ b;` |
| `==`, `!=` | Igualdad y Desigualdad | `si x == 0 { ... }` |
| `<`, `<=`, `>`, `>=` | Comparaciones algebraicas | `si (x como u32) >= 10 { ... }` |
| `+=`, `-=`, `*=`, `/=`, `%=` | Asignaciones compuestas | `x += 5;` |
| `<<=`, `>>=`, `&=`, `\|=`, `^=` | Asignaciones de bits | `mascara &= 0x0F;` |

---

## CAPÍTULO 4: LA BIBLIOTECA ESTÁNDAR POR MÓDULOS (STD)

La biblioteca estándar de UPL+ reside en `modules/std/` y proporciona soluciones inmediatas para los problemas cotidianos de la ingeniería de software.

### 4.1 Sistema de Archivos y Carpetas (`std_fs.upl`)

Proporciona el control completo sobre archivos y almacenamiento:

```upl
modulo ejemplo_archivos;

// 1. Guardar un buffer completo a disco
let datos: str = "CONFIG_PARAM=100";
let ok_save: bool = fs_guardar("config.ini", datos as *u8, 16);

// 2. Verificar existencia y tamano
si fs_existe("config.ini") {
    let bytes: u64 = fs_tamano("config.ini");
}

// 3. Cambiar la extension de un archivo
let mut nueva_ruta: [u8; 128];
fs_cambiar_extension("foto.bmp", ".gxp", &nueva_ruta[0], 128);

// 4. Renombrar archivo en disco
fs_cambiar_nombre("config.ini", "config_segura.ini");

// 5. Crear y borrar carpetas
fs_crear_carpeta("saves");
fs_borrar("temporal.dat");
```

### 4.2 Diccionarios y Tablas Hash $O(1)$ (`std_dict.upl`)

Estructura de clave-valor ultra-optimizada con algoritmo FNV-1a y sondeo lineal sin fragmentación de memoria:

```upl
modulo ejemplo_diccionario;

pub fn main() -> i32 {
    let mut tabla: Diccionario;
    dict_iniciar(&tabla);

    // Insercion O(1)
    let k: str = "nivel_jugador";
    dict_poner(&tabla, k as *u8, 13, 85);

    // Recuperacion O(1)
    let mut valor: u64 = 0;
    si dict_obtener(&tabla, k as *u8, 13, &valor) {
        // valor == 85
    }
    retornar 0;
}
```

### 4.3 Memoria Soberana y Canarios de Seguridad (`std_memory.upl`)

Operaciones deterministas de memoria con canarios anti-corrupción:
- `mem_copy(dest, src, count)`: Copia contigua segura sin desbordamiento.
- `mem_set(dest, val, count)`: Relleno rápido de memoria.
- `mem_cmp(a, b, count)`: Comparación lexicográfica exacta.
- `canary_write(addr, 0xDEADBEEF)`: Inyecta centinela de integridad.
- `canary_verify(addr, 0xDEADBEEF)`: Comprueba si hubo intrusión o desbordamiento de búfer.

### 4.4 Matemáticas y Vectores (`std_math.upl`)
- `abs_i32(x)`: Valor absoluto.
- `clamp_i32(val, min, max)`: Acotación de rango para coordenadas de pantalla.
- `lerp_f32(a, b, t)`: Interpolación lineal para animaciones de juegos.
- Constantes de precisión: `PI_F32` (3.14159265), `DEG_TO_RAD`, `RAD_TO_DEG`.

### 4.5 E/S y Redireccionamiento de Flujos (`std_io.upl`)
- `print_str(msg)` / `print_i32(val)` / `print_u64_hex(val)`.
- `io_redirigir_a_archivo(ruta)`: Desvía `stdout` a un archivo de disco en caliente.
- `io_restaurar_consola()`: Restaura la consola de terminal de forma atómica.

---

## CAPÍTULO 5: FUSIÓN POLÍGLOTA: ENSAMBLADOR Y C EMBED

En UPL+, no necesitas herramientas externas (MASM, NASM, CMake) para usar ensamblador o C:

```upl
modulo fusion_avanzada;

// 1. Incrustacion C99 directa
embed {
#include <windows.h>
void alerta(const char *txt) {
    MessageBoxA(NULL, txt, "Alerta UPL+", MB_OK);
}
}

// 2. Ensamblador de silicio directo dentro de funciones UPL
pub fn leer_ciclos_cpu() -> u64 {
    let mut ciclos: u64 = 0;
    asm {
        "rdtsc"
        "shl rdx, 32"
        "or rax, rdx"
    }
    return ciclos;
}
```

---

## CAPÍTULO 6: ENTORNO DE VIRTUALIZACIÓN Y JIT (NEUROWILL / NUASM)

UPL+ incluye módulos de virtualización y ejecución en RAM nativos:
1. **`neurowill_core.upl` (Máquina Virtual Autónoma):**
   - 16 registros virtuales de 64 bits (`r0` a `r15`).
   - Puntero de instrucción (`ip`), banderas `zero_flag` y `carry_flag`.
   - Control de límite de ciclos para evitar bucles infinitos en código no confiable.
2. **`nuasm_core.upl` (JIT & Opcode Encoder):**
   - Ensambla y emite código máquina x86_64 directamente a memoria RAM en caliente con `VirtualAlloc(PAGE_EXECUTE_READWRITE)` sin necesidad de compilar en disco.

---

## CAPÍTULO 7: MODOS DE EMPLEO DEL COMPILADOR (`uplc`)

El compilador de UPL+ se invoca desde la línea de comandos con sintaxis minimalista y potente:

```powershell
# 1. Compilar a Ejecutable Nativo (.exe)
uplc mi_programa.upl -o bin/mi_programa.exe

# 2. Compilar a Biblioteca Dinámica Compartida (.dll)
uplc motor_grafico.upl -o bin/motor_grafico.dll

# 3. Enlazar con Librerías del Sistema (-l, -L, -I)
uplc juego.upl -lwinmm -lgdi32 -luser32 -o bin/juego.exe

# 4. Exportar Código Fuente C99 Limpio (--emit-c)
uplc programa.upl --emit-c -o programa.c

# 5. Inspeccionar Árbol AST de Sintaxis (--dump-ast)
uplc programa.upl --dump-ast

# 6. Inspeccionar Tokens Léxicos (--dump-tokens)
uplc programa.upl --dump-tokens
```

---

## CAPÍTULO 8: TABLA MAESTRA DE ERRORES Y SOLUCIONES

Esta tabla diagnóstica permite resolver de inmediato cualquier error durante el desarrollo:

| Código de Error / Síntoma | Causa Raíz Probable | Solución Inmediata con Código |
| :--- | :--- | :--- |
| **`error: 'p' is a pointer; did you mean to use '->'?`** | En compiladores viejos, acceder a campos de punteros con `.` fallaba. | **Solucionado en UPL+ v2.0**: El compilador rastrea automáticamente los punteros y emite `->`. Puedes escribir `p.x = 10;` de forma transparente. |
| **`warning: initialization discards 'const' qualifier`** | Declarar un puntero mutable hacia una variable inmutable (`let val`). | Declara la variable origen como mutable: `let mut val: i32 = 42;` antes de tomar su dirección `&val`. |
| **`error: missing terminating " character in asm`** | Instrucciones de ensamblador con saltos de línea sin formatear dentro del bloque `asm { ... }`. | Usa una instrucción por línea. El generador `emit_asm_block` de UPL+ ahora sanea automáticamente las comillas. |
| **`Buffer overflow en NuasmEmitByte`** | El buffer JIT no tiene capacidad suficiente para las instrucciones ensambladas. | Inicializa con mayor tamaño en `NuasmInit(capacidad)` (por ejemplo, 65536 bytes en lugar de 1024). |
| **`Factor de carga maximo alcanzado en dict_poner`** | El diccionario superó los 240 elementos ocupados en la tabla fija. | Aumenta la constante `DICT_CAPACITY` en `std_dict.upl` a 1024 o crea diccionarios particionados. |
| **`Implicit declaration of function 'upl_setup_console_fullscreen'`** | La versión previa del transpilador no inyectaba las cabeceras de consola. | **Solucionado**: `uplc` ahora inyecta automáticamente el runtime de consola, pantalla completa y temporización en todo programa ejecutable. |
| **`error: condition parenthesis consumed prematurely`** | Expresiones condicionales que comienzan con un casting `si (x como u32) >= y`. | **Solucionado**: El parser Pratt formal analiza las subexpresiones parentizadas sin consumir erróneamente la condición del `si`/`mientras`. |
| **`Error de enlace: undefined reference to PlaySoundA`** | Se invoca una API multimedia externa sin pasar el flag de enlace. | Agrega el flag de enlace al compilar: `uplc juego.upl -lwinmm -o bin/juego.exe`. |

---

# CONCLUSIÓN DEL MANUAL

UPL+ no es un subconjunto ni un experimento: es una **plataforma completa de ingeniería de software soberana**.  
Diseñada para durar años sin requerir revisiones de emergencia, con pruebas automatizadas que verifican cada coma, cada tipo y cada instrucción de silicio.
