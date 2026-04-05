#include <stdio.h>
#include <stdlib.h>
#include "parser.tab.h"

extern FILE* yyin;
extern int has_errors;

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }
    
    FILE* file = fopen(argv[1], "r");
    if (!file) {
        fprintf(stderr, "Error: Cannot open file '%s'\n", argv[1]);
        return 1;
    }
    
    yyin = file;
    
    printf("Parsing file: %s\n", argv[1]);
    printf("----------------------------------------\n");
    
    int result = yyparse();
    
    fclose(file);
    
    printf("----------------------------------------\n");
    if (has_errors || result != 0) {
        printf("RESULT: INVALID (syntax errors found)\n");
        return 1;
    } else {
        printf("RESULT: VALID (no syntax errors)\n");
        return 0;
    }
}