# MultiLang-ASM Language Pack: Hindi / हिन्दी (hi)
# Metadata for the Babel Community system

METADATA = {
    "name": "Hindi / हिन्दी",
    "code": "hi",
    "author": "Neuro-OS Community",
    "version": "1.0",
    "description": "Full support for Assembly in Hindi."
}

# Standard mapping: Native Keyword -> NASM Mnemonic
KEYWORDS = {
    # डेटा संचलन (Movement)
    "स्थानांतरित": "mov", "बदलें": "xchg", "पता_लोड": "lea",
    
    # अंकगणित (Arithmetic)
    "जोड़ें": "add", "घटाएं": "sub", "गुणा": "mul",
    "विभाजन": "div", "बढ़ाएं": "inc", "घटाएं_एक": "dec",
    
    # तुलना (Comparison)
    "तुलना": "cmp", "परीक्षण": "test",
    
    # नियंत्रण प्रवाह (Flow Control)
    "कूदें": "jmp", "कॉल": "call", "वापस": "ret",
    "यदि_बराबर": "je", "यदि_शून्य": "jz", "यदि_नहीं_बराबर": "jne",
    "यदि_बड़ा": "jg", "यदि_छोटा": "jl",
    
    # ढेर (Stack)
    "दबाएं": "push", "निकालें": "pop", "फ्लैग_दबाएं": "pushf", "फ्लैग_निकालें": "popf",
    
    # स्ट्रिंग्स (Strings)
    "बाइट_स्थानांतरित": "movsb", "बाइट_संग्रहित": "stosb", "बाइट_लोड": "lodsb",
    "दोहराएं": "rep",
    
    # लूप्स (Loops)
    "लूप": "loop", "यदि_शून्य_लूप": "loopz",
    
    # प्रणाली (System)
    "व्यवधान": "int", "सिस्टम_कॉल": "syscall", "सिस्टम_वापसी": "sysret",
    "रुकें": "hlt", "कुछ_नहीं": "nop",
    
    # रूपांतरण (Conversion)
    "बाइट_को_वर्ड": "cbw", "वर्ड_को_डबल": "cwd",
}

# Simplified/Kids mapping
KIDS_KEYWORDS = {
    "hi": {"rakho": "mov", "jodo": "add", "ghatao": "sub", "dikhao": "syscall"},
}
