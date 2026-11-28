# MultiLang-ASM Language Pack: Afrikaans (af)
# Metadata for the Babel Community system

METADATA = {
    "name": "Afrikaans",
    "code": "af",
    "author": "Neuro-OS Community",
    "version": "1.0",
    "description": "Volledige ondersteuning vir assembler in Afrikaans."
}

# Standard mapping: Native Keyword -> NASM Mnemonic
KEYWORDS = {
    # Databeweging
    "skuif": "mov", "ruil": "xchg", "laai_effektief": "lea",
    "brei_nul_uit": "movzx", "brei_teken_uit": "movsx",
    
    # Arithmetika
    "tel_by": "add", "trek_af": "sub", "vermenigvuldig": "mul",
    "deel": "div", "vermeerder": "inc", "verminder": "dec",
    "ontken": "neg",
    
    # Vergelyking
    "vergelyk": "cmp", "toets": "test",
    
    # Beheervloei
    "spring": "jmp", "roep": "call", "keer_terug": "ret",
    "as_gelyk": "je", "as_nul": "jz", "as_nie_gelyk": "jne",
    "as_groter": "jg", "as_kleiner": "jl",
    
    # Stapel
    "stoot": "push", "haal": "pop", "stoot_vlae": "pushf", "haal_vlae": "popf",
    
    # Stringe
    "skuif_greep": "movsb", "stoor_greep": "stosb", "laai_greep": "lodsb",
    "herhaal": "rep",
    
    # Lus
    "lus": "loop", "lus_as_nul": "loopz",
    
    # Stelsel
    "onderbreking": "int", "stelselroep": "syscall", "stelselterugkeer": "sysret",
    "stop": "hlt", "niks": "nop",
    
    # Omskakeling
    "greep_na_woord": "cbw", "woord_na_dubbel": "cwd", "dubbel_na_quad": "cdq",
}

# Simplified/Kids mapping
KIDS_KEYWORDS = {
    "af": {"sit": "mov", "plus": "add", "minus": "sub", "wys": "syscall"},
}
