# 📖 MANUAL DE SINTAXIS Y GRAMÁTICA: LENGUAJE UPL
## Especificación Oficial de Palabras Clave, Tipos y Estructuras

---

### 1. Palabras Clave y Equivalencia Bilingüe

En UPL, cualquier palabra clave puede escribirse indistintamente en inglés universal o en español soberano:

| Inglés Universal | Español Soberano | Función |
|---|---|---|
| `fn` | `funcion` | Declaración de función |
| `let` | `sea` / `variable` | Declaración de variable local inmutable |
| `mut` | `mutable` | Modificador de mutabilidad |
| `const` | `constante` | Constante inmutable en tiempo de compilación |
| `struct` | `estructura` | Declaración de estructura de datos |
| `if` / `else` | `si` / `sino` | Ramificación condicional |
| `while` | `mientras` | Bucle condicional |
| `for` ... `in` | `para` ... `en` | Bucle de rango |
| `return` | `retornar` | Retorno de función |
| `pub` | `publico` | Modificador de visibilidad exportada |
| `module` | `modulo` | Declaración de espacio de nombres |
| `as` | `como` | Conversión explícita de tipos (Cast) |
| `asm` | `asm` | Bloque de instrucciones en ensamblador inline |
| `embed` | `embed` | Bloque incrustado de otro lenguaje |
| `and` / `&&` | `and` / `&&` | Conjunción lógica |
| `or` / `\|\|` | `or` / `\|\|` | Disyunción lógica |

---

### 2. Tipos de Datos Primitivos

* **Enteros sin signo:** `u8` (byte), `u16` (puertos I/O), `u32` (colores, enteros 32b), `u64` (direcciones de 64 bits), `usize` (punteros de máquina).
* **Enteros con signo:** `i8`, `i16`, `i32`, `i64`.
* **Coma flotante:** `f32`, `f64`.
* **Lógicos y texto:** `bool` (`true`/`false`, `verdadero`/`falso`), `str` (cadena UTF-8), `void`.
* **Aritmética Exacta NeuroCOBOL:** `comp3` / `decimal` (BCD exacto para finanzas y cómputo crítico).
* **Punteros e Índices:** `*u32` (puntero crudo a memoria), `[u32; 10]` (array estático de 10 elementos).

---

### 3. Directivas de Silicio y Compilación

Las directivas comienzan con `@` y modifican la estrategia del compilador:
* `@baremetal`: Desactiva las librerías del sistema operativo anfitrión. Permite emitir instrucciones privilegiadas de CPU (`cli`, `sti`, `inb`, `outb`, `hlt`).
* `@snapshot`: Activa la captura de estado atómica y la inserción de Landing-Pads de reversión rápida.
* `@dvtrgas30` / `@vectorial`: Activa optimizaciones vectoriales y sellado de memoria con paridad `0xAA30`.
* `@guarded`: Inserta comprobaciones de límites estáticos de memoria para eliminar vulnerabilidades de desbordamiento.
* `@pure`: Marca funciones deterministas sin efectos secundarios.

---

### 4. Declaración de Variables e Inmutabilidad

Por defecto, toda variable es **inmutable**:
```rust
let x: u32 = 100;           // Inmutable: x no puede cambiar
let mut y: u32 = 0;         // Mutable: y puede modificarse
y = y + 1;
```

---

### 5. Estructuras y Punteros

```rust
struct SensorHardware {
    id_puerto: u16,
    frecuencia_hz: u32,
    esta_activo: bool
}

pub fn configurar_sensor(s: *SensorHardware, puerto: u16) -> void {
    (*s).id_puerto = puerto;
    (*s).frecuencia_hz = 1000;
    (*s).esta_activo = true;
}
```

---

### 6. Acceso a Hardware con Ensamblador Inline (`asm`)

```rust
@baremetal
pub fn leer_puerto_8bits(puerto: u16) -> u8 {
    let mut valor: u8 = 0;
    asm {
        mov dx, puerto
        in al, dx
        mov valor, al
    }
    return valor;
}
```
