# MultiLang-ASM Language Pack: Hebrew (he)
# Metadata for the Babel Community system

METADATA = {
    "name": "Hebrew / עברית",
    "code": "he",
    "author": "Neuro-OS Community",
    "version": "1.0",
    "description": "Full support for Assembly in Hebrew."
}

# Standard mapping: Native Keyword -> NASM Mnemonic
KEYWORDS = {
    # Movimiento
    "הזז": "mov", "החלף": "xchg", "טען_כתובת": "lea",
    
    # Aritmética
    "חבר": "add", "חסר": "sub", "כפול": "mul",
    # השוואה
    "השווה": "cmp", "בדוק": "test",
    
    # זרימה
    "קפוץ": "jmp", "קרא": "call", "חזור": "ret",
    "אם_שווה": "je", "אם_אפס": "jz", "אם_לא_שווה": "jne",
    "אם_גדול": "jg", "אם_קטן": "jl",
    
    # מחסנית
    "דחוף": "push", "משוך": "pop", "דחוף_דגלים": "pushf", "משוך_דגלים": "popf",
    
    # מחרוζות
    "הזז_בייט": "movsb", "שמור_בייט": "stosb", "טען_בייט": "lodsb",
    "חזור_על": "rep",
    
    # לולאות
    "לולאה": "loop", "לולאה_אם_אפס": "loopz",
    
    # מערכת
    "פסיקה": "int", "קריאת_מערכת": "syscall", "חזרת_מערכת": "sysret",
    "עצור": "hlt", "כלום": "nop",
    
    # המרה
    "בייט_לוורד": "cbw", "וורד_לדאבל": "cwd",
}

# Simplified/Kids mapping
KIDS_KEYWORDS = {
    "he": {"sim": "mov", "hosef": "add", "haser": "sub", "hare": "syscall"},
}
