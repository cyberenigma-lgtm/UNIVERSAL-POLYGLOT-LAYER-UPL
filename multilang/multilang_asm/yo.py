# MultiLang-ASM Language Pack: Yoruba (yo)
# Metadata for the Babel Community system

METADATA = {
    "name": "Yoruba",
    "code": "yo",
    "author": "Neuro-OS Community",
    "version": "1.0",
    "description": "Atilẹyin kikun fun assembler ni ede Yoruba."
}

# Standard mapping: Native Keyword -> NASM Mnemonic
KEYWORDS = {
    # Iṣipopada Data
    "gbe": "mov", "paarọ": "xchg", "kojọpọ_adirẹsi": "lea",
    "faagun_odo": "movzx", "faagun_ami": "movsx",
    
    # Iṣiro
    "fikun": "add", "yọkuro": "sub", "pọsi": "mul",
    "pin": "div", "fikun_ẹyọ": "inc", "yọkuro_ẹyọ": "dec",
    "sẹ": "neg",
    
    # Ifiwe
    "fiwe": "cmp", "danwo": "test",
    
    # Iṣakoso Sisan
    "mẹfọ": "jmp", "pè": "call", "padà": "ret",
    "bi_o_ba_dogba": "je", "bi_o_ba_odo": "jz", "bi_o_ba_yatọ": "jne",
    "bi_o_ba_tobi": "jg", "bi_o_ba_kere": "jl",
    
    # Akopọ (Stack)
    "tì": "push", "yọ": "pop", "tì_asia": "pushf", "yọ_asia": "popf",
    
    # Okùn (String)
    "gbe_byte": "movsb", "fipamọ_byte": "stosb", "kojọpọ_byte": "lodsb",
    "tunṣe": "rep",
    
    # Ibalopo (Loop)
    "yipo": "loop", "yipo_bi_o_ba_odo": "loopz",
    
    # Eto (System)
    "da_duro": "int", "pe_eto": "syscall", "padà_eto": "sysret",
    "duro": "hlt", "asán": "nop",
    
    # Iyipada (Convert)
    "byte_si_ọrọ": "cbw", "ọrọ_si_meji": "cwd", "meji_si_mẹrin": "cdq",
}

# Simplified/Kids mapping
KIDS_KEYWORDS = {
    "yo": {"fi_le": "mov", "fikun": "add", "yọkuro": "sub", "fihan": "syscall"},
}
