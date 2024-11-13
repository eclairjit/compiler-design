%{
#include <stdio.h>
#include <stdlib.h>

void yyerror(const char* s);
int yylex();
%}

%token NUMBER
%token OR AND NOT
%token LPAREN RPAREN

%left OR AND NOT

%%
E: F {printf("Parsed Successfully\n");}
;
F: F OR F		{printf("%d OR %d = %d\n", $1, $3, $$=$1|$3);}
| F AND F		{printf("%d AND %d = %d\n", $1, $3, $$=$1&$3);}
| NOT F			{printf("NOT %d = %d\n", $2, $$=1-$2);}
| LPAREN F RPAREN	{$$=$2;}
| NUMBER		{$$=$1;}
;
%%

void yyerror(const char* s)
{
fprintf(stderr, "Error: %s\n", s);
}

int main()
{
printf("Enter boolean expression: ");
yyparse();
return 0;
}
