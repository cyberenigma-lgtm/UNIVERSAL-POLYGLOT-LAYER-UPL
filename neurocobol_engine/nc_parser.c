#include "nc_parser.h"
#include <string.h>

// 🧬 NATIVE MEMORY LINK: Redirect to System malloc (stubs.c)
extern void* malloc(size_t size);
extern void free(void* ptr);

#define malloc malloc
#define free free

static Token current_token;
static int defense_integrity_loaded = 0;
static int defense_crypto_loaded = 0;
static int defense_auth_loaded = 0;

static ASTNode* create_node(ASTNodeType type, const char* text) {
    // 🧬 HARDWARE DEBUG PULSE: BLUE (Parser Node Created v30.22)
    extern void dvtrga_debug_pixel(int x, int y, unsigned int color);
    static int parse_pulse_x = 0;
    dvtrga_debug_pixel(parse_pulse_x++ % 1024, 300, 0x0000FF);

    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = type;
    memset(node->text, 0, sizeof(node->text));
    if (text) strncpy(node->text, text, sizeof(node->text) - 1);
    node->left = node->right = node->next = NULL;
    return node;
}

static void consume(TokenType type) {
    if (current_token.type == type) {
        current_token = nc_lexer_next_token();
    } else {
        current_token = nc_lexer_next_token();
    }
}

static ASTNode* parse_statement();
static ASTNode* parse_logic_block();
static ASTNode* parse_ui_element();

static ASTNode* parse_expression() {
    if (current_token.type == TOK_STRING) {
        ASTNode* node = create_node(NODE_STRING_LITERAL, current_token.text);
        consume(TOK_STRING);
        return node;
    } else if (current_token.type == TOK_IDENTIFIER) {
        ASTNode* node = create_node(NODE_IDENTIFIER, current_token.text);
        consume(TOK_IDENTIFIER);
        return node;
    }
    return NULL;
}

static ASTNode* parse_ui_element() {
    if (current_token.type == TOK_WINDOW) {
        consume(TOK_WINDOW);
        char winName[64] = {0};
        if (current_token.type == TOK_IDENTIFIER) {
            strncpy(winName, current_token.text, sizeof(winName)-1);
            consume(TOK_IDENTIFIER);
        }
        ASTNode* node = create_node(NODE_WINDOW, winName);
        ASTNode* head = NULL;
        ASTNode* current = NULL;
        while (current_token.type != TOK_END && current_token.type != TOK_EOF) {
            ASTNode* item = parse_ui_element();
            if (!item) item = parse_statement();
            if (item) {
                if (!head) head = item;
                else current->next = item;
                current = item;
            } else current_token = nc_lexer_next_token();
        }
        node->right = head;
        consume(TOK_END);
        return node;
    } else if (current_token.type == TOK_BUTTON) {
        consume(TOK_BUTTON);
        char btnText[64] = {0};
        if (current_token.type == TOK_STRING || current_token.type == TOK_IDENTIFIER) {
            strncpy(btnText, current_token.text, sizeof(btnText)-1);
            consume(current_token.type);
        }
        ASTNode* node = create_node(NODE_BUTTON, btnText);
        if (current_token.type == TOK_ON) { consume(TOK_ON); consume(TOK_CLICK); }
        node->right = parse_logic_block();
        consume(TOK_END);
        return node;
    }
    return NULL;
}

static ASTNode* parse_file_statement() {
    if (current_token.type == TOK_WRITE) {
        consume(TOK_WRITE);
        consume(TOK_FILE);
        return create_node(NODE_WRITE, "WRITE");
    }
    return NULL;
}

static ASTNode* parse_native_block() {
    consume(TOK_NATIVO);
    char native_code[1024] = {0};
    // Note: Simple greedy consumer for NATIVO logic
    while (current_token.type != TOK_FIN && current_token.type != TOK_END && current_token.type != TOK_EOF) {
        strncat(native_code, current_token.text, 1023);
        strncat(native_code, " ", 1023);
        consume(current_token.type);
    }
    consume(TOK_FIN);
    return create_node(NODE_NATIVO, native_code);
}

static ASTNode* parse_statement() {
    if (current_token.type == TOK_SHOW) {
        consume(TOK_SHOW);
        ASTNode* node = create_node(NODE_SHOW, "SHOW");
        node->left = parse_expression();
        return node;
    } else if (current_token.type == TOK_IF) {
        consume(TOK_IF);
        ASTNode* node = create_node(NODE_IF, "IF");
        node->left = parse_expression();
        consume(TOK_THEN);
        node->right = parse_logic_block();
        consume(TOK_END);
        return node;
    } else if (current_token.type == TOK_WINDOW || current_token.type == TOK_BUTTON) {
        return parse_ui_element();
    } else if (current_token.type == TOK_CAMPO || current_token.type == TOK_FIELD) {
        consume(current_token.type);
        char fieldName[64] = {0};
        if (current_token.type == TOK_IDENTIFIER) {
            strncpy(fieldName, current_token.text, 63);
            consume(TOK_IDENTIFIER);
        }
        return create_node(NODE_FIELD, fieldName);
    } else if (current_token.type == TOK_WRITE) {
        return parse_file_statement();
    } else if (current_token.type == TOK_NATIVO) {
        return parse_native_block();
    } else if (current_token.type == TOK_NITRO) {
        consume(TOK_NITRO);
        return create_node(NODE_NITRO, "NITRO");
    }
    return NULL;
}

static ASTNode* parse_logic_block() {
    ASTNode* head = NULL;
    ASTNode* current = NULL;
    while (current_token.type != TOK_END && current_token.type != TOK_END_IF && current_token.type != TOK_EOF) {
        ASTNode* stmt = parse_statement();
        if (stmt) {
            if (!head) head = stmt;
            else current->next = stmt;
            current = stmt;
        } else break;
    }
    return head;
}

ASTNode* nc_parse(const char* source) {
    // 🧬 HARDWARE ENTRY PULSE: RED BLOCK (Parser Nexus v30.31.2)
    extern void dvtrga_debug_pixel(int x, int y, unsigned int color);
    for (int yp = 100; yp < 150; yp++) {
        for (int xp = 500; xp < 600; xp++) {
            dvtrga_debug_pixel(xp, yp, 0xFF0000);
        }
    }

    nc_lexer_init(source);
    current_token = nc_lexer_next_token();
    if (current_token.type == TOK_PROGRAM) {
        consume(TOK_PROGRAM);
        ASTNode* prog = create_node(NODE_PROGRAM, current_token.text);
        consume(TOK_IDENTIFIER);
        while (current_token.type != TOK_EOF) {
            if (current_token.type == TOK_DATA) {
                consume(TOK_DATA);
                // 🧬 SYSTEM SNAP: Prevent infinite loop in data section
                while (current_token.type != TOK_LOGIC && current_token.type != TOK_EOF) {
                    current_token = nc_lexer_next_token();
                }
            } else if (current_token.type == TOK_LOGIC) {
                consume(TOK_LOGIC);
                prog->right = parse_logic_block();
            } else if (current_token.type == TOK_END) { consume(TOK_END); break; }
            else if (current_token.type == TOK_DOT) { consume(TOK_DOT); }
            else { 
                // 🧬 DEBUG: Unknown token in main parse loop (v30.20)
                // extern void k_printf(const char* fmt, ...);
                // k_printf(" [PARSE] Skipping Unknown: %d\n", current_token.type);
                current_token = nc_lexer_next_token(); 
            }
        }
        return prog;
    }
    return NULL;
}

void nc_free_ast(ASTNode* node) {
    if (!node) return;
    nc_free_ast(node->left);
    nc_free_ast(node->right);
    nc_free_ast(node->next);
    free(node);
}
