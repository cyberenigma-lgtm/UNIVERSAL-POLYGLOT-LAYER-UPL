# MultiLang-ASM Language Pack: Catalan (ca)
# Metadata for the Babel Community system

METADATA = {
    "name": "Catalan / Català",
    "code": "ca",
    "author": "Neuro-OS Community",
    "version": "1.0",
    "description": "Suport complet per a l'assemblador en Català."
}

# Standard mapping: Native Keyword -> NASM Mnemonic
KEYWORDS = {
    # Moviment de Dades
    "moure": "mov", "intercanviar": "xchg", "carregar_efectiva": "lea",
    "estendre_zero": "movzx", "estendre_signe": "movsx",
    
    # Aritmètica
    "sumar": "add", "restar": "sub", "multiplicar": "mul",
    "dividir": "div", "incrementar": "inc", "decrementar": "dec",
    "negar": "neg",
    
    # Comparació
    "comparar": "cmp", "testar": "test",
    
    # Flux de Control
    "saltar": "jmp", "cridar": "call", "retornar": "ret",
    "si_igual": "je", "si_zero": "jz", "si_no_igual": "jne",
    "si_major": "jg", "si_menor": "jl",
    
    # Pila
    "empènyer": "push", "extreure": "pop", "empènyer_banderes": "pushf", "extreure_banderes": "popf",
    
    # Cadenes
    "moure_byte": "movsb", "emmagatzemar_byte": "stosb", "carregar_byte": "lodsb",
    "repetir": "rep",
    
    # Bucles
    "bucle": "loop", "bucle_si_zero": "loopz",
    
    # Sistema
    "interrupció": "int", "crida_sistema": "syscall", "retorn_sistema": "sysret",
    "aturar": "hlt", "res": "nop",
    
    # Conversió
    "byte_a_paraula": "cbw", "paraula_a_doble": "cwd", "doble_a_quad": "cdq",
}

# Simplified/Kids mapping
KIDS_KEYWORDS = {
    "ca": {"posa": "mov", "suma": "add", "treu": "sub", "mostra": "syscall"},
}
