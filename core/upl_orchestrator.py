# UPL Orchestrator v1.1.0 - Sovereign Edition
# Copyright (C) 2026 Neuro-OS Genesis
# Registered in the Intellectual Community of Neuro-OS Genesis.
# Derivative work of Neuro-OS Genesis source code.
# Licensed under GNU General Public License v3.0 (GPL-3)
# Future Notice: UPL v2 will be released under a private paid use license.

import os
import sys
import json

# Entorno soberano UPL 1.1
BASE_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
if BASE_DIR not in sys.path:
    sys.path.append(BASE_DIR)

from mixer.upl_mixer import UPLMixer
from core.upl_codegen import UPLCodeGen

# Arsenal de Parsers (26 lenguajes)
from parsers.python.python_parser import PythonParser
from parsers.c.c_parser import CParser
from parsers.js.javascript_parser import JSParser
from parsers.rust.rust_parser import RustParser
from parsers.go.go_parser import GoParser
from parsers.basic.basic_parser import BASICParser
from parsers.cobol.cobol_parser import COBOLParser
from parsers.java.java_parser import JavaParser
from parsers.cs.cs_parser import CSharpParser
from parsers.lisp.lisp_parser import LispParser
from parsers.swift.swift_parser import SwiftParser
from parsers.kotlin.kotlin_parser import KotlinParser
from parsers.typescript.typescript_parser import TSParser
from parsers.haskell.haskell_parser import HaskellParser
from parsers.julia.julia_parser import JuliaParser
from parsers.ruby.ruby_parser import RubyParser
from parsers.php.php_parser import PHPParser
from parsers.lua.lua_parser import LuaParser
from parsers.perl.perl_parser import PerlParser
from parsers.bash.bash_parser import BashParser
from parsers.elixir.elixir_parser import ElixirParser
from parsers.zig.zig_parser import ZigParser
from parsers.odin.odin_parser import OdinParser
from parsers.nim.nim_parser import NimParser
from parsers.crystal.crystal_parser import CrystalParser

class UPLOrchestrator:
    """
    UPL Orchestrator v1.1.0 - Sovereign Edition
    The first and only system in the world to unify 26+ languages with human localization.
    """
    def __init__(self, locale="en"):
        self.version = "1.1.0-sovereign"
        self.base_dir = BASE_DIR
        self.mixer = UPLMixer()
        self.codegen = UPLCodeGen()
        self.current_locale = locale
        
        # Cargar diccionarios de idiomas
        self.locales = self._load_locales()
        
        # Registros de Parsers
        self.parsers = {
            'py': PythonParser(), 'c': CParser(), 'js': JSParser(),
            'rs': RustParser(), 'go': GoParser(), 'bas': BASICParser(),
            'cob': COBOLParser(), 'java': JavaParser(), 'cs': CSharpParser(),
            'lisp': LispParser(), 'swift': SwiftParser(), 'kt': KotlinParser(),
            'ts': TSParser(), 'hs': HaskellParser(), 'jl': JuliaParser(),
            'rb': RubyParser(), 'php': PHPParser(), 'lua': LuaParser(),
            'pl': PerlParser(), 'sh': BashParser(), 'ex': ElixirParser(),
            'zig': ZigParser(), 'odin': OdinParser(), 'nim': NimParser(),
            'cr': CrystalParser()
        }
        print(f"🛡️ UPL Nucleus v{self.version} - {len(self.parsers)} Languages Ready.")

    def _load_locales(self):
        locale_path = os.path.join(self.base_dir, "core", "upl_locales.json")
        if os.path.exists(locale_path):
            with open(locale_path, 'r', encoding='utf-8') as f:
                return json.load(f)
        return {"en": {}}

    def localize_ir(self, ir_content):
        """Translates UPL-IR mnemonics to the selected human language."""
        if self.current_locale == "en": return ir_content
        
        mapping = self.locales.get(self.current_locale, {})
        for eng, term in mapping.items():
            ir_content = ir_content.replace(eng, term)
        return ir_content

    def compile_upl(self, content):
        ir = self.process_to_ir(content)
        localized_ir = self.localize_ir(ir)
        asm = self.codegen.translate(ir) # ASM always uses standard IR mnemonics
        return localized_ir, asm

    def process_to_ir(self, content):
        blocks = self.mixer.split_blocks(content)
        ir_results = []
        for lang_name, code in blocks:
            lang_id = self._identify_lang(lang_name)
            if lang_id in self.parsers:
                ir_results.append(self.parsers[lang_id].parse(code))
            else:
                ir_results.append(f"# BLOCK {lang_name.upper()}\n{code}")
        return "\n\n".join(ir_results)

    def _identify_lang(self, name):
        name = name.lower()
        mapping = {
            'python': 'py', 'py': 'py', 'javascript': 'js', 'js': 'js',
            'ruby': 'rb', 'rb': 'rb', 'php': 'php', 'lua': 'lua',
            'perl': 'pl', 'bash': 'sh', 'sh': 'sh', 'elixir': 'ex',
            'rust': 'rs', 'go': 'go', 'c': 'c', 'basic': 'bas', 'cobol': 'cob',
            'java': 'java', 'cs': 'cs', 'lisp': 'lisp', 'zig': 'zig', 
            'odin': 'odin', 'nim': 'nim', 'crystal': 'cr', 'cr': 'cr'
        }
        return mapping.get(name, name)

if __name__ == "__main__":
    # Test soberano en Español
    orch = UPLOrchestrator(locale="es")
    sample = """
zig
const x = 7;
std.debug.print("Zig logic", .{});

nim
echo "Nim logic"

python
print("Final unification")
"""
    ir, _ = orch.compile_upl(sample)
    print("\n--- TEST: ZIG + NIM + PYTHON (LOCALIZADO ES) ---")
    print(ir)
