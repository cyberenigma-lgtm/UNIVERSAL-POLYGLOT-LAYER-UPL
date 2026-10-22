#include "upl_ast.h"
#include <stdio.h>
#include <string.h>

static UplAstNode* alloc_node(UPL_Arena *arena, UplAstKind kind, size_t line, size_t col) {
    UplAstNode *node = (UplAstNode*)upl_arena_alloc(arena, sizeof(UplAstNode));
    if (!node) return NULL;
    memset(node, 0, sizeof(UplAstNode));
    node->kind = kind;
    node->line = line;
    node->column = col;
    return node;
}

UplAstNode* upl_ast_new_literal_int(UPL_Arena *arena, int64_t val, size_t line, size_t col) {
    UplAstNode *n = alloc_node(arena, AST_LITERAL_INT, line, col);
    if (n) n->as.lit_int.val = val;
    return n;
}

UplAstNode* upl_ast_new_literal_float(UPL_Arena *arena, double val, size_t line, size_t col) {
    UplAstNode *n = alloc_node(arena, AST_LITERAL_FLOAT, line, col);
    if (n) n->as.lit_float.val = val;
    return n;
}

UplAstNode* upl_ast_new_literal_str(UPL_Arena *arena, const char *val, size_t line, size_t col) {
    UplAstNode *n = alloc_node(arena, AST_LITERAL_STRING, line, col);
    if (n) n->as.lit_str.val = upl_arena_strdup(arena, val);
    return n;
}

UplAstNode* upl_ast_new_literal_bool(UPL_Arena *arena, bool val, size_t line, size_t col) {
    UplAstNode *n = alloc_node(arena, AST_LITERAL_BOOL, line, col);
    if (n) n->as.lit_bool.val = val;
    return n;
}

UplAstNode* upl_ast_new_ident(UPL_Arena *arena, const char *name, size_t line, size_t col) {
    UplAstNode *n = alloc_node(arena, AST_IDENTIFIER, line, col);
    if (n) n->as.ident.name = upl_arena_strdup(arena, name);
    return n;
}

UplAstNode* upl_ast_new_type_primitive(UPL_Arena *arena, UplPrimitiveType prim, size_t line, size_t col) {
    UplAstNode *n = alloc_node(arena, AST_TYPE_PRIMITIVE, line, col);
    if (n) n->as.type_spec.prim = prim;
    return n;
}

UplAstNode* upl_ast_new_type_comp3(UPL_Arena *arena, int precision, int scale, size_t line, size_t col) {
    UplAstNode *n = alloc_node(arena, AST_TYPE_COMP3, line, col);
    if (n) {
        n->as.type_spec.comp3_precision = precision;
        n->as.type_spec.comp3_scale = scale;
    }
    return n;
}

UplAstNode* upl_ast_new_type_pointer(UPL_Arena *arena, UplAstNode *base_type, size_t line, size_t col) {
    UplAstNode *n = alloc_node(arena, AST_TYPE_POINTER, line, col);
    if (n) n->as.type_spec.base_or_elem_type = base_type;
    return n;
}

UplAstNode* upl_ast_new_type_array(UPL_Arena *arena, UplAstNode *elem_type, size_t len, size_t line, size_t col) {
    UplAstNode *n = alloc_node(arena, AST_TYPE_ARRAY, line, col);
    if (n) {
        n->as.type_spec.base_or_elem_type = elem_type;
        n->as.type_spec.array_len = len;
    }
    return n;
}

UplAstNode* upl_ast_new_type_custom(UPL_Arena *arena, const char *name, size_t line, size_t col) {
    UplAstNode *n = alloc_node(arena, AST_TYPE_CUSTOM, line, col);
    if (n) n->as.type_spec.custom_name = upl_arena_strdup(arena, name);
    return n;
}

UplAstNode* upl_ast_new_binary(UPL_Arena *arena, TokenType op, UplAstNode *left, UplAstNode *right, size_t line, size_t col) {
    UplAstNode *n = alloc_node(arena, (op == TOKEN_OP_ASSIGN) ? AST_EXPR_ASSIGN : AST_EXPR_BINARY, line, col);
    if (n) {
        n->as.binary_expr.op = op;
        n->as.binary_expr.left = left;
        n->as.binary_expr.right = right;
    }
    return n;
}

UplAstNode* upl_ast_new_unary(UPL_Arena *arena, TokenType op, UplAstNode *operand, size_t line, size_t col) {
    UplAstNode *n = alloc_node(arena, AST_EXPR_UNARY, line, col);
    if (n) {
        n->as.unary_expr.op = op;
        n->as.unary_expr.operand = operand;
    }
    return n;
}

UplAstNode* upl_ast_new_call(UPL_Arena *arena, UplAstNode *callee, UplAstList *args, size_t line, size_t col) {
    UplAstNode *n = alloc_node(arena, AST_EXPR_CALL, line, col);
    if (n) {
        n->as.call_expr.callee = callee;
        n->as.call_expr.args = args;
    }
    return n;
}

UplAstNode* upl_ast_new_member(UPL_Arena *arena, UplAstNode *object, const char *member, size_t line, size_t col) {
    UplAstNode *n = alloc_node(arena, AST_EXPR_MEMBER, line, col);
    if (n) {
        n->as.member_expr.object = object;
        n->as.member_expr.member = upl_arena_strdup(arena, member);
    }
    return n;
}

UplAstNode* upl_ast_new_index(UPL_Arena *arena, UplAstNode *array, UplAstNode *index, size_t line, size_t col) {
    UplAstNode *n = alloc_node(arena, AST_EXPR_INDEX, line, col);
    if (n) {
        n->as.index_expr.array = array;
        n->as.index_expr.index = index;
    }
    return n;
}

UplAstNode* upl_ast_new_cast(UPL_Arena *arena, UplAstNode *expr, UplAstNode *target_type, size_t line, size_t col) {
    UplAstNode *n = alloc_node(arena, AST_EXPR_CAST, line, col);
    if (n) {
        n->as.cast_expr.expr = expr;
        n->as.cast_expr.target_type = target_type;
    }
    return n;
}

UplAstNode* upl_ast_new_stmt_let(UPL_Arena *arena, const char *name, UplAstNode *type, UplAstNode *init_expr, bool is_mut, size_t line, size_t col) {
    UplAstNode *n = alloc_node(arena, AST_STMT_LET, line, col);
    if (n) {
        n->as.stmt_let.name = upl_arena_strdup(arena, name);
        n->as.stmt_let.type = type;
        n->as.stmt_let.init_expr = init_expr;
        n->as.stmt_let.is_mut = is_mut;
    }
    return n;
}

UplAstNode* upl_ast_new_stmt_if(UPL_Arena *arena, UplAstNode *cond, UplAstNode *then_block, UplAstNode *else_branch, size_t line, size_t col) {
    UplAstNode *n = alloc_node(arena, AST_STMT_IF, line, col);
    if (n) {
        n->as.stmt_if.cond = cond;
        n->as.stmt_if.then_block = then_block;
        n->as.stmt_if.else_branch = else_branch;
    }
    return n;
}

UplAstNode* upl_ast_new_stmt_while(UPL_Arena *arena, UplAstNode *cond, UplAstNode *body, size_t line, size_t col) {
    UplAstNode *n = alloc_node(arena, AST_STMT_WHILE, line, col);
    if (n) {
        n->as.stmt_while.cond = cond;
        n->as.stmt_while.body = body;
    }
    return n;
}

UplAstNode* upl_ast_new_stmt_return(UPL_Arena *arena, UplAstNode *expr, size_t line, size_t col) {
    UplAstNode *n = alloc_node(arena, AST_STMT_RETURN, line, col);
    if (n) n->as.stmt_return.expr = expr;
    return n;
}

UplAstNode* upl_ast_new_stmt_expr(UPL_Arena *arena, UplAstNode *expr, size_t line, size_t col) {
    UplAstNode *n = alloc_node(arena, AST_STMT_EXPR, line, col);
    if (n) n->as.unary_expr.operand = expr; // Guardado en operand para simplicidad
    return n;
}

UplAstNode* upl_ast_new_block(UPL_Arena *arena, UplAstList *stmts, size_t line, size_t col) {
    UplAstNode *n = alloc_node(arena, AST_STMT_BLOCK, line, col);
    if (n) n->as.block.stmts = stmts;
    return n;
}

UplAstNode* upl_ast_new_raw_block(UPL_Arena *arena, UplAstKind kind, const char *raw_code, size_t line, size_t col) {
    UplAstNode *n = alloc_node(arena, kind, line, col);
    if (n) n->as.raw_block.raw_code = upl_arena_strdup(arena, raw_code);
    return n;
}

UplAstNode* upl_ast_new_stmt_break(UPL_Arena *arena, size_t line, size_t col) {
    return alloc_node(arena, AST_STMT_BREAK, line, col);
}

UplAstNode* upl_ast_new_stmt_continue(UPL_Arena *arena, size_t line, size_t col) {
    return alloc_node(arena, AST_STMT_CONTINUE, line, col);
}

UplAstNode* upl_ast_new_decl_param(UPL_Arena *arena, const char *name, UplAstNode *type, size_t line, size_t col) {
    UplAstNode *n = alloc_node(arena, AST_DECL_PARAM, line, col);
    if (n) {
        n->as.decl_param.name = upl_arena_strdup(arena, name);
        n->as.decl_param.type = type;
    }
    return n;
}

UplAstNode* upl_ast_new_decl_fn(UPL_Arena *arena, const char *name, UplAstList *params, UplAstNode *ret_type, UplAstNode *body, bool is_pub, size_t line, size_t col) {
    UplAstNode *n = alloc_node(arena, AST_DECL_FN, line, col);
    if (n) {
        n->as.decl_fn.name = upl_arena_strdup(arena, name);
        n->as.decl_fn.params = params;
        n->as.decl_fn.return_type = ret_type;
        n->as.decl_fn.body = body;
        n->as.decl_fn.is_pub = is_pub;
    }
    return n;
}

UplAstNode* upl_ast_new_decl_field(UPL_Arena *arena, const char *name, UplAstNode *type, size_t line, size_t col) {
    UplAstNode *n = alloc_node(arena, AST_DECL_FIELD, line, col);
    if (n) {
        n->as.decl_field.name = upl_arena_strdup(arena, name);
        n->as.decl_field.type = type;
    }
    return n;
}

UplAstNode* upl_ast_new_decl_struct(UPL_Arena *arena, const char *name, UplAstList *fields, size_t line, size_t col) {
    UplAstNode *n = alloc_node(arena, AST_DECL_STRUCT, line, col);
    if (n) {
        n->as.decl_struct.name = upl_arena_strdup(arena, name);
        n->as.decl_struct.fields = fields;
    }
    return n;
}

UplAstNode* upl_ast_new_decl_module(UPL_Arena *arena, const char *name, size_t line, size_t col) {
    UplAstNode *n = alloc_node(arena, AST_DECL_MODULE, line, col);
    if (n) n->as.decl_module.name = upl_arena_strdup(arena, name);
    return n;
}

UplAstNode* upl_ast_new_decl_import(UPL_Arena *arena, const char *path, size_t line, size_t col) {
    UplAstNode *n = alloc_node(arena, AST_DECL_IMPORT, line, col);
    if (n) n->as.decl_import.path = upl_arena_strdup(arena, path);
    return n;
}

UplAstNode* upl_ast_new_program(UPL_Arena *arena, UplAstList *items, size_t line, size_t col) {
    UplAstNode *n = alloc_node(arena, AST_PROGRAM, line, col);
    if (n) n->as.program.items = items;
    return n;
}

void upl_ast_list_append(UPL_Arena *arena, UplAstList **head, UplAstNode *node) {
    if (!head || !node) return;
    UplAstList *elem = (UplAstList*)upl_arena_alloc(arena, sizeof(UplAstList));
    if (!elem) return;
    elem->node = node;
    elem->next = NULL;

    if (*head == NULL) {
        *head = elem;
    } else {
        UplAstList *curr = *head;
        while (curr->next) curr = curr->next;
        curr->next = elem;
    }
}

size_t upl_ast_list_length(const UplAstList *head) {
    size_t count = 0;
    while (head) {
        count++;
        head = head->next;
    }
    return count;
}

const char* upl_ast_kind_name(UplAstKind kind) {
    switch (kind) {
        case AST_LITERAL_INT:    return "LiteralInt";
        case AST_LITERAL_FLOAT:  return "LiteralFloat";
        case AST_LITERAL_STRING: return "LiteralString";
        case AST_LITERAL_BOOL:   return "LiteralBool";
        case AST_IDENTIFIER:     return "Identifier";
        case AST_TYPE_PRIMITIVE: return "TypePrimitive";
        case AST_TYPE_COMP3:     return "TypeComp3";
        case AST_TYPE_POINTER:   return "TypePointer";
        case AST_TYPE_ARRAY:     return "TypeArray";
        case AST_TYPE_CUSTOM:    return "TypeCustom";
        case AST_EXPR_BINARY:    return "ExprBinary";
        case AST_EXPR_UNARY:     return "ExprUnary";
        case AST_EXPR_CALL:      return "ExprCall";
        case AST_EXPR_INDEX:     return "ExprIndex";
        case AST_EXPR_MEMBER:    return "ExprMember";
        case AST_EXPR_CAST:      return "ExprCast";
        case AST_EXPR_ASSIGN:    return "ExprAssign";
        case AST_STMT_LET:       return "StmtLet";
        case AST_STMT_RETURN:    return "StmtReturn";
        case AST_STMT_IF:        return "StmtIf";
        case AST_STMT_WHILE:     return "StmtWhile";
        case AST_STMT_EXPR:      return "StmtExpr";
        case AST_STMT_BLOCK:     return "StmtBlock";
        case AST_STMT_EMBED:     return "StmtEmbed";
        case AST_STMT_ASM:       return "StmtAsm";
        case AST_STMT_BREAK:     return "StmtBreak";
        case AST_STMT_CONTINUE:  return "StmtContinue";
        case AST_DECL_PARAM:     return "DeclParam";
        case AST_DECL_FN:        return "DeclFn";
        case AST_DECL_FIELD:     return "DeclField";
        case AST_DECL_STRUCT:    return "DeclStruct";
        case AST_DECL_MODULE:    return "DeclModule";
        case AST_DECL_IMPORT:    return "DeclImport";
        case AST_PROGRAM:        return "Program";
        default:                 return "Unknown";
    }
}

static void print_indent(int indent) {
    for (int i = 0; i < indent; ++i) printf("  ");
}

void upl_ast_dump(const UplAstNode *node, int indent) {
    if (!node) {
        print_indent(indent);
        printf("(null)\n");
        return;
    }

    print_indent(indent);
    printf("%s [L%zu:C%zu]", upl_ast_kind_name(node->kind), node->line, node->column);

    switch (node->kind) {
        case AST_LITERAL_INT:
            printf(": %lld\n", (long long)node->as.lit_int.val);
            break;
        case AST_LITERAL_FLOAT:
            printf(": %f\n", node->as.lit_float.val);
            break;
        case AST_LITERAL_STRING:
            printf(": \"%s\"\n", node->as.lit_str.val);
            break;
        case AST_LITERAL_BOOL:
            printf(": %s\n", node->as.lit_bool.val ? "true" : "false");
            break;
        case AST_IDENTIFIER:
            printf(": %s\n", node->as.ident.name);
            break;
        case AST_TYPE_COMP3:
            printf(" (%d, %d)\n", node->as.type_spec.comp3_precision, node->as.type_spec.comp3_scale);
            break;
        case AST_DECL_FN:
            printf(": %s (pub=%d)\n", node->as.decl_fn.name, node->as.decl_fn.is_pub);
            if (node->as.decl_fn.body) upl_ast_dump(node->as.decl_fn.body, indent + 1);
            break;
        case AST_STMT_BLOCK:
            printf("\n");
            for (UplAstList *l = node->as.block.stmts; l != NULL; l = l->next) {
                upl_ast_dump(l->node, indent + 1);
            }
            break;
        case AST_STMT_LET:
            printf(": %s (mut=%d)\n", node->as.stmt_let.name, node->as.stmt_let.is_mut);
            if (node->as.stmt_let.init_expr) upl_ast_dump(node->as.stmt_let.init_expr, indent + 1);
            break;
        case AST_EXPR_BINARY:
        case AST_EXPR_ASSIGN:
            printf(" [op=%s]\n", token_type_name(node->as.binary_expr.op));
            upl_ast_dump(node->as.binary_expr.left, indent + 1);
            upl_ast_dump(node->as.binary_expr.right, indent + 1);
            break;
        default:
            printf("\n");
            break;
    }
}
