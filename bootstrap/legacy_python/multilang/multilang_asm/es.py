# MultiLang-ASM Language Pack: Spanish (es)
# Metadata for the Babel Community system

METADATA = {
    "name": "Spanish / Español",
    "code": "es",
    "author": "Neuro-OS Community",
    "version": "1.0",
    "description": "Soporte completo para ensamblador en español, incluyendo dialectos regionales."
}

# Standard mapping: Native Keyword -> NASM Mnemonic
KEYWORDS = {
    # Movimiento
    "mover": "mov", "copiar": "mov", "intercambiar": "xchg", "cargar": "mov",
    "cargar_efectivo": "lea", "extender_cero": "movzx", "extender_signo": "movsx",
    
    # Aritmética
    "sumar": "add", "añadir": "add", "agregar": "add", "restar": "sub",
    "multiplicar": "mul", "multiplicar_signado": "imul",
    "dividir": "div", "dividir_signado": "idiv",
    "incrementar": "inc", "decrementar": "dec", "negar": "neg",
    
    # Lógica
    "y": "and", "o": "or", "no": "not", "exclusivo": "xor",
    "desplazar_izq": "shl", "desplazar_der": "shr",
    "desplazar_arit_izq": "sal", "desplazar_arit_der": "sar",
    "rotar_izq": "rol", "rotar_der": "ror",
    
    # Comparación
    "comparar": "cmp", "probar": "test",
    
    # Flujo
    "saltar": "jmp", "llamar": "call", "retornar": "ret", "volver": "ret",
    "si_igual": "je", "si_cero": "jz", "si_no_igual": "jne", "si_no_cero": "jnz",
    "si_mayor": "jg", "si_mayor_igual": "jge", "si_menor": "jl", "si_menor_igual": "jle",
    "si_arriba": "ja", "si_abajo": "jb", "si_arriba_igual": "jae", "si_abajo_igual": "jbe",
    "si_signo": "js", "si_no_signo": "jns", "si_desborde": "jo", "si_no_desborde": "jno",
    "si_paridad": "jp", "si_no_paridad": "jnp",
    
    # Pila
    "meter": "push", "sacar": "pop", "meter_banderas": "pushf", "sacar_banderas": "popf",
    
    # Cadenas
    "mover_byte": "movsb", "mover_palabra": "movsw", "mover_doble": "movsd",
    "almacenar_byte": "stosb", "cargar_byte": "lodsb", "escanear_byte": "scasb",
    "repetir": "rep", "repetir_mientras": "repne",
    
    # Bucles
    "ciclo": "loop", "ciclo_si_cero": "loopz", "ciclo_si_no_cero": "loopnz",
    
    # Sistema
    "interrupcion": "int", "llamada_sistema": "syscall",
    "retorno_sistema": "sysret", "retorno_interrupcion": "iret", "retorno_interrupcion_q": "iretq",
    
    # Miscelánea
    "nada": "nop", "detener": "hlt", "esperar": "wait",
    "limpiar_interrupciones": "cli", "activar_interrupciones": "sti",
    "limpiar_direccion": "cld", "fijar_direccion": "std",
    
    # Conversión
    "convertir_byte_palabra": "cbw", "convertir_palabra_doble": "cwd",
    "convertir_doble_cuadruple": "cdq", "convertir_cuadruple_octo": "cqo",
}

# Simplified/Kids mapping
KIDS_KEYWORDS = {
    "es": {"pon": "mov", "suma": "add", "resta": "sub", "enseña": "syscall"},
    "es_ast": {"pon": "mov", "amesta": "add", "resta": "sub", "amuesa": "syscall"},
    "es_val": {"posa": "mov", "suma": "add", "resta": "sub", "mostra": "syscall"},
    "es_and": {"pon": "mov", "suma": "add", "resta": "sub", "mira": "syscall"},
    "es_mad": {"apanca": "mov", "suma": "add", "pilla": "sub", "lipa": "syscall"},
    "es_sev": {"pon": "mov", "suma": "add", "resta": "sub", "mira": "syscall"},
    "es_gad": {"pon": "mov", "suma": "add", "resta": "sub", "mira": "syscall"},
}
