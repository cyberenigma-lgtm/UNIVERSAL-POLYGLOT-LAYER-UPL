import re

class PHPParser:
    """
    Modular PHP Parser for UPL
    Translates PHP syntax to UPL-IR.
    """
    def __init__(self):
        self.rules = [
            # function name(args) -> PROC name(args)
            (r'function\s+(\w+)\s*\((.*?)\)', r'PROC \1(\2)'),
            # echo "..." -> IO_WRITE CONSOLE, "..."
            (r'echo\s+(.*);', r'IO_WRITE CONSOLE, \1'),
            # $x = 10; -> SET x, 10
            (r'\$(\w+)\s*=\s*(.*);', r'SET \1, \2'),
            # return $x; -> RET x
            (r'return\s+\$(\w+);', r'RET \1'),
        ]

    def parse(self, code):
        lines = code.split('\n')
        translated = ["# PHP to UPL-IR Translation"]
        for line in lines:
            line = line.strip()
            if not line or line.startswith(('<?php', '?>', '//', '#')): continue
            
            for pattern, subst in self.rules:
                if re.search(pattern, line):
                    line = re.sub(pattern, subst, line)
                    break
            translated.append(line)
        return "\n".join(translated)

if __name__ == "__main__":
    parser = PHPParser()
    sample = "<?php $x = 10; echo $x; ?>"
    print(parser.parse(sample))
