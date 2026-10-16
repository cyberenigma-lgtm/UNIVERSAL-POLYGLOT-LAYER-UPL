import re

class PerlParser:
    """
    Modular Perl Parser for UPL
    Translates Perl syntax to UPL-IR.
    """
    def __init__(self):
        self.rules = [
            # print "..." -> IO_WRITE CONSOLE, "..."
            (r'print\s+(.*);', r'IO_WRITE CONSOLE, \1'),
            # $x = 10; -> SET x, 10
            (r'\$(\w+)\s*=\s*(.*);', r'SET \1, \2'),
        ]

    def parse(self, code):
        lines = code.split('\n')
        translated = ["# Perl to UPL-IR Translation"]
        for line in lines:
            line = line.strip()
            if not line or line.startswith(('#', 'use ', 'package ')): continue
            
            for pattern, subst in self.rules:
                if re.search(pattern, line):
                    line = re.sub(pattern, subst, line)
                    break
            translated.append(line)
        return "\n".join(translated)

if __name__ == "__main__":
    parser = PerlParser()
    sample = "$val = 42; print $val;"
    print(parser.parse(sample))
