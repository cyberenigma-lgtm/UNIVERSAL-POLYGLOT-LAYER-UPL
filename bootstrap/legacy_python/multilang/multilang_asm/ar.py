# MultiLang-ASM Language Pack: Arabic (ar)
# Metadata for the Babel Community system

METADATA = {
    "name": "Arabic / العربية",
    "code": "ar",
    "author": "Neuro-OS Community",
    "version": "1.0",
    "description": "الدعم الكامل للأسمبلر باللغة العربية، بما في ذلك اللهجات العامية."
}

# Standard mapping: Native Keyword -> NASM Mnemonic
KEYWORDS = {
    # نقل البيانات
    "نقل": "mov", "تبديل": "xchg", "تحميل_فعال": "lea",
    "توسيع_صفر": "movzx", "توسيع_إشارة": "movsx",
    
    # العمليات الحسابية
    "جمع": "add", "إضافة": "add", "طرح": "sub",
    "ضرب": "mul", "ضرب_إشارة": "imul",
    "قسمة": "div", "قسمة_إشارة": "idiv",
    "زيادة": "inc", "نقص": "dec", "نفي": "neg",
    
    # المقارنة
    "قارن": "cmp", "اختبر": "test",
    
    # التحكم في التدفق
    "اقفز": "jmp", "اتصل": "call", "استدعاء": "call",
    "عد": "ret", "رجوع": "ret",
    "إذا_يساوي": "je", "إذا_صفر": "jz", "إذا_لا_يساوي": "jne",
    "إذا_أكبر": "jg", "إذا_أصغر": "jl",
    
    # المكدس
    "ادفع": "push", "اسحب": "pop", "ادفع_الأعلام": "pushf", "اسحب_الأعلام": "popf",
    
    # السلاسل
    "نقل_بايت": "movsb", "تخزين_بايت": "stosb", "تحميل_بايت": "lodsb",
    "كرر": "rep",
    
    # الحلقات
    "حلقة": "loop", "حلقة_إذا_صفر": "loopz",
    
    # النظام
    "مقاطعة": "int", "استدعاء_النظام": "syscall", "عودة_النظام": "sysret",
    "توقف": "hlt", "لاشيء": "nop",
    
    # التحويل
    "تحويل_بايت_كلمة": "cbw", "تحويل_كلمة_مزدوجة": "cwd", "تحويل_مزدوج_رباعي": "cdq",
}

# Simplified/Kids mapping
KIDS_KEYWORDS = {
    "ar": {"da": "mov", "ijma": "add", "itrah": "sub", "anzur": "syscall"},
    "ar_eg": {"hot": "mov", "zawed": "add", "na'as": "sub", "bos": "syscall"},
}
