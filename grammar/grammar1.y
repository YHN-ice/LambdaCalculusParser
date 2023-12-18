%{
#include <stdio.h>
#include <stdlib.h>
#include "Scanner1.hpp"
%}

%code requires{
#include "lambda.hpp"
#include "store.hpp"

}

%require "3.7.4"
%language "C++"
%defines "Parser1.hpp"
%output "Parser1.cpp"
 
%define api.parser.class {Parser}
%define api.namespace {lambda}
%define api.value.type variant
%param {yyscan_t scanner}
 
%code provides
{
    #define YY_DECL \
        int yylex(lambda::Parser::semantic_type *yylval, yyscan_t yyscanner)
    YY_DECL;
}
 
%token                      EOL LPAREN RPAREN 
%token                      LAMBDA DOT EQ
%token <std::string>        IDENTIFIER SYMBOL
 
%nterm <std::string>        assignment
%nterm <L>             expression_r expression_l expression function application 
 
%code
{
    namespace lambda {
    } // namespace lambda
} // %code
 
%%
exprlist    : %empty 
            | exprlist assignment EOL 
                {
#ifdef STANDALONE
                    printf("stored symbol %s...\n\n\n> ", $2.c_str());
#endif
                } 
            | exprlist expression EOL 
                { 
#ifdef STANDALONE
                    printf("\n****************\n");
                    $2.dump();
                    printf("\nreduce...\n");
                    L res = $2.apply();
                    int res_i = L::l2i(res);
                    printf("\nresult...\n");
                    res.dump();
                    printf("\nephemeral result cleanup...\n");
                    res.cleanup();
                    printf("\nephemeral function/application cleanup...\n");
                    $2.cleanup();
                    printf("\n%d\n++++++++++++++++\n",res_i);
                    printf("\n> ");
#endif
                }
            | exprlist EOL 
                { 
#ifdef STANDALONE
                    print_table();
                    printf("table size %d\n", get_table_size());
                    printf("> ");
#endif
                } 
            ;

assignment  : SYMBOL EQ expression
                {
                    int success = store_symbol($1, $3);
#ifndef PROD
                    printf("%s ASSIGNMENT %d\n", $1.c_str(),success);
#endif
                    $$ = $1;
                }
            ;

expression  : expression_r
            | expression_l 
            ;

expression_r: SYMBOL 
                { 
                    if(!has_symbol($1)){
#ifndef PROD
                        printf("undefined symbol:%s\n",$1.c_str());
#endif
                    } else {
                        $$ = load_symbol($1);
#ifndef PROD
                        printf("loading symbol %s...\n",$1.c_str());
#endif
                    }
                }
            | IDENTIFIER 
                { 
#ifndef PROD
                    printf("parsing identifier %s\n",$1.c_str());
#endif
                    $$=L($1);
                }
            | function
            | LPAREN expression RPAREN {$$ = $2;}
            ;

expression_l: application;

function    : LAMBDA IDENTIFIER DOT expression
                { 
                        $$ = L($2, KEY($4));
#ifndef PROD
                    printf("parsing function with head %s\n", $2.c_str());
#endif
                }
            ;

application : expression expression_r
                {
#ifndef PROD
                    printf("parsing apply with heads %s and tail, %s\n", $1.key.c_str(), $2.key.c_str());
#endif
                    $$ = L(KEY($1),KEY($2),false);
                }
            ;
%%
 
void lambda::Parser::error(const std::string& msg) {
    std::cerr << msg << '\n';
}
#ifdef STANDALONE
int main() {
    yyscan_t scanner;
    yylex_init(&scanner);
    lambda::Parser parser( scanner );
    std::cout.precision(10);
    parser.parse();
    yylex_destroy(scanner);
}
#else
void read_in_file(const char *filename){
    FILE *in;
    std::string input_dir = "input/";
    std::string input_filename = std::string(filename);
    std::string path = input_dir+input_filename;
    in = fopen(path.c_str(), "r");

    yyscan_t scanner;
    yylex_init(&scanner);
    yyset_in(in, scanner);

    lambda::Parser parser( scanner );
    std::cout.precision(10);
    parser.parse();
    yylex_destroy(scanner);
    fclose(in);
}
#endif
