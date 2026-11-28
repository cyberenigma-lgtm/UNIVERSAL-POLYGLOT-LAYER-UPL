# MultiLang-ASM Language Pack: Indonesian (id)

METADATA = {
    "name": "Indonesian / Bahasa Indonesia",
    "code": "id",
    "author": "Neuro-OS Community",
    "version": "1.0",
    "description": "Dukungan penuh untuk assembler dalam Bahasa Indonesia."
}

KEYWORDS = {
    # Perpindahan Data
    "pindah": "mov", "salin": "mov", "tukar": "xchg",
    "muat_efektif": "lea", "perpanjang_nol": "movzx",
    "perpanjang_tanda": "movsx",
    
    # Aritmatika
    "tambah": "add", "jumlah": "add", "kurang": "sub",
    "kali": "mul", "kali_tanda": "imul",
    "bagi": "div", "bagi_tanda": "idiv",
    "tambah_satu": "inc", "kurang_satu": "dec", "negatif": "neg",
    
    # Perbandingan
    "bandingkan": "cmp", "uji": "test",
    
    # Kontrol Alur
    "lompat": "jmp", "panggil": "call", "kembali": "ret",
    "jika_sama": "je", "jika_nol": "jz", "jika_beda": "jne",
    "jika_lebih_besar": "jg", "jika_lebih_kecil": "jl",
    
    # Tumpukan
    "masukkan": "push", "keluarkan": "pop", "masukkan_flag": "pushf", "keluarkan_flag": "popf",
    
    # String
    "pindah_byte": "movsb", "simpan_byte": "stosb", "muat_byte": "lodsb",
    "ulang": "rep",
    
    # Loop
    "putaran": "loop", "putaran_jika_nol": "loopz",
    
    # Sistem
    "interupsi": "int", "panggilan_sistem": "syscall", "kembali_sistem": "sysret",
    "henti": "hlt", "kosong": "nop",
    
    # Konversi
    "byte_ke_word": "cbw", "word_ke_double": "cwd", "double_ke_quad": "cdq",
}

KIDS_KEYWORDS = {
    "id": {"taruh": "mov", "tambah": "add", "kurang": "sub", "tampil": "syscall"},
}
