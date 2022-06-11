/* A Bison parser, made by GNU Bison 3.7.6.  */

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
#define YYBISON 30706

/* Bison version string.  */
#define YYBISON_VERSION "3.7.6"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 3 "Alpha_Compiler/parser.y"


#include "Alpha_Compiler/Symtable/symtable.h"
#include "Alpha_Compiler/Quads/quad.h"
#include "Alpha_Compiler/Final_Code/final_code.h"

#define yyerror(...) custom_yyerror(__VA_ARGS__, NULL)
#define yywarning(...) custom_warning(__VA_ARGS__, NULL)
void custom_yyerror(char const *str, ...);
void custom_warning(const char *str, ...);

extern int yylex();
extern int yylineno;
extern char* yytext;

extern struct SymTable_S *symTable;
extern struct ScopeList *scopeList;
extern struct ScopeList  *funcScopeList;

unsigned loopcounter = 0;
unsigned errorcounter = 0;
unsigned warningcounter = 0;


#line 96 "parser.c"

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

#include "parser.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_NUMBER = 3,                     /* "number"  */
  YYSYMBOL_STRING = 4,                     /* "string"  */
  YYSYMBOL_ID = 5,                         /* "identifier"  */
  YYSYMBOL_IF = 6,                         /* "if"  */
  YYSYMBOL_ELSE = 7,                       /* "else"  */
  YYSYMBOL_WHILE = 8,                      /* "while"  */
  YYSYMBOL_FOR = 9,                        /* "for"  */
  YYSYMBOL_FUNCTION = 10,                  /* "function"  */
  YYSYMBOL_RETURN = 11,                    /* "return"  */
  YYSYMBOL_BREAK = 12,                     /* "break"  */
  YYSYMBOL_CONTINUE = 13,                  /* "continue"  */
  YYSYMBOL_AND = 14,                       /* "and"  */
  YYSYMBOL_NOT = 15,                       /* "not"  */
  YYSYMBOL_OR = 16,                        /* "or"  */
  YYSYMBOL_LOCAL = 17,                     /* "local"  */
  YYSYMBOL_TRUE = 18,                      /* "true"  */
  YYSYMBOL_FALSE = 19,                     /* "false"  */
  YYSYMBOL_NIL = 20,                       /* "nil"  */
  YYSYMBOL_COLON_COLON = 21,               /* "::"  */
  YYSYMBOL_DOT_DOT = 22,                   /* ".."  */
  YYSYMBOL_EQUAL = 23,                     /* "=="  */
  YYSYMBOL_NOT_EQUAL = 24,                 /* "!="  */
  YYSYMBOL_PLUS_PLUS = 25,                 /* "++"  */
  YYSYMBOL_MINUS_MINUS = 26,               /* "--"  */
  YYSYMBOL_GREATER_EQUAL = 27,             /* ">="  */
  YYSYMBOL_LESS_EQUAL = 28,                /* "<="  */
  YYSYMBOL_29_ = 29,                       /* '='  */
  YYSYMBOL_30_ = 30,                       /* '>'  */
  YYSYMBOL_31_ = 31,                       /* '<'  */
  YYSYMBOL_32_ = 32,                       /* '+'  */
  YYSYMBOL_33_ = 33,                       /* '-'  */
  YYSYMBOL_34_ = 34,                       /* '*'  */
  YYSYMBOL_35_ = 35,                       /* '/'  */
  YYSYMBOL_36_ = 36,                       /* '%'  */
  YYSYMBOL_UMINUS = 37,                    /* UMINUS  */
  YYSYMBOL_38_ = 38,                       /* '.'  */
  YYSYMBOL_39_ = 39,                       /* '['  */
  YYSYMBOL_40_ = 40,                       /* ']'  */
  YYSYMBOL_41_ = 41,                       /* '('  */
  YYSYMBOL_42_ = 42,                       /* ')'  */
  YYSYMBOL_43_ = 43,                       /* ';'  */
  YYSYMBOL_44_ = 44,                       /* ','  */
  YYSYMBOL_45_ = 45,                       /* '{'  */
  YYSYMBOL_46_ = 46,                       /* ':'  */
  YYSYMBOL_47_ = 47,                       /* '}'  */
  YYSYMBOL_YYACCEPT = 48,                  /* $accept  */
  YYSYMBOL_program = 49,                   /* program  */
  YYSYMBOL_stmts = 50,                     /* stmts  */
  YYSYMBOL_stmt = 51,                      /* stmt  */
  YYSYMBOL_expr = 52,                      /* expr  */
  YYSYMBOL_53_1 = 53,                      /* $@1  */
  YYSYMBOL_54_2 = 54,                      /* $@2  */
  YYSYMBOL_55_3 = 55,                      /* $@3  */
  YYSYMBOL_56_4 = 56,                      /* $@4  */
  YYSYMBOL_W = 57,                         /* W  */
  YYSYMBOL_term = 58,                      /* term  */
  YYSYMBOL_assignexpr = 59,                /* assignexpr  */
  YYSYMBOL_primary = 60,                   /* primary  */
  YYSYMBOL_lvalue = 61,                    /* lvalue  */
  YYSYMBOL_member = 62,                    /* member  */
  YYSYMBOL_call = 63,                      /* call  */
  YYSYMBOL_callsuffix = 64,                /* callsuffix  */
  YYSYMBOL_normcall = 65,                  /* normcall  */
  YYSYMBOL_methodcall = 66,                /* methodcall  */
  YYSYMBOL_elist = 67,                     /* elist  */
  YYSYMBOL_objectdef = 68,                 /* objectdef  */
  YYSYMBOL_indexed = 69,                   /* indexed  */
  YYSYMBOL_indexedelem = 70,               /* indexedelem  */
  YYSYMBOL_block = 71,                     /* block  */
  YYSYMBOL_72_5 = 72,                      /* $@5  */
  YYSYMBOL_funcname = 73,                  /* funcname  */
  YYSYMBOL_funcprefix = 74,                /* funcprefix  */
  YYSYMBOL_funcargs = 75,                  /* funcargs  */
  YYSYMBOL_funcbody = 76,                  /* funcbody  */
  YYSYMBOL_funcdef = 77,                   /* funcdef  */
  YYSYMBOL_78_6 = 78,                      /* $@6  */
  YYSYMBOL_79_7 = 79,                      /* $@7  */
  YYSYMBOL_const = 80,                     /* const  */
  YYSYMBOL_idlist = 81,                    /* idlist  */
  YYSYMBOL_ifprefix = 82,                  /* ifprefix  */
  YYSYMBOL_elseprefix = 83,                /* elseprefix  */
  YYSYMBOL_ifstmt = 84,                    /* ifstmt  */
  YYSYMBOL_whilestart = 85,                /* whilestart  */
  YYSYMBOL_whilecond = 86,                 /* whilecond  */
  YYSYMBOL_whilestmt = 87,                 /* whilestmt  */
  YYSYMBOL_N = 88,                         /* N  */
  YYSYMBOL_M = 89,                         /* M  */
  YYSYMBOL_E = 90,                         /* E  */
  YYSYMBOL_forprefix = 91,                 /* forprefix  */
  YYSYMBOL_forstmt = 92,                   /* forstmt  */
  YYSYMBOL_returnstmt = 93,                /* returnstmt  */
  YYSYMBOL_loopstmt = 94,                  /* loopstmt  */
  YYSYMBOL_95_8 = 95,                      /* $@8  */
  YYSYMBOL_break = 96,                     /* break  */
  YYSYMBOL_continue = 97                   /* continue  */
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

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
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

#if 1

/* The parser invokes alloca or malloc; define the necessary symbols.  */

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
# define YYCOPY_NEEDED 1
#endif /* 1 */

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
#define YYFINAL  75
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   809

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  48
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  50
/* YYNRULES -- Number of rules.  */
#define YYNRULES  115
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  199

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   284


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
       2,     2,     2,     2,     2,     2,     2,    36,     2,     2,
      41,    42,    34,    32,    44,    33,    38,    35,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    46,    43,
      31,    29,    30,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    39,     2,    40,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    45,     2,    47,     2,     2,     2,     2,
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
      25,    26,    27,    28,    37
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    87,    87,    88,    91,    98,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   130,   131,
     131,   132,   132,   133,   133,   134,   137,   140,   141,   142,
     143,   144,   145,   147,   155,   171,   189,   190,   191,   192,
     193,   196,   197,   198,   199,   202,   203,   205,   214,   224,
     225,   227,   245,   252,   260,   261,   264,   265,   268,   269,
     271,   272,   275,   283,   296,   310,   311,   314,   325,   326,
     326,   329,   330,   333,   348,   354,   362,   370,   370,   370,
     386,   387,   388,   389,   390,   393,   394,   397,   406,   409,
     411,   426,   429,   438,   447,   448,   449,   449,   451,   461,
     473,   483,   495,   495,   497,   506
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "\"number\"",
  "\"string\"", "\"identifier\"", "\"if\"", "\"else\"", "\"while\"",
  "\"for\"", "\"function\"", "\"return\"", "\"break\"", "\"continue\"",
  "\"and\"", "\"not\"", "\"or\"", "\"local\"", "\"true\"", "\"false\"",
  "\"nil\"", "\"::\"", "\"..\"", "\"==\"", "\"!=\"", "\"++\"", "\"--\"",
  "\">=\"", "\"<=\"", "'='", "'>'", "'<'", "'+'", "'-'", "'*'", "'/'",
  "'%'", "UMINUS", "'.'", "'['", "']'", "'('", "')'", "';'", "','", "'{'",
  "':'", "'}'", "$accept", "program", "stmts", "stmt", "expr", "$@1",
  "$@2", "$@3", "$@4", "W", "term", "assignexpr", "primary", "lvalue",
  "member", "call", "callsuffix", "normcall", "methodcall", "elist",
  "objectdef", "indexed", "indexedelem", "block", "$@5", "funcname",
  "funcprefix", "funcargs", "funcbody", "funcdef", "$@6", "$@7", "const",
  "idlist", "ifprefix", "elseprefix", "ifstmt", "whilestart", "whilecond",
  "whilestmt", "N", "M", "E", "forprefix", "forstmt", "returnstmt",
  "loopstmt", "$@8", "break", "continue", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,    61,
      62,    60,    43,    45,    42,    47,    37,   284,    46,    91,
      93,    40,    41,    59,    44,   123,    58,   125
};
#endif

#define YYPACT_NINF (-161)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-4)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     165,  -161,  -161,  -161,  -161,   -39,  -161,    -4,    54,   119,
      20,    38,   467,    65,  -161,  -161,  -161,    81,    30,    30,
     467,   285,   442,  -161,    52,   101,   211,  -161,   536,  -161,
    -161,  -161,    80,  -161,    27,  -161,  -161,    62,  -161,  -161,
     254,  -161,    63,  -161,  -161,  -161,  -161,  -161,  -161,   467,
     467,  -161,  -161,  -161,   559,  -161,  -161,  -161,  -161,  -161,
      97,    23,    27,    23,  -161,  -161,   467,   720,   -35,     4,
    -161,   605,    68,  -161,   254,  -161,  -161,  -161,  -161,  -161,
    -161,   467,   467,   467,   467,   467,   467,   467,   467,   467,
    -161,   106,  -161,  -161,   467,   107,   467,   316,  -161,  -161,
    -161,   108,   467,   347,     1,  -161,   113,   467,  -161,   467,
     628,    70,    72,  -161,    83,   512,  -161,   467,  -161,    82,
    -161,    87,    13,  -161,  -161,   467,   467,   773,   773,   773,
     773,    56,    56,  -161,  -161,  -161,    89,   720,  -161,   674,
    -161,   -34,  -161,   697,  -161,   -15,  -161,  -161,    11,    86,
    -161,   254,   651,  -161,   254,    90,  -161,  -161,    87,   467,
     720,  -161,   378,  -161,   467,   467,   749,   749,   409,  -161,
    -161,  -161,  -161,  -161,   128,  -161,  -161,  -161,  -161,  -161,
    -161,   467,   487,  -161,    25,   763,   735,  -161,    40,  -161,
    -161,  -161,   582,  -161,  -161,  -161,  -161,  -161,  -161
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,    16,    90,    91,    52,     0,   101,     0,    82,     0,
       0,     0,     0,     0,    93,    94,    92,     0,     0,     0,
       0,     0,     0,    15,    79,     0,     0,     4,     0,    35,
      17,    38,    49,    51,    46,    47,    13,     0,    14,    48,
       0,     7,     0,     8,   104,     9,    10,    11,    12,     0,
     106,    81,    83,   110,     0,   114,   115,    44,    53,    54,
       0,    39,     0,    40,    43,    72,     0,    70,     0,     0,
      75,     0,     0,    78,     0,     1,     5,    31,    33,    27,
      29,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       6,     0,    41,    42,     0,     0,     0,     0,    61,    64,
      65,     0,     0,     0,     0,    87,    99,     0,   112,   106,
       0,   107,     0,   111,     0,     0,    73,     0,    74,     0,
      37,    50,     0,    36,    36,     0,     0,    24,    26,    23,
      25,    18,    19,    20,    22,    21,     0,    45,    55,     0,
      66,     0,    56,     0,    59,     0,    95,    84,     0,     0,
      98,     0,     0,   103,     0,     0,    97,   105,     0,     0,
      71,    76,     0,    80,     0,     0,    28,    30,     0,    57,
      67,    58,    60,    85,     0,    86,    88,   100,   102,   113,
     104,     0,     0,    62,     0,    32,    34,    68,     0,    96,
      89,   112,     0,    77,    63,    69,   104,   108,   109
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -161,  -161,    61,   -25,    -9,  -161,  -161,  -161,  -161,    17,
    -161,  -161,  -161,    31,  -161,    77,  -161,  -161,  -161,   -14,
    -161,  -161,    24,    -7,  -161,  -161,  -161,  -161,  -161,   -18,
    -161,  -161,  -161,  -161,  -161,  -161,  -161,  -161,  -161,  -161,
    -160,  -161,    37,  -161,  -161,  -161,   -44,  -161,  -161,  -161
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,    25,    26,    27,    28,   125,   126,   123,   124,   164,
      29,    30,    31,    32,    33,    34,    98,    99,   100,   111,
      35,    69,    70,    36,    74,    52,    37,   105,   176,    38,
     149,   190,    39,   148,    40,   151,    41,    42,   108,    43,
     109,   181,   112,    44,    45,    46,   153,   154,    47,    48
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      54,    76,    49,    57,    72,   116,   146,    68,   170,   117,
     117,    64,    67,    71,     1,   106,     2,     3,     4,     5,
     191,     6,     7,     8,     9,    10,    11,   172,    12,   117,
      13,    14,    15,    16,    17,     4,   198,    50,    18,    19,
     110,    67,   114,   147,   118,    91,    20,    13,   119,    61,
      63,    17,    21,   173,    22,   174,    23,   115,    24,    51,
     163,    95,    96,    55,    97,   101,   102,   194,   103,   117,
      58,    60,   127,   128,   129,   130,   131,   132,   133,   134,
     135,    56,   195,   141,   117,   137,    59,   139,    67,   145,
      87,    88,    89,   143,    67,    62,    62,    76,   152,    73,
      67,    75,    91,   104,   107,    92,    93,     8,   160,    94,
     121,   136,   138,   142,   117,   157,   166,   167,    95,    96,
     150,    97,     2,     3,     4,   158,   177,    66,   162,   179,
     168,    24,   180,   189,    12,   122,    13,    14,    15,    16,
      17,   165,   175,   161,    18,    19,   155,   196,   184,     0,
     182,     0,    20,    67,   188,   185,   186,     0,    21,    67,
      22,     0,    53,     0,     0,    -2,     1,     0,     2,     3,
       4,     5,   192,     6,     7,     8,     9,    10,    11,     0,
      12,     0,    13,    14,    15,    16,    17,     0,     0,     0,
      18,    19,     0,     0,     0,     0,     0,     0,    20,     0,
       0,     0,     0,     0,    21,     0,    22,     0,    23,     0,
      24,    -3,     1,     0,     2,     3,     4,     5,     0,     6,
       7,     8,     9,    10,    11,     0,    12,     0,    13,    14,
      15,    16,    17,     0,     0,     0,    18,    19,     0,     0,
       0,     0,     0,     0,    20,     0,     0,     0,     0,     0,
      21,     0,    22,     0,    23,     1,    24,     2,     3,     4,
       5,     0,     6,     7,     8,     9,    10,    11,     0,    12,
       0,    13,    14,    15,    16,    17,     0,     0,     0,    18,
      19,     0,     0,     0,     0,     0,     0,    20,     2,     3,
       4,     0,     0,    21,     0,    22,     0,    23,     0,    24,
      12,     0,    13,    14,    15,    16,    17,     0,     0,     0,
      18,    19,     0,     0,     0,     0,     0,     0,    20,     2,
       3,     4,     0,     0,    21,    65,    22,     0,     0,     0,
      66,    12,     0,    13,    14,    15,    16,    17,     0,     0,
       0,    18,    19,     0,     0,     0,     0,     0,     0,    20,
       2,     3,     4,     0,     0,    21,     0,    22,   140,     0,
       0,     0,    12,     0,    13,    14,    15,    16,    17,     0,
       0,     0,    18,    19,     0,     0,     0,     0,     0,     0,
      20,     2,     3,     4,     0,     0,    21,     0,    22,   144,
       0,     0,     0,    12,     0,    13,    14,    15,    16,    17,
       0,     0,     0,    18,    19,     0,     0,     0,     0,     0,
       0,    20,     2,     3,     4,     0,     0,    21,     0,    22,
     183,     0,     0,     0,    12,     0,    13,    14,    15,    16,
      17,     0,     0,     0,    18,    19,     0,     0,     0,     0,
       0,     0,    20,     0,     0,     2,     3,     4,    21,     0,
      22,   187,     8,     0,     0,     0,     0,    12,     0,    13,
      14,    15,    16,    17,     0,     0,     0,    18,    19,     0,
       2,     3,     4,     0,     0,    20,     0,     0,     0,     0,
       0,    21,    12,    22,    13,    14,    15,    16,    17,     0,
       0,     0,    18,    19,     0,     0,     0,     0,     0,     0,
      20,    77,     0,    78,     0,     0,    21,     0,    22,     0,
      79,    80,     0,     0,    81,    82,     0,    83,    84,    85,
      86,    87,    88,    89,     0,     0,    77,     0,    78,     0,
       0,     0,     0,     0,   193,    79,    80,     0,     0,    81,
      82,     0,    83,    84,    85,    86,    87,    88,    89,     0,
      77,     0,    78,     0,     0,     0,     0,     0,   159,    79,
      80,     0,     0,    81,    82,     0,    83,    84,    85,    86,
      87,    88,    89,    77,     0,    78,     0,     0,     0,    90,
       0,     0,    79,    80,     0,     0,    81,    82,     0,    83,
      84,    85,    86,    87,    88,    89,    77,     0,    78,     0,
       0,     0,   113,     0,     0,    79,    80,     0,     0,    81,
      82,     0,    83,    84,    85,    86,    87,    88,    89,    77,
       0,    78,     0,     0,     0,   197,     0,     0,    79,    80,
       0,     0,    81,    82,     0,    83,    84,    85,    86,    87,
      88,    89,    77,     0,    78,     0,     0,   120,     0,     0,
       0,    79,    80,     0,     0,    81,    82,     0,    83,    84,
      85,    86,    87,    88,    89,    77,     0,    78,     0,     0,
     156,     0,     0,     0,    79,    80,     0,     0,    81,    82,
       0,    83,    84,    85,    86,    87,    88,    89,    77,     0,
      78,     0,     0,   178,     0,     0,     0,    79,    80,     0,
       0,    81,    82,     0,    83,    84,    85,    86,    87,    88,
      89,    77,     0,    78,   169,     0,     0,     0,     0,     0,
      79,    80,     0,     0,    81,    82,     0,    83,    84,    85,
      86,    87,    88,    89,    77,     0,    78,   171,     0,     0,
       0,     0,     0,    79,    80,     0,     0,    81,    82,    77,
      83,    84,    85,    86,    87,    88,    89,     0,    79,    80,
       0,     0,    81,    82,     0,    83,    84,    85,    86,    87,
      88,    89,    -4,    -4,     0,     0,    81,    82,     0,    83,
      84,    85,    86,    87,    88,    89,    79,    80,     0,     0,
      81,    82,     0,    83,    84,    85,    86,    87,    88,    89,
      -4,    -4,     0,    -4,    -4,    85,    86,    87,    88,    89
};

static const yytype_int16 yycheck[] =
{
       9,    26,    41,    12,    22,    40,     5,    21,    42,    44,
      44,    20,    21,    22,     1,    40,     3,     4,     5,     6,
     180,     8,     9,    10,    11,    12,    13,    42,    15,    44,
      17,    18,    19,    20,    21,     5,   196,    41,    25,    26,
      49,    50,    60,    42,    40,    22,    33,    17,    44,    18,
      19,    21,    39,    42,    41,    44,    43,    66,    45,     5,
      47,    38,    39,    43,    41,    38,    39,    42,    41,    44,
       5,    41,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    43,    42,    97,    44,    94,     5,    96,    97,   103,
      34,    35,    36,   102,   103,    18,    19,   122,   107,    47,
     109,     0,    22,    41,    41,    25,    26,    10,   117,    29,
      42,     5,     5,     5,    44,    43,   125,   126,    38,    39,
       7,    41,     3,     4,     5,    42,   151,    45,    41,   154,
      41,    45,    42,     5,    15,    74,    17,    18,    19,    20,
      21,   124,   149,   119,    25,    26,   109,   191,   162,    -1,
     159,    -1,    33,   162,   168,   164,   165,    -1,    39,   168,
      41,    -1,    43,    -1,    -1,     0,     1,    -1,     3,     4,
       5,     6,   181,     8,     9,    10,    11,    12,    13,    -1,
      15,    -1,    17,    18,    19,    20,    21,    -1,    -1,    -1,
      25,    26,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      -1,    -1,    -1,    -1,    39,    -1,    41,    -1,    43,    -1,
      45,     0,     1,    -1,     3,     4,     5,     6,    -1,     8,
       9,    10,    11,    12,    13,    -1,    15,    -1,    17,    18,
      19,    20,    21,    -1,    -1,    -1,    25,    26,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,
      39,    -1,    41,    -1,    43,     1,    45,     3,     4,     5,
       6,    -1,     8,     9,    10,    11,    12,    13,    -1,    15,
      -1,    17,    18,    19,    20,    21,    -1,    -1,    -1,    25,
      26,    -1,    -1,    -1,    -1,    -1,    -1,    33,     3,     4,
       5,    -1,    -1,    39,    -1,    41,    -1,    43,    -1,    45,
      15,    -1,    17,    18,    19,    20,    21,    -1,    -1,    -1,
      25,    26,    -1,    -1,    -1,    -1,    -1,    -1,    33,     3,
       4,     5,    -1,    -1,    39,    40,    41,    -1,    -1,    -1,
      45,    15,    -1,    17,    18,    19,    20,    21,    -1,    -1,
      -1,    25,    26,    -1,    -1,    -1,    -1,    -1,    -1,    33,
       3,     4,     5,    -1,    -1,    39,    -1,    41,    42,    -1,
      -1,    -1,    15,    -1,    17,    18,    19,    20,    21,    -1,
      -1,    -1,    25,    26,    -1,    -1,    -1,    -1,    -1,    -1,
      33,     3,     4,     5,    -1,    -1,    39,    -1,    41,    42,
      -1,    -1,    -1,    15,    -1,    17,    18,    19,    20,    21,
      -1,    -1,    -1,    25,    26,    -1,    -1,    -1,    -1,    -1,
      -1,    33,     3,     4,     5,    -1,    -1,    39,    -1,    41,
      42,    -1,    -1,    -1,    15,    -1,    17,    18,    19,    20,
      21,    -1,    -1,    -1,    25,    26,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    -1,     3,     4,     5,    39,    -1,
      41,    42,    10,    -1,    -1,    -1,    -1,    15,    -1,    17,
      18,    19,    20,    21,    -1,    -1,    -1,    25,    26,    -1,
       3,     4,     5,    -1,    -1,    33,    -1,    -1,    -1,    -1,
      -1,    39,    15,    41,    17,    18,    19,    20,    21,    -1,
      -1,    -1,    25,    26,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    14,    -1,    16,    -1,    -1,    39,    -1,    41,    -1,
      23,    24,    -1,    -1,    27,    28,    -1,    30,    31,    32,
      33,    34,    35,    36,    -1,    -1,    14,    -1,    16,    -1,
      -1,    -1,    -1,    -1,    47,    23,    24,    -1,    -1,    27,
      28,    -1,    30,    31,    32,    33,    34,    35,    36,    -1,
      14,    -1,    16,    -1,    -1,    -1,    -1,    -1,    46,    23,
      24,    -1,    -1,    27,    28,    -1,    30,    31,    32,    33,
      34,    35,    36,    14,    -1,    16,    -1,    -1,    -1,    43,
      -1,    -1,    23,    24,    -1,    -1,    27,    28,    -1,    30,
      31,    32,    33,    34,    35,    36,    14,    -1,    16,    -1,
      -1,    -1,    43,    -1,    -1,    23,    24,    -1,    -1,    27,
      28,    -1,    30,    31,    32,    33,    34,    35,    36,    14,
      -1,    16,    -1,    -1,    -1,    43,    -1,    -1,    23,    24,
      -1,    -1,    27,    28,    -1,    30,    31,    32,    33,    34,
      35,    36,    14,    -1,    16,    -1,    -1,    42,    -1,    -1,
      -1,    23,    24,    -1,    -1,    27,    28,    -1,    30,    31,
      32,    33,    34,    35,    36,    14,    -1,    16,    -1,    -1,
      42,    -1,    -1,    -1,    23,    24,    -1,    -1,    27,    28,
      -1,    30,    31,    32,    33,    34,    35,    36,    14,    -1,
      16,    -1,    -1,    42,    -1,    -1,    -1,    23,    24,    -1,
      -1,    27,    28,    -1,    30,    31,    32,    33,    34,    35,
      36,    14,    -1,    16,    40,    -1,    -1,    -1,    -1,    -1,
      23,    24,    -1,    -1,    27,    28,    -1,    30,    31,    32,
      33,    34,    35,    36,    14,    -1,    16,    40,    -1,    -1,
      -1,    -1,    -1,    23,    24,    -1,    -1,    27,    28,    14,
      30,    31,    32,    33,    34,    35,    36,    -1,    23,    24,
      -1,    -1,    27,    28,    -1,    30,    31,    32,    33,    34,
      35,    36,    23,    24,    -1,    -1,    27,    28,    -1,    30,
      31,    32,    33,    34,    35,    36,    23,    24,    -1,    -1,
      27,    28,    -1,    30,    31,    32,    33,    34,    35,    36,
      27,    28,    -1,    30,    31,    32,    33,    34,    35,    36
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     1,     3,     4,     5,     6,     8,     9,    10,    11,
      12,    13,    15,    17,    18,    19,    20,    21,    25,    26,
      33,    39,    41,    43,    45,    49,    50,    51,    52,    58,
      59,    60,    61,    62,    63,    68,    71,    74,    77,    80,
      82,    84,    85,    87,    91,    92,    93,    96,    97,    41,
      41,     5,    73,    43,    52,    43,    43,    52,     5,     5,
      41,    61,    63,    61,    52,    40,    45,    52,    67,    69,
      70,    52,    77,    47,    72,     0,    51,    14,    16,    23,
      24,    27,    28,    30,    31,    32,    33,    34,    35,    36,
      43,    22,    25,    26,    29,    38,    39,    41,    64,    65,
      66,    38,    39,    41,    41,    75,    51,    41,    86,    88,
      52,    67,    90,    43,    77,    52,    40,    44,    40,    44,
      42,    42,    50,    55,    56,    53,    54,    52,    52,    52,
      52,    52,    52,    52,    52,    52,     5,    52,     5,    52,
      42,    67,     5,    52,    42,    67,     5,    42,    81,    78,
       7,    83,    52,    94,    95,    90,    42,    43,    42,    46,
      52,    70,    41,    47,    57,    57,    52,    52,    41,    40,
      42,    40,    42,    42,    44,    71,    76,    51,    42,    51,
      42,    89,    52,    42,    67,    52,    52,    42,    67,     5,
      79,    88,    52,    47,    42,    42,    94,    43,    88
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    48,    49,    49,    50,    50,    51,    51,    51,    51,
      51,    51,    51,    51,    51,    51,    51,    52,    52,    52,
      52,    52,    52,    52,    52,    52,    52,    53,    52,    54,
      52,    55,    52,    56,    52,    52,    57,    58,    58,    58,
      58,    58,    58,    58,    58,    59,    60,    60,    60,    60,
      60,    61,    61,    61,    61,    62,    62,    62,    62,    63,
      63,    63,    63,    63,    64,    64,    65,    65,    66,    66,
      67,    67,    68,    68,    68,    69,    69,    70,    71,    72,
      71,    73,    73,    74,    75,    75,    76,    78,    79,    77,
      80,    80,    80,    80,    80,    81,    81,    82,    83,    84,
      84,    85,    86,    87,    88,    89,    90,    90,    91,    92,
      93,    93,    95,    94,    96,    97
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     1,     1,     2,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     0,     4,     0,
       4,     0,     5,     0,     5,     1,     0,     3,     1,     2,
       2,     2,     2,     2,     2,     3,     1,     1,     1,     1,
       3,     1,     1,     2,     2,     3,     3,     4,     4,     3,
       4,     2,     5,     6,     1,     1,     2,     3,     4,     5,
       1,     3,     2,     3,     3,     1,     3,     5,     2,     0,
       4,     1,     0,     2,     2,     3,     1,     0,     0,     5,
       1,     1,     1,     1,     1,     1,     3,     4,     1,     2,
       4,     1,     3,     3,     0,     0,     0,     1,     7,     7,
       2,     3,     0,     2,     2,     2
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        YY_LAC_DISCARD ("YYBACKUP");                              \
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

/* This macro is provided for backward compatibility. */
# ifndef YY_LOCATION_PRINT
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif


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
# ifdef YYPRINT
  if (yykind < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yykind], *yyvaluep);
# endif
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


/* Given a state stack such that *YYBOTTOM is its bottom, such that
   *YYTOP is either its top or is YYTOP_EMPTY to indicate an empty
   stack, and such that *YYCAPACITY is the maximum number of elements it
   can hold without a reallocation, make sure there is enough room to
   store YYADD more elements.  If not, allocate a new stack using
   YYSTACK_ALLOC, copy the existing elements, and adjust *YYBOTTOM,
   *YYTOP, and *YYCAPACITY to reflect the new capacity and memory
   location.  If *YYBOTTOM != YYBOTTOM_NO_FREE, then free the old stack
   using YYSTACK_FREE.  Return 0 if successful or if no reallocation is
   required.  Return YYENOMEM if memory is exhausted.  */
static int
yy_lac_stack_realloc (YYPTRDIFF_T *yycapacity, YYPTRDIFF_T yyadd,
#if YYDEBUG
                      char const *yydebug_prefix,
                      char const *yydebug_suffix,
#endif
                      yy_state_t **yybottom,
                      yy_state_t *yybottom_no_free,
                      yy_state_t **yytop, yy_state_t *yytop_empty)
{
  YYPTRDIFF_T yysize_old =
    *yytop == yytop_empty ? 0 : *yytop - *yybottom + 1;
  YYPTRDIFF_T yysize_new = yysize_old + yyadd;
  if (*yycapacity < yysize_new)
    {
      YYPTRDIFF_T yyalloc = 2 * yysize_new;
      yy_state_t *yybottom_new;
      /* Use YYMAXDEPTH for maximum stack size given that the stack
         should never need to grow larger than the main state stack
         needs to grow without LAC.  */
      if (YYMAXDEPTH < yysize_new)
        {
          YYDPRINTF ((stderr, "%smax size exceeded%s", yydebug_prefix,
                      yydebug_suffix));
          return YYENOMEM;
        }
      if (YYMAXDEPTH < yyalloc)
        yyalloc = YYMAXDEPTH;
      yybottom_new =
        YY_CAST (yy_state_t *,
                 YYSTACK_ALLOC (YY_CAST (YYSIZE_T,
                                         yyalloc * YYSIZEOF (*yybottom_new))));
      if (!yybottom_new)
        {
          YYDPRINTF ((stderr, "%srealloc failed%s", yydebug_prefix,
                      yydebug_suffix));
          return YYENOMEM;
        }
      if (*yytop != yytop_empty)
        {
          YYCOPY (yybottom_new, *yybottom, yysize_old);
          *yytop = yybottom_new + (yysize_old - 1);
        }
      if (*yybottom != yybottom_no_free)
        YYSTACK_FREE (*yybottom);
      *yybottom = yybottom_new;
      *yycapacity = yyalloc;
    }
  return 0;
}

/* Establish the initial context for the current lookahead if no initial
   context is currently established.

   We define a context as a snapshot of the parser stacks.  We define
   the initial context for a lookahead as the context in which the
   parser initially examines that lookahead in order to select a
   syntactic action.  Thus, if the lookahead eventually proves
   syntactically unacceptable (possibly in a later context reached via a
   series of reductions), the initial context can be used to determine
   the exact set of tokens that would be syntactically acceptable in the
   lookahead's place.  Moreover, it is the context after which any
   further semantic actions would be erroneous because they would be
   determined by a syntactically unacceptable token.

   YY_LAC_ESTABLISH should be invoked when a reduction is about to be
   performed in an inconsistent state (which, for the purposes of LAC,
   includes consistent states that don't know they're consistent because
   their default reductions have been disabled).  Iff there is a
   lookahead token, it should also be invoked before reporting a syntax
   error.  This latter case is for the sake of the debugging output.

   For parse.lac=full, the implementation of YY_LAC_ESTABLISH is as
   follows.  If no initial context is currently established for the
   current lookahead, then check if that lookahead can eventually be
   shifted if syntactic actions continue from the current context.
   Report a syntax error if it cannot.  */
#define YY_LAC_ESTABLISH                                                \
do {                                                                    \
  if (!yy_lac_established)                                              \
    {                                                                   \
      YYDPRINTF ((stderr,                                               \
                  "LAC: initial context established for %s\n",          \
                  yysymbol_name (yytoken)));                            \
      yy_lac_established = 1;                                           \
      switch (yy_lac (yyesa, &yyes, &yyes_capacity, yyssp, yytoken))    \
        {                                                               \
        case YYENOMEM:                                                  \
          goto yyexhaustedlab;                                          \
        case 1:                                                         \
          goto yyerrlab;                                                \
        }                                                               \
    }                                                                   \
} while (0)

/* Discard any previous initial lookahead context because of Event,
   which may be a lookahead change or an invalidation of the currently
   established initial context for the current lookahead.

   The most common example of a lookahead change is a shift.  An example
   of both cases is syntax error recovery.  That is, a syntax error
   occurs when the lookahead is syntactically erroneous for the
   currently established initial context, so error recovery manipulates
   the parser stacks to try to find a new initial context in which the
   current lookahead is syntactically acceptable.  If it fails to find
   such a context, it discards the lookahead.  */
#if YYDEBUG
# define YY_LAC_DISCARD(Event)                                           \
do {                                                                     \
  if (yy_lac_established)                                                \
    {                                                                    \
      YYDPRINTF ((stderr, "LAC: initial context discarded due to "       \
                  Event "\n"));                                          \
      yy_lac_established = 0;                                            \
    }                                                                    \
} while (0)
#else
# define YY_LAC_DISCARD(Event) yy_lac_established = 0
#endif

/* Given the stack whose top is *YYSSP, return 0 iff YYTOKEN can
   eventually (after perhaps some reductions) be shifted, return 1 if
   not, or return YYENOMEM if memory is exhausted.  As preconditions and
   postconditions: *YYES_CAPACITY is the allocated size of the array to
   which *YYES points, and either *YYES = YYESA or *YYES points to an
   array allocated with YYSTACK_ALLOC.  yy_lac may overwrite the
   contents of either array, alter *YYES and *YYES_CAPACITY, and free
   any old *YYES other than YYESA.  */
static int
yy_lac (yy_state_t *yyesa, yy_state_t **yyes,
        YYPTRDIFF_T *yyes_capacity, yy_state_t *yyssp, yysymbol_kind_t yytoken)
{
  yy_state_t *yyes_prev = yyssp;
  yy_state_t *yyesp = yyes_prev;
  /* Reduce until we encounter a shift and thereby accept the token.  */
  YYDPRINTF ((stderr, "LAC: checking lookahead %s:", yysymbol_name (yytoken)));
  if (yytoken == YYSYMBOL_YYUNDEF)
    {
      YYDPRINTF ((stderr, " Always Err\n"));
      return 1;
    }
  while (1)
    {
      int yyrule = yypact[+*yyesp];
      if (yypact_value_is_default (yyrule)
          || (yyrule += yytoken) < 0 || YYLAST < yyrule
          || yycheck[yyrule] != yytoken)
        {
          /* Use the default action.  */
          yyrule = yydefact[+*yyesp];
          if (yyrule == 0)
            {
              YYDPRINTF ((stderr, " Err\n"));
              return 1;
            }
        }
      else
        {
          /* Use the action from yytable.  */
          yyrule = yytable[yyrule];
          if (yytable_value_is_error (yyrule))
            {
              YYDPRINTF ((stderr, " Err\n"));
              return 1;
            }
          if (0 < yyrule)
            {
              YYDPRINTF ((stderr, " S%d\n", yyrule));
              return 0;
            }
          yyrule = -yyrule;
        }
      /* By now we know we have to simulate a reduce.  */
      YYDPRINTF ((stderr, " R%d", yyrule - 1));
      {
        /* Pop the corresponding number of values from the stack.  */
        YYPTRDIFF_T yylen = yyr2[yyrule];
        /* First pop from the LAC stack as many tokens as possible.  */
        if (yyesp != yyes_prev)
          {
            YYPTRDIFF_T yysize = yyesp - *yyes + 1;
            if (yylen < yysize)
              {
                yyesp -= yylen;
                yylen = 0;
              }
            else
              {
                yyesp = yyes_prev;
                yylen -= yysize;
              }
          }
        /* Only afterwards look at the main stack.  */
        if (yylen)
          yyesp = yyes_prev -= yylen;
      }
      /* Push the resulting state of the reduction.  */
      {
        yy_state_fast_t yystate;
        {
          const int yylhs = yyr1[yyrule] - YYNTOKENS;
          const int yyi = yypgoto[yylhs] + *yyesp;
          yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyesp
                     ? yytable[yyi]
                     : yydefgoto[yylhs]);
        }
        if (yyesp == yyes_prev)
          {
            yyesp = *yyes;
            YY_IGNORE_USELESS_CAST_BEGIN
            *yyesp = YY_CAST (yy_state_t, yystate);
            YY_IGNORE_USELESS_CAST_END
          }
        else
          {
            if (yy_lac_stack_realloc (yyes_capacity, 1,
#if YYDEBUG
                                      " (", ")",
#endif
                                      yyes, yyesa, &yyesp, yyes_prev))
              {
                YYDPRINTF ((stderr, "\n"));
                return YYENOMEM;
              }
            YY_IGNORE_USELESS_CAST_BEGIN
            *++yyesp = YY_CAST (yy_state_t, yystate);
            YY_IGNORE_USELESS_CAST_END
          }
        YYDPRINTF ((stderr, " G%d", yystate));
      }
    }
}

/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yy_state_t *yyesa;
  yy_state_t **yyes;
  YYPTRDIFF_T *yyes_capacity;
  yysymbol_kind_t yytoken;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;

  int yyx;
  for (yyx = 0; yyx < YYNTOKENS; ++yyx)
    {
      yysymbol_kind_t yysym = YY_CAST (yysymbol_kind_t, yyx);
      if (yysym != YYSYMBOL_YYerror && yysym != YYSYMBOL_YYUNDEF)
        switch (yy_lac (yyctx->yyesa, yyctx->yyes, yyctx->yyes_capacity, yyctx->yyssp, yysym))
          {
          case YYENOMEM:
            return YYENOMEM;
          case 1:
            continue;
          default:
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = yysym;
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}




#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
# endif
#endif

#ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;
      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
#endif


static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
       In the first two cases, it might appear that the current syntax
       error should have been detected in the previous state when yy_lac
       was invoked.  However, at that time, there might have been a
       different syntax error that discarded a different initial context
       during error recovery, leaving behind the current lookahead.
  */
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      YYDPRINTF ((stderr, "Constructing syntax error message\n"));
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else if (yyn == 0)
        YYDPRINTF ((stderr, "No expected tokens.\n"));
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.  In order to see if a particular token T is a
   valid looakhead, invoke yy_lac (YYESA, YYES, YYES_CAPACITY, YYSSP, T).

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store or if
   yy_lac returned YYENOMEM.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
      case N:                               \
        yyformat = S;                       \
        break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yytnamerr (YY_NULLPTR, yytname[yyarg[yyi]]);
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yytname[yyarg[yyi++]]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}


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

    yy_state_t yyesa[20];
    yy_state_t *yyes = yyesa;
    YYPTRDIFF_T yyes_capacity = 20 < YYMAXDEPTH ? 20 : YYMAXDEPTH;

  /* Whether LAC context is established.  A Boolean.  */
  int yy_lac_established = 0;
  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

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
    goto yyexhaustedlab;
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
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
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
    {
      YY_LAC_ESTABLISH;
      goto yydefault;
    }
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      YY_LAC_ESTABLISH;
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
  YY_LAC_DISCARD ("shift");
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
  {
    int yychar_backup = yychar;
    switch (yyn)
      {
  case 2: /* program: %empty  */
#line 87 "Alpha_Compiler/parser.y"
                                {}
#line 1958 "parser.c"
    break;

  case 3: /* program: stmts  */
#line 88 "Alpha_Compiler/parser.y"
                                        {free((yyvsp[0].stmtVal));}
#line 1964 "parser.c"
    break;

  case 4: /* stmts: stmt  */
#line 92 "Alpha_Compiler/parser.y"
                        {
				(yyval.stmtVal) = make_stmt();
				(yyval.stmtVal)->breaklist = (yyvsp[0].stmtVal)->breaklist;
				(yyval.stmtVal)->contlist = (yyvsp[0].stmtVal)->contlist;
				free((yyvsp[0].stmtVal));
			}
#line 1975 "parser.c"
    break;

  case 5: /* stmts: stmts stmt  */
#line 99 "Alpha_Compiler/parser.y"
                        {
				(yyvsp[-1].stmtVal)->breaklist = mergelist((yyvsp[-1].stmtVal)->breaklist, (yyvsp[0].stmtVal)->breaklist);
				(yyvsp[-1].stmtVal)->contlist = mergelist((yyvsp[-1].stmtVal)->contlist, (yyvsp[0].stmtVal)->contlist);
				(yyval.stmtVal) = (yyvsp[-1].stmtVal);
				free((yyvsp[0].stmtVal));
			}
#line 1986 "parser.c"
    break;

  case 6: /* stmt: expr ';'  */
#line 107 "Alpha_Compiler/parser.y"
                                {do_backpatching((yyvsp[-1].exprNode)); (yyval.stmtVal) = make_stmt();}
#line 1992 "parser.c"
    break;

  case 7: /* stmt: ifstmt  */
#line 108 "Alpha_Compiler/parser.y"
                                        {(yyval.stmtVal) = (yyvsp[0].stmtVal);}
#line 1998 "parser.c"
    break;

  case 8: /* stmt: whilestmt  */
#line 109 "Alpha_Compiler/parser.y"
                                {(yyval.stmtVal) = make_stmt();}
#line 2004 "parser.c"
    break;

  case 9: /* stmt: forstmt  */
#line 110 "Alpha_Compiler/parser.y"
                                        {(yyval.stmtVal) = make_stmt();}
#line 2010 "parser.c"
    break;

  case 10: /* stmt: returnstmt  */
#line 111 "Alpha_Compiler/parser.y"
                                {(yyval.stmtVal) = make_stmt();}
#line 2016 "parser.c"
    break;

  case 11: /* stmt: break  */
#line 112 "Alpha_Compiler/parser.y"
                                        {(yyval.stmtVal) = (yyvsp[0].stmtVal);}
#line 2022 "parser.c"
    break;

  case 12: /* stmt: continue  */
#line 113 "Alpha_Compiler/parser.y"
                                {(yyval.stmtVal) = (yyvsp[0].stmtVal);}
#line 2028 "parser.c"
    break;

  case 13: /* stmt: block  */
#line 114 "Alpha_Compiler/parser.y"
                                        {(yyval.stmtVal) = (yyvsp[0].stmtVal);}
#line 2034 "parser.c"
    break;

  case 14: /* stmt: funcdef  */
#line 115 "Alpha_Compiler/parser.y"
                                        {(yyval.stmtVal) = make_stmt();}
#line 2040 "parser.c"
    break;

  case 15: /* stmt: ';'  */
#line 116 "Alpha_Compiler/parser.y"
                                        {resetTemp(); (yyval.stmtVal) = make_stmt();}
#line 2046 "parser.c"
    break;

  case 16: /* stmt: error  */
#line 117 "Alpha_Compiler/parser.y"
                                        {yyclearin; resetTemp(); (yyval.stmtVal) = make_stmt();}
#line 2052 "parser.c"
    break;

  case 17: /* expr: assignexpr  */
#line 120 "Alpha_Compiler/parser.y"
                                {(yyval.exprNode) = (yyvsp[0].exprNode);}
#line 2058 "parser.c"
    break;

  case 18: /* expr: expr '+' expr  */
#line 121 "Alpha_Compiler/parser.y"
                                {(yyval.exprNode) = do_arithop(add, (yyvsp[-2].exprNode), (yyvsp[0].exprNode), "addition");}
#line 2064 "parser.c"
    break;

  case 19: /* expr: expr '-' expr  */
#line 122 "Alpha_Compiler/parser.y"
                                {(yyval.exprNode) = do_arithop(sub, (yyvsp[-2].exprNode), (yyvsp[0].exprNode), "subtraction");}
#line 2070 "parser.c"
    break;

  case 20: /* expr: expr '*' expr  */
#line 123 "Alpha_Compiler/parser.y"
                                {(yyval.exprNode) = do_arithop(mul, (yyvsp[-2].exprNode), (yyvsp[0].exprNode), "multiplication");}
#line 2076 "parser.c"
    break;

  case 21: /* expr: expr '%' expr  */
#line 124 "Alpha_Compiler/parser.y"
                                {(yyval.exprNode) = do_arithop(mod, (yyvsp[-2].exprNode), (yyvsp[0].exprNode), "modulo");}
#line 2082 "parser.c"
    break;

  case 22: /* expr: expr '/' expr  */
#line 125 "Alpha_Compiler/parser.y"
                                {(yyval.exprNode) = do_arithop(division, (yyvsp[-2].exprNode), (yyvsp[0].exprNode), "division");}
#line 2088 "parser.c"
    break;

  case 23: /* expr: expr '>' expr  */
#line 126 "Alpha_Compiler/parser.y"
                                {(yyval.exprNode) = do_relop(if_greater, (yyvsp[-2].exprNode), (yyvsp[0].exprNode), "greater");}
#line 2094 "parser.c"
    break;

  case 24: /* expr: expr ">=" expr  */
#line 127 "Alpha_Compiler/parser.y"
                                {(yyval.exprNode) = do_relop(if_greatereq, (yyvsp[-2].exprNode), (yyvsp[0].exprNode), "greater or equal");}
#line 2100 "parser.c"
    break;

  case 25: /* expr: expr '<' expr  */
#line 128 "Alpha_Compiler/parser.y"
                                {(yyval.exprNode) = do_relop(if_less, (yyvsp[-2].exprNode), (yyvsp[0].exprNode), "less");}
#line 2106 "parser.c"
    break;

  case 26: /* expr: expr "<=" expr  */
#line 129 "Alpha_Compiler/parser.y"
                                {(yyval.exprNode) = do_relop(if_lesseq, (yyvsp[-2].exprNode), (yyvsp[0].exprNode), "less or equal");}
#line 2112 "parser.c"
    break;

  case 27: /* $@1: %empty  */
#line 130 "Alpha_Compiler/parser.y"
                     {do_backpatching((yyvsp[-1].exprNode));}
#line 2118 "parser.c"
    break;

  case 28: /* expr: expr "==" $@1 expr  */
#line 130 "Alpha_Compiler/parser.y"
                                                 {do_backpatching((yyvsp[0].exprNode)); (yyval.exprNode) = do_relop(if_eq, (yyvsp[-3].exprNode), (yyvsp[0].exprNode), "");}
#line 2124 "parser.c"
    break;

  case 29: /* $@2: %empty  */
#line 131 "Alpha_Compiler/parser.y"
                     {do_backpatching((yyvsp[-1].exprNode));}
#line 2130 "parser.c"
    break;

  case 30: /* expr: expr "!=" $@2 expr  */
#line 131 "Alpha_Compiler/parser.y"
                                                 {do_backpatching((yyvsp[0].exprNode)); (yyval.exprNode) = do_relop(if_noteq, (yyvsp[-3].exprNode), (yyvsp[0].exprNode), "");}
#line 2136 "parser.c"
    break;

  case 31: /* $@3: %empty  */
#line 132 "Alpha_Compiler/parser.y"
                    {emit_ifnotboolexpr((yyvsp[-1].exprNode));}
#line 2142 "parser.c"
    break;

  case 32: /* expr: expr "and" $@3 W expr  */
#line 132 "Alpha_Compiler/parser.y"
                                                     {(yyval.exprNode) = do_boolop(and, (yyvsp[-4].exprNode), (yyvsp[0].exprNode), (yyvsp[-1].numVal));}
#line 2148 "parser.c"
    break;

  case 33: /* $@4: %empty  */
#line 133 "Alpha_Compiler/parser.y"
                    {emit_ifnotboolexpr((yyvsp[-1].exprNode));}
#line 2154 "parser.c"
    break;

  case 34: /* expr: expr "or" $@4 W expr  */
#line 133 "Alpha_Compiler/parser.y"
                                                     {(yyval.exprNode) = do_boolop(or, (yyvsp[-4].exprNode), (yyvsp[0].exprNode), (yyvsp[-1].numVal));}
#line 2160 "parser.c"
    break;

  case 35: /* expr: term  */
#line 134 "Alpha_Compiler/parser.y"
                 {(yyval.exprNode) = (yyvsp[0].exprNode);}
#line 2166 "parser.c"
    break;

  case 36: /* W: %empty  */
#line 137 "Alpha_Compiler/parser.y"
     {(yyval.numVal) = nextQuadLabel();}
#line 2172 "parser.c"
    break;

  case 37: /* term: '(' expr ')'  */
#line 140 "Alpha_Compiler/parser.y"
                          {(yyval.exprNode) = (yyvsp[-1].exprNode);}
#line 2178 "parser.c"
    break;

  case 38: /* term: primary  */
#line 141 "Alpha_Compiler/parser.y"
                                          {(yyval.exprNode) = (yyvsp[0].exprNode);}
#line 2184 "parser.c"
    break;

  case 39: /* term: "++" lvalue  */
#line 142 "Alpha_Compiler/parser.y"
                                  {check_arith((yyvsp[0].exprNode), "left increment");  (yyval.exprNode) = do_prefixcalculation((yyvsp[0].exprNode), add);}
#line 2190 "parser.c"
    break;

  case 40: /* term: "--" lvalue  */
#line 143 "Alpha_Compiler/parser.y"
                                  {check_arith((yyvsp[0].exprNode), "left decrement");  (yyval.exprNode) = do_prefixcalculation((yyvsp[0].exprNode), sub);}
#line 2196 "parser.c"
    break;

  case 41: /* term: lvalue "++"  */
#line 144 "Alpha_Compiler/parser.y"
                                  {check_arith((yyvsp[-1].exprNode), "right increment"); (yyval.exprNode) = do_postfixcalculation((yyvsp[-1].exprNode), add);}
#line 2202 "parser.c"
    break;

  case 42: /* term: lvalue "--"  */
#line 145 "Alpha_Compiler/parser.y"
                                  {check_arith((yyvsp[-1].exprNode), "right decrement"); (yyval.exprNode) = do_postfixcalculation((yyvsp[-1].exprNode), sub);}
#line 2208 "parser.c"
    break;

  case 43: /* term: '-' expr  */
#line 148 "Alpha_Compiler/parser.y"
                        {
				check_arith((yyvsp[0].exprNode), "unary operation");
				(yyval.exprNode) = newexpr(arithexpr_e);
				(yyval.exprNode)->sym = istempexpr((yyvsp[0].exprNode)) ? (yyvsp[0].exprNode)->sym : newTemp();
				emit(mul, (yyval.exprNode), (yyvsp[0].exprNode), newexpr_constnum(-1), 0, yylineno);
			}
#line 2219 "parser.c"
    break;

  case 44: /* term: "not" expr  */
#line 156 "Alpha_Compiler/parser.y"
                        {
				(yyval.exprNode) = newexpr(boolexpr_e);
				(yyval.exprNode)->sym = istempexpr((yyvsp[0].exprNode)) ? (yyvsp[0].exprNode)->sym : newTemp();
				if((yyvsp[0].exprNode)->type != boolexpr_e) {
					(yyval.exprNode)->truelist = makeSceEntry(nextQuadLabel()+1);
					(yyval.exprNode)->falselist = makeSceEntry(nextQuadLabel());
					emit(if_eq, NULL, (yyvsp[0].exprNode), newexpr_constbool(1), 0, yylineno);
					emit(jump, NULL, NULL, NULL, 0, yylineno);
				} else {
					(yyval.exprNode)->truelist = (yyvsp[0].exprNode)->falselist;
					(yyval.exprNode)->falselist = (yyvsp[0].exprNode)->truelist;
				}
			}
#line 2237 "parser.c"
    break;

  case 45: /* assignexpr: lvalue '=' expr  */
#line 172 "Alpha_Compiler/parser.y"
                                {
					if((yyvsp[-2].exprNode) && ((yyvsp[-2].exprNode)->type == programfunc_e || (yyvsp[-2].exprNode)->type == libraryfunc_e)) 
						yyerror("syntax error, can't change value of function ", CYN, (yyvsp[-2].exprNode)->sym->value.funcVal->name, RESET, ", functions are constant");
					do_backpatching((yyvsp[0].exprNode));
					if((yyvsp[-2].exprNode)->type == tableitem_e) {
						emit(tablesetelem, (yyvsp[-2].exprNode), (yyvsp[-2].exprNode)->index, (yyvsp[0].exprNode), 0, yylineno);
						(yyval.exprNode) = emit_iftableitem((yyvsp[-2].exprNode));
						(yyval.exprNode)->type = assignexpr_e;
					} else {
						emit(assign, (yyvsp[-2].exprNode), (yyvsp[0].exprNode), NULL, 0, yylineno);
						(yyval.exprNode) = newexpr(assignexpr_e);
						(yyval.exprNode)->sym = newTemp();
						emit(assign, (yyval.exprNode), (yyvsp[-2].exprNode), NULL, 0, yylineno);
					}
				}
#line 2257 "parser.c"
    break;

  case 46: /* primary: call  */
#line 189 "Alpha_Compiler/parser.y"
                                        {(yyval.exprNode) = (yyvsp[0].exprNode);}
#line 2263 "parser.c"
    break;

  case 47: /* primary: objectdef  */
#line 190 "Alpha_Compiler/parser.y"
                                        {(yyval.exprNode) = (yyvsp[0].exprNode);}
#line 2269 "parser.c"
    break;

  case 48: /* primary: const  */
#line 191 "Alpha_Compiler/parser.y"
                                                {(yyval.exprNode) = (yyvsp[0].exprNode);}
#line 2275 "parser.c"
    break;

  case 49: /* primary: lvalue  */
#line 192 "Alpha_Compiler/parser.y"
                                                {(yyval.exprNode) = emit_iftableitem((yyvsp[0].exprNode));}
#line 2281 "parser.c"
    break;

  case 50: /* primary: '(' funcdef ')'  */
#line 193 "Alpha_Compiler/parser.y"
                                        {(yyval.exprNode) = newexpr(programfunc_e); (yyval.exprNode)->sym = (yyvsp[-1].symbol);}
#line 2287 "parser.c"
    break;

  case 51: /* lvalue: member  */
#line 196 "Alpha_Compiler/parser.y"
                          {(yyval.exprNode) = (yyvsp[0].exprNode);}
#line 2293 "parser.c"
    break;

  case 52: /* lvalue: "identifier"  */
#line 197 "Alpha_Compiler/parser.y"
                              {SymbolTableEntry *sym; (yyval.exprNode) = (sym = symtable_ID((yyvsp[0].strVal))) ? lvalue_expr(sym) : newexpr(nil_e);}
#line 2299 "parser.c"
    break;

  case 53: /* lvalue: "local" "identifier"  */
#line 198 "Alpha_Compiler/parser.y"
                          {SymbolTableEntry *sym; (yyval.exprNode) = (sym = symtable_LOCAL_ID((yyvsp[0].strVal))) ? lvalue_expr(sym) : newexpr(nil_e);}
#line 2305 "parser.c"
    break;

  case 54: /* lvalue: "::" "identifier"  */
#line 199 "Alpha_Compiler/parser.y"
                          {SymbolTableEntry *sym; (yyval.exprNode) = (sym = symtable_GLOBAL_ID((yyvsp[0].strVal))) ? lvalue_expr(sym) : newexpr(nil_e);}
#line 2311 "parser.c"
    break;

  case 55: /* member: lvalue '.' "identifier"  */
#line 202 "Alpha_Compiler/parser.y"
                                {(yyval.exprNode) = member_item((yyvsp[-2].exprNode), (yyvsp[0].strVal));}
#line 2317 "parser.c"
    break;

  case 56: /* member: call '.' "identifier"  */
#line 203 "Alpha_Compiler/parser.y"
                                        {(yyval.exprNode) = member_item((yyvsp[-2].exprNode), (yyvsp[0].strVal));}
#line 2323 "parser.c"
    break;

  case 57: /* member: lvalue '[' expr ']'  */
#line 206 "Alpha_Compiler/parser.y"
                        {
				do_backpatching((yyvsp[-1].exprNode));
				(yyvsp[-3].exprNode) = emit_iftableitem((yyvsp[-3].exprNode));
				(yyval.exprNode) = newexpr(tableitem_e);
				(yyval.exprNode)->sym = (yyvsp[-3].exprNode)->sym;
				(yyval.exprNode)->index = (yyvsp[-1].exprNode);
			}
#line 2335 "parser.c"
    break;

  case 58: /* member: call '[' expr ']'  */
#line 215 "Alpha_Compiler/parser.y"
                        {
				do_backpatching((yyvsp[-1].exprNode));
				(yyvsp[-3].exprNode) = emit_iftableitem((yyvsp[-3].exprNode));
				(yyval.exprNode) = newexpr(tableitem_e);
				(yyval.exprNode)->sym = (yyvsp[-3].exprNode)->sym;
				(yyval.exprNode)->index = (yyvsp[-1].exprNode);
			}
#line 2347 "parser.c"
    break;

  case 59: /* call: call '(' ')'  */
#line 224 "Alpha_Compiler/parser.y"
                                        {(yyval.exprNode) = make_call((yyvsp[-2].exprNode), NULL);}
#line 2353 "parser.c"
    break;

  case 60: /* call: call '(' elist ')'  */
#line 225 "Alpha_Compiler/parser.y"
                                        {(yyval.exprNode) = make_call((yyvsp[-3].exprNode), (yyvsp[-1].exprNode));}
#line 2359 "parser.c"
    break;

  case 61: /* call: lvalue callsuffix  */
#line 228 "Alpha_Compiler/parser.y"
                        {
				(yyvsp[-1].exprNode) = emit_iftableitem((yyvsp[-1].exprNode));
				if((yyvsp[0].Call)->method) {
					expr* t = (yyvsp[-1].exprNode);
					(yyvsp[-1].exprNode) = emit_iftableitem(member_item(t, (yyvsp[0].Call)->name));
					if(!(yyvsp[0].Call)->elist)
						(yyvsp[0].Call)->elist = t;
					else {
						expr* e;
						for(e = (yyvsp[0].Call)->elist; e->next; e = e->next);
						e->next = t;
					}
				}
				(yyval.exprNode) = make_call((yyvsp[-1].exprNode), (yyvsp[0].Call)->elist);
				free((yyvsp[0].Call));
			}
#line 2380 "parser.c"
    break;

  case 62: /* call: '(' funcdef ')' '(' ')'  */
#line 246 "Alpha_Compiler/parser.y"
                        {
				expr* func = newexpr(programfunc_e);
				func->sym = (yyvsp[-3].symbol);
				(yyval.exprNode) = make_call(func, NULL);
			}
#line 2390 "parser.c"
    break;

  case 63: /* call: '(' funcdef ')' '(' elist ')'  */
#line 253 "Alpha_Compiler/parser.y"
                        {
				expr* func = newexpr(programfunc_e);
				func->sym = (yyvsp[-4].symbol);
				(yyval.exprNode) = make_call(func, (yyvsp[-1].exprNode));
			}
#line 2400 "parser.c"
    break;

  case 64: /* callsuffix: normcall  */
#line 260 "Alpha_Compiler/parser.y"
                                {(yyval.Call) = (yyvsp[0].Call);}
#line 2406 "parser.c"
    break;

  case 65: /* callsuffix: methodcall  */
#line 261 "Alpha_Compiler/parser.y"
                                        {(yyval.Call) = (yyvsp[0].Call);}
#line 2412 "parser.c"
    break;

  case 66: /* normcall: '(' ')'  */
#line 264 "Alpha_Compiler/parser.y"
                                {(yyval.Call) = new_call(NULL, 0, NULL);}
#line 2418 "parser.c"
    break;

  case 67: /* normcall: '(' elist ')'  */
#line 265 "Alpha_Compiler/parser.y"
                                        {(yyval.Call) = new_call((yyvsp[-1].exprNode), 0, NULL);}
#line 2424 "parser.c"
    break;

  case 68: /* methodcall: ".." "identifier" '(' ')'  */
#line 268 "Alpha_Compiler/parser.y"
                                        {(yyval.Call) = new_call(NULL, 1, (yyvsp[-2].strVal));}
#line 2430 "parser.c"
    break;

  case 69: /* methodcall: ".." "identifier" '(' elist ')'  */
#line 269 "Alpha_Compiler/parser.y"
                                                {(yyval.Call) = new_call((yyvsp[-1].exprNode), 1, (yyvsp[-3].strVal));}
#line 2436 "parser.c"
    break;

  case 70: /* elist: expr  */
#line 271 "Alpha_Compiler/parser.y"
                                        {do_backpatching((yyvsp[0].exprNode)); (yyval.exprNode) = (yyvsp[0].exprNode);}
#line 2442 "parser.c"
    break;

  case 71: /* elist: elist ',' expr  */
#line 272 "Alpha_Compiler/parser.y"
                                        {do_backpatching((yyvsp[0].exprNode)); (yyvsp[0].exprNode)->next = (yyvsp[-2].exprNode); (yyval.exprNode) = (yyvsp[0].exprNode);}
#line 2448 "parser.c"
    break;

  case 72: /* objectdef: '[' ']'  */
#line 276 "Alpha_Compiler/parser.y"
                                {
					expr* t = newexpr(newtable_e);
					t->sym = newTemp();
					emit(tablecreate, t, NULL, NULL, 0, 0);
					(yyval.exprNode) = t;
				}
#line 2459 "parser.c"
    break;

  case 73: /* objectdef: '[' elist ']'  */
#line 284 "Alpha_Compiler/parser.y"
                                {
					expr* t = newexpr(newtable_e);
					t->sym = newTemp();
					emit(tablecreate, t, NULL, NULL, 0, 0);
					int len = -1;
					for(expr* e=(yyvsp[-1].exprNode); e; e=e->next) ++len;
					for(int i=len; (yyvsp[-1].exprNode); (yyvsp[-1].exprNode) = (yyvsp[-1].exprNode)->next)
						emit(tablesetelem, t, newexpr_constnum(len--), (yyvsp[-1].exprNode), 0, 0);
					(yyval.exprNode) = t;

				}
#line 2475 "parser.c"
    break;

  case 74: /* objectdef: '[' indexed ']'  */
#line 297 "Alpha_Compiler/parser.y"
                                {
					expr* t = newexpr(newtable_e);
					t->sym = newTemp();
					emit(tablecreate, t, NULL, NULL, 0, 0);
					int count = 0;
					for(indexed_s *i = (yyvsp[-1].indexedVal); i; i = i->next) {
						emit(tablesetelem, t, i->index, i->value, 0, 0);
					}
					(yyval.exprNode) = t;
					free((yyvsp[-1].indexedVal));
				}
#line 2491 "parser.c"
    break;

  case 75: /* indexed: indexedelem  */
#line 310 "Alpha_Compiler/parser.y"
                                                {(yyval.indexedVal) = (yyvsp[0].indexedVal);}
#line 2497 "parser.c"
    break;

  case 76: /* indexed: indexed ',' indexedelem  */
#line 311 "Alpha_Compiler/parser.y"
                                                {(yyvsp[0].indexedVal)->next = (yyvsp[-2].indexedVal);(yyval.indexedVal) = (yyvsp[0].indexedVal);}
#line 2503 "parser.c"
    break;

  case 77: /* indexedelem: '{' expr ':' expr '}'  */
#line 315 "Alpha_Compiler/parser.y"
                                {
					do_backpatching((yyvsp[-3].exprNode));
					do_backpatching((yyvsp[-1].exprNode));
					(yyval.indexedVal) = malloc(sizeof(indexed_s));
					(yyval.indexedVal)->index = (yyvsp[-3].exprNode);
					(yyval.indexedVal)->value = (yyvsp[-1].exprNode);
					(yyval.indexedVal)->next = (indexed_s*) 0;
				}
#line 2516 "parser.c"
    break;

  case 78: /* block: '{' '}'  */
#line 325 "Alpha_Compiler/parser.y"
                                                        {hide(current_scope+1); (yyval.stmtVal)=make_stmt();}
#line 2522 "parser.c"
    break;

  case 79: /* $@5: %empty  */
#line 326 "Alpha_Compiler/parser.y"
                {SCOPEUP;}
#line 2528 "parser.c"
    break;

  case 80: /* block: '{' $@5 stmts '}'  */
#line 326 "Alpha_Compiler/parser.y"
                                        {hide(current_scope); (yyval.stmtVal)=(yyvsp[-1].stmtVal); SCOPEDOWN;}
#line 2534 "parser.c"
    break;

  case 81: /* funcname: "identifier"  */
#line 329 "Alpha_Compiler/parser.y"
                                {(yyval.strVal) = (yyvsp[0].strVal);}
#line 2540 "parser.c"
    break;

  case 82: /* funcname: %empty  */
#line 330 "Alpha_Compiler/parser.y"
                                {(yyval.strVal) = newTempFuncName();}
#line 2546 "parser.c"
    break;

  case 83: /* funcprefix: "function" funcname  */
#line 334 "Alpha_Compiler/parser.y"
                                {
					emit(jump, NULL, NULL, NULL, 0, yylineno);
					pushFuncJump(nextQuadLabel());
					(yyval.symbol) = symtable_FUNC((yyvsp[0].strVal));
					(yyval.symbol)->value.funcVal->iaddress = nextQuadLabel();
					expr* e = newexpr(programfunc_e);
					e->sym = (yyval.symbol);
					emit(funcstart, e, NULL, NULL, 0, yylineno);
					pushCurrScopeOffset();
					enterScopeSpace();
					resetFormalArgsOffset();
				}
#line 2563 "parser.c"
    break;

  case 84: /* funcargs: '(' ')'  */
#line 349 "Alpha_Compiler/parser.y"
                        {
				enterScopeSpace();
				resetFunctionLocalsOffset();
				pushCurrLoopCounter(loopcounter);
			}
#line 2573 "parser.c"
    break;

  case 85: /* funcargs: '(' idlist ')'  */
#line 355 "Alpha_Compiler/parser.y"
                        {	
				enterScopeSpace();
				resetFunctionLocalsOffset();
				pushCurrLoopCounter(loopcounter);
			}
#line 2583 "parser.c"
    break;

  case 86: /* funcbody: block  */
#line 363 "Alpha_Compiler/parser.y"
                        {
				(yyval.numVal) = currScopeOffset();
				exitScopeSpace();
				loopcounter = popLoopCounter();
			}
#line 2593 "parser.c"
    break;

  case 87: /* $@6: %empty  */
#line 370 "Alpha_Compiler/parser.y"
                              {loopcounter = 0; FUNCUP;}
#line 2599 "parser.c"
    break;

  case 88: /* $@7: %empty  */
#line 370 "Alpha_Compiler/parser.y"
                                                                  {FUNCDOWN;}
#line 2605 "parser.c"
    break;

  case 89: /* funcdef: funcprefix funcargs $@6 funcbody $@7  */
#line 371 "Alpha_Compiler/parser.y"
                        {
				exitScopeSpace();
				(yyvsp[-4].symbol)->value.funcVal->totalLocals = (yyvsp[-1].numVal);
				int offset = popScopeOffset();
				restoreCurrScopeOffset(offset);
				expr* e = newexpr(programfunc_e);
				e->sym = (yyvsp[-4].symbol);
				(yyval.symbol) = (yyvsp[-4].symbol); 
				incomplFuncJumpsTop->end = nextQuadLabel();
				patchReturnJump();
				incomplFuncJumpsTop = incomplFuncJumpsTop->next;
				emit(funcend, e, NULL, NULL, 0, yylineno);
			}
#line 2623 "parser.c"
    break;

  case 90: /* const: "number"  */
#line 386 "Alpha_Compiler/parser.y"
                        {(yyval.exprNode) = newexpr_constnum((yyvsp[0].numVal));}
#line 2629 "parser.c"
    break;

  case 91: /* const: "string"  */
#line 387 "Alpha_Compiler/parser.y"
                                {(yyval.exprNode) = newexpr_conststring((yyvsp[0].strVal));}
#line 2635 "parser.c"
    break;

  case 92: /* const: "nil"  */
#line 388 "Alpha_Compiler/parser.y"
                                {(yyval.exprNode) = newexpr(nil_e);}
#line 2641 "parser.c"
    break;

  case 93: /* const: "true"  */
#line 389 "Alpha_Compiler/parser.y"
                                {(yyval.exprNode) = newexpr_constbool(1);}
#line 2647 "parser.c"
    break;

  case 94: /* const: "false"  */
#line 390 "Alpha_Compiler/parser.y"
                                {(yyval.exprNode) = newexpr_constbool(0);}
#line 2653 "parser.c"
    break;

  case 95: /* idlist: "identifier"  */
#line 393 "Alpha_Compiler/parser.y"
                                        {SCOPEUP; FUNCUP; symtable_IDLIST((yyvsp[0].strVal)); SCOPEDOWN; FUNCDOWN;}
#line 2659 "parser.c"
    break;

  case 96: /* idlist: idlist ',' "identifier"  */
#line 394 "Alpha_Compiler/parser.y"
                                {SCOPEUP; FUNCUP; symtable_IDLIST((yyvsp[0].strVal)); SCOPEDOWN; FUNCDOWN;}
#line 2665 "parser.c"
    break;

  case 97: /* ifprefix: "if" '(' expr ')'  */
#line 398 "Alpha_Compiler/parser.y"
                        {
				do_backpatching((yyvsp[-1].exprNode));
				emit(if_eq, NULL, (yyvsp[-1].exprNode), newexpr_constbool(1), nextQuadLabel()+2, yylineno);
				(yyval.numVal) = nextQuadLabel();
				emit(jump, NULL, NULL, NULL, 0, yylineno);
			}
#line 2676 "parser.c"
    break;

  case 98: /* elseprefix: "else"  */
#line 406 "Alpha_Compiler/parser.y"
                         {(yyval.numVal) = nextQuadLabel(); emit(jump, NULL, NULL, NULL, 0, yylineno);}
#line 2682 "parser.c"
    break;

  case 99: /* ifstmt: ifprefix stmt  */
#line 409 "Alpha_Compiler/parser.y"
                                        {patchLabel((yyvsp[-1].numVal), nextQuadLabel()); (yyval.stmtVal) = ((yyvsp[0].stmtVal)) ? (yyvsp[0].stmtVal) : make_stmt();}
#line 2688 "parser.c"
    break;

  case 100: /* ifstmt: ifprefix stmt elseprefix stmt  */
#line 412 "Alpha_Compiler/parser.y"
                        {
				patchLabel((yyvsp[-3].numVal), (yyvsp[-1].numVal)+1);
				patchLabel((yyvsp[-1].numVal), nextQuadLabel());
				(yyval.stmtVal) = make_stmt();
				if((yyvsp[-2].stmtVal) && (yyvsp[0].stmtVal)) {
					(yyval.stmtVal)->breaklist = mergelist((yyvsp[-2].stmtVal)->breaklist, (yyvsp[0].stmtVal)->breaklist);
					(yyval.stmtVal)->contlist = mergelist((yyvsp[-2].stmtVal)->contlist, (yyvsp[0].stmtVal)->contlist);
				}else if((yyvsp[-2].stmtVal))
					(yyval.stmtVal) = (yyvsp[-2].stmtVal);
				else if((yyvsp[0].stmtVal))
					(yyval.stmtVal) = (yyvsp[0].stmtVal);
			}
#line 2705 "parser.c"
    break;

  case 101: /* whilestart: "while"  */
#line 426 "Alpha_Compiler/parser.y"
                                {(yyval.numVal) = nextQuadLabel();}
#line 2711 "parser.c"
    break;

  case 102: /* whilecond: '(' expr ')'  */
#line 430 "Alpha_Compiler/parser.y"
                                {
					do_backpatching((yyvsp[-1].exprNode));
					emit(if_eq, NULL, (yyvsp[-1].exprNode), newexpr_constbool(1), nextQuadLabel()+2, yylineno);
					(yyval.numVal) = nextQuadLabel();
					emit(jump, NULL, NULL, NULL, 0, yylineno);
				}
#line 2722 "parser.c"
    break;

  case 103: /* whilestmt: whilestart whilecond loopstmt  */
#line 439 "Alpha_Compiler/parser.y"
                                {
					emit(jump, NULL, NULL, NULL, (yyvsp[-2].numVal), yylineno);
					patchLabel((yyvsp[-1].numVal), nextQuadLabel());
					patchlist((yyvsp[0].stmtVal)->breaklist, nextQuadLabel());
					patchlist((yyvsp[0].stmtVal)->contlist, (yyvsp[-2].numVal));
				}
#line 2733 "parser.c"
    break;

  case 104: /* N: %empty  */
#line 447 "Alpha_Compiler/parser.y"
    {(yyval.numVal) = nextQuadLabel(); emit(jump, NULL, NULL, NULL, 0, yylineno);}
#line 2739 "parser.c"
    break;

  case 105: /* M: %empty  */
#line 448 "Alpha_Compiler/parser.y"
    {(yyval.numVal) = nextQuadLabel();}
#line 2745 "parser.c"
    break;

  case 108: /* forprefix: "for" '(' E ';' M expr ';'  */
#line 452 "Alpha_Compiler/parser.y"
                                {
					do_backpatching((yyvsp[-1].exprNode));
					(yyval.forloopVal) = malloc(sizeof(forloop_i));
					(yyval.forloopVal)->test = (yyvsp[-2].numVal);
					(yyval.forloopVal)->enter = nextQuadLabel();
					emit(if_eq, NULL, (yyvsp[-1].exprNode), newexpr_constbool(1), 0, yylineno);
				}
#line 2757 "parser.c"
    break;

  case 109: /* forstmt: forprefix N E ')' N loopstmt N  */
#line 462 "Alpha_Compiler/parser.y"
                                {
					patchLabel((yyvsp[-6].forloopVal)->enter, (yyvsp[-2].numVal)+1);
					patchLabel((yyvsp[-5].numVal), nextQuadLabel());
					patchLabel((yyvsp[-2].numVal), (yyvsp[-6].forloopVal)->test);
					patchLabel((yyvsp[0].numVal), (yyvsp[-5].numVal)+1);
					patchlist((yyvsp[-1].stmtVal)->breaklist, nextQuadLabel());
					patchlist((yyvsp[-1].stmtVal)->contlist, (yyvsp[-5].numVal)+1);
					free((yyvsp[-6].forloopVal));
				}
#line 2771 "parser.c"
    break;

  case 110: /* returnstmt: "return" ';'  */
#line 474 "Alpha_Compiler/parser.y"
                                {
					if(!func_scope)
						yyerror("return statement not within function");
					else 
						pushRetJump(nextQuadLabel()+1);

					emit(ret, NULL, NULL, NULL, 0, yylineno);
					emit(jump, NULL, NULL, NULL, 0, yylineno);
				}
#line 2785 "parser.c"
    break;

  case 111: /* returnstmt: "return" expr ';'  */
#line 484 "Alpha_Compiler/parser.y"
                                {
					do_backpatching((yyvsp[-1].exprNode));
					if(!func_scope)
						yyerror("return statement not within function");
					else
						pushRetJump(nextQuadLabel()+1);

					emit(ret, (yyvsp[-1].exprNode), NULL, NULL, 0, yylineno);
					emit(jump, NULL, NULL, NULL, 0, yylineno);
				}
#line 2800 "parser.c"
    break;

  case 112: /* $@8: %empty  */
#line 495 "Alpha_Compiler/parser.y"
           {++loopcounter;}
#line 2806 "parser.c"
    break;

  case 113: /* loopstmt: $@8 stmt  */
#line 495 "Alpha_Compiler/parser.y"
                                 {--loopcounter; (yyval.stmtVal) = (yyvsp[0].stmtVal) ? (yyvsp[0].stmtVal) : make_stmt();}
#line 2812 "parser.c"
    break;

  case 114: /* break: "break" ';'  */
#line 498 "Alpha_Compiler/parser.y"
                        {
				if(!loopcounter)
					yyerror("break statement not within loop");
				(yyval.stmtVal) = make_stmt();
				(yyval.stmtVal)->breaklist = newlist(nextQuadLabel());
				emit(jump, NULL, NULL, NULL, 0, yylineno);
			}
#line 2824 "parser.c"
    break;

  case 115: /* continue: "continue" ';'  */
#line 507 "Alpha_Compiler/parser.y"
                        {
				if(!loopcounter)
					yyerror("continue statement not within loop");
				(yyval.stmtVal) = make_stmt();
				(yyval.stmtVal)->contlist = newlist(nextQuadLabel()); 
				emit(jump, NULL, NULL, NULL, 0, yylineno);
			}
#line 2836 "parser.c"
    break;


#line 2840 "parser.c"

        default: break;
      }
    if (yychar_backup != yychar)
      YY_LAC_DISCARD ("yychar change");
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
      {
        yypcontext_t yyctx
          = {yyssp, yyesa, &yyes, &yyes_capacity, yytoken};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        if (yychar != YYEMPTY)
          YY_LAC_ESTABLISH;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          goto yyexhaustedlab;
      }
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

  /* If the stack popping above didn't lose the initial context for the
     current lookahead token, the shift below will for sure.  */
  YY_LAC_DISCARD ("error recovery");

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
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if 1
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturn;
#endif


/*-------------------------------------------------------.
| yyreturn -- parsing is finished, clean up and return.  |
`-------------------------------------------------------*/
yyreturn:
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
  if (yyes != yyesa)
    YYSTACK_FREE (yyes);
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}

#line 515 "Alpha_Compiler/parser.y"
          

void freeExpr(expr* e) {
	for(int i=0; e && i<currQuad; ++i) {
		if(quads[i].arg1 == e)
			quads[i].arg1 = NULL;
		if(quads[i].arg2 == e)
			quads[i].arg2 = NULL;
		if(quads[i].result == e)
			quads[i].result = NULL;
	}
	free(e);
}

void freeQuads(void) {
	for(int i=0; i<currQuad; ++i) {
		freeExpr(quads[i].arg1);
		quads[i].arg1 = NULL;
		freeExpr(quads[i].arg2);
		quads[i].arg2 = NULL;
		freeExpr(quads[i].result);
		quads[i].result = NULL;
	}
	free(quads);
}

void freeSymbols(void) {
	for(int i=0; i<scopeList->len; i++) {
		SymbolTableEntry *e = scopeList->scopes[i].head;
		while(e) {
			SymbolTableEntry *tmp;
			tmp = e->next_inScope;

			if(e->type == USERFUNC || e->type == LIBFUNC) { 
				free((void*)e->value.funcVal->name);
				free(e->value.funcVal);
			} else { 
				free((void*)e->value.varVal->name);
				free(e->value.varVal);
			}
			free(e);
			e = tmp;
		}
	}
}

void freeFuncJumps(void) {
	incomplFuncJumps_t* index = incomplFuncJumpsHead;

	while(index) {
		incomplFuncJumps_t* tmp = index->prev;
		if(index->retLabels)
			free(index->retLabels);
		free(index);
		index = tmp;
	}
}

void freeAll(void) {	
	freeQuads();
	free(anonymousFuncBuf);
	freetempname();
	freeStacks();
	freeFuncJumps();
	freeSymbols();
	free(scopeList->scopes);
	free(scopeList);
	free(funcScopeList->scopes);
	free(funcScopeList);
	free(symTable);
}

int main (int argc, char **argv) {
	
	initSymTable();
	yyparse();
	
	if(warningcounter) {
		printf("\n\n");
		if(warningcounter>1)
			printf(MAGENTA BOLD"Compiled with %d warnings..."RESET, warningcounter);
		else 
			printf(MAGENTA BOLD"Compiled with 1 warning..."RESET);
	}
	if(errorcounter) {
		printf("\n\n");
		if(errorcounter>1)
			printf(RED BOLD"Compiled with %d errors..."RESET, errorcounter);
		else 
			printf(RED BOLD"Compiled with 1 error..."RESET);
		printf("\n\n");
		return 1;
	}
	if(!warningcounter) 
		printf(GREEN BOLD"\n\nCompiled successfully" RESET "\n\n");

	backpatchIncomplFuncJumps();
	generateTCode();

	int output_flag = 0;

	if(argc > 1) {
		for(int i=1; i<argc; ++i) {
			if(!strcmp(argv[i], "-s"))
				printScopeList(scopeList);
			if(!strcmp(argv[i], "-q"))
				printQuads();
			if(!strcmp(argv[i], "-o"))
				output_flag = i;
			if(!strcmp(argv[i], "-i")) {
				print_instructions();
				print_num_consts();
				print_string_consts();
				print_user_funcs();
				print_named_lib_funcs();
			}
		}
	}

	if(output_flag) {
		if(argc < output_flag + 1) {
			fprintf(stderr, RED BOLD"error: "RESET"-o flag was given but no name was specified");
			return -1;
		}
		generateBitCode(argv[output_flag+1]);
	}else {
		generateBitCode("alpha.abc");
	}

	freeAll();
	return 0;
}

void custom_yyerror(const char *str, ...) {
    va_list ap;

	fprintf(stderr, "\n"BOLD"%d"RESET": "BOLD RED"error"RESET":  ", yylineno);

    va_start(ap, str);
    while(str) {
        fprintf(stderr, "%s", str);
        str = va_arg(ap, const char*);
    }
    fprintf(stderr, "\n");
    va_end(ap);

	++errorcounter;
} 

void custom_warning(const char *str, ...) {
	va_list ap;

	fprintf(stderr, "\n"BOLD"%d"RESET": "BOLD MAGENTA"warning"RESET":  ", yylineno);

	va_start(ap, str);
	while(str) {
        fprintf(stderr, "%s", str);
        str = va_arg(ap, const char*);
    }
    fprintf(stderr, "\n");
    va_end(ap);

	++warningcounter;
}
