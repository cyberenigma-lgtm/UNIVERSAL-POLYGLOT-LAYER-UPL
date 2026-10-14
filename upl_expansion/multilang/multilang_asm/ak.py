# MultiLang-ASM Language Pack: Akan (ak)
# Metadata for the Babel Community system

METADATA = {
    "name": "Akan",
    "code": "ak",
    "author": "Neuro-OS Community",
    "version": "1.0",
    "description": "Mmoatowa mma assembler wɔ Akan kasa mu."
}

# Standard mapping: Native Keyword -> NASM Mnemonic
KEYWORDS = {
    # Nsɛm Akwantu
    "tu": "mov", "sesa": "xchg", "fa_atadi": "lea",
    "trɛw_hwee": "movzx", "trɛw_nsɛnkyerɛnne": "movsx",
    
    # Akontaabu
    "ka_ho": "add", "yi_fi_mu": "sub", "dɔɔso": "mul",
    "kyɛ": "div", "ma_ɛnyɛ_kɛse": "inc", "ma_ɛnyɛ_ketewa": "dec",
    "po": "neg",
    
    # Ntotoho
    "toto": "cmp", "sɔ_hwɛ": "test",
    
    # Akwankyerɛ
    "huri": "jmp", "frɛ": "call", "ba_man": "ret",
    "sɛ_ɛyɛ_pɛ": "je", "sɛ_ɛyɛ_hwee": "jz", "sɛ_ɛnyɛ_pɛ": "jne",
    "sɛ_ɛsõ": "jg", "sɛ_ɛyɛ_ketewa": "jl",
    
    # Ahia (Stack)
    "pia_mu": "push", "yi_adi": "pop", "pia_frankaa": "pushf", "yi_frankaa": "popf",
    
    # Nnwama (String)
    "tu_byte": "movsb", "sie_byte": "stosb", "fa_byte": "lodsb",
    "san_yɛ": "rep",
    
    # Afidie (Loop)
    "kɔ_to_ho": "loop", "kɔ_to_ho_sɛ_hwee": "loopz",
    
    # Nhyehyɛe (System)
    "gyae": "int", "frɛ_nhyehyɛe": "syscall", "ba_nhyehyɛe": "sysret",
    "gyina": "hlt", "hwee_nni_hɔ": "nop",
    
    # Sesahɔ (Convert)
    "byte_kɔ_asɛmfua": "cbw", "asɛmfua_kɔ_mmienu": "cwd", "mmienu_kɔ_nan": "cdq",
}

# Simplified/Kids mapping
KIDS_KEYWORDS = {
    "ak": {"sie": "mov", "ka_ho": "add", "yi_fi": "sub", "kyerɛ": "syscall"},
}
