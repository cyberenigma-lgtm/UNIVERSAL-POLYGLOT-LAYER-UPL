#include "lexer.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void lexer_init(Lexer* lexer, const char* source) {
    lexer->source = source ? source : "";
    lexer->cursor = 0;
    lexer->line = 1;
    lexer->column = 1;
}

static char peek(Lexer* lexer) {
    return lexer->source[lexer->cursor];
}

static char peek_next(Lexer* lexer) {
    if (lexer->source[lexer->cursor] == '\0') return '\0';
    return lexer->source[lexer->cursor + 1];
}

static char advance(Lexer* lexer) {
    char c = lexer->source[lexer->cursor];
    if (c != '\0') {
        lexer->cursor++;
        if (c == '\n') {
            lexer->line++;
            lexer->column = 1;
        } else {
            lexer->column++;
        }
    }
    return c;
}

static void skip_whitespace_and_comments(Lexer* lexer) {
    while (peek(lexer) != '\0') {
        char c = peek(lexer);
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
            advance(lexer);
        } else if (c == '/' && peek_next(lexer) == '/') {
            // Comentario de línea única: // ...
            while (peek(lexer) != '\n' && peek(lexer) != '\0') {
                advance(lexer);
            }
        } else if (c == '/' && peek_next(lexer) == '*') {
            // Comentario multilínea: /* ... */
            advance(lexer); // Consumir '/'
            advance(lexer); // Consumir '*'
            while (peek(lexer) != '\0') {
                if (peek(lexer) == '*' && peek_next(lexer) == '/') {
                    advance(lexer); // Consumir '*'
                    advance(lexer); // Consumir '/'
                    break;
                }
                advance(lexer);
            }
        } else {
            break;
        }
    }
}

static Token create_token(TokenType type, const char* start, size_t length, size_t line, size_t col) {
    Token token;
    token.type = type;
    token.line = line;
    token.column = col;
    token.lexeme = (char*)malloc(length + 1);
    if (token.lexeme) {
        if (length > 0 && start) {
            memcpy(token.lexeme, start, length);
        }
        token.lexeme[length] = '\0';
    }
    return token;
}

static TokenType match_keyword(const char* lexeme) {
    // Modularidad y visibilidad
    if (strcmp(lexeme, "module") == 0 || strcmp(lexeme, "modulo") == 0) return TOKEN_KEYWORD_MODULE;
    if (strcmp(lexeme, "import") == 0 || strcmp(lexeme, "importar") == 0) return TOKEN_KEYWORD_IMPORT;
    if (strcmp(lexeme, "pub") == 0 || strcmp(lexeme, "publico") == 0) return TOKEN_KEYWORD_PUB;

    // Subrutinas y variables
    if (strcmp(lexeme, "fn") == 0 || strcmp(lexeme, "funcion") == 0) return TOKEN_KEYWORD_FN;
    if (strcmp(lexeme, "let") == 0 || strcmp(lexeme, "sea") == 0) return TOKEN_KEYWORD_LET;
    if (strcmp(lexeme, "mut") == 0 || strcmp(lexeme, "mutable") == 0) return TOKEN_KEYWORD_MUT;
    if (strcmp(lexeme, "const") == 0 || strcmp(lexeme, "constante") == 0) return TOKEN_KEYWORD_CONST;
    if (strcmp(lexeme, "struct") == 0 || strcmp(lexeme, "estructura") == 0) return TOKEN_KEYWORD_STRUCT;
    
    // Control de flujo
    if (strcmp(lexeme, "if") == 0 || strcmp(lexeme, "si") == 0) return TOKEN_KEYWORD_IF;
    if (strcmp(lexeme, "else") == 0 || strcmp(lexeme, "sino") == 0) return TOKEN_KEYWORD_ELSE;
    if (strcmp(lexeme, "while") == 0 || strcmp(lexeme, "mientras") == 0) return TOKEN_KEYWORD_WHILE;
    if (strcmp(lexeme, "for") == 0 || strcmp(lexeme, "para") == 0) return TOKEN_KEYWORD_FOR;
    if (strcmp(lexeme, "return") == 0 || strcmp(lexeme, "retornar") == 0) return TOKEN_KEYWORD_RETURN;
    if (strcmp(lexeme, "break") == 0 || strcmp(lexeme, "romper") == 0) return TOKEN_KEYWORD_BREAK;
    if (strcmp(lexeme, "continue") == 0 || strcmp(lexeme, "continuar") == 0) return TOKEN_KEYWORD_CONTINUE;
    
    // Conversión de tipos
    if (strcmp(lexeme, "as") == 0 || strcmp(lexeme, "como") == 0) return TOKEN_KEYWORD_AS;

    // Tipos nativos de sistemas
    if (strcmp(lexeme, "comp3") == 0) return TOKEN_KEYWORD_COMP3;
    
    // Bloques nativos e inline
    if (strcmp(lexeme, "asm") == 0 || strcmp(lexeme, "ensamblador") == 0) return TOKEN_KEYWORD_ASM;
    if (strcmp(lexeme, "embed") == 0 || strcmp(lexeme, "incrustar") == 0) return TOKEN_KEYWORD_EMBED;
    
    return TOKEN_IDENTIFIER;
}

static Token read_identifier_or_keyword(Lexer* lexer) {
    size_t start_cursor = lexer->cursor;
    size_t start_col = lexer->column;
    size_t start_line = lexer->line;

    while (isalnum((unsigned char)peek(lexer)) || peek(lexer) == '_') {
        advance(lexer);
    }

    size_t length = lexer->cursor - start_cursor;
    const char* start_ptr = &lexer->source[start_cursor];
    
    char* temp = (char*)malloc(length + 1);
    if (!temp) {
        return create_token(TOKEN_ERROR, "Error de asignacion de memoria", 30, start_line, start_col);
    }
    memcpy(temp, start_ptr, length);
    temp[length] = '\0';
    
    TokenType type = match_keyword(temp);
    free(temp);

    return create_token(type, start_ptr, length, start_line, start_col);
}

static Token read_number(Lexer* lexer) {
    size_t start_cursor = lexer->cursor;
    size_t start_col = lexer->column;
    size_t start_line = lexer->line;
    bool is_float = false;

    // Soporte para literales hexadecimales: 0x... / 0X...
    if (peek(lexer) == '0' && (peek_next(lexer) == 'x' || peek_next(lexer) == 'X')) {
        advance(lexer); // '0'
        advance(lexer); // 'x' o 'X'
        while (isxdigit((unsigned char)peek(lexer))) {
            advance(lexer);
        }
        size_t length = lexer->cursor - start_cursor;
        return create_token(TOKEN_LITERAL_INT, &lexer->source[start_cursor], length, start_line, start_col);
    }

    // Soporte para literales binarios: 0b... / 0B...
    if (peek(lexer) == '0' && (peek_next(lexer) == 'b' || peek_next(lexer) == 'B')) {
        advance(lexer); // '0'
        advance(lexer); // 'b' o 'B'
        while (peek(lexer) == '0' || peek(lexer) == '1') {
            advance(lexer);
        }
        size_t length = lexer->cursor - start_cursor;
        return create_token(TOKEN_LITERAL_INT, &lexer->source[start_cursor], length, start_line, start_col);
    }

    while (isdigit((unsigned char)peek(lexer))) {
        advance(lexer);
    }

    if (peek(lexer) == '.' && isdigit((unsigned char)peek_next(lexer))) {
        is_float = true;
        advance(lexer); // Consumir '.'
        while (isdigit((unsigned char)peek(lexer))) {
            advance(lexer);
        }
    }

    size_t length = lexer->cursor - start_cursor;
    return create_token(is_float ? TOKEN_LITERAL_FLOAT : TOKEN_LITERAL_INT, 
                        &lexer->source[start_cursor], length, start_line, start_col);
}

static Token read_string(Lexer* lexer) {
    size_t start_col = lexer->column;
    size_t start_line = lexer->line;
    advance(lexer); // Consumir '"' inicial
    size_t start_cursor = lexer->cursor;

    while (peek(lexer) != '"' && peek(lexer) != '\0') {
        if (peek(lexer) == '\\' && peek_next(lexer) != '\0') {
            advance(lexer); // Consumir caracter de escape '\\'
        }
        advance(lexer);
    }

    if (peek(lexer) == '\0') {
        return create_token(TOKEN_ERROR, "String literal no cerrado antes de EOF", 38, start_line, start_col);
    }

    size_t length = lexer->cursor - start_cursor;
    const char* start_ptr = &lexer->source[start_cursor];
    advance(lexer); // Consumir '"' final

    return create_token(TOKEN_LITERAL_STRING, start_ptr, length, start_line, start_col);
}

Token read_raw_block_body(Lexer* lexer, size_t start_line, size_t start_col) {
    size_t start_cursor = lexer->cursor;
    int brace_count = 1;

    while (brace_count > 0 && peek(lexer) != '\0') {
        char c = peek(lexer);
        if (c == '"' || c == '\'') {
            char quote = advance(lexer);
            while (peek(lexer) != quote && peek(lexer) != '\0') {
                if (peek(lexer) == '\\' && peek_next(lexer) != '\0') advance(lexer);
                advance(lexer);
            }
            if (peek(lexer) != '\0') advance(lexer);
            continue;
        }
        if (c == '{') brace_count++;
        if (c == '}') {
            brace_count--;
            if (brace_count == 0) {
                size_t length = lexer->cursor - start_cursor;
                advance(lexer); // Consumir '}' final
                return create_token(TOKEN_RAW_BLOCK, &lexer->source[start_cursor], length, start_line, start_col);
            }
        }
        advance(lexer);
    }

    return create_token(TOKEN_ERROR, "Bloque raw no cerrado antes de EOF", 34, start_line, start_col);
}

Token read_raw_block(Lexer* lexer) {
    skip_whitespace_and_comments(lexer);
    if (peek(lexer) != '{') {
        return create_token(TOKEN_ERROR, "Se esperaba '{' para iniciar bloque raw", 39, lexer->line, lexer->column);
    }

    size_t start_col = lexer->column;
    size_t start_line = lexer->line;
    advance(lexer); // Consumir '{'
    return read_raw_block_body(lexer, start_line, start_col);
}

Token lexer_next_token(Lexer* lexer) {
    skip_whitespace_and_comments(lexer);

    size_t start_col = lexer->column;
    size_t start_line = lexer->line;
    char c = peek(lexer);

    if (c == '\0') {
        return create_token(TOKEN_EOF, "", 0, start_line, start_col);
    }

    if (isalpha((unsigned char)c) || c == '_') {
        return read_identifier_or_keyword(lexer);
    }

    if (isdigit((unsigned char)c)) {
        return read_number(lexer);
    }

    if (c == '"') {
        return read_string(lexer);
    }

    advance(lexer); // Avanzar cursor para operadores y delimitadores

    // Operadores de dos caracteres y relacionales
    if (c == '=') {
        if (peek(lexer) == '=') { advance(lexer); return create_token(TOKEN_OP_EQUAL, "==", 2, start_line, start_col); }
        return create_token(TOKEN_OP_ASSIGN, "=", 1, start_line, start_col);
    }
    if (c == '!') {
        if (peek(lexer) == '=') { advance(lexer); return create_token(TOKEN_OP_NOT_EQUAL, "!=", 2, start_line, start_col); }
        return create_token(TOKEN_OP_NOT, "!", 1, start_line, start_col);
    }
    if (c == '<') {
        if (peek(lexer) == '=') { advance(lexer); return create_token(TOKEN_OP_LESS_EQUAL, "<=", 2, start_line, start_col); }
        return create_token(TOKEN_OP_LESS, "<", 1, start_line, start_col);
    }
    if (c == '>') {
        if (peek(lexer) == '=') { advance(lexer); return create_token(TOKEN_OP_GREATER_EQUAL, ">=", 2, start_line, start_col); }
        return create_token(TOKEN_OP_GREATER, ">", 1, start_line, start_col);
    }
    if (c == '-') {
        if (peek(lexer) == '>') { advance(lexer); return create_token(TOKEN_DELIM_ARROW, "->", 2, start_line, start_col); }
        return create_token(TOKEN_OP_SUB, "-", 1, start_line, start_col);
    }
    if (c == '&') {
        if (peek(lexer) == '&') { advance(lexer); return create_token(TOKEN_OP_AND, "&&", 2, start_line, start_col); }
        return create_token(TOKEN_OP_BIT_AND, "&", 1, start_line, start_col);
    }
    if (c == '|') {
        if (peek(lexer) == '|') { advance(lexer); return create_token(TOKEN_OP_OR, "||", 2, start_line, start_col); }
        return create_token(TOKEN_OP_BIT_OR, "|", 1, start_line, start_col);
    }

    // Delimitadores y operadores de un solo caracter
    switch (c) {
        case '+': return create_token(TOKEN_OP_ADD, "+", 1, start_line, start_col);
        case '*': return create_token(TOKEN_OP_MUL, "*", 1, start_line, start_col);
        case '/': return create_token(TOKEN_OP_DIV, "/", 1, start_line, start_col);
        case '^': return create_token(TOKEN_OP_XOR, "^", 1, start_line, start_col);
        case '(': return create_token(TOKEN_DELIM_LPAREN, "(", 1, start_line, start_col);
        case ')': return create_token(TOKEN_DELIM_RPAREN, ")", 1, start_line, start_col);
        case '{': return create_token(TOKEN_DELIM_LBRACE, "{", 1, start_line, start_col);
        case '}': return create_token(TOKEN_DELIM_RBRACE, "}", 1, start_line, start_col);
        case '[': return create_token(TOKEN_DELIM_LBRACKET, "[", 1, start_line, start_col);
        case ']': return create_token(TOKEN_DELIM_RBRACKET, "]", 1, start_line, start_col);
        case ';': return create_token(TOKEN_DELIM_SEMICOLON, ";", 1, start_line, start_col);
        case ',': return create_token(TOKEN_DELIM_COMMA, ",", 1, start_line, start_col);
        case ':': return create_token(TOKEN_DELIM_COLON, ":", 1, start_line, start_col);
        case '.': return create_token(TOKEN_DELIM_DOT, ".", 1, start_line, start_col);
        case '@': return create_token(TOKEN_AT, "@", 1, start_line, start_col);
    }

    return create_token(TOKEN_ERROR, &lexer->source[lexer->cursor - 1], 1, start_line, start_col);
}

void token_free(Token* token) {
    if (token && token->lexeme) {
        free(token->lexeme);
        token->lexeme = NULL;
    }
}

const char* token_type_name(TokenType type) {
    switch (type) {
        case TOKEN_EOF:            return "TOKEN_EOF";
        case TOKEN_ERROR:          return "TOKEN_ERROR";
        case TOKEN_IDENTIFIER:     return "TOKEN_IDENTIFIER";
        case TOKEN_LITERAL_INT:    return "TOKEN_LITERAL_INT";
        case TOKEN_LITERAL_FLOAT:  return "TOKEN_LITERAL_FLOAT";
        case TOKEN_LITERAL_STRING: return "TOKEN_LITERAL_STRING";
        case TOKEN_RAW_BLOCK:      return "TOKEN_RAW_BLOCK";
        case TOKEN_KEYWORD_MODULE: return "TOKEN_KEYWORD_MODULE";
        case TOKEN_KEYWORD_IMPORT: return "TOKEN_KEYWORD_IMPORT";
        case TOKEN_KEYWORD_PUB:    return "TOKEN_KEYWORD_PUB";
        case TOKEN_KEYWORD_FN:     return "TOKEN_KEYWORD_FN";
        case TOKEN_KEYWORD_LET:    return "TOKEN_KEYWORD_LET";
        case TOKEN_KEYWORD_MUT:    return "TOKEN_KEYWORD_MUT";
        case TOKEN_KEYWORD_CONST:  return "TOKEN_KEYWORD_CONST";
        case TOKEN_KEYWORD_STRUCT: return "TOKEN_KEYWORD_STRUCT";
        case TOKEN_KEYWORD_IF:     return "TOKEN_KEYWORD_IF";
        case TOKEN_KEYWORD_ELSE:   return "TOKEN_KEYWORD_ELSE";
        case TOKEN_KEYWORD_WHILE:  return "TOKEN_KEYWORD_WHILE";
        case TOKEN_KEYWORD_FOR:    return "TOKEN_KEYWORD_FOR";
        case TOKEN_KEYWORD_RETURN: return "TOKEN_KEYWORD_RETURN";
        case TOKEN_KEYWORD_AS:     return "TOKEN_KEYWORD_AS";
        case TOKEN_KEYWORD_COMP3:  return "TOKEN_KEYWORD_COMP3";
        case TOKEN_KEYWORD_ASM:    return "TOKEN_KEYWORD_ASM";
        case TOKEN_KEYWORD_EMBED:  return "TOKEN_KEYWORD_EMBED";
        case TOKEN_KEYWORD_BREAK:  return "TOKEN_KEYWORD_BREAK";
        case TOKEN_KEYWORD_CONTINUE: return "TOKEN_KEYWORD_CONTINUE";
        case TOKEN_OP_ASSIGN:      return "TOKEN_OP_ASSIGN";
        case TOKEN_OP_EQUAL:       return "TOKEN_OP_EQUAL";
        case TOKEN_OP_NOT_EQUAL:   return "TOKEN_OP_NOT_EQUAL";
        case TOKEN_OP_ADD:         return "TOKEN_OP_ADD";
        case TOKEN_OP_SUB:         return "TOKEN_OP_SUB";
        case TOKEN_OP_MUL:         return "TOKEN_OP_MUL";
        case TOKEN_OP_DIV:         return "TOKEN_OP_DIV";
        case TOKEN_OP_LESS:        return "TOKEN_OP_LESS";
        case TOKEN_OP_GREATER:     return "TOKEN_OP_GREATER";
        case TOKEN_OP_LESS_EQUAL:  return "TOKEN_OP_LESS_EQUAL";
        case TOKEN_OP_GREATER_EQUAL:return "TOKEN_OP_GREATER_EQUAL";
        case TOKEN_OP_AND:         return "TOKEN_OP_AND";
        case TOKEN_OP_OR:          return "TOKEN_OP_OR";
        case TOKEN_OP_NOT:         return "TOKEN_OP_NOT";
        case TOKEN_OP_XOR:         return "TOKEN_OP_XOR";
        case TOKEN_OP_BIT_AND:     return "TOKEN_OP_BIT_AND";
        case TOKEN_OP_BIT_OR:      return "TOKEN_OP_BIT_OR";
        case TOKEN_DELIM_LPAREN:   return "TOKEN_DELIM_LPAREN";
        case TOKEN_DELIM_RPAREN:   return "TOKEN_DELIM_RPAREN";
        case TOKEN_DELIM_LBRACE:   return "TOKEN_DELIM_LBRACE";
        case TOKEN_DELIM_RBRACE:   return "TOKEN_DELIM_RBRACE";
        case TOKEN_DELIM_LBRACKET: return "TOKEN_DELIM_LBRACKET";
        case TOKEN_DELIM_RBRACKET: return "TOKEN_DELIM_RBRACKET";
        case TOKEN_DELIM_SEMICOLON:return "TOKEN_DELIM_SEMICOLON";
        case TOKEN_DELIM_COMMA:    return "TOKEN_DELIM_COMMA";
        case TOKEN_DELIM_COLON:    return "TOKEN_DELIM_COLON";
        case TOKEN_DELIM_DOT:      return "TOKEN_DELIM_DOT";
        case TOKEN_DELIM_ARROW:    return "TOKEN_DELIM_ARROW";
        case TOKEN_AT:             return "TOKEN_AT";
        default:                   return "UNKNOWN";
    }
}
