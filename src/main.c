#include <stdio.h>
#include <unistd.h>
#include <getopt.h>

#include "ast.h"
#include "err.h"
#include "interp.h"
#include "scope.h"
#include "parser.tab.h"

extern ast *program; // Defined in lambda.y
extern FILE* yyin;   // Defined in lambda.l
extern int yylex();  // Defined in lambda.l

int main(int argc, char **argv) {
  int opt;
  int ret_val = 0;

  int lex = 0;
  int parse = 0;

  FILE* fp = NULL;
  FILE* scope_graph = NULL;

  int optind = 0;

  /// Define long options
  struct option long_opts[] = {
      {"lex",        no_argument, 0,  'l'},
      {"parse",      no_argument, 0,  'p'},
      {"scope-graph", no_argument, 0,  0},
      {0, 0, 0, 0}
  };

  // Parse flags: -p (parse), -f (lex), --scope-graph (generate scope.dot)
  while ((opt = getopt_long(argc, argv, "lp", long_opts, &optind)) != -1) {
    switch (opt) {
      case 0:
        scope_graph = fopen("scope.dot", "w");
        break;
      case 'l':
        lex = 1;
        break;
      case 'p':
        parse = 1;
        break;
      default:
        fprintf(stderr, "Usage: %s [--scope-graph] [--parse (-p)] [--lex (-l)] [file]\n", argv[0]);
        return 1;
    }
  }

  // Redirect yyin if file is given
  if (!fp && optind < argc) {
    fp = fopen(argv[optind], "r");
    if (fp) yyin = fp;
  } else {
    ERR("Could not find file %s\n", argv[optind]);
    return 1;
  }

  /// Prevent trying to lex and parse at the same time
  if (lex && parse) {
    ERR("Cannot output parse and lex at the same time.\n");
    ret_val = 1;
    goto FIN;
  }

  /// The scope graph can only be generated if interpreting finishes
  if ((lex || parse) && scope_graph) {
    ERR("Cannot only parse/lex and generate scope graph.");
    ret_val = 1;
    goto FIN;
  }

  /// Produce token output
  if (lex) {
    int tok = yylex();
    while (tok != EOF) {
      if (tok == VAR) {
        printf("%s [%c]\n", type_to_str(tok), yylval.id);
      } else {
        printf("%s\n", type_to_str(tok));
      }
      tok = yylex();
    }
    goto FIN; // Cleanup
  }

  /// Produce parser output
  if (parse) {
    int parse_result = yyparse();

    // Parse the input
    if (parse_result == 0) {
      pprint_tree(program, 0);
      del_ast(program);
    } else {
      printf("\nParse failed.\n");
    }

    ret_val = parse_result;

    goto FIN; // Cleanup
  }

  /// Interpret the program
  int parse_result = yyparse();
  if (parse_result == 0) {
    Arena env;
    arena_init(&env, sizeof(scope));

    ast *val = interp(&env, program, NULL);
    if (val) {
      print_node(val, NULL);
      printf("\n");
    } else {
      ERR("Invalid expression.\n"); /// TODO: Throw better errors while interpreting
      ret_val = 2;
    }

    if (scope_graph) {
      print_scope_dfa(&env, scope_graph);
    }

    // Free memory
    del_ast(program);
    arena_free(&env);

  } else {
    ERR("Invalid syntax.\n"); /// TODO: throw better syntax errors
    ret_val = 3;
  }


  FIN: /// Close any open files

  if (fp) {
    fclose(fp);
    fp = NULL;
  }

  if (scope_graph) {
    fclose(scope_graph);
    scope_graph = NULL;
  }

  return ret_val;
}
