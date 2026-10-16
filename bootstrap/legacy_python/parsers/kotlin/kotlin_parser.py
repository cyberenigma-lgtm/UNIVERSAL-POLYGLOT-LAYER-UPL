import re

class KotlinParser:
    """
    Modular Kotlin Parser for UPL
    Translates Kotlin syntax to UPL-IR.
    """
    def __init__(self):
        self.rules = [
            # fun name(args) -> PROC name(args)
            (r'fun\s+(\w+)\s*\((.*?)\)', r'PROC \1(\2)'),
            # println("...") -> IO_WRITE CONSOLE, "..."
            (r'println\s*\((.*)\)', r'IO_WRITE CONSOLE, \1'),
            # val/var x = 10 -> SET x, 10
            (r'(?:val|var)\s+(\w+)\s*(?::\s*\w+)?\s*=\s*(.*)', r'SET \1, \2'),
            # return x -> RET x
            (r'return\s+(.*)', r'RET \1'),
        ]

    def parse(self, code):
        lines = code.split('\n')
        translated = ["# Kotlin to UPL-IR Translation"]
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
    parser = KotlinParser()
    sample = "fun main() { val msg = \"Core\"; println(msg) }"
    print(parser.parse(sample))
