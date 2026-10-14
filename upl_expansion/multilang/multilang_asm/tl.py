# MultiLang-ASM Language Pack: Tagalog (tl)
# Metadata for the Babel Community system

METADATA = {
    "name": "Tagalog / Filipino",
    "code": "tl",
    "author": "Neuro-OS Community",
    "version": "1.0",
    "description": "Buong suporta para sa assembler sa Tagalog."
}

# Standard mapping: Native Keyword -> NASM Mnemonic
KEYWORDS = {
    # Paglipat ng Datos
    "ilipat": "mov", "pagpalit": "xchg", "i-load": "lea",
    "palawakin_zero": "movzx", "palawakin_sign": "movsx",
    
    # Aritmetika
    "idagdag": "add", "ibawas": "sub", "i-multiply": "mul",
    "i-divide": "div", "itaas": "inc", "ibaba": "dec",
    "i-negate": "neg",
    
    # Paghahambing
    "ihambing": "cmp", "subukan": "test",
    
    # Daloy ng Kontrol
    "tumalon": "jmp", "itawag": "call", "bumalik": "ret",
    "kung_pareho": "je", "kung_zero": "jz", "kung_hindi_pareho": "jne",
    "kung_mas_malaki": "jg", "kung_mas_maliit": "jl",
    
    # Stack
    "itulak": "push", "kunin": "pop", "itulak_flag": "pushf", "kunin_flag": "popf",
    
    # String
    "ilipat_byte": "movsb", "i-save_byte": "stosb", "i-load_byte": "lodsb",
    "ulitin": "rep",
    
    # Loop
    "ikot": "loop", "ikot_kung_zero": "loopz",
    
    # System
    "abala": "int", "tawag_system": "syscall", "balik_system": "sysret",
    "itigil": "hlt", "wala": "nop",
    
    # Konbersyon
    "byte_sa_word": "cbw", "word_sa_double": "cwd", "double_sa_quad": "cdq",
}

# Simplified/Kids mapping
KIDS_KEYWORDS = {
    "tl": {"ilagay": "mov", "dagdag": "add", "bawas": "sub", "ipakita": "syscall"},
}
