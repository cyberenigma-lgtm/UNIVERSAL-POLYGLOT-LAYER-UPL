# MultiLang-ASM Language Pack: German (de)
# Metadata for the Babel Community system

METADATA = {
    "name": "German / Deutsch",
    "code": "de",
    "author": "Neuro-OS Community",
    "version": "1.0",
    "description": "Unterstützung für Assembler in Deutsch, einschließlich Kids Mode Dialekten."
}

# Standard mapping: Native Keyword -> NASM Mnemonic
KEYWORDS = {
    # Datenbewegung
    "bewegen": "mov", "tauschen": "xchg", "effektiv_laden": "lea",
    "null_erweitern": "movzx", "vorzeichen_erweitern": "movsx",
    
    # Arithmetik
    "addieren": "add", "subtrahieren": "sub", "multiplizieren": "mul",
    "multiplizieren_vorzeichen": "imul", "dividieren": "div",
    "dividieren_vorzeichen": "idiv", "inkrementieren": "inc",
    "dekrementieren": "dec", "negieren": "neg",
    
    # Vergleich
    "vergleichen": "cmp", "testen": "test",
    
    # Ablaufsteuerung
    "springen": "jmp", "rufen": "call", "zurueckkehren": "ret",
    "wenn_gleich": "je", "wenn_null": "jz", "wenn_nicht_gleich": "jne",
    "wenn_groesser": "jg", "wenn_kleiner": "jl",
    
    # Stapel
    "druecken": "push", "holen": "pop", "stapel_druecken": "push",
    "stapel_holen": "pop", "stapel_flags_druecken": "pushf", "stapel_flags_holen": "popf",
    
    # Zeichenketten
    "byte_bewegen": "movsb", "byte_speichern": "stosb", "byte_laden": "lodsb",
    "wiederholen": "rep",
    
    # Schleifen
    "schleife": "loop", "schleife_wenn_null": "loopz",
    
    # System
    "unterbrechung": "int", "systemaufruf": "syscall", "system_rueckkehr": "sysret",
    "stoppen": "hlt", "nichts": "nop",
    
    # Konvertierung
    "byte_zu_wort": "cbw", "wort_zu_doppel": "cwd", "doppel_zu_quad": "cdq",
}

# Simplified/Kids mapping
KIDS_KEYWORDS = {
    "de": {"setze": "mov", "addiere": "add", "ziehe_ab": "sub", "zeige": "syscall"},
    "de_bay": {"pack": "mov", "dazu": "add", "weg": "sub", "schau": "syscall"},
    "de_sw": {"tue": "mov", "zelle": "add", "nimm": "sub", "lueg": "syscall"},
    "de_at": {"gib": "mov", "dazua": "add", "weg": "sub", "schau": "syscall"},
}
