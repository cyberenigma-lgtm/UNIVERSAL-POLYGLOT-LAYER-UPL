/* ============================================================================ */
/* CÓDIGO GENERADO AUTOMÁTICAMENTE POR EL COMPILADOR SOBERANO UPLC (ANSI C99) */
/* ============================================================================ */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* --- BLINDAJE SOBERANO: LANDING-PADS DVTRGAS-30 Y NEUROCOBOL EXACTO --- */
typedef struct {
    uint8_t landing_pad_head[6];
    int64_t valor_entero;
    uint32_t escala;
    uint8_t landing_pad_tail[6];
} NeuroCOBOL_Comp3;

static inline void salto_logico_guardar(const char* nombre) {
    printf("[SALTO_LOGICO] Punto de control guardado: %s\n", nombre);
}

int32_t calcular_fibonacci(int32_t n);
int32_t main(void);

int32_t calcular_fibonacci(int32_t n) {
    if ((n <= 1)) {
        return n;
    }
    return (calcular_fibonacci((n - 1)) + calcular_fibonacci((n - 2)));
}

int32_t main(void) {
    int32_t i = 0;
    int32_t total = 0;
    while ((i < 10)) {
        total = (total + calcular_fibonacci(i));
        i = (i + 1);
    }
    salto_logico_guardar("punto_fibonacci_completado.gxp");
    return total;
}
