%{
# include "test.tab.h"
%}


%%
"$"	{ printf("readin lambda\n");return LAMBDA; }
"."	{ return DOT; }
[a-z]+	{ printf("readin identifier %c\n", yytext);yylval.val = *yytext; return IDENTIFIER; }

"("     { return LP;}
")"     { return RP;}
[ \t]   { /* ignore white space */ }
.	{ yyerror("unknown character %c\n", *yytext); }
\n      { return EOL; }
%%
