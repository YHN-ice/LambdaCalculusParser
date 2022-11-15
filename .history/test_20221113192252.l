%{
# include "test.tab.h"
%}


%%
"λ"	{ return LAMBDA; }
"("     { return LP;}
")"     { return RP;}
[ \t]   { /* ignore white space */ }
.	{ yyerror("unknown character %c\n", *yytext); }
\n      { return EOL; }
%%
