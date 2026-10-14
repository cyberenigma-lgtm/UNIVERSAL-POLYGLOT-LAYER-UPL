# MultiLang-ASM Language Pack: Norwegian (no)
# Metadata for the Babel Community system

METADATA = {
    "name": "Norwegian / Norsk",
    "code": "no",
    "author": "Neuro-OS Community",
    "version": "1.0",
    "description": "Full støtte for assembler på norsk."
}

# Standard mapping: Native Keyword -> NASM Mnemonic
KEYWORDS = {
    # Databevegelse
    "flytt": "mov", "veksle": "xchg", "last_effektiv": "lea",
    "utvid_null": "movzx", "utvid_tegn": "movsx",
    
    # Aritmetikk
    "legg_til": "add", "trekk_fra": "sub", "multipliser": "mul",
    "divider": "div", "øk": "inc", "minsk": "dec",
    "neger": "neg",
    
    # Sammenligning
    "sammenlign": "cmp", "test": "test",
    
    # Kontrollflyt
    "hopp": "jmp", "kall": "call", "returner": "ret",
    "hvis_lik": "je", "hvis_null": "jz", "hvis_ikke_lik": "jne",
    "hvis_større": "jg", "hvis_mindre": "jl",
    
    # Stabel
    "skyv": "push", "hent": "pop", "skyv_flagg": "pushf", "hent_flagg": "popf",
    
    # Strenger
    "flytt_byte": "movsb", "lagre_byte": "stosb", "last_byte": "lodsb",
    "gjenta": "rep",
    
    # Løkker
    "løkke": "loop", "løkke_hvis_null": "loopz",
    
    # System
    "avbrudd": "int", "systemkall": "syscall", "systemretur": "sysret",
    "stopp": "hlt", "ingen": "nop",
    
    # Konvertering
    "byte_til_ord": "cbw", "ord_til_dobbel": "cwd", "dobbel_til_quad": "cdq",
}

# Simplified/Kids mapping
KIDS_KEYWORDS = {
    "no": {"sett": "mov", "pluss": "add", "minus": "sub", "vis": "syscall"},
}
