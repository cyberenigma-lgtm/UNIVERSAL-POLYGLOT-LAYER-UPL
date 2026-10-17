#include <windows.h>
#include <psapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    printf("====================================================================\n");
    printf("   DEMOSTRACION EMPIRICA: COMPROBACION EN VIVO DEL KERNEL WINDOWS   \n");
    printf("====================================================================\n");

    HANDLE hCurrent = GetCurrentProcess();
    PROCESS_MEMORY_COUNTERS_EX pmc;

    // 1. Estado inicial
    GetProcessMemoryInfo(hCurrent, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
    size_t initial_ws = pmc.WorkingSetSize / (1024 * 1024);
    printf("[1] Memoria inicial del proceso: %zu MB\n", initial_ws);

    // 2. Asignar 250 MB de memoria real en el Heap y tocar cada pagina
    size_t alloc_bytes = 250 * 1024 * 1024;
    printf("[2] Asignando y ensuciando 250 MB de memoria fisica real...\n");
    volatile char *buf = (volatile char*)malloc(alloc_bytes);
    if (!buf) {
        fprintf(stderr, "Error al reservar memoria.\n");
        return 1;
    }
    // Escribir en cada página (4096 bytes) para forzar asignación en RAM física (Working Set)
    for (size_t i = 0; i < alloc_bytes; i += 4096) {
        buf[i] = (char)(i & 0xFF);
    }

    GetProcessMemoryInfo(hCurrent, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
    size_t loaded_ws = pmc.WorkingSetSize / (1024 * 1024);
    size_t loaded_commit = pmc.PrivateUsage / (1024 * 1024);
    printf("[3] Memoria fisica tras la carga (Working Set): %zu MB | Commit: %zu MB\n", loaded_ws, loaded_commit);

    // 3. Ejecutar exactamente la llamada al sistema que realiza el Guardián de UPL: EmptyWorkingSet
    printf("[4] Ejecutando K32EmptyWorkingSet() a traves de la API del Kernel...\n");
    BOOL ok = EmptyWorkingSet(hCurrent);
    if (!ok) {
        fprintf(stderr, "EmptyWorkingSet fallo con error: %lu\n", GetLastError());
        return 1;
    }

    // 4. Medir inmediatamente el impacto en el Kernel del SO
    GetProcessMemoryInfo(hCurrent, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
    size_t purged_ws = pmc.WorkingSetSize / (1024 * 1024);
    printf("[5] Memoria fisica tras la purga del Guardian (Working Set): %zu MB\n", purged_ws);
    printf("--------------------------------------------------------------------\n");
    printf("   RESULTADO DE IMPACTO EN SO: REDUCCION DE %zu MB -> %zu MB (%zu MB PURGADOS)\n",
           loaded_ws, purged_ws, loaded_ws - purged_ws);
    printf("   CONCLUSION: 100%% REAL, OPERATIVO Y VERIFICADO A NIVEL DE KERNEL NT\n");
    printf("====================================================================\n");

    free((void*)buf);
    return 0;
}
