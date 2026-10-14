# 📖 DICCIONARIO MAESTRO DE CÓDIGOS DE ERROR, SEÑALES Y DIAGNÓSTICO MULTI-SO
## Ecosistema Soberano Universal Polyglot Layer (UPL)
**Autor:** José Manuel Moreno Cano (Noxferion)  
**Cobertura:** Windows (NTSTATUS), Linux / POSIX (Signals), Bare-Metal x86_64 Ring-0 (Microkernels y Firmware) y UPLC Compiler.

---

## 📌 ÍNDICE GENERAL
1. [Excepciones y Códigos de Salida de Windows (NTSTATUS)](#1-windows-ntstatus)
2. [Señales y Códigos de Salida de Linux / macOS / POSIX](#2-linux--posix)
3. [Excepciones de Silicio y CPU Bare-Metal Ring-0](#3-bare-metal-x86_64)
4. [Códigos de Error Nativos del Compilador UPL (UPL-Exxx)](#4-errores-nativos-upl)
5. [Guía de Resolución Rápida con Landing-Pads y Salto Lógico](#5-guía-de-resolución)

---

## 🪟 1. WINDOWS NTSTATUS Y CÓDIGOS DE CRASH / SALIDA

En Windows, cuando un proceso finaliza con un error no capturado, el kernel de Windows devuelve un código numérico signado de 32 bits (generalmente negativo en decimal o prefijado con `0xC0000000` en hexadecimal).

| Código Decimal | Código Hexadecimal | Nombre de la Excepción | Desglose y Causa Física | Solución en UPL |
|---|---|---|---|---|
| `0` | `0x00000000` | `STATUS_SUCCESS` | El programa finalizó con éxito absoluto. | No requiere acción. |
| `-1073741819` | `0xC0000005` | `STATUS_ACCESS_VIOLATION` | **Violación de Acceso a Memoria (Puntero Nulo / Dirección Ilegal):** El hilo intentó leer o escribir en una página de memoria virtual que no le pertenece o que tiene permisos incompatibles (ej: escribir en memoria de solo lectura). | Comprueba que tus punteros `*ptr` no sean nulos (`NULL / 0`). Si defines estructuras, usa `@snapshot` para activar los landing-pads de DVTRGAS-30 que previenen lecturas y escrituras desalineadas. |
| `-1073741674` | `0xC0000096` | `STATUS_PRIVILEGED_INSTRUCTION` | **Instrucción Privilegiada (Conflicto de Ring):** Intentaste ejecutar una instrucción de CPU Ring-0 (como `cli`, `sti`, `inb`, `outb`, `hlt`, `mov cr3`, `invlpg`) dentro del espacio de usuario normal de Windows (Ring-3). | Esa lógica pertenece al modo Ring-0 / Bare-Metal. Compila tu archivo usando `upl build archivo.upl --target=asm --baremetal` para arrancar en modo nativo en el PC real o en QEMU. |
| `-1073741795` | `0xC000001D` | `STATUS_ILLEGAL_INSTRUCTION` | **Instrucción Ilegal:** La CPU encontró un byte de código que no corresponde a ningún código de operación válido en la arquitectura actual, o se intentó usar AVX-512 en un procesador que no lo soporta. | Verifica el ensamblador inline en `asm { ... }` y asegúrate de no saltar o llamar a un puntero corrupto que apunte a datos en lugar de código ejecutable. |
| `-1073741571` | `0xC00000FD` | `STATUS_STACK_OVERFLOW` | **Desbordamiento de la Pila (Stack Overflow):** La pila asignada al hilo (típicamente 1 MB en Windows) se agotó por llamadas recursivas sin fin o arrays locales de tamaño excesivo. | Revisa la condición base de las funciones recursivas o reserva memoria en el heap en lugar de declarar buffers masivos en variables locales. |
| `-1073741676` | `0xC0000094` | `STATUS_INTEGER_DIVIDE_BY_ZERO` | **División Entera por Cero:** Se intentó realizar una división (`/`) o residuo (`%`) donde el operando derecho era cero. | Añade una condición `if divisor != 0` antes de operar, o usa los tipos `comp3` de NeuroCOBOL que tienen control de sobreflujo incorporado. |
| `-1073741682` | `0xC000008E` | `STATUS_FLOAT_DIVIDE_BY_ZERO` | **División Flotante por Cero:** Ocurrió una división por `0.0f` en las unidades SSE o x87. | Añadir guarda de validación para variables flotantes. |
| `-1073741510` | `0xC000013A` | `STATUS_CONTROL_C_EXIT` | **Interrupción de Teclado:** El usuario o el sistema cerró la ejecución con `Ctrl + C`. | Comportamiento esperado si se interrumpió la consola manualmente. |
| `-1073741816` | `0xC0000008` | `STATUS_INVALID_HANDLE` | **Handle Inválido:** Se intentó cerrar, leer o escribir en un descriptor del sistema operativo no abierto o corrupto. | Verificar retorno de inicialización de archivos o dispositivos. |

---

## 🐧 2. LINUX / MACOS / POSIX (SEÑALES Y EXIT CODES)

En sistemas Unix (Linux, macOS, BSD, Android), el código de salida de un proceso abortado por una señal fatal es `128 + N`, donde `N` es el número de señal de POSIX.

| Código de Salida | Señal POSIX | Nombre de la Señal | Desglose y Causa Física | Solución en UPL |
|---|---|---|---|---|
| `0` | - | `EXIT_SUCCESS` | Terminación normal exitosa. | Ninguna. |
| `1` | - | `CATCHALL_ERROR` | Error genérico de ejecución devuelto por la aplicación (`exit(1)`). | Revisar el mensaje de error impreso en consola. |
| `126` | - | `PERMISSION_DENIED` | El binario fue encontrado pero el usuario no tiene permisos de ejecución (+x). | Ejecutar `chmod +x <binario>` en la terminal. |
| `127` | - | `COMMAND_NOT_FOUND` | No se encontró el binario o falta una biblioteca compartida dinámica (`.so`). | Verificar rutas o compilar con `-static`. |
| `130` | `SIGINT (2)` | `Interrupt` | Proceso cancelado por señal `SIGINT` (Ctrl+C en la consola). | Interrupción manual del usuario. |
| `132` | `SIGILL (4)` | `Illegal Instruction` | Intento de ejecutar un opcode inválido o instrucciones de Ring 0 en espacio de usuario Linux. | Asegurarse de que el código no use instrucciones privilegiadas fuera del modo `@baremetal`. |
| `134` | `SIGABRT (6)` | `Abort` | Llamada explícita a `abort()` (aserciones fallidas de seguridad o corrupción de memoria). | Revisar las comprobaciones de integridad antes de la llamada. |
| `135` | `SIGBUS (7)` | `Bus Error` | Acceso a memoria con desalineación física o acceso a mmap truncado. | Los landing-pads de DVTRGAS-30 en UPL fuerzan la alineación automática a 16 bytes. |
| `136` | `SIGFPE (8)` | `Floating Point Exception`| Operación aritmética errónea: división entera por cero o desbordamiento de coma flotante. | Comprobar divisores antes de la operación. |
| `137` | `SIGKILL (9)` | `Kill / OOM-Killer` | El kernel de Linux mató el proceso forzosamente (generalmente porque el sistema se quedó sin memoria RAM). | Optimizar estructuras de datos o usar puntos de control de Salto Lógico para guardar a disco. |
| `139` | `SIGSEGV (11)` | `Segmentation Fault` | Violación de Segmento: Lectura o escritura fuera de las páginas asignadas al proceso. | Comprobar punteros no inicializados o fuera de rango en arrays. |
| `141` | `SIGPIPE (13)` | `Broken Pipe` | Intento de escribir en un pipe o socket cerrado por el otro extremo. | Manejar el cierre de la conexión antes de escribir. |
| `143` | `SIGTERM (15)` | `Termination Signal` | Señal de apagado o terminación ordenada enviada por el sistema (`kill <pid>`). | Gestionar limpieza ordenada antes de cerrar. |

---

## ⚡ 3. EXCEPCIONES DE CPU EN BARE-METAL / RING-0 (MICROKERNELS Y FIRMWARE)

Cuando UPL corre directamente en el silicio (modo `@baremetal` en el Microkernel de 64 bits), **no hay sistema operativo debajo**: es la propia CPU Intel/AMD x86_64 la que dispara estas interrupciones a través de la IDT.

| Vector CPU | Mnemónico | Nombre de la Excepción | Desglose y Causa en Silicio | Solución en el Microkernel UPL |
|---|---|---|---|---|
| `0x00` | `#DE` | `Divide Error` | Ocurre cuando se ejecuta `idiv` con divisor 0 o cuando el cociente no cabe en el registro de destino (`rax`). | Implementar comprobación previa antes de la instrucción de división. |
| `0x01` | `#DB` | `Debug Exception` | Disparada por hardware cuando se activa el bit de paso a paso o se alcanza un registro de depuración `DR0-DR3`. | Se utiliza para tracing y telemetría del Salto Lógico. |
| `0x02` | `NMI` | `Non-Maskable Interrupt` | Error crítico de hardware físico independiente del software (ej: fallo de paridad de bus RAM). | Requiere diagnóstico de hardware o reinicio seguro. |
| `0x03` | `#BP` | `Breakpoint` | Generada por la instrucción `INT 3` (opcode `0xCC`). | Usada para congelar el sistema en un punto de control y emitir el snapshot. |
| `0x04` | `#OF` | `Overflow` | Disparada por la instrucción `INTO` si la bandera de desbordamiento `OF` está activa. | Manejo con aritmética segura COMP-3 de NeuroCOBOL. |
| `0x05` | `#BR` | `BOUND Range Exceeded` | La instrucción `BOUND` detectó que un índice excedió el límite inferior o superior. | Los Landing-Pads de 6 ceros de DVTRGAS-30 absorben el desbordamiento. |
| `0x06` | `#UD` | `Invalid Opcode` | La CPU intentó decodificar un byte que no corresponde a una instrucción x86_64 válida. | Verificar que el trampolín `entry_x64.S` cargó correctamente el selector de 64 bits `0x08`. |
| `0x07` | `#NM` | `Device Not Available` | Acceso a instrucciones SSE/AVX mientras el bit `CR0.TS` o `CR0.EM` está activo. | El Microkernel debe ejecutar `clts` y configurar `CR4.OSFXSR = 1` en el arranque. |
| `0x08` | `#DF` | `Double Fault` | Ocurre si la CPU sufre una segunda excepción mientras intentaba invocar el manejador de la primera. | Crítico: Indica que la pila del kernel o la tabla IDT está corrupta. Requiere Task State Segment (TSS) con pila IST dedicada. |
| `0x0A` | `#TS` | `Invalid TSS` | El descriptor del Segmento de Estado de Tarea (TSS) no es válido. | Verificar la entrada TSS en la GDT de 64 bits. |
| `0x0B` | `#NP` | `Segment Not Present` | El bit Presente (P) del selector de segmento cargado en `ds`, `es` o `ss` está en 0. | Verificar descriptores en la GDT. |
| `0x0C` | `#SS` | `Stack Fault` | Límite del segmento de pila excedido o página de pila no presente. | Aumentar el tamaño del stack asignado en `boot_x64.S`. |
| `0x0D` | `#GP` | `General Protection Fault` | **Fallo General de Protección:** Ocurre por: 1) Escribir a un registro reservado en `CR0/CR4`, 2) Dirección no canónica (fuera de los 48 bits válidos de x86_64), o 3) Cargar un selector de GDT corrupto. | Verificar que todas las direcciones de puntero residan en espacio canónico (`0x0000000000000000` a `0x00007FFFFFFFFFFF`). |
| `0x0E` | `#PF` | `Page Fault` | **Fallo de Página:** Se intentó acceder a una dirección virtual que no tiene entrada válida en las tablas PML4 / PDPT / Page Directory o carece de permisos de escritura (bit R/W = 0). | El registro `CR2` contiene la dirección exacta que causó el fallo. El código de error en la pila indica si fue lectura/escritura y si estaba en Ring-0 o Ring-3. |

---

## 🛡️ 4. CÓDIGOS DE ERROR NATIVOS DEL COMPILADOR UPL

El compilador `uplc` utiliza códigos estructurados para que el programador sepa exactamente en qué fase del pipeline ocurrió un problema:

```
UPL - [FASE] [NÚMERO]
       E0 = Léxico (Tokens)
       E1 = Sintáctico (Parser / Gramática)
       E2 = Semántico (Tipos y Ámbitos)
       E3 = Generador de Código (Backends C / ASM)
       E4 = Salto Lógico y Persistencia
       E5 = Blindaje Vectorial DVTRGAS-30
       E6 = Precisión y Paridad NeuroCOBOL
```

| Código | Categoría | Mensaje y Causa | Solución |
|---|---|---|---|
| `UPL-E001` | Léxico | Carácter desconocido o no admitido en el archivo fuente `.upl`. | Eliminar caracteres de control o invisibles no permitidos. |
| `UPL-E002` | Léxico | Cadena de texto literal sin cerrar (falta comilla `"` o `'` de cierre). | Cerrar la cadena de texto en la misma línea o usar barra de escape. |
| `UPL-E101` | Sintaxis | Token inesperado: Se esperaba un símbolo como `;`, `}`, `)` o `:`. | Revisar la línea indicada por el parser y verificar cierres de llaves. |
| `UPL-E102` | Sintaxis | Declaración de función o estructura mal formada. | Asegurarse de seguir la forma `fn nombre(args) -> Tipo { ... }`. |
| `UPL-E201` | Semántica | Incompatibilidad de tipos: Intento de asignar un tipo a otro sin conversión explícita (`as`). | Usar la palabra clave `as` para convertir explícitamente: `valor as u32`. |
| `UPL-E202` | Semántica | Variable o identificador no declarado en el bloque actual. | Declarar la variable previamente con `let` o `const`. |
| `UPL-E203` | Semántica | Intento de reasignar una variable inmutable declarada con `const`. | Si la variable debe cambiar de valor, declárala con `let mut`. |
| `UPL-E301` | Backend | Característica no soportada por el emisor seleccionado. | Revisar que las directivas `@baremetal` se compilen con `--target=asm`. |
| `UPL-E401` | Salto Lógico | Fallo al persistir snapshot: No se pudo escribir el archivo `.gxp`. | Comprobar permisos de escritura en disco o espacio disponible. |
| `UPL-E501` | DVTRGAS-30 | Violación de Landing-Pad: Se detectó alteración en las guardas de 6 ceros. | Un puntero descontrolado intentó sobreescribir la memoria protegida del tensor. |
| `UPL-E601` | NeuroCOBOL | Fallo de Integridad Criptográfica: La firma o paridad del módulo no coincide. | Re-firmar el módulo soberano con el sello de autoridad Noxferion. |

---

## 🚀 5. GUÍA DE RESOLUCIÓN RÁPIDA CON UPL

Gracias al motor inteligente de diagnóstico integrado en `upl`:

1. Si ejecutas un programa con:
   ```bash
   upl run mi_app.upl
   ```
2. Y el programa termina con cualquier código de error (como el `-1073741674` que vimos antes al intentar ejecutar `cli` en Windows):
3. La propia herramienta `upl` desglosa en tu pantalla en tiempo real:
   - Qué significa ese código en tu sistema operativo actual.
   - La causa física exacta.
   - Cómo adaptarlo en el código fuente de UPL.
