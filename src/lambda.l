%option noyywrap yylineno
%option noinput nounput

%{

#include <stdio.h>
#include "err.h"
#include "parser.tab.h"

%}

%%

[\t\r\n\a ] ;
"("   { return LPAR; }
")"   { return RPAR; }
"\\"  { return LAMBDA; }
[.]   { return DOT; }
[a-zA-Z1-9] { yylval.id = yytext[0]; return VAR; }
<<EOF>> { return EOF; }
.     { ERR("Invalid character [%s] found.", yytext); }

%%

