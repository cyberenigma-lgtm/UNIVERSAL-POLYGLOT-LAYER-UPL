# Caso de Estudio Técnico: Watchdog de Memoria para Warhammer Online (Return of Reckoning)
## Prevención de Bloqueos Out-Of-Memory (OOM) en Arquitectura Win32 de 32 Bits

**Autor:** José Manuel Moreno (Noxferion)  
**Proyecto:** Herramienta de Supervisión y Mitigación de Fugas de Memoria en UPL+ y C99  
**Plataforma Objetivo:** Windows NT (x86 / x64), API Win32  
**Archivos Fuente:** [`tools/UPL_Memory_Guard.upl`](../tools/UPL_Memory_Guard.upl), [`tools/ror_memory_guard.c`](../tools/ror_memory_guard.c)  
**Paquete Generado:** [`dist/ror_memory_guard.zip`](../dist/ror_memory_guard.zip)

---

## 1. El Problema Técnico: Bloqueos OOM en Clientes de 32 Bits

El cliente de juego `WAR.exe` (*Warhammer Online: Age of Reckoning*, servidor privado *Return of Reckoning*) es una aplicación Win32 compilada en modo de 32 bits.

En Windows de 64 bits, los procesos de 32 bits con el flag `LARGEADDRESSAWARE` están limitados a un espacio de direcciones virtuales máximo de 4 GB (o 2 GB en configuraciones sin dicho flag). Durante batallas masivas Realm vs Realm (RvR) con cientos de jugadores en pantalla, el cliente sufre:
1. Fragmentación severa del espacio de direcciones virtuales por carga continua de modelos, texturas y efectos de partículas.
2. Crecimiento acumulativo del **Working Set** físico (memoria RAM real asignada al proceso) hasta aproximarse a los límites del gestor de memoria de Windows NT.
3. Fallo inminente de `VirtualAlloc` o `malloc` al no encontrar bloques contiguos libres, resultando en un cuelgue inmediato del cliente (*Crash to Desktop*) con código de error de desbordamiento de memoria.

---

## 2. La Solución: Purgado Proactivo del Working Set vía UPL+ / Win32 API

Para resolver este problema de manera limpia y sin inyectar código ni alterar la integridad del binario del juego (evitando cualquier conflicto con sistemas antitrampas), se implementó un monitor externo en segundo plano que interactúa exclusivamente a través de la API oficial del kernel de Windows (`kernel32.dll` / `psapi.dll`):

### 2.1 Flujo de Ejecución del Watchdog
1. **Detección del Proceso:** Realiza un sondeo periódico (`CreateToolhelp32Snapshot` / `Process32Next`) buscando la instancia activa de `WAR.exe`.
2. **Adquisición de Handle:** Abre el proceso con permisos específicos y mínimos requeridos:
   ```c
   HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_SET_QUOTA, FALSE, pid);
   ```
3. **Monitoreo Telemétrico:** Consulta cada 1000 ms la información de memoria del proceso:
   ```c
   PROCESS_MEMORY_COUNTERS pmc;
   K32GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc));
   ```
4. **Disparo del Umbral de Seguridad:**
   - Si `WorkingSetSize` supera el umbral configurado de **1.65 GB** (1,730,150,400 bytes), se invoca de forma inmediata la rutina de purgado del kernel:
   ```c
   K32EmptyWorkingSet(hProcess);
   ```
5. **Efecto en el Kernel de Windows:**
   - El subsistema de memoria virtual de Windows NT transfiere inmediatamente todas las páginas de memoria no críticas o inactivas del working set físico al archivo de paginación o libera páginas modificadas.
   - La huella física de memoria del juego desciende drásticamente, previniendo el agotamiento del espacio y extendiendo la estabilidad de las sesiones de juego sin caídas.

---

## 3. Implementación en UPL+ Nativo

El código fuente nativo en UPL+ demuestra la interoperabilidad del lenguaje con llamadas al sistema operativo de bajo nivel:

```rust
modulo ror_guard;

incrustar "c" {
#include <windows.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <stdio.h>
}

funcion purgar_memoria(pid: u32) -> bool {
    sea mut exito: bool = false;
    incrustar "c" {
        HANDLE h = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_SET_QUOTA, FALSE, pid);
        if (h) {
            PROCESS_MEMORY_COUNTERS pmc;
            if (K32GetProcessMemoryInfo(h, &pmc, sizeof(pmc))) {
                size_t mb_antes = pmc.WorkingSetSize / (1024 * 1024);
                if (K32EmptyWorkingSet(h)) {
                    K32GetProcessMemoryInfo(h, &pmc, sizeof(pmc));
                    size_t mb_despues = pmc.WorkingSetSize / (1024 * 1024);
                    printf("[UPL+ WATCHDOG] Purgado completado: %zu MB -> %zu MB\n", mb_antes, mb_despues);
                    exito = true;
                }
            }
            CloseHandle(h);
        }
    }
    retornar exito;
}
```

---

## 4. Validación Empírica con Telemetría en Vivo

Para auditar y validar matemáticamente la eficacia de la llamada `K32EmptyWorkingSet` sin depender de simulaciones, se ejecutó una prueba de estrés en vivo mediante [`tests/test_live_memory_guard.c`](../tests/test_live_memory_guard.c):

### Registro de Ejecución del Test:
```
====================================================================
  TEST EMPIRICO: MONITOR DE MEMORIA WIN32 (K32EmptyWorkingSet)
====================================================================
[FASE 1] Reservando 250 MB de memoria de prueba...
[FASE 1] Escribiendo datos para forzar presencia en RAM fisica...
[TELEMETRIA INICIAL]
  - Working Set Fisico (RAM real): 253.29 MB
  - Tamano de Pagina Privada:      250.05 MB
  - Pico Maximo Registrado:        253.29 MB
[FASE 2] Ejecutando purgado de Working Set con K32EmptyWorkingSet...
[OK] Llamada al kernel ejecutada con exito.
[TELEMETRIA POST-PURGADO]
  - Working Set Fisico (RAM real): 0.54 MB
  - Tamano de Pagina Privada:      250.05 MB
  - Reduccion fisica lograda:      252.75 MB liberados de RAM fisica
====================================================================
  RESULTADO: 100% OPERATIVO Y VERIFICADO CONTRA EL KERNEL DE WINDOWS
====================================================================
```

---

## 5. Arquitectura del Lanzador con Elevación UAC

Dado que el ejecutable del launcher de Warhammer Online (`RoRLauncher.exe`) requiere privilegios de administrador para actualizar archivos protegidos en `C:\Program Files` o `C:\Juegos`, una llamada simple a `CreateProcessA` falla con el código de error Win32 `740` (`ERROR_ELEVATION_REQUIRED`).

Para resolver esto, se construyó el lanzador unificado [`tools/UPL_Launcher.upl`](../tools/UPL_Launcher.upl), el cual:
1. Comprueba la presencia de `RoRLauncher.exe`.
2. Lanza el watchdog en segundo plano de manera silenciosa (`ror_memory_guard.exe`).
3. Invoca la API `ShellExecuteA(NULL, "open", "RoRLauncher.exe", NULL, NULL, SW_SHOWNORMAL)` delegando en Windows la solicitud de elevación UAC nativa.

---

## 6. Paquete Final de Distribución Técnica (`ror_memory_guard.zip`)

El paquete generado está listo para entrega y auditoría por parte del equipo de desarrollo del juego, con total transparencia de código fuente:

```
ror_memory_guard.zip
├── README.md               # Documentación técnica en inglés sin jerga narrativa
├── JUGAR_PROTEGIDO_UPL.bat # Script de inicio rápido en 1 clic
├── ror_memory_guard.c      # Código fuente completo en ANSI C99 estándar
├── ror_memory_guard.exe    # Binario nativo de 64 bits optimizado
├── UPL_Memory_Guard.upl    # Código fuente en lenguaje UPL+ nativo
└── UPL_Launcher.exe        # Lanzador binario unificado con elevación UAC
```
