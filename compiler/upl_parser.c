#include "upl_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Precedencia de Operadores para el analizador de expresiones (Pratt/Climbing) */
typedef enum {
    PREC_NONE,
    PREC_ASSIGNMENT, // =
    PREC_OR,         // ||
    PREC_AND,        // &&
    PREC_BIT_OR,     // |
    PREC_BIT_XOR,    // ^
    PREC_BIT_AND,    // &
    PREC_EQUALITY,   // ==, !=
    PREC_COMPARISON, // <, <=, >, >=
    PREC_SHIFT,      // <<, >>
    PREC_TERM,       // +, -
    PREC_FACTOR,     // *, /, %
    PREC_CAST,       // as / como
    PREC_UNARY,      // !, -
    PREC_CALL,       // ., (), []
    PREC_PRIMARY
} Precedence;

static void error_at(UplParser *p, Token *tok, const char *message) {
    if (p->panic_mode) return;
    p->panic_mode = true;
    p->had_error = true;
    p->error_line = tok->line;
    p->error_column = tok->column;
    snprintf(p->error_message, sizeof(p->error_message), "[L%zu:C%zu] Error sintactico: %s (token '%s')",
             tok->line, tok->column, message, tok->lexeme ? tok->lexeme : "");
}

static void advance(UplParser *p) {
    token_free(&p->previous);
    p->previous = p->current;

    for (;;) {
        p->current = lexer_next_token(p->lexer);
        if (p->current.type != TOKEN_ERROR) break;
        error_at(p, &p->current, p->current.lexeme);
    }
}

static bool check(UplParser *p, TokenType type) {
    return p->current.type == type;
}

static bool match(UplParser *p, TokenType type) {
    if (!check(p, type)) return false;
    advance(p);
    return true;
}

static bool consume(UplParser *p, TokenType type, const char *message) {
    if (check(p, type)) {
        advance(p);
        return true;
    }
    error_at(p, &p->current, message);
    return false;
}

/* Declaraciones adelantadas */
static UplAstNode* parse_declaration(UplParser *p);
static UplAstNode* parse_block(UplParser *p);
static UplAstNode* parse_expression_precedence(UplParser *p, Precedence prec);

void upl_parser_init(UplParser *parser, Lexer *lexer, UPL_Arena *arena) {
    parser->lexer = lexer;
    parser->arena = arena;
    parser->had_error = false;
    parser->panic_mode = false;
    parser->error_message[0] = '\0';
    parser->error_line = 0;
    parser->error_column = 0;
    parser->previous.type = TOKEN_EOF;
    parser->previous.lexeme = NULL;
    parser->current.type = TOKEN_EOF;
    parser->current.lexeme = NULL;

    advance(parser); // Cargar primer token
}

/* ------------------------------------------------------------- */
/*                ANÁLISIS DE TIPOS DE DATOS                     */
/* ------------------------------------------------------------- */

static UplPrimitiveType get_primitive_type(const char *name, bool *is_prim) {
    *is_prim = true;
    if (strcmp(name, "i8") == 0) return UPL_TYPE_I8;
    if (strcmp(name, "i16") == 0) return UPL_TYPE_I16;
    if (strcmp(name, "i32") == 0 || strcmp(name, "entero") == 0) return UPL_TYPE_I32;
    if (strcmp(name, "i64") == 0) return UPL_TYPE_I64;
    if (strcmp(name, "u8") == 0) return UPL_TYPE_U8;
    if (strcmp(name, "u16") == 0) return UPL_TYPE_U16;
    if (strcmp(name, "u32") == 0) return UPL_TYPE_U32;
    if (strcmp(name, "u64") == 0) return UPL_TYPE_U64;
    if (strcmp(name, "usize") == 0) return UPL_TYPE_USIZE;
    if (strcmp(name, "f32") == 0) return UPL_TYPE_F32;
    if (strcmp(name, "f64") == 0 || strcmp(name, "flotante") == 0) return UPL_TYPE_F64;
    if (strcmp(name, "bool") == 0 || strcmp(name, "booleano") == 0) return UPL_TYPE_BOOL;
    if (strcmp(name, "str") == 0 || strcmp(name, "cadena") == 0) return UPL_TYPE_STR;
    if (strcmp(name, "void") == 0 || strcmp(name, "vacio") == 0) return UPL_TYPE_VOID;
    *is_prim = false;
    return UPL_TYPE_VOID;
}

UplAstNode* upl_parser_parse_type(UplParser *p) {
    size_t line = p->current.line;
    size_t col = p->current.column;

    // 1. Puntero: *T
    if (match(p, TOKEN_OP_MUL)) {
        UplAstNode *base = upl_parser_parse_type(p);
        return upl_ast_new_type_pointer(p->arena, base, line, col);
    }

    // 2. Arreglo: [T; N]
    if (match(p, TOKEN_DELIM_LBRACKET)) {
        UplAstNode *elem = upl_parser_parse_type(p);
        consume(p, TOKEN_DELIM_SEMICOLON, "Esperaba ';' tras tipo de elemento de arreglo");
        size_t len = 0;
        if (check(p, TOKEN_LITERAL_INT)) {
            len = (size_t)atoll(p->current.lexeme);
            advance(p);
        } else {
            error_at(p, &p->current, "Esperaba longitud entera para arreglo");
        }
        consume(p, TOKEN_DELIM_RBRACKET, "Esperaba ']' al cerrar declaracion de arreglo");
        return upl_ast_new_type_array(p->arena, elem, len, line, col);
    }

    // 3. Tipo comp3(precision, escala)
    if (match(p, TOKEN_KEYWORD_COMP3)) {
        consume(p, TOKEN_DELIM_LPAREN, "Esperaba '(' tras comp3");
        int prec = 0, scale = 0;
        if (check(p, TOKEN_LITERAL_INT)) {
            prec = atoi(p->current.lexeme);
            advance(p);
        } else {
            error_at(p, &p->current, "Esperaba digito entero para precision comp3");
        }
        consume(p, TOKEN_DELIM_COMMA, "Esperaba ',' entre precision y escala de comp3");
        if (check(p, TOKEN_LITERAL_INT)) {
            scale = atoi(p->current.lexeme);
            advance(p);
        } else {
            error_at(p, &p->current, "Esperaba digito entero para escala comp3");
        }
        consume(p, TOKEN_DELIM_RPAREN, "Esperaba ')' tras comp3");
        return upl_ast_new_type_comp3(p->arena, prec, scale, line, col);
    }

    // 4. Tipo Primitivo o Estructura Personalizada por Identificador
    if (match(p, TOKEN_IDENTIFIER)) {
        const char *name = upl_arena_strdup(p->arena, p->previous.lexeme);
        bool is_prim = false;
        UplPrimitiveType prim = get_primitive_type(name, &is_prim);
        if (is_prim) {
            return upl_ast_new_type_primitive(p->arena, prim, line, col);
        }
        return upl_ast_new_type_custom(p->arena, name, line, col);
    }

    error_at(p, &p->current, "Se esperaba un especificador de tipo valido");
    return NULL;
}

/* ------------------------------------------------------------- */
/*                ANÁLISIS DE EXPRESIONES (PRATT)                */
/* ------------------------------------------------------------- */

static Precedence get_precedence(TokenType type) {
    switch (type) {
        case TOKEN_OP_ASSIGN:
        case TOKEN_OP_ADD_ASSIGN:
        case TOKEN_OP_SUB_ASSIGN:
        case TOKEN_OP_MUL_ASSIGN:
        case TOKEN_OP_DIV_ASSIGN:
        case TOKEN_OP_MOD_ASSIGN:
        case TOKEN_OP_AND_ASSIGN:
        case TOKEN_OP_OR_ASSIGN:
        case TOKEN_OP_XOR_ASSIGN:
        case TOKEN_OP_SHL_ASSIGN:
        case TOKEN_OP_SHR_ASSIGN:    return PREC_ASSIGNMENT;
        case TOKEN_OP_OR:            return PREC_OR;
        case TOKEN_OP_AND:           return PREC_AND;
        case TOKEN_OP_BIT_OR:        return PREC_BIT_OR;
        case TOKEN_OP_XOR:           return PREC_BIT_XOR;
        case TOKEN_OP_BIT_AND:       return PREC_BIT_AND;
        case TOKEN_OP_EQUAL:
        case TOKEN_OP_NOT_EQUAL:     return PREC_EQUALITY;
        case TOKEN_OP_LESS:
        case TOKEN_OP_GREATER:
        case TOKEN_OP_LESS_EQUAL:
        case TOKEN_OP_GREATER_EQUAL: return PREC_COMPARISON;
        case TOKEN_OP_SHL:
        case TOKEN_OP_SHR:           return PREC_SHIFT;
        case TOKEN_OP_ADD:
        case TOKEN_OP_SUB:           return PREC_TERM;
        case TOKEN_OP_MUL:
        case TOKEN_OP_DIV:
        case TOKEN_OP_MOD:           return PREC_FACTOR;
        case TOKEN_KEYWORD_AS:       return PREC_CAST;
        case TOKEN_DELIM_LPAREN:
        case TOKEN_DELIM_LBRACKET:
        case TOKEN_DELIM_DOT:        return PREC_CALL;
        default:                     return PREC_NONE;
    }
}

static UplAstNode* parse_primary(UplParser *p) {
    size_t line = p->current.line;
    size_t col = p->current.column;

    if (match(p, TOKEN_LITERAL_INT)) {
        int64_t val = (int64_t)strtoll(p->previous.lexeme, NULL, 0);
        return upl_ast_new_literal_int(p->arena, val, line, col);
    }

    if (match(p, TOKEN_LITERAL_FLOAT)) {
        double val = strtod(p->previous.lexeme, NULL);
        return upl_ast_new_literal_float(p->arena, val, line, col);
    }

    if (match(p, TOKEN_LITERAL_STRING)) {
        return upl_ast_new_literal_str(p->arena, p->previous.lexeme, line, col);
    }

    if (match(p, TOKEN_IDENTIFIER)) {
        const char *name = p->previous.lexeme;
        if (strcmp(name, "true") == 0 || strcmp(name, "verdadero") == 0) {
            return upl_ast_new_literal_bool(p->arena, true, line, col);
        }
        if (strcmp(name, "false") == 0 || strcmp(name, "falso") == 0) {
            return upl_ast_new_literal_bool(p->arena, false, line, col);
        }
        return upl_ast_new_ident(p->arena, name, line, col);
    }

    if (match(p, TOKEN_DELIM_LPAREN)) {
        UplAstNode *expr = upl_parser_parse_expression(p);
        consume(p, TOKEN_DELIM_RPAREN, "Esperaba ')' tras expresion agrupada");
        return expr;
    }

    error_at(p, &p->current, "Esperaba expresion primaria (literal o identificador)");
    return NULL;
}

static UplAstNode* parse_postfix(UplParser *p, UplAstNode *left);

static UplAstNode* parse_unary(UplParser *p) {
    size_t line = p->current.line;
    size_t col = p->current.column;

    if (match(p, TOKEN_OP_SUB) || match(p, TOKEN_OP_NOT) || match(p, TOKEN_OP_MUL) ||
        match(p, TOKEN_OP_BIT_NOT) || match(p, TOKEN_OP_BIT_AND)) {
        TokenType op = p->previous.type;
        UplAstNode *operand = parse_unary(p);
        return upl_ast_new_unary(p->arena, op, operand, line, col);
    }

    UplAstNode *left = parse_primary(p);
    while (check(p, TOKEN_DELIM_LPAREN) || check(p, TOKEN_DELIM_DOT) || check(p, TOKEN_DELIM_LBRACKET) || check(p, TOKEN_KEYWORD_AS)) {
        left = parse_postfix(p, left);
    }
    return left;
}

static UplAstNode* parse_postfix(UplParser *p, UplAstNode *left) {
    size_t line = p->previous.line;
    size_t col = p->previous.column;

    // Llamada a función: f(args...)
    if (match(p, TOKEN_DELIM_LPAREN)) {
        UplAstList *args = NULL;
        if (!check(p, TOKEN_DELIM_RPAREN)) {
            do {
                UplAstNode *arg = upl_parser_parse_expression(p);
                upl_ast_list_append(p->arena, &args, arg);
            } while (match(p, TOKEN_DELIM_COMMA));
        }
        consume(p, TOKEN_DELIM_RPAREN, "Esperaba ')' al cerrar argumentos de funcion");
        return upl_ast_new_call(p->arena, left, args, line, col);
    }

    // Acceso a miembro: obj.campo
    if (match(p, TOKEN_DELIM_DOT)) {
        consume(p, TOKEN_IDENTIFIER, "Esperaba nombre de campo tras '.'");
        const char *member = upl_arena_strdup(p->arena, p->previous.lexeme);
        return upl_ast_new_member(p->arena, left, member, line, col);
    }

    // Indexación: arr[idx]
    if (match(p, TOKEN_DELIM_LBRACKET)) {
        UplAstNode *idx = upl_parser_parse_expression(p);
        consume(p, TOKEN_DELIM_RBRACKET, "Esperaba ']' tras indice de arreglo");
        return upl_ast_new_index(p->arena, left, idx, line, col);
    }

    // Conversión de tipos explícita: expr as T
    if (match(p, TOKEN_KEYWORD_AS)) {
        UplAstNode *target_type = upl_parser_parse_type(p);
        return upl_ast_new_cast(p->arena, left, target_type, line, col);
    }

    return left;
}

static UplAstNode* parse_expression_precedence(UplParser *p, Precedence prec) {
    UplAstNode *left = parse_unary(p);
    if (!left) return NULL;

    // Sufijos inmediatos (llamadas, indexaciones, puntos)
    while (check(p, TOKEN_DELIM_LPAREN) || check(p, TOKEN_DELIM_DOT) || check(p, TOKEN_DELIM_LBRACKET) || check(p, TOKEN_KEYWORD_AS)) {
        left = parse_postfix(p, left);
    }

    while (prec <= get_precedence(p->current.type)) {
        advance(p);
        TokenType op = p->previous.type;
        size_t line = p->previous.line;
        size_t col = p->previous.column;

        Precedence next_prec = get_precedence(op);
        // Operadores de asignación son asociativos a la derecha
        bool is_assign = (op == TOKEN_OP_ASSIGN ||
                          op == TOKEN_OP_ADD_ASSIGN ||
                          op == TOKEN_OP_SUB_ASSIGN ||
                          op == TOKEN_OP_MUL_ASSIGN ||
                          op == TOKEN_OP_DIV_ASSIGN ||
                          op == TOKEN_OP_MOD_ASSIGN ||
                          op == TOKEN_OP_AND_ASSIGN ||
                          op == TOKEN_OP_OR_ASSIGN ||
                          op == TOKEN_OP_XOR_ASSIGN ||
                          op == TOKEN_OP_SHL_ASSIGN ||
                          op == TOKEN_OP_SHR_ASSIGN);
        if (!is_assign) {
            next_prec = (Precedence)(next_prec + 1);
        }

        UplAstNode *right = parse_expression_precedence(p, next_prec);
        left = upl_ast_new_binary(p->arena, op, left, right, line, col);

        while (check(p, TOKEN_DELIM_LPAREN) || check(p, TOKEN_DELIM_DOT) || check(p, TOKEN_DELIM_LBRACKET) || check(p, TOKEN_KEYWORD_AS)) {
            left = parse_postfix(p, left);
        }
    }

    return left;
}

UplAstNode* upl_parser_parse_expression(UplParser *p) {
    return parse_expression_precedence(p, PREC_ASSIGNMENT);
}

/* ------------------------------------------------------------- */
/*                ANÁLISIS DE SENTENCIAS                         */
/* ------------------------------------------------------------- */

static UplAstNode* parse_block(UplParser *p) {
    size_t line = p->current.line;
    size_t col = p->current.column;
    consume(p, TOKEN_DELIM_LBRACE, "Esperaba '{' al iniciar bloque");

    UplAstList *stmts = NULL;
    while (!check(p, TOKEN_DELIM_RBRACE) && !check(p, TOKEN_EOF)) {
        size_t prev_line = p->current.line;
        size_t prev_col = p->current.column;
        TokenType prev_type = p->current.type;

        UplAstNode *stmt = upl_parser_parse_statement(p);
        if (stmt) {
            upl_ast_list_append(p->arena, &stmts, stmt);
        }
        if (p->current.line == prev_line && p->current.column == prev_col && p->current.type == prev_type) {
            advance(p);
        }
    }

    consume(p, TOKEN_DELIM_RBRACE, "Esperaba '}' al cerrar bloque");
    return upl_ast_new_block(p->arena, stmts, line, col);
}

UplAstNode* upl_parser_parse_statement(UplParser *p) {
    size_t line = p->current.line;
    size_t col = p->current.column;

    // 1. Sentencia Let / Const (sea / let / const / constante)
    if (match(p, TOKEN_KEYWORD_LET) || match(p, TOKEN_KEYWORD_CONST)) {
        bool is_mut = match(p, TOKEN_KEYWORD_MUT);
        consume(p, TOKEN_IDENTIFIER, "Esperaba identificador para declaracion de variable");
        const char *name = upl_arena_strdup(p->arena, p->previous.lexeme);
        UplAstNode *type = NULL;
        UplAstNode *init_expr = NULL;

        if (match(p, TOKEN_DELIM_COLON)) {
            type = upl_parser_parse_type(p);
        }

        if (match(p, TOKEN_OP_ASSIGN)) {
            init_expr = upl_parser_parse_expression(p);
        }

        consume(p, TOKEN_DELIM_SEMICOLON, "Esperaba ';' al finalizar declaracion let");
        return upl_ast_new_stmt_let(p->arena, name, type, init_expr, is_mut, line, col);
    }

    // 2. Sentencia If (si / if)
    if (match(p, TOKEN_KEYWORD_IF)) {
        UplAstNode *cond = upl_parser_parse_expression(p);
        UplAstNode *then_block = parse_block(p);
        UplAstNode *else_branch = NULL;

        if (match(p, TOKEN_KEYWORD_ELSE)) {
            if (check(p, TOKEN_KEYWORD_IF)) {
                else_branch = upl_parser_parse_statement(p);
            } else {
                else_branch = parse_block(p);
            }
        }
        return upl_ast_new_stmt_if(p->arena, cond, then_block, else_branch, line, col);
    }

    // 3. Sentencia While (mientras / while)
    if (match(p, TOKEN_KEYWORD_WHILE)) {
        UplAstNode *cond = upl_parser_parse_expression(p);
        UplAstNode *body = parse_block(p);
        return upl_ast_new_stmt_while(p->arena, cond, body, line, col);
    }

    // 4. Sentencia Return (retornar / return)
    if (match(p, TOKEN_KEYWORD_RETURN)) {
        UplAstNode *expr = NULL;
        if (!check(p, TOKEN_DELIM_SEMICOLON)) {
            expr = upl_parser_parse_expression(p);
        }
        consume(p, TOKEN_DELIM_SEMICOLON, "Esperaba ';' tras return");
        return upl_ast_new_stmt_return(p->arena, expr, line, col);
    }

    // Sentencia Defer (diferir / defer)
    if (match(p, TOKEN_KEYWORD_DEFER)) {
        UplAstNode *deferred = upl_parser_parse_statement(p);
        return upl_ast_new_stmt_defer(p->arena, deferred, line, col);
    }

    // Sentencia Match / Segun (coincidencia de patrones / switch)
    if (match(p, TOKEN_KEYWORD_MATCH)) {
        UplAstNode *target = upl_parser_parse_expression(p);
        consume(p, TOKEN_DELIM_LBRACE, "Esperaba '{' para iniciar bloque de casos en match/segun");
        
        UplAstList *cases = NULL;
        while (!check(p, TOKEN_DELIM_RBRACE) && !check(p, TOKEN_EOF)) {
            size_t c_line = p->current.line;
            size_t c_col = p->current.column;
            bool is_def = false;
            UplAstNode *pattern = NULL;

            if (match(p, TOKEN_KEYWORD_DEFAULT)) {
                is_def = true;
            } else if (check(p, TOKEN_IDENTIFIER) && strcmp(p->current.lexeme, "_") == 0) {
                advance(p);
                is_def = true;
            } else if (match(p, TOKEN_KEYWORD_CASE)) {
                pattern = upl_parser_parse_expression(p);
            } else {
                pattern = upl_parser_parse_expression(p);
            }

            if (!match(p, TOKEN_FAT_ARROW)) {
                consume(p, TOKEN_DELIM_COLON, "Esperaba '=>' o ':' tras caso en match/segun");
            }

            UplAstNode *body = upl_parser_parse_statement(p);
            UplAstNode *cnode = upl_ast_new_match_case(p->arena, pattern, body, is_def, c_line, c_col);
            upl_ast_list_append(p->arena, &cases, cnode);

            match(p, TOKEN_DELIM_COMMA);
        }

        consume(p, TOKEN_DELIM_RBRACE, "Esperaba '}' para finalizar bloque match/segun");
        return upl_ast_new_stmt_match(p->arena, target, cases, line, col);
    }

    // 5. Bloque Embed inline (incrustar / embed)
    if (match(p, TOKEN_KEYWORD_EMBED)) {
        if (check(p, TOKEN_LITERAL_STRING)) {
            advance(p);
        }
        if (!check(p, TOKEN_DELIM_LBRACE)) {
            error_at(p, &p->current, "Esperaba '{' para iniciar bloque embed/incrustar");
            return NULL;
        }
        Token raw = read_raw_block_body(p->lexer, p->current.line, p->current.column);
        if (raw.type == TOKEN_ERROR) {
            error_at(p, &raw, raw.lexeme);
            token_free(&raw);
            return NULL;
        }
        UplAstNode *node = upl_ast_new_raw_block(p->arena, AST_STMT_EMBED, raw.lexeme, line, col);
        token_free(&raw);
        advance(p);
        return node;
    }

    // Sentencia Break (break / romper)
    if (match(p, TOKEN_KEYWORD_BREAK)) {
        consume(p, TOKEN_DELIM_SEMICOLON, "Esperaba ';' tras break/romper");
        return upl_ast_new_stmt_break(p->arena, line, col);
    }

    // Sentencia Continue (continue / continuar)
    if (match(p, TOKEN_KEYWORD_CONTINUE)) {
        consume(p, TOKEN_DELIM_SEMICOLON, "Esperaba ';' tras continue/continuar");
        return upl_ast_new_stmt_continue(p->arena, line, col);
    }

    // 6. Bloque ASM inline (ensamblador / asm)
    if (match(p, TOKEN_KEYWORD_ASM)) {
        if (!check(p, TOKEN_DELIM_LBRACE)) {
            error_at(p, &p->current, "Esperaba '{' para iniciar bloque asm/ensamblador");
            return NULL;
        }
        Token raw = read_raw_block_body(p->lexer, p->current.line, p->current.column);
        if (raw.type == TOKEN_ERROR) {
            error_at(p, &raw, raw.lexeme);
            token_free(&raw);
            return NULL;
        }
        UplAstNode *node = upl_ast_new_raw_block(p->arena, AST_STMT_ASM, raw.lexeme, line, col);
        token_free(&raw);
        advance(p);
        return node;
    }

    // 7. Bloque compuesto { ... }
    if (check(p, TOKEN_DELIM_LBRACE)) {
        return parse_block(p);
    }

    // 8. Sentencia de Expresión
    UplAstNode *expr = upl_parser_parse_expression(p);
    consume(p, TOKEN_DELIM_SEMICOLON, "Esperaba ';' tras expresion");
    return upl_ast_new_stmt_expr(p->arena, expr, line, col);
}

/* ------------------------------------------------------------- */
/*            ANÁLISIS DE DECLARACIONES DE ALTO NIVEL            */
/* ------------------------------------------------------------- */

static UplAstNode* parse_fn_declaration(UplParser *p, bool is_pub) {
    size_t line = p->previous.line;
    size_t col = p->previous.column;

    consume(p, TOKEN_IDENTIFIER, "Esperaba nombre de funcion");
    const char *name = upl_arena_strdup(p->arena, p->previous.lexeme);

    consume(p, TOKEN_DELIM_LPAREN, "Esperaba '(' tras nombre de funcion");
    UplAstList *params = NULL;

    if (!check(p, TOKEN_DELIM_RPAREN)) {
        do {
            consume(p, TOKEN_IDENTIFIER, "Esperaba nombre de parametro");
            const char *pname = upl_arena_strdup(p->arena, p->previous.lexeme);
            consume(p, TOKEN_DELIM_COLON, "Esperaba ':' tras nombre de parametro");
            UplAstNode *ptype = upl_parser_parse_type(p);
            UplAstNode *param = upl_ast_new_decl_param(p->arena, pname, ptype, line, col);
            upl_ast_list_append(p->arena, &params, param);
        } while (match(p, TOKEN_DELIM_COMMA));
    }

    consume(p, TOKEN_DELIM_RPAREN, "Esperaba ')' tras lista de parametros");

    UplAstNode *ret_type = NULL;
    if (match(p, TOKEN_DELIM_ARROW)) {
        ret_type = upl_parser_parse_type(p);
    }

    UplAstNode *body = parse_block(p);
    return upl_ast_new_decl_fn(p->arena, name, params, ret_type, body, is_pub, line, col);
}

static UplAstNode* parse_struct_declaration(UplParser *p) {
    size_t line = p->previous.line;
    size_t col = p->previous.column;

    consume(p, TOKEN_IDENTIFIER, "Esperaba nombre de estructura");
    const char *name = upl_arena_strdup(p->arena, p->previous.lexeme);

    consume(p, TOKEN_DELIM_LBRACE, "Esperaba '{' para definir campos de estructura");
    UplAstList *fields = NULL;

    while (!check(p, TOKEN_DELIM_RBRACE) && !check(p, TOKEN_EOF)) {
        consume(p, TOKEN_IDENTIFIER, "Esperaba nombre de campo");
        const char *fname = upl_arena_strdup(p->arena, p->previous.lexeme);
        consume(p, TOKEN_DELIM_COLON, "Esperaba ':' tras nombre de campo");
        UplAstNode *ftype = upl_parser_parse_type(p);
        if (!match(p, TOKEN_DELIM_SEMICOLON)) {
            match(p, TOKEN_DELIM_COMMA);
        }
        UplAstNode *field = upl_ast_new_decl_field(p->arena, fname, ftype, line, col);
        upl_ast_list_append(p->arena, &fields, field);

        if (p->had_error) {
            while (!check(p, TOKEN_DELIM_RBRACE) && !check(p, TOKEN_EOF)) {
                advance(p);
            }
            break;
        }
    }

    consume(p, TOKEN_DELIM_RBRACE, "Esperaba '}' al cerrar estructura");
    return upl_ast_new_decl_struct(p->arena, name, fields, line, col);
}

static UplAstNode* parse_declaration(UplParser *p) {
    size_t line = p->current.line;
    size_t col = p->current.column;

    // Atributos y decoradores de sistemas: @snapshot, @baremetal, @pure, etc.
    while (match(p, TOKEN_AT)) {
        consume(p, TOKEN_IDENTIFIER, "Esperaba identificador de atributo tras '@'");
        if (match(p, TOKEN_DELIM_LPAREN)) {
            while (!check(p, TOKEN_DELIM_RPAREN) && !check(p, TOKEN_EOF)) {
                advance(p);
            }
            consume(p, TOKEN_DELIM_RPAREN, "Esperaba ')' tras argumentos de atributo");
        }
    }

    // Módulo: module / modulo nombre;
    if (match(p, TOKEN_KEYWORD_MODULE)) {
        consume(p, TOKEN_IDENTIFIER, "Esperaba identificador de modulo");
        const char *name = upl_arena_strdup(p->arena, p->previous.lexeme);
        consume(p, TOKEN_DELIM_SEMICOLON, "Esperaba ';' tras declaracion de modulo");
        return upl_ast_new_decl_module(p->arena, name, line, col);
    }

    // Constante a nivel de módulo: const / constante
    if (match(p, TOKEN_KEYWORD_CONST)) {
        consume(p, TOKEN_IDENTIFIER, "Esperaba identificador para declaracion const");
        const char *name = upl_arena_strdup(p->arena, p->previous.lexeme);
        UplAstNode *type = NULL;
        UplAstNode *init_expr = NULL;

        if (match(p, TOKEN_DELIM_COLON)) {
            type = upl_parser_parse_type(p);
        }

        if (match(p, TOKEN_OP_ASSIGN)) {
            init_expr = upl_parser_parse_expression(p);
        }

        consume(p, TOKEN_DELIM_SEMICOLON, "Esperaba ';' al finalizar declaracion const");
        return upl_ast_new_stmt_let(p->arena, name, type, init_expr, false, line, col);
    }

    // Importación: import / importar "modulo" o identificador;
    if (match(p, TOKEN_KEYWORD_IMPORT)) {
        const char *path = "";
        if (match(p, TOKEN_LITERAL_STRING) || match(p, TOKEN_IDENTIFIER)) {
            path = upl_arena_strdup(p->arena, p->previous.lexeme);
        } else {
            error_at(p, &p->current, "Esperaba nombre o ruta de modulo para import");
        }
        consume(p, TOKEN_DELIM_SEMICOLON, "Esperaba ';' tras import");
        return upl_ast_new_decl_import(p->arena, path, line, col);
    }

    // Función o estructura pública
    if (match(p, TOKEN_KEYWORD_PUB)) {
        if (match(p, TOKEN_KEYWORD_FN)) {
            return parse_fn_declaration(p, true);
        }
        if (match(p, TOKEN_KEYWORD_STRUCT)) {
            return parse_struct_declaration(p);
        }
        if (match(p, TOKEN_KEYWORD_CONST)) {
            consume(p, TOKEN_IDENTIFIER, "Esperaba identificador para declaracion const");
            const char *name = upl_arena_strdup(p->arena, p->previous.lexeme);
            UplAstNode *type = NULL;
            UplAstNode *init_expr = NULL;

            if (match(p, TOKEN_DELIM_COLON)) {
                type = upl_parser_parse_type(p);
            }

            if (match(p, TOKEN_OP_ASSIGN)) {
                init_expr = upl_parser_parse_expression(p);
            }

            consume(p, TOKEN_DELIM_SEMICOLON, "Esperaba ';' al finalizar declaracion const");
            return upl_ast_new_stmt_let(p->arena, name, type, init_expr, false, line, col);
        }
        error_at(p, &p->current, "Esperaba 'fn'/'funcion', 'struct'/'estructura' o 'const' tras modificador pub/publico");
        return NULL;
    }

    // Función estándar
    if (match(p, TOKEN_KEYWORD_FN)) {
        return parse_fn_declaration(p, false);
    }

    // Estructura
    if (match(p, TOKEN_KEYWORD_STRUCT)) {
        return parse_struct_declaration(p);
    }

    // Sentencias en el nivel superior (scripting o bloques crudos)
    return upl_parser_parse_statement(p);
}

UplAstNode* upl_parser_parse_program(UplParser *p) {
    UplAstList *items = NULL;
    size_t line = p->current.line;
    size_t col = p->current.column;

    while (!check(p, TOKEN_EOF)) {
        UplAstNode *decl = parse_declaration(p);
        if (decl) {
            upl_ast_list_append(p->arena, &items, decl);
        } else {
            advance(p);
        }
    }

    return upl_ast_new_program(p->arena, items, line, col);
}
