%{
#include <stdio.h>
#include <stdio.h>
#include "lambda.h"
#include "store.h"
%}

//influence the return value in lexer
%union {
  int val;
  struct L* lp;
}
    // declare tokens and their value type.


%token <val>IDENTIFIER SYMBOL 
%token LAMBDA DOT EQ
%token EOL
%token LP RP
%type <lp>expression, function, application
%type <val>assignment





//rules for parser
%%


exprlist: 
 | exprlist assignment EOL { printf("stored symbol %c...\n",$<val>2);} 
 | exprlist expression EOL 
   { 
    printLambda($2);
    printf("parsed initial...\n\n");
    int cnt = 0;
    while($2->type==2){
      if(cnt++==10) break;
      while($2->l.ap.func->type==2){
        apply($2->l.ap.func);
      }
      printLambda($2);
      printf("after outer apply...\n\n",cnt);
      printLambda(apply($2));
      printf("\niteration %d...\n\n",cnt);
    }
    printf("\n> ");
   }
 | exprlist EOL { printf("> "); } 
 ;

assignment: SYMBOL EQ expression
{
    if(capacity == 0) init_symbol_table();
    store_symbol($<val>1, $3);
    $$ = $<val>1;
}
;

expression: 
 SYMBOL 
{ 
    struct L *l=load_symbol_copy($<val>1);
    if(l==NULL){
      printf("undefined symbol:%c\n",$<val>1);
    }
    else{
      $$ = l;
      printf("loading symbol %c...\n",$<val>1);
    }
}
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


