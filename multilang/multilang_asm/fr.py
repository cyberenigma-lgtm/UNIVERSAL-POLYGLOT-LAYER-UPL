# MultiLang-ASM Language Pack: French (fr)
# Metadata for the Babel Community system

METADATA = {
    "name": "French / Français",
    "code": "fr",
    "author": "Neuro-OS Community",
    "version": "1.0",
    "description": "Support complet pour l'assembleur en français, incluant le dialecte Québécois."
}

# Standard mapping: Native Keyword -> NASM Mnemonic
KEYWORDS = {
    # Mouvement
    "deplacer": "mov", "copier": "mov", "echanger": "xchg",
    "charger_effectif": "lea", "etendre_zero": "movzx", "etendre_signe": "movsx",
    
    # Arithmétique
    "ajouter": "add", "soustraire": "sub", "multiplier": "mul",
    "multiplier_signe": "imul", "diviser": "div", "diviser_signe": "idiv",
    "incrementer": "inc", "decrementer": "dec", "negativer": "neg",
    
    # Logique
    "et": "and", "ou": "or", "non": "not", "ou_exclusif": "xor",
    "decaler_gauche": "shl",  "decaler_droite": "shr",
    "decaler_arith_gauche": "sal", "decaler_arith_droite": "sar",
    "rot_gauche": "rol", "rot_droite": "ror",
    
    # Comparaison
    "comparer": "cmp", "tester": "test",
    
    # Sauts
    "sauter": "jmp", "appeler": "call", "retourner": "ret",
    "si_egal": "je", "si_zero": "jz", "si_pas_egal": "jne", "si_pas_zero": "jnz",
    "si_plus_grand": "jg", "si_plus_grand_egal": "jge",
    "si_plus_petit": "jl", "si_plus_petit_egal": "jle",
    
    # Pile
    "empiler": "push", "depiler": "pop", "pousser_drapeaux": "pushf", "tirer_drapeaux": "popf",
    
    # Chaines
    "deplacer_octet": "movsb", "stocker_octet": "stosb", "charger_octet": "lodsb",
    "repeter": "rep",
    
    # Boucles
    "boucle": "loop", "boucle_si_zero": "loopz",
    
    # Système
    "interruption": "int", "appel_systeme": "syscall", "retour_systeme": "sysret",
    "retour_interruption": "iret", "rien": "nop", "arreter": "hlt",
    
    # Conversion
    "conv_octet_mot": "cbw", "conv_mot_double": "cwd",
}

# Simplified/Kids mapping
KIDS_KEYWORDS = {
    "fr": {"mets": "mov", "ajoute": "add", "enleve": "sub", "montre": "syscall"},
    "fr_qc": {"place": "mov", "ajoute": "add", "ote": "sub", "check": "syscall"},
}
