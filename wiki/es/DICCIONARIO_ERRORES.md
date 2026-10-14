# 🪟 DICCIONARIO DE DIAGNÓSTICO Y ERRORES MULTI-SO EN 57 IDIOMAS
## Resolución Inteligente de Fallos para Windows, Linux y CPU Bare-Metal

UPL cuenta con un motor de diagnóstico integrado que traduce y analiza códigos de error de cualquier plataforma al instante.

---

### 1. Consultar Errores desde la Terminal

Puedes consultar cualquier código de error hexadecimal o decimal usando el comando `upl diag`:

```bash
# Diagnosticar en Español
upl diag 0xC0000005 -l es

# Diagnosticar en Inglés
upl diag 0xC0000096 -l en

# Diagnosticar en Japonés
upl diag 0xC00000FD -l ja
```

---

### 2. Códigos Más Frecuentes en Windows (NTSTATUS)

| Código Hex | Nombre NTSTATUS | Causa Física | Solución en UPL |
|---|---|---|---|
| `0xC0000005` | `STATUS_ACCESS_VIOLATION` | Puntero nulo o intento de escribir en memoria protegida. | Verificar punteros `*ptr != NULL` o usar directiva `@snapshot`. |
| `0xC0000096` | `STATUS_PRIVILEGED_INSTRUCTION` | Instrucción Ring 0 (`cli`, `inb`, etc.) ejecutada en Ring 3. | Compilar con `upl build archivo.upl -t asm --baremetal` para Ring 0. |
| `0xC000001D` | `STATUS_ILLEGAL_INSTRUCTION` | Byte de código inválido o instrucción no soportada por la CPU. | Verificar el ensamblador inline en `asm { ... }`. |
| `0xC00000FD` | `STATUS_STACK_OVERFLOW` | La pila del hilo se agotó por recursión infinita. | Revisar condición de parada o alojar datos masivos en heap. |
| `0xC0000094` | `STATUS_INTEGER_DIVIDE_BY_ZERO` | División entera por cero (`/ 0`). | Añadir comprobación previa `if divisor != 0` o usar tipos `comp3`. |

---

### 3. Excepciones Físicas de CPU (Modo Bare-Metal / Ring 0)

| Vector CPU | Mnemónico | Causa en Silicio | Solución en UPL |
|---|---|---|---|
| `0x00` | `#DE` | División por cero o desbordamiento en registro `rax`. | Guarda de validación antes de `idiv`. |
| `0x06` | `#UD` | Opcode no válido decodificado por la CPU. | Verificar alineación y selector de 64 bits en GDT. |
| `0x08` | `#DF` | Doble fallo en el manejo de interrupciones. | Pila IST dedicada en el TSS de 64 bits. |
| `0x0D` | `#GP` | Fallo General de Protección (dirección no canónica). | Verificar que las direcciones de punteros sean válidas en x86_64. |
| `0x0E` | `#PF` | Fallo de página de memoria virtual. | Verificar tablas de páginas PML4 / PDPT. |
