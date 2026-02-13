import re

class JuliaParser:
    """
    Modular Julia Parser for UPL
    Translates Julia syntax to UPL-IR (Tier 4).
    """
    def __init__(self):
        self.rules = [
            # function name(args) -> PROC name(args)
            (r'function\s+(\w+)\s*\((.*?)\)', r'PROC \1(\2)'),
            # println(...) -> IO_WRITE CONSOLE, ...
            (r'println\s*\((.*)\)', r'IO_WRITE CONSOLE, \1'),
            # x = 10 -> SET x, 10
            (r'^(\w+)\s*=\s*(.*)', r'SET \1, \2'),
        ]

    def parse(self, code):
        lines = code.split('\n')
        translated = ["# Julia to UPL-IR Translation"]
        for line in lines:
            line = line.strip()
            if not line or line == "end" or line.startswith('#'): continue
            
            for pattern, subst in self.rules:
                if re.search(pattern, line):
                    line = re.sub(pattern, subst, line)
                    break
            translated.append(line)
        return "\n".join(translated)

if __name__ == "__main__":
    parser = JuliaParser()
    sample = "function calc(n) println(n) end"
    print(parser.parse(sample))
