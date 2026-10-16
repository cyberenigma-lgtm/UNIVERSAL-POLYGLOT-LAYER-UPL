#include "nc_parser.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef NC_ENABLE_HARDWARE_PULSE
extern void dvtrga_debug_pixel(int x, int y, unsigned int color);
#define NC_PULSE(x, y, c) dvtrga_debug_pixel(x, y, c)
#else
#define NC_PULSE(x, y, c) ((void)0)
#endif

static Token current_token;
static int parser_errors = 0;

int nc_parser_get_error_count(void) {
    return parser_errors;
}

static ASTNode* create_node(ASTNodeType type, const char* text) {
    static int parse_pulse_x = 0;
    NC_PULSE(parse_pulse_x++ % 1024, 300, 0x0000FF);

    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (!node) {
        return NULL;
    }
    node->type = type;
    memset(node->text, 0, sizeof(node->text));
    if (text) {
        strncpy(node->text, text, sizeof(node->text) - 1);
        node->text[sizeof(node->text) - 1] = '\0';
    }
    node->left = NULL;
    node->right = NULL;
    node->next = NULL;
    return node;
}

static int check(TokenType type) {
    return current_token.type == type;
}

static void advance_token(void) {
    current_token = nc_lexer_next_token();
}

static int expect(TokenType type, const char* expected_desc) {
    if (current_token.type == type) {
        advance_token();
        return 1;
    }
    parser_errors++;
    fprintf(stderr, "[NC_PARSER] Syntax Error (line %d): Expected '%s', but encountered '%s' (%s)\n",
            current_token.line, expected_desc, current_token.text, nc_token_type_to_string(current_token.type));
    
    // Panic recovery: advance if not already at EOF to prevent infinite loops
    if (current_token.type != TOK_EOF) {
        advance_token();
    }
    return 0;
}

static ASTNode* parse_statement(void);
static ASTNode* parse_logic_block(void);
static ASTNode* parse_ui_element(void);

static ASTNode* parse_expression(void) {
    if (check(TOK_STRING)) {
        ASTNode* node = create_node(NODE_STRING_LITERAL, current_token.text);
        advance_token();
        return node;
    } else if (check(TOK_NUMBER)) {
        ASTNode* node = create_node(NODE_NUMBER_LITERAL, current_token.text);
        advance_token();
        return node;
    } else if (check(TOK_IDENTIFIER)) {
        ASTNode* node = create_node(NODE_IDENTIFIER, current_token.text);
        advance_token();
        return node;
    }
    return NULL;
}

static ASTNode* parse_ui_element(void) {
    if (check(TOK_WINDOW)) {
        advance_token();
        char win_name[NC_MAX_TOKEN_LEN] = {0};
        if (check(TOK_IDENTIFIER)) {
            strncpy(win_name, current_token.text, sizeof(win_name) - 1);
            advance_token();
        }
        ASTNode* node = create_node(NODE_WINDOW, win_name);
        ASTNode* head = NULL;
        ASTNode* current = NULL;

        while (!check(TOK_END) && !check(TOK_FIN) && !check(TOK_EOF)) {
            ASTNode* item = parse_ui_element();
            if (!item) {
                item = parse_statement();
            }
            if (item) {
                if (!head) {
                    head = item;
                } else {
                    current->next = item;
                }
                current = item;
            } else {
                advance_token();
            }
        }
        if (node) {
            node->right = head;
        }
        if (check(TOK_FIN)) advance_token();
        else expect(TOK_END, "END o FIN para cerrar bloque de ventana");
        return node;
    } else if (check(TOK_BUTTON)) {
        advance_token();
        char btn_text[NC_MAX_TOKEN_LEN] = {0};
        if (check(TOK_STRING) || check(TOK_IDENTIFIER)) {
            strncpy(btn_text, current_token.text, sizeof(btn_text) - 1);
            advance_token();
        }
        ASTNode* node = create_node(NODE_BUTTON, btn_text);
        if (check(TOK_ON)) {
            advance_token();
            expect(TOK_CLICK, "CLICK / CLIC tras evento ON / AL");
        }
        if (node) {
            node->right = parse_logic_block();
        }
        if (check(TOK_FIN)) advance_token();
        else expect(TOK_END, "END o FIN para cerrar bloque de boton");
        return node;
    }
    return NULL;
}

static ASTNode* parse_file_statement(void) {
    if (check(TOK_WRITE)) {
        advance_token();
        expect(TOK_FILE, "FILE / ARCHIVO tras WRITE / ESCRIBIR");
        return create_node(NODE_WRITE, "WRITE");
    }
    return NULL;
}

static ASTNode* parse_native_block(void) {
    advance_token(); // Consume NATIVO
    size_t capacity = 1024;
    size_t length = 0;
    char* buffer = (char*)malloc(capacity);
    if (!buffer) {
        parser_errors++;
        return NULL;
    }
    buffer[0] = '\0';

    while (!check(TOK_FIN) && !check(TOK_END) && !check(TOK_EOF)) {
        size_t token_len = strlen(current_token.text);
        if (length + token_len + 2 >= capacity) {
            size_t new_cap = capacity * 2;
            if (new_cap < capacity + token_len + 2) {
                new_cap = capacity + token_len + 256;
            }
            char* new_buf = (char*)realloc(buffer, new_cap);
            if (!new_buf) {
                free(buffer);
                parser_errors++;
                return NULL;
            }
            buffer = new_buf;
            capacity = new_cap;
        }
        memcpy(buffer + length, current_token.text, token_len);
        length += token_len;
        buffer[length++] = ' ';
        buffer[length] = '\0';
        advance_token();
    }

    if (check(TOK_FIN)) advance_token();
    else expect(TOK_END, "FIN / END al cerrar bloque NATIVO");

    ASTNode* node = create_node(NODE_NATIVO, buffer);
    free(buffer);
    return node;
}

static ASTNode* parse_statement(void) {
    if (check(TOK_SHOW)) {
        advance_token();
        ASTNode* node = create_node(NODE_SHOW, "SHOW");
        if (node) {
            node->left = parse_expression();
        }
        return node;
    } else if (check(TOK_IF)) {
        advance_token();
        ASTNode* node = create_node(NODE_IF, "IF");
        if (node) {
            node->left = parse_expression();
        }
        expect(TOK_THEN, "THEN / ENTONCES tras condicion IF / SI");
        if (node) {
            node->right = parse_logic_block();
        }
        if (check(TOK_FIN)) advance_token();
        else expect(TOK_END, "FIN / END tras bloque condicional");
        return node;
    } else if (check(TOK_WINDOW) || check(TOK_BUTTON)) {
        return parse_ui_element();
    } else if (check(TOK_CAMPO) || check(TOK_FIELD)) {
        advance_token();
        char field_name[NC_MAX_TOKEN_LEN] = {0};
        if (check(TOK_IDENTIFIER)) {
            strncpy(field_name, current_token.text, sizeof(field_name) - 1);
            advance_token();
        }
        return create_node(NODE_FIELD, field_name);
    } else if (check(TOK_WRITE)) {
        return parse_file_statement();
    } else if (check(TOK_NATIVO)) {
        return parse_native_block();
    } else if (check(TOK_NITRO)) {
        advance_token();
        return create_node(NODE_NITRO, "NITRO");
    }
    return NULL;
}

static ASTNode* parse_logic_block(void) {
    ASTNode* head = NULL;
    ASTNode* current = NULL;
    while (!check(TOK_END) && !check(TOK_FIN) && !check(TOK_END_IF) && !check(TOK_EOF)) {
        ASTNode* stmt = parse_statement();
        if (stmt) {
            if (!head) {
                head = stmt;
            } else {
                current->next = stmt;
            }
            current = stmt;
        } else {
            break;
        }
    }
    return head;
}

ASTNode* nc_parse(const char* source) {
    parser_errors = 0;
    nc_lexer_init(source);
    current_token = nc_lexer_next_token();

    if (check(TOK_PROGRAM)) {
        advance_token();
        ASTNode* prog = create_node(NODE_PROGRAM, current_token.text);
        expect(TOK_IDENTIFIER, "Nombre de programa tras PROGRAMA / PROGRAM");

        while (!check(TOK_EOF)) {
            if (check(TOK_DATA)) {
                advance_token();
                while (!check(TOK_LOGIC) && !check(TOK_EOF)) {
                    advance_token();
                }
            } else if (check(TOK_LOGIC)) {
                advance_token();
                if (prog) {
                    prog->right = parse_logic_block();
                }
            } else if (check(TOK_END) || check(TOK_FIN)) {
                advance_token();
                break;
            } else if (check(TOK_DOT)) {
                advance_token();
            } else {
                advance_token();
            }
        }

        if (parser_errors > 0) {
            nc_free_ast(prog);
            return NULL;
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
