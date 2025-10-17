#include <stdlib.h>

#include "parser.tab.h"
#include "interp.h"
#include "arena.h"
#include "err.h"

/// Run the interpreter
ast *interp(Arena *env, ast *node, scope *curr_scope) {
  ast *left_val, *right_val, *final_val;

  if (!node) return NULL;

  switch (node->type) {
    case APP:
      /// Evaluate the left and right branches in the current context
      left_val = interp(env, node->left, curr_scope);
      right_val = interp(env, node->right, curr_scope);

      if (left_val->type != CLOSURE) {
        ERR("Invalid operator to application");
        return NULL;
      }

      /// and then the body of the closure within its own environment
      curr_scope = left_val->curr_env;
      curr_scope = add_to_scope(env, curr_scope, left_val->id, right_val);
      final_val = interp(env, left_val->left, curr_scope);

      return final_val;

    case LAMBDA:
      /// Add the current environment to the lambda to create a closure
      node->curr_env = curr_scope;
      node->type = CLOSURE;
      return node;

    case VAR:
      final_val = lookup_in_scope(curr_scope, node->id);
      node->fval = final_val;

      return final_val;
  }

  return node;
}
