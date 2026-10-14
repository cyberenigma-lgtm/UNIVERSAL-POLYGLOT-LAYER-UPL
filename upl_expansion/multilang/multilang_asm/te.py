# MultiLang-ASM Language Pack: Telugu (te)
# Metadata for the Babel Community system

METADATA = {
    "name": "Telugu / తెలుగు",
    "code": "te",
    "author": "Neuro-OS Community",
    "version": "1.0",
    "description": "తెలుగు భాషలో అసెంబ్లర్ కోసం పూర్తి మద్దతు."
}

# Standard mapping: Native Keyword -> NASM Mnemonic
KEYWORDS = {
    # డేటా కదలిక
    "మార్చు": "mov", "మార్పిడి": "xchg", "చిరునామా_లోడ్": "lea",
    "సున్నా_విస్తరణ": "movzx", "గుర్తు_విస్తరణ": "movsx",
    
    # అంకగణితం
    "కలుపు": "add", "తీసివేయి": "sub", "గుణించు": "mul",
    "భాగించు": "div", "పెంచు": "inc", "తగ్గించు": "dec",
    "తిరస్కరించు": "neg",
    
    # పోలిక
    "పోల్చు": "cmp", "పరీక్షించు": "test",
    
    # నియంత్రణ ప్రవాహం
    "దూకు": "jmp", "పిలువు": "call", "తిరిగి_రా": "ret",
    "సమానమైతే": "je", "సున్నా_అయితే": "jz", "సమానం_కాకపోతే": "jne",
    "ఎక్కువైతే": "jg", "తక్కువైతే": "jl",
    
    # స్టాక్
    "నెట్టు": "push", "తీయు": "pop", "ఫ్లాగ్_నెట్టు": "pushf", "ఫ్లాగ్_తీయు": "popf",
    
    # స్ట్రింగ్
    "బైట్_మార్చు": "movsb", "బైట్_సేవ్": "stosb", "బైట్_లోడ్": "lodsb",
    "మళ్ళీ_చేయి": "rep",
    
    # లూప్
    "లూప్": "loop", "సున్నా_లూప్": "loopz",
    
    # సిస్టమ్
    "అంతరాయం": "int", "సిస్టమ్_కాల్": "syscall", "సిస్టమ్_తిరిగి_రా": "sysret",
    "ఆపు": "hlt", "ఏమీ_లేదు": "nop",
    
    # మార్పిడి (Convert)
    "బైట్_వర్డ్": "cbw", "వర్డ్_డబుల్": "cwd", "డবল_క్వాడ్": "cdq",
}

# Simplified/Kids mapping
KIDS_KEYWORDS = {
    "te": {"పెట్టు": "mov", "కలుపు": "add", "తీసివేయి": "sub", "చూపించు": "syscall"},
}
