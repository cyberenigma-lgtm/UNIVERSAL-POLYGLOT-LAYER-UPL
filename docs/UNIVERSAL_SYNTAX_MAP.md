# Universal Syntax Map (USM) - Concepto de Unificación

El **USM** es el diccionario de equivalencias lógicas de UPL. Su función es garantizar que, sin importar cómo se escriba una instrucción en un lenguaje origen, el **Lenguaje Madre (UPL-IR)** reciba la misma orden lógica.

## 🗺️ Mapeo de Conceptos Base

| Concepto Lógico | C / C++ | Python | BASIC | UPL-IR (LENGUAJE MADRE) |
| :--- | :--- | :--- | :--- | :--- |
| **Asignación** | `int a = 10;` | `a = 10` | `LET A = 10` | `ASSIGN $A, 10` |
| **Suma** | `a + b` | `a + b` | `A + B` | `ADD $RES, $A, $B` |
| **Bucle For** | `for(i=0;...` | `for i in range` | `FOR I = 1 TO...` | `LOOP_FINITE $I, $MIN, $MAX` |
| **Función** | `void func()...`| `def func()...` | `GOSUB...` | `PROC <NAME>` |
| **Salida** | `printf()` | `print()` | `PRINT` | `IO_OUT <DEV>, <SRC>` |

## 🧠 Lógica de Traducción
1. **Identificación**: El parser detecta el lenguaje del bloque de código.
2. **Abstracción**: Busca la equivalencia en el USM para cada token.
3. **Normalización**: Genera el código UPL-IR estandarizado.
4. **Validación**: Verifica que los tipos de datos abstractos sean compatibles.

## 📁 Estructura de Mapeos Individuales
Cada lenguaje en `catalog/` tendrá un archivo `mapping_<id>.json` que define sus tokens específicos y su traducción al USM.
