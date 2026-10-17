#include "lexer.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

static void test_bilingual_keywords(void) {
    const char* source = "funcion fn sea let mutable mut estructura struct si if sino else mientras while retornar return comp3 asm ensamblador incrustar embed";
    Lexer lexer;
    lexer_init(&lexer, source);

    TokenType expected[] = {
        TOKEN_KEYWORD_FN,     TOKEN_KEYWORD_FN,
        TOKEN_KEYWORD_LET,    TOKEN_KEYWORD_LET,
        TOKEN_KEYWORD_MUT,    TOKEN_KEYWORD_MUT,
        TOKEN_KEYWORD_STRUCT, TOKEN_KEYWORD_STRUCT,
        TOKEN_KEYWORD_IF,     TOKEN_KEYWORD_IF,
        TOKEN_KEYWORD_ELSE,   TOKEN_KEYWORD_ELSE,
        TOKEN_KEYWORD_WHILE,  TOKEN_KEYWORD_WHILE,
        TOKEN_KEYWORD_RETURN, TOKEN_KEYWORD_RETURN,
        TOKEN_KEYWORD_COMP3,
        TOKEN_KEYWORD_ASM,    TOKEN_KEYWORD_ASM,
        TOKEN_KEYWORD_EMBED,  TOKEN_KEYWORD_EMBED,
        TOKEN_EOF
    };

    for (int i = 0; expected[i] != TOKEN_EOF; ++i) {
        Token tok = lexer_next_token(&lexer);
        if (tok.type != expected[i]) {
            fprintf(stderr, "[FAIL] Esperado %s, obtenido %s ('%s') en L%zu:C%zu\n",
                    token_type_name(expected[i]), token_type_name(tok.type), tok.lexeme, tok.line, tok.column);
        }
        assert(tok.type == expected[i]);
        token_free(&tok);
    }

    Token eof_tok = lexer_next_token(&lexer);
    assert(eof_tok.type == TOKEN_EOF);
    token_free(&eof_tok);

    printf("  [PASS] 1. Unificacion bilingue de palabras clave (EN/ES)\n");
}

static void test_tracking_lines_and_columns(void) {
    const char* source = "// Linea 1: Comentario\n"
                         "/* Bloque multilineal\n"
                         "   Linea 3 */\n"
                         "  sea saldo = 100;";
    Lexer lexer;
    lexer_init(&lexer, source);

    Token t1 = lexer_next_token(&lexer);
    assert(t1.type == TOKEN_KEYWORD_LET);
    assert(t1.line == 4);
    assert(t1.column == 3); // Tras 2 espacios en blanco
    assert(strcmp(t1.lexeme, "sea") == 0);
    token_free(&t1);

    Token t2 = lexer_next_token(&lexer);
    assert(t2.type == TOKEN_IDENTIFIER);
    assert(t2.line == 4);
    assert(t2.column == 7);
    assert(strcmp(t2.lexeme, "saldo") == 0);
    token_free(&t2);

    Token t3 = lexer_next_token(&lexer);
    assert(t3.type == TOKEN_OP_ASSIGN);
    token_free(&t3);

    Token t4 = lexer_next_token(&lexer);
    assert(t4.type == TOKEN_LITERAL_INT);
    assert(strcmp(t4.lexeme, "100") == 0);
    token_free(&t4);

    Token t5 = lexer_next_token(&lexer);
    assert(t5.type == TOKEN_DELIM_SEMICOLON);
    token_free(&t5);

    Token t6 = lexer_next_token(&lexer);
    assert(t6.type == TOKEN_EOF);
    token_free(&t6);

    printf("  [PASS] 2. Tracking de lineas, columnas y comentarios (linea y bloque)\n");
}

static void test_numeric_and_string_literals(void) {
    const char* source = "1048576 3.14159 0x90 0b1010 \"Hola \\\"Mundo\\\" con escape\\n\"";
    Lexer lexer;
    lexer_init(&lexer, source);

    Token t1 = lexer_next_token(&lexer);
    assert(t1.type == TOKEN_LITERAL_INT);
    assert(strcmp(t1.lexeme, "1048576") == 0);
    token_free(&t1);

    Token t2 = lexer_next_token(&lexer);
    assert(t2.type == TOKEN_LITERAL_FLOAT);
    assert(strcmp(t2.lexeme, "3.14159") == 0);
    token_free(&t2);

    Token t_hex = lexer_next_token(&lexer);
    assert(t_hex.type == TOKEN_LITERAL_INT);
    assert(strcmp(t_hex.lexeme, "0x90") == 0);
    token_free(&t_hex);

    Token t_bin = lexer_next_token(&lexer);
    assert(t_bin.type == TOKEN_LITERAL_INT);
    assert(strcmp(t_bin.lexeme, "0b1010") == 0);
    token_free(&t_bin);

    Token t3 = lexer_next_token(&lexer);
    assert(t3.type == TOKEN_LITERAL_STRING);
    assert(strcmp(t3.lexeme, "Hola \\\"Mundo\\\" con escape\\n") == 0);
    token_free(&t3);

    Token t4 = lexer_next_token(&lexer);
    assert(t4.type == TOKEN_EOF);
    token_free(&t4);

    printf("  [PASS] 3. Literales numericos (enteros, hex 0x, bin 0b, flotantes) y strings\n");
}

static void test_operators_and_delimiters(void) {
    const char* source = "== != -> && || <= >= ! ( ) { } [ ] ; , . : + - * / ^ @";
    Lexer lexer;
    lexer_init(&lexer, source);

    TokenType expected[] = {
        TOKEN_OP_EQUAL, TOKEN_OP_NOT_EQUAL, TOKEN_DELIM_ARROW,
        TOKEN_OP_AND, TOKEN_OP_OR, TOKEN_OP_LESS_EQUAL, TOKEN_OP_GREATER_EQUAL,
        TOKEN_OP_NOT,
        TOKEN_DELIM_LPAREN, TOKEN_DELIM_RPAREN,
        TOKEN_DELIM_LBRACE, TOKEN_DELIM_RBRACE,
        TOKEN_DELIM_LBRACKET, TOKEN_DELIM_RBRACKET,
        TOKEN_DELIM_SEMICOLON, TOKEN_DELIM_COMMA, TOKEN_DELIM_DOT, TOKEN_DELIM_COLON,
        TOKEN_OP_ADD, TOKEN_OP_SUB, TOKEN_OP_MUL, TOKEN_OP_DIV, TOKEN_OP_XOR, TOKEN_AT,
        TOKEN_EOF
    };

    for (int i = 0; expected[i] != TOKEN_EOF; ++i) {
        Token t = lexer_next_token(&lexer);
        if (t.type != expected[i]) {
            fprintf(stderr, "[FAIL] Esperado %s, obtenido %s ('%s') en L%zu:C%zu\n",
                    token_type_name(expected[i]), token_type_name(t.type), t.lexeme, t.line, t.column);
        }
        assert(t.type == expected[i]);
        token_free(&t);
    }

    Token eof_tok = lexer_next_token(&lexer);
    assert(eof_tok.type == TOKEN_EOF);
    token_free(&eof_tok);

    printf("  [PASS] 4. Operadores multicaracter y delimitadores EBNF (==, !=, <=, >=, ->, &&, ||, !, [, ], .)\n");
}

static void test_raw_blocks(void) {
    const char* source = "embed { printf(\"Raw C99 Block: %d\\n\", 42); } incrustar { uint8_t b = 0xFF; }";
    Lexer lexer;
    lexer_init(&lexer, source);

    // 1. Palabra clave 'embed'
    Token t1 = lexer_next_token(&lexer);
    assert(t1.type == TOKEN_KEYWORD_EMBED);
    token_free(&t1);

    // 2. Bloque crudo inline consumido por demanda contextual
    Token raw1 = read_raw_block(&lexer);
    assert(raw1.type == TOKEN_RAW_BLOCK);
    assert(strstr(raw1.lexeme, "printf(\"Raw C99 Block: %d\\n\", 42);") != NULL);
    token_free(&raw1);

    // 3. Palabra clave 'incrustar' (español)
    Token t2 = lexer_next_token(&lexer);
    assert(t2.type == TOKEN_KEYWORD_EMBED);
    token_free(&t2);

    // 4. Segundo bloque crudo
    Token raw2 = read_raw_block(&lexer);
    assert(raw2.type == TOKEN_RAW_BLOCK);
    assert(strstr(raw2.lexeme, "uint8_t b = 0xFF;") != NULL);
    token_free(&raw2);

    Token eof_tok = lexer_next_token(&lexer);
    assert(eof_tok.type == TOKEN_EOF);
    token_free(&eof_tok);

    printf("  [PASS] 5. Aislamiento y captura de bloques de codigo crudo (embed / incrustar)\n");
}

static void test_unclosed_string_error(void) {
    const char* source = "let x = \"cadena rota sin comilla final";
    Lexer lexer;
    lexer_init(&lexer, source);

    Token t1 = lexer_next_token(&lexer); assert(t1.type == TOKEN_KEYWORD_LET); token_free(&t1);
    Token t2 = lexer_next_token(&lexer); assert(t2.type == TOKEN_IDENTIFIER); token_free(&t2);
    Token t3 = lexer_next_token(&lexer); assert(t3.type == TOKEN_OP_ASSIGN); token_free(&t3);
    Token t4 = lexer_next_token(&lexer);
    assert(t4.type == TOKEN_ERROR); // Error léxico detectado
    token_free(&t4);

    printf("  [PASS] 6. Deteccion rigurosa de errores lexicos (string sin cerrar)\n");
}

int main(void) {
    printf("====================================================================\n");
    printf("  UPL+ SPEC SUITE: VERIFICACION FORMAL DEL ANALIZADOR LEXICO C99   \n");
    printf("====================================================================\n");

    test_bilingual_keywords();
    test_tracking_lines_and_columns();
    test_numeric_and_string_literals();
    test_operators_and_delimiters();
    test_raw_blocks();
    test_unclosed_string_error();

    printf("====================================================================\n");
    printf("  RESULTADO: 6/6 SUITES COMPLETADAS SATISFACTORIAMENTE (0 FALLOS)   \n");
    printf("====================================================================\n");
    return 0;
}
