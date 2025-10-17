#ifndef LAMBDA_INTERP_H_
#define LAMBDA_INTERP_H_

#include "ast.h"
#include "scope.h"

ast *interp(Arena *env, ast *node, scope *curr_scope);

#endif // LAMBDA_INTERP_H_
