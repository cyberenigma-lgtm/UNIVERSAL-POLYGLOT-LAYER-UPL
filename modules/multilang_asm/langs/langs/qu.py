# MultiLang-ASM Language Pack: Quechua (qu)
# Metadata for the Babel Community system

METADATA = {
    "name": "Quechua / Runa Simi",
    "code": "qu",
    "author": "Neuro-OS Community",
    "version": "1.0",
    "description": "Tukuy yanapay assemblerpapaq Quechua simipi."
}

# Standard mapping: Native Keyword -> NASM Mnemonic
KEYWORDS = {
    # Datos Kuyun
    "astay": "mov", "trukay": "xchg", "dir_kargay": "lea",
    "chusaq_mastariy": "movzx", "unancha_mastariy": "movsx",
    
    # Yupaychay
    "yapay": "add", "qichuy": "sub", "mirachiy": "mul",
    "rakiniy": "div", "wiñachiy": "inc", "pisiyachiy": "dec",
    "pampachay": "neg",
    
    # Tupachiy
    "tupachiy": "cmp", "llanchiy": "test",
    
    # Kamachiy
    "paway": "jmp", "waqyay": "call", "kutimuy": "ret",
    "kaqlla_hina": "je", "chusaq_hina": "jz", "mana_kaqlla": "jne",
    "aswan_hatun": "jg", "aswan_uchuy": "jl",
    
    # Pila
    "winay": "push", "qurpuy": "pop", "unancha_winay": "pushf", "unancha_qurpuy": "popf",
    
    # Watasqakuna
    "byte_astay": "movsb", "byte_waqaychay": "stosb", "byte_kargay": "lodsb",
    "tikray": "rep",
    
    # Muyu
    "muyuy": "loop", "chusaq_muyuy": "loopz",
    
    # Llanu
    "samachiy": "int", "llanu_waqyay": "syscall", "llanu_kutimuy": "sysret",
    "sayachiy": "hlt", "imapas_mana": "nop",
    
    # Tikray (Convert)
    "byte_wordman": "cbw", "word_doubleman": "cwd", "double_quadman": "cdq",
}

# Simplified/Kids mapping
KIDS_KEYWORDS = {
    "qu": {"churay": "mov", "yapay": "add", "qichuy": "sub", "rikuchiy": "syscall"},
}
