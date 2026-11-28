import sys
import re
import os

# ███╗   ███╗██╗     ██╗     ██╗     █████╗ ███╗   ██╗ ██████╗ 
# ████╗ ████║██║     ██║     ██║    ██╔══██╗████╗  ██║██╔════╝ 
# ██╔████╔██║██║     ██║     ██║    ███████║██╔██╗ ██║██║  ███╗
# ██║╚██╔╝██║██║     ██║     ██║    ██╔══██║██║╚██╗██║██║   ██║
# ██║ ╚═╝ ██║███████╗███████╗███████╗██║  ██║██║ ╚████║╚██████╔╝
# ╚═╝     ╚═╝╚══════╝╚══════╝╚══════╝╚═╝  ╚═╝╚═╝  ╚═══╝ ╚═════╝ 
#
#        MultiLang-ASM v0.7 — Parte del ecosistema UPL+ / Neuro-OS
#        Babel Global Expansion Edition | Technical Parity
#        https://github.com/cyberenigma-lgtm/UNIVERSAL-POLYGLOT-LAYER-UPL

# Fix para Windows: forzar UTF-8 en consola
if os.name == 'nt':
    import codecs
    sys.stdout = codecs.getwriter('utf-8')(sys.stdout.buffer, 'strict')
    sys.stderr = codecs.getwriter('utf-8')(sys.stderr.buffer, 'strict')

# ============================================================================
# NÚCLEO CANÓNICO DE INSTRUCCIONES (independiente del idioma)
# ============================================================================
INSTRUCTIONS = {
    # Movimiento
    "mov": {"category": "movimiento"},
    "xchg": {"category": "movimiento"},
    "lea": {"category": "movimiento"},
    "movzx": {"category": "movimiento"},
    "movsx": {"category": "movimiento"},

    # Aritmética
    "add": {"category": "aritmetica"},
    "sub": {"category": "aritmetica"},
    "mul": {"category": "aritmetica"},
    "imul": {"category": "aritmetica"},
    "div": {"category": "aritmetica"},
    "idiv": {"category": "aritmetica"},
    "inc": {"category": "aritmetica"},
    "dec": {"category": "aritmetica"},
    "neg": {"category": "aritmetica"},

    # Lógica
    "and": {"category": "logica"},
    "or": {"category": "logica"},
    "not": {"category": "logica"},
    "xor": {"category": "logica"},
    "shl": {"category": "logica"},
    "shr": {"category": "logica"},
    "sal": {"category": "logica"},
    "sar": {"category": "logica"},
    "rol": {"category": "logica"},
    "ror": {"category": "logica"},

    # Comparación
    "cmp": {"category": "comparacion"},
    "test": {"category": "comparacion"},

    # Flujo
    "jmp": {"category": "flujo"},
    "call": {"category": "flujo"},
    "ret": {"category": "flujo"},
    "je": {"category": "flujo"},
    "jz": {"category": "flujo"},
    "jne": {"category": "flujo"},
    "jnz": {"category": "flujo"},
    "jg": {"category": "flujo"},
    "jge": {"category": "flujo"},
    "jl": {"category": "flujo"},
    "jle": {"category": "flujo"},
    "ja": {"category": "flujo"},
    "jb": {"category": "flujo"},
    "jae": {"category": "flujo"},
    "jbe": {"category": "flujo"},
    "js": {"category": "flujo"},
    "jns": {"category": "flujo"},
    "jo": {"category": "flujo"},
    "jno": {"category": "flujo"},
    "jp": {"category": "flujo"},
    "jnp": {"category": "flujo"},

    # Pila
    "push": {"category": "pila"},
    "pop": {"category": "pila"},
    "pushf": {"category": "pila"},
    "popf": {"category": "pila"},

    # Cadenas
    "movsb": {"category": "cadenas"},
    "movsw": {"category": "cadenas"},
    "movsd": {"category": "cadenas"},
    "stosb": {"category": "cadenas"},
    "lodsb": {"category": "cadenas"},
    "scasb": {"category": "cadenas"},
    "rep": {"category": "cadenas"},
    "repne": {"category": "cadenas"},

    # Bucles
    "loop": {"category": "bucles"},
    "loopz": {"category": "bucles"},
    "loopnz": {"category": "bucles"},

    # Sistema
    "int": {"category": "sistema"},
    "syscall": {"category": "sistema"},
    "sysret": {"category": "sistema"},
    "iret": {"category": "sistema"},
    "iretq": {"category": "sistema"},

    # Miscelánea
    "nop": {"category": "miscelanea"},
    "hlt": {"category": "miscelanea"},
    "cli": {"category": "miscelanea"},
    "sti": {"category": "miscelanea"},
    "cld": {"category": "miscelanea"},
    "std": {"category": "miscelanea"},
    "wait": {"category": "miscelanea"},

    # Conversión
    "cbw": {"category": "conversion"},
    "cwd": {"category": "conversion"},
    "cdq": {"category": "conversion"},
    "cqo": {"category": "conversion"},
}

import importlib.util

# ============================================================================
# SISTEMA DE CARGA DINÁMICA - MultiLang-ASM v0.6 (Babel Community)
# ============================================================================

TABLE = {}
KIDS_INSTRUCTIONS = {}
LANG_METADATA = {}

def load_language_packs():
    """Carga dinámicamente todos los paquetes de idiomas desde la carpeta 'langs/'."""
    global TABLE, KIDS_INSTRUCTIONS, LANG_METADATA
    langs_dir = os.path.join(os.path.dirname(__file__), "langs")
    
    if not os.path.exists(langs_dir):
        os.makedirs(langs_dir)
        return

    for filename in os.listdir(langs_dir):
        if filename.endswith(".py") and filename != "__init__.py":
            lang_code = filename[:-3]
            file_path = os.path.join(langs_dir, filename)
            
            # Carga modular dinámica
            spec = importlib.util.spec_from_file_location(lang_code, file_path)
            module = importlib.util.module_from_spec(spec)
            try:
                spec.loader.exec_module(module)
                
                # Integrar diccionarios
                if hasattr(module, "KEYWORDS"):
                    TABLE[lang_code] = module.KEYWORDS
                if hasattr(module, "KIDS_KEYWORDS"):
                    KIDS_INSTRUCTIONS.update(module.KIDS_KEYWORDS)
                if hasattr(module, "METADATA"):
                    LANG_METADATA[lang_code] = module.METADATA
            except Exception as e:
                print(f"⚠️ Error cargando paquete '{lang_code}': {e}")

# Cargar idiomas al inicio
load_language_packs()

# ============================================================================
# TABLA DE ALIAS POR IDIOMA (Idioma Nativo -> Mnemónico Estándar)
# ============================================================================
# Nota: Los diccionarios TABLE y PRETTY se cargan dinámicamente en v0.6.

# ============================================================================
# MAPA INVERSO (Mnemónico Estándar -> Palabra Canónica por Idioma)
# Para modo reversible
# ============================================================================
PRETTY = {
    "es": {
        "mov": "mover", "xchg": "intercambiar", "lea": "cargar_efectivo",
        "movzx": "extender_cero", "movsx": "extender_signo",
        "add": "sumar", "sub": "restar", "mul": "multiplicar",
        "imul": "multiplicar_signado", "div": "dividir", "idiv": "dividir_signado",
        "inc": "incrementar", "dec": "decrementar", "neg": "negar",
        "and": "y", "or": "o", "not": "no", "xor": "exclusivo",
        "shl": "desplazar_izq", "shr": "desplazar_der",
        "sal": "desplazar_arit_izq", "sar": "desplazar_arit_der",
        "rol": "rotar_izq", "ror": "rotar_der",
        "cmp": "comparar", "test": "probar",
        "jmp": "saltar", "call": "llamar", "ret": "retornar",
        "je": "si_igual", "jz": "si_cero", "jne": "si_no_igual", "jnz": "si_no_cero",
        "jg": "si_mayor", "jge": "si_mayor_igual", "jl": "si_menor", "jle": "si_menor_igual",
        "ja": "si_arriba", "jb": "si_abajo", "jae": "si_arriba_igual", "jbe": "si_abajo_igual",
        "js": "si_signo", "jns": "si_no_signo", "jo": "si_desborde", "jno": "si_no_desborde",
        "jp": "si_paridad", "jnp": "si_no_paridad",
        "push": "meter", "pop": "sacar", "pushf": "meter_banderas", "popf": "sacar_banderas",
        "movsb": "mover_byte", "movsw": "mover_palabra", "movsd": "mover_doble",
        "stosb": "almacenar_byte", "lodsb": "cargar_byte", "scasb": "escanear_byte",
        "rep": "repetir", "repne": "repetir_mientras",
        "loop": "ciclo", "loopz": "ciclo_si_cero", "loopnz": "ciclo_si_no_cero",
        "int": "interrupcion", "syscall": "llamada_sistema",
        "sysret": "retorno_sistema", "iret": "retorno_interrupcion",
        "nop": "nada", "hlt": "detener",
        "cli": "limpiar_interrupciones", "sti": "activar_interrupciones",
        "cld": "limpiar_direccion", "std": "fijar_direccion", "wait": "esperar",
        "cbw": "convertir_byte_palabra", "cwd": "convertir_palabra_doble",
        "cdq": "convertir_doble_cuadruple", "cqo": "convertir_cuadruple_octo",
    },
    
    "fr": {
        "mov": "deplacer", "xchg": "echanger", "lea": "charger_effectif",
        "movzx": "etendre_zero", "movsx": "etendre_signe",
        "add": "ajouter", "sub": "soustraire", "mul": "multiplier",
        "imul": "multiplier_signe", "div": "diviser", "idiv": "diviser_signe",
        "inc": "incrementer", "dec": "decrementer", "neg": "negativer",
        "and": "et", "or": "ou", "not": "non", "xor": "ou_exclusif",
        "cmp": "comparer", "test": "tester",
        "jmp": "sauter", "call": "appeler", "ret": "retourner",
        "je": "si_egal", "jne": "si_pas_egal",
        "push": "empiler", "pop": "depiler",
        "int": "interruption", "syscall": "appel_systeme",
    },

    "pt": {
        "mov": "mover", "xchg": "trocar", "lea": "carregar_efetivo",
        "movzx": "estender_zero", "movsx": "estender_sinal",
        "add": "somar", "sub": "subtrair", "mul": "multiplicar",
        "imul": "multiplicar_sinal", "div": "dividir", "idiv": "dividir_sinal",
        "inc": "incrementar", "dec": "decrementar", "neg": "negar",
        "and": "e", "or": "ou", "not": "nao", "xor": "exclusivo",
        "shl": "deslocar_esq", "shr": "deslocar_dir",
        "rol": "rotacionar_esq", "ror": "rotacionar_dir",
        "cmp": "comparar", "test": "testar",
        "jmp": "desviar", "call": "chamar", "ret": "retornar",
        "je": "se_igual", "jz": "se_zero", "jne": "se_nao_igual", "jnz": "se_nao_zero",
        "jg": "se_maior", "jge": "se_maior_igual", "jl": "se_menor", "jle": "se_menor_igual",
        "ja": "se_acima", "jb": "se_abaixo", "jae": "se_acima_igual", "jbe": "se_abaixo_igual",
        "js": "se_sinal", "jns": "se_nao_sinal", "jo": "se_transbordo", "jno": "se_nao_transbordo",
        "jp": "se_paridade", "jnp": "se_nao_paridade",
        "push": "empilhar", "pop": "desempilhar", "pushf": "empilhar_flags", "popf": "desempilhar_flags",
        "movsb": "mover_byte", "movsw": "mover_palavra", "movsd": "mover_dupla",
        "stosb": "armazenar_byte", "lodsb": "cargar_byte", "scasb": "escanear_byte",
        "rep": "repetir", "repne": "repetir_enquanto",
        "loop": "repetir_ciclo", "loopz": "repetir_se_zero", "loopnz": "repetir_se_nao_zero",
        "int": "interrupcao", "syscall": "chamada_sistema",
        "sysret": "retorno_sistema", "iret": "retorno_interrupcao",
        "nop": "nada", "hlt": "parar",
        "cli": "limpar_interrupcoes", "sti": "ativar_interrupcoes",
        "cld": "limpar_direcao", "std": "fixar_direcao", "wait": "esperar",
        "cbw": "converter_byte_palavra", "cwd": "converter_palavra_dupla",
        "cdq": "converter_dupla_quadrupla", "cqo": "converter_quadrupla_octupla",
    },
    
    "de": {
        "mov": "bewegen", "xchg": "tauschen", "lea": "effektiv_laden",
        "movzx": "null_erweitern", "movsx": "vorzeichen_erweitern",
        "add": "addieren", "sub": "subtrahieren", "mul": "multiplizieren",
        "imul": "multiplizieren_vorzeichen", "div": "dividieren", "idiv": "dividieren_vorzeichen",
        "inc": "inkrementieren", "dec": "dekrementieren", "neg": "negieren",
        "and": "und", "or": "oder", "not": "nicht", "xor": "exklusiv_oder",
        "jmp": "springen", "call": "rufen", "ret": "zurueckkehren",
        "int": "unterbrechung",
    },
    
    "it": {
        "mov": "spostare", "xchg": "scambiare", "lea": "caricare_effettivo",
        "movzx": "estendere_zero", "movsx": "estendere_segno",
        "add": "sommare", "sub": "sottrarre", "mul": "moltiplicare",
        "imul": "moltiplicare_segno", "div": "dividere", "idiv": "dividere_segno",
        "inc": "incrementare", "dec": "decrementare", "neg": "negare",
        "and": "e", "or": "o", "not": "non", "xor": "esclusivo",
        "jmp": "saltare", "call": "chiamare", "ret": "ritornare",
        "je": "se_uguale", "jne": "se_non_uguale",
        "push": "spingere", "pop": "estrarre",
        "int": "interruzione", "syscall": "chiamata_sistema",
    },
    
    "ar": {
        "mov": "نقل", "xchg": "تبديل", "lea": "تحميل_فعال",
        "movzx": "توسيع_صفر", "movsx": "توسيع_إشارة",
        "add": "جمع", "sub": "طرح", "mul": "ضرب",
        "imul": "ضرب_إشارة", "div": "قسمة", "idiv": "قسمة_إشارة",
        "inc": "زيادة", "dec": "نقص", "neg": "نفي",
        "and": "و", "or": "أو", "not": "ليس", "xor": "أو_حصري",
        "jmp": "اقفز", "call": "استدعاء", "ret": "رجوع",
        "je": "إذا_يساوي", "jne": "إذا_لا_يساوي",
        "push": "ادفع", "pop": "اسحب",
        "int": "مقاطعة", "syscall": "استدعاء_النظام",
    },
    
    "ru": {
        "mov": "перенести", "xchg": "обменять", "lea": "загрузить_эффективный",
        "movzx": "расширить_нулями", "movsx": "расширить_знаком",
        "add": "добавить", "sub": "вычесть", "mul": "умножить",
        "imul": "умножить_знак", "div": "разделить", "idiv": "разделить_знак",
        "inc": "увеличить", "dec": "уменьшить", "neg": "отрицание",
        "and": "и", "or": "или", "not": "не", "xor": "исключающее",
        "jmp": "прыгнуть", "call": "вызвать", "ret": "вернуться",
        "je": "если_равно", "jne": "если_не_равно",
        "push": "положить", "pop": "извлечь",
        "int": "прерывание", "syscall": "системный_вызов",
    },
    
    "ko": {
        "mov": "이동", "xchg": "교환", "lea": "주소로드",
        "movzx": "영확장", "movsx": "부호확장",
        "add": "더하기", "sub": "빼기", "mul": "곱하기",
        "imul": "부호곱하기", "div": "나누기", "idiv": "부호나누기",
        "inc": "증가", "dec": "감소", "neg": "부정",
        "and": "그리고", "or": "또는", "xor": "배타적",
        "jmp": "점프", "call": "호출", "ret": "돌아가기",
        "je": "같으면", "jne": "다르면",
        "push": "넣기", "pop": "빼기",
        "int": "인터럽트", "syscall": "시스템호출",
    },
    
    "id": {
        "mov": "pindah", "xchg": "tukar", "lea": "muat_efektif",
        "movzx": "perpanjang_nol", "movsx": "perpanjang_tanda",
        "add": "tambah", "sub": "kurang", "mul": "kali",
        "imul": "kali_tanda", "div": "bagi", "idiv": "bagi_tanda",
        "inc": "tambah_satu", "dec": "kurang_satu", "neg": "negatif",
        "and": "dan", "or": "atau", "not": "tidak", "xor": "eksklusif",
        "jmp": "lompat", "call": "panggil", "ret": "kembali",
        "je": "jika_sama", "jne": "jika_beda",
        "push": "masukkan", "pop": "keluarkan",
        "int": "interupsi", "syscall": "panggilan_sistem",
    },
    
    "zh": {
        "mov": "移動", "xchg": "交換", "lea": "載入有效位址",
        "movzx": "零擴充", "movsx": "符號擴充",
        "add": "加", "sub": "減", "mul": "乘",
        "imul": "有符號乘", "div": "除", "idiv": "有符號除",
        "inc": "遞增", "dec": "遞減", "neg": "取負",
        "and": "且", "or": "或", "not": "非", "xor": "互斥或",
        "jmp": "跳躍", "call": "呼叫", "ret": "返回",
        "je": "若相等", "jne": "若不等",
        "push": "推入", "pop": "彈出",
        "int": "中斷", "syscall": "系統呼叫",
    },
    
    "ja": {
        "mov": "移動", "xchg": "交換", "lea": "実効アドレス読込",
        "movzx": "ゼロ拡張", "movsx": "符号拡張",
        "add": "加算", "sub": "減算", "mul": "乗算",
        "imul": "符号付乗算", "div": "除算", "idiv": "符号付除算",
        "inc": "インクリメント", "dec": "デクリメント", "neg": "否定",
        "and": "論理積", "or": "論理和", "xor": "排他的論理和",
        "jmp": "ジャンプ", "call": "呼出", "ret": "戻る",
        "je": "等しければ", "jne": "等しくなければ",
        "push": "プッシュ", "pop": "ポップ",
        "int": "割込", "syscall": "システムコール",
    },

    "pt": {
        "mov": "mover", "xchg": "trocar", "lea": "carregar_efetivo",
        "movzx": "estender_zero", "movsx": "estender_sinal",
        "add": "somar", "sub": "subtrair", "mul": "multiplicar",
        "imul": "multiplicar_sinal", "div": "dividir", "idiv": "dividir_sinal",
        "inc": "incrementar", "dec": "decrementar", "neg": "negar",
        "and": "e", "or": "ou", "not": "nao", "xor": "exclusivo",
        "shl": "deslocar_esq", "shr": "deslocar_dir",
        "rol": "rotacionar_esq", "ror": "rotacionar_dir",
        "cmp": "comparar", "test": "testar",
        "jmp": "desviar", "call": "chamar", "ret": "retornar",
        "je": "se_igual", "jne": "se_nao_igual",
        "jg": "se_maior", "jge": "se_maior_igual",
        "jl": "se_menor", "jle": "se_menor_igual",
        "ja": "se_acima", "jb": "se_abaixo",
        "jae": "se_acima_igual", "jbe": "se_abaixo_igual",
        "js": "se_sinal", "jns": "se_nao_sinal",
        "jo": "se_transbordo", "jno": "se_nao_transbordo",
        "jp": "se_paridade", "jnp": "se_nao_paridade",
        "push": "empilhar", "pop": "desempilhar",
        "pushf": "empilhar_flags", "popf": "desempilhar_flags",
        "movsb": "mover_byte", "movsw": "mover_palavra", "movsd": "mover_dupla",
        "stosb": "armazenar_byte", "lodsb": "cargar_byte", "scasb": "escanear_byte",
        "rep": "repetir", "repne": "repetir_enquanto",
        "loop": "repetir_ciclo", "loopz": "repetir_se_zero",
        "loopnz": "repetir_se_nao_zero",
        "int": "interrupcao", "syscall": "chamada_sistema",
        "sysret": "retorno_sistema", "iret": "retorno_interrupcao",
        "nop": "nada", "hlt": "parar",
        "cli": "limpar_interrupcoes", "sti": "activar_interrupcoes",
        "cld": "limpiar_direcao", "std": "fixar_direcao", "wait": "esperar",
        "cbw": "converter_byte_palavra", "cwd": "converter_palavra_dupla",
        "cdq": "converter_dupla_quadrupla", "cqo": "converter_quadrupla_octupla",
    },

    "hi": {
        "mov": "bhejo", "add": "joro", "sub": "ghatao",
        "jmp": "kudo", "call": "bulao", "ret": "wapas",
        "and": "aur", "or": "ya", "not": "nahi",
        "cmp": "tulna", "test": "pariksha",
        "inc": "badhao", "dec": "ghatao_ek",
    },
    
    "tr": {
        "mov": "taşı", "add": "ekle", "sub": "çıkar",
        "jmp": "atla", "call": "çağır", "ret": "dön",
        "and": "ve", "or": "veya", "not": "degil",
        "cmp": "karsilastir", "test": "sina",
        "inc": "artir", "dec": "azalt",
    },
    
    "pl": {
        "mov": "przesun", "add": "dodaj", "sub": "odejmij",
        "jmp": "skocz", "call": "wywolaj", "ret": "wroc",
        "and": "oraz", "or": "lub", "not": "nie",
        "cmp": "porownaj", "test": "testuj",
        "inc": "zwieksz", "dec": "zmniejsz",
    },
    
    "sv": {
        "mov": "flytta", "add": "addera", "sub": "subtrahera",
        "jmp": "hoppa", "call": "kalla", "ret": "returnera",
        "and": "och", "or": "eller", "not": "inte",
        "cmp": "jamfor", "test": "testa",
        "inc": "oka", "dec": "minska",
    },
    
    "nl": {
        "mov": "verplaats", "add": "optellen", "sub": "aftrekken",
        "jmp": "spring", "call": "roep", "ret": "terug",
        "and": "en", "or": "of", "not": "niet",
        "cmp": "vergelijk", "test": "test",
        "inc": "verhoog", "dec": "verlaag",
    },
}

# Sincronización bidireccional desde PRETTY para soporte políglota universal
for _l, _mappings in PRETTY.items():
    if _l not in TABLE:
        TABLE[_l] = {}
    for _std, _nat in _mappings.items():
        TABLE[_l][_nat] = _std
    if _l not in LANG_METADATA:
        LANG_METADATA[_l] = {"name": _l.upper(), "author": "Neuro-OS Sovereign Team"}

ERRORS = {
    "en": {
        "file_not_found": "❌ Error: File '{}' not found",
        "io_error": "❌ Unexpected Error: {}",
        "auto_detect_ok": "🔍 Auto-Detect: Identified Language -> {}",
        "auto_detect_fail": "⚠️ Auto-Detect: Could not identify language. Defaulting to 'en'.",
        "success": "   Status: ✅ OK",
        "mode_native": "Native (View)",
        "mode_standard": "Standard (NASM)"
    },
    "es": {
        "file_not_found": "❌ Error: No se encontró el archivo '{}'",
        "io_error": "❌ Error inesperado: {}",
        "auto_detect_ok": "🔍 Auto-Detect: Idioma identificado -> {}",
        "auto_detect_fail": "⚠️ Auto-Detect: No se pudo identificar el idioma. Usando 'en' (Standard).",
        "success": "   Estado: ✅ OK",
        "mode_native": "Natificado (Vista)",
        "mode_standard": "Estándar (NASM)"
    },
    "fr": {
        "file_not_found": "❌ Erreur: Fichier '{}' non trouvé",
        "io_error": "❌ Erreur inattendue: {}",
        "auto_detect_ok": "🔍 Auto-Detect: Langue identifiée -> {}",
        "auto_detect_fail": "⚠️ Auto-Detect: Impossible d'identifier la langue. Utilisation de 'en'.",
        "success": "   Statut: ✅ OK",
        "mode_native": "Natif (Vue)",
        "mode_standard": "Standard (NASM)"
    },
    "de": {
        "file_not_found": "❌ Fehler: Datei '{}' nicht gefunden",
        "io_error": "❌ Unerwarteter Fehler: {}",
        "auto_detect_ok": "🔍 Auto-Detect: Sprache identifiziert -> {}",
        "auto_detect_fail": "⚠️ Auto-Detect: Sprache konnte nicht identifiziert werden. Standardmäßig 'en'.",
        "success": "   Status: ✅ OK",
        "mode_native": "Nativ (Ansicht)",
        "mode_standard": "Standard (NASM)"
    },
    "ru": {
        "file_not_found": "❌ Ошибка: Файл '{}' не найден",
        "io_error": "❌ Непредвиденная ошибка: {}",
        "auto_detect_ok": "🔍 Автоопределение: Язык определен -> {}",
        "auto_detect_fail": "⚠️ Автоопределение: Не удалось определить язык. По умолчанию 'en'.",
        "success": "   Статус: ✅ OK",
        "mode_native": "Родной (Вид)",
        "mode_standard": "Стандарт (NASM)"
    }
}
# Default to English for other languages for now
for l in TABLE:
    if l not in ERRORS:
        ERRORS[l] = ERRORS["en"]

def get_msg(key, lang, *args):
    """Retrieve localized message"""
    # Fallback to English if language not supported in ERRORS
    msgs = ERRORS.get(lang, ERRORS["en"])
    msg = msgs.get(key, ERRORS["en"][key])
    msg = msgs.get(key, ERRORS["en"][key])
    return msg.format(*args)

def process_macros(code):
    """
    Procesa macros simples (%define CLAVE VALOR)
    Retorna el código con las sustituciones aplicadas.
    """
    lines = code.split('\n')
    processed_lines = []
    macros = {}
    
    # 1. Scanner de definiciones
    for line in lines:
        line_strip = line.strip()
        
        # Detectar %define
        if line_strip.lower().startswith('%define'):
            parts = line_strip.split(None, 2)
            if len(parts) >= 3:
                key = parts[1]
                val = parts[2]
                macros[key] = val
            continue # No añadir la línea de definición al output final
            
        processed_lines.append(line)
        
    # 2. Reemplazo (naive string replacement)
    final_code = '\n'.join(processed_lines)
    
    if not macros:
        return final_code
        
    for key, val in macros.items():
        # Usar regex para reemplazar solo palabras completas
        pattern = r'(?<!\w)' + re.escape(key) + r'(?!\w)'
        final_code = re.sub(pattern, val, final_code)
        
    return final_code

def translate_token(token, lang):
    """
    Traduce un token del idioma nativo al mnemónico estándar.
    
    Fallback:
    1. Si está en TABLE[lang] -> traduce
    2. Si es mnemónico estándar en INSTRUCTIONS -> deja tal cual
    3. Si no, devuelve tal cual (etiquetas, registros, números, etc.)
    """
    # Intentar traducción desde tabla de idioma (Standard)
    if lang in TABLE and token in TABLE[lang]:
        return TABLE[lang][token]

    # Intentar traducción desde KIDS MODE (y dialectos)
    if lang in KIDS_INSTRUCTIONS and token in KIDS_INSTRUCTIONS[lang]:
        return KIDS_INSTRUCTIONS[lang][token]
    
    # Si ya es un mnemónico estándar, dejarlo tal cual
    if token in INSTRUCTIONS:
        return token
    
    # Cualquier otro caso (etiquetas, registros, números)
    return token

def pretty_mnemonic(mnemonic, lang):
    """
    Convierte un mnemónico estándar a su forma nativa para visualización.
    
    Fallback:
    - Si existe en PRETTY[lang] -> usa la forma nativa
    - Si no, devuelve el mnemónico estándar
    """
    if lang in PRETTY and mnemonic in PRETTY[lang]:
        return PRETTY[lang][mnemonic]
    return mnemonic

def translate(code, lang, to_standard=True):
    """Traduce código entre mnemónicos nativos y estándar"""
    
    # PROCESAR MACROS (Si estamos traduciendo código fuente)
    if to_standard:
        code = process_macros(code)
        
    lines = code.split('\n')
    translated_lines = []
    
    for line in lines:
        # Preservar comentarios
        comment = ""
        if ';' in line:
            line, comment = line.split(';', 1)
            comment = ';' + comment
        
        # Regex robusto: soporta etiquetas, puntos, guiones, símbolos Unicode
        match = re.match(r'^(\s*)([A-Za-zÀ-ÿ_\.0-9À-ỹ가-힣一-龯ぁ-ゔァ-ヴー々〆〤]+)(.*)', line)
        if match:
            indent, token, rest = match.groups()
            token_lower = token.lower()
            
            if to_standard:
                # Modo: Nativo -> Estándar
                translated_token = translate_token(token_lower, lang)
            else:
                # Modo: Estándar -> Nativo (reversible)
                translated_token = pretty_mnemonic(token_lower, lang)
            
            translated_lines.append(f"{indent}{translated_token}{rest}{comment}")
        else:
            translated_lines.append(line + comment)
            
    return '\n'.join(translated_lines)



def detect_language(code):
    """
    Detecta automáticamente el idioma del código fuente basándose en la frecuencia de palabras clave.
    """
    scores = {lang: 0 for lang in TABLE.keys()}
    
    # Tokenizar todo el código (palabras simples)
    tokens = re.findall(r'[A-Za-zÀ-ÿ_\.0-9]+', code.lower())
    
    for token in tokens:
        for lang, keywords in TABLE.items():
            if token in keywords:
                # Standard match
                scores[lang] += 1
        
        for lang, keywords in KIDS_INSTRUCTIONS.items():
            if token in keywords:
                # Kids match (normalize simple lang codes if possible, or keep dialect)
                # If dialect creates a new key in scores, so be it
                if lang not in scores: scores[lang] = 0
                scores[lang] += 2  # Weighted higher for specific kids terms
                
    # Encontrar el idioma con más coincidencias
    best_lang = max(scores, key=scores.get)
    
    # Si no hay coincidencias claras (score 0), asumir 'es' o error?
    if scores[best_lang] == 0:
        return None
        
    return best_lang

def list_languages():
    print("🛡️ MultiLang-ASM v0.7 - Paquetes de Idiomas Instalados:")
    print("-" * 50)
    for code, meta in LANG_METADATA.items():
        print(f"[{code.upper()}] {meta.get('name', 'N/A')}")
        print(f"   Autor: {meta.get('author', 'N/A')}")
        print(f"   Versión: {meta.get('version', 'N/A')}")
        print(f"   Descripción: {meta.get('description', '')}\n")

def create_language_template(lang_code):
    template = f'''# MultiLang-ASM Language Pack: {lang_code.upper()}
# Generated by Babel CLI

METADATA = {{
    "name": "{lang_code.upper()}",
    "code": "{lang_code}",
    "author": "Tu Nombre/Comunidad",
    "version": "1.0",
    "description": "Soporte para ensamblador en {lang_code.upper()}."
}}

KEYWORDS = {{
    # Mapea aquí tus palabras clave a mnemonics de NASM
    "mover": "mov",
    "sumar": "add",
    # ... añade más
}}

KIDS_KEYWORDS = {{
    "{lang_code}": {{"pon": "mov", "suma": "add", "resta": "sub", "enseña": "syscall"}},
}}
'''
    langs_dir = os.path.join(os.path.dirname(__file__), "langs")
    file_path = os.path.join(langs_dir, f"{lang_code}.py")
    with open(file_path, "w", encoding="utf-8") as f:
        f.write(template)
    print(f"✅ Plantilla para '{lang_code}' creada con éxito en {file_path}")

def main():
    if len(sys.argv) < 2:
        print("🛡️ MultiLang-ASM v0.7 (Global Expansion Edition)")
        print("Uso:")
        print("  python mlasm.py <idioma> <entrada> <salida> [--reverse]")
        print("  python mlasm.py --list-langs           (Lista idiomas instalados)")
        print("  python mlasm.py --new-lang <código>    (Crea plantilla para nuevo idioma)")
        return

    # Comandos Especiales v0.6
    if sys.argv[1] == "--list-langs":
        list_languages()
        return
    if sys.argv[1] == "--new-lang" and len(sys.argv) > 2:
        create_language_template(sys.argv[2])
        return

    if len(sys.argv) < 4:
        print("⚠️ Error: Faltan argumentos. Usa 'python mlasm.py' para ayuda.")
        return

    lang = sys.argv[1]
    input_file = sys.argv[2]
    output_file = sys.argv[3]
    reverse = "--reverse" in sys.argv

    try:
        with open(input_file, 'r', encoding='utf-8') as f:
            content = f.read()

        # LOGICA AUTO-DETECT
        if lang == 'auto':
            detected = detect_language(content)
            if detected:
                lang = detected
                print(get_msg("auto_detect_ok", lang, lang.upper()))
            else:
                print(get_msg("auto_detect_fail", "en"))
                lang = 'en'

        # MACROS (Ahora se procesan dentro de translate)
        translated = translate(content, lang, to_standard=not reverse)

        with open(output_file, 'w', encoding='utf-8') as f:
            f.write(translated)

        # Mensajes humanos mejorados
        mode = get_msg("mode_native" if reverse else "mode_standard", lang)
        
        print(f"🛡️ MultiLang-ASM v0.7 (Global Expansion)")
        print(f"   Idioma: {lang.upper()}")
        print(f"   Modo: {mode}")
        print(f"   Entrada: {input_file}")
        print(f"   Salida: {output_file}")
        print(f"{get_msg('success', lang)}")

    except FileNotFoundError:
        # Try to use 'lang' if set, else 'en'
        print(get_msg("file_not_found", lang if lang != 'auto' else 'en', input_file))
    except Exception as e:
        print(get_msg("io_error", lang if lang != 'auto' else 'en', e))

if __name__ == "__main__":
    main()
