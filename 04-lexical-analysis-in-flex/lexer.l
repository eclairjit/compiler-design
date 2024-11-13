{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
FILE *outputFile;
void add_to_symbol_table(const char *lexeme, const
char *type);
int is_keyword(const char *word);
// List of C keywords
const char *keywords[] = {
"auto", "break", "case", "char", "const",
"continue", "default", "do", "double",
"else", "enum", "extern", "float", "for",
"goto", "if", "inline", "int", "long",
"register", "restrict", "return", "short",
"signed", "sizeof", "static", "struct",
"switch", "typedef", "union", "unsigned",
"void", "volatile", "while", NULL
};
%}
%%
#.* { /* Skip preprocessing
directives */ }
\/\/.* { /* Skip single-line comments
*/ }
\/\*([^*]|\*+[^/])*\*+\/ { /* Skip multi-line
comments */ }
\"[^"]*\" { printf("STRING: %s\n",
yytext); fprintf(outputFile, "STRING: %s\n",
yytext); }
[0-9]+ { printf("CONSTANT: %s\n",
yytext); fprintf(outputFile, "CONSTANT: %s\n",
yytext); }
[a-zA-Z_][a-zA-Z_0-9]* {
if (is_keyword(yytext)) {
printf("KEYWORD: %s\n", yytext);
fprintf(outputFile, "KEYWORD: %s\n",
yytext);
} else {
printf("IDENTIFIER: %s\n", yytext);
fprintf(outputFile, "IDENTIFIER: %s\n",
yytext);
add_to_symbol_table(yytext, "IDENTIFIER");
}
}
==|!=|<=|>=|<|>|&&|\|\| { printf("OPERATOR: %s\n",
yytext); fprintf(outputFile, "OPERATOR: %s\n",
yytext); }
[+\-*/%=&|^!<>] { printf("OPERATOR: %s\n",
yytext); fprintf(outputFile, "OPERATOR: %s\n",
yytext); }
[\(\)\{\}\[\];,] { printf("SPECIAL SYMBOL: %s\n",
yytext); fprintf(outputFile, "SPECIAL SYMBOL:
%s\n", yytext); }
[ \t\n]+ { /* Skip whitespace */ }
. { printf("UNKNOWN: %s\n",
yytext); fprintf(outputFile, "UNKNOWN: %s\n",
yytext); }
%%
void add_to_symbol_table(const char *lexeme, const
char *type) {
// Add implementation to maintain and display
the symbol table
// For simplicity, this function is a
placeholder
// Symbol table logic can be implemented here
printf("Adding %s to symbol table as %s\n",
lexeme, type);
}
int is_keyword(const char *word) {
const char **kw = keywords;
while (*kw) {
if (strcmp(word, *kw) == 0) {
return 1;
}
kw++;
}
return 0;
}
int main(int argc, char **argv) {
if (argc != 3) {
fprintf(stderr, "Usage: %s <input_file>
<output_file>\n", argv[0]);
exit(EXIT_FAILURE);
}
FILE *inputFile = fopen(argv[1], "r");
if (!inputFile) {
perror("Error opening input file");
exit(EXIT_FAILURE);
}
outputFile = fopen(argv[2], "w");
if (!outputFile) {
perror("Error opening output file");
fclose(inputFile);
exit(EXIT_FAILURE);
}
yyin = inputFile;
yylex();
fclose(inputFile);
fclose(outputFile);
return 0;
}