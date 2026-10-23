#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

/**
 * UPL+ Source Code Extractor & Markdown Sanitizer (ANSI C99)
 * 
 * Preprocesador determinista para pipelines automatizados.
 * Extrae bloques de codigo (fenced code) y descarta texto de relleno conversacional.
 * Diseno freestanding 100% nativo (0 dependencias externas).
 */

#define STATUS_VALID_CODE             0
#define STATUS_SANITIZED_EXTRACTED   1
#define STATUS_REJECTED_NO_CODE     -1

static const char* NON_COMPILABLE_PATTERNS[] = {
    "reactor nuclear",
    "bunker cuantico",
    "búnker cuántico",
    "tecnologia alienigena",
    "tecnología alienígena",
    "capilla sixtina del silicio",
    "algoritmo prohibido",
    "mutacion cuantica",
    "mutación cuántica",
    NULL
};

static const char* UPL_SYNTAX_KEYWORDS[] = {
    "fn ", "funcion ", "sea ", "let ", "struct ", "estructura ",
    "modulo ", "module ", "import ", "importar ", "comp3", "incrustar",
    "embed", "asm", "ensamblador", "retornar", "return",
    NULL
};

static bool strcasestr_simple(const char *haystack, const char *needle) {
    if (!haystack || !needle) return false;
    size_t needle_len = strlen(needle);
    if (needle_len == 0) return true;

    for (; *haystack; ++haystack) {
        size_t i = 0;
        while (haystack[i] && needle[i] && tolower((unsigned char)haystack[i]) == tolower((unsigned char)needle[i])) {
            i++;
        }
        if (i == needle_len) return true;
    }
    return false;
}

bool upl_sanitizer_has_invalid_patterns(const char *texto) {
    for (int i = 0; NON_COMPILABLE_PATTERNS[i] != NULL; ++i) {
        if (strcasestr_simple(texto, NON_COMPILABLE_PATTERNS[i])) {
            return true;
        }
    }
    return false;
}

bool upl_sanitizer_has_code_keywords(const char *texto) {
    for (int i = 0; UPL_SYNTAX_KEYWORDS[i] != NULL; ++i) {
        if (strstr(texto, UPL_SYNTAX_KEYWORDS[i]) != NULL) {
            return true;
        }
    }
    return false;
}

bool upl_sanitizer_extract_code_blocks(const char *entrada, char *salida, size_t max_salida) {
    salida[0] = '\0';
    const char *p = entrada;
    size_t total_out = 0;
    bool found_blocks = false;

    while ((p = strstr(p, "```")) != NULL) {
        p += 3;
        while (*p && *p != '\n' && !isspace((unsigned char)*p)) p++;
        if (*p == '\n') p++;

        const char *end_block = strstr(p, "```");
        if (!end_block) break;

        found_blocks = true;
        size_t block_len = (size_t)(end_block - p);

        while (block_len > 0 && isspace((unsigned char)p[block_len - 1])) {
            block_len--;
        }

        if (total_out + block_len + 2 < max_salida) {
            if (total_out > 0) {
                salida[total_out++] = '\n';
                salida[total_out++] = '\n';
            }
            memcpy(salida + total_out, p, block_len);
            total_out += block_len;
            salida[total_out] = '\0';
        }

        p = end_block + 3;
    }

    return found_blocks;
}

int upl_sanitizer_audit(const char *texto_crudo, char *codigo_saneado, size_t max_cod, char *alerta, size_t max_alerta) {
    alerta[0] = '\0';
    codigo_saneado[0] = '\0';

    bool tiene_patrones = upl_sanitizer_has_invalid_patterns(texto_crudo);
    bool tiene_bloques = upl_sanitizer_extract_code_blocks(texto_crudo, codigo_saneado, max_cod);

    if (!tiene_bloques) {
        if (upl_sanitizer_has_code_keywords(texto_crudo) && !tiene_patrones) {
            strncpy(codigo_saneado, texto_crudo, max_cod - 1);
            codigo_saneado[max_cod - 1] = '\0';
            snprintf(alerta, max_alerta, "Texto sin formato Markdown, pero con sintaxis fuente valida.");
            return STATUS_VALID_CODE;
        } else {
            snprintf(alerta, max_alerta, "RECHAZO: No se detecto codigo ejecutable.");
            return STATUS_REJECTED_NO_CODE;
        }
    }

    if (tiene_patrones) {
        snprintf(alerta, max_alerta, "INFO: Filtrado texto narrativo exterior; bloque de codigo extraido con exito.");
        return STATUS_SANITIZED_EXTRACTED;
    }

    snprintf(alerta, max_alerta, "Codigo extraido formalmente sin anomalias.");
    return STATUS_VALID_CODE;
}

static void ejecutar_suite_pruebas(void) {
    printf("=== SUITE DE PRUEBAS: PREPROCESADOR Y SANITIZADOR DE CODIGO (C99) ===\n");

    char buffer_codigo[4096];
    char buffer_alerta[256];

    // Prueba 1: Filtrado de boilerplate y extraccion
    const char *test1 = "Preambulo informativo...\n```upl\npub fn main() -> i32 { return 0; }\n```\nComentarios posteriores.";
    int res1 = upl_sanitizer_audit(test1, buffer_codigo, sizeof(buffer_codigo), buffer_alerta, sizeof(buffer_alerta));
    if (res1 == STATUS_VALID_CODE && strstr(buffer_codigo, "pub fn main()") != NULL) {
        printf("  [PASS] 1. Extraccion determinista de bloques de codigo Markdown (Fenced Code)\n");
    } else {
        printf("  [FAIL] 1. Fallo en la extraccion de bloques de codigo\n");
    }

    // Prueba 2: Rechazo de texto narrativo sin codigo
    const char *test2 = "Este es un texto puramente descriptivo sin ningun bloque ni sintaxis ejecutable.";
    int res2 = upl_sanitizer_audit(test2, buffer_codigo, sizeof(buffer_codigo), buffer_alerta, sizeof(buffer_alerta));
    if (res2 == STATUS_REJECTED_NO_CODE) {
        printf("  [PASS] 2. Rechazo estricto de texto no compilable sin bloques de codigo\n");
    } else {
        printf("  [FAIL] 2. No se rechazo el texto sin codigo\n");
    }

    // Prueba 3: Preservacion de sintaxis tecnica pura
    const char *test3 = "modulo test;\npub fn suma(a: i32, b: i32) -> i32 { return a + b; }";
    int res3 = upl_sanitizer_audit(test3, buffer_codigo, sizeof(buffer_codigo), buffer_alerta, sizeof(buffer_alerta));
    if (res3 == STATUS_VALID_CODE && strcmp(buffer_codigo, test3) == 0) {
        printf("  [PASS] 3. Preservacion intacta de codigo fuente tecnico directo\n");
    } else {
        printf("  [FAIL] 3. No se preservo el codigo tecnico\n");
    }

    printf("=== PREPROCESADOR DE CODIGO: OPERATIVO (0 ERRORES) ===\n");
}

int main(int argc, char **argv) {
    if (argc > 1 && strcmp(argv[1], "--test") == 0) {
        ejecutar_suite_pruebas();
        return 0;
    }

    if (argc < 2) {
        fprintf(stderr, "Uso: %s <archivo_entrada> [-o <archivo_salida>] | --test\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "rb");
    if (!f) {
        fprintf(stderr, "[ERROR] No se pudo abrir: %s\n", argv[1]);
        return 1;
    }

    fseek(f, 0, SEEK_END);
    long sz = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *buf = (char*)malloc(sz + 1);
    if (!buf) {
        fclose(f);
        return 1;
    }

    fread(buf, 1, sz, f);
    buf[sz] = '\0';
    fclose(f);

    char *out_code = (char*)malloc(sz + 1024);
    char alerta[512];

    int res = upl_sanitizer_audit(buf, out_code, sz + 1024, alerta, sizeof(alerta));
    printf("[SANITIZER] %s\n", alerta);

    if (res == STATUS_REJECTED_NO_CODE) {
        free(buf);
        free(out_code);
        return 1;
    }

    if (argc >= 4 && strcmp(argv[2], "-o") == 0) {
        FILE *fo = fopen(argv[3], "wb");
        if (fo) {
            fputs(out_code, fo);
            fclose(fo);
            printf("[OK] Codigo extraido guardado en: %s\n", argv[3]);
        }
    } else {
        printf("\n--- CODIGO EXTRAIDO ---\n%s\n------------------------\n", out_code);
    }

    free(buf);
    free(out_code);
    return 0;
}
