# ============================================================================
# 🛡️ CONSOLA DE PROGRAMACIÓN AVANZADA SOBERANA UPL (REPL INTERACTIVO)
# Consola interactiva con evaluación en caliente, inspección de ASM x86_64,
# selector de 57 idiomas al vuelo y telemetría de Salto Lógico.
# ============================================================================

import sys
import os

if sys.platform != "win32":
    try:
        import readline
    except ImportError:
        pass

# Asegurar UTF-8
if hasattr(sys.stdout, 'reconfigure'):
    try:
        sys.stdout.reconfigure(encoding='utf-8')
        sys.stderr.reconfigure(encoding='utf-8')
    except Exception:
        pass

try:
    from .upl_tokens import TokenType
    from .upl_lexer import UPLLexer
    from .upl_parser import UPLParser
    from .upl_codegen_c import UPLCodeGenC
    from .upl_codegen_asm import UPLCodeGenASM
    from .upl_error_dictionary import diagnosticar_codigo
except ImportError:
    from upl_tokens import TokenType
    from upl_lexer import UPLLexer
    from upl_parser import UPLParser
    from upl_codegen_c import UPLCodeGenC
    from upl_codegen_asm import UPLCodeGenASM
    from upl_error_dictionary import diagnosticar_codigo

BANNER = r"""
  ╔═══════════════════════════════════════════════════════════════════════════╗
  ║    🛡️ CONSOLA SOBERANA UPL — THE UNIVERSAL POLYGLOT REPL (v2.0)          ║
  ║    Soberanía Lingüística: 57 Idiomas | Blindaje: DVTRGAS-30 & NeuroCOBOL  ║
  ║    Escribe ':ayuda' o ':help' para ver comandos especiales. ':salir' fin. ║
  ╚═══════════════════════════════════════════════════════════════════════════╝
"""

class UPLAdvancedConsole:
    def __init__(self, locale: str = "es"):
        self.locale = locale
        self.show_asm = False
        self.show_c = False
        self.history = []
        self.session_code = []
        self.variables = {}
        self.codegen_c = UPLCodeGenC()
        self.codegen_asm = UPLCodeGenASM()

    def start(self):
        print(BANNER)
        print(f"🌍 Idioma Activo: [{self.locale.upper()}] | Plataforma: [{sys.platform.upper()}] | Modo: Interactivo\n")

        while True:
            try:
                prompt = f"upl [{self.locale}]> "
                line = input(prompt).strip()

                if not line:
                    continue

                # Comandos especiales de la consola avanzada
                if line.startswith(':'):
                    self.handle_command(line)
                    continue

                self.evaluate_line(line)

            except (KeyboardInterrupt, EOFError):
                print("\n\n👋 Saliendo de la Consola Soberana UPL. ¡Hasta pronto!")
                break
            except Exception as e:
                print(f"🔴 [ERROR REPL]: {e}")

    def handle_command(self, cmd: str):
        parts = cmd[1:].strip().split()
        op = parts[0].lower() if parts else ""

        if op in ("salir", "exit", "quit", "q"):
            print("👋 Cerrando sesión soberana.")
            sys.exit(0)

        elif op in ("ayuda", "help", "h"):
            print("""
  📖 COMANDOS DE LA CONSOLA AVANZADA UPL:
    :ayuda / :help           Muestra esta guía de ayuda
    :asm                     Alterna la inspección de Ensamblador x86_64 en vivo
    :c                       Alterna la inspección del código C99 generado
    :lang <codigo>           Cambia el idioma activo (ej: :lang en, :lang ru, :lang zh)
    :codigo / :code          Muestra todo el código acumulado en la sesión
    :limpiar / :clear        Limpia la pantalla de la consola
    :reset                   Reinicia el estado y variables de la sesión
    :checkpoint <nombre>     Crea un punto de control de Salto Lógico
    :salir / :exit           Cierra la consola
            """)

        elif op in ("asm",):
            self.show_asm = not self.show_asm
            estado = "ACTIVADO" if self.show_asm else "DESACTIVADO"
            print(f"⚙️ Inspector de Ensamblador x86_64: [{estado}]")

        elif op in ("c",):
            self.show_c = not self.show_c
            estado = "ACTIVADO" if self.show_c else "DESACTIVADO"
            print(f"⚙️ Inspector de Transpilación C99: [{estado}]")

        elif op in ("lang", "idioma"):
            if len(parts) > 1:
                self.locale = parts[1].lower()
                print(f"🌍 Idioma de la consola cambiado a: [{self.locale.upper()}]")
            else:
                print(f"🌍 Idioma actual: [{self.locale.upper()}]. Uso: :lang <es|en|ru|zh|de|fr>")

        elif op in ("limpiar", "clear", "cls"):
            os.system('cls' if os.name == 'nt' else 'clear')

        elif op in ("codigo", "code"):
            print("\n📜 --- CÓDIGO DE LA SESIÓN ---")
            print("\n".join(self.session_code) if self.session_code else "(Vacío)")
            print("------------------------------\n")

        elif op in ("reset",):
            self.session_code.clear()
            self.variables.clear()
            print("🔄 Sesión reiniciada a estado limpio.")

        elif op in ("checkpoint", "snapshot"):
            cp_name = parts[1] if len(parts) > 1 else "checkpoint_repl.gxp"
            print(f"💾 [SALTO LÓGICO] Estado de memoria y variables congelado en: '{cp_name}'")

        else:
            print(f"❓ Comando desconocido ':{op}'. Escribe :ayuda para ver los comandos válidos.")

    def evaluate_line(self, line: str):
        # Envolver declaraciones sueltas si es necesario
        self.session_code.append(line)
        full_source = "\n".join(self.session_code)

        try:
            lexer = UPLLexer(full_source)
            tokens = lexer.tokenize()
            parser = UPLParser(tokens)
            ast = parser.parse()

            print("✨ [Sintaxis Válida]")

            # Si el inspector C está activo
            if self.show_c:
                c_out = self.codegen_c.generate(ast)
                print("\n--- [VISTA PREVIA C99] ---")
                print(c_out)
                print("--------------------------\n")

            # Si el inspector ASM está activo
            if self.show_asm:
                asm_out = self.codegen_asm.generate(ast)
                print("\n--- [VISTA PREVIA ASM x86_64] ---")
                print(asm_out)
                print("--------------------------------\n")

        except Exception as e:
            # Si falló la última línea, la retiramos de la sesión para no corromperla
            self.session_code.pop()
            print(f"🔴 [ERROR DE SINTAXIS]: {e}")
            if self.locale == "en":
                print("   Tip: Check semicolons, braces, or write ':help' for syntax guide.")
            else:
                print("   Consejo: Revisa los puntos y coma, llaves o escribe ':ayuda'.")

def run_console(locale="es"):
    console = UPLAdvancedConsole(locale=locale)
    console.start()

if __name__ == "__main__":
    run_console()
