# MultiLang-ASM Language Pack: Aymara (ay)
# Metadata for the Babel Community system

METADATA = {
    "name": "Aymara",
    "code": "ay",
    "author": "Neuro-OS Community",
    "version": "1.0",
    "description": "Taqpacha yanapa assembler-taki Aymara aruna."
}

# Standard mapping: Native Keyword -> NASM Mnemonic
KEYWORDS = {
    # Yatiña Unancha
    "kuna": "mov", "turkaña": "xchg", "katuña": "lea",
    "ch'usa_mast'aña": "movzx", "unancha_mast'aña": "movsx",
    
    # Jakhuña
    "yapaña": "add", "apaqaña": "sub", "mirayaña": "mul",
    "jaljaña": "div", "jilayaña": "inc", "pisiyaña": "dec",
    "jani_arunayaña": "neg",
    
    # Kipkaña
    "kipkaña": "cmp", "yant'aña": "test",
    
    # Kamachiña
    "t'it'iña": "jmp", "jawsaña": "call", "kutiña": "ret",
    "kipka_hina": "je", "ch'usa_hina": "jz", "jani_kipka": "jne",
    "aswan_jach'a": "jg", "aswan_jisk'a": "jl",
    
    # Pila
    "uchuqaña": "push", "apsuña": "pop", "unancha_uchuqa": "pushf", "unancha_apsu": "popf",
    
    # Mayachata
    "byte_kuna": "movsb", "byte_imayaña": "stosb", "byte_katuña": "lodsb",
    "kutiyaña": "rep",
    
    # Muyu
    "muyuta": "loop", "ch'usa_muyuta": "loopz",
    
    # Sistema
    "samaña": "int", "sistema_jawsaña": "syscall", "sistema_kutiña": "sysret",
    "sayayaña": "hlt", "jani_kunnasa": "nop",
    
    # Tikraña (Convert)
    "byte_wordtech": "cbw", "word_doubletech": "cwd", "double_quadtech": "cdq",
}

# Simplified/Kids mapping
KIDS_KEYWORDS = {
    "ay": {"uchuña": "mov", "yapaña": "add", "apaqaña": "sub", "uñachaya": "syscall"},
}
