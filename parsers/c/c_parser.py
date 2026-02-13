import re

class CParser:
    """
    Modular C Parser for UPL
    Translates C syntax (C89/C99/C11) to UPL-IR.
    """
    def __init__(self):
        # Reglas de mapeo de tipos C -> UPL
        self.type_map = {
            'int': 'I32',
            'long': 'I64',
            'float': 'F32',
            'double': 'F64',
            'char': 'I8',
            'void': 'VOID'
        }
        
        # Reglas de transformación (Regex)
        self.rules = [
            # Funciones: void func(int a) -> PROC func(I32 a)
            (r'(\w+)\s+(\w+)\s*\((.*?)\)\s*\{?', self._parse_function),
            # Asignaciones: int x = 10; -> SET x, 10
            (r'(\w+)\s+(\w+)\s*=\s*(.*?);', self._parse_assignment),
            # Retorno: return 0; -> RET 0
            (r'return\s+(.*?);', r'RET \1'),
            # IO: printf("..."); -> IO_WRITE CONSOLE, "..."
            (r'printf\s*\((.*)\);', r'IO_WRITE CONSOLE, \1'),
            # Control de flujo: if (x == 0) -> IF x == 0 JUMP
            (r'if\s*\((.*)\)', r'IF \1 JUMP'),
        ]

    def _parse_function(self, match):
        ret_type = match.group(1)
        name = match.group(2)
        args = match.group(3)
        
        # Traducir tipo de retorno y argumentos si es posible
        upl_ret = self.type_map.get(ret_type, ret_type)
        return f"PROC {name}({args}) # Returns {upl_ret}"

    def _parse_assignment(self, match):
        var_type = match.group(1)
        name = match.group(2)
        val = match.group(3)
        
        upl_type = self.type_map.get(var_type, var_type)
        return f"ALLOC {name}, {upl_type}\nSET {name}, {val}"

    def parse(self, code):
        # Limpieza de comentarios y normalización
        code = re.sub(r'//.*', '', code)
        code = re.sub(r'/\*.*?\*/', '', code, flags=re.DOTALL)
        
        lines = code.split('\n')
        translated = ["# C to UPL-IR Translation"]
        
        for line in lines:
            line = line.strip()
            if not line or line in ['{', '}']: continue
            
            applied = False
            for pattern, subst in self.rules:
                match = re.search(pattern, line)
                if match:
                    if callable(subst):
                        line = subst(match)
                    else:
                        line = re.sub(pattern, subst, line)
                    applied = True
                    break
            
            if applied:
                translated.append(line)
            else:
                # Si no hay regla, mantenemos el código pero quitamos el punto y coma final
                translated.append(line.rstrip(';'))
                
        return "\n".join(translated)

if __name__ == "__main__":
    parser = CParser()
    sample = """
    int main() {
        int x = 10;
        float y = 3.14;
        printf("Value: %d", x);
        return 0;
    }
    """
    print("📝 C -> UPL-IR:\n")
    print(parser.parse(sample))
