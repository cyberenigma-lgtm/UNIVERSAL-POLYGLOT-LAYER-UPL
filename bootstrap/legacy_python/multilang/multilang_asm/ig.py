# MultiLang-ASM Language Pack: Igbo (ig)
# Metadata for the Babel Community system

METADATA = {
    "name": "Igbo",
    "code": "ig",
    "author": "Neuro-OS Community",
    "version": "1.0",
    "description": "Nkwado zuru oke maka assembler n'asụsụ Igbo."
}

# Standard mapping: Native Keyword -> NASM Mnemonic
KEYWORDS = {
    # Ngagharị Data
    "bugharịa": "mov", "gbanwee": "xchg", "bubata_adirẹsi": "lea",
    "gbasaa_efu": "movzx", "gbasaa_akara": "movsx",
    
    # Arithmetiki
    "tinye": "add", "wepụ": "sub", "mụbaa": "mul",
    "kewa": "div", "mụbaa_otu": "inc", "belata_otu": "dec",
    "jụọ": "neg",
    
    # Ntụnyere
    "tụnyere": "cmp", "nwale": "test",
    
    # Nchịkwa Ntuba
    "mali": "jmp", "kpọọ": "call", "lọta": "ret",
    "ma_ọ_bụrụ_na_nhata": "je", "ma_ọ_bụrụ_na_efu": "jz", "ma_ọ_bụrụ_na_anaghị_nhata": "jne",
    "ma_ọ_bụrụ_na_ka_ibu": "jg", "ma_ọ_bụrụ_na_ka_ntakị": "jl",
    
    # Nkwakọba (Stack)
    "tinye_n'ime": "push", "wepụ_n'ime": "pop", "tinye_ọkọlọtọ": "pushf", "wepụ_ọkọlọtọ": "popf",
    
    # Eriri (String)
    "bugharịa_byte": "movsb", "chekwaa_byte": "stosb", "bubata_byte": "lodsb",
    "meghachi_omume": "rep",
    
    # Akaghị (Loop)
    "akaghị": "loop", "akaghị_ma_ọ_bụrụ_na_efu": "loopz",
    
    # Usoro (System)
    "ndabichi": "int", "nkpọ_usoro": "syscall", "nlọta_usoro": "sysret",
    "maka": "hlt", "ntakịrị": "nop",
    
    # Ntụgharị (Convert)
    "byte_gaa_na_okwu": "cbw", "okwu_gaa_na_abụọ": "cwd", "abụọ_gaa_na_anọ": "cdq",
}

# Simplified/Kids mapping
KIDS_KEYWORDS = {
    "ig": {"tinye_ebe": "mov", "tinye": "add", "wepụ": "sub", "gosi": "syscall"},
}
