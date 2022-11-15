%{
# include "test.tab.h"
// #define __DEBUG
%}


%%
"="	{ 
    #ifdef __DEBUG
    printf("readin eq\n");
    #endif
    return LAMBDA; }
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
    #endif
    yylval.val = *yytext; return IDENTIFIER; }

[A-Z]+	{ 
    #ifdef __DEBUG
    printf("readin symbol %c\n", *yytext);
    #endif
    yylval.val = yytext; return IDENTIFIER; }

"("     { 
    #ifdef __DEBUG
    printf("readin lp\n");
    #endif
    return LP;}
")"     { 
    #ifdef __DEBUG
    printf("readin rp\n");
    #endif
    return RP;}
[ \t]   { /* ignore white space */ }
.	{ yyerror("unknown character %c\n", *yytext); }
\n      { 
    #ifdef __DEBUG
    printf("readin EOL\n");
    #endif
    return EOL; }
%%
