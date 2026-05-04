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

void print_semantic_error(const char* msg);
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

#line 149 "parser.tab.c"

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
  YYSYMBOL_INT_LITERAL = 3,                /* INT_LITERAL  */
  YYSYMBOL_IDENTIFIER = 4,                 /* IDENTIFIER  */
  YYSYMBOL_ABSTRACT = 5,                   /* ABSTRACT  */
  YYSYMBOL_ASYNC = 6,                      /* ASYNC  */
  YYSYMBOL_AWAIT = 7,                      /* AWAIT  */
  YYSYMBOL_BOOL = 8,                       /* BOOL  */
  YYSYMBOL_BREAK = 9,                      /* BREAK  */
  YYSYMBOL_CASE = 10,                      /* CASE  */
  YYSYMBOL_CATCH = 11,                     /* CATCH  */
  YYSYMBOL_CLASS = 12,                     /* CLASS  */
  YYSYMBOL_CONTINUE = 13,                  /* CONTINUE  */
  YYSYMBOL_DEFAULT = 14,                   /* DEFAULT  */
  YYSYMBOL_ELSE = 15,                      /* ELSE  */
  YYSYMBOL_FALSE = 16,                     /* FALSE  */
  YYSYMBOL_FINALLY = 17,                   /* FINALLY  */
  YYSYMBOL_FOR = 18,                       /* FOR  */
  YYSYMBOL_FOREACH = 19,                   /* FOREACH  */
  YYSYMBOL_GET = 20,                       /* GET  */
  YYSYMBOL_IF = 21,                        /* IF  */
  YYSYMBOL_INT = 22,                       /* INT  */
  YYSYMBOL_INTERNAL = 23,                  /* INTERNAL  */
  YYSYMBOL_NAMESPACE = 24,                 /* NAMESPACE  */
  YYSYMBOL_NEW = 25,                       /* NEW  */
  YYSYMBOL_NULL_ = 26,                     /* NULL_  */
  YYSYMBOL_OBJECT = 27,                    /* OBJECT  */
  YYSYMBOL_OVERRIDE = 28,                  /* OVERRIDE  */
  YYSYMBOL_PRIVATE = 29,                   /* PRIVATE  */
  YYSYMBOL_PROTECTED = 30,                 /* PROTECTED  */
  YYSYMBOL_PUBLIC = 31,                    /* PUBLIC  */
  YYSYMBOL_RETURN = 32,                    /* RETURN  */
  YYSYMBOL_SEALED = 33,                    /* SEALED  */
  YYSYMBOL_SET = 34,                       /* SET  */
  YYSYMBOL_STATIC = 35,                    /* STATIC  */
  YYSYMBOL_STRING = 36,                    /* STRING  */
  YYSYMBOL_SWITCH = 37,                    /* SWITCH  */
  YYSYMBOL_THROW = 38,                     /* THROW  */
  YYSYMBOL_TRUE = 39,                      /* TRUE  */
  YYSYMBOL_TRY = 40,                       /* TRY  */
  YYSYMBOL_USING = 41,                     /* USING  */
  YYSYMBOL_VAR = 42,                       /* VAR  */
  YYSYMBOL_VIRTUAL = 43,                   /* VIRTUAL  */
  YYSYMBOL_VOID = 44,                      /* VOID  */
  YYSYMBOL_WHILE = 45,                     /* WHILE  */
  YYSYMBOL_EQ_EQ = 46,                     /* EQ_EQ  */
  YYSYMBOL_NE = 47,                        /* NE  */
  YYSYMBOL_LE = 48,                        /* LE  */
  YYSYMBOL_GE = 49,                        /* GE  */
  YYSYMBOL_AND_AND = 50,                   /* AND_AND  */
  YYSYMBOL_OR_OR = 51,                     /* OR_OR  */
  YYSYMBOL_ARROW = 52,                     /* ARROW  */
  YYSYMBOL_NULL_COALESCE_ASSIGN = 53,      /* NULL_COALESCE_ASSIGN  */
  YYSYMBOL_NULL_COALESCE = 54,             /* NULL_COALESCE  */
  YYSYMBOL_ERROR = 55,                     /* ERROR  */
  YYSYMBOL_TASK = 56,                      /* TASK  */
  YYSYMBOL_57_ = 57,                       /* '<'  */
  YYSYMBOL_58_ = 58,                       /* '>'  */
  YYSYMBOL_59_ = 59,                       /* '!'  */
  YYSYMBOL_60_ = 60,                       /* '+'  */
  YYSYMBOL_61_ = 61,                       /* '-'  */
  YYSYMBOL_62_ = 62,                       /* '*'  */
  YYSYMBOL_63_ = 63,                       /* '/'  */
  YYSYMBOL_64_ = 64,                       /* '%'  */
  YYSYMBOL_UMINUS = 65,                    /* UMINUS  */
  YYSYMBOL_66_ = 66,                       /* '.'  */
  YYSYMBOL_67_ = 67,                       /* ';'  */
  YYSYMBOL_68_ = 68,                       /* '='  */
  YYSYMBOL_69_ = 69,                       /* '{'  */
  YYSYMBOL_70_ = 70,                       /* '}'  */
  YYSYMBOL_71_ = 71,                       /* '('  */
  YYSYMBOL_72_ = 72,                       /* ')'  */
  YYSYMBOL_73_ = 73,                       /* ':'  */
  YYSYMBOL_74_ = 74,                       /* '?'  */
  YYSYMBOL_75_ = 75,                       /* ','  */
  YYSYMBOL_76_ = 76,                       /* '|'  */
  YYSYMBOL_YYACCEPT = 77,                  /* $accept  */
  YYSYMBOL_qualified_identifier = 78,      /* qualified_identifier  */
  YYSYMBOL_program = 79,                   /* program  */
  YYSYMBOL_using_directive = 80,           /* using_directive  */
  YYSYMBOL_namespace_declaration = 81,     /* namespace_declaration  */
  YYSYMBOL_block_namespace = 82,           /* block_namespace  */
  YYSYMBOL_file_scoped_namespace = 83,     /* file_scoped_namespace  */
  YYSYMBOL_class_declaration = 84,         /* class_declaration  */
  YYSYMBOL_class_primary_constructor = 85, /* class_primary_constructor  */
  YYSYMBOL_class_inheritance = 86,         /* class_inheritance  */
  YYSYMBOL_class_modifiers = 87,           /* class_modifiers  */
  YYSYMBOL_class_body = 88,                /* class_body  */
  YYSYMBOL_class_members = 89,             /* class_members  */
  YYSYMBOL_class_member = 90,              /* class_member  */
  YYSYMBOL_field_declaration = 91,         /* field_declaration  */
  YYSYMBOL_method_declaration = 92,        /* method_declaration  */
  YYSYMBOL_93_1 = 93,                      /* $@1  */
  YYSYMBOL_94_2 = 94,                      /* $@2  */
  YYSYMBOL_95_3 = 95,                      /* $@3  */
  YYSYMBOL_96_4 = 96,                      /* $@4  */
  YYSYMBOL_property_declaration = 97,      /* property_declaration  */
  YYSYMBOL_accessors = 98,                 /* accessors  */
  YYSYMBOL_accessor = 99,                  /* accessor  */
  YYSYMBOL_modifiers = 100,                /* modifiers  */
  YYSYMBOL_type = 101,                     /* type  */
  YYSYMBOL_type_list = 102,                /* type_list  */
  YYSYMBOL_parameter_list = 103,           /* parameter_list  */
  YYSYMBOL_parameter = 104,                /* parameter  */
  YYSYMBOL_method_body = 105,              /* method_body  */
  YYSYMBOL_block = 106,                    /* block  */
  YYSYMBOL_statements = 107,               /* statements  */
  YYSYMBOL_statement = 108,                /* statement  */
  YYSYMBOL_declaration_statement = 109,    /* declaration_statement  */
  YYSYMBOL_assignment_statement = 110,     /* assignment_statement  */
  YYSYMBOL_return_statement = 111,         /* return_statement  */
  YYSYMBOL_expression = 112,               /* expression  */
  YYSYMBOL_argument_list = 113             /* argument_list  */
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
typedef yytype_int16 yy_state_t;

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
#define YYLAST   792

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  77
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  37
/* YYNRULES -- Number of rules.  */
#define YYNRULES  144
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  265

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   312


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
       2,     2,     2,    59,     2,     2,     2,    64,     2,     2,
      71,    72,    62,    60,    75,    61,    66,    63,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    73,    67,
      57,    68,    58,    74,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    69,    76,    70,     2,     2,     2,     2,
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
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    65
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   117,   117,   118,   121,   123,   124,   125,   129,   133,
     141,   142,   146,   153,   160,   168,   170,   176,   178,   184,
     186,   187,   188,   189,   190,   191,   192,   196,   199,   201,
     205,   206,   207,   211,   218,   229,   228,   251,   250,   273,
     272,   295,   294,   320,   327,   335,   337,   338,   342,   343,
     344,   345,   346,   347,   348,   349,   352,   354,   355,   356,
     357,   358,   359,   360,   361,   362,   363,   367,   368,   369,
     370,   371,   372,   373,   374,   380,   388,   396,   397,   406,
     407,   410,   412,   413,   417,   427,   432,   436,   439,   441,
     445,   446,   456,   466,   476,   477,   478,   479,   486,   493,
     516,   535,   544,   563,   567,   571,   575,   579,   589,   594,
     599,   604,   609,   622,   623,   624,   625,   635,   644,   645,
     646,   647,   648,   649,   655,   656,   662,   667,   677,   682,
     687,   692,   698,   703,   708,   713,   718,   723,   728,   733,
     737,   743,   751,   753,   754
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
  "\"end of file\"", "error", "\"invalid token\"", "INT_LITERAL",
  "IDENTIFIER", "ABSTRACT", "ASYNC", "AWAIT", "BOOL", "BREAK", "CASE",
  "CATCH", "CLASS", "CONTINUE", "DEFAULT", "ELSE", "FALSE", "FINALLY",
  "FOR", "FOREACH", "GET", "IF", "INT", "INTERNAL", "NAMESPACE", "NEW",
  "NULL_", "OBJECT", "OVERRIDE", "PRIVATE", "PROTECTED", "PUBLIC",
  "RETURN", "SEALED", "SET", "STATIC", "STRING", "SWITCH", "THROW", "TRUE",
  "TRY", "USING", "VAR", "VIRTUAL", "VOID", "WHILE", "EQ_EQ", "NE", "LE",
  "GE", "AND_AND", "OR_OR", "ARROW", "NULL_COALESCE_ASSIGN",
  "NULL_COALESCE", "ERROR", "TASK", "'<'", "'>'", "'!'", "'+'", "'-'",
  "'*'", "'/'", "'%'", "UMINUS", "'.'", "';'", "'='", "'{'", "'}'", "'('",
  "')'", "':'", "'?'", "','", "'|'", "$accept", "qualified_identifier",
  "program", "using_directive", "namespace_declaration", "block_namespace",
  "file_scoped_namespace", "class_declaration",
  "class_primary_constructor", "class_inheritance", "class_modifiers",
  "class_body", "class_members", "class_member", "field_declaration",
  "method_declaration", "$@1", "$@2", "$@3", "$@4", "property_declaration",
  "accessors", "accessor", "modifiers", "type", "type_list",
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

#define YYPACT_NINF (-158)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-78)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -158,    59,  -158,    23,    83,  -158,  -158,  -158,  -158,  -158,
     757,  -158,    81,    26,   129,  -158,   105,  -158,  -158,  -158,
    -158,  -158,  -158,   116,  -158,  -158,    23,  -158,    58,  -158,
      -6,   168,   402,    64,  -158,  -158,    92,  -158,  -158,  -158,
    -158,  -158,  -158,   102,     1,    46,  -158,    23,   100,   402,
     402,  -158,  -158,  -158,   402,   124,  -158,  -158,   -45,    19,
    -158,   348,   134,  -158,  -158,  -158,  -158,  -158,  -158,   392,
       2,  -158,  -158,  -158,  -158,  -158,  -158,  -158,  -158,  -158,
    -158,  -158,     6,     4,    -5,   150,   402,    61,  -158,   402,
     402,    22,   294,  -158,    47,   402,    67,    79,   199,  -158,
     165,   294,  -158,    23,  -158,  -158,   294,   294,   294,   425,
     -33,     3,    -1,  -158,   434,    86,  -158,  -158,   175,    85,
     673,   -40,   156,    49,   456,   294,   294,   294,   294,   294,
     294,   294,   294,   294,   294,   294,   294,   294,   294,   294,
     225,  -158,   181,   294,  -158,  -158,  -158,  -158,  -158,  -158,
    -158,    74,    98,  -158,   158,   158,   402,  -158,   673,   106,
     294,  -158,   156,   156,   156,   156,   697,   697,   673,   673,
     156,   156,   230,   230,    49,    49,    49,    62,   244,   673,
     246,  -158,  -158,  -158,  -158,   158,  -158,  -158,  -158,  -158,
     110,  -158,   294,   112,   402,   101,    65,     0,   180,   332,
     184,  -158,     5,  -158,  -158,  -158,  -158,  -158,   487,  -158,
    -158,   673,  -158,    32,   -47,  -158,   114,   402,   167,   294,
     294,  -158,   518,   294,   173,  -158,   158,   185,   187,   402,
    -158,   178,     7,  -158,   119,   549,   580,  -158,   611,  -158,
     294,  -158,   294,   172,   178,   189,  -158,  -158,   305,   305,
     642,   132,  -158,   140,   198,   248,  -158,  -158,  -158,  -158,
    -158,   170,   305,  -158,  -158
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       4,    19,     1,     0,     0,     5,     6,    10,    11,     7,
       0,     2,     0,     2,     0,    25,     0,    23,    21,    22,
      20,    26,    24,     0,    13,     4,     0,     8,    15,     3,
      19,     0,    81,    17,    12,     9,    77,    69,    67,    71,
      68,    72,    70,    73,     0,     0,    82,     0,     0,     0,
       0,    84,    78,    16,     0,    18,    28,    14,     0,     0,
      83,    56,    76,    74,    27,    29,    30,    31,    32,     0,
       0,    75,    64,    66,    60,    62,    58,    59,    57,    65,
      61,    63,    73,     0,     0,     0,     0,     0,    33,    81,
      81,     0,     0,    34,    45,    81,     0,     0,    74,   103,
     107,     0,   105,     0,   106,   104,     0,     0,     0,     0,
       0,     0,    56,    46,     0,     0,    37,    39,     0,   142,
     110,     0,   126,   117,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    43,     0,     0,    48,    88,    50,    49,    51,    44,
      47,     0,     0,    35,     0,     0,    81,   108,   143,     0,
     142,   111,   122,   123,   120,   121,   124,   125,   141,   140,
     118,   119,   112,   113,   114,   115,   116,   128,     0,   127,
       0,    52,    54,    53,    55,     0,    85,    38,    86,    40,
       0,   109,     0,     0,     0,   142,   134,   107,     0,     0,
       0,    87,     0,    90,    89,    94,    95,    96,     0,    36,
      41,   144,   139,    79,     0,   129,     0,     0,   142,     0,
       0,   101,     0,     0,     0,    97,     0,     0,     0,     0,
     130,    79,     0,   135,     0,     0,     0,   102,     0,    98,
       0,    42,   142,   142,    80,     0,   136,   100,     0,     0,
       0,     0,   132,     0,   142,    91,    93,    99,   131,   133,
     137,     0,     0,   138,    92
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -158,    13,   236,  -158,  -158,  -158,  -158,  -158,  -158,  -158,
    -158,  -158,  -158,  -158,  -158,  -158,  -158,  -158,  -158,  -158,
    -158,  -158,   152,   204,   -49,    57,   -65,   212,  -153,   -96,
    -158,  -150,  -158,  -158,  -158,   -85,  -157
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,    12,     1,     5,     6,     7,     8,     9,    33,    48,
      10,    57,    61,    65,    66,    67,   185,   154,   155,   226,
      68,   112,   113,   114,    44,   214,    45,    46,   187,   188,
     180,   204,   205,   206,   207,   158,   159
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      58,    59,   189,   193,   -77,    51,    84,   109,    87,   224,
      85,   228,    70,    62,   146,   148,   120,    14,     3,   110,
      83,   122,   123,   124,    96,    97,    23,    11,   229,    52,
     115,   160,   209,   111,   144,     4,   145,    91,   216,    31,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   182,   184,    49,   179,     2,
      55,   234,    88,    86,    34,   245,    89,   110,   219,   149,
     147,   119,   145,   241,   -77,    52,    52,    63,    52,    52,
      98,   111,   229,     3,   203,   251,   253,    13,    99,   100,
     227,   190,   101,    52,    26,   208,    52,   261,   255,   256,
       4,   102,   131,   132,    99,   100,    52,   211,   101,    28,
     103,   104,   264,    92,   222,   140,   121,   102,    53,   194,
      29,    54,   217,   142,   105,   143,   103,   104,    93,    32,
      94,   202,    95,   195,   235,   236,   218,    47,   238,   116,
     105,   181,    54,   145,   106,   213,   107,    23,    24,    49,
      25,   117,   203,   203,    54,   250,   108,   157,   153,    50,
     106,    54,   107,   208,   208,   183,   203,   145,   231,    56,
      99,   100,   108,   215,   101,    99,   100,   208,   191,   101,
     244,   192,   210,   102,   212,    54,   230,   192,   102,   192,
      23,   246,   103,   104,   192,    23,    27,   103,   104,   202,
     202,    99,   100,   118,   258,   101,   105,   192,    71,   131,
     132,   105,   259,   202,   102,   192,   135,   136,   137,   138,
     139,    90,   140,   103,   104,   186,   106,   145,   107,   177,
     142,   106,   143,   107,    23,    35,   119,   105,   108,   233,
     239,   240,   263,   108,   252,   192,   156,   178,   196,    99,
     197,   220,    52,   101,    37,   223,   242,   106,   243,   107,
     254,    30,   102,   262,   150,    69,    60,   198,    38,   108,
     260,   103,   104,    39,   232,     0,     0,     0,   199,     0,
       0,     0,    40,   131,   132,   105,     0,     0,    41,     0,
      42,   200,   137,   138,   139,     0,   140,    99,   100,     0,
       0,   101,    43,     0,   142,   106,   143,   107,    99,   197,
     102,     0,   101,    37,     0,   145,   201,   108,     0,   103,
     104,   102,     0,     0,     0,     0,   198,    38,     0,     0,
     103,   104,    39,   105,     0,    99,   100,   199,     0,   101,
       0,    40,     0,     0,   105,     0,     0,    41,   102,    42,
     200,     0,    36,   106,     0,   107,    37,   103,   104,     0,
       0,    43,     0,     0,   106,   108,   107,     0,     0,     0,
      38,   105,     0,     0,   145,    39,   108,     0,     0,     0,
       0,     0,     0,     0,    40,     0,     0,     0,     0,     0,
      41,   106,    42,   107,     0,     0,    36,    72,    73,   221,
      37,     0,     0,   108,    43,     0,    36,     0,     0,     0,
      37,     0,     0,     0,    38,    74,     0,     0,    64,    39,
      75,    76,    77,    78,    38,    79,     0,    80,    40,    39,
       0,     0,     0,     0,    41,    81,    42,     0,    40,    72,
      73,     0,     0,     0,    41,     0,    42,     0,    82,     0,
       0,     0,     0,     0,   151,     0,     0,    74,    43,     0,
       0,     0,    75,    76,    77,    78,     0,    79,   152,    80,
       0,   125,   126,   127,   128,   129,   130,    81,   131,   132,
       0,     0,   133,   134,     0,   135,   136,   137,   138,   139,
       0,   140,   141,     0,     0,     0,     0,     0,     0,   142,
       0,   143,   125,   126,   127,   128,   129,   130,     0,   131,
     132,     0,     0,   133,   134,     0,   135,   136,   137,   138,
     139,     0,   140,     0,     0,     0,     0,     0,   161,     0,
     142,     0,   143,   125,   126,   127,   128,   129,   130,     0,
     131,   132,     0,     0,   133,   134,     0,   135,   136,   137,
     138,   139,     0,   140,   225,     0,     0,     0,     0,     0,
       0,   142,     0,   143,   125,   126,   127,   128,   129,   130,
       0,   131,   132,     0,     0,   133,   134,     0,   135,   136,
     137,   138,   139,     0,   140,   237,     0,     0,     0,     0,
       0,     0,   142,     0,   143,   125,   126,   127,   128,   129,
     130,     0,   131,   132,     0,     0,   133,   134,     0,   135,
     136,   137,   138,   139,     0,   140,   247,     0,     0,     0,
       0,     0,     0,   142,     0,   143,   125,   126,   127,   128,
     129,   130,     0,   131,   132,     0,     0,   133,   134,     0,
     135,   136,   137,   138,   139,     0,   140,     0,     0,     0,
       0,     0,   248,     0,   142,     0,   143,   125,   126,   127,
     128,   129,   130,     0,   131,   132,     0,     0,   133,   134,
       0,   135,   136,   137,   138,   139,     0,   140,     0,     0,
       0,     0,     0,   249,     0,   142,     0,   143,   125,   126,
     127,   128,   129,   130,     0,   131,   132,     0,     0,   133,
     134,     0,   135,   136,   137,   138,   139,     0,   140,   257,
       0,     0,     0,     0,     0,     0,   142,     0,   143,   125,
     126,   127,   128,   129,   130,     0,   131,   132,     0,     0,
     133,   134,     0,   135,   136,   137,   138,   139,     0,   140,
       0,     0,     0,   125,   126,   127,   128,   142,     0,   143,
     131,   132,     0,     0,   133,   134,     0,   135,   136,   137,
     138,   139,    15,   140,     0,     0,     0,     0,     0,    16,
       0,   142,     0,   143,     0,     0,     0,     0,     0,     0,
      17,     0,     0,     0,     0,     0,    18,    19,    20,     0,
      21,     0,    22
};

static const yytype_int16 yycheck[] =
{
      49,    50,   155,   160,     4,     4,     4,    92,     4,     4,
       4,    58,    61,    58,   110,   111,   101,     4,    24,    20,
      69,   106,   107,   108,    89,    90,    66,     4,    75,    74,
      95,    71,   185,    34,    67,    41,    69,    86,   195,    26,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   151,   152,    57,   143,     0,
      47,   218,    67,    57,    70,    58,    71,    20,    68,    70,
      67,    71,    69,   226,    74,    74,    74,    58,    74,    74,
      58,    34,    75,    24,   180,   242,   243,     4,     3,     4,
      58,   156,     7,    74,    68,   180,    74,   254,   248,   249,
      41,    16,    53,    54,     3,     4,    74,   192,     7,     4,
      25,    26,   262,    52,   199,    66,   103,    16,    72,    57,
       4,    75,    57,    74,    39,    76,    25,    26,    67,    71,
      69,   180,    71,    71,   219,   220,    71,    73,   223,    72,
      39,    67,    75,    69,    59,   194,    61,    66,    67,    57,
      69,    72,   248,   249,    75,   240,    71,    72,    72,    57,
      59,    75,    61,   248,   249,    67,   262,    69,   217,    69,
       3,     4,    71,    72,     7,     3,     4,   262,    72,     7,
     229,    75,    72,    16,    72,    75,    72,    75,    16,    75,
      66,    72,    25,    26,    75,    66,    67,    25,    26,   248,
     249,     3,     4,     4,    72,     7,    39,    75,    74,    53,
      54,    39,    72,   262,    16,    75,    60,    61,    62,    63,
      64,    71,    66,    25,    26,    67,    59,    69,    61,     4,
      74,    59,    76,    61,    66,    67,    71,    39,    71,    72,
      67,    68,    72,    71,    72,    75,    71,    66,     4,     3,
       4,    71,    74,     7,     8,    71,    71,    59,    71,    61,
      71,    25,    16,    15,   112,    61,    54,    21,    22,    71,
      72,    25,    26,    27,   217,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    36,    53,    54,    39,    -1,    -1,    42,    -1,
      44,    45,    62,    63,    64,    -1,    66,     3,     4,    -1,
      -1,     7,    56,    -1,    74,    59,    76,    61,     3,     4,
      16,    -1,     7,     8,    -1,    69,    70,    71,    -1,    25,
      26,    16,    -1,    -1,    -1,    -1,    21,    22,    -1,    -1,
      25,    26,    27,    39,    -1,     3,     4,    32,    -1,     7,
      -1,    36,    -1,    -1,    39,    -1,    -1,    42,    16,    44,
      45,    -1,     4,    59,    -1,    61,     8,    25,    26,    -1,
      -1,    56,    -1,    -1,    59,    71,    61,    -1,    -1,    -1,
      22,    39,    -1,    -1,    69,    27,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    36,    -1,    -1,    -1,    -1,    -1,
      42,    59,    44,    61,    -1,    -1,     4,     5,     6,    67,
       8,    -1,    -1,    71,    56,    -1,     4,    -1,    -1,    -1,
       8,    -1,    -1,    -1,    22,    23,    -1,    -1,    70,    27,
      28,    29,    30,    31,    22,    33,    -1,    35,    36,    27,
      -1,    -1,    -1,    -1,    42,    43,    44,    -1,    36,     5,
       6,    -1,    -1,    -1,    42,    -1,    44,    -1,    56,    -1,
      -1,    -1,    -1,    -1,    20,    -1,    -1,    23,    56,    -1,
      -1,    -1,    28,    29,    30,    31,    -1,    33,    34,    35,
      -1,    46,    47,    48,    49,    50,    51,    43,    53,    54,
      -1,    -1,    57,    58,    -1,    60,    61,    62,    63,    64,
      -1,    66,    67,    -1,    -1,    -1,    -1,    -1,    -1,    74,
      -1,    76,    46,    47,    48,    49,    50,    51,    -1,    53,
      54,    -1,    -1,    57,    58,    -1,    60,    61,    62,    63,
      64,    -1,    66,    -1,    -1,    -1,    -1,    -1,    72,    -1,
      74,    -1,    76,    46,    47,    48,    49,    50,    51,    -1,
      53,    54,    -1,    -1,    57,    58,    -1,    60,    61,    62,
      63,    64,    -1,    66,    67,    -1,    -1,    -1,    -1,    -1,
      -1,    74,    -1,    76,    46,    47,    48,    49,    50,    51,
      -1,    53,    54,    -1,    -1,    57,    58,    -1,    60,    61,
      62,    63,    64,    -1,    66,    67,    -1,    -1,    -1,    -1,
      -1,    -1,    74,    -1,    76,    46,    47,    48,    49,    50,
      51,    -1,    53,    54,    -1,    -1,    57,    58,    -1,    60,
      61,    62,    63,    64,    -1,    66,    67,    -1,    -1,    -1,
      -1,    -1,    -1,    74,    -1,    76,    46,    47,    48,    49,
      50,    51,    -1,    53,    54,    -1,    -1,    57,    58,    -1,
      60,    61,    62,    63,    64,    -1,    66,    -1,    -1,    -1,
      -1,    -1,    72,    -1,    74,    -1,    76,    46,    47,    48,
      49,    50,    51,    -1,    53,    54,    -1,    -1,    57,    58,
      -1,    60,    61,    62,    63,    64,    -1,    66,    -1,    -1,
      -1,    -1,    -1,    72,    -1,    74,    -1,    76,    46,    47,
      48,    49,    50,    51,    -1,    53,    54,    -1,    -1,    57,
      58,    -1,    60,    61,    62,    63,    64,    -1,    66,    67,
      -1,    -1,    -1,    -1,    -1,    -1,    74,    -1,    76,    46,
      47,    48,    49,    50,    51,    -1,    53,    54,    -1,    -1,
      57,    58,    -1,    60,    61,    62,    63,    64,    -1,    66,
      -1,    -1,    -1,    46,    47,    48,    49,    74,    -1,    76,
      53,    54,    -1,    -1,    57,    58,    -1,    60,    61,    62,
      63,    64,     5,    66,    -1,    -1,    -1,    -1,    -1,    12,
      -1,    74,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    -1,    -1,    -1,    29,    30,    31,    -1,
      33,    -1,    35
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    79,     0,    24,    41,    80,    81,    82,    83,    84,
      87,     4,    78,     4,    78,     5,    12,    23,    29,    30,
      31,    33,    35,    66,    67,    69,    68,    67,     4,     4,
      79,    78,    71,    85,    70,    67,     4,     8,    22,    27,
      36,    42,    44,    56,   101,   103,   104,    73,    86,    57,
      57,     4,    74,    72,    75,    78,    69,    88,   101,   101,
     104,    89,    58,    58,    70,    90,    91,    92,    97,   100,
     101,    74,     5,     6,    23,    28,    29,    30,    31,    33,
      35,    43,    56,   101,     4,     4,    57,     4,    67,    71,
      71,   101,    52,    67,    69,    71,   103,   103,    58,     3,
       4,     7,    16,    25,    26,    39,    59,    61,    71,   112,
      20,    34,    98,    99,   100,   103,    72,    72,     4,    71,
     112,    78,   112,   112,   112,    46,    47,    48,    49,    50,
      51,    53,    54,    57,    58,    60,    61,    62,    63,    64,
      66,    67,    74,    76,    67,    69,   106,    67,   106,    70,
      99,    20,    34,    72,    94,    95,    71,    72,   112,   113,
      71,    72,   112,   112,   112,   112,   112,   112,   112,   112,
     112,   112,   112,   112,   112,   112,   112,     4,    66,   112,
     107,    67,   106,    67,   106,    93,    67,   105,   106,   105,
     103,    72,    75,   113,    57,    71,     4,     4,    21,    32,
      45,    70,   101,   106,   108,   109,   110,   111,   112,   105,
      72,   112,    72,   101,   102,    72,   113,    57,    71,    68,
      71,    67,   112,    71,     4,    67,    96,    58,    58,    75,
      72,   101,   102,    72,   113,   112,   112,    67,   112,    67,
      68,   105,    71,    71,   101,    58,    72,    67,    72,    72,
     112,   113,    72,   113,    71,   108,   108,    67,    72,    72,
      72,   113,    15,    72,   108
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    77,    78,    78,    79,    79,    79,    79,    80,    80,
      81,    81,    82,    83,    84,    85,    85,    86,    86,    87,
      87,    87,    87,    87,    87,    87,    87,    88,    89,    89,
      90,    90,    90,    91,    91,    93,    92,    94,    92,    95,
      92,    96,    92,    97,    97,    98,    98,    98,    99,    99,
      99,    99,    99,    99,    99,    99,   100,   100,   100,   100,
     100,   100,   100,   100,   100,   100,   100,   101,   101,   101,
     101,   101,   101,   101,   101,   101,   101,   101,   101,   102,
     102,   103,   103,   103,   104,   105,   105,   106,   107,   107,
     108,   108,   108,   108,   108,   108,   108,   108,   109,   109,
     110,   111,   111,   112,   112,   112,   112,   112,   112,   112,
     112,   112,   112,   112,   112,   112,   112,   112,   112,   112,
     112,   112,   112,   112,   112,   112,   112,   112,   112,   112,
     112,   112,   112,   112,   112,   112,   112,   112,   112,   112,
     112,   112,   113,   113,   113
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     3,     0,     2,     2,     2,     3,     5,
       1,     1,     5,     3,     6,     0,     3,     0,     2,     0,
       2,     2,     2,     2,     2,     2,     2,     3,     0,     2,
       1,     1,     1,     3,     4,     0,     8,     0,     7,     0,
       8,     0,    11,     6,     6,     0,     1,     2,     2,     2,
       2,     2,     3,     3,     3,     3,     0,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     1,     1,     1,
       1,     1,     1,     1,     4,     5,     4,     1,     2,     1,
       3,     0,     1,     3,     2,     1,     1,     3,     0,     2,
       1,     5,     7,     5,     1,     1,     1,     2,     3,     5,
       4,     2,     3,     1,     1,     1,     1,     1,     3,     4,
       2,     3,     3,     3,     3,     3,     3,     2,     3,     3,
       3,     3,     3,     3,     3,     3,     2,     3,     3,     5,
       6,     9,     8,     9,     4,     6,     7,     9,    10,     5,
       3,     3,     0,     1,     3
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
#line 130 "parser.y"
    {
        printf("Using directive\n");
    }
#line 1545 "parser.tab.c"
    break;

  case 9: /* using_directive: USING IDENTIFIER '=' qualified_identifier ';'  */
#line 134 "parser.y"
    {
        printf("Using alias: %s = ...\n", (yyvsp[-3].string));
        free((yyvsp[-3].string));
    }
#line 1554 "parser.tab.c"
    break;

  case 12: /* block_namespace: NAMESPACE qualified_identifier '{' program '}'  */
#line 147 "parser.y"
    {
        printf("Block namespace\n");
    }
#line 1562 "parser.tab.c"
    break;

  case 13: /* file_scoped_namespace: NAMESPACE qualified_identifier ';'  */
#line 154 "parser.y"
    {
        printf("File-scoped namespace\n");
    }
#line 1570 "parser.tab.c"
    break;

  case 14: /* class_declaration: class_modifiers CLASS IDENTIFIER class_primary_constructor class_inheritance class_body  */
#line 161 "parser.y"
    {
        printf("Valid class: %s\n", (yyvsp[-3].string));
        free((yyvsp[-3].string));
        clear_symbols();
    }
#line 1580 "parser.tab.c"
    break;

  case 16: /* class_primary_constructor: '(' parameter_list ')'  */
#line 171 "parser.y"
    {
        printf("  Primary constructor\n");
    }
#line 1588 "parser.tab.c"
    break;

  case 18: /* class_inheritance: ':' qualified_identifier  */
#line 179 "parser.y"
    {
        printf("  Inherits from\n");
    }
#line 1596 "parser.tab.c"
    break;

  case 33: /* field_declaration: type IDENTIFIER ';'  */
#line 212 "parser.y"
    {
        printf("  Field: %s\n", (yyvsp[-1].string));
        add_symbol((yyvsp[-1].string), (yyvsp[-2].string));
        free((yyvsp[-1].string));
        free((yyvsp[-2].string));
    }
#line 1607 "parser.tab.c"
    break;

  case 34: /* field_declaration: modifiers type IDENTIFIER ';'  */
#line 219 "parser.y"
    {
        printf("  Field: %s\n", (yyvsp[-1].string));
        add_symbol((yyvsp[-1].string), (yyvsp[-2].string));
        free((yyvsp[-1].string));
        free((yyvsp[-2].string));
    }
#line 1618 "parser.tab.c"
    break;

  case 35: /* $@1: %empty  */
#line 229 "parser.y"
    {
        current_method_type = strdup((yyvsp[-4].string));
        current_method_name = strdup((yyvsp[-3].string));
        current_method_has_return = 0;
        printf("  Method: %s (return type: %s)\n", (yyvsp[-3].string), (yyvsp[-4].string));
        free((yyvsp[-3].string));
        free((yyvsp[-4].string));
    }
#line 1631 "parser.tab.c"
    break;

  case 36: /* method_declaration: modifiers type IDENTIFIER '(' parameter_list ')' $@1 method_body  */
#line 238 "parser.y"
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
#line 1648 "parser.tab.c"
    break;

  case 37: /* $@2: %empty  */
#line 251 "parser.y"
    {
        current_method_type = strdup((yyvsp[-4].string));
        current_method_name = strdup((yyvsp[-3].string));
        current_method_has_return = 0;
        printf("  Method: %s (return type: %s)\n", (yyvsp[-3].string), (yyvsp[-4].string));
        free((yyvsp[-3].string));
        free((yyvsp[-4].string));
    }
#line 1661 "parser.tab.c"
    break;

  case 38: /* method_declaration: type IDENTIFIER '(' parameter_list ')' $@2 method_body  */
#line 260 "parser.y"
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
#line 1678 "parser.tab.c"
    break;

  case 39: /* $@3: %empty  */
#line 273 "parser.y"
    {
        // async Task метод - возвращает void
        current_method_type = strdup("void");
        current_method_name = strdup((yyvsp[-3].string));
        current_method_has_return = 0;
        printf("  Async Method: %s (return type: void)\n", (yyvsp[-3].string));
        free((yyvsp[-3].string));
    }
#line 1691 "parser.tab.c"
    break;

  case 40: /* method_declaration: modifiers TASK IDENTIFIER '(' parameter_list ')' $@3 method_body  */
#line 282 "parser.y"
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
#line 1708 "parser.tab.c"
    break;

  case 41: /* $@4: %empty  */
#line 295 "parser.y"
    {
        // async Task<T> метод - возвращает T
        current_method_type = strdup((yyvsp[-5].string));
        current_method_name = strdup((yyvsp[-3].string));
        current_method_has_return = 0;
        printf("  Async Method: %s (return type: %s)\n", (yyvsp[-3].string), (yyvsp[-5].string));
        free((yyvsp[-3].string));
        free((yyvsp[-5].string));
    }
#line 1722 "parser.tab.c"
    break;

  case 42: /* method_declaration: modifiers TASK '<' type '>' IDENTIFIER '(' parameter_list ')' $@4 method_body  */
#line 305 "parser.y"
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
#line 1739 "parser.tab.c"
    break;

  case 43: /* property_declaration: modifiers type IDENTIFIER ARROW expression ';'  */
#line 321 "parser.y"
    {
        printf("  Expression-bodied property: %s (type: %s)\n", (yyvsp[-3].string), (yyvsp[-4].string));
        free((yyvsp[-3].string));
        free((yyvsp[-4].string));
        free((yyvsp[-1].expr).type);
    }
#line 1750 "parser.tab.c"
    break;

  case 44: /* property_declaration: modifiers type IDENTIFIER '{' accessors '}'  */
#line 328 "parser.y"
    {
        printf("  Property: %s (type: %s)\n", (yyvsp[-3].string), (yyvsp[-4].string));
        free((yyvsp[-3].string));
        free((yyvsp[-4].string));
    }
#line 1760 "parser.tab.c"
    break;

  case 67: /* type: INT  */
#line 367 "parser.y"
        { (yyval.string) = strdup("int"); }
#line 1766 "parser.tab.c"
    break;

  case 68: /* type: STRING  */
#line 368 "parser.y"
             { (yyval.string) = strdup("string"); }
#line 1772 "parser.tab.c"
    break;

  case 69: /* type: BOOL  */
#line 369 "parser.y"
           { (yyval.string) = strdup("bool"); }
#line 1778 "parser.tab.c"
    break;

  case 70: /* type: VOID  */
#line 370 "parser.y"
           { (yyval.string) = strdup("void"); }
#line 1784 "parser.tab.c"
    break;

  case 71: /* type: OBJECT  */
#line 371 "parser.y"
             { (yyval.string) = strdup("object"); }
#line 1790 "parser.tab.c"
    break;

  case 72: /* type: VAR  */
#line 372 "parser.y"
          { (yyval.string) = strdup("var"); }
#line 1796 "parser.tab.c"
    break;

  case 73: /* type: TASK  */
#line 373 "parser.y"
           { (yyval.string) = strdup("Task"); }
#line 1802 "parser.tab.c"
    break;

  case 74: /* type: TASK '<' type '>'  */
#line 374 "parser.y"
                        { 
        char* task_type = malloc(strlen("Task<") + strlen((yyvsp[-1].string)) + 2);
        sprintf(task_type, "Task<%s>", (yyvsp[-1].string));
        (yyval.string) = task_type;
        free((yyvsp[-1].string));
    }
#line 1813 "parser.tab.c"
    break;

  case 75: /* type: IDENTIFIER '<' type '>' '?'  */
#line 381 "parser.y"
    {
        char* tmp = malloc(strlen((yyvsp[-4].string)) + strlen((yyvsp[-2].string)) + 4);
        sprintf(tmp, "%s<%s>?", (yyvsp[-4].string), (yyvsp[-2].string));
        (yyval.string) = tmp;
        free((yyvsp[-4].string));
        free((yyvsp[-2].string));
    }
#line 1825 "parser.tab.c"
    break;

  case 76: /* type: IDENTIFIER '<' type '>'  */
#line 389 "parser.y"
    {
        char* tmp = malloc(strlen((yyvsp[-3].string)) + strlen((yyvsp[-1].string)) + 3);
        sprintf(tmp, "%s<%s>", (yyvsp[-3].string), (yyvsp[-1].string));
        (yyval.string) = tmp;
        free((yyvsp[-3].string));
        free((yyvsp[-1].string));
    }
#line 1837 "parser.tab.c"
    break;

  case 77: /* type: IDENTIFIER  */
#line 396 "parser.y"
                 { (yyval.string) = (yyvsp[0].string); }
#line 1843 "parser.tab.c"
    break;

  case 78: /* type: type '?'  */
#line 397 "parser.y"
               { 
        char* nullable = malloc(strlen((yyvsp[-1].string)) + 2);
        sprintf(nullable, "%s?", (yyvsp[-1].string));
        (yyval.string) = nullable;
        free((yyvsp[-1].string));
    }
#line 1854 "parser.tab.c"
    break;

  case 84: /* parameter: type IDENTIFIER  */
#line 418 "parser.y"
    {
        add_symbol((yyvsp[0].string), (yyvsp[-1].string));
        printf("    Parameter: %s %s\n", (yyvsp[-1].string), (yyvsp[0].string));
        free((yyvsp[0].string));
        free((yyvsp[-1].string));
    }
#line 1865 "parser.tab.c"
    break;

  case 85: /* method_body: ';'  */
#line 428 "parser.y"
    {
        // Абстрактный метод
        current_method_has_return = 1;
    }
#line 1874 "parser.tab.c"
    break;

  case 91: /* statement: IF '(' expression ')' statement  */
#line 447 "parser.y"
    {
        if (strcmp((yyvsp[-2].expr).type, "int") != 0 && strcmp((yyvsp[-2].expr).type, "bool") != 0 && 
            strcmp((yyvsp[-2].expr).type, "unknown") != 0 && strcmp((yyvsp[-2].expr).type, "null") != 0) {
            char err[256];
            snprintf(err, sizeof(err), "Condition must be bool or int, got '%s'", (yyvsp[-2].expr).type);
            yyerror(err);
        }
        free((yyvsp[-2].expr).type);
    }
#line 1888 "parser.tab.c"
    break;

  case 92: /* statement: IF '(' expression ')' statement ELSE statement  */
#line 457 "parser.y"
    {
        if (strcmp((yyvsp[-4].expr).type, "int") != 0 && strcmp((yyvsp[-4].expr).type, "bool") != 0 && 
            strcmp((yyvsp[-4].expr).type, "unknown") != 0 && strcmp((yyvsp[-4].expr).type, "null") != 0) {
            char err[256];
            snprintf(err, sizeof(err), "Condition must be bool or int, got '%s'", (yyvsp[-4].expr).type);
            yyerror(err);
        }
        free((yyvsp[-4].expr).type);
    }
#line 1902 "parser.tab.c"
    break;

  case 93: /* statement: WHILE '(' expression ')' statement  */
#line 467 "parser.y"
    {
        if (strcmp((yyvsp[-2].expr).type, "int") != 0 && strcmp((yyvsp[-2].expr).type, "bool") != 0 && 
            strcmp((yyvsp[-2].expr).type, "unknown") != 0 && strcmp((yyvsp[-2].expr).type, "null") != 0) {
            char err[256];
            snprintf(err, sizeof(err), "Condition must be bool or int, got '%s'", (yyvsp[-2].expr).type);
            yyerror(err);
        }
        free((yyvsp[-2].expr).type);
    }
#line 1916 "parser.tab.c"
    break;

  case 97: /* statement: expression ';'  */
#line 480 "parser.y"
    {
        free((yyvsp[-1].expr).type);
    }
#line 1924 "parser.tab.c"
    break;

  case 98: /* declaration_statement: type IDENTIFIER ';'  */
#line 487 "parser.y"
    {
        printf("DEBUG: declaration_statement (no init)\n");
        add_symbol((yyvsp[-1].string), (yyvsp[-2].string));
        free((yyvsp[-1].string));
        free((yyvsp[-2].string));
    }
#line 1935 "parser.tab.c"
    break;

  case 99: /* declaration_statement: type IDENTIFIER '=' expression ';'  */
#line 494 "parser.y"
    {
        printf("DEBUG: declaration_statement with init, type=%s, var=%s, expr_type=%s\n", (yyvsp[-4].string), (yyvsp[-3].string), (yyvsp[-1].expr).type);

        if (strcmp((yyvsp[-4].string), "var") == 0) {
            add_symbol((yyvsp[-3].string), (yyvsp[-1].expr).type);
        } else {
            if (strcmp((yyvsp[-1].expr).type, "unknown") != 0 && !is_type_compatible((yyvsp[-4].string), (yyvsp[-1].expr).type)) {
                char err[256];
                snprintf(err, sizeof(err), "Cannot assign '%s' to '%s'", 
                        get_type_name((yyvsp[-1].expr).type), get_type_name((yyvsp[-4].string)));
                yyerror(err);
            }
            add_symbol((yyvsp[-3].string), (yyvsp[-4].string));
        }

        free((yyvsp[-3].string));
        free((yyvsp[-4].string));
        free((yyvsp[-1].expr).type);
    }
#line 1959 "parser.tab.c"
    break;

  case 100: /* assignment_statement: IDENTIFIER '=' expression ';'  */
#line 517 "parser.y"
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
#line 1979 "parser.tab.c"
    break;

  case 101: /* return_statement: RETURN ';'  */
#line 536 "parser.y"
    {
        if (current_method_type && strcmp(current_method_type, "void") != 0) {
            char err[256];
            snprintf(err, sizeof(err), "Non-void method must return a value");
            yyerror(err);
        }
        current_method_has_return = 1;
    }
#line 1992 "parser.tab.c"
    break;

  case 102: /* return_statement: RETURN expression ';'  */
#line 545 "parser.y"
    {
        if (current_method_type && strcmp(current_method_type, "void") == 0) {
            char err[256];
            snprintf(err, sizeof(err), "Void method cannot return a value (got '%s')", 
                     get_type_name((yyvsp[-1].expr).type));
            yyerror(err);
        } else if (current_method_type && strcmp((yyvsp[-1].expr).type, "unknown") != 0 && !is_type_compatible(current_method_type, (yyvsp[-1].expr).type)) {
            char err[256];
            snprintf(err, sizeof(err), "Return type mismatch: expected '%s', got '%s'",
                     get_type_name(current_method_type), get_type_name((yyvsp[-1].expr).type));
            yyerror(err);
        }
        current_method_has_return = 1;
        free((yyvsp[-1].expr).type);
    }
#line 2012 "parser.tab.c"
    break;

  case 103: /* expression: INT_LITERAL  */
#line 564 "parser.y"
    {
        (yyval.expr).type = strdup("int");
    }
#line 2020 "parser.tab.c"
    break;

  case 104: /* expression: TRUE  */
#line 568 "parser.y"
    {
        (yyval.expr).type = strdup("bool");
    }
#line 2028 "parser.tab.c"
    break;

  case 105: /* expression: FALSE  */
#line 572 "parser.y"
    {
        (yyval.expr).type = strdup("bool");
    }
#line 2036 "parser.tab.c"
    break;

  case 106: /* expression: NULL_  */
#line 576 "parser.y"
    {
        (yyval.expr).type = strdup("null");
    }
#line 2044 "parser.tab.c"
    break;

  case 107: /* expression: IDENTIFIER  */
#line 580 "parser.y"
    {
        Symbol* sym = find_symbol((yyvsp[0].string));
        if (!sym) {
            (yyval.expr).type = strdup("unknown");
        } else {
            (yyval.expr).type = strdup(sym->type);
        }
        free((yyvsp[0].string));
    }
#line 2058 "parser.tab.c"
    break;

  case 108: /* expression: IDENTIFIER '(' ')'  */
#line 590 "parser.y"
    {
        (yyval.expr).type = strdup("unknown");
        free((yyvsp[-2].string));
    }
#line 2067 "parser.tab.c"
    break;

  case 109: /* expression: IDENTIFIER '(' argument_list ')'  */
#line 595 "parser.y"
    {
        (yyval.expr).type = strdup("unknown");
        free((yyvsp[-3].string));
    }
#line 2076 "parser.tab.c"
    break;

  case 110: /* expression: AWAIT expression  */
#line 600 "parser.y"
    {
        (yyval.expr).type = strdup((yyvsp[0].expr).type);
        free((yyvsp[0].expr).type);
    }
#line 2085 "parser.tab.c"
    break;

  case 111: /* expression: '(' expression ')'  */
#line 605 "parser.y"
    {
        (yyval.expr).type = strdup((yyvsp[-1].expr).type);
        free((yyvsp[-1].expr).type);
    }
#line 2094 "parser.tab.c"
    break;

  case 112: /* expression: expression '+' expression  */
#line 610 "parser.y"
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
#line 2111 "parser.tab.c"
    break;

  case 116: /* expression: expression '%' expression  */
#line 626 "parser.y"
    {
        if (strcmp((yyvsp[-2].expr).type, "int") == 0 && strcmp((yyvsp[0].expr).type, "int") == 0) {
            (yyval.expr).type = strdup("int");
        } else {
            (yyval.expr).type = strdup("unknown");
        }
        free((yyvsp[-2].expr).type);
        free((yyvsp[0].expr).type);
    }
#line 2125 "parser.tab.c"
    break;

  case 117: /* expression: '-' expression  */
#line 636 "parser.y"
    {
        if (strcmp((yyvsp[0].expr).type, "int") == 0) {
            (yyval.expr).type = strdup("int");
        } else {
            (yyval.expr).type = strdup("unknown");
        }
        free((yyvsp[0].expr).type);
    }
#line 2138 "parser.tab.c"
    break;

  case 123: /* expression: expression NE expression  */
#line 650 "parser.y"
    {
        (yyval.expr).type = strdup("bool");
        free((yyvsp[-2].expr).type);
        free((yyvsp[0].expr).type);
    }
#line 2148 "parser.tab.c"
    break;

  case 125: /* expression: expression OR_OR expression  */
#line 657 "parser.y"
    {
        (yyval.expr).type = strdup("bool");
        free((yyvsp[-2].expr).type);
        free((yyvsp[0].expr).type);
    }
#line 2158 "parser.tab.c"
    break;

  case 126: /* expression: '!' expression  */
#line 663 "parser.y"
    {
        (yyval.expr).type = strdup("bool");
        free((yyvsp[0].expr).type);
    }
#line 2167 "parser.tab.c"
    break;

  case 127: /* expression: expression '|' expression  */
#line 668 "parser.y"
    {
        if (strcmp((yyvsp[-2].expr).type, "int") == 0 && strcmp((yyvsp[0].expr).type, "int") == 0) {
            (yyval.expr).type = strdup("int");
        } else {
            (yyval.expr).type = strdup("unknown");
        }
        free((yyvsp[-2].expr).type);
        free((yyvsp[0].expr).type);
    }
#line 2181 "parser.tab.c"
    break;

  case 128: /* expression: expression '.' IDENTIFIER  */
#line 678 "parser.y"
    {
        (yyval.expr).type = strdup("unknown");
        free((yyvsp[0].string));
    }
#line 2190 "parser.tab.c"
    break;

  case 129: /* expression: expression '.' IDENTIFIER '(' ')'  */
#line 683 "parser.y"
    {
        (yyval.expr).type = strdup("unknown");
        free((yyvsp[-2].string));
    }
#line 2199 "parser.tab.c"
    break;

  case 130: /* expression: expression '.' IDENTIFIER '(' argument_list ')'  */
#line 688 "parser.y"
    {
        (yyval.expr).type = strdup("unknown");
        free((yyvsp[-3].string));
    }
#line 2208 "parser.tab.c"
    break;

  case 131: /* expression: expression '.' IDENTIFIER '<' type '>' '(' argument_list ')'  */
#line 693 "parser.y"
    {
        (yyval.expr).type = strdup("unknown");
        free((yyvsp[-6].string));
        free((yyvsp[-4].string));
    }
#line 2218 "parser.tab.c"
    break;

  case 132: /* expression: expression '.' IDENTIFIER '<' type_list '>' '(' ')'  */
#line 699 "parser.y"
    {
        (yyval.expr).type = strdup("unknown");
        free((yyvsp[-5].string));
    }
#line 2227 "parser.tab.c"
    break;

  case 133: /* expression: expression '.' IDENTIFIER '<' type_list '>' '(' argument_list ')'  */
#line 704 "parser.y"
    {
        (yyval.expr).type = strdup("unknown");
        free((yyvsp[-6].string));
    }
#line 2236 "parser.tab.c"
    break;

  case 134: /* expression: expression '?' '.' IDENTIFIER  */
#line 709 "parser.y"
    {
        (yyval.expr).type = strdup("unknown");
        free((yyvsp[0].string));
    }
#line 2245 "parser.tab.c"
    break;

  case 135: /* expression: expression '?' '.' IDENTIFIER '(' ')'  */
#line 714 "parser.y"
    {
        (yyval.expr).type = strdup("unknown");
        free((yyvsp[-2].string));
    }
#line 2254 "parser.tab.c"
    break;

  case 136: /* expression: expression '?' '.' IDENTIFIER '(' argument_list ')'  */
#line 719 "parser.y"
    {
        (yyval.expr).type = strdup("unknown");
        free((yyvsp[-3].string));
    }
#line 2263 "parser.tab.c"
    break;

  case 137: /* expression: expression '?' '.' IDENTIFIER '<' type_list '>' '(' ')'  */
#line 724 "parser.y"
    {
        (yyval.expr).type = strdup("unknown");
        free((yyvsp[-5].string));
    }
#line 2272 "parser.tab.c"
    break;

  case 138: /* expression: expression '?' '.' IDENTIFIER '<' type_list '>' '(' argument_list ')'  */
#line 729 "parser.y"
    {
        (yyval.expr).type = strdup("unknown");
        free((yyvsp[-6].string));
    }
#line 2281 "parser.tab.c"
    break;

  case 139: /* expression: NEW qualified_identifier '(' argument_list ')'  */
#line 734 "parser.y"
    {
        (yyval.expr).type = strdup("unknown");
    }
#line 2289 "parser.tab.c"
    break;

  case 140: /* expression: expression NULL_COALESCE expression  */
#line 738 "parser.y"
    {
        (yyval.expr).type = strdup((yyvsp[-2].expr).type);
        free((yyvsp[-2].expr).type);
        free((yyvsp[0].expr).type);
    }
#line 2299 "parser.tab.c"
    break;

  case 141: /* expression: expression NULL_COALESCE_ASSIGN expression  */
#line 744 "parser.y"
    {
        (yyval.expr).type = strdup((yyvsp[-2].expr).type);
        free((yyvsp[-2].expr).type);
        free((yyvsp[0].expr).type);
    }
#line 2309 "parser.tab.c"
    break;


#line 2313 "parser.tab.c"

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

#line 757 "parser.y"


void yyerror(const char* msg) {
    print_semantic_error(msg);
    has_errors = 1;
}
