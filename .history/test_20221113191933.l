%{
# include "test.tab.h"
%}


%%
"λ"	{ return ADD; }
"-"	{ return SUB; }
"*"     { return MUL; }
"/"     { return DIV; }
"^"     { return EXPO;}
[0-9]+	{ yylval.val = atoi(yytext); return NUMBER; }
([0-9]+([.][0-9]*)?|[.][0-9]+) {yylval.floatval = atof(yytext); return DECIMAL; }

"("     { return LP;}
")"     { return RP;}
[ \t]   { /* ignore white space */ }
.	{ yyerror("unknown character %c\n", *yytext); }
\n      { return EOL; }
%%
