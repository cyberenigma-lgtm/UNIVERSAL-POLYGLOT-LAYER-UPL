# MultiLang-ASM Language Pack: Cantonese (yue)
# Metadata for the Babel Community system

METADATA = {
    "name": "Cantonese / 粵語",
    "code": "yue",
    "author": "Neuro-OS Community",
    "version": "1.0",
    "description": "粵語組合語言完整支援。"
}

# Standard mapping: Native Keyword -> NASM Mnemonic
KEYWORDS = {
    # 數據移動
    "搬": "mov", "換": "xchg", "攞有效地址": "lea",
    "零擴展": "movzx", "符號擴展": "movsx",
    
    # 算術
    "加": "add", "減": "sub", "乘": "mul",
    "除": "div", "加一": "inc", "減一": "dec",
    "變負": "neg",
    
    # 比較
    "比": "cmp", "試": "test",
    
    # 流程控制
    "跳": "jmp", "叫": "call", "返": "ret",
    "如果等": "je", "如果零": "jz", "如果唔等": "jne",
    "如果大": "jg", "如果細": "jl",
    
    # 堆棧
    "推入": "push", "彈出": "pop", "推標誌": "pushf", "彈標誌": "popf",
    
    # 字串
    "搬位元組": "movsb", "存位元組": "stosb", "攞位元組": "lodsb",
    "重複": "rep",
    
    # 迴圈
    "圈": "loop", "零圈": "loopz",
    
    # 系統
    "中斷": "int", "叫系統": "syscall", "返系統": "sysret",
    "停": "hlt", "乜都唔做": "nop",
    
    # 轉換
    "位元組變字": "cbw", "字變雙字": "cwd", "雙字變四字": "cdq",
}

# Simplified/Kids mapping
KIDS_KEYWORDS = {
    "yue": {"放": "mov", "加": "add", "減": "sub", "畀我睇": "syscall"},
}
