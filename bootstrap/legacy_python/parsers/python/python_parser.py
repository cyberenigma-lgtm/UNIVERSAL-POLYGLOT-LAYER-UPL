import re

class PythonParser:
    """
    Modular Python Parser for UPL
    Translates Python syntax to UPL-IR.
    """
    def __init__(self):
        self.rules = [
            (r'def\s+(\w+)\s*\((.*?)\):', r'PROC \1(\2)'),
            (r'return\s+(.*)', r'RET \1'),
            (r'print\s*\((.*)\)', r'IO_WRITE CONSOLE, \1'),
            (r'(\w+)\s*=\s*(.*)', r'SET \1, \2'),
            (r'if\s+(.*):', r'IF \1 JUMP'),
        ]

    def parse(self, code):
        lines = code.split('\n')
        translated = ["# Python to UPL-IR Translation"]
        for line in lines:
            line = line.strip()
            if not line or line.startswith('#'): continue
            
            for pattern, subst in self.rules:
                if re.search(pattern, line):
                    line = re.sub(pattern, subst, line)
                    break
            translated.append(line)
        return "\n".join(translated)
