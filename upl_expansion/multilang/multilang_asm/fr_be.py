# MultiLang-ASM Language Pack: Belgian French (fr_be)
# Metadata for the Babel Community system

METADATA = {
    "name": "Belgian French / Français (Belgique)",
    "code": "fr_be",
    "author": "Neuro-OS Community",
    "version": "1.0",
    "description": "Variante belge du français pour l'assembleur."
}

# Standard mapping: Native Keyword -> NASM Mnemonic
KEYWORDS = {
    # Movimiento
    "deplacer": "mov", "copier": "mov", "septante": "mov", # Easter egg: septante
    "charger_adresse": "lea",
    
    # Aritmética
    "ajouter": "add", "soustraire": "sub", "multiplier": "mul",
    "diviser": "div", "plus_un": "inc", "moins_un": "dec",
    
    # Lógica
    "et": "and", "ou": "or", "non": "not",
    
    # Comparaison
    "comparer": "cmp", "tester": "test",
    
    # Flujo
    "sauter": "jmp", "appeler": "call", "revenir": "ret",
    "si_egal": "je", "si_zero": "jz", "si_pas_egal": "jne",
    "si_plus_grand": "jg", "si_plus_petit": "jl",
    
    # Pile
    "pousser": "push", "tirer": "pop", "pousser_flags": "pushf", "tirer_flags": "popf",
    
    # Chaînes
    "bouger_octet": "movsb", "stocker_octet": "stosb", "charger_octet": "lodsb",
    "encore": "rep",
    
    # Boucles
    "tourner": "loop", "si_zero_tourner": "loopz",
    
    # Sistema
    "interrompre": "int", "appel_systeme": "syscall", "retour_systeme": "sysret",
    "stop": "hlt", "rien": "nop",
    
    # Conversion
    "octet_en_mot": "cbw", "mot_en_double": "cwd",
}

# Simplified/Kids mapping
KIDS_KEYWORDS = {
    "fr_be": {"mets": "mov", "ajoute": "add", "enleve": "sub", "montre": "syscall"},
}
