import re

class RubyParser:
    """
    Modular Ruby Parser for UPL
    Translates Ruby syntax to UPL-IR.
    """
    def __init__(self):
        self.rules = [
            # def name(args) -> PROC name(args)
            (r'def\s+(\w+)(?:\s*\((.*?)\))?', r'PROC \1(\2)'),
            # puts "..." -> IO_WRITE CONSOLE, "..."
            (r'puts\s+(.*)', r'IO_WRITE CONSOLE, \1'),
            # x = 10 -> SET x, 10
            (r'^(\w+)\s*=\s*(.*)', r'SET \1, \2'),
            # return x -> RET x
            (r'return\s+(.*)', r'RET \1'),
        ]

    def parse(self, code):
        lines = code.split('\n')
        translated = ["# Ruby to UPL-IR Translation"]
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
    parser = RubyParser()
    sample = "def greet(n)\nputs n\nend"
    print(parser.parse(sample))
