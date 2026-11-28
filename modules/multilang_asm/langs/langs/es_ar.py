# MultiLang-ASM Language Pack: Spanish (Argentina) (es_ar)
# Metadata for the Babel Community system

METADATA = {
    "name": "Spanish (Argentina) / Español (Argentina)",
    "code": "es_ar",
    "author": "Neuro-OS Community",
    "version": "1.0",
    "description": "Variante argentina del ensamblador en español."
}

# Standard mapping: Native Keyword -> NASM Mnemonic
KEYWORDS = {
    # Movimiento
    "mover": "mov", "copiar": "mov", "intercambiar": "xchg", "cargar": "mov",
    "mandar": "mov", "pasar": "mov",
    
    # Aritmética
    "sumar": "add", "restar": "sub",
    "multiplicar": "mul", "dividir": "div",
    "aumentar": "inc", "bajar": "dec",
    
    # Lógica
    "y": "and", "o": "or", "no": "not",
    
    # Comparación
    "comparar": "cmp", "chequear": "test",
    
    # Flujo
    "saltar": "jmp", "llamar": "call", "volver": "ret",
    "si_es_igual": "je", "si_es_cero": "jz", "si_es_distinto": "jne",
    "si_es_mayor": "jg", "si_es_menor": "jl",
    
    # Pila
    "empujar": "push", "sacar": "pop", "mandar_flags": "pushf", "traer_flags": "popf",
    
    # Cadenas
    "mover_byte": "movsb", "guardar_byte": "stosb", "cargar_byte": "lodsb",
    "repetir": "rep",
    
    # Bucles
    "vuelta": "loop", "vuelta_si_cero": "loopz",
    
    # Sistema
    "atencion": "int", "aviso_sistema": "syscall", "chau_sistema": "sysret",
    "cortar": "hlt", "nada": "nop",
    
    # Conversión
    "che_byte_a_word": "cbw", "word_a_doble": "cwd",
}

# Simplified/Kids mapping
KIDS_KEYWORDS = {
    "es_ar": {"pone": "mov", "sumale": "add", "restale": "sub", "mostra": "syscall"},
}
