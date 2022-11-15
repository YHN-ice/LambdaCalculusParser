%{
#include <stdio.h>
#include <stdio.h>
#include "lambda.h"
%}

//influence the return value in lexer
%union {
  int val;
  union Lambda* lp;
}
    // declare tokens and their value type.


%token <val> IDENTIFIER
%token LAMBDA DOT
%token EOL
%token LP RP
%type <lp>expression, function, application





//rules for parser
%%

expression: 
 | IDENTIFIER EOL
 { 
  union Lambda *n = createLambda(); 
  n->n = $<val>1; $$=n;
  }
 | function EOL{ $$ = $1; } 
 | application EOL{ $$ = $1; } 
 | LP expression RP EOL{$$ = $2;}
 ;

optional_EOL:
|EOL;

function: LAMBDA IDENTIFIER DOT expression
{ 
  union Lambda *f = createLambda(); 
f->f.head=$<val>2; 
f->f.body = $4; 
$$ = f;}
;

application: expression expression 
{
  union Lambda *ap = createLambda();
  ap->ap.func = $1;
  ap->ap.arg = $2;
  $$ = ap;
}
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