# MultiLang-ASM Language Pack: Maori (mi)
# Metadata for the Babel Community system

METADATA = {
    "name": "Maori / Te Reo Māori",
    "code": "mi",
    "author": "Neuro-OS Community",
    "version": "1.0",
    "description": "Tautoko katoa mo te assembler i te reo Māori."
}

# Standard mapping: Native Keyword -> NASM Mnemonic
KEYWORDS = {
    # Nekehanga Raraunga
    "neke": "mov", "fari": "xchg", "uta_nohoanga": "lea",
    "whakawhanui_kore": "movzx", "whakawhanui_tohu": "movsx",
    
    # Arithmetiki
    "tāpiri": "add", "tango": "sub", "whakarea": "mul",
    "wehe": "div", "whakapiki": "inc", "whakapalo": "dec",
    "whakakore": "neg",
    
    # Whakataurite
    "whakataurite": "cmp", "whakamātau": "test",
    
    # Rere Whakahaere
    "peke": "jmp", "karanga": "call", "hoki": "ret",
    "mehemea_rite": "je", "mehemea_kore": "jz", "mehemea_kore_rite": "jne",
    "mehemea_nui": "jg", "mehemea_iti": "jl",
    
    # Tāpae (Stack)
    "tene": "push", "unu": "pop", "tene_tohu": "pushf", "unu_tohu": "popf",
    
    # Miro (String)
    "neke_byte": "movsb", "pūmau_byte": "stosb", "uta_byte": "lodsb",
    "tukurua": "rep",
    
    # Koropiko (Loop)
    "koropiko": "loop", "koropiko_mehemea_kore": "loopz",
    
    # Pūnaha
    "aukati": "int", "karanga_pūnaha": "syscall", "hoki_pūnaha": "sysret",
    "whakamutu": "hlt", "kahore": "nop",
    
    # Hurihanga (Convert)
    "byte_ki_focal": "cbw", "focal_ki_double": "cwd", "double_ki_quad": "cdq",
}

# Simplified/Kids mapping
KIDS_KEYWORDS = {
    "mi": {"whakatakoto": "mov", "tāpiri": "add", "tango": "sub", "whakaatu": "syscall"},
}
