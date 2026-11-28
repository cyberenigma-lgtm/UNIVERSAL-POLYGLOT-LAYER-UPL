# MultiLang-ASM Language Pack: Sundanese (su)
# Metadata for the Babel Community system

METADATA = {
    "name": "Sundanese / Basa Sunda",
    "code": "su",
    "author": "Neuro-OS Community",
    "version": "1.0",
    "description": "Pangrojong lengkep pikeun assembler dina basa Sunda."
}

# Standard mapping: Native Keyword -> NASM Mnemonic
KEYWORDS = {
    # Mindahkeun Data
    "pindah": "mov", "genti": "xchg", "muat_alamat": "lea",
    "lega_nol": "movzx", "lega_tanda": "movsx",
    
    # Aritmetika
    "tambah": "add", "kurang": "sub", "kali": "mul",
    "bagi": "div", "naek": "inc", "turun": "dec",
    "ingkar": "neg",
    
    # Babandingan
    "banding": "cmp", "uji": "test",
    
    # Aliran Kontrol
    "luncat": "jmp", "ngagero": "call", "balik": "ret",
    "lamun_sarua": "je", "lamun_nol": "jz", "lamun_beda": "jne",
    "lamun_gede": "jg", "lamun_leutik": "jl",
    
    # Tumpukan (Stack)
    "asupkeun": "push", "aluarkeun": "pop", "asup_bandera": "pushf", "aluar_bandera": "popf",
    
    # Runtuyan (String)
    "pindah_byte": "movsb", "simpen_byte": "stosb", "muat_byte": "lodsb",
    "ulang": "rep",
    
    # Gelung (Loop)
    "gelung": "loop", "gelung_lamun_nol": "loopz",
    
    # Sistem
    "sela": "int", "gero_sistem": "syscall", "balik_sistem": "sysret",
    "eureun": "hlt", "kosong": "nop",
    
    # Konversi
    "byte_ka_kecap": "cbw", "kecap_ka_dua": "cwd", "dua_ka_opat": "cdq",
}

# Simplified/Kids mapping
KIDS_KEYWORDS = {
    "su": {"teundeun": "mov", "tambah": "add", "kurang": "sub", "témbongkeun": "syscall"},
}
