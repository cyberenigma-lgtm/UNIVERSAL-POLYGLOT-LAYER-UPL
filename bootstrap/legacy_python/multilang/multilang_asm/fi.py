# MultiLang-ASM Language Pack: Finnish (fi)
# Metadata for the Babel Community system

METADATA = {
    "name": "Finnish / Suomi",
    "code": "fi",
    "author": "Neuro-OS Community",
    "version": "1.0",
    "description": "Täysi tuki assemblerille suomeksi."
}

# Standard mapping: Native Keyword -> NASM Mnemonic
KEYWORDS = {
    # Tiedon siirto
    "siirrä": "mov", "vaihda": "xchg", "lataa_osoite": "lea",
    "laajenna_nolla": "movzx", "laajenna_merkki": "movsx",
    
    # Aritmetiikka
    "lisää": "add", "vähennä": "sub", "kerro": "mul",
    "jaa": "div", "kasvata": "inc", "pienennä": "dec",
    "käännä": "neg",
    
    # Vertailu
    "vertaa": "cmp", "testaa": "test",
    
    # Ohjausvirta
    "hyppää": "jmp", "kutsu": "call", "palaa": "ret",
    "jos_sama": "je", "jos_nolla": "jz", "jos_eri": "jne",
    "jos_suurempi": "jg", "jos_pienempi": "jl",
    
    # Pino
    "työnnä": "push", "vedä": "pop", "työnnä_liput": "pushf", "vedä_liput": "popf",
    
    # Merkkijonot
    "siirrä_tavu": "movsb", "tallenna_tavu": "stosb", "lataa_tavu": "lodsb",
    "toista": "rep",
    
    # Silmukat
    "silmukka": "loop", "silmukka_jos_nolla": "loopz",
    
    # Järjestelmä
    "keskeytys": "int", "järjestelmäkutsu": "syscall", "järjestelmäpaluu": "sysret",
    "pysäytä": "hlt", "tyhjä": "nop",
    
    # Muunnos
    "tavu_sanaksi": "cbw", "sana_kaksoissanaksi": "cwd", "kaksoissana_nelois": "cdq",
}

# Simplified/Kids mapping
KIDS_KEYWORDS = {
    "fi": {"laita": "mov", "plus": "add", "miinus": "sub", "nayta": "syscall"},
}
