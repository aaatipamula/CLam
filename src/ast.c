#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "parser.tab.h" // lsp error lol

char *type_to_str(int type) {
  switch (type) {
    case LAMBDA: return "LAMBDA";
    case APP: return "APP";
    case VAR: return "VAR";
    case LPAR: return "LPAR";
    case RPAR: return "RPAR";
    case DOT: return "DOT";
  }
  return "unknown";
}

void check_malloc(void *ptr) {
  if (!ptr) fprintf(stderr, "CLam: malloc failed.");
}

ast *create_ast_node(int type) {
  ast *node = (ast *)malloc(sizeof(ast));
  check_malloc(node);

  node->id = '_';
  node->left = node->right = NULL;
  node->type = type;

  return node;
}

ast *create_lambda(char id, ast *body) {
  ast *node = create_ast_node(LAMBDA);

  node->id = id;
  node->left = body;

  return node;
}

ast *create_app(ast *left, ast *right) {
  ast *node = create_ast_node(APP);

  node->left = left;
  node->right = right;

  return node;
}
ast *create_identifier(char id) {
  ast *node = create_ast_node(VAR);

  node->id = id;

  return node;
}

void pprint_tree(ast *node, unsigned int depth) {
  if (node->type == LAMBDA || node->type == VAR) {
    printf("%*s%s [%c]\n", depth*2, "", type_to_str(node->type), node->id);
  } else {
    printf("%*s%s\n", depth*2, "", type_to_str(node->type));
  }

  if (node->left) {
    pprint_tree(node->left, depth + 1);
  }

  if (node->right) {
    pprint_tree(node->right, depth + 1);
  }
}

void del_ast(ast *node) {
  if (node->left) {
    del_ast(node->left);
  }

  if (node->right) {
    del_ast(node->right);
  }

  free(node);
  node = NULL;
}
