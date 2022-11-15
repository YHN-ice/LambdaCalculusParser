%{
# include "test.tab.h"
%}


%%
"λ"	{ return LAMBDA; }
"-"	{ return SUB; }
"*"     { return MUL; }
"/"     { return DIV; }
"^"     { return EXPO;}
[0-9]+	{ yylval.val = atoi(yytext); return NUMBER; }

"("     { return LP;}
")"     { return RP;}
[ \t]   { /* ignore white space */ }
.	{ yyerror("unknown character %c\n", *yytext); }
\n      { return EOL; }
%%
