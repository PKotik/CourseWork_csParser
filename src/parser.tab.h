/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    INT_LITERAL = 258,             /* INT_LITERAL  */
    IDENTIFIER = 259,              /* IDENTIFIER  */
    ABSTRACT = 260,                /* ABSTRACT  */
    ASYNC = 261,                   /* ASYNC  */
    AWAIT = 262,                   /* AWAIT  */
    BOOL = 263,                    /* BOOL  */
    BREAK = 264,                   /* BREAK  */
    CASE = 265,                    /* CASE  */
    CATCH = 266,                   /* CATCH  */
    CLASS = 267,                   /* CLASS  */
    CONTINUE = 268,                /* CONTINUE  */
    DEFAULT = 269,                 /* DEFAULT  */
    ELSE = 270,                    /* ELSE  */
    FALSE = 271,                   /* FALSE  */
    FINALLY = 272,                 /* FINALLY  */
    FOR = 273,                     /* FOR  */
    FOREACH = 274,                 /* FOREACH  */
    GET = 275,                     /* GET  */
    IF = 276,                      /* IF  */
    INT = 277,                     /* INT  */
    INTERNAL = 278,                /* INTERNAL  */
    NAMESPACE = 279,               /* NAMESPACE  */
    NEW = 280,                     /* NEW  */
    NULL_ = 281,                   /* NULL_  */
    OBJECT = 282,                  /* OBJECT  */
    OVERRIDE = 283,                /* OVERRIDE  */
    PRIVATE = 284,                 /* PRIVATE  */
    PROTECTED = 285,               /* PROTECTED  */
    PUBLIC = 286,                  /* PUBLIC  */
    RETURN = 287,                  /* RETURN  */
    SEALED = 288,                  /* SEALED  */
    SET = 289,                     /* SET  */
    STATIC = 290,                  /* STATIC  */
    STRING = 291,                  /* STRING  */
    SWITCH = 292,                  /* SWITCH  */
    THROW = 293,                   /* THROW  */
    TRUE = 294,                    /* TRUE  */
    TRY = 295,                     /* TRY  */
    USING = 296,                   /* USING  */
    VAR = 297,                     /* VAR  */
    VIRTUAL = 298,                 /* VIRTUAL  */
    VOID = 299,                    /* VOID  */
    WHILE = 300,                   /* WHILE  */
    EQ_EQ = 301,                   /* EQ_EQ  */
    NE = 302,                      /* NE  */
    LE = 303,                      /* LE  */
    GE = 304,                      /* GE  */
    AND_AND = 305,                 /* AND_AND  */
    OR_OR = 306,                   /* OR_OR  */
    ARROW = 307,                   /* ARROW  */
    NULL_COALESCE_ASSIGN = 308,    /* NULL_COALESCE_ASSIGN  */
    NULL_COALESCE = 309,           /* NULL_COALESCE  */
    ERROR = 310,                   /* ERROR  */
    TASK = 311,                    /* TASK  */
    UMINUS = 312                   /* UMINUS  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 79 "parser.y"

    int num;
    char* string;
    struct ExprInfo {
        char* type;
    } expr;

#line 129 "parser.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */
