# MultiLang-ASM Language Pack: Basque (eu)
# Metadata for the Babel Community system

METADATA = {
    "name": "Basque / Euskara",
    "code": "eu",
    "author": "Neuro-OS Community",
    "version": "1.0",
    "description": "Euskarazko mihiztatzailearentzako laguntza osoa."
}

# Standard mapping: Native Keyword -> NASM Mnemonic
KEYWORDS = {
    # Datu Mugimendua
    "mugitu": "mov", "trukatu": "xchg", "helbidea_kargatu": "lea",
    "zero_hedatu": "movzx", "zeinu_hedatu": "movsx",
    
    # Aritmetika
    "gehitu": "add", "kendu": "sub", "biderkatu": "mul",
    "zatitu": "div", "inkrementatu": "inc", "dekrementatu": "dec",
    "ukatu": "neg",
    
    # Konparazioa
    "konparatu": "cmp", "probatu": "test",
    
    # Kontrol Fluxua
    "jauzi": "jmp", "deitu": "call", "itzuli": "ret",
    "berdina_bada": "je", "zero_bada": "jz", "ez_berdina_bada": "jne",
    "handiagoa_bada": "jg", "txikiagoa_bada": "jl",
    
    # Pila
    "sartu": "push", "atera": "pop", "banderak_sartu": "pushf", "banderak_atera": "popf",
    
    # Kateak
    "byte_mugitu": "movsb", "byte_gorde": "stosb", "byte_kargatu": "lodsb",
    "errepikatu": "rep",
    
    # Begiztak
    "begizta": "loop", "zero_bada_begizta": "loopz",
    
    # Sistema
    "etena": "int", "sistema_deia": "syscall", "sistema_itzulera": "sysret",
    "gelditu": "hlt", "ezer_ez": "nop",
    
    # Bihurketa
    "byte_hitzera": "cbw", "hitza_bikoitzera": "cwd", "bikoitza_quadrera": "cdq",
}

# Simplified/Kids mapping
KIDS_KEYWORDS = {
    "eu": {"jarri": "mov", "gehitu": "add", "kendu": "sub", "erakutsi": "syscall"},
}
