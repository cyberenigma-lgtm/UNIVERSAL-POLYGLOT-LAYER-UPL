# 🚀 INICIO RÁPIDO: LENGUAJE DE PROGRAMACIÓN UPL
## El Nuevo Lenguaje Soberano de Sistemas para el Siglo XXI

Bienvenido a la documentación oficial de **UPL (Universal Polyglot Layer)**. UPL no es una librería ni un script de automatización: es un **lenguaje de programación de sistemas totalmente nuevo**, creado para proporcionar el máximo rendimiento sobre el silicio con una garantía de seguridad superior a Rust y C++.

---

### 🌟 ¿Por qué UPL es un Lenguaje Totalmente Nuevo?

1. **Compilador Nativo Dedicado (`uplc`):** Desarrollado con análisis léxico propio, análisis sintáctico de descenso recursivo, árbol de sintaxis abstracta (AST) tipado y generadores de código para C99 y Ensamblador x86_64 NASM puro.
2. **Soberanía Bilingüe:** Primer lenguaje en el mundo con soporte nativo de sintaxis completa en Español y en Inglés en pie de igualdad.
3. **Landing-Pads DVTRGAS-30 (0xAA30):** Protección física en memoria con guardas automáticas de silicio que absorben desbordamientos antes de que dañen otros datos.
4. **Auto-Sanación con Salto Lógico (`@snapshot`):** En lugar de crashear como Rust ante un fallo imprevisto, UPL restaura el último punto de control válido en nanosegundos y continúa la ejecución.
5. **Aritmética Exacta NeuroCOBOL (`comp3`):** Tipos BCD nativos que eliminan los errores de precisión en finanzas y cálculos críticos.

---

### 📦 Tu Primer Programa en UPL

Crea un archivo llamado `hola_soberano.upl`:

```rust
module demo;

pub fn main() -> i32 {
    let mut contador: i32 = 0;
    while contador < 5 {
        contador = contador + 1;
    }
    return contador;
}
```

O si prefieres programar en Español:

```rust
modulo demo;

funcion principal() -> i32 {
    sea mutable contador: i32 = 0;
    mientras contador < 5 {
        contador = contador + 1;
    }
    retornar contador;
}
```

---

### ⚡ Cómo Ejecutar y Compilar

Desde la terminal en el directorio raíz de UPL:

```bash
# Ejecución directa e inmediata nativa (Windows: upl.bat / Linux: ./upl.sh)
upl run hola_soberano.upl

# Compilación a código C99 optimizado
upl build hola_soberano.upl -t c -o salida.c

# Compilación a Ensamblador x86_64 Long Mode (Bare-Metal / Ring 0)
upl build hola_soberano.upl -t asm --baremetal -o salida.asm
```

---

### 📚 Próximos Pasos en la Wiki
* **[Manual de Sintaxis y Gramática](SINTAXIS_Y_GRAMATICA.md)**
* **[Arquitectura Interna del Compilador](COMPILADOR_Y_ARQUITECTURA.md)**
* **[Diccionario de Errores y Diagnóstico Multi-SO](DICCIONARIO_ERRORES.md)**
* **[Galería de Ejemplos en Código UPL](EJEMPLOS.md)**
