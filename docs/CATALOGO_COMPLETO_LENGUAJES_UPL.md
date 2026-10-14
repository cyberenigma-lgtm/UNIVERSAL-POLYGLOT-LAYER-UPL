# 🌍 CATÁLOGO UNIVERSAL DE LENGUAJES SOPORTADOS POR UPL
## Universal Polyglot Layer (UPL) — El Ecosistema de Programación y Soberanía Total

UPL opera en dos dimensiones lingüísticas simultáneas e incomparables en la industria del software:
1. **Dimensión de Lenguajes de Programación:** Capacidad nativa de compilar código soberano UPL, interactuar mediante FFI estándar y absorber bloques de código de más de **30 lenguajes de programación** mediante las directivas `embed <lang> { ... }` y ensamblador directo `asm { ... }`.
2. **Dimensión de Soberanía e Idiomas Naturales:** Soporte oficial de localización, diagnósticos y ensamblador bilingüe en **57 idiomas del mundo**, permitiendo que cualquier desarrollador programe y depure sin barreras anglófonas.

---

## 💻 1. LENGUAJES DE PROGRAMACIÓN SOPORTADOS

El compilador y la arquitectura de UPL soportan interoperabilidad, compilación cruzada y absorción en diferentes Tiers de rendimiento:

| Tier | Categoría | Lenguajes Soportados | Nivel de Integración en UPL |
|:---:|---|---|---|
| **Tier 0** | **Sistemas y Silicio Directo** | **UPL (Nativo)**, **Ensamblador x86_64**, **C (C89, C99, C11, C23)**, **C++ (C++11 a C++23)**, **Rust**, **Zig** | Generación de código nativo, acceso directo a registros, inline `asm { }`, enlace directo C-ABI y modo `@baremetal` sin runtime. |
| **Tier 1** | **Alto Rendimiento y Empresa** | **Go**, **Java**, **C# (.NET)**, **Swift**, **Kotlin** | Interoperabilidad mediante librerías dinámicas (`.dll` / `.so`), FFI sin overhead y mapeo de memoria zero-copy. |
| **Tier 2** | **Scripting, Conectividad y Web** | **Python**, **JavaScript / TypeScript**, **Lua**, **Ruby**, **PHP**, **Perl**, **Bash / Shell**, **PowerShell** | Incrustación directa mediante `embed <lang> { }`, pasarela de datos C-ABI y ejecución JIT. |
| **Tier 3** | **Científico, IA y Matemáticas** | **Julia**, **Mojo**, **R**, **MATLAB / Octave** | Aceleración vectorial DVTRGAS-30 (52.80 GB/s) y pasarela de tensores en memoria continua. |
| **Tier 4** | **Financiero y Misión Crítica** | **COBOL (NeuroCOBOL COMP-3)**, **Fortran**, **Ada**, **BASIC** | Tipos exactos BCD `comp3` con sello `0x4A4D4D43`, eliminando errores de redondeo IEEE 754. |
| **Tier 5** | **Funcionales y Concurrentes** | **Haskell**, **Elixir**, **Erlang**, **Lisp / Scheme**, **OCaml**, **Clojure** | Mapeo de funciones puras (`@pure`), inmutabilidad estricta por defecto y concurrencia soberana. |
| **Tier 6** | **Modernos y Emergentes** | **Nim**, **Crystal**, **D**, **V** | Enlace estático nativo y transpilación C99 directa. |

---

## 🗣️ 2. LOS 57 IDIOMAS DEL MUNDO SOPORTADOS POR UPL

Gracias a la integración con **MultiLang-ASM** y **NeuroUniversalASM**, el compilador `uplc`, su consola interactiva (REPL) y su motor de diagnóstico entienden instrucciones, directivas y mensajes en **57 idiomas y dialectos**:

| # | Código | Idioma / Lengua | Nombre Nativo | Región Principal | Módulo de Soporte |
|:---:|:---:|---|---|---|:---:|
| 1 | `es` | Español | Español / Castellano | España, Hispanoamérica | `multilang_asm/es.py` |
| 2 | `en` | Inglés | English | Global, Internacional | `multilang_asm/en.py` |
| 3 | `fr` | Francés | Français | Francia, Canadá, África | `multilang_asm/fr.py` |
| 4 | `de` | Alemán | Deutsch | Alemania, Austria, Suiza | `multilang_asm/de.py` |
| 5 | `ru` | Ruso | Русский | Rusia, Eurasia | `multilang_asm/ru.py` |
| 6 | `zh` | Chino Mandarín | 中文 (简体) | China, Taiwán, Singapur | `multilang_asm/zh.py` |
| 7 | `ja` | Japonés | 日本語 | Japón | `multilang_asm/ja.py` |
| 8 | `ko` | Coreano | 한국어 | Corea del Sur | `multilang_asm/ko.py` |
| 9 | `pt` | Portugués | Português | Portugal, Brasil, Angola | `multilang_asm/pt.py` |
| 10 | `it` | Italiano | Italiano | Italia, Suiza | `multilang_asm/it.py` |
| 11 | `ar` | Árabe | العربية | Medio Oriente, Norte de África | `multilang_asm/ar.py` |
| 12 | `hi` | Hindi | हिन्दी | India | `multilang_asm/hi.py` |
| 13 | `bn` | Bengalí | বাংলা | Bangladés, India | `multilang_asm/bn.py` |
| 14 | `vi` | Vietnamita | Tiếng Việt | Vietnam | `multilang_asm/vi.py` |
| 15 | `tr` | Turco | Türkçe | Turquía, Chipre | `multilang_asm/tr.py` |
| 16 | `pl` | Polaco | Polski | Polonia | `multilang_asm/pl.py` |
| 17 | `nl` | Neerlandés | Nederlands | Países Bajos, Bélgica | `multilang_asm/nl.py` |
| 18 | `id` | Indonesio | Bahasa Indonesia | Indonesia | `multilang_asm/id.py` |
| 19 | `ms` | Malayo | Bahasa Melayu | Malasia, Brunéi | `multilang_asm/ms.py` |
| 20 | `sv` | Sueco | Svenska | Suecia, Finlandia | `multilang_asm/sv.py` |
| 21 | `no` | Noruego | Norsk | Noruega | `multilang_asm/no.py` |
| 22 | `fi` | Finés | Suomi | Finlandia | `multilang_asm/fi.py` |
| 23 | `el` | Griego | Ελληνικά | Grecia, Chipre | `multilang_asm/el.py` |
| 24 | `he` | Hebreo | עברית | Israel | `multilang_asm/he.py` |
| 25 | `fa` | Persa (Farsi) | فارسی | Irán, Afganistán | `multilang_asm/fa.py` |
| 26 | `th` | Tailandés | ไทย | Tailandia | `multilang_asm/th.py` |
| 27 | `tl` | Tagalo / Filipino | Wikang Tagalog | Filipinas | `multilang_asm/tl.py` |
| 28 | `uk` | Ucraniano | Українська | Ucrania | `multilang_asm/uk.py` |
| 29 | `ro` | Rumano | Română | Rumania, Moldavia | `multilang_asm/ro.py` |
| 30 | `ca` | Catalán | Català | España (Cataluña, Valencia, Baleares) | `multilang_asm/ca.py` |
| 31 | `eu` | Euskera | Euskara | España / Francia (País Vasco) | `multilang_asm/eu.py` |
| 32 | `gl` | Gallego | Galego | España (Galicia) | `multilang_asm/gl.py` |
| 33 | `nah`| Náhuatl | Nāhuatlahtōlli | México | `multilang_asm/nah.py` |
| 34 | `qu` | Quechua | Runa Simi | Perú, Bolivia, Ecuador | `multilang_asm/qu.py` |
| 35 | `ay` | Aimara | Aymar Aru | Bolivia, Perú, Chile | `multilang_asm/ay.py` |
| 36 | `sw` | Suajili | Kiswahili | Kenia, Tanzania, Uganda | `multilang_asm/sw.py` |
| 37 | `yo` | Yoruba | Èdè Yorùbá | Nigeria, Benín | `multilang_asm/yo.py` |
| 38 | `ig` | Igbo | Asụsụ Igbo | Nigeria | `multilang_asm/ig.py` |
| 39 | `ha` | Hausa | Harshen Hausa | Nigeria, Níger, Ghana | `multilang_asm/ha.py` |
| 40 | `zu` | Zulú | isiZulu | Sudáfrica | `multilang_asm/zu.py` |
| 41 | `af` | Afrikáans | Afrikaans | Sudáfrica, Namibia | `multilang_asm/af.py` |
| 42 | `am` | Amhárico | አማርኛ | Etiopía | `multilang_asm/am.py` |
| 43 | `ga` | Irlandés | Gaeilge | Irlanda | `multilang_asm/ga.py` |
| 44 | `jv` | Javanés | Basa Jawa | Indonesia (Java) | `multilang_asm/jv.py` |
| 45 | `su` | Sondanés | Basa Sunda | Indonesia (Sonda) | `multilang_asm/su.py` |
| 46 | `ta` | Tamil | தமிழ் | India, Sri Lanka, Singapur | `multilang_asm/ta.py` |
| 47 | `te` | Telugu | తెలుగు | India | `multilang_asm/te.py` |
| 48 | `tpi`| Tok Pisin | Tok Pisin | Papúa Nueva Guinea | `multilang_asm/tpi.py` |
| 49 | `mi` | Maorí | Te Reo Māori | Nueva Zelanda | `multilang_asm/mi.py` |
| 50 | `ber`| Bereber | Tamaziɣt | Marruecos, Argelia | `multilang_asm/ber.py` |
| 51 | `ak` | Akan / Twi | Twi / Akan | Ghana | `multilang_asm/ak.py` |
| 52 | `yue`| Cantonés | 粵語 (廣東話) | Hong Kong, Macao, Cantón | `multilang_asm/yue.py` |
| 53 | `es_ar`| Español (Rioplatense) | Español de Argentina | Argentina, Uruguay | `multilang_asm/es_ar.py` |
| 54 | `es_mx`| Español (Mexicano) | Español de México | México | `multilang_asm/es_mx.py` |
| 55 | `fr_be`| Francés (Bélgica) | Français de Belgique | Bélgica | `multilang_asm/fr_be.py` |
| 56 | `en_sc`| Escocés | Scots English | Escocia | `multilang_asm/en_sc.py` |
| 57 | `may`| Maya Yucateco | Maaya t'aan | México, Guatemala | `multilang_asm/may.py` |

---

## ⚡ CÓMO USAR CUALQUIER IDIOMA EN UPL

### 1. En la Línea de Comandos:
```bash
# Diagnosticar en Japonés
upl diag 0xC0000005 -l ja

# Compilar con reportes en Alemán
upl build mi_programa.upl -t c -l de

# Iniciar la Consola Interactiva en Náhuatl o Francés
upl repl -l nah
upl repl -l fr
```

### 2. Dentro de la Consola Interactiva (REPL):
```
>>> :lang ar       # Cambia el REPL a Árabe
>>> :lang ru       # Cambia el REPL a Ruso
>>> :lang zh       # Cambia el REPL a Chino
>>> :lang es       # Regresa a Español
```
