# Manual Técnico de Sintaxis y Comandos: Lenguaje UPL+
## Especificación de Programación de Sistemas y Toolchain Nativa

**Autor:** José Manuel Moreno (Noxferion)  
**Versión:** 2.0.0-Release  
**Clasificación:** Lenguaje de Programación de Sistemas Compilado (ANSI C99 Backend)  
**Licencia:** Registrado en SafeCreative

---

## 1. Naturaleza Técnica de UPL+

UPL+ es un **lenguaje de programación de sistemas fuertemente tipado y determinista**, dotado de una toolchain de compilación autónoma implementada en C99 estándar:

1. **Analizador Léxico Bilingüe (`compiler/lexer.c`):** Escanea el flujo UTF-8, genera tokens tipados con tracking de línea y columna, admite sintaxis equivalente en español e inglés, soporta literales enteros (decimales, hexadecimales `0x...`, binarios `0b...`), punto flotante, cadenas con secuencias de escape y captura bloques crudos de código host (`embed`).
2. **Asignador de Memoria por Arenas (`compiler/upl_arena.c`):** Bloques contiguos de 64 KB para alojar todos los nodos sintácticos del programa con asignación $O(1)$ y liberación instantánea al finalizar la traducción.
3. **Analizador Sintáctico y AST (`compiler/upl_parser.c`):** Implementa descenso recursivo para declaraciones/sentencias y el algoritmo de Pratt (*Precedence Climbing*) para expresiones con estricta precedencia matemática.
4. **Motor Aritmético Decimal COMP-3 BCD (`compiler/runtime/upl_comp3.c`):** Motor de software para aritmética de punto fijo en formato BCD empaquetado (4 bits por dígito con signos `+0x0C` y `-0x0D`), eliminando los errores de redondeo de la norma IEEE-754.
5. **Backend Generador ANSI C99 (`compiler/upl_codegen.c`):** Emite código C99 estricto para compilar directamente con GCC o Clang en binarios nativos de 64 bits ejecutables a velocidad de silicio.

---

## 2. Comandos de la Toolchain (`upl` / `uplc`)

La interacción con el compilador se realiza mediante los scripts lanzadores multiplataforma:
* **Windows:** `upl.bat` (o directamente `bin\uplc.exe`)
* **Linux / POSIX:** `./upl.sh` (o `./bin/uplc`)

### Sintaxis de Invocación
```bash
upl.bat [opciones] <archivo.upl>
```

### Tabla de Opciones Disponibles

| Opción | Descripción | Ejemplo de Uso |
|---|---|---|
| `-o <salida.exe>` | Compila y enlaza el binario nativo ejecutable final | `upl.bat -o mi_programa.exe codigo.upl` |
| `-c`, `--emit-c` | Emite únicamente el código C99 intermedio sin llamar al linker | `upl.bat -c codigo.upl` |
| `--dump-ast` | Imprime el Árbol de Sintaxis Abstracta (AST) en consola | `upl.bat --dump-ast codigo.upl` |
| `--dump-tokens` | Imprime la secuencia de tokens léxicos bilingües | `upl.bat --dump-tokens codigo.upl` |
| `-v`, `--version` | Muestra la versión del compilador y arquitectura host | `upl.bat --version` |

### Arnés de Pruebas Automatizado
Para validar la integridad de todas las fases del compilador:
```bash
test.bat
```

---

## 3. Tipos de Datos Primitivos y de Precisión

| Tipo UPL+ | Tamaño | Categoría | Rango y Valores |
|---|---|---|---|
| `u8` | 1 byte (8 bits) | Entero sin signo | 0 a 255 |
| `u16` | 2 bytes (16 bits) | Entero sin signo | 0 a 65,535 |
| `u32` | 4 bytes (32 bits) | Entero sin signo | 0 a 4,294,967,295 |
| `u64` | 8 bytes (64 bits) | Entero sin signo | 0 a $2^{64}-1$ |
| `usize` | 8 bytes (64 bits) | Tamaño de arquitectura | Direcciones de memoria física y punteros |
| `i8`, `i16`, `i32`, `i64` | 1, 2, 4, 8 bytes | Enteros con signo | Rango estándar complemento a 2 |
| `f32`, `f64` | 4, 8 bytes | Coma flotante IEEE-754 | Simple y doble precisión |
| `bool` | 1 byte | Lógico | `true` (`verdadero`) o `false` (`falso`) |
| `str` | Puntero a buffer | Cadena de texto | Buffer UTF-8 terminado en nulo |
| `void` | 0 bytes | Tipo vacío | Retorno de funciones sin valor |
| `comp3(P, S)` | Packed Decimal | Decimal exacto BCD | Precisión ($P$) y escala ($S$) en formato financiero |
| `*T` | 8 bytes | Puntero tipado a memoria | Punteros crudos de bajo nivel (`*u32`, `*u8`) |

---

## 4. Tabla de Palabras Clave y Equivalencia Bilingüe

El lexer unifica de forma idéntica las palabras clave técnicas en español e inglés:

| Inglés (EN) | Español (ES) | Semántica y Uso |
|---|---|---|
| `let` | `sea` | Declara una variable local inmutable por defecto. |
| `mut` | `mut` | Modificador que permite que una variable sea mutable. |
| `const` | `constante` | Define una constante inmutable en tiempo de compilación. |
| `fn` | `funcion` | Declara una función o rutina tipada. |
| `struct` | `estructura` | Declara una estructura de datos agregada. |
| `if` | `si` | Bifurcación condicional (`if cond { ... }` o `if (cond) { ... }`). |
| `else` | `sino` | Rama alternativa de la bifurcación condicional. |
| `while` | `mientras` | Bucle de iteración pre-condicional. |
| `for` | `para` | Bucle de iteración estructurado. |
| `return` | `retornar` | Retorna el control y un valor opcional desde la función. |
| `pub` | `publico` | Modificador de visibilidad exportada para módulos. |
| `module` | `modulo` | Define el espacio de nombres de la unidad de compilación. |
| `import` | `importar` | Declara la dependencia de un módulo externo. |
| `as` | `como` | Conversión explícita de tipos (Cast). |
| `embed` | `incrustar` | Bloque literal de código C/Win32/POSIX nativo. |
| `asm` | `ensamblador` | Bloque de instrucciones en ensamblador inline. |

---

## 5. Directivas de Sistemas y Atributos (`@`)

Las directivas comienzan con el token `@` y configuran atributos de compilación sobre funciones y estructuras:
* `@baremetal`: Indica que la función opera sin dependencias de librerías del sistema operativo anfitrión.
* `@snapshot`: Marca estructuras y funciones destinadas a puntos de control y serialización de estado en memoria.
* `@pure`: Marca funciones deterministas que no producen efectos secundarios.

---

## 6. Ejemplos Prácticos de Programación en UPL+

### Ejemplo 1: Cálculo con Tipos BCD Exactos (`comp3`)
```rust
modulo finanzas;

incrustar "c" {
#include <stdio.h>
}

funcion principal() -> i32 {
    sea base: i32 = 1000;
    sea tasa_iva: i32 = 21;
    sea mut total: i32 = 0;

    total = base + (base * tasa_iva / 100);

    incrustar "c" {
        printf("[FINANZAS] Subtotal: 1000 EUR, Total con IVA: %d EUR\n", total);
    }

    retornar 0;
}
```

### Ejemplo 2: Interacción con la API de Windows NT (Control de Memoria)
```rust
module control_memoria;

embed "c" {
#include <windows.h>
#include <psapi.h>
#include <stdio.h>
}

pub fn liberar_working_set(pid: u32) -> bool {
    let mut exito: bool = false;

    embed "c" {
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_SET_QUOTA, FALSE, pid);
        if (hProcess) {
            if (K32EmptyWorkingSet(hProcess)) {
                printf("[WATCHDOG] Working set liberado exitosamente para PID: %u\n", pid);
                exito = true;
            }
            CloseHandle(hProcess);
        }
    }

    return exito;
}
```

### Ejemplo 3: Estructuras y Control de Flujo
```rust
modulo estructuras_demo;

estructura Sensor {
    id: u32,
    valor: i32,
    activo: bool
}

funcion evaluar_sensor(s: *Sensor) -> bool {
    si (*s).activo && (*s).valor > 100 {
        retornar true;
    }
    retornar false;
}
```
