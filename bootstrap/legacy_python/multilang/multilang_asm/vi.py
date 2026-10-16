# MultiLang-ASM Language Pack: Vietnamese / Tiếng Việt (vi)
# Metadata for the Babel Community system

METADATA = {
    "name": "Vietnamese / Tiếng Việt",
    "code": "vi",
    "author": "Neuro-OS Community",
    "version": "1.0",
    "description": "Full support for Assembly in Vietnamese."
}

# Standard mapping: Native Keyword -> NASM Mnemonic
KEYWORDS = {
    # Movimiento
    "chuyen": "mov", "trao_doi": "xchg", "nap_dia_chi": "lea",
    
    # Aritmética
    "cong": "add", "tru": "sub", "nhan": "mul",
    
    # So sánh
    "so_sanh": "cmp", "kiem_tra": "test",
    
    # Luồng điều khiển
    "nhay": "jmp", "goi": "call", "tra_ve": "ret",
    "neu_bang": "je", "neu_khong": "jz", "neu_khac": "jne",
    "neu_lon_hon": "jg", "neu_nho_hon": "jl",
    
    # Ngăn xếp
    "day_vao": "push", "lay_ra": "pop", "day_co": "pushf", "lay_co": "popf",
    
    # Chuỗi
    "di_chuyen_byte": "movsb", "luu_byte": "stosb", "tai_byte": "lodsb",
    "lap_lai": "rep",
    
    # Vòng lặp
    "vong_lap": "loop", "vong_lap_neu_khong": "loopz",
    
    # Hệ thống
    "ngat": "int", "goi_he_thong": "syscall", "tra_ve_he_thong": "sysret",
    "dung": "hlt", "khong_lam_gi": "nop",
    
    # Chuyển đổi
    "byte_sang_word": "cbw", "word_sang_double": "cwd",
}

# Simplified/Kids mapping
KIDS_KEYWORDS = {
    "vi": {"dat": "mov", "them": "add", "tru": "sub", "hien": "syscall"},
}
