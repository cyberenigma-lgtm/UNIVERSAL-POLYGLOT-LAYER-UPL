# MultiLang-ASM Language Pack: Dutch / Nederlands (nl)
# Metadata for the Babel Community system

METADATA = {
    "name": "Dutch / Nederlands",
    "code": "nl",
    "author": "Neuro-OS Community",
    "version": "1.0",
    "description": "Full support for Assembly in Dutch."
}

# Standard mapping: Native Keyword -> NASM Mnemonic
KEYWORDS = {
    # Verplaatsing
    "verplaats": "mov", "wissel": "xchg", "laad_adres": "lea",
    
    # Rekenkunde
    "optellen": "add", "aftrekken": "sub", "vermenigvuldigen": "mul",
    "delen": "div", "verhogen": "inc", "verlagen": "dec",
    "ontkennen": "neg",
    
    # Vergelijking
    "vergelijk": "cmp", "test": "test",
    
    # Stroming
    "spring": "jmp", "roep": "call", "keer_terug": "ret",
    "als_gelijk": "je", "als_nul": "jz", "als_niet_gelijk": "jne",
    "als_groter": "jg", "als_kleiner": "jl",
    
    # Stapel
    "duw": "push", "trek": "pop", "duw_vlaggen": "pushf", "trek_vlaggen": "popf",
    
    # Strings
    "verplaats_byte": "movsb", "sla_byte_op": "stosb", "laad_byte": "lodsb",
    "herhaal": "rep",
    
    # Lussen
    "lus": "loop", "lus_als_nul": "loopz",
    
    # Systeem
    "onderbreking": "int", "systeemaanroep": "syscall", "systeem_terug": "sysret",
    "stop": "hlt", "niets": "nop",
}

# Simplified/Kids mapping
KIDS_KEYWORDS = {
    "nl": {"zet": "mov", "plus": "add", "min": "sub", "toon": "syscall"},
}
