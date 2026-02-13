import re

class NimParser:
    """
    Modular Nim Parser for UPL
    Translates Nim syntax to UPL-IR.
    """
    def __init__(self):
        self.rules = [
            # proc name(args) = -> PROC name(args)
            (r'proc\s+(\w+)\s*\((.*?)\).*?=', r'PROC \1(\2)'),
            # echo "..." -> IO_WRITE CONSOLE, "..."
            (r'echo\s+(.*)', r'IO_WRITE CONSOLE, \1'),
            # var/let x = 10 -> SET x, 10
            (r'(?:var|let)\s+(\w+)\s*=\s*(.*)', r'SET \1, \2'),
            # return x -> RET x
            (r'return\s+(.*)', r'RET \1'),
        ]

    def parse(self, code):
        lines = code.split('\n')
        translated = ["# Nim to UPL-IR Translation"]
        for line in lines:
            line = line.strip()
            if not line or line.startswith('#'): continue
            
            for pattern, subst in self.rules:
                if re.search(pattern, line):
                    line = re.sub(pattern, subst, line)
                    break
            translated.append(line)
        return "\n".join(translated)

if __name__ == "__main__":
    parser = NimParser()
    sample = "proc hello(n: string) = echo n"
    print(parser.parse(sample))
