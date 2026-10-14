// Content of nc_lexer.h (Copied for stability)
#ifndef NC_LEXER_H
#define NC_LEXER_H

#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#define NC_STRICMP _stricmp
#else
#define NC_STRICMP strcasecmp
#endif

typedef enum {
    TOK_PROGRAM, TOK_DATA, TOK_LOGIC, TOK_END, TOK_DEFINE, TOK_AS, TOK_SHOW,
    TOK_IF, TOK_THEN, TOK_ELSE, TOK_END_IF, TOK_WINDOW, TOK_BUTTON, TOK_FIELD,
    TOK_CAMPO, TOK_ON, TOK_CLICK, TOK_OPEN, TOK_READ, TOK_WRITE, TOK_CLOSE, TOK_FILE,
    TOK_RECORD, TOK_MODULE, TOK_BUS, TOK_SEND, TOK_RECEIVE, TOK_NITRO, TOK_NATIVO, TOK_FIN,
    TOK_LEFT, TOK_RIGHT, TOK_GLOBAL, TOK_IMPORT, TOK_COMPRESSED, TOK_DEFENSE,
    TOK_INTEGRITY, TOK_CRYPTO, TOK_AUTH, TOK_ERROR, TOK_IDENTIFIER,
    TOK_STRING, TOK_NUMBER, TOK_EQUALS, TOK_PLUS, TOK_MINUS, TOK_STAR,
    TOK_SLASH, TOK_COMMENT, TOK_DOT, TOK_EOF, TOK_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char text[256];
    int line;
} Token;

void nc_lexer_init(const char* source);
Token nc_lexer_next_token();
const char* nc_token_type_to_string(TokenType type);

#endif
