%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.tab.h"

extern int yylineno;
int column = 1;

void update_column(const char* text, int len) {
    int i;
    for (i = 0; i < len; i++) {
        if (text[i] == '\n') {
            column = 1;
            yylineno++;  // Увеличиваем номер строки
        } else {
            column++;
        }
    }
}
%}

%option noyywrap
%option yylineno

%%

[ \t\r]+        { update_column(yytext, yyleng); /* ignore whitespace */ }
\n              { update_column(yytext, yyleng); /* ignore newlines but count them */ }

"class"         { update_column(yytext, yyleng); return CLASS; }
"int"           { update_column(yytext, yyleng); return INT; }
"return"        { update_column(yytext, yyleng); return RETURN; }
"public"        { update_column(yytext, yyleng); return PUBLIC; }
"private"       { update_column(yytext, yyleng); return PRIVATE; }
"static"        { update_column(yytext, yyleng); return STATIC; }
"void"          { update_column(yytext, yyleng); return VOID; }

[a-zA-Z_][a-zA-Z0-9_]* {
    update_column(yytext, yyleng);
    yylval.string = strdup(yytext);
    return IDENTIFIER;
}

[0-9]+ {
    update_column(yytext, yyleng);
    yylval.num = atoi(yytext);
    return INT_LITERAL;
}

"{"             { update_column(yytext, yyleng); return '{'; }
"}"             { update_column(yytext, yyleng); return '}'; }
"("             { update_column(yytext, yyleng); return '('; }
")"             { update_column(yytext, yyleng); return ')'; }
";"             { update_column(yytext, yyleng); return ';'; }
"="             { update_column(yytext, yyleng); return '='; }
"+"             { update_column(yytext, yyleng); return '+'; }
"-"             { update_column(yytext, yyleng); return '-'; }
"*"             { update_column(yytext, yyleng); return '*'; }
"/"             { update_column(yytext, yyleng); return '/'; }
","             { update_column(yytext, yyleng); return ','; }

"//".*          { update_column(yytext, yyleng); /* ignore */ }
"/*"([^*]|\*+[^*/])*"*/" { update_column(yytext, yyleng); /* ignore */ }

. {
    fprintf(stderr, "Unknown character at line %d, column %d: '%s'\n", 
            yylineno, column, yytext);
    update_column(yytext, yyleng);
    return ERROR;
}

%%