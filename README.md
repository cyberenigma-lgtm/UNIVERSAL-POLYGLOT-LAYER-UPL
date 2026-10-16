# UPL+: Universal Polyglot Layer Plus — Systems Programming Language & Toolchain
## Lenguaje de Programación de Sistemas de Alto Rendimiento y Toolchain Nativa Independiente
### High-Performance Systems Language, Standalone Native Compiler & Bare-Metal Toolchain
**Autor:** José Manuel Moreno (Noxferion)

[![SafeCreative Protected](https://img.shields.io/badge/SafeCreative-Obra%20Protegida-blue.svg)](https://www.safecreative.org)
[![Language](https://img.shields.io/badge/Language-UPL%2B%20v1.5-green.svg)](docs/GUIA_OFICIAL_SINTAXIS_Y_COMANDOS_UPL.md)
[![Toolchain](https://img.shields.io/badge/Toolchain-Native%20x86__64-blue.svg)](#-pipeline-de-compilaci%C3%B3n)
[![Backends](https://img.shields.io/badge/Backends-ANSI%20C99%20%7C%20x86__64%20NASM-orange.svg)](#-pipeline-de-compilaci%C3%B3n)
[![Localization](https://img.shields.io/badge/Localization-57%20Languages-purple.svg)](multilang/)

> **COMUNICADO DE PROPIEDAD INTELECTUAL:**  
> ***UPL+*** *(Universal Polyglot Layer Plus)* es la suite y compilador oficial derivado de la obra protegida **Universal Polyglot Layer (UPL)**. Todas las especificaciones gramaticales, compiladores nativos (`uplc` / `upl`), directivas de bajo nivel (`@snapshot`, `@baremetal`), motores decimales exactos y arquitecturas asociadas están registradas bajo el amparo legal de **SafeCreative**.

---

![UPL & NUASM Identity](assets/logo_upl_nuasm.jpg)

---

## 🇪🇸 [Español] - Especificación Técnica de UPL+

### ¿Qué es UPL+?
**UPL+ (Universal Polyglot Layer Plus)** es un **lenguaje de programación de sistemas compilado, determinista y de alto rendimiento**, diseñado para desarrollo de sistemas operativos, motores embebidos, firmware bare-metal y aplicaciones de misión crítica.

### Pilares de Ingeniería de Software
1. **Seguridad de Memoria Determinista:** Incorpora canarios de protección de memoria (`guard canaries`) en estructuras de datos y soporte nativo para puntos de control de estado (`@snapshot`), permitiendo aislamiento de fallos sin sobrecarga de recolector de basura (GC).
2. **Acceso Directo a Silicio y Modo Bare-Metal:** La directiva `@baremetal` y bloques `asm { ... }` permiten generar código máquina para Ring 0, controlando registros de CPU, interrupciones y periféricos sin librerías del sistema operativo anfitrión.
3. **Aritmética Decimal Exacta (Fixed-Point BCD):** Tipos de datos `comp3(precision, scale)` nativos para cálculos financieros y matemáticos exactos, eliminando la imprecisión de coma flotante de la norma IEEE-754.
4. **Toolchain 100% Nativa y Autónoma:** Compilador independiente escrito en C99 y UPL+ (`compiler/uplc.c`), capaz de autogestionar el ciclo de compilación a C99 y Ensamblador x86_64 NASM sin requerir entornos interpretados, Python ni dependencias externas.
5. **Lexer Bilingüe (Español / Inglés):** Admite palabras clave universales en inglés (`fn`, `let`, `mut`, `struct`, `if`, `while`, `return`) o en español técnico (`funcion`, `sea`, `mutable`, `estructura`, `si`, `mientras`, `retornar`).

---

## 🇬🇧 [English] - UPL+ Technical Specification

### What is UPL+?
**UPL+ (Universal Polyglot Layer Plus)** is a **compiled, deterministic systems programming language** engineered for operating system kernels, bare-metal firmware, embedded engines, and mission-critical high-performance computing.

### Core Engineering Features
1. **Deterministic Memory Safety:** Memory canary guard protection and atomic checkpointing (`@snapshot`) provide robust fault recovery without garbage collection overhead.
2. **Bare-Metal & Direct Silicon Control:** The `@baremetal` directive and inline `asm` blocks compile directly to Ring 0 machine code, enabling full control over CPU registers and hardware ports with zero libc dependencies.
3. **Fixed-Point Decimal Arithmetic:** Native `comp3` BCD types deliver exact decimal representations for financial and scientific systems where IEEE-754 binary floating-point drift is unacceptable.
4. **Self-Contained Native Toolchain:** Autonomous compiler implementation in ANSI C99 and UPL+ (`compiler/uplc.c`), producing standalone binaries via C99 and x86_64 NASM backends with zero virtual machine requirements.
5. **Bilingual Grammar Support:** Native keyword parsing in both Spanish and English at the lexer level.

---

## 🏛️ Estructura del Repositorio / Repository Layout

```
UNIVERSAL-POLYGLOT-LAYER-UPL/
├── compiler/                  # Compilador nativo UPLC (C99 y código fuente UPL+)
├── docs/                      # Especificación técnica, gramática y arquitectura
├── examples/                  # Programas de prueba, benchmarks y demos en .upl
├── dvtrgas30_engine/          # Motor de verificación e integridad canónica
├── neurocobol_engine/         # Motor de aritmética BCD fija COMP-3
├── wiki/                      # Wiki técnica de referencia (Español / English)
├── upl.bat                    # Script de compilación y ejecución nativa (Windows)
├── upl.sh                     # Script de compilación y ejecución nativa (Linux / macOS)
└── README.md                  # Especificación técnica principal
```

---

## ⚡ Guía de Uso Rápido / Quickstart

```bash
# 1. Compilar y ejecutar un programa UPL directamente
upl run examples/ejemplo_basico.upl

# 2. Compilar a código ANSI C99 optimizado
upl build examples/ejemplo_basico.upl -t c -o salida.c

# 3. Compilar a Ensamblador x86_64 NASM para modo Bare-Metal (Ring 0)
upl build examples/ejemplo_basico.upl -t asm --baremetal -o kernel.asm

# 4. Comprobar la versión de la toolchain
upl --version
```

---

## 📖 Documentación Técnica
* **[Guía Oficial de Sintaxis y Comandos](docs/GUIA_OFICIAL_SINTAXIS_Y_COMANDOS_UPL.md)**
* **[Especificación de la Gramática](docs/SPEC_MOTHER_LANGUAGE.md)**
* **[Catálogo de Lenguajes Soportados](docs/CATALOGO_COMPLETO_LENGUAJES_UPL.md)**
* **[Wiki Oficial de Desarrollo](wiki/INDEX.md)**

---

## 📜 Licencia y Registro Legal
* **Registro de Propiedad:** Obra registrada y protegida en SafeCreative.
* **Licencia:** Consulta el archivo [LICENSE](LICENSE) para patentes defensivas y términos de uso del software.
