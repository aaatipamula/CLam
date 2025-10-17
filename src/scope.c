#include <stdio.h>
#include <stdlib.h>

#include "scope.h"
#include "arena.h"
#include "err.h"


static void print_scope(scope *s) {
  scope *next = s;
  while (next) {
    printf("%c->", next->id);
    print_node(next->val);
    printf(";\n");
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
  print_scope(curr);

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

