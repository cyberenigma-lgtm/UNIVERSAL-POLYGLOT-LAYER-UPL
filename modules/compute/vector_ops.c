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

typedef struct Vector4D Vector4D;

struct Vector4D {
    uint8_t _guard_canary_head[6];
    uint64_t x;
    uint64_t y;
    uint64_t z;
    uint64_t w;
    uint8_t _guard_canary_tail[6];
};

void vec4_init(Vector4D* v, uint64_t x, uint64_t y, uint64_t z, uint64_t w);
void vec4_init(Vector4D* v, uint64_t x, uint64_t y, uint64_t z, uint64_t w) {
    v->x = x ;
    v->y = y ;
    v->z = z ;
    v->w = w ;
}

void vec4_add(Vector4D* dst, Vector4D* a, Vector4D* b);
void vec4_add(Vector4D* dst, Vector4D* a, Vector4D* b) {
    dst->x = a->x + b->x ;
    dst->y = a->y + b->y ;
    dst->z = a->z + b->z ;
    dst->w = a->w + b->w ;
}

void vec4_scale(Vector4D* dst, Vector4D* src, uint64_t factor);
void vec4_scale(Vector4D* dst, Vector4D* src, uint64_t factor) {
    dst->x = src->x * factor ;
    dst->y = src->y * factor ;
    dst->z = src->z * factor ;
    dst->w = src->w * factor ;
}

uint64_t vec4_dot_product(Vector4D* a, Vector4D* b);
uint64_t vec4_dot_product(Vector4D* a, Vector4D* b) {
    var;
    prod_x;
    u64 = a->x * b->x ;
    var;
    prod_y;
    u64 = a->y * b->y ;
    var;
    prod_z;
    u64 = a->z * b->z ;
    var;
    prod_w;
    u64 = a->w * b->w ;
    return prod_x + prod_y + prod_z + prod_w ;
}

