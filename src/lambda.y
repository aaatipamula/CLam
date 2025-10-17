%define parse.error detailed

%{
#include <stdio.h>
#include "ast.h"

extern int yylex();
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
%token CLOSURE /* This is an ephemeral token which only appears when interpreting */
%token <token> LAMBDA
%token <id> VAR

%left <token> APP

%type <node> expr app atom

%start prog

%%
prog : expr                { program = $1; }
     ;

expr : LAMBDA VAR DOT expr { $$ = create_lambda($2, $4); }
     | app                 { $$ = $1; }
     ;

app : app atom             { $$ = create_app($1, $2); }
    | atom                 { $$ = $1; } 
    ;

atom : VAR                 { $$ = create_identifier($1); }
     | LPAR expr RPAR      { $$ = $2; }
     ;
%%

