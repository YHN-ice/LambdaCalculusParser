%{
#include <stdio.h>
#include <stdio.h>
#include "lambda.h"
%}

//influence the return value in lexer
%union {
  int val;
  struct L* lp;
}
    // declare tokens and their value type.


%token <val> IDENTIFIER
%token LAMBDA DOT
%token EOL
%token LP RP
%type <lp>expression, function, application





//rules for parser
%%


exprlist: 
 | exprlist expression EOL 
   { 
    printLambda($2);
    printf("applying")
    if($2->type==2)
      printLambda(apply($2));
    printf("\n> ");
   }
 | exprlist EOL { printf("> "); } 
 ;

expression: 
 IDENTIFIER 
 { 
  struct L *n = createName(); 
  n->l.n = $<val>1; $$=n;
  printf("parsing identifier %c\n",n->l.n);
  }
 | function { $$ = $1; } 
 | application { $$ = $1; } 
 | LP expression RP {$$ = $2;}
 ;

optional_EOL:
|EOL;

function: LAMBDA IDENTIFIER DOT expression
{ 
  struct L *f = createFunction(); 
f->l.f.head=$<val>2; 
f->l.f.body = $4; 
$$ = f;
  printf("parsing function with head %c\n",f->l.f.head);
}
;

application: expression expression 
{
  struct L *ap = createApplication();
  ap->l.ap.func = $1;
  ap->l.ap.arg = $2;
  printf("parsing apply with heads %c, %c\n",ap->l.ap.func->l.f.head, ap->l.ap.arg->l.f.head);
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


