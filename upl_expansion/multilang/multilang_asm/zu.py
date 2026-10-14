# MultiLang-ASM Language Pack: Zulu (zu)
# Metadata for the Babel Community system

METADATA = {
    "name": "Zulu / isiZulu",
    "code": "zu",
    "author": "Neuro-OS Community",
    "version": "1.0",
    "description": "Ukwesekwa okugcwele kwe-assembler ngesiZulu."
}

# Standard mapping: Native Keyword -> NASM Mnemonic
KEYWORDS = {
    # Ukuhamba Kwemininingwane
    "hambisa": "mov", "shintshisana": "xchg", "layisha_ikheli": "lea",
    "andisa_iqanda": "movzx", "andisa_uphawu": "movsx",
    
    # Izibalo
    "hlanganisa": "add", "susa": "sub", "phindaphinda": "mul",
    "hlukanisa": "div", "khulisa": "inc", "nciphisa": "dec",
    "phika": "neg",
    
    # Ukuqhathanisa
    "qhathanisa": "cmp", "hlola": "test",
    
    # Ukulawula Ukuhamba
    "eqa": "jmp", "biza": "call", "buya": "ret",
    "uma_kulingana": "je", "uma_iqanda": "jz", "uma_kungalingani": "jne",
    "uma_kukhulu": "jg", "uma_kuncane": "jl",
    
    # Isitaki
    "faka": "push", "khipha": "pop", "faka_amaflag": "pushf", "khipha_amaflag": "popf",
    
    # Izintambo
    "hambisa_ibyte": "movsb", "gcina_ibyte": "stosb", "layisha_ibyte": "lodsb",
    "phinda": "rep",
    
    # I-loop
    "iluphu": "loop", "iluphu_uma_iqanda": "loopz",
    
    # Isistimu
    "uphazamiso": "int", "ukubizwa_kwesistimu": "syscall", "ukubuya_kwesistimu": "sysret",
    "misa": "hlt", "lutho": "nop",
    
    # Ukuguqulwa
    "ibyte_iya_egameni": "cbw", "igama_iya_kabili": "cwd", "ikabili_iya_quad": "cdq",
}

# Simplified/Kids mapping
KIDS_KEYWORDS = {
    "zu": {"beka": "mov", "hlanganisa": "add", "susa": "sub", "bonisa": "syscall"},
}
