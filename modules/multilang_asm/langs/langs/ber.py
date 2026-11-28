# MultiLang-ASM Language Pack: Berber / Tamazight (ber)
# Metadata for the Babel Community system

METADATA = {
    "name": "Berber / Tamazight",
    "code": "ber",
    "author": "Neuro-OS Community",
    "version": "1.0",
    "description": "Annar n uhellil s Tamazight."
}

# Standard mapping: Native Keyword -> NASM Mnemonic
KEYWORDS = {
    # Amazig n Isfka
    "sihel": "mov", "senfel": "xchg", "ader_tansa": "lea",
    "semɣer_ilem": "movzx", "semɣer_tamatart": "movsx",
    
    # Isiuḍen
    "rnu": "add", "ekkes": "sub", "sget": "mul",
    "bḍu": "div", "simɣer": "inc", "sidrus": "dec",
    "agi": "neg",
    
    # Amzeri
    "zer": "cmp", "arem": "test",
    
    # Azmam n Usawal
    "neḍ": "jmp", "siwel": "call", "uɣal": "ret",
    "ma_igda": "je", "ma_ilem": "jz", "ma_ur_igda": "jne",
    "ma_imeqqer": "jg", "ma_imeẓẓi": "jl",
    
    # Agraw (Stack)
    "seres": "push", "sekker": "pop", "seres_tamatart": "pushf", "sekker_tamatart": "popf",
    
    # Izeḍwan (Strings)
    "sihel_byte": "movsb", "ḥerz_byte": "stosb", "ader_byte": "lodsb",
    "als": "rep",
    
    # Tummer (Loop)
    "azmam": "loop", "azmam_ma_ilem": "loopz",
    
    # Anagraw (System)
    "fukk": "int", "siwel_unagraw": "syscall", "uɣal_unagraw": "sysret",
    "ḥbes": "hlt", "walat": "nop",
    
    # Tiferkit (Convert)
    "byte_ar_tguri": "cbw", "taguri_ar_tusna": "cwd", "tusna_ar_quad": "cdq",
}

# Simplified/Kids mapping
KIDS_KEYWORDS = {
    "ber": {"sers": "mov", "rnu": "add", "ekkes": "sub", "mel": "syscall"},
}
