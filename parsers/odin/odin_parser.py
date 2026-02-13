import re

class OdinParser:
    """
    Modular Odin Parser for UPL
    Translates Odin syntax to UPL-IR.
    """
    def __init__(self):
        self.rules = [
            # name :: proc(args) -> PROC name(args)
            (r'(\w+)\s*::\s*proc\((.*?)\)', r'PROC \1(\2)'),
            # fmt.println(...) -> IO_WRITE CONSOLE, ...
            (r'fmt\.println\s*\((.*)\)', r'IO_WRITE CONSOLE, \1'),
            # x := 10 -> SET x, 10
            (r'(\w+)\s*:=\s*(.*)', r'SET \1, \2'),
            # x : int = 10 -> SET x, 10
            (r'(\w+)\s*:\s*\w+\s*=\s*(.*)', r'SET \1, \2'),
        ]

    def parse(self, code):
        lines = code.split('\n')
        translated = ["# Odin to UPL-IR Translation"]
        for line in lines:
            line = line.strip()
            if not line or line in ["{", "}", "package main", "import \"core:fmt\""]: continue
            
            for pattern, subst in self.rules:
                if re.search(pattern, line):
                    line = re.sub(pattern, subst, line)
                    break
            translated.append(line)
        return "\n".join(translated)

if __name__ == "__main__":
    parser = OdinParser()
    sample = "main :: proc() { x := 10; fmt.println(x) }"
    print(parser.parse(sample))
