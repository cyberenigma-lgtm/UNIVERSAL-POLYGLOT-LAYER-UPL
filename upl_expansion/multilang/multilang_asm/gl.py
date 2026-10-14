# MultiLang-ASM Language Pack: Galician (gl)
# Metadata for the Babel Community system

METADATA = {
    "name": "Galician / Galego",
    "code": "gl",
    "author": "Neuro-OS Community",
    "version": "1.0",
    "description": "Soporte completo para o ensamblador en Galego."
}

# Standard mapping: Native Keyword -> NASM Mnemonic
KEYWORDS = {
    # Movemento de Datos
    "mover": "mov", "intercambiar": "xchg", "cargar_efectiva": "lea",
    "estender_cero": "movzx", "estender_signo": "movsx",
    
    # Aritmética
    "sumar": "add", "restar": "sub", "multiplicar": "mul",
    "dividir": "div", "incrementar": "inc", "decrementar": "dec",
    "negar": "neg",
    
    # Comparación
    "comparar": "cmp", "probar": "test",
    
    # Fluxo de Control
    "saltar": "jmp", "chamar": "call", "retornar": "ret",
    "se_igual": "je", "se_cero": "jz", "se_non_igual": "jne",
    "se_maior": "jg", "se_menor": "jl",
    
    # Pila
    "meter": "push", "sacar": "pop", "meter_bandeiras": "pushf", "sacar_bandeiras": "popf",
    
    # Cadeas
    "mover_byte": "movsb", "almacenar_byte": "stosb", "cargar_byte": "lodsb",
    "repetir": "rep",
    
    # Bucles
    "bucle": "loop", "bucle_se_cero": "loopz",
    
    # Sistema
    "interrupción": "int", "chamada_sistema": "syscall", "retorno_sistema": "sysret",
    "deter": "hlt", "nada": "nop",
    
    # Conversión
    "byte_a_palabra": "cbw", "palabra_a_dobre": "cwd", "dobre_a_quad": "cdq",
}

# Simplified/Kids mapping
KIDS_KEYWORDS = {
    "gl": {"pon": "mov", "suma": "add", "resta": "sub", "amosa": "syscall"},
}
