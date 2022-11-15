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
    #endif
     return DOT; }
[a-z]+	{ 
    #ifdef __DEBUG
    printf("readin identifier %c\n", *yytext);
    yylval.val = *yytext; return IDENTIFIER; }

"("     { 
    #ifdef __DEBUG
    printf("readin lp\n");
    return LP;}
")"     { 
    #ifdef __DEBUG
    printf("readin rp\n");
    return RP;}
[ \t]   { /* ignore white space */ }
.	{ yyerror("unknown character %c\n", *yytext); }
\n      { 
    #ifdef __DEBUG
    printf("readin EOL\n");
    return EOL; }
%%
