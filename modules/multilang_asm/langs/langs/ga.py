# MultiLang-ASM Language Pack: Irish (ga)
# Metadata for the Babel Community system

METADATA = {
    "name": "Irish / Gaeilge",
    "code": "ga",
    "author": "Neuro-OS Community",
    "version": "1.0",
    "description": "Tacaíocht iomlán don tionólaí i nGaeilge."
}

# Standard mapping: Native Keyword -> NASM Mnemonic
KEYWORDS = {
    # Gluaiseacht Sonraí
    "bog": "mov", "mhalartaigh": "xchg", "luchtaigh_seolta": "lea",
    "leathnaigh_nialas": "movzx", "leathnaigh_comhartha": "movsx",
    
    # Arithmetaitic
    "suimigh": "add", "dealú": "sub", "iolraigh": "mul",
    "roinn": "div", "incrimint": "inc", "deicrimint": "dec",
    "diúltaigh": "neg",
    
    # Comparáid
    "cuir_i_gcomparáid": "cmp", "tástáil": "test",
    
    # Sreabhadh Rialaithe
    "léim": "jmp", "glaoch": "call", "filleadh": "ret",
    "más_ionann": "je", "más_nialas": "jz", "mura_ionann": "jne",
    "más_mó": "jg", "más_lú": "jl",
    
    # Cruach
    "brúigh": "push", "pop": "pop", "brúigh_bratacha": "pushf", "pop_bratacha": "popf",
    
    # Teaghráin
    "bog_beart": "movsb", "stóráil_beart": "stosb", "luchtaigh_beart": "lodsb",
    "athdhéan": "rep",
    
    # Lúba
    "lúb": "loop", "lúb_más_nialas": "loopz",
    
    # Córas
    "idirbhriseadh": "int", "glaoch_córais": "syscall", "filleadh_córais": "sysret",
    "stad": "hlt", "tada": "nop",
    
    # Tiontú
    "beart_go_focal": "cbw", "focal_go_dúbailte": "cwd", "dúbailte_go_ceathracha": "cdq",
}

# Simplified/Kids mapping
KIDS_KEYWORDS = {
    "ga": {"cuir": "mov", "breis": "add", "lúide": "sub", "taispeáin": "syscall"},
}
