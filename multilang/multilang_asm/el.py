# MultiLang-ASM Language Pack: Greek (el)
# Metadata for the Babel Community system

METADATA = {
    "name": "Greek / Ελληνικά",
    "code": "el",
    "author": "Neuro-OS Community",
    "version": "1.0",
    "description": "Full support for Assembly in Greek."
}

# Standard mapping: Native Keyword -> NASM Mnemonic
KEYWORDS = {
    # Movimiento
    "μετακίνηση": "mov", "ανταλλαγή": "xchg", "φόρτωση": "lea",
    
    # Aritmética
    "πρόσθεση": "add", "αφαίρεση": "sub", "πολλαπλασιασμός": "mul",
    # Σύγκριση
    "σύγκριση": "cmp", "έλεγχος": "test",
    
    # Ροή
    "άλμα": "jmp", "κλήση": "call", "επιστροφή": "ret",
    "εάν_ίσο": "je", "εάν_μηδέν": "jz", "εάν_όχι_ίσο": "jne",
    "εάν_μεγαλύτερο": "jg", "εάν_μικρότερο": "jl",
    
    # Στοίβα
    "ώθηση": "push", "εξαγωγή": "pop", "ώθηση_σημαιών": "pushf", "εξαγωγή_σημαιών": "popf",
    
    # Συμβολοσειρές
    "μετακίνηση_byte": "movsb", "αποθήκευση_byte": "stosb", "φόρτωση_byte": "lodsb",
    "επανάληψη": "rep",
    
    # Βρόχοι
    "βρόχος": "loop", "βρόχος_εάν_μηδέν": "loopz",
    
    # Σύστημα
    "διακοπή": "int", "κλήση_συστήματος": "syscall", "επιστροφή_συστήματος": "sysret",
    "παύση": "hlt", "τίποτα": "nop",
    
    # Μετατροπή
    "byte_σε_word": "cbw", "word_σε_double": "cwd",
}

# Simplified/Kids mapping
KIDS_KEYWORDS = {
    "el": {"bale": "mov", "prosthese": "add", "afairese": "sub", "diekse": "syscall"},
}
