#include "upl_arena.h"
#include <stdlib.h>
#include <string.h>

#define UPL_ARENA_ALIGNMENT 16
#define UPL_ALIGN_UP(size) (((size) + (UPL_ARENA_ALIGNMENT - 1)) & ~(UPL_ARENA_ALIGNMENT - 1))

static UPL_ArenaChunk* upl_arena_new_chunk(size_t capacity) {
    size_t total_size = sizeof(UPL_ArenaChunk) + capacity;
    UPL_ArenaChunk *chunk = (UPL_ArenaChunk*)malloc(total_size);
    if (!chunk) return NULL;
    chunk->next = NULL;
    chunk->capacity = capacity;
    chunk->offset = 0;
    return chunk;
}

void upl_arena_init(UPL_Arena *arena, size_t default_chunk_size) {
    if (!arena) return;
    if (default_chunk_size < 4096) default_chunk_size = 65536; // 64 KB por defecto
    arena->default_chunk_size = default_chunk_size;
    arena->first = upl_arena_new_chunk(default_chunk_size);
    arena->current = arena->first;
    arena->total_allocated = arena->first ? default_chunk_size : 0;
}

void* upl_arena_alloc(UPL_Arena *arena, size_t size) {
    if (!arena || size == 0) return NULL;

    size_t aligned_size = UPL_ALIGN_UP(size);

    if (!arena->current) {
        arena->first = upl_arena_new_chunk(arena->default_chunk_size);
        arena->current = arena->first;
    }

    // Si el bloque actual no tiene suficiente espacio
    if (arena->current->offset + aligned_size > arena->current->capacity) {
        size_t next_cap = arena->default_chunk_size;
        if (aligned_size > next_cap) {
            next_cap = aligned_size + 4096; // Chunk grande dedicado para objetos masivos
        }
        UPL_ArenaChunk *new_chunk = upl_arena_new_chunk(next_cap);
        if (!new_chunk) return NULL;
        arena->current->next = new_chunk;
        arena->current = new_chunk;
        arena->total_allocated += next_cap;
    }

    void *ptr = arena->current->data + arena->current->offset;
    arena->current->offset += aligned_size;
    return ptr;
}

const char* upl_arena_strndup(UPL_Arena *arena, const char *src, size_t len) {
    if (!arena || !src) return "";
    char *dest = (char*)upl_arena_alloc(arena, len + 1);
    if (!dest) return "";
    memcpy(dest, src, len);
    dest[len] = '\0';
    return dest;
}

const char* upl_arena_strdup(UPL_Arena *arena, const char *src) {
    if (!src) return "";
    return upl_arena_strndup(arena, src, strlen(src));
}

void upl_arena_free(UPL_Arena *arena) {
    if (!arena) return;
    UPL_ArenaChunk *curr = arena->first;
    while (curr) {
        UPL_ArenaChunk *next = curr->next;
        free(curr);
        curr = next;
    }
    arena->first = NULL;
    arena->current = NULL;
    arena->total_allocated = 0;
}

void upl_arena_reset(UPL_Arena *arena) {
    if (!arena) return;
    UPL_ArenaChunk *curr = arena->first;
    while (curr) {
        curr->offset = 0;
        curr = curr->next;
    }
    arena->current = arena->first;
}
