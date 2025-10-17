#ifndef _LAMBDA_ARENA_H
#define  _LAMBDA_ARENA_H

#include <stdlib.h>

typedef struct Arena {
    char *data;
    size_t size;
    size_t used;
} Arena;

void arena_init(Arena *a, size_t size);
void *arena_alloc(Arena *a, size_t n);
void arena_free(Arena *a);

#endif  // _LAMBDA_ARENA_H

