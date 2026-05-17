%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int line_num;
extern int column_num;
extern char* yytext;
extern FILE* yyin;

void print_semantic_error(const char* msg);
void yyerror(const char* msg);
int yylex(void);

int has_errors = 0;

int loop_depth = 0;

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

int is_task_generic(const char* type) {
    return strncmp(type, "Task<", 5) == 0;
}

int is_collection_type(const char* type) {
    if (type == NULL) return 0;
    return strncmp(type, "Dictionary<", 11) == 0 ||
           strncmp(type, "List<", 5) == 0 ||
           strncmp(type, "IEnumerable<", 12) == 0 ||
           strncmp(type, "ICollection<", 12) == 0 ||
           strcmp(type, "List") == 0 ||
           strcmp(type, "IEnumerable") == 0 ||
           strcmp(type, "ICollection") == 0 ||
           strcmp(type, "Collection") == 0 ||
           strcmp(type, "IList") == 0;
}

int is_type_compatible(const char* target, const char* source) {
    if (strcmp(target, source) == 0) return 1;
    if (strcmp(source, "collection") == 0 && is_collection_type(target)) return 1;
    if (strcmp(target, "bool") == 0 && strcmp(source, "int") == 0) return 1;
    if (strncmp(target, "ValueTask<", 10) == 0 && strcmp(source, "null") == 0) return 1;

    if (strcmp(source, "var") == 0) return 1;
    
    // null совместим с любым nullable типом (заканчивается на ?)
    if (strcmp(source, "null") == 0) {
        int len = strlen(target);
        if (len > 0 && target[len-1] == '?') return 1;
        if (strcmp(target, "string") == 0) return 1; // string - reference type
        if (strcmp(target, "object") == 0) return 1; // object - reference type
    }
    
    return 0;
}

char* extract_task_inner_type(const char* type) {
    if (strncmp(type, "Task<", 5) != 0) return NULL;

    const char* start = type + 5;
    const char* end = strrchr(type, '>');

    if (!end || end <= start) return NULL;

    int len = end - start;
    char* result = malloc(len + 1);
    strncpy(result, start, len);
    result[len] = '\0';

    return result;
}

int is_task_like(const char* type) {
    if (strcmp(type, "Task") == 0) return 1;
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
%token STATIC READONLY STRING SWITCH THROW TRUE TRY USING VAR VIRTUAL VOID WHILE
%token PARTIAL

// Операторы
%token EQ_EQ NE LE GE AND_AND OR_OR ARROW NULL_COALESCE_ASSIGN NULL_COALESCE
%token ERROR
%token TASK

%token IN
%token TYPEOF
%token CONST
%token <string> STRING_LITERAL
%token RECORD
%token WHERE STRUCT NOTNULL UNMANAGED
%token BASE THIS
%token IS
%token OUT AS NOT
%token WITH

%type <expr> expression
%type <string> type
%type <expr> lambda_expression
%type <string> type_list
%type <string> qualified_identifier

%left NOT AND_AND OR_OR
%right ARROW
%right '?' ':'
%left '<' '>' LE GE EQ_EQ NE
%right '!'
%left AS
%left '+' '-'
%left '*' '/' '%'
%nonassoc UMINUS

%start program

%%

qualified_identifier:
    IDENTIFIER
    {
        $$ = $1;
    }
    | qualified_identifier '.' IDENTIFIER
    {
        char* result = malloc(strlen($1) + strlen($3) + 2);
        sprintf(result, "%s.%s", $1, $3);
        free($1);
        free($3);
        $$ = result;
    }
    ;

top_level_statement:
    statement
    ;

program:
    /* empty */
    | program using_directive
    | program namespace_declaration
    | program type_declaration
    | program top_level_statement
    ;

using_directive:
    USING qualified_identifier ';'
    {
        // printf("Using directive\n");
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

type_declaration:
    class_declaration
    | record_declaration
    ;

record_declaration:
    class_modifiers RECORD IDENTIFIER record_primary_constructor class_inheritance class_body
    {
        printf("Valid record: %s\n", $3);
        free($3);
        clear_symbols();
    }
    | class_modifiers RECORD IDENTIFIER record_primary_constructor class_inheritance ';'
    {
        printf("Valid positional record: %s\n", $3);
        free($3);
        clear_symbols();
    }
    ;

record_primary_constructor:
    /* empty */
    | '(' parameter_list ')'
    {
        printf("  Record primary constructor\n");
    }
    ;

class_declaration:
    class_modifiers CLASS IDENTIFIER class_primary_constructor class_inheritance class_body
    {
        printf("Valid class: %s\n", $3);
        free($3);
        clear_symbols();
    }
    | class_modifiers CLASS IDENTIFIER type_parameter_list class_primary_constructor class_inheritance class_body
    {
        printf("Valid generic class: %s\n", $3);
        free($3);
        clear_symbols();
    }
    | class_modifiers CLASS IDENTIFIER type_parameter_list class_primary_constructor class_inheritance type_parameter_constraints_clause class_body
    {
        printf("Valid generic class with constraints: %s\n", $3);
        free($3);
        clear_symbols();
    }
    ;

type_parameter_list:
    '<' type_parameters '>'
    ;

type_parameters:
    IDENTIFIER
    | type_parameters ',' IDENTIFIER
    ;

type_parameter_constraints_clause:
    /* empty */
    | type_parameter_constraints_clause WHERE IDENTIFIER ':' constraint
    ;

constraint:
    IDENTIFIER
    | IDENTIFIER '?'
    | CLASS
    | STRUCT
    | NOTNULL
    | UNMANAGED
    | NEW '(' ')'
    | constraint ',' IDENTIFIER
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
    | class_modifiers PARTIAL
    ;

class_body:
    '{' class_members '}'
    ;

class_members:
    /* empty */
    | class_members class_member
    ;

const_declaration:
    modifiers CONST type IDENTIFIER '=' expression ';'
    {
        printf("  Constant: %s (type: %s)\n", $4, $3);
        add_symbol($4, $3);
        free($4);
        free($3);
        free($6.type);
    }
    ;

class_member:
    field_declaration
    | method_declaration
    | explicit_method_declaration
    | expression_method_declaration
    | property_declaration
    | const_declaration
    | constructor_declaration
    | explicit_property_declaration
    ;

explicit_property_declaration:
    type qualified_identifier ARROW expression ';'
    {
        printf("  Explicit interface property: %s (type: %s)\n", $2, $1);
        free($2);
        free($1);
        free($4.type);
    }
    ;

explicit_method_declaration:
    modifiers type qualified_identifier '(' parameter_list ')' block
    {
        current_method_type = strdup($2);
        current_method_name = strdup($3);
        current_method_has_return = 0;
        // printf("  Explicit interface method: %s (return type: %s)\n", $3, $2);
        free($3);
        free($2);
        free(current_method_type);
        free(current_method_name);
        current_method_type = NULL;
        current_method_name = NULL;
        current_method_has_return = 0;
    }
    | type qualified_identifier '(' parameter_list ')' block
    {
        current_method_type = strdup($1);
        current_method_name = strdup($2);
        current_method_has_return = 0;
        // printf("  Explicit interface method: %s (return type: %s)\n", $2, $1);
        free($2);
        free($1);
        free(current_method_type);
        free(current_method_name);
        current_method_type = NULL;
        current_method_name = NULL;
        current_method_has_return = 0;
    }
    | modifiers type qualified_identifier '(' parameter_list ')' ARROW expression ';'
    {
        // printf("  Expression-bodied explicit interface method: %s (return type: %s)\n", $3, $2);
        free($3);
        free($2);
        free($8.type);
    }
    | type qualified_identifier '(' parameter_list ')' ARROW expression ';'
    {
        // printf("  Expression-bodied explicit interface method: %s (return type: %s)\n", $2, $1);
        free($2);
        free($1);
        free($7.type);
    }
    ;

expression_method_declaration:
    modifiers type IDENTIFIER '(' parameter_list ')' ARROW expression ';'
    {
        // printf("  Expression-bodied method: %s (return type: %s)\n", $3, $2);
        free($3);
        free($2);
        free($8.type);
    }
    | type IDENTIFIER '(' parameter_list ')' ARROW expression ';'
    {
        // printf("  Expression-bodied method: %s (return type: %s)\n", $2, $1);
        free($2);
        free($1);
        free($7.type);
    }
    ;

constructor_declaration:
    modifiers IDENTIFIER '(' parameter_list ')' constructor_initializer block
    {
        printf("  Constructor: %s\n", $2);
        free($2);
    }
    ;

constructor_initializer:
    /* empty */
    | ':' BASE '(' argument_list ')'
    | ':' THIS '(' argument_list ')'
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
    | type IDENTIFIER '=' expression ';'
    {
        printf("  Field with init: %s\n", $2);
        add_symbol($2, $1);
        free($2);
        free($1);
        free($4.type);
    }
    | modifiers type IDENTIFIER '=' expression ';'
    {
        printf("  Field with init: %s\n", $3);
        add_symbol($3, $2);
        free($3);
        free($2);
        free($5.type);
    }
    ;

method_declaration:
    modifiers type IDENTIFIER '(' parameter_list ')' 
    {
        current_method_type = strdup($2);
        current_method_name = strdup($3);
        current_method_has_return = 0;
        // printf("  Method: %s (return type: %s)\n", $3, $2);
        free($3);
        free($2);
    }
    method_body
    {
        if (current_method_type 
            && strcmp(current_method_type, "void") != 0 
            && !is_task_like(current_method_type)
            && !current_method_has_return) 
        {
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
        // printf("  Method: %s (return type: %s)\n", $2, $1);
        free($2);
        free($1);
    }
    method_body
    {
        if (current_method_type 
            && strcmp(current_method_type, "void") != 0 
            && !is_task_like(current_method_type)
            && !current_method_has_return)
        {
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
    | type IDENTIFIER ARROW expression ';'
    {
        printf("  Expression-bodied property: %s (type: %s)\n", $2, $1);
        free($2);
        free($1);
        free($4.type);
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
    | modifiers READONLY
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
    | TASK { $$ = strdup("Task"); }
    | TASK '<' type '>' { 
        char* task_type = malloc(strlen("Task<") + strlen($3) + 2);
        sprintf(task_type, "Task<%s>", $3);
        $$ = task_type;
        free($3);
    }
    | IDENTIFIER '<' type '>' '?'
    {
        char* tmp = malloc(strlen($1) + strlen($3) + 4);
        sprintf(tmp, "%s<%s>?", $1, $3);
        $$ = tmp;
        free($1);
        free($3);
    }
    | IDENTIFIER '<' type '>'
    {
        char* tmp = malloc(strlen($1) + strlen($3) + 3);
        sprintf(tmp, "%s<%s>", $1, $3);
        $$ = tmp;
        free($1);
        free($3);
    }
    | IDENTIFIER { $$ = $1; }
    | type '?' { 
        char* nullable = malloc(strlen($1) + 2);
        sprintf(nullable, "%s?", $1);
        $$ = nullable;
        free($1);
    }
    | IDENTIFIER '<' type_list '>'
    {
        char* tmp = malloc(strlen($1) + strlen($3) + 3);
        sprintf(tmp, "%s<%s>", $1, $3);
        free($1);
        free($3);
        $$ = tmp;
    }
    | type '[' ']'
    {
        char* array_type = malloc(strlen($1) + 3);
        sprintf(array_type, "%s[]", $1);
        $$ = array_type;
        free($1);
    }
    ;

type_list:
    type
    {
        $$ = $1;
    }
    | type_list ',' type
    {
        char* tmp = malloc(strlen($1) + strlen($3) + 2);
        sprintf(tmp, "%s,%s", $1, $3);
        free($1);
        free($3);
        $$ = tmp;
    }

parameter_list:
    /* empty */
    | parameter
    | parameter_list ',' parameter
    ;

parameter:
    type IDENTIFIER
    {
        add_symbol($2, $1);
        // printf("    Parameter: %s %s\n", $1, $2);
        free($2);
        free($1);
    }
    | type IDENTIFIER '=' expression
    {
        add_symbol($2, $1);
        // printf("    Parameter: %s %s = default\n", $1, $2);
        free($2);
        free($1);
        free($4.type);
    }
    | IDENTIFIER
    {
        // для случаев (_, _) => ...
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

switch_labels:
    switch_label
    | switch_labels switch_label
;

switch_label:
    CASE expression ':'
    {
        free($2.type);
    }
    | DEFAULT ':'
;

switch_sections:
    /* empty */
    | switch_sections switch_section
;

switch_section:
    switch_labels statements
;

switch_block:
    '{' switch_sections '}'
    ;

statements:
    /* empty */
    | statements statement
    ;

statement:
    block
    | IF '(' expression ')' statement
    {
        if (strcmp($3.type, "int") != 0 && strcmp($3.type, "bool") != 0 && 
            strcmp($3.type, "unknown") != 0 && strcmp($3.type, "null") != 0) {
            char err[256];
            snprintf(err, sizeof(err), "Condition must be bool or int, got '%s'", $3.type);
            yyerror(err);
        }
        free($3.type);
    }
    | IF '(' expression ')' statement ELSE statement
    {
        if (strcmp($3.type, "int") != 0 && strcmp($3.type, "bool") != 0 && 
            strcmp($3.type, "unknown") != 0 && strcmp($3.type, "null") != 0) {
            char err[256];
            snprintf(err, sizeof(err), "Condition must be bool or int, got '%s'", $3.type);
            yyerror(err);
        }
        free($3.type);
    }
    | WHILE '(' expression ')'
    {
        loop_depth++;
    } 
    statement
    {
        loop_depth--;
        if (strcmp($3.type, "int") != 0 && strcmp($3.type, "bool") != 0 && 
            strcmp($3.type, "unknown") != 0 && strcmp($3.type, "null") != 0) {
            char err[256];
            snprintf(err, sizeof(err), "Condition must be bool or int, got '%s'", $3.type);
            yyerror(err);
        }
        free($3.type);
    }
    | FOREACH '(' type IDENTIFIER IN expression ')' statement
    {
        loop_depth++;
    } 
    statement
    {
        loop_depth--;
        char* var_type = NULL;
        if (strcmp($3, "var") == 0) {
            // Пытаемся получить тип элемента из коллекции
            if (strncmp($6.type, "List<", 5) == 0 || strncmp($6.type, "IEnumerable<", 12) == 0) {
                // Извлекаем тип из скобок
                char* start = strchr($6.type, '<') + 1;
                char* end = strchr(start, '>');
                if (end && end > start) {
                    int len = end - start;
                    var_type = malloc(len + 1);
                    strncpy(var_type, start, len);
                    var_type[len] = '\0';
                } else {
                    var_type = strdup("unknown");
                }
            } else {
                var_type = strdup("unknown");
            }
        } else {
            var_type = strdup($3);
        }
        add_symbol($4, var_type);
        free(var_type);
        free($4);
        free($3);
        if (strcmp($6.type, "collection") != 0 && strcmp($6.type, "unknown") != 0 && !is_collection_type($6.type)) {
            char err[256];
            snprintf(err, sizeof(err), "Expected collection type after 'in', got '%s'", $6.type);
            yyerror(err);
        }
        free($6.type);
    }
    | declaration_statement
    | assignment_statement
    | return_statement
    | expression ';'
    {
        free($1.type);
    }
    | CONTINUE ';'
    {
        if (loop_depth == 0) {
            yyerror("continue statement not within a loop");
        }
    }
    | BREAK ';'
    {
        if (loop_depth == 0) {
            yyerror("break statement not within a loop");
        }
    }
    | SWITCH '(' expression ')' switch_block
    {
        free($3.type);
    }
    | THROW expression ';'
    {
        free($2.type);
    }
    | USING type IDENTIFIER '=' expression ';'
    {
        add_symbol($3, $2);
        free($3);
        free($2);
        free($5.type);
    }
    | USING IDENTIFIER '=' expression ';'
    {
        add_symbol($2, "var");
        free($2);
        free($4.type);
    }
    | USING '(' expression ')' statement
    {
        free($3.type);
    }
    ;

declaration_statement:
    type IDENTIFIER ';'
    {
        // printf("DEBUG: declaration_statement (no init)\n");
        add_symbol($2, $1);
        free($2);
        free($1);
    }
    | type IDENTIFIER '=' expression ';'
    {
        // printf("DEBUG: declaration_statement with init, type=%s, var=%s, expr_type=%s\n", $1, $2, $4.type);

        if (strcmp($1, "var") == 0) {
            add_symbol($2, $4.type);
        } else {
            if (strcmp($4.type, "unknown") != 0 && !is_type_compatible($1, $4.type)) {
                char err[256];
                snprintf(err, sizeof(err), "Cannot assign '%s' to '%s'", 
                        get_type_name($4.type), get_type_name($1));
                yyerror(err);
            }
            add_symbol($2, $1);
        }

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
        } else if (strcmp($3.type, "unknown") != 0 && strcmp($3.type, "lambda") != 0 && !is_type_compatible(sym->type, $3.type)) {
            char err[256];
            snprintf(err, sizeof(err), "Cannot assign '%s' to '%s'", 
                     get_type_name($3.type), get_type_name(sym->type));
            yyerror(err);
        }
        free($1);
        free($3.type);
    }
    | expression '.' IDENTIFIER '=' expression ';'
    {
        // Присваивание свойству
        // printf("DEBUG: property assignment: %s.%s\n", $1.type, $3);
        free($3);
        free($1.type);
        free($5.type);
    }
    ;

return_statement:
    RETURN ';'
    {
        if (current_method_type 
            && strcmp(current_method_type, "void") != 0
            && !is_task_like(current_method_type)) 
        {
            char err[256];
            snprintf(err, sizeof(err), "Non-void method must return a value");
            yyerror(err);
        }
        current_method_has_return = 1;
    }
    | RETURN expression ';'
    {
        if (current_method_type) {
            if (strcmp(current_method_type, "void") == 0) {
                yyerror("Void method cannot return a value");
            }
            else if (is_task_generic(current_method_type)) {
                char* inner = extract_task_inner_type(current_method_type);

                if (inner && strcmp($2.type, "unknown") != 0 &&
                    !is_type_compatible(inner, $2.type)) {
                    
                    char err[256];
                    snprintf(err, sizeof(err),
                        "Return type mismatch: expected '%s', got '%s'",
                        inner, $2.type);
                    yyerror(err);
                }

                if (inner) free(inner);
            }
            else if (strcmp(current_method_type, "Task") == 0) {
                // НИЧЕГО НЕ ДЕЛАЕМ (разрешаем всё)
            }
            else {
                if (strcmp($2.type, "unknown") != 0 &&
                    !is_type_compatible(current_method_type, $2.type)) {
                    
                    char err[256];
                    snprintf(err, sizeof(err),
                        "Return type mismatch: expected '%s', got '%s'",
                        current_method_type, $2.type);
                    yyerror(err);
                }
            }
        }

        current_method_has_return = 1;
        free($2.type);
    }
    ;
    
lambda_expression:
    '(' parameter_list ')' ARROW expression
    {
        // printf("DEBUG: lambda_expression with parens\n");
        $$.type = strdup("lambda");
        free($5.type);
    }
    | IDENTIFIER ARROW expression
    {
        // printf("DEBUG: lambda_expression simple: %s => ...\n", $1);
        $$.type = strdup("lambda");
        free($1);
        free($3.type);
    }
    | '_' ARROW expression
    {
        $$.type = strdup("lambda");
        free($3.type);
    }
    ;

expression:
    lambda_expression
    {
        $$.type = strdup("lambda");
    }
    | expression '.' IDENTIFIER '(' method_arguments ')'
    {
        $$.type = strdup("unknown");
        free($3);
    }
    | INT_LITERAL
    {
        $$.type = strdup("int");
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
    | STRING
    {
        $$.type = strdup("string");
    }
    | INT
    {
        $$.type = strdup("int");
    }
    | BOOL
    {
        $$.type = strdup("bool");
    }
    | OBJECT
    {
        $$.type = strdup("object");
    }
    | IDENTIFIER
    {
        // printf("DEBUG: identifier '%s'\n", $1);
        Symbol* sym = find_symbol($1);
        if (!sym) {
            // printf("DEBUG: symbol '%s' NOT found\n", $1);
            $$.type = strdup("unknown");
        } else {
            // printf("DEBUG: symbol '%s' found, type='%s'\n", $1, sym->type);
            $$.type = strdup(sym->type);
        }
        free($1);
    }
    | IDENTIFIER '(' ')'
    {
        $$.type = strdup("unknown");
        free($1);
    }
    | IDENTIFIER '(' argument_list ')'
    {
        $$.type = strdup("unknown");
        free($1);
    }
    | AWAIT expression
    {
        $$.type = strdup($2.type);
        free($2.type);
    }
    | '(' expression ')'
    {
        $$.type = strdup($2.type);
        free($2.type);
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
    {
        if (strcmp($1.type, "int") == 0 && strcmp($3.type, "int") == 0) {
            $$.type = strdup("int");
        } else {
            $$.type = strdup("unknown");
        }
        free($1.type);
        free($3.type);
    }
    | expression '*' expression
    {
        if (strcmp($1.type, "int") == 0 && strcmp($3.type, "int") == 0) {
            $$.type = strdup("int");
        } else {
            $$.type = strdup("unknown");
        }
        free($1.type);
        free($3.type);
    }
    | expression '/' expression
    {
        if (strcmp($1.type, "int") == 0 && strcmp($3.type, "int") == 0) {
            $$.type = strdup("int");
        } else {
            $$.type = strdup("unknown");
        }
        free($1.type);
        free($3.type);
    }
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
    {
        $$.type = strdup("bool");
        free($1.type);
        free($3.type);
    }
    | expression '>' expression
    {
        $$.type = strdup("bool");
        free($1.type);
        free($3.type);
    }
    | expression LE expression
    {
        $$.type = strdup("bool");
        free($1.type);
        free($3.type);
    }
    | expression GE expression
    {
        $$.type = strdup("bool");
        free($1.type);
        free($3.type);
    }
    | expression EQ_EQ expression
    {
        $$.type = strdup("bool");
        free($1.type);
        free($3.type);
    }
    | expression NE expression
    {
        $$.type = strdup("bool");
        free($1.type);
        free($3.type);
    }
    | expression AND_AND expression
    {
        $$.type = strdup("bool");
        free($1.type);
        free($3.type);
    }
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
        // НЕ free($5) - это type, его освободит родитель
    }
    | expression '.' IDENTIFIER '<' type_list '>' '(' ')'
    {
        $$.type = strdup("unknown");
        free($3);
    }
    | expression '.' IDENTIFIER '<' type_list '>' '(' argument_list ')'
    {
        $$.type = strdup("unknown");
        free($3);
    }
    | expression '?' '.' IDENTIFIER
    {
        $$.type = strdup("unknown");
        free($4);
    }
    | expression '?' '.' IDENTIFIER '(' ')'
    {
        $$.type = strdup("unknown");
        free($4);
    }
    | expression '?' '.' IDENTIFIER '(' argument_list ')'
    {
        $$.type = strdup("unknown");
        free($4);
    }
    | expression '?' '.' IDENTIFIER '<' type_list '>' '(' ')'
    {
        $$.type = strdup("unknown");
        free($4);
    }
    | expression '?' '.' IDENTIFIER '<' type_list '>' '(' argument_list ')'
    {
        $$.type = strdup("unknown");
        free($4);
    }
    | IDENTIFIER '?' '.' IDENTIFIER
    {
        $$.type = strdup("unknown");
        free($1);
        free($4);
    }
    | IDENTIFIER '?' '.' IDENTIFIER '(' argument_list ')'
    {
        $$.type = strdup("unknown");
        free($1);
        free($4);
    }
    | NEW IDENTIFIER '(' ')' '{' property_initializers '}'
    {
        $$.type = strdup("object");
        free($2);
    }
    | NEW IDENTIFIER '(' argument_list ')' '{' property_initializers '}'
    {
        $$.type = strdup("object");
        free($2);
    }
    | NEW IDENTIFIER '(' argument_list ')'
    {
        $$.type = strdup($2);
        free($2);
    }
    | NEW IDENTIFIER '<' type '>' '(' argument_list ')'
    {
        $$.type = strdup($2);
        free($2);
        free($4);
    }
    | NEW IDENTIFIER '<' type '>' '(' ')'
    {
        $$.type = strdup($2);
        free($2);
        free($4);
    }
    | NEW IDENTIFIER '(' ')'
    {
        $$.type = strdup($2);
        free($2);
    }
    | NEW qualified_identifier '(' argument_list ')'
    {
        $$.type = strdup("unknown");
    }
    | NEW qualified_identifier '(' ')' 
    {
        $$.type = strdup("unknown");
    }
    | NEW qualified_identifier '<' type '>' '(' ')'
    {
        $$.type = strdup("unknown");
    }
    | NEW qualified_identifier '<' type '>' '(' argument_list ')'
    {
        $$.type = strdup("unknown");
    }
    | NEW type '{' argument_list '}'
    {
        $$.type = strdup("collection");
    }
    | NEW type '[' ']' '{' argument_list '}'
    {
        $$.type = strdup("collection");
    }
    | NEW qualified_identifier '<' type '>' '(' ')' '{' argument_list '}'
    {
        $$.type = strdup("collection");
    }
    | NEW qualified_identifier '(' ')' '{' argument_list '}'
    {
        $$.type = strdup("collection");
    }
    | NEW qualified_identifier '<' type '>' '{' argument_list '}'
    {
        $$.type = strdup("collection");
    }
    | NEW qualified_identifier '{' argument_list '}'
    {
        $$.type = strdup("collection");
    }
    | NEW qualified_identifier '{' property_initializers '}'
    {
        $$.type = strdup($2);  // Тип из идентификатора
        free($2);
    }
    | NEW type '{' property_initializers '}'
    {
        $$.type = strdup("object");
    }
    | TYPEOF '(' qualified_identifier ')'
    {
        $$.type = strdup("type");
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
    | IDENTIFIER ARROW block
    {
        $$.type = strdup("unknown");
        free($1);
    }
    | '(' parameter_list ')' ARROW block
    {
        $$.type = strdup("unknown");
    }
    | ASYNC IDENTIFIER ARROW expression
    {
        $$.type = strdup("unknown");
        free($2);
        free($4.type);
    }
    | ASYNC IDENTIFIER ARROW block
    {
        $$.type = strdup("unknown");
        free($2);
    }
    | ASYNC '(' parameter_list ')' ARROW expression
    {
        $$.type = strdup("unknown");
    }
    | ASYNC '(' parameter_list ')' ARROW block
    {
        $$.type = strdup("unknown");
    }
    | expression '!'
    {
        $$.type = strdup($1.type);
        free($1.type);
    }
    | '[' argument_list ']'
    {
        $$.type = strdup("collection");
    }
    | '[' ']'
    {
        $$.type = strdup("collection");
    }
    | expression '[' argument_list ']'
    {
        $$.type = strdup("unknown");
        free($1.type);
    }
    | expression '[' ']'
    {
        $$.type = strdup("unknown");
        free($1.type);
    }
    | expression '.' IDENTIFIER '(' method_arguments ')'
    {
        $$.type = strdup("unknown");
        free($3);
    }
    | STRING_LITERAL
    {
        $$.type = strdup("string");
    }
    | expression IS type IDENTIFIER
    {
        $$.type = strdup("bool");
        free($1.type);
        free($3);
        free($4);
    }
    | expression IS type
    {
        $$.type = strdup("bool");
        free($1.type);
        free($3);
    }
    | '(' type ')' expression
    {
        $$.type = strdup($2);
        free($2);
        free($4.type);
    }
    | DEFAULT
    {
        $$.type = strdup("default");
    }
    | expression '?' expression ':' expression
    {
        $$.type = strdup($3.type);
        free($1.type);
        free($3.type);
        free($5.type);
    }
    | OUT VAR IDENTIFIER
    {
        $$.type = strdup("var");
        free($3);
    }
    | OUT type IDENTIFIER
    {
        $$.type = strdup("outtype");
        free($2);
        free($3);
    }
    | expression IS NULL_
    {
        $$.type = strdup("bool");
        free($1.type);
    }
    | expression AS type
    {
        $$.type = strdup($3);
        free($1.type);
        free($3);
    }
    | expression IS NOT NULL_
    {
        $$.type = strdup("bool");
        free($1.type);
    }
    | expression WITH '{' property_initializers '}'
    {
        $$.type = strdup($1.type);
        free($1.type);
    }
    | STRING '.' IDENTIFIER '(' argument_list ')'
    {
        $$.type = strdup("bool");
        free($3);
    }
    | THIS
    {
        $$.type = strdup("this");
    }
    | THIS '.' IDENTIFIER
    {
        $$.type = strdup("unknown");
        free($3);
    }
    ;

property_initializers:
    /* empty */
    | property_initializer
    | property_initializers ',' property_initializer
    ;

property_initializer:
    IDENTIFIER '=' expression
    {
        free($1);
        free($3.type);
    }
    ;

method_arguments:
    /* empty */
    | method_argument
    | method_arguments ',' method_argument
    ;

method_argument:
    expression
    | lambda_expression
    | IDENTIFIER ':' expression
    {
        free($1);
        free($3.type);
    }
    | IDENTIFIER ':' lambda_expression
    {
        free($1);
    }
    ;

argument_list:
    /* empty */
    | argument
    | argument_list ',' argument
    ;

argument:
    expression
    | lambda_expression
    | OUT VAR IDENTIFIER
    {
        add_symbol($3, "var");
        free($3);
    }
    | OUT type IDENTIFIER
    {
        add_symbol($3, $2);
        free($2);
        free($3);
    }
    | IDENTIFIER ':' expression
    {
        free($1);
        free($3.type);
    }
    | IDENTIFIER ':' lambda_expression
    {
        free($1);
    }
    ;

%%

void yyerror(const char* msg) {
    print_semantic_error(msg);
    has_errors = 1;
}