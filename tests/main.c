#include "lexer.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

void test_bilingual_keywords(void) {
    const char* source = "funcion fn sea let estructura struct";
    Lexer lexer;
    lexer_init(&lexer, source);

    Token t1 = lexer_next_token(&lexer);
    assert(t1.type == TOKEN_KEYWORD_FN);
    assert(strcmp(t1.lexeme, "funcion") == 0);

    Token t2 = lexer_next_token(&lexer);
    assert(t2.type == TOKEN_KEYWORD_FN);
    assert(strcmp(t2.lexeme, "fn") == 0);

    Token t3 = lexer_next_token(&lexer);
    assert(t3.type == TOKEN_KEYWORD_LET);
    assert(strcmp(t3.lexeme, "sea") == 0);

    Token t4 = lexer_next_token(&lexer);
    assert(t4.type == TOKEN_KEYWORD_LET);
    assert(strcmp(t4.lexeme, "let") == 0);

    Token t5 = lexer_next_token(&lexer);
    assert(t5.type == TOKEN_KEYWORD_STRUCT);
    assert(strcmp(t5.lexeme, "estructura") == 0);

    Token t6 = lexer_next_token(&lexer);
    assert(t6.type == TOKEN_KEYWORD_STRUCT);
    assert(strcmp(t6.lexeme, "struct") == 0);

    Token t7 = lexer_next_token(&lexer);
    assert(t7.type == TOKEN_EOF);

    token_free(&t1); token_free(&t2); token_free(&t3);
    token_free(&t4); token_free(&t5); token_free(&t6);
    token_free(&t7);
    printf("[TEST] Verificacion bilingue unificada: OK\n");
}

void test_coordinates_and_comments(void) {
    const char* source = "// Comentario de linea\n  sea x = 42;";
    Lexer lexer;
    lexer_init(&lexer, source);

    Token t1 = lexer_next_token(&lexer);
    assert(t1.type == TOKEN_KEYWORD_LET);
    assert(t1.line == 2);
    assert(t1.column == 3); // salto 2 espacios

    Token t2 = lexer_next_token(&lexer);
    assert(t2.type == TOKEN_IDENTIFIER);
    assert(strcmp(t2.lexeme, "x") == 0);
    assert(t2.line == 2);
    assert(t2.column == 7);

    Token t3 = lexer_next_token(&lexer);
    assert(t3.type == TOKEN_OP_ASSIGN);

    Token t4 = lexer_next_token(&lexer);
    assert(t4.type == TOKEN_LITERAL_INT);
    assert(strcmp(t4.lexeme, "42") == 0);

    Token t5 = lexer_next_token(&lexer);
    assert(t5.type == TOKEN_DELIM_SEMICOLON);

    Token t6 = lexer_next_token(&lexer);
    assert(t6.type == TOKEN_EOF);

    token_free(&t1); token_free(&t2); token_free(&t3);
    token_free(&t4); token_free(&t5); token_free(&t6);
    printf("[TEST] Coordenadas de tracking de errores y comentarios: OK\n");
}

int main(void) {
    printf("=== INICIANDO SUITE DE PRUEBAS FORMALES DEL LEXER UPL+ ===\n");
    test_bilingual_keywords();
    test_coordinates_and_comments();
    printf("=== TODAS LAS PRUEBAS SE EJECUTARON CON EXITO (0 ERRORES) ===\n");
    return 0;
}
