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

typedef struct UrepSecurityContext UrepSecurityContext;

static const uint32_t UREP_STATUS_OK = 0x00000000;
static const uint32_t UREP_ERR_CFI_VIOLATION = 0x00000001;
static const uint32_t UREP_ERR_STACK_CORRUPT = 0x00000002;
static const uint32_t UREP_ERR_NX_FAULT = 0x00000004;
struct UrepSecurityContext {
    uint8_t _guard_canary_head[6];
    bool active;
    uint64_t text_segment_base;
    uint64_t text_segment_limit;
    uint64_t guard_canary;
    uint32_t violations_detected;
    uint32_t mitigated_events;
    uint8_t _guard_canary_tail[6];
};

void urep_guard_init(UrepSecurityContext* ctx, uint64_t base, uint64_t limit, uint64_t canary);
void urep_guard_init(UrepSecurityContext* ctx, uint64_t base, uint64_t limit, uint64_t canary) {
    ctx->active = true ;
    ctx->text_segment_base = base ;
    ctx->text_segment_limit = limit ;
    ctx->guard_canary = canary ;
    ctx->violations_detected = 0 ;
    ctx->mitigated_events = 0 ;
}

bool urep_verify_instruction_pointer(UrepSecurityContext* ctx, uint64_t instruction_pointer);
bool urep_verify_instruction_pointer(UrepSecurityContext* ctx, uint64_t instruction_pointer) {
    if (! ctx->active ) {
    return true ;
}
    if (instruction_pointer >= ctx->text_segment_base and instruction_pointer <= ctx->text_segment_limit ) {
    return true ;
}
    ctx->violations_detected = ctx->violations_detected + 1 ;
    ctx->mitigated_events = ctx->mitigated_events + 1 ;
    return false ;
}

bool urep_verify_stack_canary(uint64_t expected_canary, uint64_t current_canary);
bool urep_verify_stack_canary(uint64_t expected_canary, uint64_t current_canary) {
    if (expected_canary == current_canary ) {
    return true ;
}
    return false ;
}

