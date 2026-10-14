# Especificación UPL: LENGUAJE MADRE (v1.1)

El **Lenguaje Madre** es la representación intermedia (IR) universal de la suite UPL. Diseñado para la soberanía técnica y la independencia del hardware.

## 🛠️ Set de Instrucciones Universales

### 1. Gestión de Memoria Virtual
- `ALLOC $IDX, <TYPE>`: Reserva un slot virtual en el mapa de memoria.
- `ASSIGN $IDX, <SRC>`: Asigna un valor, puntero o resultado de operación.
- `FREE $IDX`: Libera el recurso asociado.

### 2. Unidad de Operación Lógica (VALU)
- `ADD $DEST, $IN1, $IN2`
- `SUB $DEST, $IN1, $IN2`
- `MUL $DEST, $IN1, $IN2`
- `DIV $DEST, $IN1, $IN2`
- `BIT_AND / OR / XOR / NOT`: Operaciones a nivel de bit.

### 3. Control de Flujo Soberano
- `LABEL <NAME>`: Punto de anclaje.
- `JUMP <LABEL>`: Salto incondicional.
- `IF <COND> JUMP <LABEL>`: Salto condicional (basado en banderas virtuales).
- `CALL <TARGET>, [<ARGS>]`: Invocación de procedimiento.
- `RET <VAL>`: Retorno de valor.

### 4. Interfaz de Sistema (Systems-Layer)
- `SYSCALL <ID>, [<ARGS>]`: Llamada directa a servicios del kernel / microkernel.
- `IO_WRITE <STREAM>, <SRC>`: Salida de datos universal.

## 📁 Tipos de Datos UPL
- `I8`, `I16`, `I32`, `I64`: Enteros firmados.
- `U8`, `U16`, `U32`, `U64`: Enteros no firmados.
- `F32`, `F64`: Coma flotante.
- `PTR`: Puntero universal soberano.
- `STR`: Cadenas de texto gestionadas por el sistema.
- `SOB`: Objeto Soberano (Encapsulación de datos políglota).

## 🚀 Paradigmas Soportados
- **Imperativo**: Mapeo directo de estados y saltos.
- **OOP**: Métodos como PROCEDURES con el primer argumento `$SELF` (puntero virtual).
- **Funcional**: Trato de funciones como punteros ejecutables (`PTR` a etiquetas).
