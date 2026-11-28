# MultiLang-ASM Language Pack: Javanese (jv)
# Metadata for the Babel Community system

METADATA = {
    "name": "Javanese / Basa Jawa",
    "code": "jv",
    "author": "Neuro-OS Community",
    "version": "1.0",
    "description": "Panyengkuyung lengkap kanggo assembler ing basa Jawa."
}

# Standard mapping: Native Keyword -> NASM Mnemonic
KEYWORDS = {
    # Pamindahan Data
    "pindah": "mov", "ijol": "xchg", "muat_alamat": "lea",
    "jembar_nol": "movzx", "jembar_tanda": "movsx",
    
    # Aritmetika
    "tambah": "add", "suda": "sub", "ping": "mul",
    "para": "div", "tambah_siji": "inc", "suda_siji": "dec",
    "mungkur": "neg",
    
    # Perbandingan
    "banding": "cmp", "uji": "test",
    
    # Aliran Kontrol
    "mlumpat": "jmp", "timbal": "call", "bali": "ret",
    "yen_podo": "je", "yen_nol": "jz", "yen_bedo": "jne",
    "yen_luwih": "jg", "yen_kurang": "jl",
    
    # Tumpukan (Stack)
    "lebokne": "push", "wetokne": "pop", "lebokne_gendero": "pushf", "wetokne_gendero": "popf",
    
    # Larik (String)
    "pindah_byte": "movsb", "simpen_byte": "stosb", "muat_byte": "lodsb",
    "baleni": "rep",
    
    # Gelung (Loop)
    "gelung": "loop", "gelung_yen_nol": "loopz",
    
    # Sistem
    "selo": "int", "timbal_sistem": "syscall", "bali_sistem": "sysret",
    "mandeg": "hlt", "ora_ono": "nop",
    
    # Konversi
    "byte_neng_word": "cbw", "word_neng_double": "cwd", "double_neng_quad": "cdq",
}

# Simplified/Kids mapping
KIDS_KEYWORDS = {
    "jv": {"deleh": "mov", "tambah": "add", "suda": "sub", "tuduhno": "syscall"},
}
