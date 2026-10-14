#!/usr/bin/env python3
# ============================================================================
# 🛡️ UPL SOVEREIGN CLI — Herramienta Universal de Línea de Comandos
# Compatible con Windows, Linux, macOS y Entornos Embebidos
# ============================================================================

import sys
import os
import argparse
import subprocess
import shutil

# Asegurar codificación UTF-8 universal en terminales
if hasattr(sys.stdout, 'reconfigure'):
    try:
        sys.stdout.reconfigure(encoding='utf-8')
        sys.stderr.reconfigure(encoding='utf-8')
    except Exception:
        pass

# Ubicación del compilador interno
BASE_DIR = os.path.dirname(os.path.abspath(__file__))
COMPILER_DIR = os.path.join(BASE_DIR, "compiler")
if BASE_DIR not in sys.path:
    sys.path.insert(0, BASE_DIR)
if COMPILER_DIR not in sys.path:
    sys.path.insert(0, COMPILER_DIR)

from compiler import (
    UPLLexer,
    UPLParser,
    UPLCodeGenC,
    UPLCodeGenASM,
    diagnosticar_codigo,
    run_console
)

def detectar_compilador_host():
    """Detecta qué compilador de C nativo tiene instalado el usuario en su SO"""
    for comp in ["gcc", "clang", "cl", "tcc"]:
        if shutil.which(comp):
            return comp
    return None

def cmd_run(args):
    """Compila y ejecuta inmediatamente un archivo .upl en el SO actual"""
    source_file = args.file
    if not os.path.exists(source_file):
        print(f"❌ Error: El archivo '{source_file}' no existe.")
        sys.exit(1)

    compilador = detectar_compilador_host()
    if not compilador:
        print("🔴 Advertencia: No se detectó un compilador nativo (gcc/clang/tcc/cl) en el PATH.")
        print("   Instala GCC (MinGW en Windows o build-essential en Linux) para generar binarios.")
        sys.exit(1)

    print(f"🚀 [UPL] Ejecutando '{source_file}' usando motor [{compilador.upper()}] en {sys.platform.upper()}...")
    
    with open(source_file, "r", encoding="utf-8") as f:
        src = f.read()

    # 1. Compilar a C99
    lexer = UPLLexer(src)
    tokens = lexer.tokenize()
    parser = UPLParser(tokens)
    ast = parser.parse()
    codegen = UPLCodeGenC()
    c_code = codegen.generate(ast, is_baremetal=False)

    # 2. Guardar temporalmente y compilar
    base, _ = os.path.splitext(source_file)
    c_temp = base + ".tmp.c"
    exe_temp = base + (".exe" if os.name == "nt" else ".out")

    try:
        with open(c_temp, "w", encoding="utf-8") as f:
            f.write(c_code)

        # 3. Compilar con GCC / Clang
        if compilador in ("gcc", "clang", "tcc"):
            compile_cmd = [compilador, "-O2", c_temp, "-o", exe_temp]
        else: # MSVC cl
            compile_cmd = ["cl", "/O2", c_temp, f"/Fe:{exe_temp}"]

        res = subprocess.run(compile_cmd, capture_output=True, text=True)
        if res.returncode != 0:
            print(f"❌ Error durante la compilación nativa:\n{res.stderr}")
            sys.exit(1)

        # 4. Ejecutar el programa en el SO del usuario
        print(f"▶️ --- [SALIDA DE '{os.path.basename(source_file)}'] ---")
        run_res = subprocess.run([exe_temp])
        
        # 5. Diagnóstico Inteligente de Código de Salida Multi-SO (Soberanía Multilingüe)
        user_locale = getattr(args, "lang", "es")
        diag = diagnosticar_codigo(run_res.returncode, sys.platform, locale=user_locale)
        if run_res.returncode == 0:
            print(f"🏁 --- [PROGRAMA FINALIZADO CON ÉXITO: {diag['titulo']}] ---")
        else:
            print(f"🏁 --- [PROGRAMA FINALIZADO CON CÓDIGO {run_res.returncode}] ---")
            print(f"\n🔍 [DIAGNÓSTICO SOBERANO UPL - DESGLOSE DE ESTADO]")
            print(f"   ► Estado / Señal: {diag['titulo']}")
            print(f"   ► Hexadecimal:   {diag['hex']} (Decimal: {diag['codigo']})")
            print(f"   ► Descripción:   {diag['descripcion']}")
            print(f"   ► Solución:      {diag['solucion']}\n")

    finally:
        # Limpiar archivo temporal .c si se desea
        if os.path.exists(c_temp):
            try: os.remove(c_temp)
            except: pass

def cmd_build(args):
    """Genera un archivo compilado (binario o código intermedio)"""
    source_file = args.file
    if not os.path.exists(source_file):
        print(f"❌ Error: El archivo '{source_file}' no existe.")
        sys.exit(1)

    out_file = args.output
    target = args.target.lower()
    baremetal = args.baremetal

    with open(source_file, "r", encoding="utf-8") as f:
        src = f.read()

    print(f"⚙️ [UPL BUILD] Compilando '{source_file}' -> Target: {target.upper()} (Baremetal={baremetal})...")
    lexer = UPLLexer(src)
    tokens = lexer.tokenize()
    parser = UPLParser(tokens)
    ast = parser.parse()

    if target in ("c", "c99"):
        codegen = UPLCodeGenC()
        out_code = codegen.generate(ast, is_baremetal=baremetal)
        if not out_file: out_file = os.path.splitext(source_file)[0] + ".c"
        with open(out_file, "w", encoding="utf-8") as f: f.write(out_code)
        print(f"✅ Código C99 exportado a: {out_file}")

    elif target in ("asm", "x64", "nasm"):
        codegen = UPLCodeGenASM()
        out_code = codegen.generate(ast)
        if not out_file: out_file = os.path.splitext(source_file)[0] + ".asm"
        with open(out_file, "w", encoding="utf-8") as f: f.write(out_code)
        print(f"✅ Ensamblador x86_64 exportado a: {out_file}")

    elif target in ("bin", "native", "exe"):
        # Compilación directa a binario nativo del SO
        compilador = detectar_compilador_host()
        if not compilador:
            print("🔴 Se requiere GCC o Clang para generar binarios ejecutables.")
            sys.exit(1)
        
        codegen = UPLCodeGenC()
        c_code = codegen.generate(ast, is_baremetal=baremetal)
        temp_c = os.path.splitext(source_file)[0] + ".build.tmp.c"
        with open(temp_c, "w", encoding="utf-8") as f: f.write(c_code)

        if not out_file:
            out_file = os.path.splitext(source_file)[0] + (".exe" if os.name == "nt" else "")

        res = subprocess.run([compilador, "-O2", temp_c, "-o", out_file], capture_output=True, text=True)
        if os.path.exists(temp_c): os.remove(temp_c)

        if res.returncode != 0:
            print(f"❌ Error al construir binario:\n{res.stderr}")
            sys.exit(1)
        print(f"✅ Binario nativo generado exitosamente: {out_file}")

def main():
    parser = argparse.ArgumentParser(
        prog="upl",
        description="🛡️ UPL: El Lenguaje de Programación Soberano y Universal para el Siglo XXI",
        epilog="""
Ejemplos de uso en cualquier Sistema Operativo:
  upl run hola_mundo.upl                     (Ejecuta inmediatamente)
  upl build app.upl                          (Genera el ejecutable nativo del SO)
  upl build kernel.upl --target=asm --baremetal (Genera ensamblador para sistemas Bare-Metal / Ring-0)
        """
    )
    subparsers = parser.add_subparsers(dest="command", required=True)

    # Si se invoca 'upl' sin argumentos, arrancar la Consola de Programación Avanzada
    if len(sys.argv) == 1:
        run_console(locale="es")
        return

    # Subcomando: repl
    p_repl = subparsers.add_parser("repl", help="Iniciar la Consola de Programación Avanzada Interactiva (REPL)")
    p_repl.add_argument("-l", "--lang", default="es", help="Idioma de la consola (es, en, ru, zh, etc.)")
    p_repl.set_defaults(func=lambda args: run_console(locale=args.lang))

    # Subcomando: run
    p_run = subparsers.add_parser("run", help="Compilar y ejecutar archivo .upl en el SO actual")
    p_run.add_argument("file", help="Archivo .upl a ejecutar")
    p_run.add_argument("-l", "--lang", default="es", help="Idioma para diagnósticos y mensajes (es, en, ru, zh, etc.)")
    p_run.set_defaults(func=cmd_run)

    # Subcomando: diag
    def cmd_diag(args):
        from compiler.upl_error_dictionary import diagnosticar_codigo
        val_str = args.code.strip()
        try:
            if val_str.lower().startswith("0x"):
                raw_val = int(val_str, 16)
                if raw_val >= 0x80000000:
                    val = raw_val - 0x100000000
                else:
                    val = raw_val
            else:
                val = int(val_str)
        except ValueError:
            print(f"❌ Código inválido: {args.code}")
            return
        
        info = diagnosticar_codigo(val, locale=args.lang)
        print(f"\n🔍 [DIAGNÓSTICO SOBERANO UPL - {info.get('idioma', 'ES')}]")
        print(f"   ► Estado / Tipo:  {info.get('tipo', 'DESCONOCIDO')}")
        print(f"   ► Hexadecimal:    {info.get('hex', hex(val))}")
        print(f"   ► Categoría:      {info.get('categoria', 'General')}")
        print(f"   ► Título:         {info.get('titulo', '')}")
        print(f"   ► Causa:          {info.get('descripcion', '')}")
        print(f"   ► Solución:       {info.get('solucion', '')}\n")

    p_diag = subparsers.add_parser("diag", help="Diagnosticar un código de error de SO o CPU en 57 idiomas")
    p_diag.add_argument("code", help="Código de error en hexadecimal (ej: 0xC0000005) o decimal")
    p_diag.add_argument("-l", "--lang", default="es", help="Idioma para la explicación (es, en, ja, de, etc.)")
    p_diag.set_defaults(func=cmd_diag)

    args = parser.parse_args()
    args.func(args)

if __name__ == "__main__":
    main()
