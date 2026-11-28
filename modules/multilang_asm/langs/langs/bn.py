# MultiLang-ASM Language Pack: Bengali (bn)
# Metadata for the Babel Community system

METADATA = {
    "name": "Bengali / বাংলা",
    "code": "bn",
    "author": "Neuro-OS Community",
    "version": "1.0",
    "description": "বাংলা ভাষায় অ্যাসেম্বলারের জন্য পূর্ণ সমর্থন।"
}

# Standard mapping: Native Keyword -> NASM Mnemonic
KEYWORDS = {
    # ডাটা মুভমেন্ট
    "সরানো": "mov", "বিনিময়": "xchg", "অ্যাড্রেস_লোড": "lea",
    "শূন্য_প্রসারণ": "movzx", "চিহ্ন_প্রসারণ": "movsx",
    
    # পাটিগণিত
    "যোগ": "add", "বিয়োগ": "sub", "গুণ": "mul",
    "ভাগ": "div", "বৃদ্ধি": "inc", "হ্রাস": "dec",
    "অস্বীকার": "neg",
    
    # তুলনা
    "তুলনা": "cmp", "পরীক্ষা": "test",
    
    # কন্ট্রোল ফ্লো
    "লাফানো": "jmp", "কল": "call", "ফেরা": "ret",
    "যদি_সমান": "je", "যদি_শূন্য": "jz", "যদি_অসমান": "jne",
    "если_больше": "jg", "যদি_ছোট": "jl",
    
    # স্ট্যাক
    "পুশ": "push", "পপ": "pop", "ফ্ল্যাগ_পুশ": "pushf", "ফ্ল্যাগ_পপ": "popf",
    
    # স্ট্রিং
    "বাইট_সরানো": "movsb", "বাইট_সংরক্ষণ": "stosb", "বাইট_লোড": "lodsb",
    "পুনরাবৃত্তি": "rep",
    
    # লুপ
    "লুপ": "loop", "শূন্য_লুপ": "loopz",
    
    # সিস্টেম
    "ইন্টারাপ্ট": "int", "সিস্টেম_কল": "syscall", "সিস্টেম_রিটার্ন": "sysret",
    "থামানো": "hlt", "কিছু_না": "nop",
    
    # রূপান্তর (Convert)
    "বাইট_থেকে_ওয়ার্ড": "cbw", "ওয়ার্ড_থেকে_ডাবল": "cwd", "ডাবল_থেকে_কোয়াড": "cdq",
}

# Simplified/Kids mapping
KIDS_KEYWORDS = {
    "bn": {"রাখো": "mov", "যোগ": "add", "বিয়োগ": "sub", "দেখাও": "syscall"},
}
