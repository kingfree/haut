%{
#include <stdio.h>
#include "y.tab.h"

int yywrap(void)
{
    return 1;
}
%}
%%
"+"     return ADD;
"-"     return SUB;
"*"     return MUL;
"/"     return DIV;
"\n"    return CR;
([1-9][0-9]*)|0|([0-9]+\.[0-9]*) {
    double x;
    sscanf(yytext, "%lf", &x);
    yylval.double_value = x;
    return DOUBLE_LITERAL;
}
[ \t] ;
. {
    fprintf(stderr, "表达式错误。\n");
    exit(1);
}
%%

