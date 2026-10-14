# MultiLang-ASM Language Pack: Malay (ms)
# Metadata for the Babel Community system

METADATA = {
    "name": "Malay / Bahasa Melayu",
    "code": "ms",
    "author": "Neuro-OS Community",
    "version": "1.0",
    "description": "Sokongan penuh untuk assembler dalam Bahasa Melayu."
}

# Standard mapping: Native Keyword -> NASM Mnemonic
KEYWORDS = {
    # Perpindahan Data
    "pindah": "mov", "tukar": "xchg", "muat_efektif": "lea",
    "luas_sifar": "movzx", "luas_tanda": "movsx",
    
    # Aritmetik
    "tambah": "add", "tolak": "sub", "darab": "mul",
    "bahagi": "div", "naik": "inc", "turun": "dec",
    "sangkal": "neg",
    
    # Perbandingan
    "banding": "cmp", "uji": "test",
    
    # Aliran Kawalan
    "lompat": "jmp", "panggil": "call", "kembali": "ret",
    "jika_sama": "je", "jika_sifar": "jz", "jika_beza": "jne",
    "jika_besar": "jg", "jika_kecil": "jl",
    
    # Timbunan
    "tolak_masuk": "push", "tarik_keluar": "pop", "tolak_bendera": "pushf", "tarik_bendera": "popf",
    
    # Rentetan
    "pindah_bait": "movsb", "simpan_bait": "stosb", "muat_bait": "lodsb",
    "ulang": "rep",
    
    # Gelung
    "gelung": "loop", "gelung_jika_sifar": "loopz",
    
    # Sistem
    "sampuk": "int", "panggilan_sistem": "syscall", "kembali_sistem": "sysret",
    "henti": "hlt", "kosong": "nop",
    
    # Penukaran
    "bait_ke_kata": "cbw", "kata_ke_kembar": "cwd", "kembar_ke_quad": "cdq",
}

# Simplified/Kids mapping
KIDS_KEYWORDS = {
    "ms": {"letak": "mov", "tambah": "add", "tolak": "sub", "tunjuk": "syscall"},
}
