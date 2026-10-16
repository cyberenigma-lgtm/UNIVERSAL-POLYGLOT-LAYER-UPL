# MultiLang-ASM Language Pack: Tok Pisin (tpi)
# Metadata for the Babel Community system

METADATA = {
    "name": "Tok Pisin",
    "code": "tpi",
    "author": "Neuro-OS Community",
    "version": "1.0",
    "description": "Ful sapot bilong assembler long Tok Pisin."
}

# Standard mapping: Native Keyword -> NASM Mnemonic
KEYWORDS = {
    # Muivim Data
    "muvim": "mov", "senisim": "xchg", "lodim_adres": "lea",
    "bikim_zero": "movzx", "bikim_mak": "movsx",
    
    # Matematiks
    "plusim": "add", "minusim": "sub", "mulpim": "mul",
    "divim": "div", "go_ap": "inc", "go_daun": "dec",
    "no_makim": "neg",
    
    # Sekim
    "sekim": "cmp", "testim": "test",
    
    # Kontrol Flow
    "kalap": "jmp", "singaut": "call", "go_bek": "ret",
    "sapos_sem": "je", "sapos_nating": "jz", "sapos_no_sem": "jne",
    "sapos_bik": "jg", "sapos_liklik": "jl",
    
    # Stack bilong ol samting
    "putim_insait": "push", "autim": "pop", "putim_flek": "pushf", "autim_flek": "popf",
    
    # String
    "muvim_byte": "movsb", "holim_byte": "stosb", "lodim_byte": "lodsb",
    "mekim_gen": "rep",
    
    # Lup (Loop)
    "lup": "loop", "lup_sapos_nating": "loopz",
    
    # Sistem
    "stopim": "int", "singaut_sistem": "syscall", "bek_sistem": "sysret",
    "pinis": "hlt", "nating": "nop",
    
    # Konvesin
    "byte_go_word": "cbw", "word_go_double": "cwd", "double_go_quad": "cdq",
}

# Simplified/Kids mapping
KIDS_KEYWORDS = {
    "tpi": {"putim": "mov", "plus": "add", "minus": "sub", "soim": "syscall"},
}
