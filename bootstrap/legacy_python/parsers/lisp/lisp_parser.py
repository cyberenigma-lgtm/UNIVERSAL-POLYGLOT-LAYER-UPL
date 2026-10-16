import re

class LispParser:
    """
    Modular Lisp Parser for UPL
    Translates S-Expressions to UPL-IR.
    """
    def __init__(self):
        self.rules = [
            # (defun name (args) ...) -> PROC name(args)
            (r'\(defun\s+(\w+)\s*\((.*?)\)', r'PROC \1(\2)'),
            # (setq x 10) -> SET x, 10
            (r'\(setq\s+(\w+)\s+(.*)\)', r'SET \1, \2'),
            # (print ...) -> IO_WRITE CONSOLE, ...
            (r'\(print\s+(.*)\)', r'IO_WRITE CONSOLE, \1'),
            # (+ a b) -> ADD a, b (Simplificado)
            (r'\(\+\s+(\w+)\s+(\w+)\)', r'ADD \1, \2'),
        ]

    def parse(self, code):
        # Limpieza de paréntesis finales excesivos para el IR lineal
        code = code.replace(')', ' )')
        lines = code.split('\n')
        translated = ["# Lisp to UPL-IR Translation"]
        for line in lines:
            line = line.strip()
            if not line or line.startswith(';'): continue
            
            for pattern, subst in self.rules:
                if re.search(pattern, line):
                    line = re.sub(pattern, subst, line)
                    break
            
            # Limpiar rastro de paréntesis de cierre solitarios
            line = line.replace(' )', '').replace(')', '').strip()
            if line:
                translated.append(line)
        return "\n".join(translated)

if __name__ == "__main__":
    parser = LispParser()
    sample = """
    (defun suma (a b)
        (+ a b))
    (setq result (suma 10 5))
    (print result)
    """
    print("λ Lisp -> UPL-IR:\n")
    print(parser.parse(sample))
