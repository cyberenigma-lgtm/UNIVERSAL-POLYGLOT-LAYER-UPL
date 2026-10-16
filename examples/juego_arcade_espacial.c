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

typedef struct NaveEspacial NaveEspacial;
typedef struct ProyectilLaser ProyectilLaser;

struct NaveEspacial {
    int32_t x;
    int32_t y;
    int32_t vidas;
    uint32_t escudo;
    uint64_t puntos;
};

struct ProyectilLaser {
    int32_t x;
    int32_t y;
    bool activo;
};

NaveEspacial crear_nave(void);
NaveEspacial crear_nave(void) {
    NaveEspacial nave;
    nave.x = 40 ;
    nave.y = 20 ;
    nave.vidas = 3 ;
    nave.escudo = 100 ;
    nave.puntos = 0 ;
    return nave ;
}

void disparar_laser(NaveEspacial* nave, ProyectilLaser* laser);
void disparar_laser(NaveEspacial* nave, ProyectilLaser* laser) {
    if (! laser->activo ) {
    laser->x = nave->x ;
    laser->y = nave->y - 1 ;
    laser->activo = true ;
}
}

bool actualizar_fisica(NaveEspacial* nave, ProyectilLaser* laser, int32_t enemigo_x, int32_t enemigo_y);
bool actualizar_fisica(NaveEspacial* nave, ProyectilLaser* laser, int32_t enemigo_x, int32_t enemigo_y) {
    if (laser->activo ) {
    laser->y = laser->y - 1 ;
    if (laser->y <= 0 ) {
    laser->activo = false ;
}
    if (laser->x == enemigo_x && laser->y == enemigo_y ) {
    laser->activo = false ;
    nave->puntos = nave->puntos + 100 ;
    return true ;
}
}
    return false ;
}

int32_t main(void);
int32_t main(void) {
    NaveEspacial jugador = crear_nave ( ) ;
    ProyectilLaser laser;
    laser.x = 0 ;
    laser.y = 0 ;
    laser.activo = false ;
    int32_t enemigo_x = 40 ;
    int32_t enemigo_y = 5 ;
    disparar_laser(& jugador , & laser );
    int32_t frame = 0 ;
    while (frame < 16 ) {
    bool impacto = actualizar_fisica ( & jugador , & laser , enemigo_x , enemigo_y ) ;
    if (impacto ) {
    checkpoint_guardar("checkpoint_victoria_nivel1.gxp");
}
    frame = frame + 1 ;
}
    return jugador.puntos ;
}

