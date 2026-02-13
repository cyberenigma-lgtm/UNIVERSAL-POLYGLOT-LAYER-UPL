import re

class GenericParser:
    """
    UPL Generic Parser Template
    To be specialized for each language in the catalog.
    """
    def __init__(self, lang_name="Generic"):
        self.lang_name = lang_name
        self.rules = [] # To be populated by specific sub-parsers

    def parse(self, code):
        lines = code.split('\n')
        translated = [f"# {self.lang_name} to UPL-IR Translation"]
        for line in lines:
            line = line.strip()
            if not line or line.startswith(('#', '//', ';')): continue
            
            # Simple rule application (to be expanded)
            for pattern, subst in self.rules:
                if re.search(pattern, line):
                    line = re.sub(pattern, subst, line)
                    break
            translated.append(line)
        return "\n".join(translated)
