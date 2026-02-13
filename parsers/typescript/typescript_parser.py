import re

class TSParser:
    """
    Modular TypeScript Parser for UPL
    Translates TypeScript syntax to UPL-IR.
    """
    def __init__(self):
        self.rules = [
            # function name(a: type): ret_type -> PROC name(a)
            (r'function\s+(\w+)\s*\((.*?)\)', r'PROC \1(\2)'),
            # console.log(...) -> IO_WRITE CONSOLE, ...
            (r'console\.log\s*\((.*)\);?', r'IO_WRITE CONSOLE, \1'),
            # const/let x: type = val -> SET x, val
            (r'(?:const|let|var)\s+(\w+)\s*(?::\s*\w+)?\s*=\s*(.*);?', r'SET \1, \2'),
            # return x -> RET x
            (r'return\s+(.*);?', r'RET \1'),
        ]

    def parse(self, code):
        lines = code.split('\n')
        translated = ["# TypeScript to UPL-IR Translation"]
        for line in lines:
            line = line.strip()
            if not line or line in ["{", "}"]: continue
            if line.startswith(("interface ", "type ", "enum ")): continue
            
            for pattern, subst in self.rules:
                if re.search(pattern, line):
                    line = re.sub(pattern, subst, line)
                    break
            translated.append(line)
        return "\n".join(translated)

if __name__ == "__main__":
    parser = TSParser()
    sample = "function hello(name: string): void { console.log(name); }"
    print(parser.parse(sample))
