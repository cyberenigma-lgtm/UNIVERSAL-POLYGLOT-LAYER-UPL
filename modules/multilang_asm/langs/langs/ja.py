# MultiLang-ASM Language Pack: Japanese (ja)

METADATA = {
    "name": "Japanese / 日本語",
    "code": "ja",
    "author": "Neuro-OS Community",
    "version": "1.0",
    "description": "日本語によるアセンブリ言語サポート。"
}

KEYWORDS = {
    # データ移動
    "移動": "mov", "コピー": "mov", "交換": "xchg",
    "実効アドレス読込": "lea", "ゼロ拡張": "movzx", "符号拡張": "movsx",
    
    # 算術演算
    "加算": "add", "減算": "sub", "乗算": "mul",
    "除算": "div", "インクリメント": "inc", "デクリメント": "dec",
    "否定": "neg",
    
    # 比較
    "比較": "cmp", "テスト": "test",
    
    # 制御フロー
    "ジャンプ": "jmp", "呼出": "call", "戻る": "ret",
    "等しければ": "je", "ゼロならば": "jz", "等しくなければ": "jne",
    "より大きければ": "jg", "より小さければ": "jl",
    
    # スタック
    "プッシュ": "push", "ポップ": "pop", "フラグ保存": "pushf", "フラグ復元": "popf",
    
    # 文字列操作
    "バイト移動": "movsb", "バイト格納": "stosb", "バイト読込": "lodsb",
    "繰返": "rep",
    
    # ループ
    "ループ": "loop", "ゼロならばループ": "loopz",
    
    # システム
    "割込": "int", "システムコール": "syscall", "システム帰還": "sysret",
    "停止": "hlt", "何もしない": "nop",
    
    # 変換
    "バイトをワードへ": "cbw", "ワードをダブルへ": "cwd", "ダブルをクアッドへ": "cdq",
}

KIDS_KEYWORDS = {
    "ja": {"irete": "mov", "tashite": "add", "hiite": "sub", "misete": "syscall"},
    "ja_kan": {"irete-ya": "mov", "tashite-ya": "add", "hiite-ya": "sub", "misete-ya": "syscall"},
}
