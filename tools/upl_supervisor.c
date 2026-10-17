#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

/**
 * UPL+ Native Supervisor & Guardrail (C99)
 * 
 * Filtro determinista contra infección narrativa y divagaciones de IA.
 * 100% nativo (sin dependencias externas de Python ni librerías de terceros).
 */

#define ESTADO_EXITO             0
#define ESTADO_CORREGIDO_SANEADO 1
#define ESTADO_ERROR_DIVAGACION -1

static const char* PATRONES_HUMO[] = {
    "reactor nuclear",
    "soberania tecnologica",
    "soberanía tecnológica",
    "dios supremo",
    "tecnologia alienigena",
    "tecnología alienígena",
    "bunker cuantico",
    "búnker cuántico",
    "inmortal",
    "capilla sixtina del silicio",
    "algoritmo prohibido",
    "mutacion cuantica",
    "mutación cuántica",
    "revolucion cuantica",
    "revolución cuántica",
    NULL
};

static const char* PALABRAS_CLAVE_UPL[] = {
    "fn ", "funcion ", "sea ", "let ", "struct ", "estructura ",
    "modulo ", "module ", "import ", "importar ", "comp3", "incrustar",
    "embed", "asm", "ensamblador", "retornar", "return",
    NULL
};

/* Comprobación insensible a mayúsculas/minúsculas */
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

bool supervisor_contiene_humo(const char *texto) {
    for (int i = 0; PATRONES_HUMO[i] != NULL; ++i) {
        if (strcasestr_simple(texto, PATRONES_HUMO[i])) {
            return true;
        }
    }
    return false;
}

bool supervisor_tiene_sintaxis_tecnica(const char *texto) {
    for (int i = 0; PALABRAS_CLAVE_UPL[i] != NULL; ++i) {
        if (strstr(texto, PALABRAS_CLAVE_UPL[i]) != NULL) {
            return true;
        }
    }
    return false;
}

/* Extrae todo el contenido situado dentro de bloques ``` ... ``` */
bool supervisor_extraer_bloques_codigo(const char *entrada, char *salida, size_t max_salida) {
    salida[0] = '\0';
    const char *p = entrada;
    size_t total_out = 0;
    bool found_blocks = false;

    while ((p = strstr(p, "```")) != NULL) {
        p += 3; // Saltar ```
        // Omitir tag de lenguaje si existe (ej. ```upl, ```c)
        while (*p && *p != '\n' && !isspace((unsigned char)*p)) p++;
        if (*p == '\n') p++;

        const char *end_block = strstr(p, "```");
        if (!end_block) break;

        found_blocks = true;
        size_t block_len = (size_t)(end_block - p);

        // Omitir espacios en blanco finales
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

int supervisor_auditar(const char *texto_crudo, char *codigo_saneado, size_t max_cod, char *alerta, size_t max_alerta) {
    alerta[0] = '\0';
    codigo_saneado[0] = '\0';

    bool tiene_humo = supervisor_contiene_humo(texto_crudo);
    bool tiene_bloques = supervisor_extraer_bloques_codigo(texto_crudo, codigo_saneado, max_cod);

    if (!tiene_bloques) {
        if (supervisor_tiene_sintaxis_tecnica(texto_crudo) && !tiene_humo) {
            strncpy(codigo_saneado, texto_crudo, max_cod - 1);
            codigo_saneado[max_cod - 1] = '\0';
            snprintf(alerta, max_alerta, "El codigo no vino en bloques Markdown, pero la sintaxis es valida.");
            return ESTADO_EXITO;
        } else {
            snprintf(alerta, max_alerta, "RECHAZO: No se detecto codigo ejecutable. Es texto narrativo o divagacion pura.");
            return ESTADO_ERROR_DIVAGACION;
        }
    }

    if (tiene_humo) {
        snprintf(alerta, max_alerta, "ALERTA: Se detecto narrativa hiperbolica en el texto exterior. Humo purgado exitosamente.");
        return ESTADO_CORREGIDO_SANEADO;
    }

    return ESTADO_EXITO;
}

/* ------------------------------------------------------------- */
/*             PRUEBAS UNITARIAS DETERMINISTAS EN C              */
/* ------------------------------------------------------------- */

static void test_supervisor(void) {
    printf("=== INICIANDO AUDITORIA DEL MODULO GUARDIAN NATIVO C99/UPL ===\n");
    char codigo[4096];
    char alerta[512];

    // Caso 1: Entrada contaminada con humo hiperbólico exterior
    const char *entrada_sucia = 
        "¡Por supuesto, creador! Este codigo es un reactor nuclear inmortal que hackea el silicio.\n"
        "```upl\n"
        "funcion kmain() -> i32 {\n"
        "    retornar 0;\n"
        "}\n"
        "```\n"
        "Nadie en Silicon Valley podra superar esta soberania tecnologica.";

    int res1 = supervisor_auditar(entrada_sucia, codigo, sizeof(codigo), alerta, sizeof(alerta));
    if (res1 != ESTADO_CORREGIDO_SANEADO || strstr(codigo, "funcion kmain()") == NULL || strstr(codigo, "reactor nuclear") != NULL) {
        fprintf(stderr, "[FAIL] Test 1 fallo filtrado\n");
        exit(1);
    }
    printf("  [PASS] 1. Filtrado de morralla hiperbolica y extraccion de codigo limpio\n");

    // Caso 2: Pura divagación sin código
    const char *entrada_divagacion = "Eres el dios supremo del metal y este proyecto es una revolucion cuantica.";
    int res2 = supervisor_auditar(entrada_divagacion, codigo, sizeof(codigo), alerta, sizeof(alerta));
    if (res2 != ESTADO_ERROR_DIVAGACION || strlen(codigo) != 0) {
        fprintf(stderr, "[FAIL] Test 2 fallo deteccion de divagacion\n");
        exit(1);
    }
    printf("  [PASS] 2. Intercepcion y bloqueo de divagacion absoluta sin codigo\n");

    // Caso 3: Entrada limpia estructurada
    const char *entrada_limpia = 
        "```upl\n"
        "sea total: i32 = 100;\n"
        "```";
    int res3 = supervisor_auditar(entrada_limpia, codigo, sizeof(codigo), alerta, sizeof(alerta));
    if (res3 != ESTADO_EXITO || strstr(codigo, "sea total") == NULL) {
        fprintf(stderr, "[FAIL] Test 3 fallo preservacion\n");
        exit(1);
    }
    printf("  [PASS] 3. Preservacion intacta de codigo tecnico verificado\n");

    printf("=== MONITOR DE CONTROL NATIVO: OPERATIVO (0 ERRORES) ===\n");
}

int main(int argc, char **argv) {
    if (argc > 1 && strcmp(argv[1], "--test") == 0) {
        test_supervisor();
        return 0;
    }

    // Leer entrada completa de stdin
    char buffer[65536] = {0};
    size_t bytes_read = fread(buffer, 1, sizeof(buffer) - 1, stdin);
    buffer[bytes_read] = '\0';

    if (bytes_read == 0) {
        // Si no se proporcionaron datos por stdin y no hay args, correr test
        test_supervisor();
        return 0;
    }

    char codigo[65536] = {0};
    char alerta[512] = {0};

    int estado = supervisor_auditar(buffer, codigo, sizeof(codigo), alerta, sizeof(alerta));
    if (alerta[0] != '\0') {
        fprintf(stderr, "[SUPERVISOR_UPL] %s\n", alerta);
    }

    if (estado == ESTADO_ERROR_DIVAGACION) {
        fprintf(stderr, "[FATAL] Salida rechazada por divagacion.\n");
        return 1;
    }

    printf("%s\n", codigo);
    return 0;
}
