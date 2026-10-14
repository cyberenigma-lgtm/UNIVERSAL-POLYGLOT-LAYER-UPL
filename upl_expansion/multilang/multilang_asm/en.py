# MultiLang-ASM Language Pack: English (en)
# Metadata for the Babel Community system

METADATA = {
    "name": "English",
    "code": "en",
    "author": "Neuro-OS Community",
    "version": "1.0",
    "description": "Standard English support, primarily focused on Kids Mode dialects."
}

# Standard mapping: Native Keyword -> NASM Mnemonic
KEYWORDS = {
    # Movement
    "move": "mov", "copy": "mov", "exchange": "xchg", "load": "mov",
    "load_effective": "lea", "extend_zero": "movzx", "extend_sign": "movsx",
    
    # Arithmetic
    "add": "add", "subtract": "sub", "multiply": "mul", "imul": "imul",
    "divide": "div", "idiv": "idiv", "increment": "inc", "decrement": "dec", "negate": "neg",
    
    # Logic
    "and": "and", "or": "or", "not": "not", "xor": "xor",
    "shift_left": "shl", "shift_right": "shr", "rotate_left": "rol", "rotate_right": "ror",
    
    # Comparison
    "compare": "cmp", "test": "test",
    
    # Flow
    "jump": "jmp", "call": "call", "return": "ret",
    "if_equal": "je", "if_zero": "jz", "if_not_equal": "jne",
    "if_greater": "jg", "if_less": "jl",
    
    # Stack
    "push": "push", "pop": "pop", "push_flags": "pushf", "pop_flags": "popf",
    
    # Strings
    "move_byte": "movsb", "store_byte": "stosb", "load_byte": "lodsb",
    "repeat": "rep",
    
    # Loops
    "loop": "loop", "loop_if_zero": "loopz",
    
    # System
    "interrupt": "int", "syscall": "syscall", "sysret": "sysret",
    "halt": "hlt", "nop": "nop",
}

# Simplified/Kids mapping
KIDS_KEYWORDS = {
    "en": {"put": "mov", "add": "add", "take": "sub", "look": "syscall"},
    "en_cockney": {"stash": "mov", "lob": "add", "nick": "sub", "gawk": "syscall"},
    "en_aus": {"chuck": "mov", "reckon": "add", "nix": "sub", "squiz": "syscall"},
    "en_tx": {"hitch": "mov", "roundup": "add", "cut": "sub", "spy": "syscall"},
    "en_ie": {"park": "mov", "tally": "add", "slash": "sub", "dekko": "syscall"},
    "en_scots": {"pit": "mov", "tot": "add", "dock": "sub", "keek": "syscall"},
}
