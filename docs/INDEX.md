# 📚 UPL+ Documentation Index — Multilingual Hub

> **UPL+ Universal Polyglot Layer** — Official Documentation Index  
> **Version:** v2.3.0 (stable) · v2.4.0 (in development)  
> **Author:** José Manuel Moreno (Noxferion) · [@cyberenigma-lgtm](https://github.com/cyberenigma-lgtm)

---

## 🌐 Choose Your Language / Elige tu Idioma

| Language | Quickstart | Compiler Architecture | Syntax & Grammar |
|----------|------------|----------------------|-----------------|
| 🇬🇧 **English** | [Quickstart](en/QUICKSTART.md) | [Architecture](en/COMPILER_ARCHITECTURE.md) | [Syntax](en/SYNTAX_AND_GRAMMAR.md) |
| 🇪🇸 **Español** | [Guía Oficial de Sintaxis](GUIA_OFICIAL_SINTAXIS_Y_COMANDOS_UPL.md) | [Arquitectura](ARQUITECTURA_COMPILADOR.md) | [Gramática](SPEC_GRAMATICA_Y_SISTEMA_TIPOS.md) |
| 🇨🇳 **中文** | [快速入门](zh/QUICKSTART.md) | [编译器架构](zh/COMPILER_ARCHITECTURE.md) | [语法规范](zh/SYNTAX_AND_GRAMMAR.md) |
| 🇫🇷 **Français** | [Démarrage Rapide](fr/QUICKSTART.md) | [Architecture](fr/COMPILER_ARCHITECTURE.md) | [Syntaxe](fr/SYNTAX_AND_GRAMMAR.md) |
| 🇵🇹 **Português** | [Início Rápido](pt/QUICKSTART.md) | [Arquitetura](pt/COMPILER_ARCHITECTURE.md) | [Sintaxe](pt/SYNTAX_AND_GRAMMAR.md) |

---

## 📖 All Documentation (Español — Base Canónica)

### Core Language

| Document | Description |
|----------|-------------|
| [MANUAL_OFICIAL_DEL_LENGUAJE_UPL.md](MANUAL_OFICIAL_DEL_LENGUAJE_UPL.md) | Manual oficial completo del lenguaje UPL+ |
| [SPEC_GRAMATICA_Y_SISTEMA_TIPOS.md](SPEC_GRAMATICA_Y_SISTEMA_TIPOS.md) | Especificación formal EBNF y sistema de tipos |
| [GUIA_OFICIAL_SINTAXIS_Y_COMANDOS_UPL.md](GUIA_OFICIAL_SINTAXIS_Y_COMANDOS_UPL.md) | Guía completa de sintaxis y comandos CLI |
| [ARQUITECTURA_COMPILADOR.md](ARQUITECTURA_COMPILADOR.md) | Pipeline de compilación, fases y componentes internos |
| [TABLA_MAESTRA_9_IDIOMAS_UPL.md](TABLA_MAESTRA_9_IDIOMAS_UPL.md) | Tabla de palabras clave en 9 idiomas |

### Testing & Quality

| Document | Description |
|----------|-------------|
| [SUITE_DE_PRUEBAS.md](SUITE_DE_PRUEBAS.md) | Suite completa de pruebas y casos verificados |
| [BENCHMARKS_OFICIALES_UPL_VS_C_VS_RUST.md](BENCHMARKS_OFICIALES_UPL_VS_C_VS_RUST.md) | Benchmarks oficiales UPL+ vs C99 vs Rust |
| [DICCIONARIO_MAESTRO_CODIGOS_ERROR_MULTI_SO.md](DICCIONARIO_MAESTRO_CODIGOS_ERROR_MULTI_SO.md) | Diccionario maestro de códigos de error multi-SO |

### Advanced & Case Studies

| Document | Description |
|----------|-------------|
| [CASO_ESTUDIO_WARHAMMER_MEMORY_GUARD.md](CASO_ESTUDIO_WARHAMMER_MEMORY_GUARD.md) | Caso de estudio: Memory Guard en entorno de juego |
| [INFORME_TECNICO_GRAFICOS_ULTRA_UPL_VS_MOTORES.md](INFORME_TECNICO_GRAFICOS_ULTRA_UPL_VS_MOTORES.md) | UPL+ vs motores gráficos: análisis técnico |
| [INFORME_TECNICO_TITAN_ULTRA_VS_PS5.md](INFORME_TECNICO_TITAN_ULTRA_VS_PS5.md) | Informe técnico Titan Ultra vs PS5 |
| [MANIFIESTO_DIFERENCIAL_UPL_VS_RESTO_DEL_MUNDO.md](MANIFIESTO_DIFERENCIAL_UPL_VS_RESTO_DEL_MUNDO.md) | Manifiesto diferencial UPL+ vs el mundo |
| [PLAN_ESTRATEGICO_ADOPCION_GLOBAL_UPL.md](PLAN_ESTRATEGICO_ADOPCION_GLOBAL_UPL.md) | Plan estratégico de adopción global |
| [MATRIZ_CONVIVENCIA_POLIGLOTA_UNIVERSAL.md](MATRIZ_CONVIVENCIA_POLIGLOTA_UNIVERSAL.md) | Matriz de convivencia políglota universal |
| [MATRIZ_TOTAL_34_LENGUAJES_INDUSTRIA.md](MATRIZ_TOTAL_34_LENGUAJES_INDUSTRIA.md) | Comparativa con 34 lenguajes de la industria |

### Modules & Integration

| Document | Description |
|----------|-------------|
| [modules/MANUAL_DE_MODULOS_SISTEMAS.md](modules/MANUAL_DE_MODULOS_SISTEMAS.md) | Manual de módulos de sistemas |
| [modules/GUIA_DE_INTEGRACION_Y_EJEMPLOS.md](modules/GUIA_DE_INTEGRACION_Y_EJEMPLOS.md) | Guía de integración y ejemplos |
| [NUASM_TROUBLESHOOTING.md](NUASM_TROUBLESHOOTING.md) | NuASM: solución de problemas |

### Toolchain & Roadmap (English)

| Document | Description |
|----------|-------------|
| [MCP_SERVER.md](MCP_SERVER.md) | MCP Server setup, tools, and IDE integration |
| [ROADMAP_v2.4.0.md](ROADMAP_v2.4.0.md) | v2.4.0 development roadmap and checklist |

---

## 🗂️ Documentation Structure

```
docs/
├── INDEX.md                          ← You are here / Estás aquí
├── en/                               🇬🇧 English
│   ├── QUICKSTART.md
│   ├── COMPILER_ARCHITECTURE.md
│   └── SYNTAX_AND_GRAMMAR.md
├── zh/                               🇨🇳 中文
│   ├── QUICKSTART.md
│   ├── COMPILER_ARCHITECTURE.md
│   └── SYNTAX_AND_GRAMMAR.md
├── fr/                               🇫🇷 Français
│   ├── QUICKSTART.md
│   ├── COMPILER_ARCHITECTURE.md
│   └── SYNTAX_AND_GRAMMAR.md
├── pt/                               🇵🇹 Português
│   ├── QUICKSTART.md
│   ├── COMPILER_ARCHITECTURE.md
│   └── SYNTAX_AND_GRAMMAR.md
├── MCP_SERVER.md                     (EN) MCP server guide
├── ROADMAP_v2.4.0.md                 (EN) v2.4.0 roadmap
├── ARQUITECTURA_COMPILADOR.md        (ES) Compiler architecture
├── SPEC_GRAMATICA_Y_SISTEMA_TIPOS.md (ES) Formal grammar spec
├── MANUAL_OFICIAL_DEL_LENGUAJE_UPL.md (ES) Official language manual
├── GUIA_OFICIAL_SINTAXIS_Y_COMANDOS_UPL.md (ES) CLI guide
└── modules/                          (ES) Module documentation
```

---

## 🚀 Quick Links

- **GitHub:** [cyberenigma-lgtm/UNIVERSAL-POLYGLOT-LAYER-UPL](https://github.com/cyberenigma-lgtm/UNIVERSAL-POLYGLOT-LAYER-UPL)
- **Latest Release:** [v2.3.0](https://github.com/cyberenigma-lgtm/UNIVERSAL-POLYGLOT-LAYER-UPL/releases/tag/v2.3.0)
- **Wiki:** [GitHub Wiki](https://github.com/cyberenigma-lgtm/UNIVERSAL-POLYGLOT-LAYER-UPL/wiki)
- **Issues:** [Report a Bug](https://github.com/cyberenigma-lgtm/UNIVERSAL-POLYGLOT-LAYER-UPL/issues)

---

*UPL+ — The Universal Polyglot Layer. Write once, speak all languages.*  
*UPL+ — La Capa Políglota Universal. Escribe una vez, habla todos los idiomas.*
