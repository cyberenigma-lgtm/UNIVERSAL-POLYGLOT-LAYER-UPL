import re

class UPLMixer:
    """
    UPL Mixer v1.0
    The core responsible for separating language blocks in a .upl file.
    """
    def __init__(self):
        # Pattern: looks for a language tag on a single line starting with # or //
        # or just the language name followed by a block.
        # Example: #python or //c or simply python
        self.block_pattern = re.compile(r'(?im)^(?:#|//|)?(\w+)\s*\n(.*?)(?=\n^(?:#|//|)?\w+\s*\n|\Z)', re.DOTALL)

    def split_blocks(self, content):
        """
        Splits content into (language, code_block) tuples.
        """
        content = content.replace('\r\n', '\n').strip()
        matches = self.block_pattern.finditer(content)
        blocks = []
        for match in matches:
            lang = match.group(1).lower()
            code = match.group(2).strip()
            blocks.append((lang, code))
        return blocks

if __name__ == "__main__":
    mixer = UPLMixer()
    test = """
#python
x = 10

//c
int y = 5;

asm
mov rax, 42
"""
    for lang, code in mixer.split_blocks(test):
        print(f"[{lang.upper()}] detected:\n{code}\n---")
