%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int yylineno;
extern int column;
extern char* yytext;
extern FILE* yyin;

void yyerror(const char* msg);
int yylex(void);

int has_errors = 0;
%}

%union {
    int num;
    char* string;
}

// Токены из лексера
%token CLASS
%token INT
%token RETURN
%token PUBLIC
%token PRIVATE
%token STATIC
%token VOID
%token ERROR

// Токены с типами
%token <num> INT_LITERAL
%token <string> IDENTIFIER

// Приоритеты операторов
%left '+' '-'
%left '*' '/'
%nonassoc UMINUS

%start program

%%

program:
    /* empty */
    | program class_declaration
    ;

class_declaration:
    class_modifiers CLASS IDENTIFIER class_body
    {
        printf("Valid class: %s\n", $3);
        free($3);
    }
    ;

class_modifiers:
    /* empty */
    | class_modifiers PUBLIC
    | class_modifiers PRIVATE
    | class_modifiers STATIC
    ;

class_body:
    '{' class_members '}'
    ;

class_members:
    /* empty */
    | class_members class_member
    ;

class_member:
    field_declaration
    | method_declaration
    ;

field_declaration:
    type IDENTIFIER ';'
    {
        printf("  Field: %s\n", $2);
        free($2);
    }
    ;

method_declaration:
    type IDENTIFIER '(' parameter_list ')' method_body
    {
        printf("  Method: %s\n", $2);
        free($2);
    }
    | method_modifiers type IDENTIFIER '(' parameter_list ')' method_body
    {
        printf("  Method: %s\n", $3);
        free($3);
    }
    ;

method_modifiers:
    PUBLIC
    | PRIVATE
    | STATIC
    | method_modifiers PUBLIC
    | method_modifiers PRIVATE
    | method_modifiers STATIC
    ;

type:
    INT
    | VOID
    | IDENTIFIER { free($1); }
    ;

parameter_list:
    /* empty */
    | parameter
    | parameter_list ',' parameter
    ;

parameter:
    type IDENTIFIER { free($2); }
    ;

method_body:
    ';'
    | block
    ;

block:
    '{' statements '}'
    ;

statements:
    /* empty */
    | statements statement
    ;

statement:
    declaration_statement
    | assignment_statement
    | return_statement
    | block
    ;

declaration_statement:
    type IDENTIFIER ';'
    {
        printf("    Variable declaration: %s\n", $2);
        free($2);
    }
    | type IDENTIFIER '=' expression ';'
    {
        printf("    Variable initialization: %s = ...\n", $2);
        free($2);
    }
    ;

assignment_statement:
    IDENTIFIER '=' expression ';'
    {
        printf("    Assignment: %s = ...\n", $1);
        free($1);
    }
    ;

return_statement:
    RETURN ';'
    {
        printf("    Return (void)\n");
    }
    | RETURN expression ';'
    {
        printf("    Return value\n");
    }
    ;

expression:
    INT_LITERAL
    | IDENTIFIER { free($1); }
    | expression '+' expression
    | expression '-' expression
    | expression '*' expression
    | expression '/' expression
    | '(' expression ')'
    | '-' expression %prec UMINUS
    ;

%%

void yyerror(const char* msg) {
    fprintf(stderr, "Syntax error at line %d, column %d: %s (token: '%s')\n", 
            yylineno, column, msg, yytext);
    has_errors = 1;
}