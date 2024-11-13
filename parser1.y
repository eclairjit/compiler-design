%{
#include <stdio.h>
#include <stdlib.h>

void yyerror(const char* s);
int yylex();
%}

%token NUMBER
%token PLUS MINUS MULT DIV
%token LPAREN RPAREN

%%
E: F {printf("Parsed Successfully\n");}
;
F: F PLUS F		{printf("%d + %d = %d\n", $1, $3, $$=$1+$3);}
| F MINUS F		{printf("%d - %d = %d\n", $1, $3, $$=$1-$3);}
| F MULT F		{printf("%d * %d = %d\n", $1, $3, $$=$1*$3);}
| F DIV F		{if($3 == 0) {yyerror("Division by zero.\n");} else {printf("%d / %d = %d\n", $1, $3, $$=$1/$3);}}
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
printf("Enter infix expression: ");
yyparse();
return 0;
}
