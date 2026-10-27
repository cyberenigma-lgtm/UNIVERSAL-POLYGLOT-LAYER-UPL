#include "lexer.h"
#include "upl_arena.h"
#include "upl_ast.h"
#include "upl_parser.h"
#include "upl_codegen.h"
#include "runtime/upl_comp3.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#ifdef _WIN32
unsigned long __stdcall GetModuleFileNameA(void *hModule, char *lpFilename, unsigned long nSize);
#ifndef MAX_PATH
#define MAX_PATH 260
#endif
#else
#  include <strings.h>
#endif

static void print_usage(const char *prog_name) {
    fprintf(stderr, "UPL+ Compilador Oficial (v2.1.1 Game Dev Edition - Dragon Book Architecture)\n");
    fprintf(stderr, "Uso: %s [subcomando|opciones] [archivo.upl]\n\n", prog_name);
    fprintf(stderr, "Subcomandos:\n");
    fprintf(stderr, "  run <archivo.upl>     Compilar y ejecutar inmediatamente\n");
    fprintf(stderr, "  test, probar          Ejecutar la suite integral de pruebas\n");
    fprintf(stderr, "  init <proyecto>       Inicializar nuevo proyecto UPL+\n");
    fprintf(stderr, "  init-game, juego <nom>Inicializar entorno de desarrollo de videojuego 2D/3D\n");
    fprintf(stderr, "  migrate <orig.c>      Migrar codigo C99 existente a UPL+ (c2upl)\n");
    fprintf(stderr, "  pm, pkg               Gestor de paquetes UPL\n\n");
    fprintf(stderr, "Opciones:\n");
    fprintf(stderr, "  -o <salida>       Especificar nombre del binario ejecutable de salida\n");
    fprintf(stderr, "  -c, --emit-c      Generar solo codigo fuente C99 sin invocar gcc\n");
    fprintf(stderr, "  --dump-ast        Mostrar arbol de sintaxis abstracta (AST)\n");
    fprintf(stderr, "  --dump-tokens     Mostrar secuencia de tokens lexico bilingue\n");
    fprintf(stderr, "  -v, --version     Mostrar version del compilador\n");
}

static char* read_entire_file(const char *path) {
    FILE *f = fopen(path, "rb");
    if (!f) return NULL;
    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *buf = (char*)malloc(len + 1);
    if (!buf) {
        fclose(f);
        return NULL;
    }
    size_t read_bytes = fread(buf, 1, len, f);
    buf[read_bytes] = '\0';
    fclose(f);
    return buf;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }

    const char *input_file = NULL;
    const char *output_file = NULL;
    bool emit_c_only = false;
    bool dump_ast = false;
    bool dump_tokens = false;
    bool run_after_build = false;
    char extra_gcc_flags[1024] = "";

    // Subcomando TEST: uplc test / uplc probar
    if (argc >= 2 && (strcmp(argv[1], "test") == 0 || strcmp(argv[1], "probar") == 0)) {
        printf("====================================================================\n");
        printf("  UPLC TEST RUNNER: EJECUTANDO SUITE INTEGRAL DE PRUEBAS (C99)\n");
        printf("====================================================================\n");
#ifdef _WIN32
        return system("cmd /c test.bat");
#else
        return system("./test.sh");
#endif
    }

    // Subcomando INIT: uplc init <nombre_proyecto>
    if (argc >= 2 && (strcmp(argv[1], "init") == 0 || strcmp(argv[1], "nuevo") == 0)) {
        const char *proj = (argc >= 3) ? argv[2] : "mi_proyecto_upl";
        printf("[UPLC INIT] Inicializando nuevo proyecto UPL+: '%s'...\n", proj);
#ifdef _WIN32
        char cmd[1024];
        snprintf(cmd, sizeof(cmd), "mkdir \"%s\" \"%s\\src\" \"%s\\modules\" \"%s\\tests\" 2>nul", proj, proj, proj, proj);
        system(cmd);

        char main_path[512];
        snprintf(main_path, sizeof(main_path), "%s\\src\\main.upl", proj);
        FILE *f = fopen(main_path, "wb");
        if (f) {
            fprintf(f, "// Proyecto UPL+: %s\nmodulo main;\n\nincrustar \"c\" {\n#include <stdio.h>\n}\n\npub fn main() -> i32 {\n    incrustar \"c\" {\n        printf(\"¡Hola desde UPL+ (Universal Polyglot Layer)!\\n\");\n    }\n    retornar 0;\n}\n", proj);
            fclose(f);
        }

        char json_path[512];
        snprintf(json_path, sizeof(json_path), "%s\\upl.json", proj);
        f = fopen(json_path, "wb");
        if (f) {
            fprintf(f, "{\n  \"nombre\": \"%s\",\n  \"version\": \"1.0.0\",\n  \"autor\": \"Noxferion\",\n  \"punto_entrada\": \"src/main.upl\"\n}\n", proj);
            fclose(f);
        }
#endif
        printf("[OK] Estructura creada con exito:\n");
        printf("     - %s/src/main.upl\n", proj);
        printf("     - %s/modules/\n", proj);
        printf("     - %s/tests/\n", proj);
        printf("     - %s/upl.json\n", proj);
        printf("[TIP] Para compilar y ejecutar: uplc run %s/src/main.upl\n", proj);
        return 0;
    }

    // Subcomando INIT-GAME: uplc init-game <nombre_juego> / uplc juego <nombre_juego>
    if (argc >= 2 && (strcmp(argv[1], "init-game") == 0 || strcmp(argv[1], "juego") == 0 || strcmp(argv[1], "game") == 0)) {
        const char *game_name = (argc >= 3) ? argv[2] : "mi_juego_upl";
        printf("====================================================================\n");
        printf("  UPLC GAME ENGINE: CREANDO ENTORNO DE VIDEOJUEGOS 2D/3D EN UPL+\n");
        printf("====================================================================\n");
        printf("[UPLC GAME] Inicializando proyecto de juego: '%s'...\n", game_name);

#ifdef _WIN32
        char cmd[1024];
        snprintf(cmd, sizeof(cmd), "mkdir \"%s\" \"%s\\src\" \"%s\\include\" \"%s\\assets\" \"%s\\bin\" 2>nul",
                 game_name, game_name, game_name, game_name, game_name);
        system(cmd);

        // Copiar o crear upl_game.h en el proyecto
        char h_path[512];
        snprintf(h_path, sizeof(h_path), "%s\\include\\upl_game.h", game_name);
        FILE *fh = fopen(h_path, "wb");
        if (fh) {
            FILE *src_h = fopen("compiler\\runtime\\upl_game.h", "rb");
            if (!src_h) src_h = fopen("..\\compiler\\runtime\\upl_game.h", "rb");
            if (src_h) {
                char buf[4096];
                size_t n;
                while ((n = fread(buf, 1, sizeof(buf), src_h)) > 0) {
                    fwrite(buf, 1, n, fh);
                }
                fclose(src_h);
            }
            fclose(fh);
        }

        // Crear src/main.upl
        char main_path[512];
        snprintf(main_path, sizeof(main_path), "%s\\src\\main.upl", game_name);
        FILE *fm = fopen(main_path, "wb");
        if (fm) {
            fprintf(fm,
                "// ====================================================================\n"
                "//  VIDEOJUEGO CREADO CON UPL+ GAME ENGINE (Zero-CRT Freestanding)\n"
                "// ====================================================================\n"
                "modulo %s;\n\n"
                "incrustar \"c\" {\n"
                "#include \"../include/upl_game.h\"\n"
                "}\n\n"
                "pub fn main() -> i32 {\n"
                "    incrustar \"c\" {\n"
                "        upl_game_init(\"%s :: UPL+ Game Engine 60 FPS\", 960, 540);\n"
                "        upl_camera_t cam = { .pos = { 0.0f, 2.5f, -7.0f }, .yaw = 0.0f, .pitch = 0.30f, .fov = 720.0f };\n"
                "        float px = 0.0f, pz = 0.0f;\n\n"
                "        while (upl_game_poll_events()) {\n"
                "            // 1. Controles del Jugador (WASD)\n"
                "            if (g_upl_game.keys['W']) pz += 0.16f;\n"
                "            if (g_upl_game.keys['S']) pz -= 0.16f;\n"
                "            if (g_upl_game.keys['A']) px -= 0.16f;\n"
                "            if (g_upl_game.keys['D']) px += 0.16f;\n\n"
                "            // 2. Control Libre de Vision 360 con Raton\n"
                "            if (g_upl_game.mouse_locked) {\n"
                "                cam.yaw   += (float)g_upl_game.mouse_dx * 0.0055f;\n"
                "                cam.pitch += (float)g_upl_game.mouse_dy * 0.0055f;\n"
                "                if (cam.pitch < 0.05f) cam.pitch = 0.05f;\n"
                "                if (cam.pitch > 1.40f) cam.pitch = 1.40f;\n"
                "            }\n\n"
                "            // 3. Renderizado y Limpieza de Pantalla\n"
                "            upl_game_clear(0x281A45); // Cielo purpura cosmico\n\n"
                "            // Dibujar Plataforma Flotante y Avatar\n"
                "            upl_draw_cube(0.0f, 0.0f, 0.0f, 14.0f, 0.6f, 14.0f, 0x1BE7A5, 0x136B73, cam);\n"
                "            upl_draw_cube(px, 1.0f, pz, 1.2f, 1.4f, 1.0f, 0x00E5FF, 0x0088AA, cam);\n\n"
                "            // 4. Interfaz HUD\n"
                "            char hud[96];\n"
                "            sprintf(hud, \"💎 UPL+ GAME ENGINE | FPS: %%%%d | Raton: [ESC para liberar]\", g_upl_game.fps);\n"
                "            upl_draw_text(20, 20, hud, 0x00FFFF);\n\n"
                "            upl_game_present();\n"
                "        }\n"
                "    }\n"
                "    retornar 0;\n"
                "}\n",
                game_name, game_name
            );
            fclose(fm);
        }

        // Crear jugar.bat
        char bat_path[512];
        snprintf(bat_path, sizeof(bat_path), "%s\\jugar.bat", game_name);
        FILE *fb = fopen(bat_path, "wb");
        if (fb) {
            fprintf(fb,
                "@echo off\n"
                "echo [UPLC] Compilando juego en UPL+ nativo...\n"
                "uplc src\\main.upl -o bin\\juego.exe\n"
                "if %%ERRORLEVEL%% EQU 0 (\n"
                "    echo [OK] Ejecutando juego a 60 FPS...\n"
                "    bin\\juego.exe\n"
                ") else (\n"
                "    echo [ERROR] Error durante la compilacion.\n"
                "    pause\n"
                ")\n"
            );
            fclose(fb);
        }

        // Crear upl.json
        char json_path[512];
        snprintf(json_path, sizeof(json_path), "%s\\upl.json", game_name);
        FILE *fj = fopen(json_path, "wb");
        if (fj) {
            fprintf(fj, "{\n  \"nombre\": \"%s\",\n  \"tipo\": \"videojuego_3d\",\n  \"version\": \"1.0.0\",\n  \"motor\": \"upl_game_runtime\",\n  \"punto_entrada\": \"src/main.upl\"\n}\n", game_name);
            fclose(fj);
        }
#endif
        printf("[OK] ¡Entorno de Videojuegos UPL+ creado con exito!\n");
        printf("     - %s/src/main.upl        (Codigo fuente del juego con bucle a 60 FPS)\n", game_name);
        printf("     - %s/include/upl_game.h  (Motor 2D/3D con Vóxeles, Matriz, Vision 360 y Audio)\n", game_name);
        printf("     - %s/jugar.bat           (Lanzador de 1-Clic para compilar y jugar)\n", game_name);
        printf("     - %s/assets/             (Directorio para modelos, texturas y sonidos)\n", game_name);
        printf("     - %s/upl.json            (Configuracion del juego)\n", game_name);
        printf("\n[TIP] Para jugar inmediatamente:\n");
        printf("      cd %s && jugar.bat\n", game_name);
        return 0;
    }


    // Subcomando MIGRATE / C2UPL: uplc migrate <archivo.c> [-o salida.upl]
    if (argc >= 2 && (strcmp(argv[1], "migrate") == 0 || strcmp(argv[1], "migrar") == 0 || strcmp(argv[1], "c2upl") == 0)) {
        if (argc < 3) {
            fprintf(stderr, "Uso: uplc migrate [opciones] <archivo.c>\n");
            fprintf(stderr, "Opciones:\n");
            fprintf(stderr, "  -o <salida.upl>     Ruta del archivo UPL+ generado\n");
            fprintf(stderr, "  -l <en|es>          Dialecto de destino (por defecto: es)\n");
            return 1;
        }
#ifdef _WIN32
        char cmd[2048] = "bin\\c2upl.exe";
#else
        char cmd[2048] = "./bin/c2upl";
#endif
        for (int i = 2; i < argc; ++i) {
            strcat(cmd, " \"");
            strcat(cmd, argv[i]);
            strcat(cmd, "\"");
        }
        return system(cmd);
    }

    if (argc >= 2 && (strcmp(argv[1], "pm") == 0 || strcmp(argv[1], "pkg") == 0)) {
#ifdef _WIN32
        char cmd[1024] = "bin\\upl_pm.exe";
#else
        char cmd[1024] = "./bin/upl_pm";
#endif
        for (int i = 2; i < argc; ++i) {
            strcat(cmd, " ");
            strcat(cmd, argv[i]);
        }
        return system(cmd);
    }

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "build") == 0 || strcmp(argv[i], "compilar") == 0) {
            continue;
        } else if (strcmp(argv[i], "run") == 0 || strcmp(argv[i], "ejecutar") == 0) {
            run_after_build = true;
            continue;
        } else if (strcmp(argv[i], "-t") == 0 && i + 1 < argc) {
            const char *target = argv[++i];
            if (strcmp(target, "c") == 0) emit_c_only = true;
        } else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            output_file = argv[++i];
        } else if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--emit-c") == 0) {
            emit_c_only = true;
        } else if (strcmp(argv[i], "--dump-ast") == 0) {
            dump_ast = true;
        } else if (strcmp(argv[i], "--dump-tokens") == 0) {
            dump_tokens = true;
        } else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
            printf("uplc version 2.1.1 (x86_64-w64-mingw32 Game Dev Edition - Dragon Book pipeline)\n");
            return 0;
        } else if (argv[i][0] == '-' && (argv[i][1] == 'l' || argv[i][1] == 'L' || argv[i][1] == 'I' || argv[i][1] == 'm' || argv[i][1] == 'W')) {
            strcat(extra_gcc_flags, " ");
            strcat(extra_gcc_flags, argv[i]);
            if ((strcmp(argv[i], "-L") == 0 || strcmp(argv[i], "-I") == 0) && i + 1 < argc) {
                strcat(extra_gcc_flags, " \"");
                strcat(extra_gcc_flags, argv[++i]);
                strcat(extra_gcc_flags, "\"");
            }
        } else if (argv[i][0] != '-') {
            input_file = argv[i];
        } else {
            fprintf(stderr, "[ERROR] Opcion desconocida: %s\n", argv[i]);
            return 1;
        }
    }

    if (!input_file) {
        fprintf(stderr, "[ERROR] No se especifico archivo de entrada .upl\n");
        return 1;
    }

    char *source = read_entire_file(input_file);
    if (!source) {
        fprintf(stderr, "[ERROR] No se pudo leer el archivo: %s\n", input_file);
        return 1;
    }

    // 1. Fase de Depuración Léxica si se solicita
    if (dump_tokens) {
        printf("=== FLUJO DE TOKENS LEXICOS (BILINGUE) ===\n");
        Lexer lex;
        lexer_init(&lex, source);
        for (;;) {
            Token tok = lexer_next_token(&lex);
            printf("[L%zu:C%zu] %-20s : '%s'\n", tok.line, tok.column, token_type_name(tok.type), tok.lexeme ? tok.lexeme : "");
            TokenType t = tok.type;
            token_free(&tok);
            if (t == TOKEN_EOF || t == TOKEN_ERROR) break;
        }
        free(source);
        return 0;
    }

    // 2. Inicialización de Memory Arena para el AST
    UPL_Arena arena;
    upl_arena_init(&arena, 131072); // 128 KB por bloque

    // 3. Inicialización de Lexer y Parser
    Lexer lexer;
    lexer_init(&lexer, source);

    UplParser parser;
    upl_parser_init(&parser, &lexer, &arena);

    // 4. Construcción del Árbol de Sintaxis Abstracta (AST)
    UplAstNode *program = upl_parser_parse_program(&parser);

    if (parser.had_error) {
        fprintf(stderr, "%s\n", parser.error_message);
        upl_arena_free(&arena);
        free(source);
        return 1;
    }

    // 5. Depuración de AST si se solicita
    if (dump_ast) {
        printf("=== ARBOL DE SINTAXIS ABSTRACTA (AST) ===\n");
        upl_ast_dump(program, 0);
        if (!output_file && !emit_c_only) {
            upl_arena_free(&arena);
            free(source);
            return 0;
        }
    }

    // 6. Generación de Código C99
    char c_filename[512];
    if (output_file && emit_c_only) {
        strncpy(c_filename, output_file, sizeof(c_filename) - 1);
    } else {
        snprintf(c_filename, sizeof(c_filename), "%s.c", input_file);
    }

    FILE *c_out = fopen(c_filename, "wb");
    if (!c_out) {
        fprintf(stderr, "[ERROR] No se pudo crear archivo C intermedio: %s\n", c_filename);
        upl_arena_free(&arena);
        free(source);
        return 1;
    }

    UplCodeGen codegen;
    upl_codegen_init(&codegen, c_out);
    bool ok_gen = upl_codegen_generate(&codegen, program);
    fclose(c_out);

    if (!ok_gen) {
        fprintf(stderr, "[ERROR] Fallo en la fase de generacion de codigo C99\n");
        upl_arena_free(&arena);
        free(source);
        return 1;
    }

    // Si solo se requería emitir C, terminar aquí
    if (emit_c_only) {
        printf("[OK] Codigo C99 emitido: %s\n", c_filename);
        upl_arena_free(&arena);
        free(source);
        return 0;
    }

    // 7. Invocación de GCC para compilación binaria nativa
    char exe_filename[512];
    if (output_file) {
        strncpy(exe_filename, output_file, sizeof(exe_filename) - 1);
    } else {
        snprintf(exe_filename, sizeof(exe_filename), "%s.exe", input_file);
    }

    char gcc_cmd[2048];
    bool is_shared = false;
    size_t out_len = strlen(exe_filename);
#ifdef _WIN32
    if (out_len >= 4 && (_stricmp(exe_filename + out_len - 4, ".dll") == 0)) {
        is_shared = true;
    }
#else
    if (out_len >= 3 && (strcasecmp(exe_filename + out_len - 3, ".so") == 0)) {
        is_shared = true;
    }
#endif

    char runtime_dir[512] = "compiler/runtime";
    char runtime_c[512] = "compiler/runtime/upl_comp3.c";

#ifdef _WIN32
    char exe_bin_path[MAX_PATH];
    if (GetModuleFileNameA(NULL, exe_bin_path, MAX_PATH) > 0) {
        char *last_sep = strrchr(exe_bin_path, '\\');
        if (!last_sep) last_sep = strrchr(exe_bin_path, '/');
        if (last_sep) {
            *last_sep = '\0';
            snprintf(runtime_dir, sizeof(runtime_dir), "%s\\..\\compiler\\runtime", exe_bin_path);
            snprintf(runtime_c, sizeof(runtime_c), "%s\\..\\compiler\\runtime\\upl_comp3.c", exe_bin_path);
        }
    }
#endif

    if (is_shared) {
        snprintf(gcc_cmd, sizeof(gcc_cmd),
                 "gcc -O2 -s -shared \"%s\" \"%s\" -I \"%s\" "
#ifdef _WIN32
                 "-Wl,--export-all-symbols -luser32 -lgdi32 -lwinmm "
#else
                 "-fPIC -lm "
#endif
                 "%s -o \"%s\"",
                 c_filename, runtime_c, runtime_dir, extra_gcc_flags, exe_filename);
    } else {
        snprintf(gcc_cmd, sizeof(gcc_cmd),
                 "gcc -O2 \"%s\" \"%s\" -I \"%s\" "
#ifdef _WIN32
                 "-luser32 -lgdi32 -lwinmm "
#else
                 "-lm "
#endif
                 "%s -o \"%s\"",
                 c_filename, runtime_c, runtime_dir, extra_gcc_flags, exe_filename);
    }

    int ret = system(gcc_cmd);
    if (ret != 0) {
        fprintf(stderr, "[ERROR] GCC fallo al compilar el binario nativo (codigo %d)\n", ret);
        upl_arena_free(&arena);
        free(source);
        return ret;
    }

    printf("[OK] Compilacion completada con exito: %s\n", exe_filename);

    if (run_after_build) {
        printf("[RUN] Ejecutando %s...\n", exe_filename);
#ifdef _WIN32
        for (char *p = exe_filename; *p; ++p) {
            if (*p == '/') *p = '\\';
        }
#endif
        system(exe_filename);
    }

    upl_arena_free(&arena);
    free(source);
    return 0;
}
