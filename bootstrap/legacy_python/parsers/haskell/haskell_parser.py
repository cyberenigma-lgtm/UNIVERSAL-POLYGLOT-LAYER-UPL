import re

class HaskellParser:
    """
    Modular Haskell Parser for UPL
    Translates Haskell syntax to UPL-IR.
    """
    def __init__(self):
        self.rules = [
            # puts "..." -> IO_WRITE CONSOLE, "..." (Simplified IO)
            (r'putStrLn\s+(.*)', r'IO_WRITE CONSOLE, \1'),
            # let x = 10 -> SET x, 10
            (r'let\s+(\w+)\s*=\s*(.*)', r'SET \1, \2'),
            # name args = body -> PROC name(args)\nSET result, body
            (r'^(\w+)\s+(.*?)\s*=\s*(.*)', r'PROC \1(\2)\nSET result, \3'),
        ]

    def parse(self, code):
        lines = code.split('\n')
        translated = ["# Haskell to UPL-IR Translation"]
        for line in lines:
            line = line.strip()
            if not line or line.startswith('--'): continue
            
            for pattern, subst in self.rules:
                if re.search(pattern, line):
                    line = re.sub(pattern, subst, line)
                    break
            translated.append(line)
        return "\n".join(translated)

if __name__ == "__main__":
    parser = HaskellParser()
    sample = "suma a b = a + b"
    print(parser.parse(sample))
