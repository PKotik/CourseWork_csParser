/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser.y"

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
    return type;
}

#line 147 "parser.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_CLASS = 3,                      /* CLASS  */
  YYSYMBOL_INT = 4,                        /* INT  */
  YYSYMBOL_STRING = 5,                     /* STRING  */
  YYSYMBOL_RETURN = 6,                     /* RETURN  */
  YYSYMBOL_PUBLIC = 7,                     /* PUBLIC  */
  YYSYMBOL_PRIVATE = 8,                    /* PRIVATE  */
  YYSYMBOL_STATIC = 9,                     /* STATIC  */
  YYSYMBOL_VOID = 10,                      /* VOID  */
  YYSYMBOL_IF = 11,                        /* IF  */
  YYSYMBOL_ELSE = 12,                      /* ELSE  */
  YYSYMBOL_WHILE = 13,                     /* WHILE  */
  YYSYMBOL_ERROR = 14,                     /* ERROR  */
  YYSYMBOL_INT_LITERAL = 15,               /* INT_LITERAL  */
  YYSYMBOL_IDENTIFIER = 16,                /* IDENTIFIER  */
  YYSYMBOL_EQ_EQ = 17,                     /* EQ_EQ  */
  YYSYMBOL_NE = 18,                        /* NE  */
  YYSYMBOL_LE = 19,                        /* LE  */
  YYSYMBOL_GE = 20,                        /* GE  */
  YYSYMBOL_AND_AND = 21,                   /* AND_AND  */
  YYSYMBOL_OR_OR = 22,                     /* OR_OR  */
  YYSYMBOL_NAMESPACE = 23,                 /* NAMESPACE  */
  YYSYMBOL_USING = 24,                     /* USING  */
  YYSYMBOL_ASYNC = 25,                     /* ASYNC  */
  YYSYMBOL_AWAIT = 26,                     /* AWAIT  */
  YYSYMBOL_VAR = 27,                       /* VAR  */
  YYSYMBOL_TASK = 28,                      /* TASK  */
  YYSYMBOL_TRUE = 29,                      /* TRUE  */
  YYSYMBOL_FALSE = 30,                     /* FALSE  */
  YYSYMBOL_NULL_ = 31,                     /* NULL_  */
  YYSYMBOL_NEW = 32,                       /* NEW  */
  YYSYMBOL_ARROW = 33,                     /* ARROW  */
  YYSYMBOL_NULL_COALESCE_ASSIGN = 34,      /* NULL_COALESCE_ASSIGN  */
  YYSYMBOL_NULL_COALESCE = 35,             /* NULL_COALESCE  */
  YYSYMBOL_36_ = 36,                       /* '<'  */
  YYSYMBOL_37_ = 37,                       /* '>'  */
  YYSYMBOL_38_ = 38,                       /* '!'  */
  YYSYMBOL_39_ = 39,                       /* '+'  */
  YYSYMBOL_40_ = 40,                       /* '-'  */
  YYSYMBOL_41_ = 41,                       /* '*'  */
  YYSYMBOL_42_ = 42,                       /* '/'  */
  YYSYMBOL_UMINUS = 43,                    /* UMINUS  */
  YYSYMBOL_44_ = 44,                       /* '.'  */
  YYSYMBOL_45_ = 45,                       /* ';'  */
  YYSYMBOL_46_ = 46,                       /* '='  */
  YYSYMBOL_47_ = 47,                       /* '{'  */
  YYSYMBOL_48_ = 48,                       /* '}'  */
  YYSYMBOL_49_ = 49,                       /* '('  */
  YYSYMBOL_50_ = 50,                       /* ')'  */
  YYSYMBOL_51_ = 51,                       /* ':'  */
  YYSYMBOL_52_ = 52,                       /* ','  */
  YYSYMBOL_53_ = 53,                       /* '|'  */
  YYSYMBOL_YYACCEPT = 54,                  /* $accept  */
  YYSYMBOL_qualified_identifier = 55,      /* qualified_identifier  */
  YYSYMBOL_program = 56,                   /* program  */
  YYSYMBOL_using_directive = 57,           /* using_directive  */
  YYSYMBOL_namespace_declaration = 58,     /* namespace_declaration  */
  YYSYMBOL_block_namespace = 59,           /* block_namespace  */
  YYSYMBOL_file_scoped_namespace = 60,     /* file_scoped_namespace  */
  YYSYMBOL_class_declaration = 61,         /* class_declaration  */
  YYSYMBOL_class_primary_constructor = 62, /* class_primary_constructor  */
  YYSYMBOL_class_inheritance = 63,         /* class_inheritance  */
  YYSYMBOL_class_modifiers = 64,           /* class_modifiers  */
  YYSYMBOL_class_body = 65,                /* class_body  */
  YYSYMBOL_class_members = 66,             /* class_members  */
  YYSYMBOL_class_member = 67,              /* class_member  */
  YYSYMBOL_field_declaration = 68,         /* field_declaration  */
  YYSYMBOL_method_declaration = 69,        /* method_declaration  */
  YYSYMBOL_70_1 = 70,                      /* $@1  */
  YYSYMBOL_type = 71,                      /* type  */
  YYSYMBOL_parameter_list = 72,            /* parameter_list  */
  YYSYMBOL_parameter = 73,                 /* parameter  */
  YYSYMBOL_method_body = 74,               /* method_body  */
  YYSYMBOL_block = 75,                     /* block  */
  YYSYMBOL_statements = 76,                /* statements  */
  YYSYMBOL_statement = 77,                 /* statement  */
  YYSYMBOL_declaration_statement = 78,     /* declaration_statement  */
  YYSYMBOL_assignment_statement = 79,      /* assignment_statement  */
  YYSYMBOL_return_statement = 80,          /* return_statement  */
  YYSYMBOL_expression = 81,                /* expression  */
  YYSYMBOL_argument_list = 82              /* argument_list  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   406

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  54
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  29
/* YYNRULES -- Number of rules.  */
#define YYNRULES  89
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  167

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   291


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    38,     2,     2,     2,     2,     2,     2,
      49,    50,    41,    39,    52,    40,    44,    42,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    51,    45,
      36,    46,    37,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    47,    53,    48,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    43
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   109,   109,   110,   113,   115,   116,   117,   121,   125,
     133,   134,   142,   149,   156,   164,   166,   172,   174,   179,
     185,   187,   188,   189,   193,   196,   198,   202,   203,   207,
     214,   221,   228,   239,   238,   265,   266,   267,   268,   271,
     273,   274,   278,   288,   292,   296,   299,   301,   305,   306,
     307,   308,   309,   318,   327,   339,   345,   361,   380,   389,
     408,   412,   422,   435,   436,   437,   447,   452,   461,   462,
     463,   464,   465,   466,   472,   473,   479,   486,   490,   494,
     498,   503,   508,   513,   518,   524,   530,   543,   545,   546
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "CLASS", "INT",
  "STRING", "RETURN", "PUBLIC", "PRIVATE", "STATIC", "VOID", "IF", "ELSE",
  "WHILE", "ERROR", "INT_LITERAL", "IDENTIFIER", "EQ_EQ", "NE", "LE", "GE",
  "AND_AND", "OR_OR", "NAMESPACE", "USING", "ASYNC", "AWAIT", "VAR",
  "TASK", "TRUE", "FALSE", "NULL_", "NEW", "ARROW", "NULL_COALESCE_ASSIGN",
  "NULL_COALESCE", "'<'", "'>'", "'!'", "'+'", "'-'", "'*'", "'/'",
  "UMINUS", "'.'", "';'", "'='", "'{'", "'}'", "'('", "')'", "':'", "','",
  "'|'", "$accept", "qualified_identifier", "program", "using_directive",
  "namespace_declaration", "block_namespace", "file_scoped_namespace",
  "class_declaration", "class_primary_constructor", "class_inheritance",
  "class_modifiers", "class_body", "class_members", "class_member",
  "field_declaration", "method_declaration", "$@1", "type",
  "parameter_list", "parameter", "method_body", "block", "statements",
  "statement", "declaration_statement", "assignment_statement",
  "return_statement", "expression", "argument_list", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-144)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -144,     6,  -144,    -5,    -2,  -144,  -144,  -144,  -144,  -144,
      66,  -144,    32,     3,    72,    56,  -144,  -144,  -144,    62,
    -144,  -144,    -5,  -144,    33,  -144,   -22,    98,    12,    40,
    -144,  -144,  -144,  -144,  -144,  -144,    87,    34,  -144,   103,
      74,  -144,  -144,    12,  -144,    82,  -144,  -144,  -144,    47,
      12,    12,    12,  -144,  -144,  -144,  -144,   115,   124,   130,
     131,   -18,   104,   105,   110,  -144,    12,  -144,  -144,  -144,
      44,  -144,    67,  -144,  -144,  -144,  -144,    -1,   331,   111,
     113,   102,  -144,   147,  -144,  -144,  -144,  -144,  -144,  -144,
    -144,   357,  -144,  -144,  -144,   148,   357,   357,  -144,   357,
      88,   357,   357,   357,    99,   286,   116,    58,    27,   117,
     357,   357,   357,   357,   357,   357,   357,   357,   357,   357,
     357,   357,   357,   357,   150,  -144,   357,   154,   191,   228,
    -144,   357,   357,  -144,    58,    58,    58,    58,   314,   314,
     286,   286,    58,    58,   340,   340,    27,    27,   119,   286,
      54,    54,  -144,   257,   286,    63,   357,   157,  -144,  -144,
    -144,   357,    68,    54,   286,  -144,  -144
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       4,    20,     1,     0,     0,     5,     6,    10,    11,     7,
       0,     2,     0,     2,     0,     0,    21,    22,    23,     0,
      13,     4,     0,     8,    15,     3,    20,     0,    39,    17,
      12,     9,    35,    36,    37,    38,     0,     0,    40,     0,
       0,    42,    16,     0,     2,    19,    25,    14,    41,     0,
       0,     0,     0,    24,    26,    27,    28,     0,     0,     0,
       0,     0,     0,     0,     0,    29,    39,    30,    31,    32,
       0,    33,     0,    43,    46,    34,    44,     0,     0,     0,
       0,    38,    45,     0,    51,    47,    48,    49,    50,    60,
      61,     0,    77,    78,    79,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,    82,     0,    76,    67,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    59,     0,     0,     0,     0,
      55,     0,    87,    66,    72,    73,    70,    71,    74,    75,
      85,    84,    68,    69,    62,    63,    64,    65,    80,    86,
       0,     0,    57,     0,    88,     0,    87,    52,    54,    56,
      83,     0,     0,     0,    89,    81,    53
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -144,    46,   156,  -144,  -144,  -144,  -144,  -144,  -144,  -144,
    -144,  -144,  -144,  -144,  -144,  -144,  -144,    38,   112,   136,
    -144,   108,  -144,  -143,  -144,  -144,  -144,   -78,    25
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,    12,     1,     5,     6,     7,     8,     9,    29,    40,
      10,    47,    49,    54,    55,    56,    72,    83,    37,    38,
      75,    84,    77,    85,    86,    87,    88,   154,   155
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
     100,     3,     4,    32,    33,    78,     2,   157,   158,    34,
      79,    11,    80,   105,    13,    81,    32,    33,   107,   108,
     166,   109,    34,   127,   128,   129,    30,    65,    35,     3,
       4,    66,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,    74,    82,   149,    22,
      14,    32,    33,   153,    50,    51,    52,    34,    32,    33,
      78,   116,   117,    35,    34,    79,    36,    80,    27,    15,
      81,   124,    24,    16,    17,    18,    19,    20,    25,    21,
     126,    36,    28,   164,    42,    45,    43,    57,    58,    59,
      60,    39,   116,   117,    71,    53,    43,   120,   121,   122,
     123,    74,   124,    41,    36,   110,   111,   112,   113,   114,
     115,   126,    73,   160,    74,   161,    19,    23,   165,    44,
     161,    46,   116,   117,   118,   119,    19,   120,   121,   122,
     123,    61,   124,   125,   110,   111,   112,   113,   114,   115,
      62,   126,    19,    31,   130,   131,    63,    64,   103,    67,
      68,   116,   117,   118,   119,    69,   120,   121,   122,   123,
     101,   124,   102,   104,   106,   132,   148,   133,   156,   163,
     126,   110,   111,   112,   113,   114,   115,    26,    70,    48,
      76,   162,     0,     0,     0,     0,     0,     0,   116,   117,
     118,   119,     0,   120,   121,   122,   123,     0,   124,     0,
       0,     0,     0,     0,   150,     0,     0,   126,   110,   111,
     112,   113,   114,   115,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   116,   117,   118,   119,     0,
     120,   121,   122,   123,     0,   124,     0,     0,     0,     0,
       0,   151,     0,     0,   126,   110,   111,   112,   113,   114,
     115,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   116,   117,   118,   119,     0,   120,   121,   122,
     123,     0,   124,   152,   110,   111,   112,   113,   114,   115,
       0,   126,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   116,   117,   118,   119,     0,   120,   121,   122,   123,
       0,   124,   159,   110,   111,   112,   113,   114,   115,     0,
     126,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     116,   117,   118,   119,     0,   120,   121,   122,   123,     0,
     124,   110,   111,   112,   113,     0,     0,     0,     0,   126,
       0,     0,     0,     0,     0,     0,    89,    90,   116,   117,
     118,   119,     0,   120,   121,   122,   123,    91,   124,     0,
      92,    93,    94,    95,     0,     0,     0,   126,     0,    96,
       0,    97,    89,    90,   116,   117,    98,     0,     0,     0,
      99,   122,   123,    91,   124,     0,    92,    93,    94,    95,
       0,     0,     0,   126,     0,    96,     0,    97,     0,     0,
       0,     0,     0,     0,     0,     0,    99
};

static const yytype_int16 yycheck[] =
{
      78,    23,    24,     4,     5,     6,     0,   150,   151,    10,
      11,    16,    13,    91,    16,    16,     4,     5,    96,    97,
     163,    99,    10,   101,   102,   103,    48,    45,    16,    23,
      24,    49,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,    47,    48,   126,    46,
       4,     4,     5,   131,     7,     8,     9,    10,     4,     5,
       6,    34,    35,    16,    10,    11,    28,    13,    22,     3,
      16,    44,    16,     7,     8,     9,    44,    45,    16,    47,
      53,    43,    49,   161,    50,    39,    52,    49,    50,    51,
      52,    51,    34,    35,    50,    48,    52,    39,    40,    41,
      42,    47,    44,    16,    66,    17,    18,    19,    20,    21,
      22,    53,    45,    50,    47,    52,    44,    45,    50,    16,
      52,    47,    34,    35,    36,    37,    44,    39,    40,    41,
      42,    16,    44,    45,    17,    18,    19,    20,    21,    22,
      16,    53,    44,    45,    45,    46,    16,    16,    46,    45,
      45,    34,    35,    36,    37,    45,    39,    40,    41,    42,
      49,    44,    49,    16,    16,    49,    16,    50,    49,    12,
      53,    17,    18,    19,    20,    21,    22,    21,    66,    43,
      72,   156,    -1,    -1,    -1,    -1,    -1,    -1,    34,    35,
      36,    37,    -1,    39,    40,    41,    42,    -1,    44,    -1,
      -1,    -1,    -1,    -1,    50,    -1,    -1,    53,    17,    18,
      19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    35,    36,    37,    -1,
      39,    40,    41,    42,    -1,    44,    -1,    -1,    -1,    -1,
      -1,    50,    -1,    -1,    53,    17,    18,    19,    20,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    35,    36,    37,    -1,    39,    40,    41,
      42,    -1,    44,    45,    17,    18,    19,    20,    21,    22,
      -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    35,    36,    37,    -1,    39,    40,    41,    42,
      -1,    44,    45,    17,    18,    19,    20,    21,    22,    -1,
      53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    35,    36,    37,    -1,    39,    40,    41,    42,    -1,
      44,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    53,
      -1,    -1,    -1,    -1,    -1,    -1,    15,    16,    34,    35,
      36,    37,    -1,    39,    40,    41,    42,    26,    44,    -1,
      29,    30,    31,    32,    -1,    -1,    -1,    53,    -1,    38,
      -1,    40,    15,    16,    34,    35,    45,    -1,    -1,    -1,
      49,    41,    42,    26,    44,    -1,    29,    30,    31,    32,
      -1,    -1,    -1,    53,    -1,    38,    -1,    40,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    49
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    56,     0,    23,    24,    57,    58,    59,    60,    61,
      64,    16,    55,    16,    55,     3,     7,     8,     9,    44,
      45,    47,    46,    45,    16,    16,    56,    55,    49,    62,
      48,    45,     4,     5,    10,    16,    71,    72,    73,    51,
      63,    16,    50,    52,    16,    55,    47,    65,    73,    66,
       7,     8,     9,    48,    67,    68,    69,    71,    71,    71,
      71,    16,    16,    16,    16,    45,    49,    45,    45,    45,
      72,    50,    70,    45,    47,    74,    75,    76,     6,    11,
      13,    16,    48,    71,    75,    77,    78,    79,    80,    15,
      16,    26,    29,    30,    31,    32,    38,    40,    45,    49,
      81,    49,    49,    46,    16,    81,    16,    81,    81,    81,
      17,    18,    19,    20,    21,    22,    34,    35,    36,    37,
      39,    40,    41,    42,    44,    45,    53,    81,    81,    81,
      45,    46,    49,    50,    81,    81,    81,    81,    81,    81,
      81,    81,    81,    81,    81,    81,    81,    81,    16,    81,
      50,    50,    45,    81,    81,    82,    49,    77,    77,    45,
      50,    52,    82,    12,    81,    50,    77
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    54,    55,    55,    56,    56,    56,    56,    57,    57,
      58,    58,    59,    60,    61,    62,    62,    63,    63,    63,
      64,    64,    64,    64,    65,    66,    66,    67,    67,    68,
      68,    68,    68,    70,    69,    71,    71,    71,    71,    72,
      72,    72,    73,    74,    74,    75,    76,    76,    77,    77,
      77,    77,    77,    77,    77,    78,    78,    79,    80,    80,
      81,    81,    81,    81,    81,    81,    81,    81,    81,    81,
      81,    81,    81,    81,    81,    81,    81,    81,    81,    81,
      81,    81,    81,    81,    81,    81,    81,    82,    82,    82
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     3,     0,     2,     2,     2,     3,     5,
       1,     1,     5,     3,     6,     0,     3,     0,     2,     2,
       0,     2,     2,     2,     3,     0,     2,     1,     1,     3,
       4,     4,     4,     0,     7,     1,     1,     1,     1,     0,
       1,     3,     2,     1,     1,     3,     0,     2,     1,     1,
       1,     1,     5,     7,     5,     3,     5,     4,     2,     3,
       1,     1,     3,     3,     3,     3,     3,     2,     3,     3,
       3,     3,     3,     3,     3,     3,     2,     1,     1,     1,
       3,     6,     2,     5,     3,     3,     3,     0,     1,     3
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 8: /* using_directive: USING qualified_identifier ';'  */
#line 122 "parser.y"
    {
        printf("Using directive\n");
    }
#line 1377 "parser.tab.c"
    break;

  case 9: /* using_directive: USING IDENTIFIER '=' qualified_identifier ';'  */
#line 126 "parser.y"
    {
        printf("Using alias: %s\n", (yyvsp[-3].string));
        free((yyvsp[-3].string));
    }
#line 1386 "parser.tab.c"
    break;

  case 12: /* block_namespace: NAMESPACE qualified_identifier '{' program '}'  */
#line 143 "parser.y"
    {
        printf("Block namespace\n");
    }
#line 1394 "parser.tab.c"
    break;

  case 13: /* file_scoped_namespace: NAMESPACE qualified_identifier ';'  */
#line 150 "parser.y"
    {
        printf("File-scoped namespace\n");
    }
#line 1402 "parser.tab.c"
    break;

  case 14: /* class_declaration: class_modifiers CLASS IDENTIFIER class_primary_constructor class_inheritance class_body  */
#line 157 "parser.y"
    {
        printf("Valid class: %s\n", (yyvsp[-3].string));
        free((yyvsp[-3].string));
        clear_symbols();
    }
#line 1412 "parser.tab.c"
    break;

  case 16: /* class_primary_constructor: '(' parameter_list ')'  */
#line 167 "parser.y"
    {
        printf("  Primary constructor\n");
    }
#line 1420 "parser.tab.c"
    break;

  case 18: /* class_inheritance: ':' IDENTIFIER  */
#line 175 "parser.y"
    {
        printf("  Inherits from: %s\n", (yyvsp[0].string));
        free((yyvsp[0].string));
    }
#line 1429 "parser.tab.c"
    break;

  case 19: /* class_inheritance: ':' qualified_identifier  */
#line 180 "parser.y"
    {
        printf("  Inherits from\n");
    }
#line 1437 "parser.tab.c"
    break;

  case 29: /* field_declaration: type IDENTIFIER ';'  */
#line 208 "parser.y"
    {
        printf("  Field: %s\n", (yyvsp[-1].string));
        add_symbol((yyvsp[-1].string), (yyvsp[-2].string));
        free((yyvsp[-1].string));
        free((yyvsp[-2].string));
    }
#line 1448 "parser.tab.c"
    break;

  case 30: /* field_declaration: PUBLIC type IDENTIFIER ';'  */
#line 215 "parser.y"
    {
        printf("  Field (public): %s\n", (yyvsp[-1].string));
        add_symbol((yyvsp[-1].string), (yyvsp[-2].string));
        free((yyvsp[-1].string));
        free((yyvsp[-2].string));
    }
#line 1459 "parser.tab.c"
    break;

  case 31: /* field_declaration: PRIVATE type IDENTIFIER ';'  */
#line 222 "parser.y"
    {
        printf("  Field (private): %s\n", (yyvsp[-1].string));
        add_symbol((yyvsp[-1].string), (yyvsp[-2].string));
        free((yyvsp[-1].string));
        free((yyvsp[-2].string));
    }
#line 1470 "parser.tab.c"
    break;

  case 32: /* field_declaration: STATIC type IDENTIFIER ';'  */
#line 229 "parser.y"
    {
        printf("  Field (static): %s\n", (yyvsp[-1].string));
        add_symbol((yyvsp[-1].string), (yyvsp[-2].string));
        free((yyvsp[-1].string));
        free((yyvsp[-2].string));
    }
#line 1481 "parser.tab.c"
    break;

  case 33: /* $@1: %empty  */
#line 239 "parser.y"
    {
        // Сохраняем тип и имя метода ДО того, как парсим тело
        current_method_type = strdup((yyvsp[-4].string));
        current_method_name = strdup((yyvsp[-3].string));
        current_method_has_return = 0;
        printf("  Method: %s (return type: %s)\n", (yyvsp[-3].string), (yyvsp[-4].string));
        free((yyvsp[-3].string));
        free((yyvsp[-4].string));
    }
#line 1495 "parser.tab.c"
    break;

  case 34: /* method_declaration: type IDENTIFIER '(' parameter_list ')' $@1 method_body  */
#line 249 "parser.y"
    {
        // После тела метода проверяем, есть ли return
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
#line 1513 "parser.tab.c"
    break;

  case 35: /* type: INT  */
#line 265 "parser.y"
        { (yyval.string) = strdup("int"); }
#line 1519 "parser.tab.c"
    break;

  case 36: /* type: STRING  */
#line 266 "parser.y"
             { (yyval.string) = strdup("string"); }
#line 1525 "parser.tab.c"
    break;

  case 37: /* type: VOID  */
#line 267 "parser.y"
           { (yyval.string) = strdup("void"); }
#line 1531 "parser.tab.c"
    break;

  case 38: /* type: IDENTIFIER  */
#line 268 "parser.y"
                 { (yyval.string) = (yyvsp[0].string); }
#line 1537 "parser.tab.c"
    break;

  case 42: /* parameter: type IDENTIFIER  */
#line 279 "parser.y"
    {
        add_symbol((yyvsp[0].string), (yyvsp[-1].string));
        printf("    Parameter: %s %s\n", (yyvsp[-1].string), (yyvsp[0].string));
        free((yyvsp[0].string));
        free((yyvsp[-1].string));
    }
#line 1548 "parser.tab.c"
    break;

  case 43: /* method_body: ';'  */
#line 289 "parser.y"
    {
        // Абстрактный метод
    }
#line 1556 "parser.tab.c"
    break;

  case 52: /* statement: IF '(' expression ')' statement  */
#line 310 "parser.y"
    {
        if (strcmp((yyvsp[-2].expr).type, "int") != 0 && strcmp((yyvsp[-2].expr).type, "bool") != 0 && strcmp((yyvsp[-2].expr).type, "unknown") != 0) {
            char err[256];
            snprintf(err, sizeof(err), "Condition must be bool or int, got '%s'", (yyvsp[-2].expr).type);
            yyerror(err);
        }
        free((yyvsp[-2].expr).type);
    }
#line 1569 "parser.tab.c"
    break;

  case 53: /* statement: IF '(' expression ')' statement ELSE statement  */
#line 319 "parser.y"
    {
        if (strcmp((yyvsp[-4].expr).type, "int") != 0 && strcmp((yyvsp[-4].expr).type, "bool") != 0 && strcmp((yyvsp[-4].expr).type, "unknown") != 0) {
            char err[256];
            snprintf(err, sizeof(err), "Condition must be bool or int, got '%s'", (yyvsp[-4].expr).type);
            yyerror(err);
        }
        free((yyvsp[-4].expr).type);
    }
#line 1582 "parser.tab.c"
    break;

  case 54: /* statement: WHILE '(' expression ')' statement  */
#line 328 "parser.y"
    {
        if (strcmp((yyvsp[-2].expr).type, "int") != 0 && strcmp((yyvsp[-2].expr).type, "bool") != 0 && strcmp((yyvsp[-2].expr).type, "unknown") != 0) {
            char err[256];
            snprintf(err, sizeof(err), "Condition must be bool or int, got '%s'", (yyvsp[-2].expr).type);
            yyerror(err);
        }
        free((yyvsp[-2].expr).type);
    }
#line 1595 "parser.tab.c"
    break;

  case 55: /* declaration_statement: type IDENTIFIER ';'  */
#line 340 "parser.y"
    {
        add_symbol((yyvsp[-1].string), (yyvsp[-2].string));
        free((yyvsp[-1].string));
        free((yyvsp[-2].string));
    }
#line 1605 "parser.tab.c"
    break;

  case 56: /* declaration_statement: type IDENTIFIER '=' expression ';'  */
#line 346 "parser.y"
    {
        if (strcmp((yyvsp[-1].expr).type, "unknown") != 0 && !is_type_compatible((yyvsp[-4].string), (yyvsp[-1].expr).type)) {
            char err[256];
            snprintf(err, sizeof(err), "Cannot assign '%s' to '%s'", 
                     get_type_name((yyvsp[-1].expr).type), get_type_name((yyvsp[-4].string)));
            yyerror(err);
        }
        add_symbol((yyvsp[-3].string), (yyvsp[-4].string));
        free((yyvsp[-3].string));
        free((yyvsp[-4].string));
        free((yyvsp[-1].expr).type);
    }
#line 1622 "parser.tab.c"
    break;

  case 57: /* assignment_statement: IDENTIFIER '=' expression ';'  */
#line 362 "parser.y"
    {
        Symbol* sym = find_symbol((yyvsp[-3].string));
        if (!sym) {
            char err[256];
            snprintf(err, sizeof(err), "Variable '%s' not declared", (yyvsp[-3].string));
            yyerror(err);
        } else if (strcmp((yyvsp[-1].expr).type, "unknown") != 0 && !is_type_compatible(sym->type, (yyvsp[-1].expr).type)) {
            char err[256];
            snprintf(err, sizeof(err), "Cannot assign '%s' to '%s'", 
                     get_type_name((yyvsp[-1].expr).type), get_type_name(sym->type));
            yyerror(err);
        }
        free((yyvsp[-3].string));
        free((yyvsp[-1].expr).type);
    }
#line 1642 "parser.tab.c"
    break;

  case 58: /* return_statement: RETURN ';'  */
#line 381 "parser.y"
    {
        if (current_method_type && strcmp(current_method_type, "void") != 0) {
            char err[256];
            snprintf(err, sizeof(err), "Non-void method '%s' must return a value", current_method_name);
            yyerror(err);
        }
        current_method_has_return = 1;
    }
#line 1655 "parser.tab.c"
    break;

  case 59: /* return_statement: RETURN expression ';'  */
#line 390 "parser.y"
    {
        if (current_method_type && strcmp(current_method_type, "void") == 0) {
            char err[256];
            snprintf(err, sizeof(err), "Void method '%s' cannot return a value (got '%s')", 
                     current_method_name, get_type_name((yyvsp[-1].expr).type));
            yyerror(err);
        } else if (current_method_type && strcmp((yyvsp[-1].expr).type, "unknown") != 0 && !is_type_compatible(current_method_type, (yyvsp[-1].expr).type)) {
            char err[256];
            snprintf(err, sizeof(err), "Return type mismatch in '%s': expected '%s', got '%s'",
                     current_method_name, get_type_name(current_method_type), get_type_name((yyvsp[-1].expr).type));
            yyerror(err);
        }
        current_method_has_return = 1;
        free((yyvsp[-1].expr).type);
    }
#line 1675 "parser.tab.c"
    break;

  case 60: /* expression: INT_LITERAL  */
#line 409 "parser.y"
    {
        (yyval.expr).type = strdup("int");
    }
#line 1683 "parser.tab.c"
    break;

  case 61: /* expression: IDENTIFIER  */
#line 413 "parser.y"
    {
        Symbol* sym = find_symbol((yyvsp[0].string));
        if (!sym) {
            (yyval.expr).type = strdup("unknown");
        } else {
            (yyval.expr).type = strdup(sym->type);
        }
        free((yyvsp[0].string));
    }
#line 1697 "parser.tab.c"
    break;

  case 62: /* expression: expression '+' expression  */
#line 423 "parser.y"
    {
        if (strcmp((yyvsp[-2].expr).type, "int") == 0 && strcmp((yyvsp[0].expr).type, "int") == 0) {
            (yyval.expr).type = strdup("int");
        } else if ((strcmp((yyvsp[-2].expr).type, "string") == 0 || strcmp((yyvsp[0].expr).type, "string") == 0) &&
                   (strcmp((yyvsp[-2].expr).type, "unknown") != 0 && strcmp((yyvsp[0].expr).type, "unknown") != 0)) {
            (yyval.expr).type = strdup("string");
        } else {
            (yyval.expr).type = strdup("unknown");
        }
        free((yyvsp[-2].expr).type);
        free((yyvsp[0].expr).type);
    }
#line 1714 "parser.tab.c"
    break;

  case 65: /* expression: expression '/' expression  */
#line 438 "parser.y"
    {
        if (strcmp((yyvsp[-2].expr).type, "int") == 0 && strcmp((yyvsp[0].expr).type, "int") == 0) {
            (yyval.expr).type = strdup("int");
        } else {
            (yyval.expr).type = strdup("unknown");
        }
        free((yyvsp[-2].expr).type);
        free((yyvsp[0].expr).type);
    }
#line 1728 "parser.tab.c"
    break;

  case 66: /* expression: '(' expression ')'  */
#line 448 "parser.y"
    {
        (yyval.expr).type = strdup((yyvsp[-1].expr).type);
        free((yyvsp[-1].expr).type);
    }
#line 1737 "parser.tab.c"
    break;

  case 67: /* expression: '-' expression  */
#line 453 "parser.y"
    {
        if (strcmp((yyvsp[0].expr).type, "int") == 0) {
            (yyval.expr).type = strdup("int");
        } else {
            (yyval.expr).type = strdup("unknown");
        }
        free((yyvsp[0].expr).type);
    }
#line 1750 "parser.tab.c"
    break;

  case 73: /* expression: expression NE expression  */
#line 467 "parser.y"
    {
        (yyval.expr).type = strdup("bool");
        free((yyvsp[-2].expr).type);
        free((yyvsp[0].expr).type);
    }
#line 1760 "parser.tab.c"
    break;

  case 75: /* expression: expression OR_OR expression  */
#line 474 "parser.y"
    {
        (yyval.expr).type = strdup("bool");
        free((yyvsp[-2].expr).type);
        free((yyvsp[0].expr).type);
    }
#line 1770 "parser.tab.c"
    break;

  case 76: /* expression: '!' expression  */
#line 480 "parser.y"
    {
        (yyval.expr).type = strdup("bool");
        free((yyvsp[0].expr).type);
    }
#line 1779 "parser.tab.c"
    break;

  case 77: /* expression: TRUE  */
#line 487 "parser.y"
    {
        (yyval.expr).type = strdup("bool");
    }
#line 1787 "parser.tab.c"
    break;

  case 78: /* expression: FALSE  */
#line 491 "parser.y"
    {
        (yyval.expr).type = strdup("bool");
    }
#line 1795 "parser.tab.c"
    break;

  case 79: /* expression: NULL_  */
#line 495 "parser.y"
    {
        (yyval.expr).type = strdup("null");
    }
#line 1803 "parser.tab.c"
    break;

  case 80: /* expression: expression '.' IDENTIFIER  */
#line 499 "parser.y"
    {
        (yyval.expr).type = strdup("unknown");
        free((yyvsp[0].string));
    }
#line 1812 "parser.tab.c"
    break;

  case 81: /* expression: expression '.' IDENTIFIER '(' argument_list ')'  */
#line 504 "parser.y"
    {
        (yyval.expr).type = strdup("unknown");
        free((yyvsp[-3].string));
    }
#line 1821 "parser.tab.c"
    break;

  case 82: /* expression: AWAIT expression  */
#line 509 "parser.y"
    {
        (yyval.expr).type = strdup((yyvsp[0].expr).type);
        free((yyvsp[0].expr).type);
    }
#line 1830 "parser.tab.c"
    break;

  case 83: /* expression: NEW IDENTIFIER '(' argument_list ')'  */
#line 514 "parser.y"
    {
        (yyval.expr).type = strdup((yyvsp[-3].string));
        free((yyvsp[-3].string));
    }
#line 1839 "parser.tab.c"
    break;

  case 84: /* expression: expression NULL_COALESCE expression  */
#line 519 "parser.y"
    {
        (yyval.expr).type = strdup((yyvsp[-2].expr).type);
        free((yyvsp[-2].expr).type);
        free((yyvsp[0].expr).type);
    }
#line 1849 "parser.tab.c"
    break;

  case 85: /* expression: expression NULL_COALESCE_ASSIGN expression  */
#line 525 "parser.y"
    {
        (yyval.expr).type = strdup((yyvsp[-2].expr).type);
        free((yyvsp[-2].expr).type);
        free((yyvsp[0].expr).type);
    }
#line 1859 "parser.tab.c"
    break;

  case 86: /* expression: expression '|' expression  */
#line 531 "parser.y"
    {
        if (strcmp((yyvsp[-2].expr).type, "int") == 0 && strcmp((yyvsp[0].expr).type, "int") == 0) {
            (yyval.expr).type = strdup("int");
        } else {
            (yyval.expr).type = strdup("unknown");
        }
        free((yyvsp[-2].expr).type);
        free((yyvsp[0].expr).type);
    }
#line 1873 "parser.tab.c"
    break;


#line 1877 "parser.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 549 "parser.y"


void yyerror(const char* msg) {
    fprintf(stderr, "Semantic error at line %d, column %d: %s\n", 
            line_num, column_num, msg);
    has_errors = 1;
}
