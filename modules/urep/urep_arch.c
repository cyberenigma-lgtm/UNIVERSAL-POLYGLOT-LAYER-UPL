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

typedef struct ArchAddressConversion ArchAddressConversion;

static const uint32_t ARCH_BITS_16 = 16;
static const uint32_t ARCH_BITS_32 = 32;
static const uint32_t ARCH_BITS_64 = 64;
static const uint32_t ARCH_BITS_128 = 128;
static const uint64_t ARCH_MAX_16 = 0xFFFF;
static const uint64_t ARCH_MAX_32 = 0xFFFFFFFF;
struct ArchAddressConversion {
    uint8_t _guard_canary_head[6];
    uint64_t source_address;
    uint32_t from_bits;
    uint32_t to_bits;
    uint64_t converted_address;
    bool overflow_detected;
    uint32_t real_mode_segment;
    uint32_t real_mode_offset;
    uint8_t _guard_canary_tail[6];
};

void urep_arch_init(ArchAddressConversion* conv);
void urep_arch_init(ArchAddressConversion* conv) {
    conv->source_address = 0 ;
    conv->from_bits = 64 ;
    conv->to_bits = 64 ;
    conv->converted_address = 0 ;
    conv->overflow_detected = false ;
    conv->real_mode_segment = 0 ;
    conv->real_mode_offset = 0 ;
}

bool urep_arch_convert(ArchAddressConversion* conv, uint64_t addr, uint32_t from_bits, uint32_t to_bits);
bool urep_arch_convert(ArchAddressConversion* conv, uint64_t addr, uint32_t from_bits, uint32_t to_bits) {
    conv->source_address = addr ;
    conv->from_bits = from_bits ;
    conv->to_bits = to_bits ;
    conv->overflow_detected = false ;
    if (to_bits == ARCH_BITS_16 ) {
    if (addr > ARCH_MAX_16 ) {
    conv->overflow_detected = true ;
    conv->converted_address = addr - ( ( addr / 65536 ) * 65536 ) ;
    return false ;
}
    conv->converted_address = addr ;
    return true ;
}
    if (to_bits == ARCH_BITS_32 ) {
    if (addr > ARCH_MAX_32 ) {
    conv->overflow_detected = true ;
    conv->converted_address = addr ;
    return false ;
}
    conv->converted_address = addr ;
    return true ;
}
    conv->converted_address = addr ;
    return true ;
}

void urep_arch_calculate_segmented(ArchAddressConversion* conv, uint64_t addr);
void urep_arch_calculate_segmented(ArchAddressConversion* conv, uint64_t addr) {
    conv->real_mode_segment = addr / 16 ;
    conv->real_mode_offset = addr - ( conv->real_mode_segment * 16 ) ;
}

uint64_t urep_arch_align_boundary(uint64_t addr, uint64_t alignment);
uint64_t urep_arch_align_boundary(uint64_t addr, uint64_t alignment) {
    if (alignment == 0 ) {
    return addr ;
}
    var;
    quotient;
    u64 = addr / alignment ;
    return quotient * alignment ;
}

