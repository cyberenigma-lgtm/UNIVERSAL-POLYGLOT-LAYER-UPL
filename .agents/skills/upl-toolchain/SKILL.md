---
name: upl-toolchain
description: Guía y procedimientos para compilar, ejecutar, empaquetar y depurar aplicaciones y módulos en UPL (Universal Polyglot Layer) con el compilador nativo UPLC.
---

# UPL Toolchain Workflow Guide

Utiliza esta habilidad para interactuar con la toolchain nativa de UPL, compilar programas `.upl`, empaquetar binarios y utilizar los módulos del ecosistema.

## 1. Comandos Principales de UPLC

El compilador C99 se encuentra en `bin/uplc.exe` (invocable también como `upl` o `./bin/uplc`):

```bash
# Ejecución directa (compila a C en memoria, enlaza y corre inmediatamente):
./bin/uplc.exe run <archivo.upl>

# Compilación a binario ejecutable (.exe / nativo):
./bin/uplc.exe build <archivo.upl> -o <salida.exe>

# Transpilación a código C99 puro:
./bin/uplc.exe build <archivo.upl> -t c -o <salida.c>

# Transpilación a Ensamblador NASM x86_64:
./bin/uplc.exe build <archivo.upl> -t asm -o <salida.asm>

# Listar módulos de sistemas registrados en el ecosistema:
./bin/uplc.exe list
```

## 2. Empaquetado Multi-Target

UPLC incluye empaquetado multi-distribución integrado:

```bash
# Generar paquete Debian / Ubuntu (.deb):
./bin/uplc.exe build <archivo.upl> -t deb

# Generar paquete RedHat / Fedora (.rpm):
./bin/uplc.exe build <archivo.upl> -t rpm

# Generar Bundle para macOS (.app):
./bin/uplc.exe build <archivo.upl> -t app

# Generar paquete Android (.apk):
./bin/uplc.exe build <archivo.upl> -t apk
```

## 3. Uso de Módulos del Sistema

Para cargar módulos en cualquier archivo `.upl`, escribe al inicio:

```rust
import urep_guard;
import vector_ops;
// O en español:
importar urep_guard;
importar vector_ops;
```

El compilador buscará los módulos recursivamente en `modules/` y en las rutas adicionales pasadas con `-I <ruta>`.
