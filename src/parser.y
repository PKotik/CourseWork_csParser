%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int line_num;
extern int column_num;
extern char* yytext;
extern FILE* yyin;

void yyerror(const char* msg);
int yylex(void);

int has_errors = 0;

typedef struct Symbol {
    char* name;
    char* type;
    struct Symbol* next;
} Symbol;

Symbol* symbol_table = NULL;
char* current_method_type = NULL;
char* current_method_name = NULL;
int current_method_has_return = 0;

Symbol* find_symbol(const char* name) {
    Symbol* sym = symbol_table;
    while (sym) {
        if (strcmp(sym->name, name) == 0) {
            return sym;
        }
        sym = sym->next;
    }
    return NULL;
}

void add_symbol(const char* name, const char* type) {
    Symbol* sym = find_symbol(name);
    if (sym) {
        free(sym->type);
        sym->type = strdup(type);
        return;
    }
    sym = malloc(sizeof(Symbol));
    sym->name = strdup(name);
    sym->type = strdup(type);
    sym->next = symbol_table;
    symbol_table = sym;
}

void clear_symbols() {
    while (symbol_table) {
        Symbol* next = symbol_table->next;
        free(symbol_table->name);
        free(symbol_table->type);
        free(symbol_table);
        symbol_table = next;
    }
}

int is_type_compatible(const char* target, const char* source) {
    if (strcmp(target, source) == 0) return 1;
    if (strcmp(target, "bool") == 0 && strcmp(source, "int") == 0) return 1;
    return 0;
}

const char* get_type_name(const char* type) {
    if (strcmp(type, "int") == 0) return "int";
    if (strcmp(type, "string") == 0) return "string";
    if (strcmp(type, "void") == 0) return "void";
    if (strcmp(type, "bool") == 0) return "bool";
    if (strcmp(type, "object") == 0) return "object";
    return type;
}
%}

%union {
    int num;
    char* string;
    struct ExprInfo {
        char* type;
    } expr;
}

// Базовые токены
%token <num> INT_LITERAL
%token <string> IDENTIFIER

// Ключевые слова C#
%token ABSTRACT ASYNC AWAIT BOOL BREAK CASE CATCH CLASS CONTINUE DEFAULT
%token ELSE FALSE FINALLY FOR FOREACH GET IF INT INTERNAL NAMESPACE NEW
%token NULL_ OBJECT OVERRIDE PRIVATE PROTECTED PUBLIC RETURN SEALED SET
%token STATIC STRING SWITCH THROW TRUE TRY USING VAR VIRTUAL VOID WHILE

// Операторы
%token EQ_EQ NE LE GE AND_AND OR_OR ARROW NULL_COALESCE_ASSIGN NULL_COALESCE
%token ERROR

%type <expr> expression
%type <string> type

%left AND_AND OR_OR
%left '<' '>' LE GE EQ_EQ NE
%right '!'
%left '+' '-'
%left '*' '/' '%'
%nonassoc UMINUS

%start program

%%

qualified_identifier:
    IDENTIFIER
    | qualified_identifier '.' IDENTIFIER
    ;

program:
    /* empty */
    | program using_directive
    | program namespace_declaration
    | program class_declaration
    ;

using_directive:
    USING qualified_identifier ';'
    {
        printf("Using directive\n");
    }
    | USING IDENTIFIER '=' qualified_identifier ';'
    {
        printf("Using alias: %s = ...\n", $2);
        free($2);
    }
    ;

namespace_declaration:
    block_namespace
    | file_scoped_namespace
    ;

block_namespace:
    NAMESPACE qualified_identifier '{' program '}'
    {
        printf("Block namespace\n");
    }
    ;

file_scoped_namespace:
    NAMESPACE qualified_identifier ';'
    {
        printf("File-scoped namespace\n");
    }
    ;

class_declaration:
    class_modifiers CLASS IDENTIFIER class_primary_constructor class_inheritance class_body
    {
        printf("Valid class: %s\n", $3);
        free($3);
        clear_symbols();
    }
    ;

class_primary_constructor:
    /* empty */
    | '(' parameter_list ')'
    {
        printf("  Primary constructor\n");
    }
    ;

class_inheritance:
    /* empty */
    | ':' qualified_identifier
    {
        printf("  Inherits from\n");
    }
    ;

class_modifiers:
    /* empty */
    | class_modifiers PUBLIC
    | class_modifiers PRIVATE
    | class_modifiers PROTECTED
    | class_modifiers INTERNAL
    | class_modifiers STATIC
    | class_modifiers ABSTRACT
    | class_modifiers SEALED
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
    | property_declaration
    ;

field_declaration:
    type IDENTIFIER ';'
    {
        printf("  Field: %s\n", $2);
        add_symbol($2, $1);
        free($2);
        free($1);
    }
    | modifiers type IDENTIFIER ';'
    {
        printf("  Field: %s\n", $3);
        add_symbol($3, $2);
        free($3);
        free($2);
    }
    ;

method_declaration:
    modifiers type IDENTIFIER '(' parameter_list ')' 
    {
        current_method_type = strdup($2);
        current_method_name = strdup($3);
        current_method_has_return = 0;
        printf("  Method: %s (return type: %s)\n", $3, $2);
        free($3);
        free($2);
    }
    method_body
    {
        if (current_method_type && strcmp(current_method_type, "void") != 0 && !current_method_has_return) {
            char err[256];
            snprintf(err, sizeof(err), "Non-void method '%s' must return a value", current_method_name);
            yyerror(err);
        }
        free(current_method_type);
        free(current_method_name);
        current_method_type = NULL;
        current_method_name = NULL;
        current_method_has_return = 0;
    }
    | type IDENTIFIER '(' parameter_list ')' 
    {
        current_method_type = strdup($1);
        current_method_name = strdup($2);
        current_method_has_return = 0;
        printf("  Method: %s (return type: %s)\n", $2, $1);
        free($2);
        free($1);
    }
    method_body
    {
        if (current_method_type && strcmp(current_method_type, "void") != 0 && !current_method_has_return) {
            char err[256];
            snprintf(err, sizeof(err), "Non-void method '%s' must return a value", current_method_name);
            yyerror(err);
        }
        free(current_method_type);
        free(current_method_name);
        current_method_type = NULL;
        current_method_name = NULL;
        current_method_has_return = 0;
    }
    ;

property_declaration:
    modifiers type IDENTIFIER ARROW expression ';'
    {
        printf("  Expression-bodied property: %s (type: %s)\n", $3, $2);
        free($3);
        free($2);
        free($5.type);
    }
    | modifiers type IDENTIFIER '{' accessors '}'
    {
        printf("  Property: %s (type: %s)\n", $3, $2);
        free($3);
        free($2);
    }
    ;

accessors:
    /* empty */
    | accessor
    | accessors accessor
    ;

accessor:
    GET ';'
    | SET ';'
    | GET block
    | SET block
    | modifiers GET ';'
    | modifiers SET ';'
    | modifiers GET block
    | modifiers SET block
    ;

modifiers:
    /* empty */
    | modifiers PUBLIC
    | modifiers PRIVATE
    | modifiers PROTECTED
    | modifiers INTERNAL
    | modifiers STATIC
    | modifiers OVERRIDE
    | modifiers VIRTUAL
    | modifiers ABSTRACT
    | modifiers SEALED
    | modifiers ASYNC
    ;

type:
    INT { $$ = strdup("int"); }
    | STRING { $$ = strdup("string"); }
    | BOOL { $$ = strdup("bool"); }
    | VOID { $$ = strdup("void"); }
    | OBJECT { $$ = strdup("object"); }
    | VAR { $$ = strdup("var"); }
    | IDENTIFIER { $$ = $1; }  // Любой "пользовательский" тип (DateTime, Task, и т.д.)
    | type '?' { 
        char* nullable = malloc(strlen($1) + 2);
        sprintf(nullable, "%s?", $1);
        $$ = nullable;
        free($1);
    }
    ;

parameter_list:
    /* empty */
    | parameter
    | parameter_list ',' parameter
    ;

parameter:
    type IDENTIFIER
    {
        add_symbol($2, $1);
        printf("    Parameter: %s %s\n", $1, $2);
        free($2);
        free($1);
    }
    ;

method_body:
    ';' 
    {
        // Абстрактный метод
        current_method_has_return = 1;
    }
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
    | IF '(' expression ')' statement
    {
        if (strcmp($3.type, "int") != 0 && strcmp($3.type, "bool") != 0 && strcmp($3.type, "unknown") != 0) {
            char err[256];
            snprintf(err, sizeof(err), "Condition must be bool or int, got '%s'", $3.type);
            yyerror(err);
        }
        free($3.type);
    }
    | IF '(' expression ')' statement ELSE statement
    {
        if (strcmp($3.type, "int") != 0 && strcmp($3.type, "bool") != 0 && strcmp($3.type, "unknown") != 0) {
            char err[256];
            snprintf(err, sizeof(err), "Condition must be bool or int, got '%s'", $3.type);
            yyerror(err);
        }
        free($3.type);
    }
    | WHILE '(' expression ')' statement
    {
        if (strcmp($3.type, "int") != 0 && strcmp($3.type, "bool") != 0 && strcmp($3.type, "unknown") != 0) {
            char err[256];
            snprintf(err, sizeof(err), "Condition must be bool or int, got '%s'", $3.type);
            yyerror(err);
        }
        free($3.type);
    }
    ;

declaration_statement:
    type IDENTIFIER ';'
    {
        add_symbol($2, $1);
        free($2);
        free($1);
    }
    | type IDENTIFIER '=' expression ';'
    {
        if (strcmp($4.type, "unknown") != 0 && !is_type_compatible($1, $4.type)) {
            char err[256];
            snprintf(err, sizeof(err), "Cannot assign '%s' to '%s'", 
                     get_type_name($4.type), get_type_name($1));
            yyerror(err);
        }
        add_symbol($2, $1);
        free($2);
        free($1);
        free($4.type);
    }
    ;

assignment_statement:
    IDENTIFIER '=' expression ';'
    {
        Symbol* sym = find_symbol($1);
        if (!sym) {
            char err[256];
            snprintf(err, sizeof(err), "Variable '%s' not declared", $1);
            yyerror(err);
        } else if (strcmp($3.type, "unknown") != 0 && !is_type_compatible(sym->type, $3.type)) {
            char err[256];
            snprintf(err, sizeof(err), "Cannot assign '%s' to '%s'", 
                     get_type_name($3.type), get_type_name(sym->type));
            yyerror(err);
        }
        free($1);
        free($3.type);
    }
    ;

return_statement:
    RETURN ';'
    {
        if (current_method_type && strcmp(current_method_type, "void") != 0) {
            char err[256];
            snprintf(err, sizeof(err), "Non-void method must return a value");
            yyerror(err);
        }
        current_method_has_return = 1;
    }
    | RETURN expression ';'
    {
        if (current_method_type && strcmp(current_method_type, "void") == 0) {
            char err[256];
            snprintf(err, sizeof(err), "Void method cannot return a value (got '%s')", 
                     get_type_name($2.type));
            yyerror(err);
        } else if (current_method_type && strcmp($2.type, "unknown") != 0 && !is_type_compatible(current_method_type, $2.type)) {
            char err[256];
            snprintf(err, sizeof(err), "Return type mismatch: expected '%s', got '%s'",
                     get_type_name(current_method_type), get_type_name($2.type));
            yyerror(err);
        }
        current_method_has_return = 1;
        free($2.type);
    }
    ;

expression:
    INT_LITERAL
    {
        $$.type = strdup("int");
    }
    | IDENTIFIER
    {
        Symbol* sym = find_symbol($1);
        if (!sym) {
            $$.type = strdup("unknown");
        } else {
            $$.type = strdup(sym->type);
        }
        free($1);
    }
    | TRUE
    {
        $$.type = strdup("bool");
    }
    | FALSE
    {
        $$.type = strdup("bool");
    }
    | NULL_
    {
        $$.type = strdup("null");
    }
    | expression '+' expression
    {
        if (strcmp($1.type, "int") == 0 && strcmp($3.type, "int") == 0) {
            $$.type = strdup("int");
        } else if ((strcmp($1.type, "string") == 0 || strcmp($3.type, "string") == 0) &&
                   (strcmp($1.type, "unknown") != 0 && strcmp($3.type, "unknown") != 0)) {
            $$.type = strdup("string");
        } else {
            $$.type = strdup("unknown");
        }
        free($1.type);
        free($3.type);
    }
    | expression '-' expression
    | expression '*' expression
    | expression '/' expression
    | expression '%' expression
    {
        if (strcmp($1.type, "int") == 0 && strcmp($3.type, "int") == 0) {
            $$.type = strdup("int");
        } else {
            $$.type = strdup("unknown");
        }
        free($1.type);
        free($3.type);
    }
    | '(' expression ')'
    {
        $$.type = strdup($2.type);
        free($2.type);
    }
    | '-' expression %prec UMINUS
    {
        if (strcmp($2.type, "int") == 0) {
            $$.type = strdup("int");
        } else {
            $$.type = strdup("unknown");
        }
        free($2.type);
    }
    | expression '<' expression
    | expression '>' expression
    | expression LE expression
    | expression GE expression
    | expression EQ_EQ expression
    | expression NE expression
    {
        $$.type = strdup("bool");
        free($1.type);
        free($3.type);
    }
    | expression AND_AND expression
    | expression OR_OR expression
    {
        $$.type = strdup("bool");
        free($1.type);
        free($3.type);
    }
    | '!' expression
    {
        $$.type = strdup("bool");
        free($2.type);
    }
    | expression '.' IDENTIFIER
    {
        $$.type = strdup("unknown");
        free($3);
    }
    | expression '.' IDENTIFIER '(' ')'
    {
        $$.type = strdup("unknown");
        free($3);
    }
    | expression '.' IDENTIFIER '(' argument_list ')'
    {
        $$.type = strdup("unknown");
        free($3);
    }
    | expression '.' IDENTIFIER '<' type '>' '(' argument_list ')'
    {
        $$.type = strdup("unknown");
        free($3);
        free($5);
    }
    | AWAIT expression
    {
        $$.type = strdup($2.type);
        free($2.type);
    }
    | NEW qualified_identifier '(' argument_list ')'
    {
        $$.type = strdup("unknown");
    }
    | expression NULL_COALESCE expression
    {
        $$.type = strdup($1.type);
        free($1.type);
        free($3.type);
    }
    | expression NULL_COALESCE_ASSIGN expression
    {
        $$.type = strdup($1.type);
        free($1.type);
        free($3.type);
    }
    | expression '|' expression
    {
        if (strcmp($1.type, "int") == 0 && strcmp($3.type, "int") == 0) {
            $$.type = strdup("int");
        } else {
            $$.type = strdup("unknown");
        }
        free($1.type);
        free($3.type);
    }
    ;

argument_list:
    /* empty */
    | expression
    | argument_list ',' expression
    ;

%%

void yyerror(const char* msg) {
    fprintf(stderr, "Semantic error at line %d, column %d: %s\n", 
            line_num, column_num, msg);
    has_errors = 1;
}