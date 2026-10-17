#ifndef UPL_PARSER_H
#define UPL_PARSER_H

#include "lexer.h"
#include "upl_arena.h"
#include "upl_ast.h"
#include <stdbool.h>

/**
 * Analizador Sintáctico de Descenso Recursivo para UPL+
 * 
 * Implementa la especificación formal EBNF y construye el Árbol
 * de Sintaxis Abstracta (AST) de forma determinista y agnóstica al idioma.
 */

typedef struct {
    Lexer *lexer;
    UPL_Arena *arena;
    Token current;
    Token previous;
    bool had_error;
    bool panic_mode;
    char error_message[256];
    size_t error_line;
    size_t error_column;
} UplParser;

/* Inicializar el parser con un flujo léxico y un arena de memoria */
void upl_parser_init(UplParser *parser, Lexer *lexer, UPL_Arena *arena);

/* Parsear una unidad de traducción completa (programa) */
UplAstNode* upl_parser_parse_program(UplParser *parser);

/* Parsear una sentencia individual (útil para REPL o módulos parciales) */
UplAstNode* upl_parser_parse_statement(UplParser *parser);

/* Parsear una expresión aislada con precedencia completa */
UplAstNode* upl_parser_parse_expression(UplParser *parser);

/* Parsear una especificación de tipo */
UplAstNode* upl_parser_parse_type(UplParser *parser);

#endif /* UPL_PARSER_H */
