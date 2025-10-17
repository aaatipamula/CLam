#ifndef LAMBDA_SCOPE_H_
#define LAMBDA_SCOPE_H_

#include "ast.h"
#include "arena.h"

typedef struct _lambda_scope {
  char id;
  ast *val;
  struct _lambda_scope *parent;
} scope;

scope *add_to_scope(Arena *arena, scope *parent, char id, ast *val);
ast *lookup_in_scope(scope *curr, char id);

#endif // LAMBDA_SCOPE_H_
