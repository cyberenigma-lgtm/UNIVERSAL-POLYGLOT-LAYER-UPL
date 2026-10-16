#include "nc_lexer.h"
#include <string.h>

static const char* source_code;
static int current_pos = 0;
static int current_line = 1;

#ifdef NC_ENABLE_HARDWARE_PULSE
extern void dvtrga_debug_pixel(int x, int y, unsigned int color);
#define NC_PULSE(x, y, c) dvtrga_debug_pixel(x, y, c)
#else
#define NC_PULSE(x, y, c) ((void)0)
#endif

// Character classification routines (safe across non-ASCII/locale boundaries)
static inline int local_isspace(char c) { 
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r'); 
}

static inline int local_isdigit(char c) { 
    return (c >= '0' && c <= '9'); 
}

static inline int local_isalpha(char c) { 
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')); 
}

static inline int local_isalnum(char c) { 
    return (local_isalpha(c) || local_isdigit(c)); 
}

void nc_lexer_init(const char* source) {
    source_code = source ? source : "";
    current_pos = 0;
    current_line = 1;
}

static inline char peek(void) { 
    return source_code[current_pos]; 
}

static inline char advance(void) { 
    static int emerald_pulse_x = 0;
    NC_PULSE(emerald_pulse_x++ % 1024, 230, 0x00FF88);

    if (source_code[current_pos] == '\0') {
        return '\0'; // Perimeter boundary protection
    }
    char c = source_code[current_pos++];
    if (c == '\n') {
        current_line++;
    }
    return c;
}

static inline int is_at_end(void) { 
    return source_code[current_pos] == '\0'; 
}

static void skip_whitespace(void) {
    static int amber_pulse_x = 0;
    while (!is_at_end()) {
        NC_PULSE(amber_pulse_x++ % 1024, 210, 0xFFFF00);
        char c = peek();
        if (local_isspace(c)) {
            advance();
        } else if (c == '/' && source_code[current_pos + 1] == '/') {
            while (peek() != '\n' && !is_at_end()) {
                advance();
            }
        } else {
            break;
        }
    }
}

// Keyword map entry with length pre-filtering for O(1) early rejection
typedef struct {
    const char* word;
    size_t len;
    TokenType type;
} KeywordMapping;

static const KeywordMapping KEYWORDS[] = {
    // 2 chars
    { "SI", 2, TOK_IF }, { "IF", 2, TOK_IF },
    { "AS", 2, TOK_AS }, { "AL", 2, TOK_ON }, { "ON", 2, TOK_ON },
    // 3 chars
    { "FIN", 3, TOK_FIN }, { "END", 3, TOK_END },
    { "VAR", 3, TOK_DEFINE }, { "BUS", 3, TOK_BUS },
    // 4 chars
    { "DATA", 4, TOK_DATA }, { "COMO", 4, TOK_AS },
    { "SHOW", 4, TOK_SHOW }, { "THEN", 4, TOK_THEN },
    { "ELSE", 4, TOK_ELSE }, { "CLIC", 4, TOK_CLICK },
    { "OPEN", 4, TOK_OPEN }, { "LEER", 4, TOK_READ },
    { "READ", 4, TOK_READ }, { "FILE", 4, TOK_FILE },
    { "SEND", 4, TOK_SEND }, { "LEFT", 4, TOK_LEFT },
    { "AUTH", 4, TOK_AUTH },
    // 5 chars
    { "DATOS", 5, TOK_DATA }, { "LOGIC", 5, TOK_LOGIC },
    { "FIN_SI", 5, TOK_END_IF },
    { "CAMPO", 5, TOK_FIELD }, { "FIELD", 5, TOK_FIELD },
    { "CLICK", 5, TOK_CLICK }, { "ABRIR", 5, TOK_OPEN },
    { "WRITE", 5, TOK_WRITE }, { "CLOSE", 5, TOK_CLOSE },
    { "NITRO", 5, TOK_NITRO }, { "RIGHT", 5, TOK_RIGHT },
    // 6 chars
    { "LOGICA", 6, TOK_LOGIC }, { "DEFINE", 6, TOK_DEFINE },
    { "END_IF", 6, TOK_END_IF }, { "WINDOW", 6, TOK_WINDOW },
    { "BUTTON", 6, TOK_BUTTON }, { "CERRAR", 6, TOK_CLOSE },
    { "RECORD", 6, TOK_RECORD }, { "MODULE", 6, TOK_MODULE },
    { "MODULO", 6, TOK_MODULE }, { "ENVIAR", 6, TOK_SEND },
    { "GLOBAL", 6, TOK_GLOBAL }, { "IMPORT", 6, TOK_IMPORT },
    { "COMP-3", 6, TOK_COMPRESSED }, { "CRYPTO", 6, TOK_CRYPTO },
    { "NATIVO", 6, TOK_NATIVO },
    // 7 chars
    { "PROGRAM", 7, TOK_PROGRAM }, { "DEFINIR", 7, TOK_DEFINE },
    { "MOSTRAR", 7, TOK_SHOW }, { "ENTONCES", 8, TOK_THEN },
    { "VENTANA", 7, TOK_WINDOW }, { "ARCHIVO", 7, TOK_FILE },
    { "RECEIVE", 7, TOK_RECEIVE }, { "RECIBIR", 7, TOK_RECEIVE },
    { "DEFENSE", 7, TOK_DEFENSE }, { "DEFENSA", 7, TOK_DEFENSE },
    // 8+ chars
    { "PROGRAMA", 8, TOK_PROGRAM }, { "IZQUIERDO", 9, TOK_LEFT },
    { "ESCRIBIR", 8, TOK_WRITE }, { "DERECHO", 7, TOK_RIGHT },
    { "IMPORTAR", 8, TOK_IMPORT }, { "OVERDRIVE", 9, TOK_NITRO },
    { "INTEGRITY", 9, TOK_INTEGRITY }, { "INTEGRIDAD", 10, TOK_INTEGRITY },
    { "COMPRESSED", 10, TOK_COMPRESSED }, { "COMPRIMIDO", 10, TOK_COMPRESSED },
    { "CRIPTOGRAFIA", 12, TOK_CRYPTO }, { "AUTENTICACION", 13, TOK_AUTH }
};

static const size_t KEYWORDS_COUNT = sizeof(KEYWORDS) / sizeof(KEYWORDS[0]);

static TokenType check_keyword(const char* word, size_t len) {
    for (size_t i = 0; i < KEYWORDS_COUNT; i++) {
        // Fast O(1) length pre-filter: eliminates >90% of redundant string comparisons
        if (KEYWORDS[i].len == len) {
            if (NC_STRICMP(word, KEYWORDS[i].word) == 0) {
                return KEYWORDS[i].type;
            }
        }
    }
    return TOK_IDENTIFIER;
}

Token nc_lexer_next_token(void) {
    skip_whitespace();
    Token token;
    token.line = current_line;
    memset(token.text, 0, sizeof(token.text));

    const size_t max_len = sizeof(token.text) - 1;

    if (is_at_end()) {
        token.type = TOK_EOF;
        return token;
    }

    char c = advance();
    if (local_isalpha(c) || c == '_') {
        int start = current_pos - 1;
        while (local_isalnum(peek()) || peek() == '_') {
            advance();
        }
        int len = current_pos - start;
        if ((size_t)len > max_len) {
            len = (int)max_len; // Safe defensive truncation
        }
        strncpy(token.text, source_code + start, len);
        token.text[len] = '\0';
        token.type = check_keyword(token.text, (size_t)len);
    } else if (local_isdigit(c)) {
        int start = current_pos - 1;
        while (local_isdigit(peek())) {
            advance();
        }
        int len = current_pos - start;
        if ((size_t)len > max_len) {
            len = (int)max_len;
        }
        strncpy(token.text, source_code + start, len);
        token.text[len] = '\0';
        token.type = TOK_NUMBER;
    } else if (c == '"' || c == '\'') {
        char quote = c;
        int start = current_pos;
        while (peek() != quote && !is_at_end()) {
            advance();
        }
        int len = current_pos - start;
        if ((size_t)len > max_len) {
            len = (int)max_len;
        }
        strncpy(token.text, source_code + start, len);
        token.text[len] = '\0';

        if (!is_at_end()) {
            advance(); // Consume closing quote
            token.type = TOK_STRING;
        } else {
            // Lexical error: unterminated string literal before EOF
            token.type = TOK_ERROR;
        }
    } else {
        token.text[0] = c;
        token.text[1] = '\0';
        switch (c) {
            case '=': token.type = TOK_EQUALS; break;
            case '+': token.type = TOK_PLUS; break;
            case '-': token.type = TOK_MINUS; break;
            case '*': token.type = TOK_STAR; break;
            case '/': token.type = TOK_SLASH; break;
            case '.': token.type = TOK_DOT; break;
            default:  token.type = TOK_UNKNOWN; break;
        }
    }

    static int lex_pulse_x = 0;
    NC_PULSE(lex_pulse_x++ % 1024, 200, 0x00FF00);

    return token;
}

const char* nc_token_type_to_string(TokenType type) {
    switch (type) {
        case TOK_PROGRAM: return "TOK_PROGRAM";
        case TOK_DATA: return "TOK_DATA";
        case TOK_LOGIC: return "TOK_LOGIC";
        case TOK_END: return "TOK_END";
        case TOK_FIN: return "TOK_FIN";
        case TOK_DEFINE: return "TOK_DEFINE";
        case TOK_AS: return "TOK_AS";
        case TOK_SHOW: return "TOK_SHOW";
        case TOK_IF: return "TOK_IF";
        case TOK_THEN: return "TOK_THEN";
        case TOK_ELSE: return "TOK_ELSE";
        case TOK_END_IF: return "TOK_END_IF";
        case TOK_WINDOW: return "TOK_WINDOW";
        case TOK_BUTTON: return "TOK_BUTTON";
        case TOK_FIELD: return "TOK_FIELD";
        case TOK_CAMPO: return "TOK_CAMPO";
        case TOK_ON: return "TOK_ON";
        case TOK_CLICK: return "TOK_CLICK";
        case TOK_OPEN: return "TOK_OPEN";
        case TOK_READ: return "TOK_READ";
        case TOK_WRITE: return "TOK_WRITE";
        case TOK_CLOSE: return "TOK_CLOSE";
        case TOK_FILE: return "TOK_FILE";
        case TOK_RECORD: return "TOK_RECORD";
        case TOK_MODULE: return "TOK_MODULE";
        case TOK_BUS: return "TOK_BUS";
        case TOK_SEND: return "TOK_SEND";
        case TOK_RECEIVE: return "TOK_RECEIVE";
        case TOK_NITRO: return "TOK_NITRO";
        case TOK_NATIVO: return "TOK_NATIVO";
        case TOK_LEFT: return "TOK_LEFT";
        case TOK_RIGHT: return "TOK_RIGHT";
        case TOK_GLOBAL: return "TOK_GLOBAL";
        case TOK_IMPORT: return "TOK_IMPORT";
        case TOK_COMPRESSED: return "TOK_COMPRESSED";
        case TOK_DEFENSE: return "TOK_DEFENSE";
        case TOK_INTEGRITY: return "TOK_INTEGRITY";
        case TOK_CRYPTO: return "TOK_CRYPTO";
        case TOK_AUTH: return "TOK_AUTH";
        case TOK_ERROR: return "TOK_ERROR";
        case TOK_IDENTIFIER: return "TOK_IDENTIFIER";
        case TOK_STRING: return "TOK_STRING";
        case TOK_NUMBER: return "TOK_NUMBER";
        case TOK_EQUALS: return "TOK_EQUALS";
        case TOK_PLUS: return "TOK_PLUS";
        case TOK_MINUS: return "TOK_MINUS";
        case TOK_STAR: return "TOK_STAR";
        case TOK_SLASH: return "TOK_SLASH";
        case TOK_COMMENT: return "TOK_COMMENT";
        case TOK_DOT: return "TOK_DOT";
        case TOK_EOF: return "TOK_EOF";
        default: return "TOK_UNKNOWN";
    }
}
