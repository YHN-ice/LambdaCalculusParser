%{
#include <cstdlib>
#include "Parser1.hpp"
using namespace lambda;
%}
 
%option reentrant interactive noyywrap noyylineno nodefault outfile="Scanner1.cpp" header="Scanner1.hpp"
 
%%
 
"="	    { 
            #ifdef __DEBUG
            printf("readin eq\n");
            #endif
            return Parser::token::EQ; 
        }
"$"	    { 
            #ifdef __DEBUG
            printf("readin lambda\n");
            #endif
            return Parser::token::LAMBDA; 
        }
"."	    {
            #ifdef __DEBUG
            printf("readin dot\n");
            #endif
            return Parser::token::DOT; 
        }
[a-z]+	{ 
            #ifdef __DEBUG
            printf("readin identifier %c\n", *yytext);
            #endif
            yylval->emplace<std::string>(yytext); return Parser::token::IDENTIFIER; 
        }

[A-Z]+	{ 
            #ifdef __DEBUG
            printf("readin symbol %c\n", *yytext);
            #endif
            yylval->emplace<std::string>(yytext); return Parser::token::SYMBOL; 
        }

"("     { 
            #ifdef __DEBUG
            printf("readin lp\n");
            #endif
            return Parser::token::LPAREN;
        }
")"     { 
            #ifdef __DEBUG
            printf("readin rp\n");
            #endif
            return Parser::token::RPAREN;
        }
[ \t]   { /* ignore white space */ }

.	    {   printf("unknown character %s\n", yytext); }
\n      { 
            #ifdef __DEBUG
            printf("readin EOL\n");
            #endif
            return Parser::token::EOL; 
        }
 
%%
 