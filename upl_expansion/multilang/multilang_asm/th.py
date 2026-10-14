# MultiLang-ASM Language Pack: Thai / ไทย (th)
# Metadata for the Babel Community system

METADATA = {
    "name": "Thai / ไทย",
    "code": "th",
    "author": "Neuro-OS Community",
    "version": "1.0",
    "description": "Full support for Assembly in Thai."
}

# Standard mapping: Native Keyword -> NASM Mnemonic
KEYWORDS = {
    # Movimiento
    "ย้าย": "mov", "สลับ": "xchg", "โหลด": "lea",
    
    # Aritmética
    "บวก": "add", "ลบ": "sub", "คูณ": "mul",
    # การเปรียบเทียบ
    "เปรียบเทียบ": "cmp", "ทดสอบ": "test",
    
    # การควบคุมการไหล
    "กระโดด": "jmp", "เรียก": "call", "กลับ": "ret",
    "ถ้าเท่ากับ": "je", "ถ้าเป็นศูนย์": "jz", "ถ้าไม่เท่ากับ": "jne",
    "ถ้ามากกว่า": "jg", "ถ้าน้อยกว่า": "jl",
    
    # สแต็ก
    "ผลัก": "push", "ดึง": "pop", "ผลักแฟล็ก": "pushf", "ดึงแฟล็ก": "popf",
    
    # สายอักขระ
    "ย้ายไบต์": "movsb", "เก็บบายต์": "stosb", "โหลดไบต์": "lodsb",
    "ทำซ้ำ": "rep",
    
    # ลูป
    "วนรอบ": "loop", "วนรอบถ้าเป็นศูนย์": "loopz",
    
    # ระบบ
    "ขัดจังหวะ": "int", "เรียกใช้ระบบ": "syscall", "คืนค่าระบบ": "sysret",
    "หยุด": "hlt", "ไม่มีการทำงาน": "nop",
    
    # การแปลง
    "ไบต์เป็นเวิร์ด": "cbw", "เวิร์ดเป็นดับเบิล": "cwd",
}

# Simplified/Kids mapping
KIDS_KEYWORDS = {
    "th": {"sai": "mov", "buak": "add", "lop": "sub", "sadang": "syscall"},
}
