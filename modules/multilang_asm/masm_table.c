/* ============================================================================ */
/* CÓDIGO GENERADO AUTOMÁTICAMENTE POR EL COMPILADOR UPLC (ANSI C99)            */
/* ============================================================================ */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* --- PROTECCIÓN DE MEMORIA: CANARIOS DE SEGURIDAD Y TIPO DECIMAL FIJO --- */
typedef struct {
    uint8_t guard_head[6];
    int64_t valor_entero;
    uint32_t escala;
    uint8_t guard_tail[6];
} UPL_DecimalComp3;

static inline void checkpoint_guardar(const char* nombre) {
    printf("[CHECKPOINT] Estado guardado: %s\n", nombre);
}
#define salto_logico_guardar checkpoint_guardar

typedef struct MnemonicMapping MnemonicMapping;

static const uint32_t OP_CANONICAL_NOP = 0x90;
static const uint32_t OP_CANONICAL_MOV = 0x01;
static const uint32_t OP_CANONICAL_ADD = 0x02;
static const uint32_t OP_CANONICAL_SUB = 0x03;
static const uint32_t OP_CANONICAL_CMP = 0x04;
static const uint32_t OP_CANONICAL_JMP = 0x05;
static const uint32_t OP_CANONICAL_CALL = 0x06;
static const uint32_t OP_CANONICAL_RET = 0x07;
static const uint32_t OP_CANONICAL_PUSH = 0x08;
static const uint32_t OP_CANONICAL_POP = 0x09;
static const uint32_t LANG_CODE_EN = 1;
static const uint32_t LANG_CODE_ES = 2;
static const uint32_t LANG_CODE_FR = 3;
static const uint32_t LANG_CODE_PT = 4;
struct MnemonicMapping {
    uint8_t _guard_canary_head[6];
    uint32_t canonical_opcode;
    uint32_t language_id;
    bool is_valid;
    uint8_t _guard_canary_tail[6];
};

uint32_t masm_resolve_mnemonic_id(uint32_t language_id, uint32_t token_id);
uint32_t masm_resolve_mnemonic_id(uint32_t language_id, uint32_t token_id) {
    if (language_id == LANG_CODE_EN ) {
    return token_id ;
}
    if (language_id == LANG_CODE_ES ) {
    if (token_id == 101 ) {
    return OP_CANONICAL_MOV ;
}
    if (token_id == 102 ) {
    return OP_CANONICAL_ADD ;
}
    if (token_id == 103 ) {
    return OP_CANONICAL_SUB ;
}
    if (token_id == 104 ) {
    return OP_CANONICAL_CMP ;
}
    if (token_id == 105 ) {
    return OP_CANONICAL_JMP ;
}
    if (token_id == 106 ) {
    return OP_CANONICAL_CALL ;
}
    if (token_id == 107 ) {
    return OP_CANONICAL_RET ;
}
    if (token_id == 108 ) {
    return OP_CANONICAL_PUSH ;
}
    if (token_id == 109 ) {
    return OP_CANONICAL_POP ;
}
}
    if (language_id == LANG_CODE_FR ) {
    if (token_id == 201 ) {
    return OP_CANONICAL_MOV ;
}
    if (token_id == 202 ) {
    return OP_CANONICAL_ADD ;
}
    if (token_id == 203 ) {
    return OP_CANONICAL_SUB ;
}
    if (token_id == 205 ) {
    return OP_CANONICAL_JMP ;
}
    if (token_id == 207 ) {
    return OP_CANONICAL_RET ;
}
}
    return OP_CANONICAL_NOP ;
}

