# UPL Code Generator (Target: x86_64 NASM Bare-Metal) - Sovereign Universal Language
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

class UPLCodeGenASM:
    def __init__(self):
        self.label_counter = 0
        self.data_section = []
        self.string_literals = {}
        self.local_vars = {} # name -> rbp offset
        self.constants = {}  # name -> value
        self.stack_offset = 0

    def new_label(self, prefix: str = "L_UPL") -> str:
        self.label_counter += 1
        return f"{prefix}_{self.label_counter}"

    def generate(self, program: ProgramNode) -> str:
        self.data_section = [
            "section .rodata",
            "align 16",
            "; Blindaje Vectorial DVTRGAS-30 (Landing Pad Maestro)",
            "dvtrgas30_landing_pad: times 6 db 0x00"
        ]
        self.constants = {}

        # Escanear constantes y variables globales
        for decl in program.declarations:
            if isinstance(decl, VarDeclNode):
                if decl.is_const and decl.init_expr and isinstance(decl.init_expr, LiteralExpr):
                    self.constants[decl.name] = decl.init_expr.value
                    if isinstance(decl.init_expr.value, (int, float)):
                        self.data_section.append(f"{decl.name}: dq {decl.init_expr.value}")
                elif not decl.is_const:
                    val = 0
                    if decl.init_expr and isinstance(decl.init_expr, LiteralExpr) and isinstance(decl.init_expr.value, (int, float)):
                        val = decl.init_expr.value
                    self.data_section.append(f"{decl.name}: dq {val}")
        
        self.defined_functions = {decl.name for decl in program.declarations if isinstance(decl, FunctionDeclNode)}
        self.external_calls = set()

        text_section = [
            "default rel",
            "section .text",
            "global _start_upl_microkernel",
            ""
        ]

        func_bodies = []
        for decl in program.declarations:
            if isinstance(decl, FunctionDeclNode):
                func_bodies.append(self.gen_function(decl))
                func_bodies.append("")

        # Declarar funciones externas no definidas en este archivo
        for ext_fn in sorted(self.external_calls):
            if ext_fn not in self.defined_functions:
                text_section.append(f"extern {ext_fn}")
        if self.external_calls:
            text_section.append("")

        text_section.extend(func_bodies)
        return "\n".join(self.data_section + [""] + text_section)

    def _collect_vars_recursive(self, stmts):
        found = []
        for s in stmts:
            if isinstance(s, VarDeclNode):
                found.append(s)
            elif isinstance(s, WhileStmtNode):
                found.extend(self._collect_vars_recursive(s.body.statements))
            elif isinstance(s, ForStmtNode):
                found.append(VarDeclNode(s.var_name, "i64", None))
                found.extend(self._collect_vars_recursive(s.body.statements))
            elif isinstance(s, IfStmtNode):
                found.extend(self._collect_vars_recursive(s.then_block.statements))
                if s.else_block:
                    if isinstance(s.else_block, BlockNode):
                        found.extend(self._collect_vars_recursive(s.else_block.statements))
                    elif isinstance(s.else_block, IfStmtNode):
                        found.extend(self._collect_vars_recursive([s.else_block]))
                    elif isinstance(s.else_block, list):
                        found.extend(self._collect_vars_recursive(s.else_block))
            elif isinstance(s, LoopStmtNode):
                found.extend(self._collect_vars_recursive(s.body.statements))
            elif isinstance(s, BlockNode):
                found.extend(self._collect_vars_recursive(s.statements))
        return found

    def gen_function(self, func: FunctionDeclNode) -> str:
        self.local_vars = {}
        self.stack_offset = 0
        lines = []

        fn_name = func.name
        if func.is_baremetal:
            lines.append(f"; === FUNCIÓN BARE-METAL / RING 0: {fn_name} ===")
        lines.append(f"global {fn_name}")
        lines.append(f"{fn_name}:")
        lines.append("    push rbp")
        lines.append("    mov rbp, rsp")

        # Mapear los primeros argumentos System V (rdi, rsi, rdx, rcx, r8, r9)
        arg_regs = ["rdi", "rsi", "rdx", "rcx", "r8", "r9"]
        for i, param in enumerate(func.params):
            if i < len(arg_regs):
                self.stack_offset += 8
                self.local_vars[param[0]] = -self.stack_offset
                lines.append(f"    mov [rbp - {self.stack_offset}], {arg_regs[i]}")

        # Recolectar recursivamente todas las variables locales del cuerpo
        all_local_vars = self._collect_vars_recursive(func.body.statements)
        for var_node in all_local_vars:
            if var_node.name not in self.local_vars:
                self.stack_offset += 8
                self.local_vars[var_node.name] = -self.stack_offset

        aligned_space = ((self.stack_offset + 15) // 16) * 16
        if aligned_space > 0:
            lines.insert(2, f"    sub rsp, {aligned_space}")

        # Generar cuerpo
        for stmt in func.body.statements:
            lines.append(self.gen_stmt(stmt))

        # Epílogo por defecto
        lines.append(f"epilogue_{fn_name}:")
        lines.append("    mov rsp, rbp")
        lines.append("    pop rbp")
        lines.append("    ret")

        return "\n".join(lines)

    def gen_stmt(self, stmt: ASTNode) -> str:
        lines = []
        if isinstance(stmt, VarDeclNode):
            if stmt.init_expr:
                lines.append(self.gen_expr_to_rax(stmt.init_expr))
                offset = self.local_vars.get(stmt.name, -8)
                lines.append(f"    mov [rbp {offset}], rax")

        elif isinstance(stmt, AssignStmtNode):
            # Evaluar valor a rax
            lines.append(self.gen_expr_to_rax(stmt.expr))
            # Si target es un puntero desreferenciado: *ptr = val
            if isinstance(stmt.target, DerefExpr):
                lines.append("    push rax") # Guardar valor
                lines.append(self.gen_expr_to_rax(stmt.target.expr)) # rax tiene la dirección
                lines.append("    mov rbx, rax")
                lines.append("    pop rax")
                lines.append("    mov [rbx], rax")
            elif isinstance(stmt.target, IdentifierExpr):
                if stmt.target.name in self.local_vars:
                    offset = self.local_vars[stmt.target.name]
                    lines.append(f"    mov [rbp {offset}], rax")
                else:
                    lines.append(f"    mov [{stmt.target.name}], rax")

        elif isinstance(stmt, IfStmtNode):
            label_else = self.new_label("L_else")
            label_end = self.new_label("L_endif")
            lines.append(self.gen_expr_to_rax(stmt.condition))
            lines.append("    test rax, rax")
            lines.append(f"    jz {label_else}")
            for s in stmt.then_block.statements:
                lines.append(self.gen_stmt(s))
            lines.append(f"    jmp {label_end}")
            lines.append(f"{label_else}:")
            if stmt.else_block:
                if isinstance(stmt.else_block, BlockNode):
                    for s in stmt.else_block.statements:
                        lines.append(self.gen_stmt(s))
                elif isinstance(stmt.else_block, ASTNode):
                    lines.append(self.gen_stmt(stmt.else_block))
            lines.append(f"{label_end}:")

        elif isinstance(stmt, WhileStmtNode):
            label_start = self.new_label("L_while_start")
            label_end = self.new_label("L_while_end")
            lines.append(f"{label_start}:")
            lines.append(self.gen_expr_to_rax(stmt.condition))
            lines.append("    test rax, rax")
            lines.append(f"    jz {label_end}")
            for s in stmt.body.statements:
                lines.append(self.gen_stmt(s))
            lines.append(f"    jmp {label_start}")
            lines.append(f"{label_end}:")

        elif isinstance(stmt, LoopStmtNode):
            label_start = self.new_label("L_loop_start")
            lines.append(f"{label_start}:")
            for s in stmt.body.statements:
                lines.append(self.gen_stmt(s))
            lines.append(f"    jmp {label_start}")

        elif isinstance(stmt, ForStmtNode):
            label_start = self.new_label("L_for_start")
            label_end = self.new_label("L_for_end")
            offset = self.local_vars.get(stmt.var_name, -8)
            lines.append(f"    mov qword [rbp {offset}], 0")
            lines.append(f"{label_start}:")
            lines.append(self.gen_expr_to_rax(stmt.iterable))
            lines.append(f"    cmp [rbp {offset}], rax")
            lines.append(f"    jge {label_end}")
            for s in stmt.body.statements:
                lines.append(self.gen_stmt(s))
            lines.append(f"    inc qword [rbp {offset}]")
            lines.append(f"    jmp {label_start}")
            lines.append(f"{label_end}:")

        elif isinstance(stmt, ReturnStmtNode):
            if stmt.expr:
                lines.append(self.gen_expr_to_rax(stmt.expr))
            lines.append("    mov rsp, rbp")
            lines.append("    pop rbp")
            lines.append("    ret")

        elif isinstance(stmt, AsmStmtNode):
            # Ensamblador crudo insertado directamente
            lines.append(f"    ; --- BLOQUE ASM SOBERANO CRUDA ---")
            for asm_line in stmt.asm_code.split('\n'):
                asm_line = asm_line.strip()
                if asm_line:
                    if "//" in asm_line:
                        parts = asm_line.split("//", 1)
                        asm_line = f"{parts[0].strip()} ; {parts[1].strip()}"
                    lines.append(f"    {asm_line}")

        elif isinstance(stmt, ExprStmtNode):
            lines.append(self.gen_expr_to_rax(stmt.expr))

        return "\n".join(lines)

    def gen_expr_to_rax(self, expr: ASTNode) -> str:
        lines = []
        if isinstance(expr, LiteralExpr):
            if expr.literal_type == "int":
                lines.append(f"    mov rax, {expr.value}")
            elif expr.literal_type == "bool":
                lines.append(f"    mov rax, {1 if expr.value else 0}")
            elif expr.literal_type == "str":
                str_lbl = self.new_label("str_const")
                self.data_section.append(f'{str_lbl}: db "{expr.value}", 0')
                lines.append(f"    lea rax, [{str_lbl}]")

        elif isinstance(expr, IdentifierExpr):
            if expr.name in self.local_vars:
                offset = self.local_vars[expr.name]
                lines.append(f"    mov rax, [rbp {offset}]")
            elif expr.name in self.constants:
                val = self.constants[expr.name]
                if isinstance(val, int):
                    lines.append(f"    mov rax, {val}")
                else:
                    lines.append(f"    mov rax, [{expr.name}]")
            else:
                lines.append(f"    mov rax, [{expr.name}]")

        elif isinstance(expr, DerefExpr):
            lines.append(self.gen_expr_to_rax(expr.expr))
            lines.append("    mov rax, [rax]")

        elif isinstance(expr, CastExpr):
            # En x86_64, el valor ya reside en rax
            lines.append(self.gen_expr_to_rax(expr.expr))

        elif isinstance(expr, BinaryExpr):
            lines.append(self.gen_expr_to_rax(expr.right))
            lines.append("    push rax")
            lines.append(self.gen_expr_to_rax(expr.left))
            lines.append("    pop rbx")
            if expr.op == '+':
                lines.append("    add rax, rbx")
            elif expr.op == '-':
                lines.append("    sub rax, rbx")
            elif expr.op == '*':
                lines.append("    imul rax, rbx")
            elif expr.op == '/':
                lines.append("    cqo")
                lines.append("    idiv rbx")
            elif expr.op == '==':
                lines.append("    cmp rax, rbx")
                lines.append("    sete al")
                lines.append("    movzx rax, al")
            elif expr.op == '!=':
                lines.append("    cmp rax, rbx")
                lines.append("    setne al")
                lines.append("    movzx rax, al")
            elif expr.op == '<':
                lines.append("    cmp rax, rbx")
                lines.append("    setl al")
                lines.append("    movzx rax, al")
            elif expr.op == '<=':
                lines.append("    cmp rax, rbx")
                lines.append("    setle al")
                lines.append("    movzx rax, al")
            elif expr.op == '>':
                lines.append("    cmp rax, rbx")
                lines.append("    setg al")
                lines.append("    movzx rax, al")
            elif expr.op == '>=':
                lines.append("    cmp rax, rbx")
                lines.append("    setge al")
                lines.append("    movzx rax, al")
            elif expr.op in ('&&', 'and'):
                lines.append("    test rax, rax")
                lines.append("    setnz al")
                lines.append("    test rbx, rbx")
                lines.append("    setnz bl")
                lines.append("    and al, bl")
                lines.append("    movzx rax, al")
            elif expr.op in ('||', 'or'):
                lines.append("    or rax, rbx")
                lines.append("    setnz al")
                lines.append("    movzx rax, al")
            elif expr.op == '&':
                lines.append("    and rax, rbx")
            elif expr.op == '|':
                lines.append("    or rax, rbx")
            elif expr.op == '^':
                lines.append("    xor rax, rbx")
            elif expr.op == '%':
                lines.append("    cqo")
                lines.append("    idiv rbx")
                lines.append("    mov rax, rdx")
            elif expr.op == '<<':
                lines.append("    mov rcx, rbx")
                lines.append("    shl rax, cl")
            elif expr.op == '>>':
                lines.append("    mov rcx, rbx")
                lines.append("    shr rax, cl")

        elif isinstance(expr, UnaryExpr):
            lines.append(self.gen_expr_to_rax(expr.operand))
            if expr.op == '-':
                lines.append("    neg rax")
            elif expr.op == '~':
                lines.append("    not rax")
            elif expr.op == '!':
                lines.append("    test rax, rax")
                lines.append("    setz al")
                lines.append("    movzx rax, al")

        elif isinstance(expr, AddressOfExpr):
            if isinstance(expr.expr, IdentifierExpr):
                if expr.expr.name in self.local_vars:
                    offset = self.local_vars[expr.expr.name]
                    lines.append(f"    lea rax, [rbp {offset}]")
                else:
                    lines.append(f"    lea rax, [{expr.expr.name}]")

        elif isinstance(expr, IndexExpr):
            lines.append(self.gen_expr_to_rax(expr.index))
            lines.append("    push rax")
            lines.append(self.gen_expr_to_rax(expr.target))
            lines.append("    pop rbx")
            lines.append("    shl rbx, 3")
            lines.append("    add rax, rbx")
            lines.append("    mov rax, [rax]")

        elif isinstance(expr, FieldAccessExpr):
            lines.append(self.gen_expr_to_rax(expr.target))

        elif isinstance(expr, CallExpr):
            # Llamada de función
            self.external_calls.add(expr.callee)
            arg_regs = ["rdi", "rsi", "rdx", "rcx", "r8", "r9"]
            for i, arg in enumerate(expr.args):
                if i < len(arg_regs):
                    lines.append(self.gen_expr_to_rax(arg))
                    lines.append(f"    mov {arg_regs[i]}, rax")
            lines.append(f"    call {expr.callee}")

        return "\n".join(lines)
