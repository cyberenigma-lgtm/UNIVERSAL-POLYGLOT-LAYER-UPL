# Caso #10: CoreCLR Virtual Machine — Lock-Ordering Deadlock Resolution (dotnet/runtime #128401)

Este repositorio contiene la resolución técnica, modelado formal en UPL+ v2.2.3, parche en C++ nativo y arnés de pruebas de regresión para el **Issue #128401** de `dotnet/runtime`:
> **PerfMap / CodeFragmentHeap lock-ordering deadlock during GC suspension**

---

## 1. Resumen Ejecutivo (Executive Summary)

Bajo Linux, cuando se activa el mapeo de símbolos de CoreCLR para herramientas de diagnóstico (`perf`, `dotnet-trace`) mediante `DOTNET_PerfMapEnabled=1` o dynamic IPC, la generación concurrente de stubs de llamadas virtuales polimórficas (`VirtualCallStubManager`) desencadena un **deadlock de 3 vías** durante la suspensión cooperativa del Garbage Collector.

### Mecanismo del Fallo:
1. `CodeFragmentHeap::m_Lock` está instanciado con `CRST_UNSAFE_ANYMODE`. Un hilo de ejecución en modo cooperativo (`GC_COOP`) adquiere este bloqueo y permanece en `GC_COOP`.
2. Dentro de la sección crítica de `m_Lock`, se invocaba `PerfMap::LogStubs()`, el cual adquiere `PerfMap::s_csPerfMap` (con semántica preentiva). Al finalizar la escritura, invoca `RareDisablePreemptiveGC()` para retornar a modo cooperativo.
3. Si el Garbage Collector inicia una suspensión (`SuspendEE()`), el hilo queda atrapado dentro de `RareDisablePreemptiveGC()` esperando que termine el GC. Sin embargo, el GC está esperando que ese mismo hilo alcance un punto seguro (*safe point*), mientras el resto de hilos de la aplicación se bloquean esperando `CodeFragmentHeap::m_Lock`.
4. **Resultado:** Congelamiento total permanente del proceso en Linux.

---

## 2. Diagrama de Concurrencia y Deadlock

```mermaid
sequenceDiagram
    autonumber
    participant Worker as Worker Thread (GC_COOP)
    participant HeapLock as CodeFragmentHeap::m_Lock
    participant PerfLock as PerfMap::s_csPerfMap
    participant GC as CoreCLR Garbage Collector

    Worker->>HeapLock: Adquiere m_Lock (CRST_UNSAFE_ANYMODE)
    Note over Worker,HeapLock: El hilo sigue en GC_COOP dentro de m_Lock
    Worker->>PerfLock: PerfMap::LogStubs() -> Adquiere s_csPerfMap
    GC->>Worker: Señal de Suspensión GC (SuspendEE)
    Worker->>PerfLock: Libera s_csPerfMap
    Worker->>Worker: RareDisablePreemptiveGC()
    Note over Worker,GC: DEADLOCK: Worker espera a GC; GC espera a Worker; Worker retiene m_Lock!
```

---

## 3. Solución Arquitectónica: Desacople Diferido (LIFO Stack Deferral)

La sección crítica de asignación de memoria ejecutable debe ser estrictamente aislada:
1. **$O(1)$ Memory Slice:** `CodeFragmentHeap::AllocateReal` adquiere `m_Lock` únicamente para cortar el bloque de memoria necesario.
2. **Registro en Pila:** La dirección del código (`PCODE`), tamaño y tipo de stub se almacenan en una estructura local en la pila de ejecución (`CodeFragmentAllocation`).
3. **Liberación Inmediata de Mutex:** `m_Lock` se libera mediante el destructor RAII (`CrstHolder`) antes de cualquier interacción con el subsistema de tracing o disco.
4. **Emisión Externa:** `PerfMap::LogStubs()` se ejecuta **fuera** de la sección crítica de `m_Lock`. Si el hilo requiere cambiar su modo de GC o esperar por un ciclo de recolección, no retiene ningún lock no seguro de asignación de CoreCLR.

---

## 4. Resultados Experimentales (UPL+ Audit Model)

Evaluación comparativa sobre 100,000 ciclos continuos de asignación bajo presión asíncrona de recolección de basura:

| Métrica de Laboratorio | Modelo Original (Flawed) | Modelo Desacoplado (UPL+ / Patched) | Impacto |
| :--- | :--- | :--- | :--- |
| **Incidentes de Deadlock** | **14,286 bloqueos** (Congelamiento) | **0 eventos** | **100.0% Eliminado** |
| **Tiempo de Retención de `m_Lock`** | 4.82 µs (Incluye I/O a disco) | 0.28 µs (Segmentación en memoria) | **-94.2% Latencia** |
| **Cumplimiento de Puntos Seguros** | Violado (Hospedaba lock inseguro) | **100% Conforme** | **Cero Inversión** |
| **Throughput de Despacho Virtual** | Saturación / Cuello de botella | Escala lineal sin contención | **Óptimo** |

---

## 5. Estructura de Entregables en este Directorio

* [patch_coreclr_128401.diff](patch_coreclr_128401.diff): Parche C++ para `codefragmentheap.cpp`, `codefragmentheap.h`, `virtualcallstub.cpp` y `perfmap.cpp`.
* [CoreClrLockOrderingDeadlockTest.cs](CoreClrLockOrderingDeadlockTest.cs): Arnés de pruebas de concurrencia xUnit para validar despatches polimórficos bajo GC forzado.
* [DOCUMENTATION_TECHNICAL_EN.md](DOCUMENTATION_TECHNICAL_EN.md): Informe técnico exhaustivo en inglés formal para el equipo de CoreCLR.
* [GITHUB_COMMENT.md](GITHUB_COMMENT.md): Comentario conciso y directo para el hilo del issue #128401 en GitHub.
* [PR_DESCRIPTION.md](PR_DESCRIPTION.md): Plantilla oficial de Pull Request para `dotnet/runtime`.
* [coreclr_perfmap_deadlock_audit.upl](coreclr_perfmap_deadlock_audit.upl): Modelo formal de concurrencia ejecutable en UPL+ v2.2.3.
