%define parse.error detailed

%{
#include <stdio.h>
#include "ast.h"


extern int yylex();
extern int yyparse();
extern FILE *yyin;
// TODO: More robust error messages
void yyerror(const char *s) { fprintf(stderr, "ERROR: %s\n", s); }

ast* program;
%}

%union {
  struct _lambda_ast *node;
  int token;
  char id;
}

%token LPAR RPAR DOT
%token <token> LAMBDA
%token <id> VAR

%left <token> APP

%type <node> expr app atom

%start prog

%%
prog : expr                { program = $1; }
     ;

expr : LAMBDA VAR DOT expr { $$ = create_lambda($2, $4); }
     | app
     ;

app : app atom             { $$ = create_app($1, $2); }
    | atom
    ;

atom : VAR                 { $$ = create_identifier($1); }
     | LPAR expr RPAR      { $$ = $2; }
     ;
%%

#ifdef DEBUG_BISON
int main(int argc, char **argv) {

  FILE* fp = NULL;
  extern FILE* yyin;

  // Redirect yyin if file is given
  if (argc > 1) {
    fp = fopen(argv[1], "r");
    if (fp) yyin = fp;
  }

  int parse_result = yyparse();

  // Parse the input
  if (parse_result == 0) {
    printf("\nProgram:\n");
    pprint_tree(program, 0);
    del_ast(program);
  } else {
    printf("\nParse failed.\n");
  }

  if (fp != NULL) {
    fclose(fp);
  }

  return parse_result;
}
#endif
