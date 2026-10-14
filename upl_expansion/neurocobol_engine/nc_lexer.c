#include "nc_lexer.h"
#include <string.h>

static const char* source_code;
static int current_pos = 0;
static int current_line = 1;

// 🧬 SYSTEM CTYPE ISOLATION (v30.23)
static int local_isspace(char c) { return (c == ' ' || c == '\t' || c == '\n' || c == '\r'); }
static int local_isdigit(char c) { return (c >= '0' && c <= '9'); }
static int local_isalpha(char c) { return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')); }
static int local_isalnum(char c) { return (local_isalpha(c) || local_isdigit(c)); }

void nc_lexer_init(const char* source) {
    source_code = source;
    current_pos = 0;
    current_line = 1;
}

static char peek() { return source_code[current_pos]; }
static char advance() { 
    // 🧬 HARDWARE EMERALD PULSE: ADVANCE (Lexer progressing v30.30)
    extern void dvtrga_debug_pixel(int x, int y, unsigned int color);
    static int emerald_pulse_x = 0;
    dvtrga_debug_pixel(emerald_pulse_x++ % 1024, 230, 0x00FF88);

    char c = source_code[current_pos++];
    if (c == '\n') current_line++;
    return c;
}

static int is_at_end() { return source_code[current_pos] == '\0'; }

static void skip_whitespace() {
    extern void dvtrga_debug_pixel(int x, int y, unsigned int color);
    static int amber_pulse_x = 0;
    
    while (!is_at_end()) {
        // 🧬 HARDWARE DEBUG PULSE: AMBER (Looping in skip_whitespace v30.23)
        dvtrga_debug_pixel(amber_pulse_x++ % 1024, 210, 0xFFFF00);

        char c = peek();
        if (local_isspace(c)) advance();
        else if (c == '/' && source_code[current_pos + 1] == '/') {
            while (peek() != '\n' && !is_at_end()) advance();
        } else break;
    }
}

static TokenType check_keyword(const char* word) {
    if (NC_STRICMP(word, "PROGRAMA") == 0 || NC_STRICMP(word, "PROGRAM") == 0) return TOK_PROGRAM;
    if (NC_STRICMP(word, "DATOS") == 0 || NC_STRICMP(word, "DATA") == 0) return TOK_DATA;
    if (NC_STRICMP(word, "LOGICA") == 0 || NC_STRICMP(word, "LOGIC") == 0) return TOK_LOGIC;
    if (NC_STRICMP(word, "FIN") == 0 || NC_STRICMP(word, "END") == 0) return TOK_END;
    if (NC_STRICMP(word, "DEFINIR") == 0 || NC_STRICMP(word, "DEFINE") == 0 || NC_STRICMP(word, "VAR") == 0) return TOK_DEFINE;
    if (NC_STRICMP(word, "COMO") == 0 || NC_STRICMP(word, "AS") == 0) return TOK_AS;
    if (NC_STRICMP(word, "MOSTRAR") == 0 || NC_STRICMP(word, "SHOW") == 0) return TOK_SHOW;
    if (NC_STRICMP(word, "SI") == 0 || NC_STRICMP(word, "IF") == 0) return TOK_IF;
    if (NC_STRICMP(word, "ENTONCES") == 0 || NC_STRICMP(word, "THEN") == 0) return TOK_THEN;
    if (NC_STRICMP(word, "SINO") == 0 || NC_STRICMP(word, "ELSE") == 0) return TOK_ELSE;
    if (NC_STRICMP(word, "FIN_SI") == 0 || NC_STRICMP(word, "END_IF") == 0) return TOK_END_IF;
    if (NC_STRICMP(word, "VENTANA") == 0 || NC_STRICMP(word, "WINDOW") == 0) return TOK_WINDOW;
    if (NC_STRICMP(word, "BOTON") == 0 || NC_STRICMP(word, "BUTTON") == 0) return TOK_BUTTON;
    if (NC_STRICMP(word, "CAMPO") == 0 || NC_STRICMP(word, "FIELD") == 0) return TOK_FIELD;
    if (NC_STRICMP(word, "AL") == 0 || NC_STRICMP(word, "ON") == 0) return TOK_ON;
    if (NC_STRICMP(word, "CLIC") == 0 || NC_STRICMP(word, "CLICK") == 0) return TOK_CLICK;
    if (NC_STRICMP(word, "ABRIR") == 0 || NC_STRICMP(word, "OPEN") == 0) return TOK_OPEN;
    if (NC_STRICMP(word, "LEER") == 0 || NC_STRICMP(word, "READ") == 0) return TOK_READ;
    if (NC_STRICMP(word, "ESCRIBIR") == 0 || NC_STRICMP(word, "WRITE") == 0) return TOK_WRITE;
    if (NC_STRICMP(word, "CERRAR") == 0 || NC_STRICMP(word, "CLOSE") == 0) return TOK_CLOSE;
    if (NC_STRICMP(word, "ARCHIVO") == 0 || NC_STRICMP(word, "FILE") == 0) return TOK_FILE;
    if (NC_STRICMP(word, "REGISTRO") == 0 || NC_STRICMP(word, "RECORD") == 0) return TOK_RECORD;
    if (NC_STRICMP(word, "MODULO") == 0 || NC_STRICMP(word, "MODULE") == 0) return TOK_MODULE;
    if (NC_STRICMP(word, "BUS") == 0) return TOK_BUS;
    if (NC_STRICMP(word, "ENVIAR") == 0 || NC_STRICMP(word, "SEND") == 0) return TOK_SEND;
    if (NC_STRICMP(word, "RECIBIR") == 0 || NC_STRICMP(word, "RECEIVE") == 0) return TOK_RECEIVE;
    if (NC_STRICMP(word, "NITRO") == 0 || NC_STRICMP(word, "OVERDRIVE") == 0) return TOK_NITRO;
    if (NC_STRICMP(word, "IZQUIERDO") == 0 || NC_STRICMP(word, "LEFT") == 0) return TOK_LEFT;
    if (NC_STRICMP(word, "DERECHO") == 0 || NC_STRICMP(word, "RIGHT") == 0) return TOK_RIGHT;
    if (NC_STRICMP(word, "GLOBAL") == 0) return TOK_GLOBAL;
    if (NC_STRICMP(word, "IMPORTAR") == 0 || NC_STRICMP(word, "IMPORT") == 0) return TOK_IMPORT;
    if (NC_STRICMP(word, "COMPRIMIDO") == 0 || NC_STRICMP(word, "COMPRESSED") == 0 || NC_STRICMP(word, "COMP-3") == 0) return TOK_COMPRESSED;
    if (NC_STRICMP(word, "DEFENSA") == 0 || NC_STRICMP(word, "DEFENSE") == 0) return TOK_DEFENSE;
    if (NC_STRICMP(word, "INTEGRIDAD") == 0 || NC_STRICMP(word, "INTEGRITY") == 0) return TOK_INTEGRITY;
    if (NC_STRICMP(word, "CRIPTOGRAFIA") == 0 || NC_STRICMP(word, "CRYPTO") == 0) return TOK_CRYPTO;
    if (NC_STRICMP(word, "AUTENTICACION") == 0 || NC_STRICMP(word, "AUTH") == 0) return TOK_AUTH;
    return TOK_IDENTIFIER;
}

Token nc_lexer_next_token() {
    skip_whitespace();
    Token token;
    token.line = current_line;
    memset(token.text, 0, sizeof(token.text));

    if (is_at_end()) {
        token.type = TOK_EOF;
        return token;
    }

    char c = advance();
    if (local_isalpha(c) || c == '_') {
        int start = current_pos - 1;
        while (local_isalnum(peek()) || peek() == '_') advance();
        int len = current_pos - start;
        strncpy(token.text, source_code + start, len);
        token.type = check_keyword(token.text);
    } else if (local_isdigit(c)) {
        int start = current_pos - 1;
        while (local_isdigit(peek())) advance();
        int len = current_pos - start;
        strncpy(token.text, source_code + start, len);
        token.type = TOK_NUMBER;
    } else if (c == '"' || c == '\'') {
        char quote = c;
        int start = current_pos;
        while (peek() != quote && !is_at_end()) advance();
        int len = current_pos - start;
        strncpy(token.text, source_code + start, len);
        if (!is_at_end()) advance();
        token.type = TOK_STRING;
    } else {
        token.text[0] = c;
        switch (c) {
            case '=': token.type = TOK_EQUALS; break;
            case '+': token.type = TOK_PLUS; break;
            case '-': token.type = TOK_MINUS; break;
            case '*': token.type = TOK_STAR; break;
            case '/': token.type = TOK_SLASH; break;
            case '.': token.type = TOK_DOT; break;
            default: token.type = TOK_UNKNOWN; break;
        }
    }
    
    // 🧬 HARDWARE DEBUG PULSE: GREEN (Lexer Alive v30.22)
    extern void dvtrga_debug_pixel(int x, int y, unsigned int color);
    static int lex_pulse_x = 0;
    dvtrga_debug_pixel(lex_pulse_x++ % 1024, 200, 0x00FF00);
    
    return token;
}
