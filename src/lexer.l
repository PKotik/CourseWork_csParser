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

"class"         { update_position(yytext, yyleng); return CLASS; }
"int"           { update_position(yytext, yyleng); return INT; }
"string"        { update_position(yytext, yyleng); return STRING; }
"return"        { update_position(yytext, yyleng); return RETURN; }
"public"        { update_position(yytext, yyleng); return PUBLIC; }
"private"       { update_position(yytext, yyleng); return PRIVATE; }
"static"        { update_position(yytext, yyleng); return STATIC; }
"void"          { update_position(yytext, yyleng); return VOID; }
"if"            { update_position(yytext, yyleng); return IF; }
"else"          { update_position(yytext, yyleng); return ELSE; }
"while"         { update_position(yytext, yyleng); return WHILE; }

"namespace"     { update_position(yytext, yyleng); return NAMESPACE; }
"using"         { update_position(yytext, yyleng); return USING; }
"async"         { update_position(yytext, yyleng); return ASYNC; }
"await"         { update_position(yytext, yyleng); return AWAIT; }
"var"           { update_position(yytext, yyleng); return VAR; }
"Task"          { update_position(yytext, yyleng); return TASK; }
"true"          { update_position(yytext, yyleng); return TRUE; }
"false"         { update_position(yytext, yyleng); return FALSE; }
"null"          { update_position(yytext, yyleng); return NULL_; }
"new"           { update_position(yytext, yyleng); return NEW; }


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
";"             { update_position(yytext, yyleng); return ';'; }
"="             { update_position(yytext, yyleng); return '='; }
"+"             { update_position(yytext, yyleng); return '+'; }
"-"             { update_position(yytext, yyleng); return '-'; }
"*"             { update_position(yytext, yyleng); return '*'; }
"/"             { update_position(yytext, yyleng); return '/'; }
","             { update_position(yytext, yyleng); return ','; }
"."             { update_position(yytext, yyleng); return '.'; }
"|"             { update_position(yytext, yyleng); return '|'; }
"?"             { update_position(yytext, yyleng); return '?'; }

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
"#nullable".*   { update_position(yytext, yyleng); /* ignore #nullable directive */ }

"."             { update_position(yytext, yyleng); return '.'; }

. {
    fprintf(stderr, "Lexical error at line %d, column %d: unknown character '%s'\n", 
            line_num, column_num, yytext);
    update_position(yytext, yyleng);
    return ERROR;
}

%%