# MultiLang-ASM Language Pack: Swahili / Kiswahili (sw)
# Metadata for the Babel Community system

METADATA = {
    "name": "Swahili / Kiswahili",
    "code": "sw",
    "author": "Neuro-OS Community",
    "version": "1.0",
    "description": "Full support for Assembly in Swahili."
}

# Standard mapping: Native Keyword -> NASM Mnemonic
KEYWORDS = {
    # Movimiento
    "hamisha": "mov", "badilishana": "xchg", "pakia_anwani": "lea",
    
    # Aritmética
    "jumlisha": "add", "toa": "sub", "zidisha": "mul",
    "gawa": "div", "ongeza": "inc", "punguza": "dec",
    
    # Lógica
    "na": "and", "au": "or", "sio": "not", "pekee": "xor",
    
    # Comparación
    "linganisha": "cmp", "jaribu": "test",
    
    # Mtiririko
    "ruka": "jmp", "ita": "call", "rudi": "ret",
    "kama_sawa": "je", "kama_sufuri": "jz", "kama_sio_sawa": "jne",
    "kama_kubwa": "jg", "kama_ndogo": "jl",
    
    # Rafu (Stack)
    "sukuma": "push", "vuta": "pop", "sukuma_bendera": "pushf", "vuta_bendera": "popf",
    
    # Mifuatano (Strings)
    "sogeza_baiti": "movsb", "hifadhi_baiti": "stosb", "pata_baiti": "lodsb",
    "rudia": "rep",
    
    # Mizunguko (Loops)
    "mzunguko": "loop", "mzunguko_kama_sufuri": "loopz",
    
    # Mfumo (System)
    "katiza": "int", "ito_mfumo": "syscall", "rudi_mfumo": "sysret",
    "simama": "hlt", "hakuna": "nop",
    
    # Ubadilishaji (Conversion)
    "baiti_kwa_neno": "cbw", "neno_kwa_duara": "cwd",
}

# Simplified/Kids mapping
KIDS_KEYWORDS = {
    "sw": {"weka": "mov", "ongeza": "add", "toa": "sub", "onyesha": "syscall"},
}
