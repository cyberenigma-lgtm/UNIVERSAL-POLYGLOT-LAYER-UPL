# MultiLang-ASM Language Pack: Tamil (ta)
# Metadata for the Babel Community system

METADATA = {
    "name": "Tamil / தமிழ்",
    "code": "ta",
    "author": "Neuro-OS Community",
    "version": "1.0",
    "description": "தமிழ் மொழியில் அசெம்ப்ளருக்கான முழு ஆதரவு."
}

# Standard mapping: Native Keyword -> NASM Mnemonic
KEYWORDS = {
    # தரவு இயக்கம்
    "நகர்த்து": "mov", "மாற்று": "xchg", "முகவரி_ஏற்று": "lea",
    "பூஜ்ஜிய_விரிவாக்கம்": "movzx", "குறி_விரிவாக்கம்": "movsx",
    
    # எண்கணிதம்
    "கூட்டு": "add", "கழி": "sub", "பெருக்கு": "mul",
    "வகு": "div", "அதிகரி": "inc", "குறை": "dec",
    "மறு": "neg",
    
    # ஒப்பீடு
    "ஒப்பிடு": "cmp", "சோதி": "test",
    
    # கட்டுப்பாட்டு ஓட்டம்
    "தாவு": "jmp", "அழை": "call", "திரும்பு": "ret",
    "சமம்_என்றால்": "je", "பூஜ்ஜியம்_என்றால்": "jz", "சமமில்லை_என்றால்": "jne",
    "பெரிது_என்றால்": "jg", "சிறிது_என்றால்": "jl",
    
    # அடுக்கு (Stack)
    "தள்ளு": "push", "எடு": "pop", "கொடி_தள்ளு": "pushf", "கொடி_எடு": "popf",
    
    # சரம் (String)
    "பைட்_நகர்த்து": "movsb", "பைட்_சேமி": "stosb", "பைட்_ஏற்று": "lodsb",
    "மீண்டும்_செய்": "rep",
    
    # வளையம் (Loop)
    "சுழற்சி": "loop", "பூஜ்ஜியம்_சுழற்சி": "loopz",
    
    # அமைப்பு (System)
    "தடை": "int", "அமைப்பு_அழைப்பு": "syscall", "அமைப்பு_திரும்பு": "sysret",
    "நிறுத்து": "hlt", "ஒன்றுமில்லை": "nop",
    
    # மாற்றம் (Convert)
    "பைட்_வார்த்தை": "cbw", "வார்த்தை_இரண்டு": "cwd", "இரண்டு_நான்கு": "cdq",
}

# Simplified/Kids mapping
KIDS_KEYWORDS = {
    "ta": {"வை": "mov", "கூட்டு": "add", "கழி": "sub", "காட்டு": "syscall"},
}
