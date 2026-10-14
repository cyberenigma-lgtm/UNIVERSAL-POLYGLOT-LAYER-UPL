# UPL Lexer - Sovereign Universal Language
# Copyright (C) 2026 Neuro-OS Genesis / Universal Polyglot Layer

try:
    from .upl_tokens import TokenType, Token, KEYWORDS_MAP, TYPES_MAP, DIRECTIVES_MAP
except ImportError:
    from upl_tokens import TokenType, Token, KEYWORDS_MAP, TYPES_MAP, DIRECTIVES_MAP

class UPLLexer:
    def __init__(self, source: str):
        self.source = source
        self.length = len(source)
        self.cursor = 0
        self.line = 1
        self.col = 1

    def peek(self, offset: int = 0) -> str:
        pos = self.cursor + offset
        if pos < self.length:
            return self.source[pos]
        return '\0'

    def advance(self) -> str:
        ch = self.peek()
        self.cursor += 1
        if ch == '\n':
            self.line += 1
            self.col = 1
        else:
            self.col += 1
        return ch

    def match(self, expected: str) -> bool:
        if self.peek() == expected:
            self.advance()
            return True
        return False

    def skip_whitespace_and_comments(self):
        while self.cursor < self.length:
            ch = self.peek()
            if ch in ' \t\r\n':
                self.advance()
            elif ch == '/' and self.peek(1) == '/':
                while self.cursor < self.length and self.peek() != '\n':
                    self.advance()
            elif ch == '/' and self.peek(1) == '*':
                self.advance()
                self.advance()
                while self.cursor < self.length:
                    if self.peek() == '*' and self.peek(1) == '/':
                        self.advance()
                        self.advance()
                        break
                    self.advance()
            elif ch == '#' and self.peek(1) != '[':
                while self.cursor < self.length and self.peek() != '\n':
                    self.advance()
            else:
                break

    def read_number(self, start_line: int, start_col: int) -> Token:
        num_str = ""
        is_float = False

        if self.peek() == '0' and (self.peek(1) in 'xX'):
            num_str += self.advance() # 0
            num_str += self.advance() # x
            while self.peek() in "0123456789abcdefABCDEF_":
                ch = self.advance()
                if ch != '_': num_str += ch
            return Token(TokenType.TOKEN_INT, int(num_str, 16), start_line, start_col)

        if self.peek() == '0' and (self.peek(1) in 'bB'):
            num_str += self.advance() # 0
            num_str += self.advance() # b
            while self.peek() in "01_":
                ch = self.advance()
                if ch != '_': num_str += ch
            return Token(TokenType.TOKEN_INT, int(num_str, 2), start_line, start_col)

        while self.peek().isdigit() or self.peek() == '_':
            ch = self.advance()
            if ch != '_': num_str += ch

        if self.peek() == '.' and self.peek(1).isdigit():
            is_float = True
            num_str += self.advance()
            while self.peek().isdigit() or self.peek() == '_':
                ch = self.advance()
                if ch != '_': num_str += ch

        if is_float:
            return Token(TokenType.TOKEN_FLOAT, float(num_str), start_line, start_col)
        return Token(TokenType.TOKEN_INT, int(num_str), start_line, start_col)

    def read_string(self, quote_char: str, start_line: int, start_col: int) -> Token:
        val = ""
        self.advance() # consume opening quote
        while self.cursor < self.length and self.peek() != quote_char:
            ch = self.advance()
            if ch == '\\':
                escaped = self.advance()
                if escaped == 'n': val += '\n'
                elif escaped == 't': val += '\t'
                elif escaped == 'r': val += '\r'
                elif escaped == '\\': val += '\\'
                elif escaped == quote_char: val += quote_char
                else: val += escaped
            else:
                val += ch

        if self.cursor < self.length:
            self.advance() # consume closing quote
        return Token(TokenType.TOKEN_STRING, val, start_line, start_col)

    def read_raw_block(self) -> str:
        """Captura el contenido crudo entre llaves { ... } para bloques 'asm' y 'embed'"""
        self.skip_whitespace_and_comments()
        if self.peek() != '{':
            return ""
        self.advance() # Consume '{'
        brace_depth = 1
        raw_code = []
        while self.cursor < self.length and brace_depth > 0:
            ch = self.peek()
            if ch == '{':
                brace_depth += 1
                raw_code.append(self.advance())
            elif ch == '}':
                brace_depth -= 1
                if brace_depth == 0:
                    self.advance() # Consume closing '}'
                    break
                raw_code.append(self.advance())
            else:
                raw_code.append(self.advance())
        return "".join(raw_code).strip()

    def tokenize(self) -> list[Token]:
        tokens = []

        while self.cursor < self.length:
            self.skip_whitespace_and_comments()
            if self.cursor >= self.length:
                break

            start_line = self.line
            start_col = self.col
            ch = self.peek()

            # Directivas (@baremetal, @snapshot, etc.)
            if ch == '@':
                dir_str = self.advance()
                while self.peek().isalnum() or self.peek() == '_':
                    dir_str += self.advance()
                if dir_str in DIRECTIVES_MAP:
                    tokens.append(Token(DIRECTIVES_MAP[dir_str], dir_str, start_line, start_col))
                else:
                    tokens.append(Token(TokenType.TOKEN_IDENTIFIER, dir_str, start_line, start_col))
                continue

            # Números
            if ch.isdigit():
                tokens.append(self.read_number(start_line, start_col))
                continue

            # Cadenas
            if ch in ('"', "'"):
                tokens.append(self.read_string(ch, start_line, start_col))
                continue

            # Identificadores y palabras clave
            if ch.isalpha() or ch == '_':
                ident = ""
                while self.peek().isalnum() or self.peek() == '_':
                    ident += self.advance()

                lower_ident = ident.lower()
                
                # Booleanos
                if lower_ident in ("true", "verdadero"):
                    tokens.append(Token(TokenType.TOKEN_BOOL, True, start_line, start_col))
                elif lower_ident in ("false", "falso"):
                    tokens.append(Token(TokenType.TOKEN_BOOL, False, start_line, start_col))
                elif lower_ident in KEYWORDS_MAP:
                    kw_type = KEYWORDS_MAP[lower_ident]
                    tokens.append(Token(kw_type, ident, start_line, start_col))
                    # Si es asm o embed, capturamos su contenido crudo
                    if kw_type == TokenType.KW_ASM:
                        raw = self.read_raw_block()
                        tokens.append(Token(TokenType.TOKEN_STRING, raw, start_line, start_col))
                    elif kw_type == TokenType.KW_EMBED:
                        self.skip_whitespace_and_comments()
                        # Leer nombre del lenguaje incrustado (ej: c, python, cobol)
                        lang_name = ""
                        while self.peek().isalnum():
                            lang_name += self.advance()
                        tokens.append(Token(TokenType.TOKEN_IDENTIFIER, lang_name, start_line, start_col))
                        raw = self.read_raw_block()
                        tokens.append(Token(TokenType.TOKEN_STRING, raw, start_line, start_col))
                elif lower_ident in TYPES_MAP:
                    tokens.append(Token(TYPES_MAP[lower_ident], ident, start_line, start_col))
                else:
                    tokens.append(Token(TokenType.TOKEN_IDENTIFIER, ident, start_line, start_col))
                continue

            # Operadores y signos de puntuación
            self.advance()
            if ch == '+':
                tokens.append(Token(TokenType.OP_PLUS, '+', start_line, start_col))
            elif ch == '-':
                if self.match('>'):
                    tokens.append(Token(TokenType.OP_ARROW, '->', start_line, start_col))
                else:
                    tokens.append(Token(TokenType.OP_MINUS, '-', start_line, start_col))
            elif ch == '*':
                tokens.append(Token(TokenType.OP_STAR, '*', start_line, start_col))
            elif ch == '/':
                tokens.append(Token(TokenType.OP_SLASH, '/', start_line, start_col))
            elif ch == '%':
                tokens.append(Token(TokenType.OP_PERCENT, '%', start_line, start_col))
            elif ch == '=':
                if self.match('='):
                    tokens.append(Token(TokenType.OP_EQ, '==', start_line, start_col))
                else:
                    tokens.append(Token(TokenType.OP_ASSIGN, '=', start_line, start_col))
            elif ch == '!':
                if self.match('='):
                    tokens.append(Token(TokenType.OP_NE, '!=', start_line, start_col))
            elif ch == '<':
                if self.match('='):
                    tokens.append(Token(TokenType.OP_LE, '<=', start_line, start_col))
                elif self.match('<'):
                    tokens.append(Token(TokenType.OP_SHL, '<<', start_line, start_col))
                else:
                    tokens.append(Token(TokenType.OP_LT, '<', start_line, start_col))
            elif ch == '>':
                if self.match('='):
                    tokens.append(Token(TokenType.OP_GE, '>=', start_line, start_col))
                elif self.match('>'):
                    tokens.append(Token(TokenType.OP_SHR, '>>', start_line, start_col))
                else:
                    tokens.append(Token(TokenType.OP_GT, '>', start_line, start_col))
            elif ch == '&':
                if self.match('&'):
                    tokens.append(Token(TokenType.OP_LOGICAL_AND, '&&', start_line, start_col))
                else:
                    tokens.append(Token(TokenType.OP_AMP, '&', start_line, start_col))
            elif ch == '|':
                if self.match('|'):
                    tokens.append(Token(TokenType.OP_LOGICAL_OR, '||', start_line, start_col))
                else:
                    tokens.append(Token(TokenType.OP_PIPE, '|', start_line, start_col))
            elif ch == '^':
                tokens.append(Token(TokenType.OP_CARET, '^', start_line, start_col))
            elif ch == '~':
                tokens.append(Token(TokenType.OP_TILDE, '~', start_line, start_col))
            elif ch == '(':
                tokens.append(Token(TokenType.LPAREN, '(', start_line, start_col))
            elif ch == ')':
                tokens.append(Token(TokenType.RPAREN, ')', start_line, start_col))
            elif ch == '{':
                tokens.append(Token(TokenType.LBRACE, '{', start_line, start_col))
            elif ch == '}':
                tokens.append(Token(TokenType.RBRACE, '}', start_line, start_col))
            elif ch == '[':
                tokens.append(Token(TokenType.LBRACKET, '[', start_line, start_col))
            elif ch == ']':
                tokens.append(Token(TokenType.RBRACKET, ']', start_line, start_col))
            elif ch == ':':
                tokens.append(Token(TokenType.COLON, ':', start_line, start_col))
            elif ch == ';':
                tokens.append(Token(TokenType.SEMICOLON, ';', start_line, start_col))
            elif ch == ',':
                tokens.append(Token(TokenType.COMMA, ',', start_line, start_col))
            elif ch == '.':
                tokens.append(Token(TokenType.DOT, '.', start_line, start_col))

        tokens.append(Token(TokenType.EOF, None, self.line, self.col))
        return tokens
