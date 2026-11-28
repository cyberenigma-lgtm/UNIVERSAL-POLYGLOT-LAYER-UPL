# MultiLang-ASM Language Pack: Persian / Farsi (fa)
# Metadata for the Babel Community system

METADATA = {
    "name": "Persian / فارسی",
    "code": "fa",
    "author": "Neuro-OS Community",
    "version": "1.0",
    "description": "پشتیبانی کامل از اسمبلی به زبان فارسی."
}

# Standard mapping: Native Keyword -> NASM Mnemonic
KEYWORDS = {
    # انتقال داده
    "انتقال": "mov", "تعویض": "xchg", "بارگذاری_آدرس": "lea",
    "توسعه_صفر": "movzx", "توسعه_علامت": "movsx",
    
    # محاسبات
    "جمع": "add", "تفریق": "sub", "ضرب": "mul",
    "تقسیم": "div", "افزایش": "inc", "کاهش": "dec",
    "نفی": "neg",
    
    # مقایسه
    "مقایسه": "cmp", "تست": "test",
    
    # کنترل جریان
    "پرش": "jmp", "فراخوانی": "call", "بازگشت": "ret",
    "اگر_مساوی": "je", "اگر_صفر": "jz", "اگر_نامساوی": "jne",
    "اگر_بزرگتر": "jg", "اگر_کوچکتر": "jl",
    
    # پشته
    "فشار": "push", "پاپ": "pop", "فشار_پرچم": "pushf", "پاپ_پرچم": "popf",
    
    # رشته‌ها
    "انتقال_بایت": "movsb", "ذخیره_بایت": "stosb", "بارگذاری_بایت": "lodsb",
    "تکرار": "rep",
    
    # حلقه‌ها
    "حلقه": "loop", "حلقه_اگر_صفر": "loopz",
    
    # سیستم
    "وقفه": "int", "فراخوانی_سیستم": "syscall", "بازگشت_سیستم": "sysret",
    "توقف": "hlt", "هیچی": "nop",
    
    # تبدیل
    "بایت_به_کلمه": "cbw", "کلمه_به_دوبل": "cwd", "دوبل_به_کواد": "cdq",
}

# Simplified/Kids mapping
KIDS_KEYWORDS = {
    "fa": {"بذار": "mov", "بجم": "add", "کم_کن": "sub", "ببین": "syscall"},
}
