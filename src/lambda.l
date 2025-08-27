%option noyywrap yylineno

%{
#include "parser.tab.h"
#include <stdio.h>

// extern void yyerror(const char *s);

void err(char *invalid) {
  printf("Invalid character [%s] found.\n", invalid);
}

%}

%%
[\t\r\n\a ] ;
"("   { return LPAR; }
")"   { return RPAR; }
"\\"  { return LAMBDA; }
[.]   { return DOT; }
[a-zA-Z1-9] { yylval.id = yytext[0]; return VAR; }
<<EOF>> { return EOF; }
.     { err(yytext); }
%%

#ifdef DEBUG_FLEX
#include "ast.h"

int main(int argc, char **argv) {
  FILE* fp = NULL;

  // Redirect yyin if file is given
  if (argc > 1) {
    fp = fopen(argv[1], "r");
    if (fp) yyin = fp;
  }

  int tok = yylex();
  while (tok != EOF) {
    if (tok == VAR) {
      printf("%s [%c]\n", type_to_str(tok), yylval.id);
    } else {
      printf("%s\n", type_to_str(tok));
    }
    tok = yylex();
  }

  if (fp != NULL) {
    fclose(fp);
  }

  return 0;
}
#endif
