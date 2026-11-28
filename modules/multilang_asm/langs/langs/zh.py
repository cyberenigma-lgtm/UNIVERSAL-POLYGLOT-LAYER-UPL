# MultiLang-ASM Language Pack: Chinese (zh)

METADATA = {
    "name": "Chinese / 中文",
    "code": "zh",
    "author": "Neuro-OS Community",
    "version": "1.0",
    "description": "中文匯編語言支持。"
}

KEYWORDS = {
    # 資料移動
    "移動": "mov", "複製": "mov", "交換": "xchg",
    "載入有效位址": "lea", "零擴充": "movzx", "符號擴充": "movsx",
    
    # 算術
    "加": "add", "減": "sub", "乘": "mul", "除": "div",
    "遞增": "inc", "遞減": "dec", "求反": "neg",
    
    # 比較
    "比較": "cmp", "測試": "test",
    
    # 流程控制
    "跳躍": "jmp", "呼叫": "call", "返回": "ret",
    "若相等": "je", "若為零": "jz", "若不等": "jne",
    "若大於": "jg", "若小於": "jl",
    
    # 堆疊
    "推入": "push", "彈出": "pop", "推入旗標": "pushf", "彈出旗標": "popf",
    
    # 字串
    "移動位元組": "movsb", "儲存位元組": "stosb", "載入位元組": "lodsb",
    "重複": "rep",
    
    # 迴圈
    "迴圈": "loop", "若為零迴圈": "loopz",
    
    # 系統
    "中斷": "int", "系統呼叫": "syscall", "系統返回": "sysret",
    "暫停": "hlt", "無操作": "nop",
    
    # 轉換
    "位元組轉字": "cbw", "字轉雙字": "cwd", "雙字轉四字": "cdq",
}

KIDS_KEYWORDS = {
    "zh": {"fang": "mov", "jia": "add", "jian": "sub", "kan": "syscall"},
    "zh_yue": {"fong": "mov", "ga": "add", "gaam": "sub", "tai": "syscall"},
}
