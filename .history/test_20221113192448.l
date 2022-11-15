%{
# include "test.tab.h"
%}


%%
"λ"	{ return LAMBDA; }
"."	{ return DOT; }
[a-z]+	{ yylval.val = atoi(yytext); return IDENTIFIER; }

"("     { return LP;}
")"     { return RP;}
[ \t]   { /* ignore white space */ }
.	{ yyerror("unknown character %c\n", *yytext); }
\n      { return EOL; }
%%