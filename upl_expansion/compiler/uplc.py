#!/usr/bin/env python3
# ============================================================================
# UPLC — The Universal Polyglot Language Compiler (Sovereign Edition 2026)
# ============================================================================

import sys
import os
import argparse
import subprocess

# Asegurar UTF-8 en Windows
if hasattr(sys.stdout, 'reconfigure'):
    try:
        sys.stdout.reconfigure(encoding='utf-8')
        sys.stderr.reconfigure(encoding='utf-8')
    except Exception:
        pass

# Añadir directorio actual al path
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

from upl_lexer import UPLLexer
from upl_parser import UPLParser
from upl_codegen_c import UPLCodeGenC
from upl_codegen_asm import UPLCodeGenASM

def compile_source(source_text: str, target: str = "c", baremetal: bool = False) -> str:
    lexer = UPLLexer(source_text)
    tokens = lexer.tokenize()
    parser = UPLParser(tokens)
    ast = parser.parse()

    if target.lower() in ("c", "c99"):
        codegen = UPLCodeGenC()
        return codegen.generate(ast, is_baremetal=baremetal)
    elif target.lower() in ("asm", "nasm", "x64"):
        codegen = UPLCodeGenASM()
        return codegen.generate(ast)
    else:
        raise ValueError(f"Target desconocido: {target}. Opciones: 'c', 'asm'")

def main():
    parser = argparse.ArgumentParser(
        description="🛡️ UPLC — Compilador del Lenguaje Soberano Universal UPL",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Ejemplos de uso:
  uplc kernel.upl -o kernel.asm --target=asm --baremetal
  uplc programa.upl -o programa.c --target=c
  uplc programa.upl --run
        """
    )
    parser.add_argument("source", help="Archivo fuente .upl a compilar")
    parser.add_argument("-o", "--output", help="Ruta del archivo de salida")
    parser.add_argument("-t", "--target", default="c", choices=["c", "asm"], help="Target de compilación: 'c' (por defecto) o 'asm'")
    parser.add_argument("--baremetal", action="store_true", help="Modo Bare-Metal sin dependencias (ideal para kernels de SO)")
    parser.add_argument("--run", action="store_true", help="Compilar a C y ejecutar inmediatamente")

    args = parser.parse_args()

    if not os.path.exists(args.source):
        print(f"❌ Error: El archivo fuente '{args.source}' no existe.")
        sys.exit(1)

    with open(args.source, "r", encoding="utf-8") as f:
        src = f.read()

    print(f"🚀 [UPLC] Compilando '{args.source}' -> Target [{args.target.upper()}] (Baremetal={args.baremetal})...")
    try:
        compiled_code = compile_source(src, target=args.target, baremetal=args.baremetal)
    except Exception as e:
        print(f"🔴 Error durante la compilación: {e}")
        import traceback
        traceback.print_exc()
        sys.exit(1)

    out_path = args.output
    if not out_path:
        base, _ = os.path.splitext(args.source)
        ext = ".asm" if args.target == "asm" else ".c"
        out_path = base + ext

    with open(out_path, "w", encoding="utf-8") as f:
        f.write(compiled_code)

    print(f"✅ [UPLC] Generado con éxito: {out_path}")

    # Si se pide --run
    if args.run:
        exe_path = os.path.splitext(out_path)[0] + (".exe" if os.name == "nt" else "")
        print(f"⚙️ [UPLC] Compilando binario nativo con GCC: {exe_path}...")
        res = subprocess.run(["gcc", "-O2", out_path, "-o", exe_path], capture_output=True, text=True)
        if res.returncode != 0:
            print(f"❌ Error GCC:\n{res.stderr}")
            sys.exit(1)
        print(f"▶️ [UPLC] Ejecutando {exe_path}:\n")
        subprocess.run([exe_path])

if __name__ == "__main__":
    main()
