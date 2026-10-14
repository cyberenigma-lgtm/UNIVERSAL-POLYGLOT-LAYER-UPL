# MultiLang-ASM Language Pack: Portuguese (pt)

METADATA = {
    "name": "Portuguese / Português",
    "code": "pt",
    "author": "Neuro-OS Community",
    "version": "1.0",
    "description": "Suporte completo para o assembler em português."
}

KEYWORDS = {
    # Movimentação
    "mover": "mov", "copiar": "mov", "trocar": "xchg",
    "carregar_efetivo": "lea", "estender_zero": "movzx", "estender_sinal": "movsx",

    # Aritmética
    "somar": "add", "adicionar": "add", "subtrair": "sub",
    "multiplicar": "mul", "multiplicar_sinal": "imul",
    "dividir": "div", "dividir_sinal": "idiv",
    "incrementar": "inc", "decrementar": "dec", "negar": "neg",

    # Lógica
    "e": "and", "ou": "or", "nao": "not", "exclusivo": "xor",
    "deslocar_esq": "shl", "deslocar_dir": "shr",
    "rotacionar_esq": "rol", "rotacionar_dir": "ror",

    # Comparação
    "comparar": "cmp", "testar": "test",

    # Fluxo
    "desviar": "jmp", "saltar": "jmp", "chamar": "call",
    "retornar": "ret", "voltar": "ret",
    "se_igual": "je", "se_cero": "jz", "se_nao_igual": "jne", "se_nao_cero": "jnz",
    "se_maior": "jg", "se_maior_igual": "jge", "si_menor": "jl", "si_menor_igual": "jle",
    "se_acima": "ja", "se_abaixo": "jb", "se_acima_igual": "jae", "se_abaixo_igual": "jbe",
    "se_sinal": "js", "se_nao_sinal": "jns", "se_transbordo": "jo", "se_nao_transbordo": "jno",
    "se_paridade": "jp", "se_nao_paridade": "jnp",

    # Pilha
    "empilhar": "push", "desempilhar": "pop",
    "empilhar_flags": "pushf", "desempilhar_flags": "popf",

    # Sistema
    "interrupcao": "int", "chamada_sistema": "syscall", "retorno_sistema": "sysret",
    "parar": "hlt", "nada": "nop",
    
    # Conversão
    "conv_byte_palavra": "cbw", "conv_palavra_dupla": "cwd", "conv_dupla_quad": "cdq",
}

KIDS_KEYWORDS = {
    "pt": {"coloca": "mov", "soma": "add", "tira": "sub", "mostra": "syscall"},
    "pt_br": {"bota": "mov", "soma": "add", "tira": "sub", "mostra": "syscall"},
}
