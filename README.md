# 🛡️ UPL: EL NUEVO LENGUAJE DE PROGRAMACIÓN AVANZADO DEL SIGLO XXI
## Universal Polyglot Layer (UPL) — Un Lenguaje de Programación de Sistemas Totalmente Nuevo
### Standalone Next-Generation Sovereign Programming Language & Toolchain

[![SafeCreative Protected](https://img.shields.io/badge/SafeCreative-Obra%20Protegida-blue.svg)](https://www.safecreative.org)
[![Language](https://img.shields.io/badge/Language-UPL%20v1.0%20Sovereign-green.svg)](docs/GUIA_OFICIAL_SINTAXIS_Y_COMANDOS_UPL.md)
[![Backends](https://img.shields.io/badge/Backends-ANSI%20C99%20%7C%20x86__64%20NASM%20Ring--0-orange.svg)](#-pipeline-de-compilaci%C3%B3n)
[![Languages](https://img.shields.io/badge/Localization-57%20World%20Languages-purple.svg)](multilang/)

> **COMUNICADO OFICIAL DE ACTUALIZACIÓN GENERACIONAL Y PROPIEDAD INTELECTUAL:**  
> *Este repositorio presenta la transformación evolutiva oficial de **UPL**: ha dejado de ser únicamente una capa de interoperabilidad para convertirse formalmente en un **LENGUAJE DE PROGRAMACIÓN AVANZADO TOTALMENTE NUEVO**. Todas las gramáticas, compiladores nativos, directivas de silicio y arquitecturas están registradas y protegidas bajo la misma titularidad y amparo legal preexistente de **SafeCreative**.*

---

![UPL & NUASM Identity](assets/logo_upl_nuasm.jpg)

---

## 🇪🇸 [Español] - Presentación del Nuevo Lenguaje

### 💎 ¿Qué es UPL?
**UPL (Universal Polyglot Layer)** es un **lenguaje de programación de sistemas totalmente nuevo, autónomo e independiente**, diseñado desde cero para superar los tres grandes problemas del software contemporáneo:
1. **Supera la fragilidad de C y C++:** Elimina los desbordamientos de búfer mediante **Landing-Pads en silicio** y verificación de paridad canónica (`0xAA30`).
2. **Supera el bloqueo destructivo de Rust:** Rust responde ante fallos no recuperables con `panic!`, abortando el proceso o tirando el sistema. UPL introduce **Puntos de Control Atómicos (`@snapshot`) y Auto-Sanación**, restaurando el estado en nanosegundos sin interrupción de servicio.
3. **Supera la lentitud y pérdida de precisión de lenguajes tradicionales:** Incorpora tipos nativos decimales **NeuroCOBOL (`comp3`)** empaquetados en BCD, eliminando los errores de redondeo de coma flotante de IEEE-754.

### 🚀 Características Clave del Lenguaje UPL
* **Compilador Nativo Propio (`uplc` / `upl`):** No es un script ni un intérprete secundario. Cuenta con su propio tokenizador, parser de descenso recursivo, árbol AST fuertemente tipado y doble generador de código:
  * **Backend ANSI C99:** Código portable ultra-optimizado con estructuras empaquetadas.
  * **Backend Ensamblador x86_64 Long Mode (NASM):** Código máquina puro para **Ring 0 / Bare-Metal** sin runtime ni librerías del sistema anfitrión.
* **Soberanía Bilingüe Real (Español / Inglés):** Puedes programar usando palabras clave nativas en español (`funcion`, `sea`, `mutable`, `estructura`, `si`, `mientras`, `retornar`) o en inglés universal (`fn`, `let`, `mut`, `struct`, `if`, `while`, `return`).
* **Consola Interactiva Avanzada (REPL Soberano):** Permite escribir código UPL al vuelo e inspeccionar en tiempo real el C99 (`:c`), el ensamblador (`:asm`) o cambiar a 57 idiomas (`:lang ja`).
* **Diagnóstico Multi-Plataforma en 57 Idiomas:** Motor inteligente que traduce códigos de error NTSTATUS de Windows, señales POSIX de Linux y excepciones físicas de CPU x86_64.

---

## 🇬🇧 [English] - Introducing the New Advanced Language

### 💎 What is UPL?
**UPL** is a **brand-new, next-generation systems programming language** built from scratch to provide uncompromising safety, extreme bare-metal performance, and total linguistic sovereignty:
1. **Memory Safety without Borrow-Checker Hell:** Employs hardware landing-pads and canonical parity (`0xAA30`) to prevent memory corruption at the silicon level.
2. **Zero-Bug Self-Healing (Surpassing Rust):** Unlike Rust’s fatal `panic!`, UPL intercepts faults and triggers atomic checkpoint rollbacks (`@snapshot`), keeping critical systems running uninterrupted.
3. **Exact Decimal Arithmetic:** Native NeuroCOBOL `comp3` BCD types eliminate IEEE 754 rounding inaccuracies in mission-critical applications.

---

## 🏛️ Estructura del Repositorio / Repository Layout

```
UNIVERSAL-POLYGLOT-LAYER-UPL/
├── compiler/                  # Compilador nativo UPL (Lexer, Parser, AST, Backends C y ASM)
├── docs/                      # Manuales oficiales, especificación de gramática y diagnósticos
├── examples/                  # Programas de demostración en código fuente .upl
├── multilang/                 # Dicionarios de 57 idiomas para localización y ensamblador
├── dvtrgas30_engine/          # Acelerador vectorial de paridad 0xAA30
├── neurocobol_engine/         # Motor de aritmética exacta BCD COMP-3
├── wiki/                      # Wiki técnica completa (Español / English)
├── upl.py                     # CLI principal del compilador (run, build, repl, diag)
├── upl.bat                    # Lanzador rápido para Windows
├── upl.sh                     # Lanzador rápido para Linux / macOS
└── README.md                  # Este documento oficial
```

---

## ⚡ Comandos Rápidos del Compilador / Quickstart

```bash
# 1. Compilar y ejecutar un programa UPL al instante
upl run examples/ejemplo_soberano.upl

# 2. Compilar a código fuente ANSI C99 optimizado
upl build examples/ejemplo_soberano.upl -t c -o salida.c

# 3. Compilar a Ensamblador x86_64 puro para Ring 0 (Bare-Metal / Firmware)
upl build examples/ejemplo_soberano.upl -t asm --baremetal -o kernel.asm

# 4. Iniciar la Consola Interactiva Avanzada (REPL)
upl repl

# 5. Diagnosticar un código de excepción de Windows o CPU en tu idioma
upl diag 0xC0000005 -l es
```

---

## 📖 Documentación y Wiki Oficial
* **[Catálogo Completo de Lenguajes e Idiomas Soportados](docs/CATALOGO_COMPLETO_LENGUAJES_UPL.md)**
* **[Guía Oficial de Sintaxis y Comandos](docs/GUIA_OFICIAL_SINTAXIS_Y_COMANDOS_UPL.md)**
* **[Diccionario Maestro de Errores Multi-SO (57 Idiomas)](docs/DICCIONARIO_MAESTRO_CODIGOS_ERROR_MULTI_SO.md)**
* **[Manifiesto del Lenguaje UPL para el Siglo XXI](docs/MANIFIESTO_LENGUAJE_UPL_SIGLO_XXI.md)**
* **[Portal de la Wiki Soberana](wiki/INDEX.md)**

---

## 📜 Certificación y Propiedad Intelectual
* **Amparo Legal:** Obra registrada y protegida en SafeCreative.
* **Continuidad Registral:** Este compilador, gramática y herramientas constituyen la actualización evolutiva oficial de la misma unidad de obra registrada.
* **Licencia:** Consulta el archivo [LICENSE](LICENSE) para patentes defensivas y términos de uso soberano.
