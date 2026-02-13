# UPL CLI - Universal Polyglot Layer (Sovereign Command Line Interface)
# Copyright (C) 2026 Neuro-OS Genesis
# Registered in the Intellectual Community of Neuro-OS Genesis.
# Derivative work of Neuro-OS Genesis source code.
# Licensed under GNU General Public License v3.0 (GPL-3)
# Future Notice: UPL v2 will be released under a private paid use license.

import sys
import os
import argparse

# Configuración del entorno soberano 1.1
BASE_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
if BASE_DIR not in sys.path:
    sys.path.append(BASE_DIR)

try:
    from core.upl_orchestrator import UPLOrchestrator
except ImportError as e:
    print(f"❌ ERROR CRÍTICO: No se pudo cargar el núcleo UPL: {e}")
    sys.exit(1)

def main():
    parser = argparse.ArgumentParser(description="UPL CLI - Universal Polyglot Layer (Sovereign Edition)")
    parser.add_argument("file", help="El archivo .upl políglota a procesar.")
    parser.add_argument("-l", "--locale", default="en", help="Idioma de soberanía para el Lenguaje Madre (es, en, ru, zh, etc.).")
    parser.add_argument("-o", "--output", help="Archivo de salida (ASM). Por defecto se imprime el UPL-IR.")
    parser.add_argument("-v", "--version", action="version", version="UPL CLI v1.3 (Sovereign Edition)")
    
    args = parser.parse_args()

    if not os.path.exists(args.file):
        print(f"❌ ERROR: El archivo '{args.file}' no existe.")
        sys.exit(1)

    # Iniciar Orquestador con Soberanía Lingüística
    orch = UPLOrchestrator(locale=args.locale)
    
    try:
        with open(args.file, 'r', encoding='utf-8') as f:
            content = f.read()

        print(f"🚀 Procesando '{args.file}' con Soberanía [{args.locale.upper()}]...")
        ir, asm = orch.compile_upl(content)

        if args.output:
            with open(args.output, 'w', encoding='utf-8') as f:
                f.write(asm)
            print(f"✨ ASM generado y guardado en: {args.output}")
        else:
            print("\n--- [LENGUAJE MADRE (UPL-IR)] ---")
            print(ir)
            print("\n--- [ASSEMBLER (ASM)] ---")
            print(asm if asm else "No se generó ASM para este bloque.")
            
        print("\n✅ Operación finalizada con éxito.")

    except Exception as e:
        print(f"🔴 ERROR DURANTE LA UNIFICACIÓN: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main()
