# MultiLang-ASM Language Pack: Italian (it)
# Metadata for the Babel Community system

METADATA = {
    "name": "Italian / Italiano",
    "code": "it",
    "author": "Neuro-OS Community",
    "version": "1.0",
    "description": "Supporto completo per l'assemblatore in italiano, inclusi i dialetti regionali."
}

# Standard mapping: Native Keyword -> NASM Mnemonic
KEYWORDS = {
    # Movimento
    "spostare": "mov", "copiare": "mov", "scambiare": "xchg",
    "caricare_effettivo": "lea", "estendere_zero": "movzx",
    "estendere_segno": "movsx",
    
    # Aritmetica
    "sommare": "add", "sottrarre": "sub", "moltiplicare": "mul",
    "dividere": "div", "incrementare": "inc", "decrementare": "dec",
    
    # Confronto
    "confrontare": "cmp", "testare": "test",
    
    # Flusso
    "saltare": "jmp", "chiamare": "call", "ritornare": "ret",
    "se_uguale": "je", "se_zero": "jz", "se_non_uguale": "jne",
    "se_maggiore": "jg", "se_minore": "jl",
    
    # Pila
    "mettere": "push", "togliere": "pop", "mettere_flag": "pushf", "togliere_flag": "popf",
    
    # Stringhe
    "muovi_byte": "movsb", "salva_byte": "stosb", "carica_byte": "lodsb",
    "ripeti": "rep",
    
    # Cicli
    "ciclo": "loop", "ciclo_se_zero": "loopz",
    
    # Sistema
    "interruzione": "int", "chiamata_sistema": "syscall", "ritorno_sistema": "sysret",
    "ferma": "hlt", "niente": "nop",
    
    # Conversione
    "conv_byte_parola": "cbw", "conv_parola_doppio": "cwd", "conv_doppio_quad": "cdq",
}

# Simplified/Kids mapping
KIDS_KEYWORDS = {
    "it": {"metti": "mov", "aggiungi": "add", "togli": "sub", "mostra": "syscall"},
    "it_nap": {"miett": "mov", "agna": "add", "lieve": "sub", "vide": "syscall"},
    "it_sic": {"mintici": "mov", "iungi": "add", "leva": "sub", "talía": "syscall"},
    "it_rom": {"moje": "mov", "jungi": "add", "leva": "sub", "guarda": "syscall"},
}
