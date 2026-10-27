#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifdef _WIN32
#include <direct.h>
#include <windows.h>
#define mkdir_compat(path) _mkdir(path)
#else
#include <sys/stat.h>
#include <sys/types.h>
#define mkdir_compat(path) mkdir(path, 0755)
#endif

#define UPL_PM_VERSION "1.1.0"

typedef struct {
    const char *name;
    const char *category;
    const char *path;
    const char *description;
} UplModuleEntry;

static const UplModuleEntry g_catalogue[] = {
    // 1. Biblioteca Estandar UPL (std/)
    {"std/fs",          "SILICIO/FS",   "modules/std/fs.upl",                   "Sistema de archivos y descriptores I/O"},
    {"std/dict",        "ESTRUCTURA",   "modules/std/dict.upl",                 "Tabla hash FNV-1a de alta dispersion"},
    {"std/memory",      "MEMORIA",      "modules/std/memory.upl",               "Arena allocators y memoria de silicio"},
    {"std/math",        "CALCULO",      "modules/std/math.upl",                 "Aritmetica vectorial y constantes"},
    {"std/io",          "CONSOLA",      "modules/std/io.upl",                   "Flujos binarios y formateo de texto"},
    {"std/net",         "REDES",        "modules/std/net.upl",                  "Sockets TCP/UDP y buffers de red"},
    {"std/threads",     "CONCURR",      "modules/std/threads.upl",              "Hilos cooperativos y sincronizacion"},
    {"std/json",        "SERIALIZ",     "modules/std/json.upl",                 "Parser/serializador JSON streaming"},
    {"std/time",        "CRONOMETR",    "modules/std/time.upl",                 "Temporizador de alta resolucion QPC"},

    // 2. Motores Soberanos de Ecosistema
    {"dvtrgas30",       "ACELERADOR",   "modules/dvtrgas30/dvtrgas30.upl",       "Motor vectorial de silicio nativo V30.5"},
    {"dvtrgas_v1",      "GRAFICOS",     "modules/dvtrgas_v1/dvtrgas_v1.upl",     "Motor raster inicial DVTRGAS V1.0"},
    {"gxnox",           "PARIDAD/CFI",  "modules/gxnox/gxnox_dual_parity.upl",   "Criptografia de doble paridad simetrica"},
    {"iagrok_core",     "BUS COGNITIV", "modules/iagrok_core/iagrok_core.upl",   "Motor de atencion y arbitraje cognitivo"},
    {"multilang_asm",   "ASSEMBLER",    "modules/multilang_asm/masm_table.upl",  "Tabla de instrucciones MultiLang ASM"},
    {"neurocobol",      "COMP-3/FIN",   "modules/neurocobol/neurocobol_core.upl","Motor decimal exacto BCD COMP-3"},
    {"neurowill",       "MAQUINA VM",   "modules/neurowill/neurowill_core.upl",  "Maquina virtual de pila de ejecucion"},
    {"neuro_gfx",       "RASTER 2D/3D", "modules/neuro_gfx/neuro_gfx_core.upl",  "Motor de aceleracion y rasterizacion"},
    {"neuro_probe",     "TELEMETRIA",   "modules/neuro_probe/probe_engine.upl",  "Monitoreo de latencia y canarios HW"},
    {"nuasm",           "JIT/BACKEND",  "modules/nuasm/nuasm_core.upl",          "Ensamblador universal y generador binario"},

    // 3. Modulos de Computo, Emulacion y Seguridad UREP
    {"compute",         "SIMD/VECTOR",  "modules/compute/vector_ops.upl",        "Aritmetica vectorial SIMD de alto orden"},
    {"emulator",        "CPU RUNNER",   "modules/emulator/cpu_runner.upl",       "Emulador de registros y ciclos CPU"},
    {"urep_guard",      "DEFENSA/CFI",  "modules/urep/urep_guard.upl",           "Validacion de punteros y stack canary"},
    {"urep_polyglot",   "INTEROP",      "modules/urep/urep_polyglot.upl",        "Puente poliglota FFI C/Rust/COBOL"},
    {"urep_arch",       "ARQUITECTURA", "modules/urep/urep_arch.upl",            "Definiciones formales de ISA y registros"},
    {"urep_hw",         "BAREMETAL HW", "modules/urep/urep_hw.upl",              "Acceso directo a silicio y puertos I/O"}
};

static bool file_exists(const char *path) {
    FILE *f = fopen(path, "rb");
    if (f) {
        fclose(f);
        return true;
    }
    return false;
}

static void print_banner(void) {
    printf("====================================================================\n");
    printf("  UPL PM: GESTOR SOBERANO DE MODULOS Y PAQUETES (v%s)\n", UPL_PM_VERSION);
    printf("  Toolchain Nativa Freestanding | Cero Dependencias Externas\n");
    printf("====================================================================\n\n");
}

static void cmd_list(void) {
    printf("[UPL PM] Escaneando catalogo de modulos en 'modules/'...\n\n");
    printf("%-16s %-14s %-40s %-12s\n", "MODULO", "CATEGORIA", "RUTA CANONICA", "ESTADO");
    printf("----------------------------------------------------------------------------------------\n");

    int total = sizeof(g_catalogue) / sizeof(g_catalogue[0]);
    int active = 0;

    for (int i = 0; i < total; ++i) {
        bool exists = file_exists(g_catalogue[i].path);
        if (exists) active++;

        printf("%-16s %-14s %-40s [%s]\n",
               g_catalogue[i].name,
               g_catalogue[i].category,
               g_catalogue[i].path,
               exists ? "OPERATIVO" : "FALTANTE");
    }

    printf("----------------------------------------------------------------------------------------\n");
    printf("[OK] Total de modulos soberanos: %d/%d (%.1f%% Activos)\n\n",
           active, total, (double)active / total * 100.0);
}

static void cmd_init(const char *name) {
    if (!name || strlen(name) == 0) {
        fprintf(stderr, "[ERROR] Uso: upl pm init <nombre_modulo>\n");
        return;
    }

    char dir_path[256];
    snprintf(dir_path, sizeof(dir_path), "modules/%s", name);
    mkdir_compat(dir_path);

    char file_path[256];
    snprintf(file_path, sizeof(file_path), "modules/%s/%s.upl", name, name);

    FILE *f = fopen(file_path, "w");
    if (!f) {
        fprintf(stderr, "[ERROR] No se pudo crear el archivo: %s\n", file_path);
        return;
    }

    fprintf(f, "// Modulo UPL+ generado por UPL PM (v%s)\n", UPL_PM_VERSION);
    fprintf(f, "modulo %s;\n\n", name);
    fprintf(f, "pub fn %s_iniciar() -> bool {\n", name);
    fprintf(f, "    return true;\n");
    fprintf(f, "}\n");
    fclose(f);

    printf("[OK] Modulo UPL '%s' inicializado con exito en: %s\n", name, file_path);
}

static void cmd_verify(void) {
    printf("[UPL PM] Ejecutando suite integral de verificacion de canarios y modulos...\n");
#ifdef _WIN32
    int ret = system("test.bat");
#else
    int ret = system("./test.sh");
#endif
    if (ret == 0) {
        printf("\n[OK] Verificacion de todos los modulos UPL: EXITOSA (0 Fallos)\n");
    } else {
        printf("\n[AVISO] Verificacion devolvio codigo %d\n", ret);
    }
}

static void cmd_test(void) {
    printf("[UPL PM] Ejecutando pruebas unitarias de modulos nativos...\n");
#ifdef _WIN32
    int ret = system("bin\\test_all_modules.exe");
#else
    int ret = system("./bin/test_all_modules");
#endif
    if (ret == 0) {
        printf("\n[OK] Todas las pruebas unitarias de modulos UPL pasaron al 100%%.\n");
    } else {
        printf("\n[AVISO] Fallo en pruebas unitarias (codigo %d)\n", ret);
    }
}

static void cmd_build_all(void) {
    printf("[UPL PM] Compilando modulos UPL+ a binarios nativos (.dll)...\n\n");
    int total = sizeof(g_catalogue) / sizeof(g_catalogue[0]);
    int built = 0;

    for (int i = 0; i < total; ++i) {
        if (!file_exists(g_catalogue[i].path)) continue;

        // Extraer nombre base para el DLL
        const char *slash = strrchr(g_catalogue[i].name, '/');
        const char *mod_base = slash ? slash + 1 : g_catalogue[i].name;

        char cmd[512];
        snprintf(cmd, sizeof(cmd), "bin\\uplc.exe %s -o bin\\%s.dll",
                 g_catalogue[i].path, mod_base);

        printf("  -> [%d/%d] Compilando %s...\n", i + 1, total, g_catalogue[i].name);
        int r = system(cmd);
        if (r == 0) {
            built++;
        } else {
            printf("     [AVISO] No se pudo compilar %s a DLL individual\n", g_catalogue[i].name);
        }
    }
    printf("\n[OK] Compilacion de catalogo completada: %d modulos procesados.\n\n", built);
}

int main(int argc, char **argv) {
    print_banner();

    if (argc < 2) {
        printf("Comandos disponibles:\n");
        printf("  upl pm list              - Lista todos los modulos oficiales instalados\n");
        printf("  upl pm init <nombre>     - Crea un nuevo modulo UPL con estructura estandar\n");
        printf("  upl pm build             - Compila todos los modulos UPL a DLL nativas\n");
        printf("  upl pm test              - Ejecuta la suite unitaria de los modulos\n");
        printf("  upl pm verify            - Verifica la integridad completa y canarios de silicio\n");
        printf("  upl pm version           - Muestra la version del gestor de paquetes\n\n");
        return 0;
    }

    const char *subcmd = argv[1];
    if (strcmp(subcmd, "pm") == 0 && argc > 2) {
        subcmd = argv[2];
    }

    if (strcmp(subcmd, "list") == 0 || strcmp(subcmd, "ls") == 0) {
        cmd_list();
    } else if (strcmp(subcmd, "init") == 0 || strcmp(subcmd, "new") == 0) {
        const char *mod_name = (argc >= 3 && strcmp(argv[1], "pm") == 0) ? (argc >= 4 ? argv[3] : NULL) : (argc >= 3 ? argv[2] : NULL);
        cmd_init(mod_name);
    } else if (strcmp(subcmd, "build") == 0 || strcmp(subcmd, "compile-all") == 0) {
        cmd_build_all();
    } else if (strcmp(subcmd, "test") == 0 || strcmp(subcmd, "probar") == 0) {
        cmd_test();
    } else if (strcmp(subcmd, "verify") == 0 || strcmp(subcmd, "check") == 0) {
        cmd_verify();
    } else if (strcmp(subcmd, "version") == 0 || strcmp(subcmd, "-v") == 0) {
        printf("upl pm version %s (Sovereign Package Manager)\n", UPL_PM_VERSION);
    } else {
        fprintf(stderr, "[ERROR] Comando desconocido: '%s'. Ejecuta 'upl pm' para ayuda.\n", subcmd);
        return 1;
    }

    return 0;
}
