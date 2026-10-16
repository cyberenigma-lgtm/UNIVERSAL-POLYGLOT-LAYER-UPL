# MultiLang-ASM Language Pack: Scottish English (en_sc)
# Metadata for the Babel Community system

METADATA = {
    "name": "Scottish English / Scots",
    "code": "en_sc",
    "author": "Neuro-OS Community",
    "version": "1.0",
    "description": "Scottish refined assembly keywords."
}

# Standard mapping: Native Keyword -> NASM Mnemonic
KEYWORDS = {
    # Movimiento
    "pit": "mov", "swap": "xchg", "fetch": "lea",
    
    # Aritmética
    "tot": "add", "dock": "sub", "multiply": "mul",
    "divide": "div", "heeze": "inc", "lowp": "dec",
    
    # Lógica
    "an": "and", "or": "or", "no": "not", "xor": "xor",
    
    # Comparación
    "neer": "cmp", "canny": "test",
    
    # Flujo
    "loup": "jmp", "ca": "call", "back": "ret",
    "gin_equal": "je", "gin_nought": "jz", "gin_no_equal": "jne",
    "gin_mair": "jg", "gin_wee": "jl",
    
    # Pila
    "shove": "push", "pull": "pop", "shove_flags": "pushf", "pull_flags": "popf",
    
    # Strings
    "shift_byte": "movsb", "stash_byte": "stosb", "grab_byte": "lodsb",
    "again": "rep",
    
    # Loops
    "birl": "loop", "birling_nought": "loopz",
    
    # System
    "haud": "int", "cry_system": "syscall", "gang_hame": "sysret",
    "wheesht": "hlt", "nowt": "nop",
    
    # Conversion
    "wee_to_big": "cbw", "big_to_mair": "cwd",
}

# Simplified/Kids mapping
KIDS_KEYWORDS = {
    "en_sc": {"pit": "mov", "tot": "add", "dock": "sub", "keek": "syscall"},
}
