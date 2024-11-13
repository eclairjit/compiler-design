%{
#include "y.tab.h"
%}

%%
[0-9]+ {yylval=atoi(yytext); return NUMBER;}

[ \t\n]+ {}

"+"	{return PLUS;}
"-"	{return MINUS;}
"*"	{return MULT;}
"/"	{return DIV;}
"("	{return LPAREN;}
")"	{return RPAREN;}

.	{printf("Invalid character: %s\n", yytext);}
%%

int yywrap()
{
return 1;
}
