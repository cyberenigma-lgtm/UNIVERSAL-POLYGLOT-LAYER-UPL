import re

class COBOLParser:
    """
    Modular COBOL Parser for UPL
    Translates COBOL syntax (Procedure Division focus) to UPL-IR.
    """
    def __init__(self):
        self.rules = [
            # DISPLAY "TEXT" -> IO_WRITE CONSOLE, "TEXT"
            (r'(?i)DISPLAY\s+(.*)\.?', r'IO_WRITE CONSOLE, \1'),
            # MOVE 10 TO X -> SET X, 10
            (r'(?i)MOVE\s+(.*)\s+TO\s+(\w+)\.?', r'SET \2, \1'),
            # ADD 5 TO X -> ADD X, 5
            (r'(?i)ADD\s+(.*)\s+TO\s+(\w+)\.?', r'ADD \2, \1'),
            # PERFORM PARA-NAME -> CALL PARA-NAME
            (r'(?i)PERFORM\s+(\w+)\.?', r'CALL \1'),
            # GOTO PARA-NAME -> JUMP PARA-NAME
            (r'(?i)GO\s+TO\s+(\w+)\.?', r'JUMP \1'),
            # Parrafos: MAIN-PROC. -> LABEL MAIN-PROC
            (r'^(\w+)\.\s*$', r'LABEL \1'),
        ]

    def parse(self, code):
        lines = code.split('\n')
        translated = ["# COBOL to UPL-IR Translation"]
        for line in lines:
            line = line.strip()
            # Ignorar divisiones de cabecera por ahora para el IR lógico
            if any(x in line.upper() for x in ["DIVISION", "SECTION", "IDENTIFICATION", "PROGRAM-ID"]):
                continue
            if not line: continue
            
            applied = False
            for pattern, subst in self.rules:
                if re.search(pattern, line):
                    line = re.sub(pattern, subst, line)
                    applied = True
                    break
            
            translated.append(line.rstrip('.'))
            
        return "\n".join(translated)

if __name__ == "__main__":
    parser = COBOLParser()
    sample = """
    DISPLAY "Iniciando COBOL CORE".
    MOVE 100 TO BALANCE.
    ADD 50 TO BALANCE.
    PERFORM UPDATE-LOG.
    DISPLAY BALANCE.
    """
    print("💼 COBOL -> UPL-IR:\n")
    print(parser.parse(sample))
