# MultiLang-ASM Language Pack: Amharic (am)
# Metadata for the Babel Community system

METADATA = {
    "name": "Amharic / አማርኛ",
    "code": "am",
    "author": "Neuro-OS Community",
    "version": "1.0",
    "description": "ለአሰምብለር በአማርኛ ሙሉ ድጋፍ።"
}

# Standard mapping: Native Keyword -> NASM Mnemonic
KEYWORDS = {
    # የዳታ እንቅስቃሴ
    "አንቀሳቅስ": "mov", "ቀይር": "xchg", "አድራሻ_ጫን": "lea",
    "ዜሮ_አስፋ": "movzx", "ምልክት_አስፋ": "movsx",
    
    # ሂሳብ
    "ጨምር": "add", "ቀንስ": "sub", "አባዛ": "mul",
    "አካፍል": "div", "አሳድግ": "inc", "ቀንስ_አንድ": "dec",
    "ካድ": "neg",
    
    # ንፅፅር
    "አወዳድር": "cmp", "ፈትን": "test",
    
    # የቁጥጥር ፍሰት
    "ዝለል": "jmp", "ጥራ": "call", "ተመለስ": "ret",
    "እኩል_ከሆነ": "je", "ዜሮ_ከሆነ": "jz", "እኩል_ካልሆነ": "jne",
    "የበለጠ_ከሆነ": "jg", "ያነሰ_ከሆነ": "jl",
    
    # ስታክ
    "ክተት": "push", "አውጣ": "pop", "ባንዲራ_ክተት": "pushf", "ባንዲራ_አውጣ": "popf",
    
    # ገመድ (String)
    "ባይት_አንቀሳቅስ": "movsb", "ባይት_አስቀምጥ": "stosb", "ባይት_ጫን": "lodsb",
    "ደጋግም": "rep",
    
    # ሉፕ (Loop)
    "ሉፕ": "loop", "ዜሮ_ሉፕ": "loopz",
    
    # ሲስተም
    "አቋርጥ": "int", "ሲስተም_ጥራ": "syscall", "ሲስተም_ተመለስ": "sysret",
    "አቁም": "hlt", "ምንም": "nop",
    
    # መቀየር (Convert)
    "ባይት_ወደ_ቃል": "cbw", "ቃል_ወደ_ሁለት": "cwd", "ሁለት_ወደ_አራት": "cdq",
}

# Simplified/Kids mapping
KIDS_KEYWORDS = {
    "am": {"አስቀምጥ": "mov", "ጨምር": "add", "ቀንስ": "sub", "አሳይ": "syscall"},
}
