import re

class JSParser:
    """
    Modular JavaScript Parser for UPL
    Translates JS (ES6+) syntax to UPL-IR.
    """
    def __init__(self):
        self.rules = [
            # Funciones: function name(args) { ... }
            (r'function\s+(\w+)\s*\((.*?)\)\s*\{?', r'PROC \1(\2)'),
            # Arrow functions: const name = (args) => { ... }
            (r'(?:const|let|var)\s+(\w+)\s*=\s*\((.*?)\)\s*=>', r'PROC \1(\2)'),
            # Variables: let x = 10;
            (r'(?:let|const|var)\s+(\w+)\s*=\s*(.*?);', r'SET \1, \2'),
            # Retorno: return x;
            (r'return\s+(.*?);', r'RET \1'),
            # Consola: console.log(...);
            (r'console\.log\s*\((.*)\);', r'IO_WRITE CONSOLE, \1'),
            # Estructuras: if (x > 0)
            (r'if\s*\((.*)\)', r'IF \1 JUMP'),
        ]

    def parse(self, code):
        # Limpieza básica
        code = re.sub(r'//.*', '', code)
        
        lines = code.split('\n')
        translated = ["# JavaScript to UPL-IR Translation"]
        
        for line in lines:
            line = line.strip()
            if not line or line in ['{', '}', '};']: continue
            
            applied = False
            for pattern, subst in self.rules:
                if re.search(pattern, line):
                    line = re.sub(pattern, subst, line)
                    applied = True
                    break
            
            if applied:
                translated.append(line)
            else:
                # Mantener línea sin terminadores
                translated.append(line.rstrip(';'))
                
        return "\n".join(translated)

if __name__ == "__main__":
    parser = JSParser()
    sample = """
    function greet(name) {
        let msg = "Hello " + name;
        console.log(msg);
        return msg;
    }
    const add = (a, b) => { return a + b; };
    """
    print("🌐 JS -> UPL-IR:\n")
    print(parser.parse(sample))
