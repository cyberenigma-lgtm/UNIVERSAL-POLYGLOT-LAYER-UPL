# MultiLang-ASM Language Pack: Polish / Polski (pl)
# Metadata for the Babel Community system

METADATA = {
    "name": "Polish / Polski",
    "code": "pl",
    "author": "Neuro-OS Community",
    "version": "1.0",
    "description": "Full support for Assembly in Polish."
}

# Standard mapping: Native Keyword -> NASM Mnemonic
KEYWORDS = {
    # Ruch
    "przenies": "mov", "zamien": "xchg", "zaladuj_adres": "lea",
    
    # Arytmetyka
    "dodaj": "add", "odejmij": "sub", "pomnoz": "mul",
    "podziel": "div", "zwieksz": "inc", "zmniejsz": "dec",
    "zaneguj": "neg",
    
    # Porownanie
    "porownaj": "cmp", "testuj": "test",
    
    # Sterowanie
    "skocz": "jmp", "wolaj": "call", "wroc": "ret",
    "jesli_rowne": "je", "jesli_zero": "jz", "jesli_nierowne": "jne",
    "jesli_wieksze": "jg", "jesli_mniejsze": "jl",
    
    # Stos
    "poloz": "push", "zdejmij": "pop", "poloz_flagi": "pushf", "zdejmij_flagi": "popf",
    
    # Lancuchy
    "przenies_bajt": "movsb", "zapisz_bajt": "stosb", "laduj_bajt": "lodsb",
    "powtarzaj": "rep",
    
    # Petle
    "petla": "loop", "petla_jesli_zero": "loopz",
    
    # System
    "przerwanie": "int", "wywolanie_systemowe": "syscall", "powrot_systemowy": "sysret",
    "stop": "hlt", "nic": "nop",
    
    # Konwersja
    "bajt_na_slowo": "cbw", "slowo_na_podwojne": "cwd",
}

# Simplified/Kids mapping
KIDS_KEYWORDS = {
    "pl": {"poloz": "mov", "dodaj": "add", "zabierz": "sub", "pokaz": "syscall"},
}
