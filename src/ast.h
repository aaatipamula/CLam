#ifndef LAMBDA_AST_H_
#define LAMBDA_AST_H_

typedef struct _lambda_ast ast;
typedef struct _lambda_scope scope; // Forward declaration of struct in "src/scope.h"

struct _lambda_ast {
  int type;
  char id;
  ast *left;
  ast *right;
  ast *fval;
  scope *curr_env;
};

char *type_to_str(int type);

ast *create_lambda(char id, ast *body);
ast *create_app(ast *left, ast *right);
ast *create_identifier(char id);

void print_node(ast *node);
void pprint_tree(ast *term, unsigned int depth);
void del_ast(ast *node);

#endif // LAMBDA_AST_H_
