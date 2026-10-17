#include "lexer.h"
#include "upl_arena.h"
#include "upl_ast.h"
#include "upl_parser.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

static void test_bilingual_function_parsing(void) {
    const char *source = 
        "fn add(a: i32, b: i32) -> i32 {\n"
        "    return a + b;\n"
        "}\n"
        "funcion restar(x: i64, y: i64) -> i64 {\n"
        "    retornar x - y;\n"
        "}\n";

    UPL_Arena arena;
    upl_arena_init(&arena, 65536);

    Lexer lexer;
    lexer_init(&lexer, source);

    UplParser parser;
    upl_parser_init(&parser, &lexer, &arena);

    UplAstNode *program = upl_parser_parse_program(&parser);
    assert(!parser.had_error);
    assert(program != NULL);
    assert(program->kind == AST_PROGRAM);

    UplAstList *items = program->as.program.items;
    assert(items != NULL);

    // 1. Verificación función EN 'add'
    UplAstNode *fn1 = items->node;
    assert(fn1->kind == AST_DECL_FN);
    assert(strcmp(fn1->as.decl_fn.name, "add") == 0);
    assert(upl_ast_list_length(fn1->as.decl_fn.params) == 2);
    assert(fn1->as.decl_fn.return_type != NULL);
    assert(fn1->as.decl_fn.return_type->kind == AST_TYPE_PRIMITIVE);
    assert(fn1->as.decl_fn.return_type->as.type_spec.prim == UPL_TYPE_I32);

    // 2. Verificación función ES 'restar'
    assert(items->next != NULL);
    UplAstNode *fn2 = items->next->node;
    assert(fn2->kind == AST_DECL_FN);
    assert(strcmp(fn2->as.decl_fn.name, "restar") == 0);
    assert(upl_ast_list_length(fn2->as.decl_fn.params) == 2);
    assert(fn2->as.decl_fn.return_type->as.type_spec.prim == UPL_TYPE_I64);

    upl_arena_free(&arena);
    printf("  [PASS] 1. Parseo formal de funciones bilingues (fn/funcion, return/retornar)\n");
}

static void test_comp3_and_struct_parsing(void) {
    const char *source = 
        "estructura CuentaBancaria {\n"
        "    id: u64;\n"
        "    saldo: comp3(12, 2);\n"
        "}\n";

    UPL_Arena arena;
    upl_arena_init(&arena, 65536);

    Lexer lexer;
    lexer_init(&lexer, source);

    UplParser parser;
    upl_parser_init(&parser, &lexer, &arena);

    UplAstNode *program = upl_parser_parse_program(&parser);
    assert(!parser.had_error);
    assert(program != NULL);

    UplAstNode *st = program->as.program.items->node;
    assert(st->kind == AST_DECL_STRUCT);
    assert(strcmp(st->as.decl_struct.name, "CuentaBancaria") == 0);

    UplAstList *fields = st->as.decl_struct.fields;
    assert(fields != NULL);
    assert(strcmp(fields->node->as.decl_field.name, "id") == 0);
    assert(fields->node->as.decl_field.type->as.type_spec.prim == UPL_TYPE_U64);

    assert(fields->next != NULL);
    assert(strcmp(fields->next->node->as.decl_field.name, "saldo") == 0);
    UplAstNode *comp3_type = fields->next->node->as.decl_field.type;
    assert(comp3_type->kind == AST_TYPE_COMP3);
    assert(comp3_type->as.type_spec.comp3_precision == 12);
    assert(comp3_type->as.type_spec.comp3_scale == 2);

    upl_arena_free(&arena);
    printf("  [PASS] 2. Parseo de estructuras y tipos financieros comp3(precision, escala)\n");
}

static void test_operator_precedence(void) {
    // La precedencia debe garantizar que a + b * c sea (+ a (* b c))
    const char *source = "let res = a + b * c;";

    UPL_Arena arena;
    upl_arena_init(&arena, 65536);

    Lexer lexer;
    lexer_init(&lexer, source);

    UplParser parser;
    upl_parser_init(&parser, &lexer, &arena);

    UplAstNode *stmt = upl_parser_parse_statement(&parser);
    assert(!parser.had_error);
    assert(stmt->kind == AST_STMT_LET);
    assert(stmt->as.stmt_let.init_expr != NULL);

    UplAstNode *init = stmt->as.stmt_let.init_expr;
    // Raíz debe ser suma '+'
    assert(init->kind == AST_EXPR_BINARY);
    assert(init->as.binary_expr.op == TOKEN_OP_ADD);

    // Lado izquierdo es 'a'
    assert(init->as.binary_expr.left->kind == AST_IDENTIFIER);
    assert(strcmp(init->as.binary_expr.left->as.ident.name, "a") == 0);

    // Lado derecho es (* b c)
    UplAstNode *right = init->as.binary_expr.right;
    assert(right->kind == AST_EXPR_BINARY);
    assert(right->as.binary_expr.op == TOKEN_OP_MUL);
    assert(strcmp(right->as.binary_expr.left->as.ident.name, "b") == 0);
    assert(strcmp(right->as.binary_expr.right->as.ident.name, "c") == 0);

    upl_arena_free(&arena);
    printf("  [PASS] 3. Precedencia matematica de operadores (Dragon Book Pratt Parsing)\n");
}

static void test_control_flow_bilingual(void) {
    const char *source = 
        "si (x > 10) {\n"
        "    sea mut y = x * 2;\n"
        "} sino {\n"
        "    retornar 0;\n"
        "}\n";

    UPL_Arena arena;
    upl_arena_init(&arena, 65536);

    Lexer lexer;
    lexer_init(&lexer, source);

    UplParser parser;
    upl_parser_init(&parser, &lexer, &arena);

    UplAstNode *stmt = upl_parser_parse_statement(&parser);
    assert(!parser.had_error);
    assert(stmt->kind == AST_STMT_IF);
    assert(stmt->as.stmt_if.cond->kind == AST_EXPR_BINARY);
    assert(stmt->as.stmt_if.cond->as.binary_expr.op == TOKEN_OP_GREATER);

    // Then block
    assert(stmt->as.stmt_if.then_block->kind == AST_STMT_BLOCK);
    UplAstNode *let_stmt = stmt->as.stmt_if.then_block->as.block.stmts->node;
    assert(let_stmt->kind == AST_STMT_LET);
    assert(let_stmt->as.stmt_let.is_mut == true);

    // Else branch
    assert(stmt->as.stmt_if.else_branch->kind == AST_STMT_BLOCK);
    UplAstNode *ret_stmt = stmt->as.stmt_if.else_branch->as.block.stmts->node;
    assert(ret_stmt->kind == AST_STMT_RETURN);

    upl_arena_free(&arena);
    printf("  [PASS] 4. Estructuras de control de flujo (si/sino, mutabilidad)\n");
}

static void test_raw_embed_block_ast(void) {
    const char *source = "incrustar { volatile int status = PEEK(0x4000); }";

    UPL_Arena arena;
    upl_arena_init(&arena, 65536);

    Lexer lexer;
    lexer_init(&lexer, source);

    UplParser parser;
    upl_parser_init(&parser, &lexer, &arena);

    UplAstNode *node = upl_parser_parse_statement(&parser);
    assert(!parser.had_error);
    assert(node->kind == AST_STMT_EMBED);
    assert(strstr(node->as.raw_block.raw_code, "volatile int status") != NULL);

    upl_arena_free(&arena);
    printf("  [PASS] 5. Captura y preservacion de bloques C99 nativos (embed/incrustar)\n");
}

static void test_parser_syntax_error(void) {
    // Falta ';' en sentencia let
    const char *source = "let x = 100\n let y = 200;";

    UPL_Arena arena;
    upl_arena_init(&arena, 65536);

    Lexer lexer;
    lexer_init(&lexer, source);

    UplParser parser;
    upl_parser_init(&parser, &lexer, &arena);

    upl_parser_parse_program(&parser);
    assert(parser.had_error);
    assert(parser.error_line == 2);
    assert(strstr(parser.error_message, "Error sintactico") != NULL);

    upl_arena_free(&arena);
    printf("  [PASS] 6. Deteccion y localizacion precisa de errores sintacticos (L:C)\n");
}

int main(void) {
    printf("====================================================================\n");
    printf("  UPL+ SPEC SUITE: VERIFICACION FORMAL DEL ANALIZADOR SINTACTICO   \n");
    printf("====================================================================\n");

    test_bilingual_function_parsing();
    test_comp3_and_struct_parsing();
    test_operator_precedence();
    test_control_flow_bilingual();
    test_raw_embed_block_ast();
    test_parser_syntax_error();

    printf("====================================================================\n");
    printf("  RESULTADO: 6/6 SUITES PARSER COMPLETADAS CON EXITO (0 FALLOS)     \n");
    printf("====================================================================\n");
    return 0;
}
