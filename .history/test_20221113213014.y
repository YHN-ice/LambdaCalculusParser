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
 | exprlist expression EOL { printLambd($2);aprintf("\n> ");}
 | exprlist EOL { printf("> "); } 
 ;

expression: 
 IDENTIFIER 
 { 
  struct L *n = createName(); 
  n->l.n = $<val>1; $$=n;
  printf("parsing identifier %c\n",n->n);
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
  printf("parsing function with head %c\n",f->f.head);
}
;

application: expression expression 
{
  struct L *ap = createApplication();
  ap->l.ap.func = $1;
  ap->ap.arg = $2;
  printf("parsing apply with heads %c, %c\n",ap->ap.func->f.head, ap->ap.arg->f.head);
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
