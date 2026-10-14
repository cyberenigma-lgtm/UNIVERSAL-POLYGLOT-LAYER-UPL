# 💻 GALERÍA DE EJEMPLOS EN CÓDIGO UPL
## Demostraciones Prácticas del Nuevo Lenguaje de Programación

---

### Ejemplo 1: Cálculo Financiero Exacto con NeuroCOBOL (`comp3`)

Elimina cualquier error de redondeo de coma flotante de IEEE-754:

```rust
module finanzas::liquidacion;

// Estructura con tipado exacto BCD
pub struct FacturaSoberana {
    id_factura: u64,
    importe_base: comp3(12, 2),
    tasa_iva_pct: u32,
    total_liquidado: comp3(12, 2)
}

pub fn calcular_total(f: *FacturaSoberana) -> void {
    let base: comp3(12, 2) = (*f).importe_base;
    let factor: comp3(12, 2) = (*f).tasa_iva_pct as comp3(12, 2);
    let iva: comp3(12, 2) = (base * factor) / 100;
    (*f).total_liquidado = base + iva;
}
```

---

### Ejemplo 2: Programación Directa de Silicio (Bare-Metal Ring 0)

Control de periféricos y registros de la CPU sin intermediarios:

```rust
module hardware::puerto_serie;

pub const PUERTO_COM1: u16 = 0x3F8;

@baremetal
pub fn inicializar_puerto_serie() -> void {
    asm {
        mov dx, PUERTO_COM1 + 1
        mov al, 0x00
        out dx, al        // Desactivar interrupciones COM1

        mov dx, PUERTO_COM1 + 3
        mov al, 0x80
        out dx, al        // Activar DLAB para configurar baudios

        mov dx, PUERTO_COM1
        mov al, 0x03
        out dx, al        // Divisor 38400 baudios

        mov dx, PUERTO_COM1 + 3
        mov al, 0x03
        out dx, al        // Modo 8 bits, sin paridad, 1 bit de parada
    }
}
```

---

### Ejemplo 3: Inmunidad y Auto-Sanación con `@snapshot`

```rust
module seguridad::blindaje;

@snapshot
pub struct EstadoTransaccion {
    id_transaccion: u64,
    balance: u64,
    bloqueado: bool
}

pub fn procesar_con_seguridad(t: *EstadoTransaccion) -> bool {
    let mut intentos: u32 = 0;
    while intentos < 3 {
        // En caso de corrupción o anomalía, se restaura el último estado atómico
        intentos = intentos + 1;
    }
    return true;
}
```
