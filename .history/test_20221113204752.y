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
 | IDENTIFIER optional_EOL
 { 
  union Lambda *n = createLambda(); 
  n->n = $<val>1; $$=n;
  printf("parsing identifier %c\n",n->n.val);
  }
 | function optional_EOL{ $$ = $1; } 
 | application optional_EOL{ $$ = $1; } 
 | LP expression RP optional_EOL{$$ = $2;}
 ;

optional_EOL:
|EOL;

function: LAMBDA IDENTIFIER DOT expression
{ 
  union Lambda *f = createLambda(); 
f->f.head=$<val>2; 
f->f.body = $4; 
$$ = f;
  printf("parsing function with head %c\n",f->f.head);
}
;

application: expression expression 
{
  union Lambda *ap = createLambda();
  ap->ap.func = $1;
  ap->ap.arg = $2;
  printf("parsing apply with heads %c, %c\n",ap->ap.func->f.head, ap->ap.ap.arg);
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
