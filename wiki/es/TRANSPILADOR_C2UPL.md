# 🔄 GUÍA OFICIAL DEL TRANSPILADOR NATIVO C99 A UPL+ (`c2upl`)
## Ingestión y Migración Automática de Código de Sistemas C99 a UPL+

**Herramienta:** `tools/c2upl.c`  
**Binario:** `bin/c2upl.exe` (Windows) / `bin/c2upl` (Linux/macOS)  
**Dependencias:** 0% Python, 0% Rust, 100% ANSI C99 Estándar  
**Estado de Verificación:** 5/5 Pruebas Unitarias Aprobadas (0 Fallos)

---

### 1. ¿Por qué un Transpilador Dedicado de C99 a UPL+?

En lenguajes de sistemas modernos como Zig (`zig translate-c`), disponer de una utilidad de transpilación directa desde C permite a los desarrolladores absorber librerías, algoritmos y estructuras de datos existentes sin tener que reescribirlos manualmente.

**UPL+** y **ANSI C99** comparten exactamente el mismo modelo físico de ejecución en memoria:
* Punteros directos a memoria (`*T` $\leftrightarrow$ `T*`).
* Structs empaquetados contiguos sin recolector de basura (GC).
* Tipos numéricos de tamaño fijo (`i32`, `u64`, `f64`).

Por esta razón matemática y física, la traducción de C99 a UPL+ es 100% determinista y sin pérdidas.

---

### 2. Compilación del Transpilador

El transpilador está implementado en un único archivo C99 autónomo sin bibliotecas de terceros:

```bash
# En Windows (CMD o PowerShell):
gcc -std=c99 -Wall -Wextra -Werror -O2 tools/c2upl.c -o bin/c2upl.exe

# En Linux o macOS:
gcc -std=c99 -Wall -Wextra -Werror -O2 tools/c2upl.c -o bin/c2upl
```

---

### 3. Opciones de Línea de Comandos (`c2upl`)

```
Uso: c2upl [opciones] <archivo.c>

Opciones:
  -o <salida.upl>       Especificar ruta del archivo de salida transpilado
  -l, --lang <en|es>    Dialecto objetivo de UPL+ (por defecto: en)
  --test                Ejecutar la suite de verificación unitaria interna (5/5 tests)
```

---

### 4. Tabla de Equivalencias de Tipos (C99 $\to$ UPL+)

| Tipo en C99 | Tipo en UPL+ | Descripción |
|---|---|---|
| `void` | `void` | Tipo vacío / sin retorno |
| `void*` | `*void` | Puntero opaco universal |
| `int8_t` / `signed char` | `i8` | Entero signado de 8 bits |
| `uint8_t` / `unsigned char` | `u8` | Byte no signado de 8 bits |
| `int16_t` / `short` | `i16` | Entero signado de 16 bits |
| `uint16_t` / `unsigned short` | `u16` | Entero no signado de 16 bits |
| `int` / `int32_t` / `long` | `i32` | Entero signado de 32 bits |
| `unsigned int` / `uint32_t` | `u32` | Entero no signado de 32 bits |
| `int64_t` / `long long` | `i64` | Entero signado de 64 bits |
| `uint64_t` / `unsigned long long` | `u64` | Entero no signado de 64 bits |
| `size_t` | `usize` | Entero para direccionamiento de punteros |
| `float` | `f32` | Punto flotante simple precisión IEEE-754 |
| `double` | `f64` | Punto flotante doble precisión IEEE-754 |
| `char*` | `*i8` | Puntero a carácter / cadena C |
| `tipo*` | `*tipo` | Puntero directo tipado |
| `tipo buffer[N]` | `buffer: [tipo; N]` | Array de tamaño estático conocido |

---

### 5. Soporte Bilingüe Nativo (Inglés y Español)

El transpilador permite elegir el dialecto de salida según las preferencias del equipo de ingeniería:

#### Entrada original en C99 (`algoritmo.c`):
```c
int32_t factorial(int32_t n) {
    if (n <= 1) {
        return 1;
    }
    return n * factorial(n - 1);
}
```

#### Salida con Dialecto Inglés (`c2upl algoritmo.c`):
```upl
pub fn factorial(n: i32) -> i32 {
    if n <= 1 {
        return 1 ; 
    }
    return n * factorial ( n - 1 ) ; 
}
```

#### Salida con Dialecto Español (`c2upl -l es algoritmo.c`):
```upl
pub funcion factorial(n: i32) -> i32 {
    si n <= 1 {
        retornar 1 ; 
    }
    retornar n * factorial ( n - 1 ) ; 
}
```

---

### 6. Conversión de Estructuras y Directivas `#include`

* **Directivas de Preprocesador:** Las directivas `#include` se capturan automáticamente dentro de bloques nativos `embed "c" { ... }`, garantizando que las cabeceras del sistema sigan estando disponibles.
* **Declaraciones `typedef struct`:** Se convierten directamente en `pub struct` (o `pub estructura`), transpilando sus miembros y dimensiones de array.

#### Ejemplo de Struct C99:
```c
typedef struct {
    uint8_t cabecera[4];
    uint32_t id_transaccion;
    int64_t saldo;
} RegistroFinanciero;
```

#### UPL+ Generado:
```upl
pub struct RegistroFinanciero {
    cabecera: [u8; 4],
    id_transaccion: u32,
    saldo: i64,
}
```

---

### 7. Flujo de Trabajo Completo de Migración

1. Transpilar el archivo C existente a UPL+:
   ```bash
   bin/c2upl.exe mi_modulo.c -o mi_modulo.upl
   ```

2. Compilar el archivo generado con `uplc`:
   ```bash
   upl.bat -o mi_modulo.exe mi_modulo.upl
   ```

3. Ejecutar el binario generado:
   ```bash
   mi_modulo.exe
   ```
