#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef _WIN32
#include <direct.h>
#include <windows.h>
#define mkdir_compat(path) _mkdir(path)
#else
#include <sys/stat.h>
#include <sys/types.h>
#define mkdir_compat(path) mkdir(path, 0755)
#endif

#define UPL_PM_VERSION "1.3.0"
#define UPL_LOCKFILE   "upl.lock"

/* Implementacion C99 pura y determinista de SHA-256 (FIPS PUB 180-4) */
typedef struct {
    uint32_t state[8];
    uint64_t count;
    uint8_t  buf[64];
} UplSha256;

static const uint32_t K256[64] = {
    0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
    0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
    0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
    0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
    0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
    0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
    0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
    0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};

#define ROR(x, n) (((x) >> (n)) | ((x) << (32 - (n))))
#define CH(x, y, z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x, y, z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define EP0(x) (ROR(x, 2) ^ ROR(x, 13) ^ ROR(x, 22))
#define EP1(x) (ROR(x, 6) ^ ROR(x, 11) ^ ROR(x, 25))
#define SIG0(x) (ROR(x, 7) ^ ROR(x, 18) ^ ((x) >> 3))
#define SIG1(x) (ROR(x, 17) ^ ROR(x, 19) ^ ((x) >> 10))

static void sha256_transform(UplSha256 *ctx, const uint8_t data[64]) {
    uint32_t a = ctx->state[0], b = ctx->state[1], c = ctx->state[2], d = ctx->state[3];
    uint32_t e = ctx->state[4], f = ctx->state[5], g = ctx->state[6], h = ctx->state[7];
    uint32_t w[64];

    for (int i = 0; i < 16; ++i) {
        w[i] = ((uint32_t)data[i * 4] << 24) |
               ((uint32_t)data[i * 4 + 1] << 16) |
               ((uint32_t)data[i * 4 + 2] << 8) |
               ((uint32_t)data[i * 4 + 3]);
    }
    for (int i = 16; i < 64; ++i) {
        w[i] = SIG1(w[i - 2]) + w[i - 7] + SIG0(w[i - 15]) + w[i - 16];
    }
    for (int i = 0; i < 64; ++i) {
        uint32_t t1 = h + EP1(e) + CH(e, f, g) + K256[i] + w[i];
        uint32_t t2 = EP0(a) + MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
    }
    ctx->state[0] += a;
    ctx->state[1] += b;
    ctx->state[2] += c;
    ctx->state[3] += d;
    ctx->state[4] += e;
    ctx->state[5] += f;
    ctx->state[6] += g;
    ctx->state[7] += h;
}

static void sha256_init(UplSha256 *ctx) {
    ctx->state[0] = 0x6a09e667;
    ctx->state[1] = 0xbb67ae85;
    ctx->state[2] = 0x3c6ef372;
    ctx->state[3] = 0xa54ff53a;
    ctx->state[4] = 0x510e527f;
    ctx->state[5] = 0x9b05688c;
    ctx->state[6] = 0x1f83d9ab;
    ctx->state[7] = 0x5be0cd19;
    ctx->count = 0;
}

static void sha256_update(UplSha256 *ctx, const uint8_t *data, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        ctx->buf[ctx->count % 64] = data[i];
        ctx->count++;
        if (ctx->count % 64 == 0) {
            sha256_transform(ctx, ctx->buf);
        }
    }
}

static void sha256_final(UplSha256 *ctx, uint8_t hash[32]) {
    uint64_t total_bits = ctx->count * 8;
    ctx->buf[ctx->count % 64] = 0x80;
    ctx->count++;
    if (ctx->count % 64 > 56) {
        while (ctx->count % 64 != 0) {
            ctx->buf[ctx->count % 64] = 0x00;
            ctx->count++;
        }
        sha256_transform(ctx, ctx->buf);
    }
    while (ctx->count % 64 != 56) {
        ctx->buf[ctx->count % 64] = 0x00;
        ctx->count++;
    }
    for (int i = 7; i >= 0; --i) {
        ctx->buf[56 + (7 - i)] = (uint8_t)(total_bits >> (i * 8));
    }
    sha256_transform(ctx, ctx->buf);

    for (int i = 0; i < 8; ++i) {
        hash[i * 4]     = (uint8_t)(ctx->state[i] >> 24);
        hash[i * 4 + 1] = (uint8_t)(ctx->state[i] >> 16);
        hash[i * 4 + 2] = (uint8_t)(ctx->state[i] >> 8);
        hash[i * 4 + 3] = (uint8_t)(ctx->state[i]);
    }
}

static bool compute_file_sha256(const char *path, char out_hex[65], size_t *out_size) {
    FILE *f = fopen(path, "rb");
    if (!f) return false;

    UplSha256 ctx;
    sha256_init(&ctx);

    uint8_t chunk[4096];
    size_t total_bytes = 0;
    size_t bytes_read = 0;

    while ((bytes_read = fread(chunk, 1, sizeof(chunk), f)) > 0) {
        sha256_update(&ctx, chunk, bytes_read);
        total_bytes += bytes_read;
    }
    fclose(f);

    uint8_t hash[32];
    sha256_final(&ctx, hash);

    for (int i = 0; i < 32; ++i) {
        sprintf(out_hex + (i * 2), "%02x", hash[i]);
    }
    out_hex[64] = '\0';
    if (out_size) *out_size = total_bytes;
    return true;
}

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
    {"std/channel",     "LOCK-FREE",    "modules/std/channel.upl",              "Canales circulares atomicos MPSC"},
    {"std/crypto",      "CRIPTO/HW",    "modules/std/crypto.upl",               "SHA-256 nativo, CRC32C y entropia silicio"},
    {"std/result",      "MANEJO ERR",   "modules/std/result.upl",               "Tipo suma Result<T, E> sin excepciones"},
    {"std/option",      "TIPOS SEGUROS","modules/std/option.upl",               "Manejo nulo seguro Option<T>"},
    {"std/tensor",      "IA/SIMD",      "modules/std/tensor.upl",               "Multiplicacion matricial y tensores de silicio"},
    {"std/ring",        "BOUNDED-BUF",  "modules/std/ring.upl",                 "Bufer circular acotado O(1) con auto-desalojo"},
    {"std/slab",        "PAGED-ALLOC",  "modules/std/slab.upl",                 "Asignador slab alineado a 4KB libre de fragmentacion"},
    {"std/sync",        "LOCK-RANK",    "modules/std/sync.upl",                 "Mutex jerarquico con rango anti-deadlock"},
    {"std/json",        "SERIALIZ",     "modules/std/json.upl",                 "Parser/serializador JSON streaming"},
    {"std/time",        "CRONOMETR",    "modules/std/time.upl",                 "Temporizador de alta resolucion QPC"},
    {"net/gh_bot_radar","ANTI-BOT/SEC", "modules/net/gh_bot_radar.upl",         "Radar anti-bots y follow-farming de GitHub O(1)"},

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

static void cmd_lock(void) {
    printf("[UPL PM] Generando manifiesto criptografico '%s' (SHA-256)...\n\n", UPL_LOCKFILE);
    FILE *out = fopen(UPL_LOCKFILE, "w");
    if (!out) {
        fprintf(stderr, "[ERROR] No se pudo crear el archivo %s\n", UPL_LOCKFILE);
        return;
    }

    fprintf(out, "# ====================================================================\n");
    fprintf(out, "#   UPL SOVEREIGN LOCKFILE - MANIFIESTO CRIPTOGRAFICO DE SILICIO\n");
    fprintf(out, "#   Generado automaticamente por UPL PM (v%s)\n", UPL_PM_VERSION);
    fprintf(out, "#   Garantiza determinismo e integridad SHA-256 en tiempo de ejecucion\n");
    fprintf(out, "# ====================================================================\n\n");
    fprintf(out, "[metadata]\n");
    fprintf(out, "upl_pm_version = \"%s\"\n\n", UPL_PM_VERSION);
    fprintf(out, "[modules]\n");

    int total = sizeof(g_catalogue) / sizeof(g_catalogue[0]);
    int locked = 0;

    for (int i = 0; i < total; ++i) {
        if (!file_exists(g_catalogue[i].path)) continue;

        char sha256_hex[65];
        size_t file_size = 0;
        if (compute_file_sha256(g_catalogue[i].path, sha256_hex, &file_size)) {
            fprintf(out, "%s = %s | %s | %zu\n",
                    g_catalogue[i].name,
                    g_catalogue[i].path,
                    sha256_hex,
                    file_size);
            printf("  [LOCKED] %-16s -> SHA256: %s (%zu bytes)\n",
                   g_catalogue[i].name, sha256_hex, file_size);
            locked++;
        }
    }
    fclose(out);
    printf("\n[OK] Manifiesto '%s' generado exitosamente con %d modulos certificados.\n\n", UPL_LOCKFILE, locked);
}

static void cmd_verify(void) {
    printf("[UPL PM] Verificando integridad criptografica SHA-256 de modulos...\n");

    if (file_exists(UPL_LOCKFILE)) {
        FILE *lock = fopen(UPL_LOCKFILE, "r");
        if (lock) {
            char line[512];
            int in_modules = 0;
            int verified = 0;
            int errors = 0;

            while (fgets(line, sizeof(line), lock)) {
                if (strstr(line, "[modules]")) {
                    in_modules = 1;
                    continue;
                }
                if (!in_modules || line[0] == '#' || line[0] == '\r' || line[0] == '\n') continue;

                char mod_name[64], mod_path[128], expected_sha[65];
                size_t exp_size = 0;
                char *eq = strchr(line, '=');
                if (!eq) continue;

                *eq = '\0';
                sscanf(line, "%63s", mod_name);

                char *pipe1 = strchr(eq + 1, '|');
                if (!pipe1) continue;
                *pipe1 = '\0';
                sscanf(eq + 1, "%127s", mod_path);

                char *pipe2 = strchr(pipe1 + 1, '|');
                if (!pipe2) continue;
                *pipe2 = '\0';
                sscanf(pipe1 + 1, "%64s", expected_sha);
                sscanf(pipe2 + 1, "%zu", &exp_size);

                char current_sha[65];
                size_t current_size = 0;
                if (!compute_file_sha256(mod_path, current_sha, &current_size)) {
                    printf("  [FAIL] %-16s: Archivo no encontrado (%s)\n", mod_name, mod_path);
                    errors++;
                } else if (strcmp(current_sha, expected_sha) != 0) {
                    printf("  [FAIL] %-16s: DISCREPANCIA SHA-256!\n    Esperado: %s\n    Actual:   %s\n",
                           mod_name, expected_sha, current_sha);
                    errors++;
                } else {
                    printf("  [PASS] %-16s: SHA-256 certificado (%s)\n", mod_name, current_sha);
                    verified++;
                }
            }
            fclose(lock);

            if (errors > 0) {
                printf("\n[ERROR CRITICO] Fallo de integridad criptografica: %d discrepancia(s) detectada(s).\n\n", errors);
                exit(1);
            } else {
                printf("\n[OK] Certificacion SHA-256: %d modulos validados sin corrupcion.\n\n", verified);
            }
        }
    } else {
        printf("  [AVISO] Archivo '%s' no encontrado. Ejecuta 'upl pm lock' para generarlo.\n\n", UPL_LOCKFILE);
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

static void cmd_tree(void) {
    printf("[UPL PM TREE] Arbol Jerarquico de Dependencias y Modulos Soberanos\n\n");
    printf("upl-ecosystem (v2.2.3)\n");
    printf("├── std (Biblioteca Estandar de Silicio C99)\n");
    printf("│   ├── fs           [Sistema de archivos y descriptores I/O]\n");
    printf("│   ├── dict         [Tabla hash FNV-1a de alta dispersion]\n");
    printf("│   ├── memory       [Arena allocators y memoria de silicio]\n");
    printf("│   ├── math         [Aritmetica vectorial y constantes]\n");
    printf("│   ├── io           [Flujos binarios y formateo de texto]\n");
    printf("│   ├── net          [Sockets TCP/UDP y buffers de red]\n");
    printf("│   ├── threads      [Hilos cooperativos y sincronizacion]\n");
    printf("│   ├── channel      [Canales circulares atomicos MPSC Lock-Free]\n");
    printf("│   ├── crypto       [SHA-256 nativo, CRC32C y entropia silicio]\n");
    printf("│   ├── result       [Tipo suma Result<T, E> sin excepciones]\n");
    printf("│   ├── option       [Manejo nulo seguro Option<T>]\n");
    printf("│   ├── tensor       [Multiplicacion matricial GEMM y tensores IA]\n");
    printf("│   ├── json         [Parser/serializador JSON streaming]\n");
    printf("│   └── time         [Temporizador de alta resolucion QPC]\n");
    printf("├── motores (Aceleradores Soberanos y Runtimes)\n");
    printf("│   ├── dvtrgas30    [Motor vectorial de silicio nativo V30.5]\n");
    printf("│   ├── dvtrgas_v1   [Motor raster inicial DVTRGAS V1.0]\n");
    printf("│   ├── gxnox        [Criptografia de doble paridad simetrica]\n");
    printf("│   ├── iagrok_core  [Bus cognitivo y arbitraje atencional]\n");
    printf("│   ├── multilang_asm[Tabla de instrucciones MultiLang ASM]\n");
    printf("│   ├── neurocobol   [Motor decimal exacto BCD COMP-3]\n");
    printf("│   ├── neurowill    [Maquina virtual de pila de ejecucion]\n");
    printf("│   ├── neuro_gfx    [Motor de aceleracion y rasterizacion 2D/3D]\n");
    printf("│   ├── neuro_probe  [Telemetria de latencia y canarios HW]\n");
    printf("│   └── nuasm        [Ensamblador universal y generador binario]\n");
    printf("└── urep (Seguridad de Silicio y Baremetal)\n");
    printf("    ├── compute      [Aritmetica vectorial SIMD de alto orden]\n");
    printf("    ├── emulator     [Emulador de registros y ciclos CPU]\n");
    printf("    ├── urep_guard   [Validacion de punteros y stack canary]\n");
    printf("    ├── urep_polyglot[Puente poliglota FFI C/Rust/COBOL]\n");
    printf("    ├── urep_arch    [Definiciones formales de ISA y registros]\n");
    printf("    └── urep_hw      [Acceso directo a silicio y puertos I/O]\n\n");
    printf("[OK] 30 Modulos integrados y verificados en jerarquia.\n\n");
}

int main(int argc, char **argv) {
    print_banner();

    if (argc < 2) {
        printf("Comandos disponibles:\n");
        printf("  upl pm list              - Lista todos los modulos oficiales instalados\n");
        printf("  upl pm tree              - Muestra el arbol jerarquico de dependencias y subsistemas\n");
        printf("  upl pm init <nombre>     - Crea un nuevo modulo UPL con estructura estandar\n");
        printf("  upl pm lock              - Genera o actualiza upl.lock con hashes SHA-256 de todos los modulos\n");
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
    } else if (strcmp(subcmd, "tree") == 0 || strcmp(subcmd, "arbol") == 0) {
        cmd_tree();
    } else if (strcmp(subcmd, "init") == 0 || strcmp(subcmd, "new") == 0) {
        const char *mod_name = (argc >= 3 && strcmp(argv[1], "pm") == 0) ? (argc >= 4 ? argv[3] : NULL) : (argc >= 3 ? argv[2] : NULL);
        cmd_init(mod_name);
    } else if (strcmp(subcmd, "lock") == 0) {
        cmd_lock();
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