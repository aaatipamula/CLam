#include <stdlib.h>

#include "scope.h"
#include "arena.h"
#include "err.h"

void print_scope_dfa(Arena *a, FILE *out) {
  if (!out) {
    out = stdout;
  }

  fprintf(out, "digraph ScopeTree {\n  rankdir=\"BT\";\n");

  /// Loop through the arena and print each scope node
  for (size_t off = 0; off < a->used; off += a->data_size) {
    scope *s = (scope*)(a->data + off);
    fprintf(out, "  node%p [label=\"%c->", (void *)s, s->id);
    print_node(s->val, out);
    fprintf(out, "\"];\n");
    if (s->parent) {
      fprintf(out, "  node%p -> node%p;\n", (void *)s, (void *)s->parent);
    }
  }

  fprintf(out, "}\n");
}

static void print_scope(scope *s, FILE *out) {
  scope *next = s;

  if (!out) {
    out = stdout;
  }

  while (next) {
    fprintf(out, "%c->", next->id);
    print_node(next->val, out);
    fprintf(out, ";\n");
    next = next->parent;
  }
  printf("\n");
}

ast *lookup_in_scope(scope *curr, char id) {
  scope *next = curr;
  while (next) {
    if (id == next->id) {
      return next->val;
    }
    next = next->parent;
  }

  ERR("[%c] does not exist in scope:\n", id);
  print_scope(curr, NULL);

  return NULL;
}

scope *add_to_scope(Arena *arena, scope *parent, char id, ast *val) {
  scope *new_entry;
  new_entry = arena_alloc(arena);
  if (!new_entry) ERR("CLam: malloc failed.");

  new_entry->id = id;
  new_entry->val = val;
  new_entry->parent = NULL;

  if (parent) {
    new_entry->parent = parent;
  }

  return new_entry;
}

