# UPL Code Generator (Target: ANSI C99) - Sovereign Universal Language
# Copyright (C) 2026 Neuro-OS Genesis / Universal Polyglot Layer

try:
    from .upl_ast import (
        ProgramNode, FunctionDeclNode, StructDeclNode, VarDeclNode,
        BlockNode, IfStmtNode, WhileStmtNode, ForStmtNode, LoopStmtNode,
        ReturnStmtNode, AsmStmtNode, EmbedStmtNode, AssignStmtNode, ExprStmtNode,
        BinaryExpr, UnaryExpr, CallExpr, DerefExpr, AddressOfExpr, CastExpr,
        FieldAccessExpr, IndexExpr, LiteralExpr, IdentifierExpr, ASTNode
    )
except ImportError:
    from upl_ast import (
        ProgramNode, FunctionDeclNode, StructDeclNode, VarDeclNode,
        BlockNode, IfStmtNode, WhileStmtNode, ForStmtNode, LoopStmtNode,
        ReturnStmtNode, AsmStmtNode, EmbedStmtNode, AssignStmtNode, ExprStmtNode,
        BinaryExpr, UnaryExpr, CallExpr, DerefExpr, AddressOfExpr, CastExpr,
        FieldAccessExpr, IndexExpr, LiteralExpr, IdentifierExpr, ASTNode
    )

class UPLCodeGenC:
    def __init__(self):
        self.indent_level = 0
        self.type_map = {
            "i8": "int8_t", "u8": "uint8_t",
            "i16": "int16_t", "u16": "uint16_t",
            "i32": "int32_t", "u32": "uint32_t",
            "i64": "int64_t", "u64": "uint64_t",
            "usize": "size_t",
            "f32": "float", "f64": "double",
            "bool": "bool", "str": "const char*",
            "void": "void", "auto": "int32_t"
        }

    def indent(self) -> str:
        return "    " * self.indent_level

    def translate_type(self, upl_type: str) -> str:
        if not upl_type:
            return "void"
        upl_type = upl_type.strip()
        if upl_type.startswith('*'):
            inner = self.translate_type(upl_type[1:])
            return f"{inner}*"
        if upl_type.startswith('[') and ';' in upl_type:
            # Array estático: lo manejamos en el contexto de declaración
            return upl_type
        if upl_type.startswith("comp3") or upl_type.startswith("decimal"):
            return "NeuroCOBOL_Comp3"
        return self.type_map.get(upl_type, upl_type)

    def generate(self, program: ProgramNode, is_baremetal: bool = False) -> str:
        out = []
        out.append("/* ============================================================================ */")
        out.append("/* CÓDIGO GENERADO AUTOMÁTICAMENTE POR EL COMPILADOR SOBERANO UPLC (ANSI C99) */")
        out.append("/* ============================================================================ */")
        out.append("")
        
        if is_baremetal:
            out.append("#include <stdint.h>")
            out.append("#include <stddef.h>")
            out.append("#include <stdbool.h>")
        else:
            out.append("#include <stdint.h>")
            out.append("#include <stddef.h>")
            out.append("#include <stdbool.h>")
            out.append("#include <stdio.h>")
            out.append("#include <stdlib.h>")
            out.append("#include <string.h>")

        # Inclusión de infraestructura soberana: DVTRGAS-30 y NeuroCOBOL
        out.append("")
        out.append("/* --- BLINDAJE SOBERANO: LANDING-PADS DVTRGAS-30 Y NEUROCOBOL EXACTO --- */")
        out.append("typedef struct {")
        out.append("    uint8_t landing_pad_head[6];")
        out.append("    int64_t valor_entero;")
        out.append("    uint32_t escala;")
        out.append("    uint8_t landing_pad_tail[6];")
        out.append("} NeuroCOBOL_Comp3;")
        out.append("")
        out.append("static inline void salto_logico_guardar(const char* nombre) {")
        if not is_baremetal:
            out.append('    printf("[SALTO_LOGICO] Punto de control guardado: %s\\n", nombre);')
        out.append("}")
        out.append("")

        # Declaraciones hacia adelante de todas las estructuras (evita 'Unknown type name')
        struct_names = [decl.name for decl in program.declarations if isinstance(decl, StructDeclNode)]
        if struct_names:
            out.append("/* --- DECLARACIONES HACIA ADELANTE DE ESTRUCTURAS SOBERANAS --- */")
            for s_name in struct_names:
                out.append(f"typedef struct {s_name} {s_name};")
            out.append("")

        # 1. Definición completa de estructuras
        for decl in program.declarations:
            if isinstance(decl, StructDeclNode):
                out.append(self.gen_declaration(decl))
                out.append("")

        # 2. Variables y constantes globales
        for decl in program.declarations:
            if isinstance(decl, VarDeclNode):
                out.append(self.gen_declaration(decl))
                out.append("")

        # 3. Prototipos hacia adelante de funciones
        for decl in program.declarations:
            if isinstance(decl, FunctionDeclNode):
                params_str = ", ".join([f"{self.translate_type(p[1])} {p[0]}" for p in decl.params]) or "void"
                ret_str = self.translate_type(decl.return_type)
                out.append(f"{ret_str} {decl.name}({params_str});")
        out.append("")

        # 4. Implementación de funciones
        for decl in program.declarations:
            if isinstance(decl, FunctionDeclNode):
                out.append(self.gen_declaration(decl))
                out.append("")

        return "\n".join(out)

    def gen_declaration(self, decl: ASTNode) -> str:
        if isinstance(decl, StructDeclNode):
            lines = []
            lines.append(f"struct {decl.name} {{")
            # Si tiene blindaje vectorial o snapshot, añadimos landing-pad DVTRGAS-30
            if decl.is_snapshot or decl.is_dvtrgas:
                lines.append("    uint8_t _dvtrgas30_pad_head[6];")
            for f_name, f_type in decl.fields:
                lines.append(f"    {self.translate_type(f_type)} {f_name};")
            if decl.is_snapshot or decl.is_dvtrgas:
                lines.append("    uint8_t _dvtrgas30_pad_tail[6];")
            lines.append(f"}};")
            return "\n".join(lines)

        elif isinstance(decl, FunctionDeclNode):
            self.pointer_vars = set()
            for p_name, p_type in decl.params:
                if "*" in p_type or p_type.startswith("*"):
                    self.pointer_vars.add(p_name)

            lines = []
            params_str = ", ".join([f"{self.translate_type(p[1])} {p[0]}" for p in decl.params]) or "void"
            ret_str = self.translate_type(decl.return_type)
            lines.append(f"{ret_str} {decl.name}({params_str}) {{")
            self.indent_level += 1
            for stmt in decl.body.statements:
                lines.append(self.gen_statement(stmt))
            self.indent_level -= 1
            lines.append("}")
            return "\n".join(lines)

        elif isinstance(decl, VarDeclNode):
            c_type = self.translate_type(decl.var_type)
            const_kw = "const " if decl.is_const else ""
            init_str = f" = {self.gen_expr(decl.init_expr)}" if decl.init_expr else ""
            return f"{const_kw}{c_type} {decl.name}{init_str};"

        return ""

    def gen_statement(self, stmt: ASTNode) -> str:
        if isinstance(stmt, VarDeclNode):
            c_type = self.translate_type(stmt.var_type)
            const_kw = "const " if stmt.is_const else ""
            init_str = f" = {self.gen_expr(stmt.init_expr)}" if stmt.init_expr else ""
            return f"{self.indent()}{const_kw}{c_type} {stmt.name}{init_str};"

        elif isinstance(stmt, AssignStmtNode):
            return f"{self.indent()}{self.gen_expr(stmt.target)} = {self.gen_expr(stmt.expr)};"

        elif isinstance(stmt, IfStmtNode):
            lines = []
            cond = self.gen_expr(stmt.condition)
            if not (cond.startswith('(') and cond.endswith(')')):
                cond = f"({cond})"
            lines.append(f"{self.indent()}if {cond} {{")
            self.indent_level += 1
            for s in stmt.then_block.statements:
                lines.append(self.gen_statement(s))
            self.indent_level -= 1
            if stmt.else_block:
                if isinstance(stmt.else_block, IfStmtNode):
                    lines.append(f"{self.indent()}}} else {self.gen_statement(stmt.else_block).strip()}")
                elif isinstance(stmt.else_block, BlockNode):
                    lines.append(f"{self.indent()}}} else {{")
                    self.indent_level += 1
                    for s in stmt.else_block.statements:
                        lines.append(self.gen_statement(s))
                    self.indent_level -= 1
                    lines.append(f"{self.indent()}}}")
            else:
                lines.append(f"{self.indent()}}}")
            return "\n".join(lines)

        elif isinstance(stmt, WhileStmtNode):
            lines = []
            cond = self.gen_expr(stmt.condition)
            if not (cond.startswith('(') and cond.endswith(')')):
                cond = f"({cond})"
            lines.append(f"{self.indent()}while {cond} {{")
            self.indent_level += 1
            for s in stmt.body.statements:
                lines.append(self.gen_statement(s))
            self.indent_level -= 1
            lines.append(f"{self.indent()}}}")
            return "\n".join(lines)

        elif isinstance(stmt, LoopStmtNode):
            lines = []
            lines.append(f"{self.indent()}while (1) {{")
            self.indent_level += 1
            for s in stmt.body.statements:
                lines.append(self.gen_statement(s))
            self.indent_level -= 1
            lines.append(f"{self.indent()}}}")
            return "\n".join(lines)

        elif isinstance(stmt, ForStmtNode):
            lines = []
            lines.append(f"{self.indent()}for (int64_t {stmt.var_name} = 0; {stmt.var_name} < ({self.gen_expr(stmt.iterable)}); ++{stmt.var_name}) {{")
            self.indent_level += 1
            for s in stmt.body.statements:
                lines.append(self.gen_statement(s))
            self.indent_level -= 1
            lines.append(f"{self.indent()}}}")
            return "\n".join(lines)

        elif isinstance(stmt, ReturnStmtNode):
            if stmt.expr:
                return f"{self.indent()}return {self.gen_expr(stmt.expr)};"
            return f"{self.indent()}return;"

        elif isinstance(stmt, AsmStmtNode):
            # Ensamblador inline para GCC / Clang
            clean_asm = stmt.asm_code.replace('"', '\\"').replace('\n', '\\n\\t')
            return f'{self.indent()}__asm__ volatile ("{clean_asm}");'

        elif isinstance(stmt, EmbedStmtNode):
            if stmt.lang.lower() == "c":
                return f"{self.indent()}/* --- BLOQUE EMBED C NATIVO --- */\n{stmt.code}"
            else:
                return f"{self.indent()}/* --- EMBED {stmt.lang.upper()} DISPATCHER --- */"

        elif isinstance(stmt, ExprStmtNode):
            return f"{self.indent()}{self.gen_expr(stmt.expr)};"

        return ""

    def gen_expr(self, expr: ASTNode) -> str:
        if isinstance(expr, LiteralExpr):
            if expr.literal_type == "str":
                return f'"{expr.value}"'
            if expr.literal_type == "bool":
                return "true" if expr.value else "false"
            if expr.literal_type == "int":
                if expr.value > 0xFFFFFFFF:
                    return f"{expr.value}ULL"
                return str(expr.value)
            return str(expr.value)

        elif isinstance(expr, IdentifierExpr):
            return expr.name

        elif isinstance(expr, BinaryExpr):
            return f"({self.gen_expr(expr.left)} {expr.op} {self.gen_expr(expr.right)})"

        elif isinstance(expr, UnaryExpr):
            return f"({expr.op}{self.gen_expr(expr.operand)})"

        elif isinstance(expr, DerefExpr):
            return f"(*{self.gen_expr(expr.expr)})"

        elif isinstance(expr, AddressOfExpr):
            return f"(&{self.gen_expr(expr.expr)})"

        elif isinstance(expr, CastExpr):
            c_target = self.translate_type(expr.target_type)
            return f"(({c_target})({self.gen_expr(expr.expr)}))"

        elif isinstance(expr, FieldAccessExpr):
            op = "."
            if isinstance(expr.target, IdentifierExpr) and expr.target.name in getattr(self, 'pointer_vars', set()):
                op = "->"
            return f"{self.gen_expr(expr.target)}{op}{expr.field}"

        elif isinstance(expr, IndexExpr):
            return f"{self.gen_expr(expr.target)}[{self.gen_expr(expr.index)}]"

        elif isinstance(expr, CallExpr):
            args_str = ", ".join([self.gen_expr(a) for a in expr.args])
            return f"{expr.callee}({args_str})"

        return "0"
