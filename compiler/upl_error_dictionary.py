# ============================================================================
# 🛡️ DICCIONARIO MAESTRO GLOBAL MULTILINGÜE DE ERRORES Y DIAGNÓSTICO UPL
# Integración Soberana: MultiLang-ASM (57 idiomas) + NeuroUniversalASM (NUASM)
# Cobertura: Windows (NTSTATUS), Linux/POSIX (Signals), Bare-Metal x86_64 y Compilador UPL
# ============================================================================

import sys
import os

# 1. BASE DE DATOS DE ERRORES BILINGÜE / MULTILINGÜE
DB_ERRORS = {
    # --- WINDOWS NTSTATUS ---
    0: {
        "hex": "0x00000000",
        "tipo": "STATUS_SUCCESS",
        "es": {
            "titulo": "Ejecución Exitosa",
            "categoria": "Éxito",
            "descripcion": "El programa finalizó correctamente sin errores.",
            "solucion": "No requiere acción."
        },
        "en": {
            "titulo": "Execution Successful",
            "categoria": "Success",
            "descripcion": "The program finished normally without errors.",
            "solucion": "No action required."
        }
    },
    -1073741819: { # 0xC0000005
        "hex": "0xC0000005",
        "tipo": "STATUS_ACCESS_VIOLATION",
        "es": {
            "titulo": "Violación de Acceso a Memoria (Puntero Nulo / Dirección Ilegal)",
            "categoria": "Memoria / Punteros",
            "descripcion": "El hilo intentó leer o escribir en una página de memoria no mapeada o prohibida (ej: puntero nulo NULL o desbordamiento de buffer).",
            "solucion": "Verifica que tus punteros (*ptr) no sean 0. Usa la directiva @snapshot para activar los Landing-Pads de DVTRGAS-30 que previenen la desalineación."
        },
        "en": {
            "titulo": "Access Violation (Null Pointer / Illegal Memory Address)",
            "categoria": "Memory / Pointers",
            "descripcion": "The thread attempted to read/write from an unmapped or protected memory page (e.g., NULL pointer or buffer overflow).",
            "solucion": "Check that pointers (*ptr) are non-zero. Use @snapshot directive to enable DVTRGAS-30 landing pads preventing memory corruption."
        }
    },
    -1073741674: { # 0xC0000096
        "hex": "0xC0000096",
        "tipo": "STATUS_PRIVILEGED_INSTRUCTION",
        "es": {
            "titulo": "Instrucción Privilegiada (Conflicto de Ring-0 en Ring-3)",
            "categoria": "Silicio / Privilegios de CPU",
            "descripcion": "Intentaste ejecutar una instrucción que solo puede correr en Ring 0 (Modo Núcleo / Silicio), como 'cli', 'sti', 'inb', 'outb', 'hlt' o manipular 'cr3', dentro del espacio de usuario normal de Windows.",
            "solucion": "Esta lógica pertenece al modo Ring 0 / Bare-Metal. Compílala con 'upl build archivo.upl --target=asm --baremetal' para arrancar directamente en silicio."
        },
        "en": {
            "titulo": "Privileged Instruction (Ring-0 Conflict in Ring-3 Userland)",
            "categoria": "Silicon / CPU Privileges",
            "descripcion": "Attempted to execute an instruction reserved exclusively for Ring 0 (Kernel/Silicon Mode), such as 'cli', 'sti', 'inb', 'outb', 'hlt' or 'cr3' access, inside Windows userland.",
            "solucion": "This code belongs to Ring 0 / Bare-Metal systems. Compile it with 'upl build file.upl --target=asm --baremetal' to run natively on hardware."
        }
    },
    -1073741795: { # 0xC000001D
        "hex": "0xC000001D",
        "tipo": "STATUS_ILLEGAL_INSTRUCTION",
        "es": {
            "titulo": "Instrucción Ilegal (Opcode Inválido)",
            "categoria": "Hardware / CPU",
            "descripcion": "La CPU encontró un código de operación no válido o una extensión SIMD no soportada por este procesador.",
            "solucion": "Verifica las instrucciones en bloques 'asm { ... }' y asegúrate de no saltar a memoria de datos."
        },
        "en": {
            "titulo": "Illegal Instruction (Invalid Opcode)",
            "categoria": "Hardware / CPU",
            "descripcion": "The CPU encountered an invalid opcode or unsupported SIMD extension.",
            "solucion": "Check inline assembly in 'asm { ... }' blocks and ensure you are not jumping to data memory."
        }
    },
    -1073741571: { # 0xC00000FD
        "hex": "0xC00000FD",
        "tipo": "STATUS_STACK_OVERFLOW",
        "es": {
            "titulo": "Desbordamiento de Pila (Stack Overflow)",
            "categoria": "Pila de Ejecución",
            "descripcion": "La pila del hilo se agotó por recursión infinita o asignaciones gigantescas en variables locales.",
            "solucion": "Verifica la condición de salida de funciones recursivas o usa memoria dinámica / buffers globales."
        },
        "en": {
            "titulo": "Stack Overflow",
            "categoria": "Execution Stack",
            "descripcion": "Thread stack exhausted due to infinite recursion or massive local variable allocations.",
            "solucion": "Check recursive termination conditions or allocate dynamic/global memory instead of deep stack arrays."
        }
    },
    -1073741676: { # 0xC0000094
        "hex": "0xC0000094",
        "tipo": "STATUS_INTEGER_DIVIDE_BY_ZERO",
        "es": {
            "titulo": "División Entera por Cero",
            "categoria": "Aritmética",
            "descripcion": "Se intentó dividir o calcular residuo (%) con divisor igual a cero.",
            "solucion": "Añade 'if divisor != 0' o usa el tipo exacto 'comp3' de NeuroCOBOL."
        },
        "en": {
            "titulo": "Integer Division By Zero",
            "categoria": "Arithmetic",
            "descripcion": "Attempted to divide or modulo (%) by zero.",
            "solucion": "Check divisor != 0 or use exact decimal 'comp3' types from NeuroCOBOL."
        }
    },
    -1073741510: { # 0xC000013A
        "hex": "0xC000013A",
        "tipo": "STATUS_CONTROL_C_EXIT",
        "es": {
            "titulo": "Interrupción de Teclado (Ctrl+C)",
            "categoria": "Usuario",
            "descripcion": "El proceso fue terminado manualmente por el usuario mediante Ctrl+C.",
            "solucion": "Comportamiento normal de detención."
        },
        "en": {
            "titulo": "Control-C Termination",
            "categoria": "User",
            "descripcion": "Process was terminated manually by the user pressing Ctrl+C.",
            "solucion": "Normal user interrupt behavior."
        }
    },

    # --- SEÑALES LINUX / POSIX (128 + N) ---
    130: { # SIGINT
        "hex": "0x82",
        "tipo": "SIGINT (2)",
        "es": {
            "titulo": "POSIX: Interrupción por Teclado (SIGINT)",
            "categoria": "Señal",
            "descripcion": "Señal SIGINT enviada por Ctrl+C.",
            "solucion": "Detención normal."
        },
        "en": {
            "titulo": "POSIX: Keyboard Interrupt (SIGINT)",
            "categoria": "Signal",
            "descripcion": "SIGINT signal received via Ctrl+C.",
            "solucion": "Normal interrupt."
        }
    },
    132: { # SIGILL
        "hex": "0x84",
        "tipo": "SIGILL (4)",
        "es": {
            "titulo": "POSIX: Instrucción Ilegal (SIGILL)",
            "categoria": "Silicio",
            "descripcion": "Instrucción no permitida en espacio de usuario Linux.",
            "solucion": "Compila con --baremetal para el kernel de Neuro-OS."
        },
        "en": {
            "titulo": "POSIX: Illegal Instruction (SIGILL)",
            "categoria": "Silicon",
            "descripcion": "Instruction not allowed in Linux userland.",
            "solucion": "Compile with --baremetal for Neuro-OS kernel."
        }
    },
    134: { # SIGABRT
        "hex": "0x86",
        "tipo": "SIGABRT (6)",
        "es": {
            "titulo": "POSIX: Aborto Voluntario (SIGABRT)",
            "categoria": "Seguridad",
            "descripcion": "Llamada a abort() por aserción fallida o corrupción detectada.",
            "solucion": "Revisar aserciones previas a la llamada."
        },
        "en": {
            "titulo": "POSIX: Abort Signal (SIGABRT)",
            "categoria": "Security",
            "descripcion": "abort() triggered by failed assertion or detected memory corruption.",
            "solucion": "Check assert statements prior to crash."
        }
    },
    137: { # SIGKILL
        "hex": "0x89",
        "tipo": "SIGKILL (9)",
        "es": {
            "titulo": "POSIX: Liquidación por OOM-Killer (SIGKILL)",
            "categoria": "Memoria RAM",
            "descripcion": "El kernel de Linux liquidó el proceso por falta de memoria RAM física.",
            "solucion": "Optimizar estructuras o utilizar Salto Lógico para guardar a disco."
        },
        "en": {
            "titulo": "POSIX: Killed by OOM-Killer (SIGKILL)",
            "categoria": "RAM Memory",
            "descripcion": "Linux kernel terminated process due to out-of-memory condition.",
            "solucion": "Optimize memory allocations or use Salto Lógico checkpoints."
        }
    },
    139: { # SIGSEGV
        "hex": "0x8B",
        "tipo": "SIGSEGV (11)",
        "es": {
            "titulo": "POSIX: Violación de Segmentación (SIGSEGV)",
            "categoria": "Memoria / Punteros",
            "descripcion": "Fallo de segmentación: acceso a memoria prohibida o puntero nulo.",
            "solucion": "Revisar punteros y activar Landing-Pads de DVTRGAS-30 con @snapshot."
        },
        "en": {
            "titulo": "POSIX: Segmentation Fault (SIGSEGV)",
            "categoria": "Memory / Pointers",
            "descripcion": "Segmentation fault: access to invalid memory address or null pointer.",
            "solucion": "Validate pointers and activate DVTRGAS-30 Landing-Pads with @snapshot."
        }
    }
}

# 2. TABLA DE ERRORES NUASM (NeuroUniversalASM / MultiLang-ASM)
NUASM_ERRORS = {
    "E001": {"es": "Instrucción desconocida en el paquete de lenguaje activo.", "en": "Unknown instruction in active language pack."},
    "E002": {"es": "Operando inválido (registro o valor inmediato incorrecto).", "en": "Invalid operand (incorrect register or immediate value)."},
    "E003": {"es": "Salto fuera de rango en la arquitectura x86_64.", "en": "Jump out of range in x86_64 architecture."},
    "E004": {"es": "Etiqueta no definida en el bloque de código.", "en": "Undefined label in code block."},
    "E005": {"es": "Registro de CPU no válido para la operación.", "en": "Invalid CPU register for operation."},
    "E006": {"es": "Tamaño de operando incorrecto (mismatch de byte/word/dword/qword).", "en": "Incorrect operand size (byte/word/dword/qword mismatch)."},
    "E007": {"es": "Sintaxis de instrucción incorrecta.", "en": "Incorrect mnemonic syntax."},
    "E008": {"es": "Archivo fuente no encontrado.", "en": "Source file not found."},
    "E009": {"es": "Idioma no soportado en el motor de traducción.", "en": "Language not supported in translation engine."},
    "E010": {"es": "Formato de salida inválido.", "en": "Invalid output format."}
}

def diagnosticar_codigo(codigo_salida: int, os_plataforma: str = sys.platform, locale: str = "es") -> dict:
    """Diagnóstico universal en múltiples idiomas (es, en, ru, zh, etc.)"""
    lang = locale.lower()
    if lang not in ("es", "en"):
        # Respaldo a inglés para otros idiomas si no hay traducción completa
        lang = "en"

    # Buscar en la base de datos
    if codigo_salida in DB_ERRORS:
        entry = DB_ERRORS[codigo_salida]
        local_data = entry.get(lang, entry.get("en"))
        return {
            "estado": "CONOCIDO",
            "codigo": codigo_salida,
            "hex": entry["hex"],
            "tipo": entry["tipo"],
            "titulo": local_data["titulo"],
            "categoria": local_data["categoria"],
            "descripcion": local_data["descripcion"],
            "solucion": local_data["solucion"],
            "idioma": lang.upper()
        }

    # Código devuelto por el usuario / aplicación
    return {
        "estado": "RETORNO_USUARIO",
        "codigo": codigo_salida,
        "hex": hex(codigo_salida) if codigo_salida >= 0 else hex((1 << 32) + codigo_salida),
        "tipo": f"EXIT_CODE_{codigo_salida}",
        "titulo": f"Código de retorno del programa: {codigo_salida}" if lang == "es" else f"Program return code: {codigo_salida}",
        "categoria": "Lógica de Usuario" if lang == "es" else "User Logic",
        "descripcion": f"El programa retornó el valor numérico {codigo_salida} desde 'main()'." if lang == "es" else f"Program returned {codigo_salida} from 'main()'.",
        "solucion": "Si este valor no es un error intencional, revisa el valor de 'return' al final de tu función." if lang == "es" else "If this value is not intentional, verify the return statement at the end of your function.",
        "idioma": lang.upper()
    }
