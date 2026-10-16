import re

class BASICParser:
    """
    Modular BASIC Parser for UPL
    Translates classic BASIC syntax to UPL-IR.
    """
    def __init__(self):
        self.rules = [
            # PRINT "Hello" -> IO_WRITE CONSOLE, "Hello"
            (r'(?i)PRINT\s+(.*)', r'IO_WRITE CONSOLE, \1'),
            # LET X = 10 -> SET X, 10
            (r'(?i)LET\s+(\w+)\s*=\s*(.*)', r'SET \1, \2'),
            # X = 10 (sin LET) -> SET X, 10
            (r'^(\w+)\s*=\s*(.*)', r'SET \1, \2'),
            # GOTO 100 -> JUMP L100
            (r'(?i)GOTO\s+(\d+)', r'JUMP L\1'),
            # IF X = 10 THEN GOTO 100 -> IF X == 10 JUMP L100
            (r'(?i)IF\s+(.*)\s+THEN\s+GOTO\s+(\d+)', r'IF \1 JUMP L\2'),
            # Números de línea: 10 PRINT -> LABEL L10
            (r'^(\d+)\s+(.*)', r'LABEL L\1\n\2'),
        ]

    def parse(self, code):
        lines = code.split('\n')
        translated = ["# BASIC to UPL-IR Translation"]
        for line in lines:
            line = line.strip()
            if not line: continue
            
            applied = False
            for pattern, subst in self.rules:
                if re.search(pattern, line):
                    # Manejo especial para LABEL que añade un salto de línea interno
                    line = re.sub(pattern, subst, line)
                    applied = True
            
            translated.append(line)
        return "\n".join(translated)

if __name__ == "__main__":
    parser = BASICParser()
    sample = """
    10 LET X = 5
    20 PRINT X
    30 IF X = 5 THEN GOTO 10
    """
    print("📜 BASIC -> UPL-IR:\n")
    print(parser.parse(sample))
