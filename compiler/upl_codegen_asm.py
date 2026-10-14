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
        
        text_section = [
            "default rel",
            "section .text",
            "global _start_upl_microkernel",
            ""
        ]

        for decl in program.declarations:
            if isinstance(decl, FunctionDeclNode):
                text_section.append(self.gen_function(decl))
                text_section.append("")

        return "\n".join(self.data_section + [""] + text_section)

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

        # Asignar espacio de stack para variables locales (múltiplo de 16)
        # Pre-escaneo simple de variables locales
        var_count = 0
        for stmt in func.body.statements:
            if isinstance(stmt, VarDeclNode):
                var_count += 1
                self.stack_offset += 8
                self.local_vars[stmt.name] = -self.stack_offset

        aligned_space = ((self.stack_offset + 15) // 16) * 16
        if aligned_space > 0:
            lines.append(f"    sub rsp, {aligned_space}")

        # Mapear los primeros argumentos System V (rdi, rsi, rdx, rcx, r8, r9)
        arg_regs = ["rdi", "rsi", "rdx", "rcx", "r8", "r9"]
        for i, param in enumerate(func.params):
            if i < len(arg_regs):
                self.stack_offset += 8
                self.local_vars[param[0]] = -self.stack_offset
                lines.append(f"    mov [rbp - {self.stack_offset}], {arg_regs[i]}")

        # Generar cuerpo
        for stmt in func.body.statements:
            lines.append(self.gen_stmt(stmt))

        # Epílogo por defecto
        lines.append(f".epilogue_{fn_name}:")
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
                offset = self.local_vars.get(stmt.target.name, -8)
                lines.append(f"    mov [rbp {offset}], rax")

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

        elif isinstance(expr, CallExpr):
            # Llamada de función
            arg_regs = ["rdi", "rsi", "rdx", "rcx", "r8", "r9"]
            for i, arg in enumerate(expr.args):
                if i < len(arg_regs):
                    lines.append(self.gen_expr_to_rax(arg))
                    lines.append(f"    mov {arg_regs[i]}, rax")
            lines.append(f"    call {expr.callee}")

        return "\n".join(lines)
