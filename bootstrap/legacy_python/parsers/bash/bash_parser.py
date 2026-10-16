import re

class BashParser:
    """
    Modular Bash Parser for UPL
    Translates Shell syntax to UPL-IR.
    """
    def __init__(self):
        self.rules = [
            # echo "..." -> IO_WRITE CONSOLE, "..."
            (r'echo\s+(.*)', r'IO_WRITE CONSOLE, \1'),
            # x=10 -> SET x, 10
            (r'^(\w+)=(.*)', r'SET \1, \2'),
            # function name() { ... }
            (r'function\s+(\w+)\s*\(?\)?\s*\{', r'PROC \1()'),
        ]

    def parse(self, code):
        lines = code.split('\n')
        translated = ["# Bash to UPL-IR Translation"]
        for line in lines:
            line = line.strip()
            if not line or line.startswith(('#', '#!')): continue
            if line == "}": continue
            
            for pattern, subst in self.rules:
                if re.search(pattern, line):
                    line = re.sub(pattern, subst, line)
                    break
            translated.append(line)
        return "\n".join(translated)

if __name__ == "__main__":
    parser = BashParser()
    sample = "x=10\necho $x"
    print(parser.parse(sample))
