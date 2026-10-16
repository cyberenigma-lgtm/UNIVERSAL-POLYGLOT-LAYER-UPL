#ifndef NC_PARSER_H
#define NC_PARSER_H

#include "nc_lexer.h"

typedef enum {
    NODE_PROGRAM, NODE_DATA, NODE_LOGIC, NODE_DEFINE, NODE_IDENTIFIER,
    NODE_STRING_LITERAL, NODE_NUMBER_LITERAL, NODE_SHOW, NODE_IF, NODE_BLOCK, NODE_ELSE,
    NODE_WINDOW, NODE_BUTTON, NODE_FIELD, NODE_OPEN, NODE_READ,
    NODE_WRITE, NODE_CLOSE, NODE_MODULE_DECL, NODE_BUS_SEND,
    NODE_BUS_RECV, NODE_NITRO, NODE_NATIVO, NODE_IMPORT, NODE_COMPRESSED_DATA,
    NODE_ERROR
} ASTNodeType;

typedef struct ASTNode {
    ASTNodeType type;
    char text[NC_MAX_TOKEN_LEN];
    struct ASTNode* left;
    struct ASTNode* right;
    struct ASTNode* next;
} ASTNode;

ASTNode* nc_parse(const char* source);
void nc_free_ast(ASTNode* node);
int nc_parser_get_error_count(void);

#endif
