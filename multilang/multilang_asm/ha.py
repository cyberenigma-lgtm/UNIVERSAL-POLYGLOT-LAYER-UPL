# MultiLang-ASM Language Pack: Hausa (ha)
# Metadata for the Babel Community system

METADATA = {
    "name": "Hausa",
    "code": "ha",
    "author": "Neuro-OS Community",
    "version": "1.0",
    "description": "Cikakken tallafi ga taro (assembler) a yaren Hausa."
}

# Standard mapping: Native Keyword -> NASM Mnemonic
KEYWORDS = {
    # Motsin Bayanai
    "motsa": "mov", "musaya": "xchg", "loda_adireshi": "lea",
    "fadada_sifiri": "movzx", "fadada_shara": "movsx",
    
    # Lissafi
    "kara": "add", "rage": "sub", "rubanya": "mul",
    "raba": "div", "kara_daya": "inc", "rage_daya": "dec",
    "karyata": "neg",
    
    # Gwaji
    "gwada": "cmp", "jarraba": "test",
    
    # Sarrafa Gudana
    "tsallaka": "jmp", "kira": "call", "koma": "ret",
    "idan_daidai": "je", "idan_sifiri": "jz", "idan_ba_daidai_ba": "jne",
    "idan_fi": "jg", "idan_kasa": "jl",
    
    # Tari
    "tura": "push", "cire": "pop", "tura_tuta": "pushf", "cire_tuta": "popf",
    
    # Zaren Bayanai
    "motsa_byte": "movsb", "ajiye_byte": "stosb", "loda_byte": "lodsb",
    "maimaita": "rep",
    
    # Madauki
    "madauki": "loop", "madauki_idan_sifiri": "loopz",
    
    # Tsarin Guda
    "tsayawa": "int", "kiran_tsarin": "syscall", "koma_tsarin": "sysret",
    "dakatar": "hlt", "babu": "nop",
    
    # Canji (Convert)
    "byte_zuwa_kalma": "cbw", "kalma_zuwa_biyu": "cwd", "biyu_zuwa_hudu": "cdq",
}

# Simplified/Kids mapping
KIDS_KEYWORDS = {
    "ha": {"sanya": "mov", "kara": "add", "rage": "sub", "nuna": "syscall"},
}
