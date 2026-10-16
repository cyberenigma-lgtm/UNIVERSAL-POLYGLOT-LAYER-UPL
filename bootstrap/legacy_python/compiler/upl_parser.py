# UPL Parser - Sovereign Universal Language
# Copyright (C) 2026 Neuro-OS Genesis / Universal Polyglot Layer

try:
    from .upl_tokens import TokenType, Token
    from .upl_ast import (
        ProgramNode, FunctionDeclNode, StructDeclNode, VarDeclNode,
        BlockNode, IfStmtNode, WhileStmtNode, ForStmtNode, LoopStmtNode,
        ReturnStmtNode, AsmStmtNode, EmbedStmtNode, AssignStmtNode, ExprStmtNode,
        BinaryExpr, UnaryExpr, CallExpr, DerefExpr, AddressOfExpr, CastExpr,
        FieldAccessExpr, IndexExpr, LiteralExpr, IdentifierExpr, ASTNode
    )
except ImportError:
    from upl_tokens import TokenType, Token
    from upl_ast import (
        ProgramNode, FunctionDeclNode, StructDeclNode, VarDeclNode,
        BlockNode, IfStmtNode, WhileStmtNode, ForStmtNode, LoopStmtNode,
        ReturnStmtNode, AsmStmtNode, EmbedStmtNode, AssignStmtNode, ExprStmtNode,
        BinaryExpr, UnaryExpr, CallExpr, DerefExpr, AddressOfExpr, CastExpr,
        FieldAccessExpr, IndexExpr, LiteralExpr, IdentifierExpr, ASTNode
    )

class UPLParser:
    def __init__(self, tokens: list[Token]):
        self.tokens = tokens
        self.cursor = 0

    def peek(self, offset: int = 0) -> Token:
        pos = self.cursor + offset
        if pos < len(self.tokens):
            return self.tokens[pos]
        return self.tokens[-1] # EOF

    def check(self, token_type: TokenType) -> bool:
        return self.peek().type == token_type

    def advance(self) -> Token:
        tok = self.peek()
        if self.cursor < len(self.tokens):
            self.cursor += 1
        return tok

    def match(self, *token_types: TokenType) -> bool:
        for t in token_types:
            if self.check(t):
                self.advance()
                return True
        return False

    def expect(self, token_type: TokenType, error_msg: str) -> Token:
        if self.check(token_type):
            return self.advance()
        tok = self.peek()
        raise SyntaxError(f"Error sintáctico en L{tok.line}:C{tok.col}: {error_msg} (se encontró {tok.type.name} con valor {repr(tok.value)})")

    def parse(self) -> ProgramNode:
        declarations = []
        while not self.check(TokenType.EOF):
            decl = self.parse_top_level_declaration()
            if decl:
                declarations.append(decl)
        return ProgramNode(declarations)

    def parse_type(self) -> str:
        # Puntero: *Tipo
        if self.match(TokenType.OP_STAR):
            inner = self.parse_type()
            return f"*{inner}"
        
        # Array: [Tipo; N] o [Tipo]
        if self.match(TokenType.LBRACKET):
            elem_type = self.parse_type()
            if self.match(TokenType.SEMICOLON):
                size_tok = self.expect(TokenType.TOKEN_INT, "Se esperaba el tamaño del array")
                self.expect(TokenType.RBRACKET, "Se esperaba ']' al cerrar tipo de array")
                return f"[{elem_type}; {size_tok.value}]"
            self.expect(TokenType.RBRACKET, "Se esperaba ']' al cerrar tipo de slice")
            return f"[{elem_type}]"

        tok = self.advance()
        if tok.type in (
            TokenType.TYPE_I8, TokenType.TYPE_I16, TokenType.TYPE_I32, TokenType.TYPE_I64,
            TokenType.TYPE_U8, TokenType.TYPE_U16, TokenType.TYPE_U32, TokenType.TYPE_U64,
            TokenType.TYPE_USIZE, TokenType.TYPE_F32, TokenType.TYPE_F64,
            TokenType.TYPE_BOOL, TokenType.TYPE_STR, TokenType.TYPE_VOID, TokenType.TYPE_COMP3,
            TokenType.TOKEN_IDENTIFIER
        ):
            type_str = str(tok.value)
            # Soporte para comp3(precision, escala)
            if self.check(TokenType.LPAREN):
                self.advance()
                p = self.expect(TokenType.TOKEN_INT, "Precisión comp3").value
                self.expect(TokenType.COMMA, ",")
                s = self.expect(TokenType.TOKEN_INT, "Escala comp3").value
                self.expect(TokenType.RPAREN, ")")
                return f"comp3({p},{s})"
            return type_str

        return "void"

    DIRECTIVE_TYPES = (
        TokenType.DIR_BAREMETAL,
        TokenType.DIR_SNAPSHOT,
        TokenType.DIR_DVTRGAS30,
        TokenType.DIR_VECTORIAL,
        TokenType.DIR_PURE,
        TokenType.DIR_GUARDED,
        TokenType.DIR_QUANTUM_SAFE,
        TokenType.DIR_TAINT_SAFE,
    )

    def parse_top_level_declaration(self) -> ASTNode:
        directives = []
        while self.peek().type in self.DIRECTIVE_TYPES:
            directives.append(self.advance().type)

        is_pub = False
        if self.match(TokenType.KW_PUB):
            is_pub = True

        # module <nombre>;
        if self.match(TokenType.KW_MODULE):
            mod_name = self.expect(TokenType.TOKEN_IDENTIFIER, "Nombre de módulo esperado").value
            self.match(TokenType.SEMICOLON)
            return None # Los módulos se registran en metadatos

        # fn <nombre>(<params>) -> <ret> { <body> }
        if self.match(TokenType.KW_FN):
            return self.parse_function_declaration(is_pub, TokenType.DIR_BAREMETAL in directives)

        # struct <nombre> { <fields> }
        if self.match(TokenType.KW_STRUCT):
            is_snap = TokenType.DIR_SNAPSHOT in directives
            is_dvtr = TokenType.DIR_DVTRGAS30 in directives
            return self.parse_struct_declaration(is_snap, is_dvtr)

        # let / const a nivel global
        if self.peek().type in (TokenType.KW_LET, TokenType.KW_CONST):
            is_const = self.match(TokenType.KW_CONST)
            if not is_const:
                self.advance() # consume let
            is_mut = self.match(TokenType.KW_MUT)
            name = self.expect(TokenType.TOKEN_IDENTIFIER, "Nombre de variable esperado").value
            var_type = "auto"
            if self.match(TokenType.COLON):
                var_type = self.parse_type()
            init_expr = None
            if self.match(TokenType.OP_ASSIGN):
                init_expr = self.parse_expression()
            self.match(TokenType.SEMICOLON)
            return VarDeclNode(name, var_type, init_expr, is_mut, is_const, TokenType.DIR_VECTORIAL in directives)

        # Si encontramos un token huérfano, avanzamos para no bloquear el bucle
        self.advance()
        return None

    def parse_function_declaration(self, is_pub: bool, is_baremetal: bool) -> FunctionDeclNode:
        name_tok = self.expect(TokenType.TOKEN_IDENTIFIER, "Se esperaba el nombre de la función")
        self.expect(TokenType.LPAREN, "Se esperaba '(' después del nombre de la función")
        
        params = []
        if not self.check(TokenType.RPAREN):
            while True:
                p_name = self.expect(TokenType.TOKEN_IDENTIFIER, "Nombre de parámetro esperado").value
                self.expect(TokenType.COLON, "':' esperado tras nombre de parámetro")
                p_type = self.parse_type()
                params.append((p_name, p_type))
                if not self.match(TokenType.COMMA):
                    break
        self.expect(TokenType.RPAREN, "Se esperaba ')' tras los parámetros")

        ret_type = "void"
        if self.match(TokenType.OP_ARROW):
            ret_type = self.parse_type()

        body = self.parse_block()
        return FunctionDeclNode(name_tok.value, params, ret_type, body, is_pub, is_baremetal)

    def parse_struct_declaration(self, is_snapshot: bool, is_dvtrgas: bool) -> StructDeclNode:
        name_tok = self.expect(TokenType.TOKEN_IDENTIFIER, "Nombre de estructura esperado")
        self.expect(TokenType.LBRACE, "Se esperaba '{' en la declaración de estructura")
        
        fields = []
        while not self.check(TokenType.RBRACE) and not self.check(TokenType.EOF):
            f_name = self.expect(TokenType.TOKEN_IDENTIFIER, "Nombre de campo esperado").value
            self.expect(TokenType.COLON, "':' esperado tras nombre de campo")
            f_type = self.parse_type()
            fields.append((f_name, f_type))
            self.match(TokenType.COMMA)
            self.match(TokenType.SEMICOLON)

        self.expect(TokenType.RBRACE, "Se esperaba '}' al cerrar estructura")
        return StructDeclNode(name_tok.value, fields, is_snapshot, is_dvtrgas)

    def parse_block(self) -> BlockNode:
        self.expect(TokenType.LBRACE, "Se esperaba '{' para abrir bloque")
        statements = []
        while not self.check(TokenType.RBRACE) and not self.check(TokenType.EOF):
            stmt = self.parse_statement()
            if stmt:
                statements.append(stmt)
        self.expect(TokenType.RBRACE, "Se esperaba '}' para cerrar bloque")
        return BlockNode(statements)

    def parse_statement(self) -> ASTNode:
        directives = []
        while self.peek().type in (TokenType.DIR_SNAPSHOT, TokenType.DIR_VECTORIAL):
            directives.append(self.advance().type)

        # let / const
        if self.peek().type in (TokenType.KW_LET, TokenType.KW_CONST):
            is_const = self.match(TokenType.KW_CONST)
            if not is_const:
                self.advance() # let
            is_mut = self.match(TokenType.KW_MUT)
            name = self.expect(TokenType.TOKEN_IDENTIFIER, "Nombre de variable esperado").value
            var_type = "auto"
            if self.match(TokenType.COLON):
                var_type = self.parse_type()
            init_expr = None
            if self.match(TokenType.OP_ASSIGN):
                init_expr = self.parse_expression()
            self.match(TokenType.SEMICOLON)
            return VarDeclNode(name, var_type, init_expr, is_mut, is_const, TokenType.DIR_VECTORIAL in directives)

        # if / si
        if self.match(TokenType.KW_IF):
            cond = self.parse_expression()
            then_branch = self.parse_block()
            else_branch = None
            if self.match(TokenType.KW_ELSE):
                if self.check(TokenType.KW_IF):
                    else_branch = self.parse_statement()
                else:
                    else_branch = self.parse_block()
            return IfStmtNode(cond, then_branch, else_branch)

        # while / mientras
        if self.match(TokenType.KW_WHILE):
            cond = self.parse_expression()
            body = self.parse_block()
            return WhileStmtNode(cond, body)

        # for / para
        if self.match(TokenType.KW_FOR):
            v_name = self.expect(TokenType.TOKEN_IDENTIFIER, "Nombre de variable del bucle esperado").value
            self.expect(TokenType.KW_IN, "'in' o 'en' esperado en bucle for")
            iterable = self.parse_expression()
            body = self.parse_block()
            return ForStmtNode(v_name, iterable, body)

        # loop / repetir
        if self.match(TokenType.KW_LOOP):
            body = self.parse_block()
            return LoopStmtNode(body)

        # return / retornar
        if self.match(TokenType.KW_RETURN):
            ret_expr = None
            if not self.check(TokenType.SEMICOLON) and not self.check(TokenType.RBRACE):
                ret_expr = self.parse_expression()
            self.match(TokenType.SEMICOLON)
            return ReturnStmtNode(ret_expr)

        # asm { ... }
        if self.match(TokenType.KW_ASM):
            code_tok = self.expect(TokenType.TOKEN_STRING, "Código asm esperado")
            return AsmStmtNode(code_tok.value)

        # embed <lang> { ... }
        if self.match(TokenType.KW_EMBED):
            lang_tok = self.expect(TokenType.TOKEN_IDENTIFIER, "Nombre de lenguaje incrustado")
            code_tok = self.expect(TokenType.TOKEN_STRING, "Código incrustado esperado")
            return EmbedStmtNode(lang_tok.value, code_tok.value)

        # Asignación o llamada de función
        expr = self.parse_expression()
        if self.match(TokenType.OP_ASSIGN):
            val = self.parse_expression()
            self.match(TokenType.SEMICOLON)
            return AssignStmtNode(expr, val)

        self.match(TokenType.SEMICOLON)
        return ExprStmtNode(expr)

    # Parser de Expresiones con precedencia estándar
    def parse_expression(self) -> ASTNode:
        return self.parse_logical_or()

    def parse_logical_or(self) -> ASTNode:
        expr = self.parse_logical_and()
        while self.peek().type == TokenType.OP_LOGICAL_OR:
            op = self.advance().value
            right = self.parse_logical_and()
            expr = BinaryExpr(expr, "||", right)
        return expr

    def parse_logical_and(self) -> ASTNode:
        expr = self.parse_equality()
        while self.peek().type == TokenType.OP_LOGICAL_AND:
            op = self.advance().value
            right = self.parse_equality()
            expr = BinaryExpr(expr, "&&", right)
        return expr

    def parse_equality(self) -> ASTNode:
        expr = self.parse_comparison()
        while self.peek().type in (TokenType.OP_EQ, TokenType.OP_NE):
            op = self.advance().value
            right = self.parse_comparison()
            expr = BinaryExpr(expr, op, right)
        return expr

    def parse_comparison(self) -> ASTNode:
        expr = self.parse_bitwise()
        while self.peek().type in (TokenType.OP_LT, TokenType.OP_LE, TokenType.OP_GT, TokenType.OP_GE):
            op = self.advance().value
            right = self.parse_bitwise()
            expr = BinaryExpr(expr, op, right)
        return expr

    def parse_bitwise(self) -> ASTNode:
        expr = self.parse_term()
        while self.peek().type in (TokenType.OP_PIPE, TokenType.OP_CARET, TokenType.OP_AMP, TokenType.OP_SHL, TokenType.OP_SHR):
            op = self.advance().value
            right = self.parse_term()
            expr = BinaryExpr(expr, op, right)
        return expr

    def parse_term(self) -> ASTNode:
        expr = self.parse_factor()
        while self.peek().type in (TokenType.OP_PLUS, TokenType.OP_MINUS):
            if self.cursor > 0 and self.peek().line > self.tokens[self.cursor - 1].line:
                break
            op = self.advance().value
            right = self.parse_factor()
            expr = BinaryExpr(expr, op, right)
        return expr

    def parse_factor(self) -> ASTNode:
        expr = self.parse_cast()
        while self.peek().type in (TokenType.OP_STAR, TokenType.OP_SLASH, TokenType.OP_PERCENT):
            if self.cursor > 0 and self.peek().line > self.tokens[self.cursor - 1].line:
                break
            op = self.advance().value
            right = self.parse_cast()
            expr = BinaryExpr(expr, op, right)
        return expr

    def parse_cast(self) -> ASTNode:
        expr = self.parse_unary()
        while self.match(TokenType.KW_AS):
            target_t = self.parse_type()
            expr = CastExpr(expr, target_t)
        return expr

    def parse_unary(self) -> ASTNode:
        if self.match(TokenType.OP_MINUS):
            return UnaryExpr("-", self.parse_unary())
        if self.match(TokenType.OP_TILDE):
            return UnaryExpr("~", self.parse_unary())
        if self.match(TokenType.OP_STAR): # Desreferenciación (*ptr)
            return DerefExpr(self.parse_unary())
        if self.match(TokenType.OP_AMP): # Dirección (&var)
            return AddressOfExpr(self.parse_unary())
        return self.parse_postfix()

    def parse_postfix(self) -> ASTNode:
        expr = self.parse_primary()
        while True:
            # Llamada de función: expr(args)
            if self.match(TokenType.LPAREN):
                args = []
                if not self.check(TokenType.RPAREN):
                    while True:
                        args.append(self.parse_expression())
                        if not self.match(TokenType.COMMA):
                            break
                self.expect(TokenType.RPAREN, "')' esperada tras argumentos")
                callee_name = expr.name if isinstance(expr, IdentifierExpr) else "anonymous"
                expr = CallExpr(callee_name, args)
            # Acceso a campo: expr.campo
            elif self.match(TokenType.DOT):
                f_tok = self.expect(TokenType.TOKEN_IDENTIFIER, "Nombre de campo esperado")
                expr = FieldAccessExpr(expr, f_tok.value)
            # Indexación: expr[index]
            elif self.match(TokenType.LBRACKET):
                idx = self.parse_expression()
                self.expect(TokenType.RBRACKET, "']' esperada al cerrar índice")
                expr = IndexExpr(expr, idx)
            else:
                break
        return expr

    def parse_primary(self) -> ASTNode:
        tok = self.peek()
        if self.match(TokenType.TOKEN_INT):
            return LiteralExpr(tok.value, "int")
        if self.match(TokenType.TOKEN_FLOAT):
            return LiteralExpr(tok.value, "float")
        if self.match(TokenType.TOKEN_STRING):
            return LiteralExpr(tok.value, "str")
        if self.match(TokenType.TOKEN_BOOL):
            return LiteralExpr(tok.value, "bool")
        if self.match(TokenType.TOKEN_IDENTIFIER):
            return IdentifierExpr(tok.value)
        if self.match(TokenType.LPAREN):
            inner = self.parse_expression()
            self.expect(TokenType.RPAREN, "')' esperada")
            return inner

        self.advance()
        return LiteralExpr(0, "int")
