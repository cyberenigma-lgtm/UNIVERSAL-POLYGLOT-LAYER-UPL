# MultiLang-ASM Language Pack: Yucatec Maya (may/yua)
# Metadata for the Babel Community system

METADATA = {
    "name": "Maya / Maaya T'aan",
    "code": "may",
    "author": "Neuro-OS Community",
    "version": "1.0",
    "description": "Tuláakal áantaj ti'al assembler ich Maaya T'aan."
}

# Standard mapping: Native Keyword -> NASM Mnemonic
KEYWORDS = {
    # Su'utul Raraunga
    "túuxt": "mov", "k'ex": "xchg", "palk'es": "lea",
    "mixba'al_jalk'es": "movzx", "unancha_jalk'es": "movsx",
    
    # Xook
    "ts'áaj": "add", "túul": "sub", "ya'abkún": "mul",
    "t'ox": "div", "ch'íij": "inc", "ch'enb": "dec",
    "ma'": "neg",
    
    # Ket
    "ket": "cmp", "pilis": "test",
    
    # Bel
    "si'it": "jmp", "t'an": "call", "suut": "ret",
    "je'el_bey": "je", "mixba'al_bey": "jz", "ma'_bey": "jne",
    "nojoch": "jg", "chichan": "jl",
    
    # Pila
    "ts'áaj_ich": "push", "jok's": "pop", "flag_ts'áaj": "pushf", "flag_jok's": "popf",
    
    # T'an (String)
    "byte_túuxt": "movsb", "byte_li'is": "stosb", "byte_palk'es": "lodsb",
    "ka'a": "rep",
    
    # Malacatl
    "nalkut": "loop", "mixba'al_nalkut": "loopz",
    
    # Sistema
    "jet'": "int", "t'an_sistema": "syscall", "suut_sistema": "sysret",
    "p'ul": "hlt", "mixba'al": "nop",
    
    # Tikray (Convert)
    "byte_wordtech": "cbw", "word_doubletech": "cwd", "double_quadtech": "cdq",
}

# Simplified/Kids mapping
KIDS_KEYWORDS = {
    "may": {"ts'áaj": "mov", "ya'ab": "add", "túul": "sub", "e'es": "syscall"},
}
