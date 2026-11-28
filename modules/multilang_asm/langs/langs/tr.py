# MultiLang-ASM Language Pack: Turkish / Türkçe (tr)
# Metadata for the Babel Community system

METADATA = {
    "name": "Turkish / Türkçe",
    "code": "tr",
    "author": "Neuro-OS Community",
    "version": "1.0",
    "description": "Full support for Assembly in Turkish."
}

# Standard mapping: Native Keyword -> NASM Mnemonic
KEYWORDS = {
    # Hareket
    "tasi": "mov", "degistir": "xchg", "adresi_yukle": "lea",
    
    # Aritmetik
    "topla": "add", "ekle": "add", "cikar": "sub",
    "carp": "mul", "bol": "div", "arttir": "inc",
    "azalt": "dec", "degille": "neg",
    
    # Karsilastirma
    "karsilastir": "cmp", "test_et": "test",
    
    # Akis Kontrolu
    "atla": "jmp", "cagir": "call", "don": "ret",
    "esitse": "je", "sifirsa": "jz", "esit_degilse": "jne",
    "buyukse": "jg", "kucukse": "jl",
    
    # Yigin
    "it": "push", "cek": "pop", "bayraklari_it": "pushf", "bayraklari_cek": "popf",
    
    # Diziler
    "bayt_tasi": "movsb", "bayt_sakla": "stosb", "bayt_yukle": "lodsb",
    "tekrarla": "rep",
    
    # Donguler
    "dongu": "loop", "sifirsa_dongu": "loopz",
    
    # Sistem
    "kesme": "int", "sistem_cagrisi": "syscall", "sistem_donusu": "sysret",
    "dur": "hlt", "bos": "nop",
    
    # Donusum
    "bayt_to_word": "cbw", "word_to_double": "cwd",
}

# Simplified/Kids mapping
KIDS_KEYWORDS = {
    "tr": {"koy": "mov", "ekle": "add", "cikar": "sub", "goster": "syscall"},
}
