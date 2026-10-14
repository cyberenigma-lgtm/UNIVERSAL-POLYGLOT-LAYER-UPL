# MultiLang-ASM Language Pack: Swedish / Svenska (sv)
# Metadata for the Babel Community system

METADATA = {
    "name": "Swedish / Svenska",
    "code": "sv",
    "author": "Neuro-OS Community",
    "version": "1.0",
    "description": "Full support for Assembly in Swedish."
}

# Standard mapping: Native Keyword -> NASM Mnemonic
KEYWORDS = {
    # Flytt
    "flytta": "mov", "byt": "xchg", "ladda_adress": "lea",
    
    # Aritmetik
    "addera": "add", "subtrahera": "sub", "multiplicera": "mul",
    "dividera": "div", "oka": "inc", "minska": "dec",
    "negera": "neg",
    
    # Jamforelse
    "jamfor": "cmp", "testa": "test",
    
    # Flode
    "hoppa": "jmp", "anropa": "call", "returnera": "ret",
    "om_lika": "je", "om_noll": "jz", "om_olika": "jne",
    "om_storre": "jg", "om_mindre": "jl",
    
    # Stack
    "lagg": "push", "ta": "pop", "lagg_flaggor": "pushf", "ta_flaggor": "popf",
    
    # Strangar
    "flytta_byte": "movsb", "spara_byte": "stosb", "ladda_byte": "lodsb",
    "repetera": "rep",
    
    # Loopar
    "loop": "loop", "loop_om_noll": "loopz",
    
    # System
    "avbrott": "int", "systemanrop": "syscall", "systemretur": "sysret",
    "stopp": "hlt", "inget": "nop",
}

# Simplified/Kids mapping
KIDS_KEYWORDS = {
    "sv": {"and": "mov", "plus": "add", "minus": "sub", "visa": "syscall"},
}
