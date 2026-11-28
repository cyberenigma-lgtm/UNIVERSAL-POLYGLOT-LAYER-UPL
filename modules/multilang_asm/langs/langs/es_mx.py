# MultiLang-ASM Language Pack: Spanish (Mexico) (es_mx)
# Metadata for the Babel Community system

METADATA = {
    "name": "Spanish (Mexico) / Español (México)",
    "code": "es_mx",
    "author": "Neuro-OS Community",
    "version": "1.0",
    "description": "Variante mexicana del ensamblador en español."
}

# Standard mapping: Native Keyword -> NASM Mnemonic
KEYWORDS = {
    # Movimiento
    "mover": "mov", "copiar": "mov", "intercambiar": "xchg", "cargar": "mov",
    "cargar_dirección": "lea", "extender_cero": "movzx", "extender_signo": "movsx",
    
    # Aritmética
    "sumar": "add", "añadir": "add", "restar": "sub",
    "multiplicar": "mul", "multiplicar_signado": "imul",
    "dividir": "div", "dividir_signado": "idiv",
    "incrementar": "inc", "decrementar": "dec", "negar": "neg",
    
    # Lógica
    "y": "and", "o": "or", "no": "not", "exclusivo": "xor",
    "desplazar_izq": "shl", "desplazar_der": "shr",
    
    # Comparación
    "comparar": "cmp", "probar": "test",
    
    # Flujo
    "saltar": "jmp", "pueblos": "call", "regresar": "ret",
    "si_igual": "je", "si_cero": "jz", "si_no_igual": "jne",
    "si_mayor": "jg", "si_menor": "jl",
    
    # Pila
    "meter": "push", "sacar": "pop", "meter_flags": "pushf", "sacar_flags": "popf",
    
    # Cadenas
    "mover_byte": "movsb", "guardar_byte": "stosb", "cargar_byte": "lodsb",
    "repetir": "rep",
    
    # Bucles
    "ciclo": "loop", "ciclo_si_cero": "loopz",
    
    # Sistema
    "interrupcion": "int", "llamada_sistema": "syscall", "regreso_sistema": "sysret",
    "alto": "hlt", "nadamás": "nop",
    
    # Conversión
    "conv_byte_palabra": "cbw", "conv_palabra_doble": "cwd",
}

# Simplified/Kids mapping
KIDS_KEYWORDS = {
    "es_mx": {"pon": "mov", "suma": "add", "resta": "sub", "enseña": "syscall"},
}
