#ifndef UPL_CODEGEN_H
#define UPL_CODEGEN_H

#include "upl_ast.h"
#include <stdio.h>
#include <stdbool.h>

/**
 * Generador de Código C99 (Backend Real) para UPL+
 * 
 * Transforma el Árbol de Sintaxis Abstracta (AST) validado
 * en código C99 estándar, portable y optimizable por GCC/Clang.
 */

typedef struct {
    FILE *out;
    int indent_level;
    bool had_error;
    char error_message[256];
    char pointer_vars[128][64];
    int pointer_var_count;
} UplCodeGen;

/* Inicializar el generador con un flujo de salida */
void upl_codegen_init(UplCodeGen *gen, FILE *out);

/* Emitir código C99 completo a partir de un nodo AST_PROGRAM */
bool upl_codegen_generate(UplCodeGen *gen, const UplAstNode *program);

#endif /* UPL_CODEGEN_H */
