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
 | name { $$ = $1;}
 | function { $$ = $1; } 
 | application { $$ = $1; } 
 ;


name: IDENTIFIER { Lambda *n = new Lambda()$<val>1; }
 ;

term: factor
|term MUL factor {$$=$1*$3;}
| term DIV factor {$$=$1/$3;}
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
