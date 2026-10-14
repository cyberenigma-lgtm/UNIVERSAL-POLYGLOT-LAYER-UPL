# MultiLang-ASM Language Pack: Nahuatl (nah)
# Metadata for the Babel Community system

METADATA = {
    "name": "Nahuatl / Mexicano",
    "code": "nah",
    "author": "Neuro-OS Community",
    "version": "1.0",
    "description": "Nahuatlahtolli tlapalhuiliztli assembler-techcopa."
}

# Standard mapping: Native Keyword -> NASM Mnemonic
KEYWORDS = {
    # Tlapalhuiliztli
    "huica": "mov", "patla": "xchg", "tlatzacuia_maca": "lea",
    "ay_tic_tlapalo": "movzx", "unancha_tlapalo": "movsx",
    
    # Tlapohualiztli
    "tlapohua": "add", "tlatzacuia": "sub", "miquilia": "mul",
    "xelua": "div", "chicahua": "inc", "ahcic": "dec",
    "amo": "neg",
    
    # Tlanonotzaliztli
    "tlanonotza": "cmp", "yehuatl": "test",
    
    # Tlanequiliztli
    "choloa": "jmp", "notza": "call", "cuepa": "ret",
    "neneuhqui": "je", "ayac_neneuh": "jz", "amo_neneuh": "jne",
    "hueyi": "jg", "tepitzin": "jl",
    
    # Tlatzacuilli
    "tlatzacuilli_huica": "push", "quixtia": "pop", "flag_huica": "pushf", "flag_quixtia": "popf",
    
    # Tlahtolli
    "byte_huica": "movsb", "tlatzacuia_byte": "stosb", "kargay_byte": "lodsb",
    "yancuic": "rep",
    
    # Malacatl
    "malacatl": "loop", "ayac_malacatl": "loopz",
    
    # Altepetl
    "tzacuia": "int", "tequitl_notza": "syscall", "tequitl_cuepa": "sysret",
    "uactia": "hlt", "amo_itla": "nop",
    
    # Tikray (Convert)
    "byte_wordtech": "cbw", "word_doubletech": "cwd", "double_quadtech": "cdq",
}

# Simplified/Kids mapping
KIDS_KEYWORDS = {
    "nah": {"tlalia": "mov", "tlapohua": "add", "tlatzacuia": "sub", "itla_nextia": "syscall"},
}
