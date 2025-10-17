#include "arena.h"
#include "string.h"

void arena_init(Arena *a, size_t size) {
    a->data = malloc(size);
    a->size = size;
    a->used = 0;
}

void arena_free(Arena *a) { free(a->data); a->data = NULL; a->size = a->used = 0; }

void *arena_alloc(Arena *a, size_t n) {
    if (a->used + n > a->size) {
        /* grow */
        size_t newsize = (a->size * 2) + n;
        a->data = realloc(a->data, newsize);
        a->size = newsize;
    }
    void *p = a->data + a->used;
    a->used += (n + (sizeof(void*)-1)) & ~(sizeof(void*)-1); /* align */
    return p;
}

static char *arena_strdup(Arena *a, const char *s) {
    size_t n = strlen(s) + 1;
    char *d = arena_alloc(a, n);
    memcpy(d, s, n);
    return d;
}

