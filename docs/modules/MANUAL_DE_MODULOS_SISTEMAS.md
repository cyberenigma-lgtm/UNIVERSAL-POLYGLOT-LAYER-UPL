# Manual de Arquitectura y Módulos de Sistemas UPL+

Este documento define la especificación técnica, estructuras de memoria, invariantes de seguridad y casos de uso de los módulos de ingeniería de sistemas integrados en el ecosistema **Universal Polyglot Layer (UPL+)**.

---

## Tabla de Módulos del Núcleo

| Módulo | Ruta Física | Dominio Técnico | Abstracción de Hardware |
| :--- | :--- | :--- | :--- |
| **`urep_guard`** | `modules/urep/urep_guard.upl` | Seguridad de memoria y CFI | Canarios de pila, segmento `.text`, W^X / NX-bit |
| **`urep_arch`** | `modules/urep/urep_arch.upl` | Multi-Arquitectura y Punteros | Espacios 16, 32, 64 y 128 bits, Segment:Offset |
| **`urep_hw`** | `modules/urep/urep_hw.upl` | Introspección y Periféricos | MMIO, Framebuffer VESA/VGA, HPET, Bus PCI |
| **`masm_table`** | `modules/multilang_asm/masm_table.upl`| Mnemónicos Políglotas | Normalización de instrucciones a x86_64 canónico |
| **`cpu_runner`** | `modules/emulator/cpu_runner.upl` | Máquina de Estados de CPU | Simulación de registros, despacho de opcodes |
| **`vector_compute`**| `modules/compute/vector_ops.upl` | Álgebra Vectorial y SIMD | Operaciones 4D, producto escalar, escalado escalar |

---

## 1. Módulo `urep_guard` (Control Flow Integrity & Stack Canaries)

### 1.1 Propósito Técnico
Mitigación en tiempo de compilación y ejecución contra ataques de corrupción de memoria, desbordamiento de búfer (Stack Smashing) y desvío de flujo de control (ROP/JOP Chains).

### 1.2 Definición de Estructura de Datos (Tooltip & Campos)

```rust
@snapshot
struct UrepSecurityContext {
    active: bool,               // [Tooltip: Bandera de activación del escudo]
    text_segment_base: u64,     // [Tooltip: Dirección base física/virtual de la sección .text]
    text_segment_limit: u64,    // [Tooltip: Límite superior de la sección .text ejecutable]
    guard_canary: u64,          // [Tooltip: Valor canario determinista contra desbordamientos]
    violations_detected: u32,   // [Tooltip: Contador de violaciones de puntero de instrucción]
    mitigated_events: u32       // [Tooltip: Contador de eventos abortados exitosamente]
}
```

### 1.3 Referencia de Funciones de la API

*   `urep_guard_init(ctx: *UrepSecurityContext, base: u64, limit: u64, canary: u64) -> void`
    *   *Precondición:* `limit >= base`.
    *   *Comportamiento:* Inicializa el contexto de seguridad y restablece los contadores de fallos a cero.
*   `urep_verify_instruction_pointer(ctx: *UrepSecurityContext, rip: u64) -> bool`
    *   *Validación:* Retorna `true` si `base <= rip <= limit`. Si `rip` se ubica fuera del rango (ej. pila o heap), incrementa `violations_detected` y retorna `false`.
*   `urep_verify_stack_canary(expected_canary: u64, current_canary: u64) -> bool`
    *   *Validación:* Comprueba si el canario en la cabecera del marco de pila ha sufrido sobrescritura.

---

## 2. Módulo `urep_arch` (Conversor Multi-Arquitectura)

### 2.1 Propósito Técnico
Normalización y truncado seguro de punteros y direcciones lineales de memoria al compilar o interoperar entre arquitecturas de 16, 32, 64 y 128 bits, así como descomposición en segmentos de modo real x86 (`segment:offset`).

### 2.2 Definición de Estructura de Datos

```rust
@snapshot
struct ArchAddressConversion {
    source_address: u64,       // [Tooltip: Dirección original de entrada]
    from_bits: u32,            // [Tooltip: Ancho de bus de origen (16, 32, 64)]
    to_bits: u32,              // [Tooltip: Ancho de bus destino objetivo]
    converted_address: u64,    // [Tooltip: Dirección resultante tras máscara o truncado]
    overflow_detected: bool,   // [Tooltip: 'true' si el valor excede la capacidad del destino]
    real_mode_segment: u32,    // [Tooltip: Segmento base de 16 bits (addr / 16)]
    real_mode_offset: u32      // [Tooltip: Desplazamiento de 16 bits (addr % 16)]
}
```

### 2.3 Ejemplo de Uso en Sistemas

```rust
let mut conv: ArchAddressConversion;
urep_arch_init(&conv);

// Conversión de dirección lineal (0x11140) a formato Segmento:Offset
urep_arch_calculate_segmented(&conv, 0x11140);
// conv.real_mode_segment == 0x1114
// conv.real_mode_offset  == 0x0000
```

---

## 3. Módulo `urep_hw` (Registro MMIO y Periféricos)

### 3.1 Propósito Técnico
Mapeo de periféricos físicos en modo bare-metal mediante acceso a memoria mapeada (MMIO) e información de arranque (VGA, VESA, HPET, Controladores de Bus).

### 3.2 Tabla de Direcciones MMIO Predeterminadas

| Periférico | Dirección Base | Longitud | Modo de Operación |
| :--- | :--- | :--- | :--- |
| **VGA 13h** | `0x000A0000` | 64,000 bytes | Framebuffer lineal 320x200x8bpp |
| **HPET Timer** | `0xFED00000` | 1,024 bytes | Temporizador de alta precisión |

---

## 4. Módulo `multilang_asm` (Mnemónicos Políglotas)

### 4.1 Propósito Técnico
Resolución léxica de instrucciones de bajo nivel escritas en lenguajes naturales hacia los códigos de operación canónicos de la arquitectura de destino (x86_64).

### 4.2 Mapeo Canónico de Instrucciones

| Código Canónico | Inglés (x86) | Español (`masm`) | Francés (`masm`) | Operación Binaria |
| :--- | :--- | :--- | :--- | :--- |
| `0x01` | `MOV` | `mover` | `deplacer` | Transferencia de registro/memoria |
| `0x02` | `ADD` | `sumar` | `ajouter` | Suma aritmética con acarreo |
| `0x03` | `SUB` | `restar` | `soustraire` | Resta aritmética |
| `0x04` | `CMP` | `comparar`| `comparer` | Comparación y flags de estado |
| `0x05` | `JMP` | `saltar` | `sauter` | Salto incondicional |
| `0x07` | `RET` | `retornar`| `retour` | Retorno de subrutina |

---

## 5. Módulo `cpu_runner` (Simulador de Instrucciones en Espacio de Usuario)

### 5.1 Propósito Técnico
Emulador y máquina de estados finitos que simula un subconjunto de instrucciones x86_64 en espacio de usuario. Permite ejecutar pruebas unitarias y validación de algoritmos bare-metal sin necesidad de virtualización asistida por hardware (KVM/QEMU).

### 5.2 Estructura de Registros

```rust
@snapshot
struct CpuRegisterState {
    rax: u64,                  // [Tooltip: Registro acumulador principal]
    rbx: u64,                  // [Tooltip: Registro base]
    rcx: u64,                  // [Tooltip: Registro contador para bucles]
    rdx: u64,                  // [Tooltip: Registro de datos / E/S]
    rip: u64,                  // [Tooltip: Puntero de instrucción actual]
    rsp: u64,                  // [Tooltip: Puntero de cabeza de pila]
    flags_zero: bool,          // [Tooltip: Bandera de cero (ZF)]
    halted: bool,              // [Tooltip: Estado de parada de la CPU (HLT)]
    cycles_executed: u64       // [Tooltip: Contador monotónico de ciclos de reloj ejecutados]
}
```

---

## 6. Módulo `vector_compute` (Operaciones Vectoriales SIMD)

### 6.1 Propósito Técnico
Primitivas de cómputo para álgebra lineal de 4 dimensiones sobre vectores empaquetados, diseñadas para transpilar a registros vectoriales SSE/AVX en x86_64 o NEON en ARM64.

### 6.2 Operaciones Fundamentales

*   `vec4_add(dst: *Vector4D, a: *Vector4D, b: *Vector4D) -> void`: Suma elemento a elemento en paralelo.
*   `vec4_scale(dst: *Vector4D, src: *Vector4D, factor: u64) -> void`: Escalado escalar uniforme.
*   `vec4_dot_product(a: *Vector4D, b: *Vector4D) -> u64`: Cálculo de producto escalar acumulado.

---

## Verificación de Compilación

Todos los módulos están verificados y certificados para compilar limpiamente tanto a **ANSI C99** como a **Ensamblador NASM x86_64** utilizando el compilador oficial nativo:

```bash
# Compilación a código fuente C99
bin/uplc.exe build modules/urep/urep_guard.upl -t c -o modules/urep/urep_guard.c

# Compilación a Ensamblador NASM nativo
bin/uplc.exe build modules/urep/urep_guard.upl -t asm -o modules/urep/urep_guard.asm
```
