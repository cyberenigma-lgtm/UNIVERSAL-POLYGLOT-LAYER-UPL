import re

class JavaParser:
    """
    Modular Java Parser for UPL
    Translates Java syntax to UPL-IR.
    """
    def __init__(self):
        self.rules = [
            # System.out.println("...") -> IO_WRITE CONSOLE, "..."
            (r'System\.out\.println\s*\((.*)\);', r'IO_WRITE CONSOLE, \1'),
            # public static void main(String[] args) -> PROC main(args)
            (r'public\s+static\s+void\s+main\s*\(.*?\)', r'PROC main(args)'),
            # public void func() -> PROC func()
            (r'(?:public|private|protected)?\s+(?:\w+)\s+(\w+)\s*\((.*?)\)\s*\{', r'PROC \1(\2)'),
            # int x = 10; -> SET x, 10
            (r'(?:\w+)\s+(\w+)\s*=\s*(.*?);', r'SET \1, \2'),
            # return x; -> RET x
            (r'return\s+(.*?);', r'RET \1'),
        ]

    def parse(self, code):
        lines = code.split('\n')
        translated = ["# Java to UPL-IR Translation"]
        for line in lines:
            line = line.strip()
            # Omitir cabeceras de clase por ahora
            if line.startswith(("class ", "public class ", "package ", "import ")): continue
            if not line or line in ["{", "}"]: continue
            
            for pattern, subst in self.rules:
                if re.search(pattern, line):
                    line = re.sub(pattern, subst, line)
                    break
            translated.append(line)
        return "\n".join(translated)

if __name__ == "__main__":
    parser = JavaParser()
    sample = """
    public class Main {
        public static void main(String[] args) {
            int x = 100;
            System.out.println(x);
        }
    }
    """
    print("☕ Java -> UPL-IR:\n")
    print(parser.parse(sample))
