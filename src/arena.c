#include "arena.h"
#include "string.h"

void arena_init(Arena *a, size_t size) {
  a->data_size = size;
  a->data = malloc(size * 4); /// TODO: Remove arbitrary size expansion
  a->size = size;
  a->used = 0;
}

void arena_free(Arena *a) { free(a->data); a->data = NULL; a->size = a->used = 0; }

void *arena_alloc(Arena *a) {
  if (a->used + a->data_size > a->size) {
    /// Grow the arena
    size_t newsize = (a->size * 2) + a->data_size;
    a->data = realloc(a->data, newsize);
    a->size = newsize;
  }
  void *p = a->data + a->used;
  a->used += (a->data_size + (sizeof(void*)-1)) & ~(sizeof(void*)-1); /// align
  return p;
}

