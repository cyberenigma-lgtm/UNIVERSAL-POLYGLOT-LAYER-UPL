import re

class ZigParser:
    """
    Modular Zig Parser for UPL
    Translates Zig syntax to UPL-IR.
    """
    def __init__(self):
        self.rules = [
            # fn name(args) ret_type { -> PROC name(args)
            (r'fn\s+(\w+)\s*\((.*?)\)\s*.*?\s*\{', r'PROC \1(\2)'),
            # std.debug.print("...", .{}) -> IO_WRITE CONSOLE, "..."
            (r'std\.debug\.print\s*\((.*?),.*?\);', r'IO_WRITE CONSOLE, \1'),
            # const x = 10; -> SET x, 10
            (r'(?:const|var)\s+(\w+)\s*(?::\s*.*?)?\s*=\s*(.*?);', r'SET \1, \2'),
            # return x; -> RET x
            (r'return\s+(.*?);', r'RET \1'),
        ]

    def parse(self, code):
        lines = code.split('\n')
        translated = ["# Zig to UPL-IR Translation"]
        for line in lines:
            line = line.strip()
            if not line or line in ["{", "}"]: continue
            if line.startswith(("const std =", "pub fn ")): 
                # Handle pub fn special case
                line = line.replace("pub ", "")
            
            for pattern, subst in self.rules:
                if re.search(pattern, line):
                    line = re.sub(pattern, subst, line)
                    break
            translated.append(line)
        return "\n".join(translated)

if __name__ == "__main__":
    parser = ZigParser()
    sample = "const x = 5; std.debug.print(\"val: {}\", .{x});"
    print(parser.parse(sample))
