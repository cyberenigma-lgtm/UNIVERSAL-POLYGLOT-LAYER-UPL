#ifndef UPL_AST_H
#define UPL_AST_H

#include "lexer.h"
#include "upl_arena.h"
#include <stdint.h>
#include <stdbool.h>

/**
 * Árbol de Sintaxis Abstracta (AST) para UPL
 * 
 * Diseñado bajo principios estándar de compiladores (Dragon Book):
 * - Representación canónica agnóstica del idioma de origen (EN / ES).
 * - Asignación basada en Memory Arena (O(1), 0 fugas).
 * - Coordenadas de origen precisas para diagnósticos semánticos.
 */

typedef enum {
    // Tipos de nodos primarios y literales
    AST_LITERAL_INT,
    AST_LITERAL_FLOAT,
    AST_LITERAL_STRING,
    AST_LITERAL_BOOL,
    AST_IDENTIFIER,

    // Especificación de tipos estáticos
    AST_TYPE_PRIMITIVE,
    AST_TYPE_COMP3,
    AST_TYPE_POINTER,
    AST_TYPE_ARRAY,
    AST_TYPE_CUSTOM,

    // Expresiones
    AST_EXPR_BINARY,
    AST_EXPR_UNARY,
    AST_EXPR_CALL,
    AST_EXPR_INDEX,
    AST_EXPR_MEMBER,
    AST_EXPR_CAST,
    AST_EXPR_ASSIGN,

    // Sentencias
    AST_STMT_LET,
    AST_STMT_RETURN,
    AST_STMT_IF,
    AST_STMT_WHILE,
    AST_STMT_EXPR,
    AST_STMT_BLOCK,
    AST_STMT_EMBED,
    AST_STMT_ASM,
    AST_STMT_BREAK,
    AST_STMT_CONTINUE,

    // Declaraciones de alto nivel
    AST_DECL_PARAM,
    AST_DECL_FN,
    AST_DECL_FIELD,
    AST_DECL_STRUCT,
    AST_DECL_MODULE,
    AST_DECL_IMPORT,
    AST_PROGRAM
} UplAstKind;

typedef enum {
    UPL_TYPE_I8,
    UPL_TYPE_I16,
    UPL_TYPE_I32,
    UPL_TYPE_I64,
    UPL_TYPE_U8,
    UPL_TYPE_U16,
    UPL_TYPE_U32,
    UPL_TYPE_U64,
    UPL_TYPE_USIZE,
    UPL_TYPE_F32,
    UPL_TYPE_F64,
    UPL_TYPE_BOOL,
    UPL_TYPE_STR,
    UPL_TYPE_VOID
} UplPrimitiveType;

typedef struct UplAstNode UplAstNode;
typedef struct UplAstList UplAstList;

struct UplAstList {
    UplAstNode *node;
    UplAstList *next;
};

struct UplAstNode {
    UplAstKind kind;
    size_t line;
    size_t column;
    union {
        // Literales
        struct { int64_t val; } lit_int;
        struct { double val; } lit_float;
        struct { const char *val; } lit_str;
        struct { bool val; } lit_bool;
        struct { const char *name; } ident;

        // Tipos
        struct {
            UplPrimitiveType prim;
            int comp3_precision;
            int comp3_scale;
            UplAstNode *base_or_elem_type;
            size_t array_len;
            const char *custom_name;
        } type_spec;

        // Expresión Binaria / Asignación
        struct {
            TokenType op;
            UplAstNode *left;
            UplAstNode *right;
        } binary_expr;

        // Expresión Unaria
        struct {
            TokenType op;
            UplAstNode *operand;
        } unary_expr;

        // Llamada a función: callee(args...)
        struct {
            UplAstNode *callee;
            UplAstList *args;
        } call_expr;

        // Acceso a miembro: obj.campo
        struct {
            UplAstNode *object;
            const char *member;
        } member_expr;

        // Indexación: arr[idx]
        struct {
            UplAstNode *array;
            UplAstNode *index;
        } index_expr;

        // Cast: expr as type
        struct {
            UplAstNode *expr;
            UplAstNode *target_type;
        } cast_expr;

        // Sentencia Let (sea / let)
        struct {
            const char *name;
            UplAstNode *type;
            UplAstNode *init_expr;
            bool is_mut;
        } stmt_let;

        // Sentencia If
        struct {
            UplAstNode *cond;
            UplAstNode *then_block;
            UplAstNode *else_branch;
        } stmt_if;

        // Sentencia While
        struct {
            UplAstNode *cond;
            UplAstNode *body;
        } stmt_while;

        // Sentencia Return
        struct {
            UplAstNode *expr;
        } stmt_return;

        // Bloque de sentencias
        struct {
            UplAstList *stmts;
        } block;

        // Código Crudo (embed / asm)
        struct {
            const char *raw_code;
        } raw_block;

        // Declaración de función
        struct {
            const char *name;
            UplAstList *params;
            UplAstNode *return_type;
            UplAstNode *body;
            bool is_pub;
        } decl_fn;

        // Parámetro formal
        struct {
            const char *name;
            UplAstNode *type;
        } decl_param;

        // Declaración de struct
        struct {
            const char *name;
            UplAstList *fields;
        } decl_struct;

        // Campo de struct
        struct {
            const char *name;
            UplAstNode *type;
        } decl_field;

        // Módulo e importación
        struct { const char *name; } decl_module;
        struct { const char *path; } decl_import;

        // Programa completo
        struct {
            UplAstList *items;
        } program;
    } as;
};

/* Constructores de Nodos AST (asignados en UPL_Arena) */
UplAstNode* upl_ast_new_literal_int(UPL_Arena *arena, int64_t val, size_t line, size_t col);
UplAstNode* upl_ast_new_literal_float(UPL_Arena *arena, double val, size_t line, size_t col);
UplAstNode* upl_ast_new_literal_str(UPL_Arena *arena, const char *val, size_t line, size_t col);
UplAstNode* upl_ast_new_literal_bool(UPL_Arena *arena, bool val, size_t line, size_t col);
UplAstNode* upl_ast_new_ident(UPL_Arena *arena, const char *name, size_t line, size_t col);

UplAstNode* upl_ast_new_type_primitive(UPL_Arena *arena, UplPrimitiveType prim, size_t line, size_t col);
UplAstNode* upl_ast_new_type_comp3(UPL_Arena *arena, int precision, int scale, size_t line, size_t col);
UplAstNode* upl_ast_new_type_pointer(UPL_Arena *arena, UplAstNode *base_type, size_t line, size_t col);
UplAstNode* upl_ast_new_type_array(UPL_Arena *arena, UplAstNode *elem_type, size_t len, size_t line, size_t col);
UplAstNode* upl_ast_new_type_custom(UPL_Arena *arena, const char *name, size_t line, size_t col);

UplAstNode* upl_ast_new_binary(UPL_Arena *arena, TokenType op, UplAstNode *left, UplAstNode *right, size_t line, size_t col);
UplAstNode* upl_ast_new_unary(UPL_Arena *arena, TokenType op, UplAstNode *operand, size_t line, size_t col);
UplAstNode* upl_ast_new_call(UPL_Arena *arena, UplAstNode *callee, UplAstList *args, size_t line, size_t col);
UplAstNode* upl_ast_new_member(UPL_Arena *arena, UplAstNode *object, const char *member, size_t line, size_t col);
UplAstNode* upl_ast_new_index(UPL_Arena *arena, UplAstNode *array, UplAstNode *index, size_t line, size_t col);
UplAstNode* upl_ast_new_cast(UPL_Arena *arena, UplAstNode *expr, UplAstNode *target_type, size_t line, size_t col);

UplAstNode* upl_ast_new_stmt_let(UPL_Arena *arena, const char *name, UplAstNode *type, UplAstNode *init_expr, bool is_mut, size_t line, size_t col);
UplAstNode* upl_ast_new_stmt_if(UPL_Arena *arena, UplAstNode *cond, UplAstNode *then_block, UplAstNode *else_branch, size_t line, size_t col);
UplAstNode* upl_ast_new_stmt_while(UPL_Arena *arena, UplAstNode *cond, UplAstNode *body, size_t line, size_t col);
UplAstNode* upl_ast_new_stmt_return(UPL_Arena *arena, UplAstNode *expr, size_t line, size_t col);
UplAstNode* upl_ast_new_stmt_expr(UPL_Arena *arena, UplAstNode *expr, size_t line, size_t col);
UplAstNode* upl_ast_new_block(UPL_Arena *arena, UplAstList *stmts, size_t line, size_t col);
UplAstNode* upl_ast_new_raw_block(UPL_Arena *arena, UplAstKind kind, const char *raw_code, size_t line, size_t col);
UplAstNode* upl_ast_new_stmt_break(UPL_Arena *arena, size_t line, size_t col);
UplAstNode* upl_ast_new_stmt_continue(UPL_Arena *arena, size_t line, size_t col);

UplAstNode* upl_ast_new_decl_param(UPL_Arena *arena, const char *name, UplAstNode *type, size_t line, size_t col);
UplAstNode* upl_ast_new_decl_fn(UPL_Arena *arena, const char *name, UplAstList *params, UplAstNode *ret_type, UplAstNode *body, bool is_pub, size_t line, size_t col);
UplAstNode* upl_ast_new_decl_field(UPL_Arena *arena, const char *name, UplAstNode *type, size_t line, size_t col);
UplAstNode* upl_ast_new_decl_struct(UPL_Arena *arena, const char *name, UplAstList *fields, size_t line, size_t col);
UplAstNode* upl_ast_new_decl_module(UPL_Arena *arena, const char *name, size_t line, size_t col);
UplAstNode* upl_ast_new_decl_import(UPL_Arena *arena, const char *path, size_t line, size_t col);
UplAstNode* upl_ast_new_program(UPL_Arena *arena, UplAstList *items, size_t line, size_t col);

/* Manejo de listas de nodos AST */
void upl_ast_list_append(UPL_Arena *arena, UplAstList **head, UplAstNode *node);
size_t upl_ast_list_length(const UplAstList *head);

/* Diagnósticos e inspección de AST */
void upl_ast_dump(const UplAstNode *node, int indent);
const char* upl_ast_kind_name(UplAstKind kind);

#endif /* UPL_AST_H */
