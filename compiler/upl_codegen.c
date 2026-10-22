#include "upl_codegen.h"
#include <stdio.h>
#include <string.h>

static void emit_indent(UplCodeGen *gen) {
    for (int i = 0; i < gen->indent_level; ++i) {
        fprintf(gen->out, "    ");
    }
}

static void emit_type(UplCodeGen *gen, const UplAstNode *type) {
    if (!type) {
        fprintf(gen->out, "void");
        return;
    }

    switch (type->kind) {
        case AST_TYPE_PRIMITIVE:
            switch (type->as.type_spec.prim) {
                case UPL_TYPE_I8:    fprintf(gen->out, "int8_t"); break;
                case UPL_TYPE_I16:   fprintf(gen->out, "int16_t"); break;
                case UPL_TYPE_I32:   fprintf(gen->out, "int32_t"); break;
                case UPL_TYPE_I64:   fprintf(gen->out, "int64_t"); break;
                case UPL_TYPE_U8:    fprintf(gen->out, "uint8_t"); break;
                case UPL_TYPE_U16:   fprintf(gen->out, "uint16_t"); break;
                case UPL_TYPE_U32:   fprintf(gen->out, "uint32_t"); break;
                case UPL_TYPE_U64:   fprintf(gen->out, "uint64_t"); break;
                case UPL_TYPE_USIZE: fprintf(gen->out, "size_t"); break;
                case UPL_TYPE_F32:   fprintf(gen->out, "float"); break;
                case UPL_TYPE_F64:   fprintf(gen->out, "double"); break;
                case UPL_TYPE_BOOL:  fprintf(gen->out, "bool"); break;
                case UPL_TYPE_STR:   fprintf(gen->out, "const char*"); break;
                case UPL_TYPE_VOID:  fprintf(gen->out, "void"); break;
            }
            break;
        case AST_TYPE_COMP3:
            fprintf(gen->out, "uint8_t* /* comp3 */");
            break;
        case AST_TYPE_POINTER:
            emit_type(gen, type->as.type_spec.base_or_elem_type);
            fprintf(gen->out, "*");
            break;
        case AST_TYPE_ARRAY:
            emit_type(gen, type->as.type_spec.base_or_elem_type);
            fprintf(gen->out, "*");
            break;
        case AST_TYPE_CUSTOM:
            fprintf(gen->out, "%s", type->as.type_spec.custom_name);
            break;
        default:
            fprintf(gen->out, "int64_t");
            break;
    }
}

static void emit_expression(UplCodeGen *gen, const UplAstNode *node);

static void emit_binary(UplCodeGen *gen, const UplAstNode *node) {
    fprintf(gen->out, "(");
    emit_expression(gen, node->as.binary_expr.left);

    const char *op_str = "+";
    switch (node->as.binary_expr.op) {
        case TOKEN_OP_ADD:           op_str = " + "; break;
        case TOKEN_OP_SUB:           op_str = " - "; break;
        case TOKEN_OP_MUL:           op_str = " * "; break;
        case TOKEN_OP_DIV:           op_str = " / "; break;
        case TOKEN_OP_MOD:           op_str = " % "; break;
        case TOKEN_OP_EQUAL:         op_str = " == "; break;
        case TOKEN_OP_NOT_EQUAL:     op_str = " != "; break;
        case TOKEN_OP_LESS:          op_str = " < "; break;
        case TOKEN_OP_GREATER:       op_str = " > "; break;
        case TOKEN_OP_LESS_EQUAL:    op_str = " <= "; break;
        case TOKEN_OP_GREATER_EQUAL: op_str = " >= "; break;
        case TOKEN_OP_AND:           op_str = " && "; break;
        case TOKEN_OP_OR:            op_str = " || "; break;
        case TOKEN_OP_XOR:           op_str = " ^ "; break;
        case TOKEN_OP_BIT_AND:       op_str = " & "; break;
        case TOKEN_OP_BIT_OR:        op_str = " | "; break;
        case TOKEN_OP_SHL:           op_str = " << "; break;
        case TOKEN_OP_SHR:           op_str = " >> "; break;
        case TOKEN_OP_ASSIGN:        op_str = " = "; break;
        case TOKEN_OP_ADD_ASSIGN:    op_str = " += "; break;
        case TOKEN_OP_SUB_ASSIGN:    op_str = " -= "; break;
        case TOKEN_OP_MUL_ASSIGN:    op_str = " *= "; break;
        case TOKEN_OP_DIV_ASSIGN:    op_str = " /= "; break;
        case TOKEN_OP_MOD_ASSIGN:    op_str = " %= "; break;
        case TOKEN_OP_AND_ASSIGN:    op_str = " &= "; break;
        case TOKEN_OP_OR_ASSIGN:     op_str = " |= "; break;
        case TOKEN_OP_XOR_ASSIGN:    op_str = " ^= "; break;
        case TOKEN_OP_SHL_ASSIGN:    op_str = " <<= "; break;
        case TOKEN_OP_SHR_ASSIGN:    op_str = " >>= "; break;
        default:                     op_str = " + "; break;
    }
    fprintf(gen->out, "%s", op_str);
    emit_expression(gen, node->as.binary_expr.right);
    fprintf(gen->out, ")");
}

static void upl_codegen_add_pointer_var(UplCodeGen *gen, const char *name) {
    if (!name || gen->pointer_var_count >= 128) return;
    for (int i = 0; i < gen->pointer_var_count; ++i) {
        if (strcmp(gen->pointer_vars[i], name) == 0) return;
    }
    strncpy(gen->pointer_vars[gen->pointer_var_count++], name, 63);
}

static void upl_codegen_clear_pointer_vars(UplCodeGen *gen) {
    gen->pointer_var_count = 0;
}

static bool upl_codegen_is_pointer_var(UplCodeGen *gen, const char *name) {
    if (!name) return false;
    for (int i = 0; i < gen->pointer_var_count; ++i) {
        if (strcmp(gen->pointer_vars[i], name) == 0) return true;
    }
    return false;
}

static void emit_expression(UplCodeGen *gen, const UplAstNode *node) {
    if (!node) return;

    switch (node->kind) {
        case AST_LITERAL_INT:
            fprintf(gen->out, "%lldLL", (long long)node->as.lit_int.val);
            break;
        case AST_LITERAL_FLOAT:
            fprintf(gen->out, "%f", node->as.lit_float.val);
            break;
        case AST_LITERAL_STRING:
            fprintf(gen->out, "\"%s\"", node->as.lit_str.val);
            break;
        case AST_LITERAL_BOOL:
            fprintf(gen->out, "%s", node->as.lit_bool.val ? "true" : "false");
            break;
        case AST_IDENTIFIER:
            fprintf(gen->out, "%s", node->as.ident.name);
            break;
        case AST_EXPR_BINARY:
        case AST_EXPR_ASSIGN:
            emit_binary(gen, node);
            break;
        case AST_EXPR_UNARY:
            if (node->as.unary_expr.op == TOKEN_OP_SUB) fprintf(gen->out, "-");
            else if (node->as.unary_expr.op == TOKEN_OP_NOT) fprintf(gen->out, "!");
            else if (node->as.unary_expr.op == TOKEN_OP_MUL) fprintf(gen->out, "*");
            else if (node->as.unary_expr.op == TOKEN_OP_BIT_NOT) fprintf(gen->out, "~");
            else if (node->as.unary_expr.op == TOKEN_OP_BIT_AND) fprintf(gen->out, "&");
            emit_expression(gen, node->as.unary_expr.operand);
            break;
        case AST_EXPR_CALL:
            emit_expression(gen, node->as.call_expr.callee);
            fprintf(gen->out, "(");
            for (UplAstList *l = node->as.call_expr.args; l != NULL; l = l->next) {
                emit_expression(gen, l->node);
                if (l->next) fprintf(gen->out, ", ");
            }
            fprintf(gen->out, ")");
            break;
        case AST_EXPR_MEMBER: {
            emit_expression(gen, node->as.member_expr.object);
            bool is_ptr = false;
            if (node->as.member_expr.object && node->as.member_expr.object->kind == AST_IDENTIFIER) {
                is_ptr = upl_codegen_is_pointer_var(gen, node->as.member_expr.object->as.ident.name);
            }
            fprintf(gen->out, "%s%s", is_ptr ? "->" : ".", node->as.member_expr.member);
            break;
        }
        case AST_EXPR_INDEX:
            emit_expression(gen, node->as.index_expr.array);
            fprintf(gen->out, "[");
            emit_expression(gen, node->as.index_expr.index);
            fprintf(gen->out, "]");
            break;
        case AST_EXPR_CAST:
            fprintf(gen->out, "((");
            emit_type(gen, node->as.cast_expr.target_type);
            fprintf(gen->out, ")(");
            emit_expression(gen, node->as.cast_expr.expr);
            fprintf(gen->out, "))");
            break;
        default:
            break;
    }
}

static void emit_asm_block(UplCodeGen *gen, const char *raw_code) {
    if (!raw_code) return;
    fprintf(gen->out, "__asm__ volatile (\n");
    const char *p = raw_code;
    while (*p) {
        while (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n') p++;
        if (!*p) break;
        const char *end = p;
        while (*end && *end != '\r' && *end != '\n') end++;
        
        size_t len = (size_t)(end - p);
        char line[256];
        if (len >= sizeof(line)) len = sizeof(line) - 1;
        strncpy(line, p, len);
        line[len] = '\0';
        
        char *start_str = line;
        while (*start_str == ' ' || *start_str == '\t') start_str++;
        if (*start_str == '"') {
            start_str++;
            size_t slen = strlen(start_str);
            if (slen > 0 && start_str[slen - 1] == '"') {
                start_str[slen - 1] = '\0';
            }
        }
        
        if (strlen(start_str) > 0) {
            fprintf(gen->out, "    \"%s\\n\\t\"\n", start_str);
        }
        p = end;
    }
    fprintf(gen->out, ");\n");
}

static void emit_statement(UplCodeGen *gen, const UplAstNode *node);

static void emit_block(UplCodeGen *gen, const UplAstNode *node) {
    if (!node || node->kind != AST_STMT_BLOCK) return;
    fprintf(gen->out, "{\n");
    gen->indent_level++;
    for (UplAstList *l = node->as.block.stmts; l != NULL; l = l->next) {
        emit_statement(gen, l->node);
    }
    gen->indent_level--;
    emit_indent(gen);
    fprintf(gen->out, "}\n");
}

static void emit_statement(UplCodeGen *gen, const UplAstNode *node) {
    if (!node) return;
    emit_indent(gen);

    switch (node->kind) {
        case AST_STMT_LET:
            if (node->as.stmt_let.type) {
                if (node->as.stmt_let.type->kind == AST_TYPE_ARRAY) {
                    emit_type(gen, node->as.stmt_let.type->as.type_spec.base_or_elem_type);
                    fprintf(gen->out, " %s[%zu];\n", node->as.stmt_let.name, node->as.stmt_let.type->as.type_spec.array_len);
                    return;
                } else if (node->as.stmt_let.type->kind == AST_TYPE_COMP3) {
                    int prec = node->as.stmt_let.type->as.type_spec.comp3_precision;
                    size_t bytes = (size_t)((prec / 2) + 1);
                    fprintf(gen->out, "uint8_t %s[%zu];\n", node->as.stmt_let.name, bytes);
                    if (node->as.stmt_let.init_expr) {
                        emit_indent(gen);
                        fprintf(gen->out, "upl_comp3_from_str(%s, sizeof(%s), ", node->as.stmt_let.name, node->as.stmt_let.name);
                        emit_expression(gen, node->as.stmt_let.init_expr);
                        fprintf(gen->out, ", %d, %d);\n", prec, node->as.stmt_let.type->as.type_spec.comp3_scale);
                    }
                    return;
                } else if (node->as.stmt_let.type->kind == AST_TYPE_POINTER) {
                    upl_codegen_add_pointer_var(gen, node->as.stmt_let.name);
                    emit_type(gen, node->as.stmt_let.type);
                    if (!node->as.stmt_let.is_mut) {
                        fprintf(gen->out, " const");
                    }
                    fprintf(gen->out, " ");
                } else {
                    bool is_str = (node->as.stmt_let.type->kind == AST_TYPE_PRIMITIVE && node->as.stmt_let.type->as.type_spec.prim == UPL_TYPE_STR);
                    if (!node->as.stmt_let.is_mut && !is_str) {
                        fprintf(gen->out, "const ");
                    }
                    emit_type(gen, node->as.stmt_let.type);
                    fprintf(gen->out, " ");
                }
            } else {
                if (!node->as.stmt_let.is_mut) {
                    fprintf(gen->out, "const ");
                }
                fprintf(gen->out, "__auto_type ");
            }
            fprintf(gen->out, "%s", node->as.stmt_let.name);
            if (node->as.stmt_let.init_expr) {
                fprintf(gen->out, " = ");
                emit_expression(gen, node->as.stmt_let.init_expr);
            }
            fprintf(gen->out, ";\n");
            break;

        case AST_STMT_BREAK:
            fprintf(gen->out, "break;\n");
            break;

        case AST_STMT_CONTINUE:
            fprintf(gen->out, "continue;\n");
            break;

        case AST_STMT_RETURN:
            fprintf(gen->out, "return");
            if (node->as.stmt_return.expr) {
                fprintf(gen->out, " ");
                emit_expression(gen, node->as.stmt_return.expr);
            }
            fprintf(gen->out, ";\n");
            break;

        case AST_STMT_IF:
            fprintf(gen->out, "if (");
            emit_expression(gen, node->as.stmt_if.cond);
            fprintf(gen->out, ") ");
            emit_block(gen, node->as.stmt_if.then_block);
            if (node->as.stmt_if.else_branch) {
                emit_indent(gen);
                fprintf(gen->out, "else ");
                if (node->as.stmt_if.else_branch->kind == AST_STMT_IF) {
                    emit_statement(gen, node->as.stmt_if.else_branch);
                } else {
                    emit_block(gen, node->as.stmt_if.else_branch);
                }
            }
            break;

        case AST_STMT_WHILE:
            fprintf(gen->out, "while (");
            emit_expression(gen, node->as.stmt_while.cond);
            fprintf(gen->out, ") ");
            emit_block(gen, node->as.stmt_while.body);
            break;

        case AST_STMT_BLOCK:
            emit_block(gen, node);
            break;

        case AST_STMT_EMBED:
            fprintf(gen->out, "/* [UPL_EMBED_START] */\n%s\n", node->as.raw_block.raw_code);
            emit_indent(gen);
            fprintf(gen->out, "/* [UPL_EMBED_END] */\n");
            break;

        case AST_STMT_ASM:
            emit_asm_block(gen, node->as.raw_block.raw_code);
            break;

        case AST_STMT_EXPR:
            emit_expression(gen, node->as.unary_expr.operand);
            fprintf(gen->out, ";\n");
            break;

        default:
            break;
    }
}

void upl_codegen_init(UplCodeGen *gen, FILE *out) {
    gen->out = out ? out : stdout;
    gen->indent_level = 0;
    gen->had_error = false;
    gen->error_message[0] = '\0';
    gen->pointer_var_count = 0;
}

bool upl_codegen_generate(UplCodeGen *gen, const UplAstNode *program) {
    if (!program || program->kind != AST_PROGRAM) return false;

    // 1. Cabecera C99 estándar con runtime autónomo
    fprintf(gen->out, "/* Codigo generado formalmente por el compilador UPL+ (Dragon Book Standard) */\n");
    fprintf(gen->out, "#include <stdio.h>\n");
    fprintf(gen->out, "#include <stdint.h>\n");
    fprintf(gen->out, "#include <stdbool.h>\n");
    fprintf(gen->out, "#include <stdlib.h>\n");
    fprintf(gen->out, "#include <string.h>\n");
    fprintf(gen->out, "#include <math.h>\n");
    fprintf(gen->out, "#ifdef _WIN32\n");
    fprintf(gen->out, "#  include <conio.h>\n");
    fprintf(gen->out, "#  include <windows.h>\n");
    fprintf(gen->out, "static inline int upl_kbhit(void) { return _kbhit(); }\n");
    fprintf(gen->out, "static inline int upl_getch(void) { return _getch(); }\n");
    fprintf(gen->out, "static inline void upl_sleep_ms(int ms) { Sleep(ms); }\n");
    fprintf(gen->out, "static inline void upl_clear_screen(void) {\n");
    fprintf(gen->out, "    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);\n");
    fprintf(gen->out, "    COORD coord = {0, 0};\n");
    fprintf(gen->out, "    SetConsoleCursorPosition(hOut, coord);\n");
    fprintf(gen->out, "}\n");
    fprintf(gen->out, "static inline void upl_setup_console_fullscreen(void) {\n");
    fprintf(gen->out, "    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);\n");
    fprintf(gen->out, "    CONSOLE_CURSOR_INFO cursorInfo;\n");
    fprintf(gen->out, "    GetConsoleCursorInfo(hOut, &cursorInfo);\n");
    fprintf(gen->out, "    cursorInfo.bVisible = FALSE;\n");
    fprintf(gen->out, "    SetConsoleCursorInfo(hOut, &cursorInfo);\n");
    fprintf(gen->out, "    HWND hwnd = GetConsoleWindow();\n");
    fprintf(gen->out, "    if (hwnd) ShowWindow(hwnd, SW_MAXIMIZE);\n");
    fprintf(gen->out, "    system(\"cls\");\n");
    fprintf(gen->out, "}\n");
    fprintf(gen->out, "#else\n");
    fprintf(gen->out, "#  include <unistd.h>\n");
    fprintf(gen->out, "#  include <termios.h>\n");
    fprintf(gen->out, "static inline int upl_kbhit(void) { return 0; }\n");
    fprintf(gen->out, "static inline int upl_getch(void) { return getchar(); }\n");
    fprintf(gen->out, "static inline void upl_sleep_ms(int ms) { usleep(ms * 1000); }\n");
    fprintf(gen->out, "static inline void upl_clear_screen(void) { printf(\"\\033[H\"); fflush(stdout); }\n");
    fprintf(gen->out, "static inline void upl_setup_console_fullscreen(void) { printf(\"\\033[?25l\\033[2J\\033[H\"); fflush(stdout); }\n");
    fprintf(gen->out, "#endif\n\n");
    fprintf(gen->out, "static inline void checkpoint_guardar(const char* nombre) { (void)nombre; }\n");
    fprintf(gen->out, "#define salto_logico_guardar checkpoint_guardar\n\n");
    fprintf(gen->out, "#include \"upl_comp3.h\"\n\n");

    fprintf(gen->out, "#ifndef UPL_EXPORT\n");
    fprintf(gen->out, "#  if defined(_WIN32) || defined(__CYGWIN__)\n");
    fprintf(gen->out, "#    define UPL_EXPORT __declspec(dllexport)\n");
    fprintf(gen->out, "#  else\n");
    fprintf(gen->out, "#    define UPL_EXPORT __attribute__((visibility(\"default\")))\n");
    fprintf(gen->out, "#  endif\n");
    fprintf(gen->out, "#endif\n\n");

    // 2. Bloques embed de nivel superior (cabeceras nativas C/Win32)
    for (UplAstList *l = program->as.program.items; l != NULL; l = l->next) {
        if (l->node->kind == AST_STMT_EMBED) {
            emit_statement(gen, l->node);
            fprintf(gen->out, "\n");
        }
    }

    // 3. Declaración de estructuras (typedefs)
    for (UplAstList *l = program->as.program.items; l != NULL; l = l->next) {
        if (l->node->kind == AST_DECL_STRUCT) {
            fprintf(gen->out, "typedef struct %s {\n", l->node->as.decl_struct.name);
            for (UplAstList *f = l->node->as.decl_struct.fields; f != NULL; f = f->next) {
                fprintf(gen->out, "    ");
                if (f->node->as.decl_field.type && f->node->as.decl_field.type->kind == AST_TYPE_COMP3) {
                    int prec = f->node->as.decl_field.type->as.type_spec.comp3_precision;
                    size_t bytes = (size_t)((prec / 2) + 1);
                    fprintf(gen->out, "uint8_t %s[%zu]; /* comp3(%d) */\n", f->node->as.decl_field.name, bytes, prec);
                } else if (f->node->as.decl_field.type && f->node->as.decl_field.type->kind == AST_TYPE_ARRAY) {
                    emit_type(gen, f->node->as.decl_field.type->as.type_spec.base_or_elem_type);
                    fprintf(gen->out, " %s[%zu];\n", f->node->as.decl_field.name, f->node->as.decl_field.type->as.type_spec.array_len);
                } else {
                    emit_type(gen, f->node->as.decl_field.type);
                    fprintf(gen->out, " %s;\n", f->node->as.decl_field.name);
                }
            }
            fprintf(gen->out, "} %s;\n\n", l->node->as.decl_struct.name);
        }
    }

    // 4. Variables globales a nivel de módulo (sea / sea mut)
    for (UplAstList *l = program->as.program.items; l != NULL; l = l->next) {
        if (l->node->kind == AST_STMT_LET) {
            emit_statement(gen, l->node);
        }
    }
    fprintf(gen->out, "\n");

    // 5. Prototipos de funciones
    for (UplAstList *l = program->as.program.items; l != NULL; l = l->next) {
        if (l->node->kind == AST_DECL_FN) {
            const char *fname = l->node->as.decl_fn.name;
            if (strcmp(fname, "main") == 0 || strcmp(fname, "kmain") == 0) continue;
            if (l->node->as.decl_fn.is_pub) {
                fprintf(gen->out, "UPL_EXPORT ");
            }
            emit_type(gen, l->node->as.decl_fn.return_type);
            fprintf(gen->out, " %s(", fname);
            for (UplAstList *p = l->node->as.decl_fn.params; p != NULL; p = p->next) {
                emit_type(gen, p->node->as.decl_param.type);
                fprintf(gen->out, " %s", p->node->as.decl_param.name);
                if (p->next) fprintf(gen->out, ", ");
            }
            fprintf(gen->out, ");\n");
        }
    }
    fprintf(gen->out, "\n");

    // 6. Definición de funciones
    for (UplAstList *l = program->as.program.items; l != NULL; l = l->next) {
        if (l->node->kind == AST_DECL_FN) {
            const char *fname = l->node->as.decl_fn.name;
            bool is_entrypoint = (strcmp(fname, "main") == 0 || strcmp(fname, "kmain") == 0);

            upl_codegen_clear_pointer_vars(gen);
            for (UplAstList *p = l->node->as.decl_fn.params; p != NULL; p = p->next) {
                if (p->node->as.decl_param.type && p->node->as.decl_param.type->kind == AST_TYPE_POINTER) {
                    upl_codegen_add_pointer_var(gen, p->node->as.decl_param.name);
                }
            }

            if (is_entrypoint) {
                fprintf(gen->out, "int main(int argc, char **argv) ");
            } else {
                if (l->node->as.decl_fn.is_pub) {
                    fprintf(gen->out, "UPL_EXPORT ");
                }
                emit_type(gen, l->node->as.decl_fn.return_type);
                fprintf(gen->out, " %s(", fname);
                for (UplAstList *p = l->node->as.decl_fn.params; p != NULL; p = p->next) {
                    emit_type(gen, p->node->as.decl_param.type);
                    fprintf(gen->out, " %s", p->node->as.decl_param.name);
                    if (p->next) fprintf(gen->out, ", ");
                }
                fprintf(gen->out, ") ");
            }

            emit_block(gen, l->node->as.decl_fn.body);
            fprintf(gen->out, "\n");
        }
    }

    return true;
}
