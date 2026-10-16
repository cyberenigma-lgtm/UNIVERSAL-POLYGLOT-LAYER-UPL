# Guía Práctica de Integración y Ejemplos de Sistemas en UPL+

Esta guía técnica proporciona patrones de desarrollo, firmas de función con tooltips conceptuales y ejemplos completos para la integración de los módulos del sistema en programas UPL+.

---

## 1. Guía de Integración de Seguridad: `urep_guard`

### 1.1 Diagrama de Flujo de Verificación CFI
```
[Entrada de Función / Retorno]
          │
          ▼
┌──────────────────────────────────────┐
│  urep_verify_instruction_pointer()   │
└──────────────────────────────────────┘
          │
    ¿RIP en [.text_base, .text_limit]?
         ├── SÍ ──► [Flujo de ejecución normal]
         └── NO ──► [Incrementar violaciones -> Abortar ejecución]
```

### 1.2 Parámetros de la API con Tooltip de Ingeniería
*   `ctx`: Puntero al bloque de control de seguridad `UrepSecurityContext`. Debe residir en un segmento de memoria no modificable por código no privilegiado.
*   `instruction_pointer` (`rip`): Valor del registro contador de programa (`PC` / `RIP`) capturado en el punto de interrupción o prólogo de función.
*   `expected_canary`: Valor secreto fijado en el arranque del hilo o proceso (`0xDEADBEEF...`).
*   `current_canary`: Valor extraído inmediatamente antes del epílogo de la función (`ret`). Si no coinciden, se detecta corrupción de memoria activa.

### 1.3 Ejemplo de Uso Completo
```rust
module demo_seguridad_sistemas;

@snapshot
struct UrepSecurityContext {
    active: bool,
    text_segment_base: u64,
    text_segment_limit: u64,
    guard_canary: u64,
    violations_detected: u32,
    mitigated_events: u32
}

pub fn main() -> i32 {
    let mut ctx: UrepSecurityContext;
    urep_guard_init(&ctx, 0x00400000, 0x004FFFFF, 0xCAFEBABE11223344);

    // Verificación de una llamada a función autorizada
    let puntero_actual: u64 = 0x004015A0;
    if !urep_verify_instruction_pointer(&ctx, puntero_actual) {
        return 1; // Violación de seguridad detectada
    }

    return 0; // Ejecución correcta
}
```

---

## 2. Guía de Normalización Multi-Arquitectura: `urep_arch`

### 2.1 Uso en Cross-Compilación y Emulación
Cuando un algoritmo se traslada entre microcontroladores de 16 bits y procesadores de 64 bits, las direcciones lineales pueden provocar pérdidas silenciosas de datos por desbordamiento. El módulo `urep_arch` normaliza de forma determinista estos límites.

### 2.2 Parámetros con Tooltip
*   `conv`: Estructura receptora del resultado de la conversión.
*   `addr`: Dirección de 64 bits a evaluar.
*   `from_bits`: Ancho de bus original (ej: 64 para sistemas modernos).
*   `to_bits`: Ancho de bus de destino (ej: 16 o 32 bits).

### 2.3 Ejemplo de Conversión de Direcciones y Segmentación
```rust
module demo_conversion_arquitectura;

pub fn main() -> i32 {
    let mut conv: ArchAddressConversion;
    urep_arch_init(&conv);

    // Calcular dirección segmentada de memoria de video VGA en modo real
    let dir_vga_lineal: u64 = 0x000A0000;
    urep_arch_calculate_segmented(&conv, dir_vga_lineal);

    // conv.real_mode_segment contiene 0xA000
    // conv.real_mode_offset  contiene 0x0000
    return 0;
}
```

---

## 3. Guía de Acceso a Hardware Bare-Metal: `urep_hw`

### 3.1 Inicialización de Periféricos de Video y Temporización
En entornos donde no existe un sistema operativo subyacente (Ring 0 / Freestanding), el módulo `urep_hw` provee la tabla de periféricos base:

```rust
module demo_acceso_hardware;

pub fn main() -> i32 {
    let mut registro_hw: HardwareRegistry;
    urep_hw_init(&registro_hw);

    let vram_base: u64 = urep_hw_get_framebuffer(&registro_hw);
    // vram_base == 0x000A0000 (Memoria de video lista para escritura directa)

    return 0;
}
```

---

## 4. Guía de Simulación de CPU: `cpu_runner`

### 4.1 Ciclo de Instrucción
El simulador `cpu_runner` implementa la secuencia clásica de un procesador CISC/RISC:
1. **Fetch:** Recepción del código de operación (`opcode`).
2. **Decode:** Identificación de operandos fuente y destino.
3. **Execute:** Modificación atómica de registros (`RAX`, `RBX`, `RIP`).
4. **Step:** Incremento monotónico del contador de ciclos y avance del puntero de instrucción.

### 4.2 Ejemplo de Pruebas Unitarias de Algoritmos
```rust
module demo_test_cpu;

pub fn main() -> i32 {
    let mut cpu: CpuRegisterState;
    cpu_init(&cpu, 0x00007FFFFFFFFFFF);

    // Cargar 250 en RAX
    cpu_step(&cpu, CPU_OP_MOV, 0, 250);

    // Sumar 50 -> RAX = 300
    cpu_step(&cpu, CPU_OP_ADD, 0, 50);

    // Validar resultado en registro acumulador
    if cpu.rax != 300 {
        return 1; // Fallo de aserción en simulación
    }

    return 0; // Prueba unitaria superada
}
```

---

## 5. Guía de Cómputo Vectorial: `vector_compute`

### 5.1 Primitiva de Álgebra Lineal
Diseñado para cálculos geométricos tridimensionales con componente homogénea $w$ ($[x, y, z, w]$).

$$ \text{Producto Escalar} = A_x \cdot B_x + A_y \cdot B_y + A_z \cdot B_z + A_w \cdot B_w $$

### 5.2 Ejemplo de Uso
```rust
module demo_calculo_vectorial;

pub fn main() -> i32 {
    let mut a: Vector4D;
    let mut b: Vector4D;
    let mut resultado: Vector4D;

    vec4_init(&a, 2, 4, 6, 8);
    vec4_init(&b, 1, 1, 1, 1);

    // Suma vectorial en paralelo
    vec4_add(&resultado, &a, &b);
    // resultado: [3, 5, 7, 9]

    let dot: u64 = vec4_dot_product(&a, &b);
    // dot: (2*1 + 4*1 + 6*1 + 8*1) = 20

    return 0;
}
```
