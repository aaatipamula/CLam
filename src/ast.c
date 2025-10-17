#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "parser.tab.h" // lsp error if not symlinked
#include "err.h"

/// Convert a token to a string
char *type_to_str(int type) {
  switch (type) {
    case LAMBDA: return "LAMBDA";
    case APP:    return "APP";
    case VAR:    return "VAR";
    case LPAR:   return "LPAR";
    case RPAR:   return "RPAR";
    case DOT:    return "DOT";
  }
  return "unknown";
}

/// Check the status of malloc (private)
static void check_malloc(void *ptr) {
  if (!ptr) ERR("CLam: malloc failed.");
}

// Create a generic ast node (private)
static ast *create_ast_node(int type) {
  ast *node = (ast *)malloc(sizeof(ast));
  check_malloc(node);

  node->id = '_';
  node->fval = node->left = node->right = NULL;
  node->curr_env = NULL;
  node->type = type;

  return node;
}

/// Create a lambda node
ast *create_lambda(char id, ast *body) {
  ast *node = create_ast_node(LAMBDA);

  node->id = id;
  node->left = body;

  return node;
}

/// Create an app node
ast *create_app(ast *left, ast *right) {
  ast *node = create_ast_node(APP);

  node->left = left;
  node->right = right;

  return node;
}

/// Create an identifier node
ast *create_identifier(char id) {
  ast *node = create_ast_node(VAR);

  node->id = id;

  return node;
}

/// Print a single node (and its children)
void print_node(ast *node) {
  if (!node) ERR("No value.");

  switch (node->type) {
    case LAMBDA:
      printf("(\\%c.", node->id);
      print_node(node->left);
      printf(")");
      break;
    case VAR:
      if (node->fval) {
        print_node(node->fval);
      } else {
        printf("%c", node->id);
      }
      break;
    case APP:
      printf("(");
      print_node(node->left);
      printf(" ");
      print_node(node->right);
      printf(")");
      break;
  }
}

/// Print the tree in a pretty way :)
void pprint_tree(ast *node, unsigned int depth) {
  if (!node) ERR("No node.");

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

/// Recursively free all the memory associated with the AST
void del_ast(ast *node) {
  if (!node) return;

  if (node->left) {
    del_ast(node->left);
  }

  if (node->right) {
    del_ast(node->right);
  }

  free(node);
  node = NULL;
}
