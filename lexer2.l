%{
#include "y.tab.h"
%}

%%
[0-1]+ {yylval=atoi(yytext); return NUMBER;}

[ \t\n]+ {}

"NOT"	{return NOT;}
"OR"	{return OR;}
"AND"	{return AND;}
"("	{return LPAREN;}
")"	{return RPAREN;}

.	{printf("Invalid character: %s\n", yytext);}
%%

int yywrap()
{
return 1;
}
