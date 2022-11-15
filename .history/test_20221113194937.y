%{
#include <stdio.h>
#include <stdio.h>
#include "lambda.h"
%}

//influence the return value in lexer
%union {
  int val;
}
    // declare tokens and their value type.


%token <val> IDENTIFIER
%token LAMBDA DOT
%token EOL
%token LP RP




//rules for parser
%%

expression: 
 | IDENTIFIER 
 { 
  Lambda *n = new Lambda(); n.n = $<val>1; $$=n;}
 | function { $$ = $1; } 
 | application { $$ = $1; } 
 ;


function: LAMBDA IDENTIFIER DOT expression{ Lambda *f = new Lambda(); f.f.head=$<val>2; f.f.body = $4; $$ = f;}
;

factor: atom
|factor EXPO factor{$$=pow($1,$3);}
atom : NUMBER{$$=(double)$<val>1;}
 |DECIMAL
 | LP exp RP{$$=$2;}
 ;


%%
main()
{
  printf("> "); 
  yyparse();
}

yyerror(char *s)
{
  fprintf(stderr, "error: %s\n", s);
}
