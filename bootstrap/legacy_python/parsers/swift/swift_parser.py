import re

class SwiftParser:
    """
    Modular Swift Parser for UPL
    Translates Swift syntax to UPL-IR.
    """
    def __init__(self):
        self.rules = [
            # func name(a: Type) -> PROC name(a)
            (r'func\s+(\w+)\s*\((.*?)\)', r'PROC \1(\2)'),
            # print("...") -> IO_WRITE CONSOLE, "..."
            (r'print\s*\((.*)\)', r'IO_WRITE CONSOLE, \1'),
            # let/var x = 10 -> SET x, 10
            (r'(?:let|var)\s+(\w+)\s*(?::\s*\w+)?\s*=\s*(.*)', r'SET \1, \2'),
            # return x -> RET x
            (r'return\s+(.*)', r'RET \1'),
        ]

    def parse(self, code):
        lines = code.split('\n')
        translated = ["# Swift to UPL-IR Translation"]
        for line in lines:
            line = line.strip()
            if not line or line in ["{", "}"]: continue
            
            for pattern, subst in self.rules:
                if re.search(pattern, line):
                    line = re.sub(pattern, subst, line)
                    break
            translated.append(line)
        return "\n".join(translated)

if __name__ == "__main__":
    parser = SwiftParser()
    sample = "func test() { let x = 10; print(x) }"
    print(parser.parse(sample))
