/* ============================================================================ */
/* c2upl: C99 to UPL+ Transpiler & Migration Tool                               */
/* Language: ANSI C99 / Standalone Systems Utility                              */
/* Author: José Manuel Moreno (Noxferion)                                       */
/* License: SafeCreative Protected / Defensive                                  */
/* ============================================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdarg.h>

#define MAX_TOKEN_LEN 512
#define MAX_PARAMS 32

/* Idioma objetivo para el código UPL+ emitido */
typedef enum {
    LANG_EN,
    LANG_ES
} TargetLang;

/* Tokens de C99 */
typedef enum {
    CTOK_EOF,
    CTOK_IDENT,
    CTOK_INT_LIT,
    CTOK_FLOAT_LIT,
    CTOK_STRING_LIT,
    CTOK_CHAR_LIT,
    
    /* Palabras Clave de C */
    CTOK_TYPEDEF,
    CTOK_STRUCT,
    CTOK_CONST,
    CTOK_STATIC,
    CTOK_INLINE,
    CTOK_EXTERN,
    CTOK_VOLATILE,
    CTOK_VOID,
    CTOK_CHAR,
    CTOK_SHORT,
    CTOK_INT,
    CTOK_LONG,
    CTOK_FLOAT,
    CTOK_DOUBLE,
    CTOK_SIGNED,
    CTOK_UNSIGNED,
    CTOK_BOOL,
    CTOK_IF,
    CTOK_ELSE,
    CTOK_WHILE,
    CTOK_FOR,
    CTOK_RETURN,

    /* Operadores y Delimitadores */
    CTOK_LPAREN,    /* ( */
    CTOK_RPAREN,    /* ) */
    CTOK_LBRACE,    /* { */
    CTOK_RBRACE,    /* } */
    CTOK_LBRACKET,  /* [ */
    CTOK_RBRACKET,  /* ] */
    CTOK_SEMICOLON, /* ; */
    CTOK_COMMA,     /* , */
    CTOK_DOT,       /* . */
    CTOK_ARROW,     /* -> */
    CTOK_STAR,      /* * */
    CTOK_ASSIGN,    /* = */
    CTOK_EQUAL,     /* == */
    CTOK_NOTEQUAL,  /* != */
    CTOK_LESS,      /* < */
    CTOK_GREATER,   /* > */
    CTOK_LESSEQ,    /* <= */
    CTOK_GREATEREQ, /* >= */
    CTOK_PLUS,      /* + */
    CTOK_MINUS,     /* - */
    CTOK_SLASH,     /* / */
    CTOK_PERCENT,   /* % */
    CTOK_AND,       /* && */
    CTOK_OR,        /* || */
    CTOK_NOT,       /* ! */
    CTOK_AMP,       /* & */
    CTOK_PIPE,      /* | */
    CTOK_CARET,     /* ^ */
    CTOK_PREPROC    /* #... */
} CTokenType;

typedef struct {
    CTokenType type;
    char text[MAX_TOKEN_LEN];
    size_t line;
    size_t col;
} CToken;

typedef struct {
    const char *source;
    size_t cursor;
    size_t line;
    size_t col;
} CLexer;

static void clexer_init(CLexer *lex, const char *source) {
    lex->source = source;
    lex->cursor = 0;
    lex->line = 1;
    lex->col = 1;
}

static char c_peek(const CLexer *lex) {
    return lex->source[lex->cursor];
}

static char c_peek_next(const CLexer *lex) {
    if (lex->source[lex->cursor] == '\0') return '\0';
    return lex->source[lex->cursor + 1];
}

static char c_advance(CLexer *lex) {
    char ch = lex->source[lex->cursor++];
    if (ch == '\n') {
        lex->line++;
        lex->col = 1;
    } else {
        lex->col++;
    }
    return ch;
}

static void skip_whitespace_and_comments(CLexer *lex) {
    for (;;) {
        char ch = c_peek(lex);
        if (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n') {
            c_advance(lex);
        } else if (ch == '/' && c_peek_next(lex) == '/') {
            while (c_peek(lex) != '\n' && c_peek(lex) != '\0') {
                c_advance(lex);
            }
        } else if (ch == '/' && c_peek_next(lex) == '*') {
            c_advance(lex); // /
            c_advance(lex); // *
            while (!(c_peek(lex) == '*' && c_peek_next(lex) == '/') && c_peek(lex) != '\0') {
                c_advance(lex);
            }
            if (c_peek(lex) == '*') {
                c_advance(lex);
                c_advance(lex);
            }
        } else {
            break;
        }
    }
}

static CToken clexer_next_token(CLexer *lex) {
    skip_whitespace_and_comments(lex);

    CToken tok;
    tok.line = lex->line;
    tok.col = lex->col;
    tok.text[0] = '\0';

    if (c_peek(lex) == '\0') {
        tok.type = CTOK_EOF;
        return tok;
    }

    /* Directivas de preprocesador (#include, #define) */
    if (c_peek(lex) == '#') {
        size_t idx = 0;
        while (c_peek(lex) != '\n' && c_peek(lex) != '\0' && idx < MAX_TOKEN_LEN - 1) {
            tok.text[idx++] = c_advance(lex);
        }
        tok.text[idx] = '\0';
        tok.type = CTOK_PREPROC;
        return tok;
    }

    char ch = c_advance(lex);

    /* Identificadores y palabras clave */
    if (isalpha((unsigned char)ch) || ch == '_') {
        size_t idx = 0;
        tok.text[idx++] = ch;
        while ((isalnum((unsigned char)c_peek(lex)) || c_peek(lex) == '_') && idx < MAX_TOKEN_LEN - 1) {
            tok.text[idx++] = c_advance(lex);
        }
        tok.text[idx] = '\0';

        if (strcmp(tok.text, "typedef") == 0) tok.type = CTOK_TYPEDEF;
        else if (strcmp(tok.text, "struct") == 0) tok.type = CTOK_STRUCT;
        else if (strcmp(tok.text, "const") == 0) tok.type = CTOK_CONST;
        else if (strcmp(tok.text, "static") == 0) tok.type = CTOK_STATIC;
        else if (strcmp(tok.text, "inline") == 0) tok.type = CTOK_INLINE;
        else if (strcmp(tok.text, "extern") == 0) tok.type = CTOK_EXTERN;
        else if (strcmp(tok.text, "volatile") == 0) tok.type = CTOK_VOLATILE;
        else if (strcmp(tok.text, "void") == 0) tok.type = CTOK_VOID;
        else if (strcmp(tok.text, "char") == 0) tok.type = CTOK_CHAR;
        else if (strcmp(tok.text, "short") == 0) tok.type = CTOK_SHORT;
        else if (strcmp(tok.text, "int") == 0) tok.type = CTOK_INT;
        else if (strcmp(tok.text, "long") == 0) tok.type = CTOK_LONG;
        else if (strcmp(tok.text, "float") == 0) tok.type = CTOK_FLOAT;
        else if (strcmp(tok.text, "double") == 0) tok.type = CTOK_DOUBLE;
        else if (strcmp(tok.text, "signed") == 0) tok.type = CTOK_SIGNED;
        else if (strcmp(tok.text, "unsigned") == 0) tok.type = CTOK_UNSIGNED;
        else if (strcmp(tok.text, "bool") == 0 || strcmp(tok.text, "_Bool") == 0) tok.type = CTOK_BOOL;
        else if (strcmp(tok.text, "if") == 0) tok.type = CTOK_IF;
        else if (strcmp(tok.text, "else") == 0) tok.type = CTOK_ELSE;
        else if (strcmp(tok.text, "while") == 0) tok.type = CTOK_WHILE;
        else if (strcmp(tok.text, "for") == 0) tok.type = CTOK_FOR;
        else if (strcmp(tok.text, "return") == 0) tok.type = CTOK_RETURN;
        else tok.type = CTOK_IDENT;

        return tok;
    }

    /* Literales numéricos (enteros, hex, flotantes) */
    if (isdigit((unsigned char)ch)) {
        size_t idx = 0;
        tok.text[idx++] = ch;
        bool is_float = false;

        if (ch == '0' && (c_peek(lex) == 'x' || c_peek(lex) == 'X')) {
            tok.text[idx++] = c_advance(lex);
            while (isxdigit((unsigned char)c_peek(lex)) && idx < MAX_TOKEN_LEN - 1) {
                tok.text[idx++] = c_advance(lex);
            }
        } else {
            while ((isdigit((unsigned char)c_peek(lex)) || c_peek(lex) == '.') && idx < MAX_TOKEN_LEN - 1) {
                if (c_peek(lex) == '.') is_float = true;
                tok.text[idx++] = c_advance(lex);
            }
        }
        tok.text[idx] = '\0';
        tok.type = is_float ? CTOK_FLOAT_LIT : CTOK_INT_LIT;
        return tok;
    }

    /* Literales de cadena */
    if (ch == '"') {
        size_t idx = 0;
        tok.text[idx++] = '"';
        while (c_peek(lex) != '"' && c_peek(lex) != '\0' && idx < MAX_TOKEN_LEN - 2) {
            if (c_peek(lex) == '\\') {
                tok.text[idx++] = c_advance(lex);
            }
            tok.text[idx++] = c_advance(lex);
        }
        if (c_peek(lex) == '"') {
            tok.text[idx++] = c_advance(lex);
        }
        tok.text[idx] = '\0';
        tok.type = CTOK_STRING_LIT;
        return tok;
    }

    /* Operadores compuestos */
    if (ch == '=' && c_peek(lex) == '=') { c_advance(lex); strcpy(tok.text, "=="); tok.type = CTOK_EQUAL; return tok; }
    if (ch == '!' && c_peek(lex) == '=') { c_advance(lex); strcpy(tok.text, "!="); tok.type = CTOK_NOTEQUAL; return tok; }
    if (ch == '<' && c_peek(lex) == '=') { c_advance(lex); strcpy(tok.text, "<="); tok.type = CTOK_LESSEQ; return tok; }
    if (ch == '>' && c_peek(lex) == '=') { c_advance(lex); strcpy(tok.text, ">="); tok.type = CTOK_GREATEREQ; return tok; }
    if (ch == '-' && c_peek(lex) == '>') { c_advance(lex); strcpy(tok.text, "->"); tok.type = CTOK_ARROW; return tok; }
    if (ch == '&' && c_peek(lex) == '&') { c_advance(lex); strcpy(tok.text, "&&"); tok.type = CTOK_AND; return tok; }
    if (ch == '|' && c_peek(lex) == '|') { c_advance(lex); strcpy(tok.text, "||"); tok.type = CTOK_OR; return tok; }

    /* Delimitadores simples */
    tok.text[0] = ch;
    tok.text[1] = '\0';
    switch (ch) {
        case '(': tok.type = CTOK_LPAREN; break;
        case ')': tok.type = CTOK_RPAREN; break;
        case '{': tok.type = CTOK_LBRACE; break;
        case '}': tok.type = CTOK_RBRACE; break;
        case '[': tok.type = CTOK_LBRACKET; break;
        case ']': tok.type = CTOK_RBRACKET; break;
        case ';': tok.type = CTOK_SEMICOLON; break;
        case ',': tok.type = CTOK_COMMA; break;
        case '.': tok.type = CTOK_DOT; break;
        case '*': tok.type = CTOK_STAR; break;
        case '=': tok.type = CTOK_ASSIGN; break;
        case '<': tok.type = CTOK_LESS; break;
        case '>': tok.type = CTOK_GREATER; break;
        case '+': tok.type = CTOK_PLUS; break;
        case '-': tok.type = CTOK_MINUS; break;
        case '/': tok.type = CTOK_SLASH; break;
        case '%': tok.type = CTOK_PERCENT; break;
        case '!': tok.type = CTOK_NOT; break;
        case '&': tok.type = CTOK_AMP; break;
        case '|': tok.type = CTOK_PIPE; break;
        case '^': tok.type = CTOK_CARET; break;
        default:  tok.type = CTOK_IDENT; break;
    }
    return tok;
}

/* ------------------------------------------------------------- */
/*             MAPEO DE TIPOS C99 ➔ UPL+                         */
/* ------------------------------------------------------------- */

static void map_c_type_to_upl(const char *c_type, char *out_upl_type, size_t out_size) {
    bool is_ptr = false;
    char clean[MAX_TOKEN_LEN];
    strncpy(clean, c_type, sizeof(clean) - 1);
    clean[sizeof(clean) - 1] = '\0';

    if (strchr(clean, '*')) {
        is_ptr = true;
        char *p = strchr(clean, '*');
        *p = '\0';
    }

    // Recortar espacios
    char *start = clean;
    while (*start == ' ') start++;
    char *end = start + strlen(start) - 1;
    while (end > start && *end == ' ') { *end = '\0'; end--; }

    const char *mapped = start;

    if (strcmp(start, "int") == 0 || strcmp(start, "int32_t") == 0 || strcmp(start, "signed int") == 0) mapped = "i32";
    else if (strcmp(start, "unsigned int") == 0 || strcmp(start, "uint32_t") == 0 || strcmp(start, "unsigned") == 0) mapped = "u32";
    else if (strcmp(start, "short") == 0 || strcmp(start, "int16_t") == 0) mapped = "i16";
    else if (strcmp(start, "unsigned short") == 0 || strcmp(start, "uint16_t") == 0) mapped = "u16";
    else if (strcmp(start, "char") == 0 || strcmp(start, "int8_t") == 0) mapped = "i8";
    else if (strcmp(start, "unsigned char") == 0 || strcmp(start, "uint8_t") == 0) mapped = "u8";
    else if (strcmp(start, "long long") == 0 || strcmp(start, "int64_t") == 0 || strcmp(start, "long") == 0) mapped = "i64";
    else if (strcmp(start, "unsigned long long") == 0 || strcmp(start, "uint64_t") == 0 || strcmp(start, "unsigned long") == 0 || strcmp(start, "size_t") == 0) mapped = "u64";
    else if (strcmp(start, "float") == 0) mapped = "f32";
    else if (strcmp(start, "double") == 0) mapped = "f64";
    else if (strcmp(start, "bool") == 0 || strcmp(start, "_Bool") == 0) mapped = "bool";
    else if (strcmp(start, "void") == 0) mapped = "void";

    if (is_ptr) {
        snprintf(out_upl_type, out_size, "*%s", mapped);
    } else {
        snprintf(out_upl_type, out_size, "%s", mapped);
    }
}

/* ------------------------------------------------------------- */
/*             PARSER Y TRANSPILADOR C99 ➔ UPL+                  */
/* ------------------------------------------------------------- */

typedef struct {
    CLexer lexer;
    CToken current;
    TargetLang lang;
    FILE *out;
    char buffer[65536];
    size_t buf_pos;
} C2UplTranspiler;

static void ctrans_emit(C2UplTranspiler *tr, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    if (tr->out) {
        vfprintf(tr->out, fmt, args);
    } else {
        char temp[1024];
        vsnprintf(temp, sizeof(temp), fmt, args);
        size_t len = strlen(temp);
        if (tr->buf_pos + len < sizeof(tr->buffer) - 1) {
            strcpy(&tr->buffer[tr->buf_pos], temp);
            tr->buf_pos += len;
        }
    }
    va_end(args);
}

static void ctrans_advance(C2UplTranspiler *tr) {
    tr->current = clexer_next_token(&tr->lexer);
}

static bool ctrans_match(C2UplTranspiler *tr, CTokenType type) {
    if (tr->current.type == type) {
        ctrans_advance(tr);
        return true;
    }
    return false;
}

static void safe_strcpy(char *dst, const char *src, size_t dst_size) {
    if (!dst || dst_size == 0) return;
    size_t i = 0;
    while (src && src[i] && i + 1 < dst_size) {
        dst[i] = src[i];
        i++;
    }
    dst[i] = '\0';
}

static const char* kw_fn(TargetLang l)     { return (l == LANG_ES) ? "funcion" : "fn"; }
static const char* kw_let(TargetLang l)    { return (l == LANG_ES) ? "sea" : "let"; }
static const char* kw_mut(TargetLang l)    { (void)l; return "mut"; }
static const char* kw_return(TargetLang l) { return (l == LANG_ES) ? "retornar" : "return"; }
static const char* kw_if(TargetLang l)     { return (l == LANG_ES) ? "si" : "if"; }
static const char* kw_else(TargetLang l)   { return (l == LANG_ES) ? "sino" : "else"; }
static const char* kw_while(TargetLang l)  { return (l == LANG_ES) ? "mientras" : "while"; }
static const char* kw_struct(TargetLang l) { return (l == LANG_ES) ? "estructura" : "struct"; }

/* Determinar si el token actual inicia una especificación de tipo de C */
static bool is_type_start(CTokenType t) {
    return t == CTOK_INT || t == CTOK_CHAR || t == CTOK_SHORT || t == CTOK_LONG ||
           t == CTOK_FLOAT || t == CTOK_DOUBLE || t == CTOK_VOID || t == CTOK_BOOL ||
           t == CTOK_UNSIGNED || t == CTOK_SIGNED || t == CTOK_CONST || t == CTOK_STATIC ||
           t == CTOK_INLINE || t == CTOK_EXTERN || t == CTOK_VOLATILE || t == CTOK_STRUCT;
}

static CToken ctrans_peek_token(C2UplTranspiler *tr) {
    CLexer saved = tr->lexer;
    return clexer_next_token(&saved);
}

static bool is_c_type_start(C2UplTranspiler *tr) {
    if (is_type_start(tr->current.type)) return true;
    if (tr->current.type == CTOK_IDENT) {
        CToken next = ctrans_peek_token(tr);
        if (next.type == CTOK_IDENT || next.type == CTOK_STAR) return true;
    }
    return false;
}

/* Leer un tipo C completo incluyendo modificadores (unsigned long, int*, struct X*, etc.) */
static void parse_c_type(C2UplTranspiler *tr, char *out_type, size_t max_len) {
    out_type[0] = '\0';
    bool in_struct = false;

    for (;;) {
        if (tr->current.type == CTOK_STRUCT) {
            in_struct = true;
            ctrans_advance(tr);
            continue;
        }

        if (tr->current.type == CTOK_CONST || tr->current.type == CTOK_STATIC ||
            tr->current.type == CTOK_INLINE || tr->current.type == CTOK_EXTERN ||
            tr->current.type == CTOK_VOLATILE) {
            ctrans_advance(tr);
            continue;
        }

        if (is_type_start(tr->current.type)) {
            if (out_type[0] != '\0') {
                strncat(out_type, " ", max_len - strlen(out_type) - 1);
            }
            strncat(out_type, tr->current.text, max_len - strlen(out_type) - 1);
            ctrans_advance(tr);

            while (tr->current.type == CTOK_STAR) {
                strncat(out_type, "*", max_len - strlen(out_type) - 1);
                ctrans_advance(tr);
            }
            continue;
        }

        // Si es un identificador, comprobar con lookahead si representa un tipo
        if (tr->current.type == CTOK_IDENT) {
            CToken next = ctrans_peek_token(tr);
            if (in_struct || next.type == CTOK_IDENT || next.type == CTOK_STAR) {
                if (out_type[0] != '\0') {
                    strncat(out_type, " ", max_len - strlen(out_type) - 1);
                }
                strncat(out_type, tr->current.text, max_len - strlen(out_type) - 1);
                ctrans_advance(tr);
                in_struct = false;

                while (tr->current.type == CTOK_STAR) {
                    strncat(out_type, "*", max_len - strlen(out_type) - 1);
                    ctrans_advance(tr);
                }
                continue;
            }
        }

        break;
    }
}

/* Transpilar una declaración de estructura en C: typedef struct { ... } Nombre; */
static void transpile_struct(C2UplTranspiler *tr) {
    bool is_typedef = false;
    if (tr->current.type == CTOK_TYPEDEF) {
        is_typedef = true;
        ctrans_advance(tr);
    }
    
    ctrans_advance(tr); // Consumir 'struct'

    char struct_name[MAX_TOKEN_LEN] = "AnonStruct";
    if (tr->current.type == CTOK_IDENT) {
        safe_strcpy(struct_name, tr->current.text, sizeof(struct_name));
        ctrans_advance(tr);
    }

    if (!ctrans_match(tr, CTOK_LBRACE)) return;

    ctrans_emit(tr, "pub %s ", kw_struct(tr->lang));

    // Si no tenía nombre tras struct, se tomará al final del typedef
    char fields_buf[8192] = "";
    size_t fpos = 0;

    while (tr->current.type != CTOK_RBRACE && tr->current.type != CTOK_EOF) {
        char c_ftype[MAX_TOKEN_LEN];
        parse_c_type(tr, c_ftype, sizeof(c_ftype));

        if (tr->current.type == CTOK_IDENT) {
            char fname[MAX_TOKEN_LEN];
            safe_strcpy(fname, tr->current.text, sizeof(fname));
            ctrans_advance(tr);

            char upl_ftype[MAX_TOKEN_LEN];
            map_c_type_to_upl(c_ftype, upl_ftype, sizeof(upl_ftype));

            // Soporte para dimensiones de array en campos C: tipo nombre[N];
            if (ctrans_match(tr, CTOK_LBRACKET)) {
                char arr_size[64] = "";
                if (tr->current.type == CTOK_INT_LIT || tr->current.type == CTOK_IDENT) {
                    safe_strcpy(arr_size, tr->current.text, sizeof(arr_size));
                    ctrans_advance(tr);
                }
                ctrans_match(tr, CTOK_RBRACKET);
                char temp[MAX_TOKEN_LEN * 2];
                if (arr_size[0] != '\0') {
                    snprintf(temp, sizeof(temp), "[%s; %s]", upl_ftype, arr_size);
                } else {
                    snprintf(temp, sizeof(temp), "[%s]", upl_ftype);
                }
                safe_strcpy(upl_ftype, temp, sizeof(upl_ftype));
            }

            fpos += snprintf(&fields_buf[fpos], sizeof(fields_buf) - fpos, "    %s: %s,\n", fname, upl_ftype);
        }

        // Garantía absoluta de avance hacia adelante hasta ';' o '}'
        while (tr->current.type != CTOK_SEMICOLON && tr->current.type != CTOK_RBRACE && tr->current.type != CTOK_EOF) {
            ctrans_advance(tr);
        }
        ctrans_match(tr, CTOK_SEMICOLON);
    }
    ctrans_match(tr, CTOK_RBRACE);

    if (is_typedef && tr->current.type == CTOK_IDENT) {
        safe_strcpy(struct_name, tr->current.text, sizeof(struct_name));
        ctrans_advance(tr);
    }
    ctrans_match(tr, CTOK_SEMICOLON);

    ctrans_emit(tr, "%s {\n%s}\n\n", struct_name, fields_buf);
}

/* Transpilar una función o bloque de sentencias */
static void transpile_function_or_statement(C2UplTranspiler *tr) {
    if (tr->current.type == CTOK_TYPEDEF || (tr->current.type == CTOK_STRUCT && tr->lexer.source[tr->lexer.cursor] == '{')) {
        transpile_struct(tr);
        return;
    }

    if (tr->current.type == CTOK_PREPROC) {
        ctrans_emit(tr, "embed \"c\" {\n%s\n}\n\n", tr->current.text);
        ctrans_advance(tr);
        return;
    }

    /* Posible declaración o definición de función */
    char c_ret_type[MAX_TOKEN_LEN];
    parse_c_type(tr, c_ret_type, sizeof(c_ret_type));

    if (tr->current.type == CTOK_IDENT) {
        char fn_name[MAX_TOKEN_LEN];
        safe_strcpy(fn_name, tr->current.text, sizeof(fn_name));
        ctrans_advance(tr);

        if (ctrans_match(tr, CTOK_LPAREN)) {
            char upl_ret[MAX_TOKEN_LEN];
            map_c_type_to_upl(c_ret_type, upl_ret, sizeof(upl_ret));

            ctrans_emit(tr, "pub %s %s(", kw_fn(tr->lang), fn_name);

            bool first = true;
            while (tr->current.type != CTOK_RPAREN && tr->current.type != CTOK_EOF) {
                char c_ptype[MAX_TOKEN_LEN];
                parse_c_type(tr, c_ptype, sizeof(c_ptype));

                if (tr->current.type == CTOK_IDENT) {
                    if (!first) ctrans_emit(tr, ", ");
                    first = false;

                    char pname[MAX_TOKEN_LEN];
                    safe_strcpy(pname, tr->current.text, sizeof(pname));
                    ctrans_advance(tr);

                    char upl_ptype[MAX_TOKEN_LEN];
                    map_c_type_to_upl(c_ptype, upl_ptype, sizeof(upl_ptype));
                    ctrans_emit(tr, "%s: %s", pname, upl_ptype);
                } else if (strcmp(c_ptype, "void") == 0) {
                    break;
                }
                ctrans_match(tr, CTOK_COMMA);
            }
            ctrans_match(tr, CTOK_RPAREN);

            if (strcmp(upl_ret, "void") != 0 && upl_ret[0] != '\0') {
                ctrans_emit(tr, ") -> %s ", upl_ret);
            } else {
                ctrans_emit(tr, ") ");
            }

            if (ctrans_match(tr, CTOK_SEMICOLON)) {
                ctrans_emit(tr, ";\n\n");
                return;
            }

            if (ctrans_match(tr, CTOK_LBRACE)) {
                ctrans_emit(tr, "{\n");
                int brace_depth = 1;

                while (brace_depth > 0 && tr->current.type != CTOK_EOF) {
                    if (tr->current.type == CTOK_LBRACE) {
                        brace_depth++;
                        ctrans_emit(tr, "{\n");
                        ctrans_advance(tr);
                    } else if (tr->current.type == CTOK_RBRACE) {
                        brace_depth--;
                        if (brace_depth == 0) {
                            ctrans_emit(tr, "}\n\n");
                        } else {
                            ctrans_emit(tr, "}\n");
                        }
                        ctrans_advance(tr);
                    } else if (tr->current.type == CTOK_IF) {
                        ctrans_emit(tr, "    %s ", kw_if(tr->lang));
                        ctrans_advance(tr);
                        if (ctrans_match(tr, CTOK_LPAREN)) {
                            int pdepth = 1;
                            while (pdepth > 0 && tr->current.type != CTOK_EOF) {
                                if (tr->current.type == CTOK_LPAREN) pdepth++;
                                if (tr->current.type == CTOK_RPAREN) {
                                    pdepth--;
                                    if (pdepth == 0) { ctrans_advance(tr); break; }
                                }
                                ctrans_emit(tr, "%s ", tr->current.text);
                                ctrans_advance(tr);
                            }
                        }
                    } else if (tr->current.type == CTOK_ELSE) {
                        ctrans_emit(tr, "    %s ", kw_else(tr->lang));
                        ctrans_advance(tr);
                    } else if (tr->current.type == CTOK_WHILE) {
                        ctrans_emit(tr, "    %s ", kw_while(tr->lang));
                        ctrans_advance(tr);
                        if (ctrans_match(tr, CTOK_LPAREN)) {
                            int pdepth = 1;
                            while (pdepth > 0 && tr->current.type != CTOK_EOF) {
                                if (tr->current.type == CTOK_LPAREN) pdepth++;
                                if (tr->current.type == CTOK_RPAREN) {
                                    pdepth--;
                                    if (pdepth == 0) { ctrans_advance(tr); break; }
                                }
                                ctrans_emit(tr, "%s ", tr->current.text);
                                ctrans_advance(tr);
                            }
                        }
                    } else if (tr->current.type == CTOK_RETURN) {
                        ctrans_emit(tr, "    %s ", kw_return(tr->lang));
                        ctrans_advance(tr);
                    } else if (is_c_type_start(tr)) {
                        char vtype[MAX_TOKEN_LEN];
                        parse_c_type(tr, vtype, sizeof(vtype));
                        char upl_vtype[MAX_TOKEN_LEN];
                        map_c_type_to_upl(vtype, upl_vtype, sizeof(upl_vtype));

                        if (tr->current.type == CTOK_IDENT) {
                            char vname[MAX_TOKEN_LEN];
                            safe_strcpy(vname, tr->current.text, sizeof(vname));
                            ctrans_advance(tr);
                            ctrans_emit(tr, "    %s %s %s: %s ", kw_let(tr->lang), kw_mut(tr->lang), vname, upl_vtype);
                        }
                    } else {
                        ctrans_emit(tr, "%s ", tr->current.text);
                        if (tr->current.type == CTOK_SEMICOLON) {
                            ctrans_emit(tr, "\n");
                        }
                        ctrans_advance(tr);
                    }
                }
                return;
            }
        }
    } else {
        ctrans_advance(tr);
    }
}

static void transpile_c_to_upl(const char *c_source, TargetLang lang, FILE *out, char *out_buf, size_t buf_size) {
    C2UplTranspiler tr;
    clexer_init(&tr.lexer, c_source);
    tr.lang = lang;
    tr.out = out;
    tr.buf_pos = 0;
    tr.buffer[0] = '\0';
    ctrans_advance(&tr);

    ctrans_emit(&tr, "// Generado automaticamente por c2upl (C99 a UPL+ Transpiler)\n\n");

    while (tr.current.type != CTOK_EOF) {
        transpile_function_or_statement(&tr);
    }

    if (out_buf && buf_size > 0) {
        strncpy(out_buf, tr.buffer, buf_size - 1);
        out_buf[buf_size - 1] = '\0';
    }
}

/* ------------------------------------------------------------- */
/*            SUITE DE VERIFICACIÓN UNITARIA INTERNA             */
/* ------------------------------------------------------------- */

static void run_internal_tests(void) {
    printf("====================================================================\n");
    printf("  SUITE DE PRUEBAS: TRANSPILADOR NATIVO C99 A UPL+ (c2upl.c)        \n");
    printf("====================================================================\n");

    // Test 1: Tipos elementales
    char upl_t1[64], upl_t2[64], upl_t3[64];
    map_c_type_to_upl("int32_t", upl_t1, sizeof(upl_t1));
    map_c_type_to_upl("uint64_t", upl_t2, sizeof(upl_t2));
    map_c_type_to_upl("void*", upl_t3, sizeof(upl_t3));
    if (strcmp(upl_t1, "i32") != 0 || strcmp(upl_t2, "u64") != 0 || strcmp(upl_t3, "*void") != 0) {
        fprintf(stderr, "[FAIL] Error en mapeo de tipos elementales\n");
        exit(1);
    }
    printf("  [PASS] 1. Mapeo estricto de tipos C99 (int32_t->i32, uint64_t->u64, void*->*void)\n");

    // Test 2: Funciones en inglés (fn / return)
    const char *c_src_fn = "int calcular_suma(int a, int b) { return a + b; }";
    char out_buf[4096];
    transpile_c_to_upl(c_src_fn, LANG_EN, NULL, out_buf, sizeof(out_buf));
    if (!strstr(out_buf, "pub fn calcular_suma(a: i32, b: i32) -> i32") || !strstr(out_buf, "return a + b ;")) {
        fprintf(stderr, "[FAIL] Fallo en transpilacion de funcion EN:\n%s\n", out_buf);
        exit(1);
    }
    printf("  [PASS] 2. Transpilacion de funciones y firmas con dialecto ingles (fn / return)\n");

    // Test 3: Funciones en español (funcion / retornar)
    transpile_c_to_upl(c_src_fn, LANG_ES, NULL, out_buf, sizeof(out_buf));
    if (!strstr(out_buf, "pub funcion calcular_suma(a: i32, b: i32) -> i32") || !strstr(out_buf, "retornar a + b ;")) {
        fprintf(stderr, "[FAIL] Fallo en transpilacion de funcion ES:\n%s\n", out_buf);
        exit(1);
    }
    printf("  [PASS] 3. Transpilacion de funciones y firmas con dialecto espanol (funcion / retornar)\n");

    // Test 4: Estructuras (typedef struct)
    const char *c_src_struct = "typedef struct { int x; int y; } Punto;";
    transpile_c_to_upl(c_src_struct, LANG_EN, NULL, out_buf, sizeof(out_buf));
    if (!strstr(out_buf, "pub struct Punto") || !strstr(out_buf, "x: i32,") || !strstr(out_buf, "y: i32,")) {
        fprintf(stderr, "[FAIL] Fallo en transpilacion de struct:\n%s\n", out_buf);
        exit(1);
    }
    printf("  [PASS] 4. Transpilacion de structs agregados (typedef struct -> pub struct)\n");

    // Test 5: Control de flujo (if / while / variables)
    const char *c_src_flow = "int prueba_flujo(int val) { int mut_val = val; if (mut_val > 0) { return mut_val; } return 0; }";
    transpile_c_to_upl(c_src_flow, LANG_EN, NULL, out_buf, sizeof(out_buf));
    if (!strstr(out_buf, "if mut_val > 0") || !strstr(out_buf, "let mut mut_val: i32")) {
        fprintf(stderr, "[FAIL] Fallo en transpilacion de flujo:\n%s\n", out_buf);
        exit(1);
    }
    printf("  [PASS] 5. Transpilacion de variables mutables (let mut) y control condicional (if)\n");

    printf("====================================================================\n");
    printf("  RESULTADO: 5/5 PRUEBAS c2upl SUPERADAS SATISFACTORIAMENTE (0 FALLOS)\n");
    printf("====================================================================\n");
}

/* ------------------------------------------------------------- */
/*                      DRIVER PRINCIPAL                         */
/* ------------------------------------------------------------- */

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "c2upl: Transpilador Nativo C99 a UPL+ (v1.0 Standard C99)\n");
        fprintf(stderr, "Uso: %s [opciones] <archivo.c>\n\n", argv[0]);
        fprintf(stderr, "Opciones:\n");
        fprintf(stderr, "  -o <salida.upl>     Especificar archivo de salida\n");
        fprintf(stderr, "  -l, --lang <en|es>  Seleccionar dialecto UPL+ objetivo (por defecto: en)\n");
        fprintf(stderr, "  --test              Ejecutar la suite de pruebas unitarias internas\n");
        return 1;
    }

    if (strcmp(argv[1], "--test") == 0) {
        run_internal_tests();
        return 0;
    }

    const char *input_path = NULL;
    const char *output_path = NULL;
    TargetLang lang = LANG_EN;

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            output_path = argv[++i];
        } else if ((strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "--lang") == 0) && i + 1 < argc) {
            i++;
            if (strcmp(argv[i], "es") == 0) lang = LANG_ES;
            else lang = LANG_EN;
        } else if (strcmp(argv[i], "--test") == 0) {
            run_internal_tests();
            return 0;
        } else if (argv[i][0] != '-') {
            input_path = argv[i];
        }
    }

    if (!input_path) {
        fprintf(stderr, "[ERROR] No se especifico archivo de entrada .c\n");
        return 1;
    }

    FILE *fin = fopen(input_path, "rb");
    if (!fin) {
        fprintf(stderr, "[ERROR] No se pudo abrir archivo C: %s\n", input_path);
        return 1;
    }

    fseek(fin, 0, SEEK_END);
    long len = ftell(fin);
    fseek(fin, 0, SEEK_SET);

    char *source = (char*)malloc(len + 1);
    if (!source) {
        fclose(fin);
        return 1;
    }
    size_t rb = fread(source, 1, len, fin);
    source[rb] = '\0';
    fclose(fin);

    FILE *fout = stdout;
    if (output_path) {
        fout = fopen(output_path, "wb");
        if (!fout) {
            fprintf(stderr, "[ERROR] No se pudo crear archivo UPL+: %s\n", output_path);
            free(source);
            return 1;
        }
    }

    transpile_c_to_upl(source, lang, fout, NULL, 0);

    if (output_path && fout != stdout) {
        fclose(fout);
        printf("[OK] Archivo C99 transpilado a UPL+ exitosamente: %s\n", output_path);
    }

    free(source);
    return 0;
}
