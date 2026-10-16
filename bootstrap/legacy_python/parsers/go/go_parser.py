import re

class GoParser:
    """
    Modular Go Parser for UPL
    Translates Go syntax to UPL-IR.
    """
    def __init__(self):
        self.rules = [
            # Funciones: func name(args) { ... }
            (r'func\s+(\w+)\s*\((.*?)\)(?:\s*\w+)?\s*\{', r'PROC \1(\2)'),
            # Variables de inferencia: x := 10
            (r'(\w+)\s*:=\s*(.*)', r'SET \1, \2'),
            # Variables explícitas: var x int = 10
            (r'var\s+(\w+)\s*(?:\w+)?\s*=\s*(.*)', r'SET \1, \2'),
            # Retorno: return x
            (r'return\s+(.*)', r'RET \1'),
            # Salida: fmt.Println(...)
            (r'fmt\.Println\s*\((.*)\)', r'IO_WRITE CONSOLE, \1'),
            # Control de flujo: if x > 0 { ... }
            (r'if\s+(.*?)\s*\{', r'IF \1 JUMP'),
        ]

    def parse(self, code):
        code = re.sub(r'//.*', '', code)
        
        lines = code.split('\n')
        translated = ["# Go to UPL-IR Translation"]
        
        for line in lines:
            line = line.strip()
            if not line or line in ['{', '}', 'package main', 'import "fmt"']: continue
            
            applied = False
            for pattern, subst in self.rules:
                if re.search(pattern, line):
                    line = re.sub(pattern, subst, line)
                    applied = True
                    break
            
            if applied:
                translated.append(line)
            else:
                translated.append(line)
                
        return "\n".join(translated)

if __name__ == "__main__":
    parser = GoParser()
    sample = """
    func main() {
        x := 10
        fmt.Println(x)
        return
    }
    """
    print("🐹 Go -> UPL-IR:\n")
    print(parser.parse(sample))
