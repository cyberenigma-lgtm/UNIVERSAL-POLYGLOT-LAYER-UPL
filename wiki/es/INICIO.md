# 🚀 INICIO RÁPIDO: LENGUAJE DE PROGRAMACIÓN UPL+
## Lenguaje de Programación de Sistemas y Compilador Nativo C99

Bienvenido a la documentación técnica de **UPL+ (Universal Polyglot Layer Plus)**. UPL+ es un **lenguaje de programación de sistemas compilado, fuertemente tipado y autónomo**, diseñado para proporcionar alto rendimiento, interoperabilidad sin coste con el sistema operativo y aritmética decimal exacta para entornos críticos.

---

### 🌟 Pilares de la Arquitectura de UPL+

1. **Compilador Nativo Dedicado (`uplc`):** Desarrollado en ANSI C99 modular conforme a la arquitectura canónica Dragon Book (analizador léxico bilingüe, asignador de memoria por arenas, AST tipado, parser recursivo descendente con algoritmo Pratt y generador C99).
2. **Sintaxis Bilingüe Nativa:** Soporte formal de palabras clave en Español (`funcion`, `sea`, `mut`, `estructura`, `si`, `mientras`, `retornar`) y en Inglés (`fn`, `let`, `mut`, `struct`, `if`, `while`, `return`) en igualdad de condiciones a nivel de token.
3. **Aritmética Decimal Exacta (`comp3`):** Tipos BCD empaquetados por software (`comp3(precision, escala)`) para cálculos financieros y contables sin los errores de redondeo de punto flotante de la norma IEEE-754.
4. **Interoperabilidad Nativa con C/Win32/POSIX (`embed`):** Inyección directa de bloques de código nativo sin capas de serialización ni sobrecoste de llamadas FFI.
5. **Asignación de Memoria por Arenas:** Gestión determinista de la memoria interna del compilador en bloques de 64 KB, eliminando la fragmentación y permitiendo liberaciones instantáneas $O(1)$.

---

### 📦 Tu Primer Programa en UPL+ (`hola_mundo.upl`)

```rust
modulo ejemplo;

incrustar "c" {
#include <stdio.h>
}

funcion principal() -> i32 {
    sea mut total: i32 = 0;
    sea base: i32 = 1000;
    sea tasa: i32 = 21;

    total = base + (base * tasa / 100);

    incrustar "c" {
        printf("[UPL+ NATIVO] Base: 1000, Total con IVA (21%%): %d\n", total);
    }

    retornar 0;
}
```

---

### ⚡ Cómo Compilar y Ejecutar

Desde la terminal en el directorio raíz del proyecto:

```bash
# 1. Compilación a binario ejecutable nativo (Windows: upl.bat / POSIX: ./upl.sh):
upl.bat -o hola_mundo.exe hola_mundo.upl
hola_mundo.exe

# 2. Generar únicamente el código fuente C99 sin invocar gcc:
upl.bat -c hola_mundo.upl

# 3. Inspeccionar el Árbol de Sintaxis Abstracta (AST):
upl.bat --dump-ast hola_mundo.upl

# 4. Ejecutar el arnés maestro de pruebas automatizadas:
test.bat
```

---

### 📚 Documentación Técnica Detallada
* **[Manual del Transpilador Nativo C99 a UPL+ (`c2upl`)](TRANSPILADOR_C2UPL.md)**
* **[Especificación Formal de Gramática y Tipos (EBNF)](file:///c:/Users/cyber/OneDrive/Documentos/NeuroOs/UNIVERSAL-POLYGLOT-LAYER-UPL/docs/SPEC_GRAMATICA_Y_SISTEMA_TIPOS.md)**
* **[Arquitectura Interna del Compilador C99](file:///c:/Users/cyber/OneDrive/Documentos/NeuroOs/UNIVERSAL-POLYGLOT-LAYER-UPL/docs/ARQUITECTURA_COMPILADOR.md)**
* **[Suite y Arnés de Pruebas](file:///c:/Users/cyber/OneDrive/Documentos/NeuroOs/UNIVERSAL-POLYGLOT-LAYER-UPL/docs/SUITE_DE_PRUEBAS.md)**
* **[Caso de Estudio Real: Watchdog de Memoria para Warhammer Online](file:///c:/Users/cyber/OneDrive/Documentos/NeuroOs/UNIVERSAL-POLYGLOT-LAYER-UPL/docs/CASO_ESTUDIO_WARHAMMER_MEMORY_GUARD.md)**
