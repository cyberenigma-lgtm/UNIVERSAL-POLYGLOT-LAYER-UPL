import re

class CSharpParser:
    """
    Modular C# Parser for UPL
    Translates C# syntax to UPL-IR.
    """
    def __init__(self):
        self.rules = [
            # Console.WriteLine("...") -> IO_WRITE CONSOLE, "..."
            (r'Console\.WriteLine\s*\((.*)\);', r'IO_WRITE CONSOLE, \1'),
            # static void Main() -> PROC main()
            (r'static\s+void\s+Main\s*\(.*?\)', r'PROC main()'),
            # void Method(Type arg) -> PROC Method(arg)
            (r'(?:public|private|static)?\s+(?:\w+)\s+(\w+)\s*\((.*?)\)\s*\{', r'PROC \1(\2)'),
            # var x = 10; o int x = 10;
            (r'(?:\w+)\s+(\w+)\s*=\s*(.*?);', r'SET \1, \2'),
            # return x;
            (r'return\s+(.*?);', r'RET \1'),
        ]

    def parse(self, code):
        lines = code.split('\n')
        translated = ["# C# to UPL-IR Translation"]
        for line in lines:
            line = line.strip()
            if line.startswith(("using ", "namespace ", "class ", "public class ")): continue
            if not line or line in ["{", "}"]: continue
            
            for pattern, subst in self.rules:
                if re.search(pattern, line):
                    line = re.sub(pattern, subst, line)
                    break
            translated.append(line)
        return "\n".join(translated)

if __name__ == "__main__":
    parser = CSharpParser()
    sample = """
    class Program {
        static void Main() {
            var val = 500;
            Console.WriteLine(val);
        }
    }
    """
    print("🎯 C# -> UPL-IR:\n")
    print(parser.parse(sample))
