# 🚀 INICIO RÁPIDO: LENGUAJE DE PROGRAMACIÓN UPL
## Lenguaje de Programación de Sistemas y Alto Rendimiento para el Siglo XXI

Bienvenido a la documentación técnica de **UPL (Universal Polyglot Layer)**. UPL es un **lenguaje de programación de sistemas compilado y autónomo**, diseñado para proporcionar rendimiento a nivel de silicio, seguridad de memoria sin sobrecarga en tiempo de ejecución e interoperabilidad multi-arquitectura.

---

### 🌟 Pilares de la Arquitectura de UPL

1. **Compilador Nativo Dedicado (`uplc`):** Desarrollado en ANSI C99 autónomo, con análisis léxico propio, análisis sintáctico de descenso recursivo, árbol de sintaxis abstracta (AST) tipado y generadores de código directo para C99 y Ensamblador x86_64 NASM.
2. **Sintaxis Bilingüe Nativa:** Soporte formal de palabras clave en Español y en Inglés en igualdad de condiciones sintácticas.
3. **Protección de Límites con Canarios de Memoria:** Inserción de bytes de guarda y canarios en cabecera y cola de estructuras `@snapshot` para mitigar desbordamientos de búfer (*buffer overflow*).
4. **Puntos de Control Deterministas (`@snapshot`):** Serialización atómica del estado de memoria en tiempo de ejecución para tolerancia a fallos y auditoría de ejecución.
5. **Aritmética Decimal de Punto Fijo (`comp3`):** Tipos BCD empaquetados nativos para cálculos financieros y científicos sin errores de redondeo de punto flotante.

---

### 📦 Tu Primer Programa en UPL

Crea un archivo llamado `hola_mundo.upl`:

```rust
module demo;

pub fn main() -> i32 {
    let mut contador: i32 = 0;
    while contador < 5 {
        contador = contador + 1;
    }
    return 0;
}
```

O utilizando palabras clave en Español:

```rust
modulo demo;

funcion principal() -> i32 {
    sea mut contador: i32 = 0;
    mientras contador < 5 {
        contador = contador + 1;
    }
    retornar 0;
}
```

---

### ⚡ Cómo Compilar y Ejecutar

Desde la terminal en el directorio raíz del proyecto:

```bash
# 1. Ejecución directa en memoria (Windows: upl.bat / POSIX: ./upl.sh):
upl run hola_mundo.upl

# 2. Compilación a binario ejecutable optimizado:
upl build hola_mundo.upl -o hola_mundo.exe

# 3. Transpilación a código C99 estándar:
upl build hola_mundo.upl -t c -o salida.c

# 4. Transpilación a Ensamblador NASM x86_64:
upl build hola_mundo.upl -t asm -o salida.asm

# 5. Ejecutar la suite completa de tests automatizados:
upl test
```
