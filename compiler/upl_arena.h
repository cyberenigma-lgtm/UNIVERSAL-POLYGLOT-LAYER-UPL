#ifndef UPL_ARENA_H
#define UPL_ARENA_H

#include <stddef.h>
#include <stdint.h>

/**
 * UPL Memory Arena Allocator
 * Proporciona asignación de memoria lineal de alta velocidad y libre de fugas
 * para el analizador léxico, tabla de símbolos y árbol de sintaxis abstracta (AST).
 */

typedef struct UPL_ArenaChunk {
    struct UPL_ArenaChunk *next;
    size_t capacity;
    size_t offset;
    uint8_t data[];
} UPL_ArenaChunk;

typedef struct {
    UPL_ArenaChunk *first;
    UPL_ArenaChunk *current;
    size_t default_chunk_size;
    size_t total_allocated;
} UPL_Arena;

/* Inicializar un Arena de memoria con tamaño de bloque sugerido */
void upl_arena_init(UPL_Arena *arena, size_t default_chunk_size);

/* Asignar un bloque de memoria alineado en el Arena */
void* upl_arena_alloc(UPL_Arena *arena, size_t size);

/* Duplicar una cadena de texto en el Arena con terminador nulo garantizado */
const char* upl_arena_strndup(UPL_Arena *arena, const char *src, size_t len);

/* Duplicar una cadena terminada en nulo */
const char* upl_arena_strdup(UPL_Arena *arena, const char *src);

/* Liberar todos los bloques y devolver la memoria al sistema operativo */
void upl_arena_free(UPL_Arena *arena);

/* Reiniciar el offset de asignación sin liberar los bloques (reutilización rápida) */
void upl_arena_reset(UPL_Arena *arena);

#endif /* UPL_ARENA_H */
