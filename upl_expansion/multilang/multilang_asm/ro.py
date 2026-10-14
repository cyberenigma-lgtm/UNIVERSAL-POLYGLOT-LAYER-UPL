# MultiLang-ASM Language Pack: Romanian (ro)
# Metadata for the Babel Community system

METADATA = {
    "name": "Romanian / Română",
    "code": "ro",
    "author": "Neuro-OS Community",
    "version": "1.0",
    "description": "Suport complet pentru asamblor în limba română."
}

# Standard mapping: Native Keyword -> NASM Mnemonic
KEYWORDS = {
    # Mișcare Date
    "muta": "mov", "schimba": "xchg", "incarca_efectiv": "lea",
    "extinde_zero": "movzx", "extinde_semn": "movsx",
    
    # Aritmetică
    "aduna": "add", "scade": "sub", "inmulteste": "mul",
    "imparte": "div", "incrementeaza": "inc", "decrementeaza": "dec",
    "neaga": "neg",
    
    # Comparare
    "compara": "cmp", "testeaza": "test",
    
    # Flux de Control
    "sari": "jmp", "apeleaza": "call", "returneaza": "ret",
    "daca_egal": "je", "daca_zero": "jz", "daca_nu_egal": "jne",
    "daca_mai_mare": "jg", "daca_mai_mic": "jl",
    
    # Stivă
    "pune": "push", "scoate": "pop", "pune_steguri": "pushf", "scoate_steguri": "popf",
    
    # Șiruri
    "muta_byte": "movsb", "stocheaza_byte": "stosb", "incarca_byte": "lodsb",
    "repeta": "rep",
    
    # Bucle
    "bucla": "loop", "bucla_daca_zero": "loopz",
    
    # Sistem
    "intrerupere": "int", "apel_sistem": "syscall", "retur_sistem": "sysret",
    "opreste": "hlt", "nimic": "nop",
    
    # Conversie
    "byte_la_word": "cbw", "word_la_double": "cwd", "double_la_quad": "cdq",
}

# Simplified/Kids mapping
KIDS_KEYWORDS = {
    "ro": {"pune": "mov", "plus": "add", "minus": "sub", "arata": "syscall"},
}
