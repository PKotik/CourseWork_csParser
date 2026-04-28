%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.tab.h"

int line_num = 1;
int column_num = 1;

void update_position(const char* text, int len) {
    int i;
    for (i = 0; i < len; i++) {
        if (text[i] == '\n') {
            line_num++;
            column_num = 1;
        } else {
            column_num++;
        }
    }
}
%}

%option noyywrap
%option yylineno

%x COMMENT

%%

[ \t\r]+        { update_position(yytext, yyleng); /* ignore */ }
\n              { update_position(yytext, yyleng); /* ignore */ }

"abstract"      { update_position(yytext, yyleng); return ABSTRACT; }
"async"         { update_position(yytext, yyleng); return ASYNC; }
"await"         { update_position(yytext, yyleng); return AWAIT; }
"bool"          { update_position(yytext, yyleng); return BOOL; }
"break"         { update_position(yytext, yyleng); return BREAK; }
"case"          { update_position(yytext, yyleng); return CASE; }
"catch"         { update_position(yytext, yyleng); return CATCH; }
"class"         { update_position(yytext, yyleng); return CLASS; }
"continue"      { update_position(yytext, yyleng); return CONTINUE; }
"default"       { update_position(yytext, yyleng); return DEFAULT; }
"else"          { update_position(yytext, yyleng); return ELSE; }
"false"         { update_position(yytext, yyleng); return FALSE; }
"finally"       { update_position(yytext, yyleng); return FINALLY; }
"for"           { update_position(yytext, yyleng); return FOR; }
"foreach"       { update_position(yytext, yyleng); return FOREACH; }
"get"           { update_position(yytext, yyleng); return GET; }
"if"            { update_position(yytext, yyleng); return IF; }
"int"           { update_position(yytext, yyleng); return INT; }
"internal"      { update_position(yytext, yyleng); return INTERNAL; }
"namespace"     { update_position(yytext, yyleng); return NAMESPACE; }
"new"           { update_position(yytext, yyleng); return NEW; }
"null"          { update_position(yytext, yyleng); return NULL_; }
"object"        { update_position(yytext, yyleng); return OBJECT; }
"override"      { update_position(yytext, yyleng); return OVERRIDE; }
"private"       { update_position(yytext, yyleng); return PRIVATE; }
"protected"     { update_position(yytext, yyleng); return PROTECTED; }
"public"        { update_position(yytext, yyleng); return PUBLIC; }
"return"        { update_position(yytext, yyleng); return RETURN; }
"sealed"        { update_position(yytext, yyleng); return SEALED; }
"set"           { update_position(yytext, yyleng); return SET; }
"static"        { update_position(yytext, yyleng); return STATIC; }
"string"        { update_position(yytext, yyleng); return STRING; }
"switch"        { update_position(yytext, yyleng); return SWITCH; }
"throw"         { update_position(yytext, yyleng); return THROW; }
"true"          { update_position(yytext, yyleng); return TRUE; }
"try"           { update_position(yytext, yyleng); return TRY; }
"using"         { update_position(yytext, yyleng); return USING; }
"var"           { update_position(yytext, yyleng); return VAR; }
"virtual"       { update_position(yytext, yyleng); return VIRTUAL; }
"void"          { update_position(yytext, yyleng); return VOID; }
"while"         { update_position(yytext, yyleng); return WHILE; }

[a-zA-Z_][a-zA-Z0-9_]* {
    update_position(yytext, yyleng);
    yylval.string = strdup(yytext);
    return IDENTIFIER;
}

[0-9]+ {
    update_position(yytext, yyleng);
    yylval.num = atoi(yytext);
    return INT_LITERAL;
}

"{"             { update_position(yytext, yyleng); return '{'; }
"}"             { update_position(yytext, yyleng); return '}'; }
"("             { update_position(yytext, yyleng); return '('; }
")"             { update_position(yytext, yyleng); return ')'; }
"["             { update_position(yytext, yyleng); return '['; }
"]"             { update_position(yytext, yyleng); return ']'; }
";"             { update_position(yytext, yyleng); return ';'; }
"="             { update_position(yytext, yyleng); return '='; }
"+"             { update_position(yytext, yyleng); return '+'; }
"-"             { update_position(yytext, yyleng); return '-'; }
"*"             { update_position(yytext, yyleng); return '*'; }
"/"             { update_position(yytext, yyleng); return '/'; }
"%"             { update_position(yytext, yyleng); return '%'; }
","             { update_position(yytext, yyleng); return ','; }
":"             { update_position(yytext, yyleng); return ':'; }
"."             { update_position(yytext, yyleng); return '.'; }
"|"             { update_position(yytext, yyleng); return '|'; }
"?"             { update_position(yytext, yyleng); return '?'; }
"~"             { update_position(yytext, yyleng); return '~'; }

"=="            { update_position(yytext, yyleng); return EQ_EQ; }
"!="            { update_position(yytext, yyleng); return NE; }
"<"             { update_position(yytext, yyleng); return '<'; }
">"             { update_position(yytext, yyleng); return '>'; }
"<="            { update_position(yytext, yyleng); return LE; }
">="            { update_position(yytext, yyleng); return GE; }
"&&"            { update_position(yytext, yyleng); return AND_AND; }
"||"            { update_position(yytext, yyleng); return OR_OR; }
"!"             { update_position(yytext, yyleng); return '!'; }

"=>"            { update_position(yytext, yyleng); return ARROW; }
"??="           { update_position(yytext, yyleng); return NULL_COALESCE_ASSIGN; }
"??"            { update_position(yytext, yyleng); return NULL_COALESCE; }

"///".*         { update_position(yytext, yyleng); /* ignore XML doc comments */ }
"//".*          { update_position(yytext, yyleng); /* ignore */ }
"/*"            { BEGIN(COMMENT); }
<COMMENT>"*/"   { BEGIN(INITIAL); update_position(yytext, yyleng); }
<COMMENT>.|\n   { update_position(yytext, yyleng); /* ignore */ }

"#".*           { update_position(yytext, yyleng); /* ignore preprocessor directives */ }

. {
    fprintf(stderr, "Lexical error at line %d, column %d: unknown character '%s'\n", 
            line_num, column_num, yytext);
    update_position(yytext, yyleng);
    return ERROR;
}

%%