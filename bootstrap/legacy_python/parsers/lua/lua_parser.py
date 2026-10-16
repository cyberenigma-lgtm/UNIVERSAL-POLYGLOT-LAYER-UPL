import re

class LuaParser:
    """
    Modular Lua Parser for UPL
    Translates Lua syntax to UPL-IR.
    """
    def __init__(self):
        self.rules = [
            # function name(args) -> PROC name(args)
            (r'function\s+(\w+)\s*\((.*?)\)', r'PROC \1(\2)'),
            # print(...) -> IO_WRITE CONSOLE, ...
            (r'print\s*\((.*)\)', r'IO_WRITE CONSOLE, \1'),
            # x = 10 -> SET x, 10
            (r'^(\w+)\s*=\s*(.*)', r'SET \1, \2'),
            # local x = 10 -> SET x, 10
            (r'local\s+(\w+)\s*=\s*(.*)', r'SET \1, \2'),
        ]

    def parse(self, code):
        lines = code.split('\n')
        translated = ["# Lua to UPL-IR Translation"]
        for line in lines:
            line = line.strip()
            if not line or line == "end" or line.startswith('--'): continue
            
            for pattern, subst in self.rules:
                if re.search(pattern, line):
                    line = re.sub(pattern, subst, line)
                    break
            translated.append(line)
        return "\n".join(translated)

if __name__ == "__main__":
    parser = LuaParser()
    sample = "local x = 5\nprint(x)"
    print(parser.parse(sample))
