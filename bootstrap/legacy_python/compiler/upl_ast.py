# UPL Abstract Syntax Tree (AST) - Sovereign Universal Language
# Copyright (C) 2026 Neuro-OS Genesis / Universal Polyglot Layer

class ASTNode:
    pass

class ProgramNode(ASTNode):
    def __init__(self, declarations: list[ASTNode]):
        self.declarations = declarations

    def __repr__(self):
        return f"ProgramNode({len(self.declarations)} declarations)"

class FunctionDeclNode(ASTNode):
    def __init__(self, name: str, params: list, return_type: str, body: ASTNode, is_pub: bool = False, is_baremetal: bool = False):
        self.name = name
        self.params = params # List of (name, type_str)
        self.return_type = return_type
        self.body = body
        self.is_pub = is_pub
        self.is_baremetal = is_baremetal

    def __repr__(self):
        return f"FunctionDeclNode({self.name}, params={len(self.params)}, ret={self.return_type})"

class StructDeclNode(ASTNode):
    def __init__(self, name: str, fields: list, is_snapshot: bool = False, is_dvtrgas: bool = False):
        self.name = name
        self.fields = fields # List of (name, type_str)
        self.is_snapshot = is_snapshot
        self.is_dvtrgas = is_dvtrgas

    def __repr__(self):
        return f"StructDeclNode({self.name}, fields={len(self.fields)}, snapshot={self.is_snapshot})"

class VarDeclNode(ASTNode):
    def __init__(self, name: str, var_type: str, init_expr: ASTNode, is_mut: bool = True, is_const: bool = False, is_vectorial: bool = False):
        self.name = name
        self.var_type = var_type
        self.init_expr = init_expr
        self.is_mut = is_mut
        self.is_const = is_const
        self.is_vectorial = is_vectorial

    def __repr__(self):
        return f"VarDeclNode({self.name}: {self.var_type})"

class BlockNode(ASTNode):
    def __init__(self, statements: list[ASTNode]):
        self.statements = statements

class IfStmtNode(ASTNode):
    def __init__(self, condition: ASTNode, then_block: ASTNode, else_block: ASTNode = None):
        self.condition = condition
        self.then_block = then_block
        self.else_block = else_block

class WhileStmtNode(ASTNode):
    def __init__(self, condition: ASTNode, body: ASTNode):
        self.condition = condition
        self.body = body

class ForStmtNode(ASTNode):
    def __init__(self, var_name: str, iterable: ASTNode, body: ASTNode):
        self.var_name = var_name
        self.iterable = iterable
        self.body = body

class LoopStmtNode(ASTNode):
    def __init__(self, body: ASTNode):
        self.body = body

class ReturnStmtNode(ASTNode):
    def __init__(self, expr: ASTNode = None):
        self.expr = expr

class AsmStmtNode(ASTNode):
    def __init__(self, asm_code: str):
        self.asm_code = asm_code

class EmbedStmtNode(ASTNode):
    def __init__(self, lang: str, code: str):
        self.lang = lang
        self.code = code

class AssignStmtNode(ASTNode):
    def __init__(self, target: ASTNode, expr: ASTNode):
        self.target = target
        self.expr = expr

class ExprStmtNode(ASTNode):
    def __init__(self, expr: ASTNode):
        self.expr = expr

# Expresiones
class BinaryExpr(ASTNode):
    def __init__(self, left: ASTNode, op: str, right: ASTNode):
        self.left = left
        self.op = op
        self.right = right

class UnaryExpr(ASTNode):
    def __init__(self, op: str, operand: ASTNode):
        self.op = op
        self.operand = operand

class CallExpr(ASTNode):
    def __init__(self, callee: str, args: list[ASTNode]):
        self.callee = callee
        self.args = args

class DerefExpr(ASTNode):
    def __init__(self, expr: ASTNode):
        self.expr = expr

class AddressOfExpr(ASTNode):
    def __init__(self, expr: ASTNode):
        self.expr = expr

class CastExpr(ASTNode):
    def __init__(self, expr: ASTNode, target_type: str):
        self.expr = expr
        self.target_type = target_type

class FieldAccessExpr(ASTNode):
    def __init__(self, target: ASTNode, field: str):
        self.target = target
        self.field = field

class IndexExpr(ASTNode):
    def __init__(self, target: ASTNode, index: ASTNode):
        self.target = target
        self.index = index

class LiteralExpr(ASTNode):
    def __init__(self, value: any, literal_type: str):
        self.value = value
        self.literal_type = literal_type # "int", "float", "str", "bool"

class IdentifierExpr(ASTNode):
    def __init__(self, name: str):
        self.name = name
