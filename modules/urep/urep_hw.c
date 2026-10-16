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

typedef struct PeripheralDescriptor PeripheralDescriptor;
typedef struct HardwareRegistry HardwareRegistry;

static const uint32_t PERIPHERAL_TYPE_VGA = 0;
static const uint32_t PERIPHERAL_TYPE_VESA = 1;
static const uint32_t PERIPHERAL_TYPE_STORAGE = 2;
static const uint32_t PERIPHERAL_TYPE_NETWORK = 3;
static const uint32_t PERIPHERAL_TYPE_TIMER = 4;
static const uint64_t VGA_FRAMEBUFFER_ADDR = 0x000A0000;
static const uint64_t VGA_FRAMEBUFFER_SIZE = 64000;
static const uint64_t HPET_BASE_ADDR = 0xFED00000;
struct PeripheralDescriptor {
    uint8_t _guard_canary_head[6];
    uint32_t device_id;
    uint32_t device_type;
    uint64_t mmio_base;
    uint64_t mmio_size;
    bool is_available;
    uint8_t _guard_canary_tail[6];
};

struct HardwareRegistry {
    uint8_t _guard_canary_head[6];
    uint32_t devices_registered;
    PeripheralDescriptor primary_video;
    PeripheralDescriptor primary_timer;
    uint8_t _guard_canary_tail[6];
};

void urep_hw_init(HardwareRegistry* registry);
void urep_hw_init(HardwareRegistry* registry) {
    registry->devices_registered = 2 ;
    registry->primary_video    device_id = 1 ;
    registry->primary_video    device_type = PERIPHERAL_TYPE_VGA ;
    registry->primary_video    mmio_base = VGA_FRAMEBUFFER_ADDR ;
    registry->primary_video    mmio_size = VGA_FRAMEBUFFER_SIZE ;
    registry->primary_video    is_available = true ;
    registry->primary_timer    device_id = 2 ;
    registry->primary_timer    device_type = PERIPHERAL_TYPE_TIMER ;
    registry->primary_timer    mmio_base = HPET_BASE_ADDR ;
    registry->primary_timer    mmio_size = 1024 ;
    registry->primary_timer    is_available = true ;
}

uint64_t urep_hw_get_framebuffer(HardwareRegistry* registry);
uint64_t urep_hw_get_framebuffer(HardwareRegistry* registry) {
    if (registry->primary_video . is_available ) {
    return registry->primary_video . mmio_base ;
}
    return 0 ;
}

