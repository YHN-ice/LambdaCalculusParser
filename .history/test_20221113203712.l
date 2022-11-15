%{
# include "test.tab.h"
#define __DEBUG
%}


%%
"$"	{ 
    #ifdef __DEBUG
    printf("readin lambda\n");
    #endif
    return LAMBDA; }
"."	{
    #ifdef __DEBUG
    printf("readin dot\n");
     return DOT; }
[a-z]+	{ 
    printf("readin identifier %c\n", *yytext);
    yylval.val = *yytext; return IDENTIFIER; }

"("     { 
    printf("readin lp\n");
    return LP;}
")"     { 
    printf("readin rp\n");
    return RP;}
[ \t]   { /* ignore white space */ }
.	{ yyerror("unknown character %c\n", *yytext); }
\n      { 
    printf("readin EOL\n");
    return EOL; }
%%
