# Registro de Cambios (Changelog) - Universal Polyglot Layer (UPL+)

Todas las modificaciones notables de UPL+ se documentan en este archivo conforme al estándar [Keep a Changelog](https://keepachangelog.com/es-ES/1.1.0/).

---

## [2.3.0] - 2026-11-28 — Sovereign Systems & Tier-1 Upstream Verification Edition

### Resumen de la Versión
Esta versión mayor consolida a UPL+ como lenguaje y toolchain de ingeniería de sistemas soberana, incorporando el Quickstart multiplataforma de 30 segundos (`examples/quickstart_30s.upl`), el motor nativo de auditoría (`uplc audit`), y las lecciones y patrones arquitectónicos derivados de auditorías técnicas sobre bases de código de producción de Nivel 1 (Microsoft .NET Runtime, Windows PowerToys `#51448`, `#50647`, `#51546`, `#51077`, `#52414`, Mono WASM Runtime, Win32 Kernel, HTTP/2 Sockets).

### Novedades y Características Principales

#### 1. Motor de Auditoría Estática Formal Integrado (`uplc audit`)
- **Subcomando nativo:** `uplc audit <archivo.upl> [-o reporte.md]` evalúa formalmente en tiempo de compilación y análisis estático:
  - Ciclo de vida y afinidad de recursos (aperturas vs limpiezas `defer` LIFO).
  - Jerarquía estática de mutexes para prevención matemática de deadlocks ABBA.
  - Inmutabilidad profunda en estructuras compartidas (`shared readonly`).
- Emisión de informes estructurados en Markdown aptos para auditorías de seguridad, certificaciones aeronáuticas y pull requests técnicos.

#### 2. Cinco Nuevas Palabras Clave de Sistemas y Concurrencia
- **`own` / `propio`:** Calificador de propiedad lineal para handles y memoria nativa. Evita fugas por retorno anticipado.
- **`move` / `mover`:** Semántica de transferencia explícita de propiedad sin duplicación de descriptores.
- **`lock_rank(N)` / `rango_bloqueo(N)`:** Declaración de jerarquía y prioridad en mutexes; previene inversiones de bloqueo en compilación.
- **`shared` / `compartido`:** Marcador explícito de entidades accesibles entre múltiples hilos.
- **`readonly` / `solo_lectura`:** Garantía de inmutabilidad profunda y const transitivo en tiempo de compilación.

#### 3. Nuevos Módulos en Biblioteca Estándar (`modules/std/`)
- **`std/ring` (`ring.upl`):** Búfer circular estático y acotado con desalojo determinista $O(1)$ de entradas antiguas; elimina desbordamientos de memoria en telemetría y logging.
- **`std/slab` (`slab.upl`):** Asignador en superbloques alineados a página (4 KB) con free-list intrínseca $O(1)$; erradica la fragmentación de heap y la duplicación de páginas de memoria virtual.
- **`std/sync` (`sync.upl`):** Primitivas de sincronización jerárquica con verificación de rango ascendente, impidiendo ciclos de adquisición cruzada.
- **`std/channel` (`channel.upl`):** Sincronización multi-productor (MPSC) garantizando entrega atómica y libre de corrupción bajo alta concurrencia.
- **`net/gh_bot_radar` (`modules/net/gh_bot_radar.upl`):** Motor nativo C99/WinHTTP de detección anti-bots y granjas de *follow-farming* en GitHub con calibración **0% falsos positivos** (protección de cuentas recientes, repositorios privados y autores OSS prolíficos) + **Mini Base de Vigilancia Conductual de 5 Días** (`gh_bot_watchlist.db`) que mide la evolución temporal ($\Delta\text{following}/\text{día}$ vs commits reales) antes de emitir veredicto definitivo (`CONFIRMED_BOT` / `CLEARED_HUMAN`).

#### 4. Cumplimiento 100% de Estándares Comunitarios de GitHub
- Inclusión formal de:
  - `CODE_OF_CONDUCT.md` (Adaptación estricta de Contributor Covenant v2.1 para ingeniería de sistemas).
  - `CONTRIBUTING.md` (Norma de cero binarios commitados, compilación limpia con UPLC, pruebas 100% verdes en `test.bat`).
  - `.github/ISSUE_TEMPLATE/` (`bug_report.md`, `feature_request.md`, `system_audit_case.md`).
  - `.github/pull_request_template.md`.
  - `ACCESSIBILITY.md` (Declaración de accesibilidad WCAG 2.1 Nivel AA para terminales y documentación técnica).

#### 5. Extensiones de Entorno y Herramientas
- **Visual Studio Code / Cursor / VSCodium:** Actualización de extensión oficial a **v2.3.0** (`upl-lang-2.3.0.vsix`) con resaltado de sintaxis bilingüe para las nuevas palabras clave y tipos de sistemas.
- **Gestor de Paquetes (`upl_pm`):** Registro de nuevos módulos estándar y validación estricta SHA-256 en `upl.lock` para los 33 módulos oficiales.
- **Suite Maestra de Verificación (`test.bat`):** 8/8 fases automatizadas en verde con 0 fallos.

---

## [2.2.3] - 2026-10-29
- Módulo `std/tensor` para tensores lineales de inteligencia artificial.
- Módulo `std/crypto` con implementación en silicio de SHA-256 y CRC-32C Castagnoli por hardware.
- Canales atómicos lock-free con semántica MPSC inicial.
- Eliminación de advertencias de compilación y optimización de transpilador `c2upl`.

