#ifndef LAMBDA_AST_H
#define LAMBDA_AST_H

typedef struct _lambda_ast ast;

struct _lambda_ast {
  int type;
  char id;
  ast *left;
  ast *right;
};

// TODO: Add arenas
ast *create_lambda(char id, ast *body);
ast *create_app(ast *left, ast *right);
ast *create_identifier(char id);

char *type_to_str(int type);

void pprint_tree(ast *term, unsigned int depth);
void del_ast(ast *node);

#endif
