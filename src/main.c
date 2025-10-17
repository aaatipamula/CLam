#include <stdio.h>
#include <unistd.h>

#include "ast.h"
#include "err.h"
#include "interp.h"
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

  // Parse flags: -p (parse), -f (lex)
  while ((opt = getopt(argc, argv, "lp")) != -1) {
    switch (opt) {
      case 'l':
        lex = 1;
        break;
      case 'p':
        parse = 1;
        break;
      default:
        fprintf(stderr, "Usage: %s [-p] [-l] [file]\n", argv[0]);
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
    return 1;
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
    goto FIN; // PLEASE DON'T DO THIS ACTUALLY
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

    goto FIN; // THIS IS NOT GOOD CODE DO NOT DO THIS
  }

  /// Interpret the program
  int parse_result = yyparse();
  if (parse_result == 0) {
    Arena env;
    arena_init(&env, sizeof(scope) * 4); // TODO: Find a better way to estimate the initial size of arena

    ast *val = interp(&env, program, NULL);
    print_node(val);
    printf("\n");

    // Free memory
    del_ast(program);
    arena_free(&env);

  } else {
    ERR("Invalid expression.\n");
  }

  FIN: /// DON'T DO THIS LMAO

  /// Close any open files
  if (fp) {
    fclose(fp);
    fp = NULL;
  }

  return ret_val;
}
