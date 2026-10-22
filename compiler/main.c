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

static void print_usage(const char *prog_name) {
    fprintf(stderr, "UPL+ Compilador Oficial (v2.0 Dragon Book Architecture)\n");
    fprintf(stderr, "Uso: %s [opciones] <archivo.upl>\n\n", prog_name);
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

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            output_file = argv[++i];
        } else if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--emit-c") == 0) {
            emit_c_only = true;
        } else if (strcmp(argv[i], "--dump-ast") == 0) {
            dump_ast = true;
        } else if (strcmp(argv[i], "--dump-tokens") == 0) {
            dump_tokens = true;
        } else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
            printf("uplc version 2.0.0 (x86_64-w64-mingw32 standard C99 pipeline)\n");
            return 0;
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
    if (out_len >= 4 && (_stricmp(exe_filename + out_len - 4, ".dll") == 0)) {
        is_shared = true;
    }

    if (is_shared) {
        snprintf(gcc_cmd, sizeof(gcc_cmd),
                 "gcc -O2 -s -shared \"%s\" compiler/runtime/upl_comp3.c -I compiler/runtime -Wl,--export-all-symbols -luser32 -lgdi32 -o \"%s\"",
                 c_filename, exe_filename);
    } else {
        snprintf(gcc_cmd, sizeof(gcc_cmd),
                 "gcc -O2 \"%s\" compiler/runtime/upl_comp3.c -I compiler/runtime -luser32 -lgdi32 -o \"%s\"",
                 c_filename, exe_filename);
    }

    int ret = system(gcc_cmd);
    if (ret != 0) {
        fprintf(stderr, "[ERROR] GCC fallo al compilar el binario nativo (codigo %d)\n", ret);
        upl_arena_free(&arena);
        free(source);
        return ret;
    }

    printf("[OK] Compilacion completada con exito: %s\n", exe_filename);

    upl_arena_free(&arena);
    free(source);
    return 0;
}
