import re

class RustParser:
    """
    Modular Rust Parser for UPL
    Translates Rust syntax to UPL-IR.
    """
    def __init__(self):
        self.rules = [
            # Funciones: fn name(args) -> type { ... }
            (r'fn\s+(\w+)\s*\((.*?)\)(?:\s*->\s*\w+)?\s*\{', r'PROC \1(\2)'),
            # Variables: let x = 10; o let mut x = 10;
            (r'let\s+(?:mut\s+)?(\w+)\s*(?::\s*\w+)?\s*=\s*(.*?);', r'SET \1, \2'),
            # Retorno: return x; o simplemente x (sin punto y coma)
            (r'return\s+(.*?);', r'RET \1'),
            # Macros de impresión: println!(...);
            (r'println!\s*\((.*)\);', r'IO_WRITE CONSOLE, \1'),
            # Control de flujo: if x > 0 { ... }
            (r'if\s+(.*?)\s*\{', r'IF \1 JUMP'),
        ]

    def parse(self, code):
        # Limpieza básica de comentarios
        code = re.sub(r'//.*', '', code)
        
        lines = code.split('\n')
        translated = ["# Rust to UPL-IR Translation"]
        
        for line in lines:
            line = line.strip()
            if not line or line == '{' or line == '}': continue
            
            applied = False
            for pattern, subst in self.rules:
                if re.search(pattern, line):
                    line = re.sub(pattern, subst, line)
                    applied = True
                    break
            
            if applied:
                translated.append(line)
            else:
                # En Rust, una línea sin ; al final de una función es un retorno implícito
                if not line.endswith(';') and not line.endswith('}') and not line.endswith('{'):
                    translated.append(f"RET {line}")
                else:
                    translated.append(line.rstrip(';'))
                
        return "\n".join(translated)

if __name__ == "__main__":
    parser = RustParser()
    sample = """
    fn main() {
        let x = 42;
        println!("Value: {}", x);
        x
    }
    """
    print("🦀 Rust -> UPL-IR:\n")
    print(parser.parse(sample))
