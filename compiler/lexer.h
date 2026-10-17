#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>
#include <stdbool.h>

typedef enum {
    /* Especiales */
    TOKEN_EOF,
    TOKEN_ERROR,
    TOKEN_IDENTIFIER,
    
    /* Literales */
    TOKEN_LITERAL_INT,
    TOKEN_LITERAL_FLOAT,
    TOKEN_LITERAL_STRING,
    TOKEN_RAW_BLOCK,

    /* Palabras Clave Unificadas (Bilingüe EN / ES) */
    TOKEN_KEYWORD_MODULE,   /* module / modulo */
    TOKEN_KEYWORD_IMPORT,   /* import / importar */
    TOKEN_KEYWORD_PUB,      /* pub / publico */
    TOKEN_KEYWORD_FN,       /* fn / funcion */
    TOKEN_KEYWORD_LET,      /* let / sea */
    TOKEN_KEYWORD_MUT,      /* mut / mutable */
    TOKEN_KEYWORD_CONST,    /* const / constante */
    TOKEN_KEYWORD_STRUCT,   /* struct / estructura */
    TOKEN_KEYWORD_IF,       /* if / si */
    TOKEN_KEYWORD_ELSE,     /* else / sino */
    TOKEN_KEYWORD_WHILE,    /* while / mientras */
    TOKEN_KEYWORD_FOR,      /* for / para */
    TOKEN_KEYWORD_RETURN,   /* return / retornar */
    TOKEN_KEYWORD_AS,       /* as / como */
    TOKEN_KEYWORD_COMP3,    /* comp3 (tipo decimal exacto) */
    TOKEN_KEYWORD_ASM,      /* asm / ensamblador */
    TOKEN_KEYWORD_EMBED,    /* embed / incrustar */

    /* Operadores */
    TOKEN_OP_ASSIGN,        /* = */
    TOKEN_OP_EQUAL,         /* == */
    TOKEN_OP_NOT_EQUAL,     /* != */
    TOKEN_OP_ADD,           /* + */
    TOKEN_OP_SUB,           /* - */
    TOKEN_OP_MUL,           /* * */
    TOKEN_OP_DIV,           /* / */
    TOKEN_OP_LESS,          /* < */
    TOKEN_OP_GREATER,       /* > */
    TOKEN_OP_LESS_EQUAL,    /* <= */
    TOKEN_OP_GREATER_EQUAL, /* >= */
    TOKEN_OP_AND,           /* && */
    TOKEN_OP_OR,            /* || */
    TOKEN_OP_NOT,           /* ! */
    TOKEN_OP_XOR,           /* ^ */

    /* Delimitadores */
    TOKEN_DELIM_LPAREN,     /* ( */
    TOKEN_DELIM_RPAREN,     /* ) */
    TOKEN_DELIM_LBRACE,     /* { */
    TOKEN_DELIM_RBRACE,     /* } */
    TOKEN_DELIM_LBRACKET,   /* [ */
    TOKEN_DELIM_RBRACKET,   /* ] */
    TOKEN_DELIM_SEMICOLON,  /* ; */
    TOKEN_DELIM_COMMA,      /* , */
    TOKEN_DELIM_COLON,      /* : */
    TOKEN_DELIM_DOT,        /* . */
    TOKEN_DELIM_ARROW       /* -> */
} TokenType;

typedef struct {
    TokenType type;
    char* lexeme;           /* Cadena asignada dinámicamente en Heap */
    size_t line;
    size_t column;
} Token;

typedef struct {
    const char* source;
    size_t cursor;
    size_t line;
    size_t column;
} Lexer;

/* Inicializar el estado del analizador léxico */
void lexer_init(Lexer* lexer, const char* source);

/* Obtener el siguiente token del flujo */
Token lexer_next_token(Lexer* lexer);

/* Leer un bloque crudo delimitado por { ... } para bloques inline (asm/embed) */
Token read_raw_block(Lexer* lexer);

/* Leer el cuerpo de un bloque crudo cuando '{' ya fue consumido por el parser */
Token read_raw_block_body(Lexer* lexer, size_t start_line, size_t start_col);

/* Liberar la memoria asignada para el lexema del token */
void token_free(Token* token);

/* Obtener representación en cadena del tipo de token (para diagnósticos) */
const char* token_type_name(TokenType type);

#endif /* LEXER_H */
