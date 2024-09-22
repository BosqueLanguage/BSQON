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
#line 1 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

int yylex(void);
void yyerror(const char* s, ...);

#define MK_SPOS_S(T) (createSourcePos((T).first_line, (T).first_column, (T).last_line, (T).last_column))
#define MK_SPOS_R(S, E) (createSourcePos((S).first_line, (S).first_column, (E).last_line, (E).last_column))

struct BSQON_TYPE_AST_Node* yybsqonval_type;
struct BSQON_AST_Node* yybsqonval;
char* filename = "<stdin>";

#define MAX_PARSER_ERRORS 128
#define MAX_ERROR_LENGTH 1024

char errorbuf[MAX_ERROR_LENGTH];
char* errors[MAX_PARSER_ERRORS];
int errorcount = 0;

#define YYDEBUG 1

#line 97 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"

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

#include "bsqon.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_KW_TRUE = 3,                    /* "true"  */
  YYSYMBOL_KW_FALSE = 4,                   /* "false"  */
  YYSYMBOL_KW_NONE = 5,                    /* "none"  */
  YYSYMBOL_KW_SOME = 6,                    /* "some"  */
  YYSYMBOL_KW_OK = 7,                      /* "ok"  */
  YYSYMBOL_KW_ERR = 8,                     /* "err"  */
  YYSYMBOL_KW_NPOS = 9,                    /* "$npos"  */
  YYSYMBOL_KW_INDEX = 10,                  /* "$i"  */
  YYSYMBOL_KW_KEY = 11,                    /* "$key"  */
  YYSYMBOL_KW_SRC = 12,                    /* "$src"  */
  YYSYMBOL_KW_LET = 13,                    /* "let"  */
  YYSYMBOL_KW_IN = 14,                     /* "in"  */
  YYSYMBOL_KW_ENV = 15,                    /* "env"  */
  YYSYMBOL_SYM_DOUBLE_COLON = 16,          /* "::"  */
  YYSYMBOL_SYM_ENTRY = 17,                 /* "=>"  */
  YYSYMBOL_SYM_COLON = 18,                 /* ":"  */
  YYSYMBOL_SYM_COMMA = 19,                 /* ","  */
  YYSYMBOL_SYM_EQUALS = 20,                /* "="  */
  YYSYMBOL_SYM_DOT = 21,                   /* "."  */
  YYSYMBOL_SYM_ELIST_LEFT = 22,            /* "(|"  */
  YYSYMBOL_SYM_ELIST_RIGHT = 23,           /* "|)"  */
  YYSYMBOL_SYM_LBRACK_BAR = 24,            /* "[|"  */
  YYSYMBOL_SYM_RBRACK_BAR = 25,            /* "|]"  */
  YYSYMBOL_TOKEN_NAT = 26,                 /* "nat literal"  */
  YYSYMBOL_TOKEN_INT = 27,                 /* "int literal"  */
  YYSYMBOL_TOKEN_BIG_NAT = 28,             /* "big nat literal"  */
  YYSYMBOL_TOKEN_BIG_INT = 29,             /* "big int literal"  */
  YYSYMBOL_TOKEN_RATIONAL = 30,            /* "rational literal"  */
  YYSYMBOL_TOKEN_FLOAT = 31,               /* "float literal"  */
  YYSYMBOL_TOKEN_DECIMAL = 32,             /* "decimal literal"  */
  YYSYMBOL_TOKEN_DECIMAL_DEGREE = 33,      /* "decimal degree literal"  */
  YYSYMBOL_TOKEN_COMPLEX = 34,             /* "complex literal"  */
  YYSYMBOL_TOKEN_LAT_LONG = 35,            /* "geo coordinate literal"  */
  YYSYMBOL_TOKEN_NUMBERINO = 36,           /* "numberino"  */
  YYSYMBOL_TOKEN_BYTE_BUFFER = 37,         /* "byte buffer"  */
  YYSYMBOL_TOKEN_UUID_V4 = 38,             /* "uuid (v4)"  */
  YYSYMBOL_TOKEN_UUID_V7 = 39,             /* "uuid (v7)"  */
  YYSYMBOL_TOKEN_SHA_HASH = 40,            /* "sha3 hashcode (512 bits)"  */
  YYSYMBOL_TOKEN_STRING = 41,              /* "string"  */
  YYSYMBOL_TOKEN_CSTRING = 42,             /* "cstring"  */
  YYSYMBOL_TOKEN_REGEX = 43,               /* "regular expression"  */
  YYSYMBOL_TOKEN_PATH_ITEM = 44,           /* "path item"  */
  YYSYMBOL_TOKEN_TZ_DATE_TIME = 45,        /* "date & time with timezone"  */
  YYSYMBOL_TOKEN_TIA_TIME = 46,            /* "date & time in TIA"  */
  YYSYMBOL_TOKEN_PLAIN_DATE = 47,          /* "plain date"  */
  YYSYMBOL_TOKEN_PLAIN_TIME = 48,          /* "plain time"  */
  YYSYMBOL_TOKEN_LOGICAL_TIME = 49,        /* "logical time"  */
  YYSYMBOL_TOKEN_TIMESTAMP = 50,           /* "ISO timestamp"  */
  YYSYMBOL_TOKEN_DELTA_DATE_TIME = 51,     /* "date & time delta"  */
  YYSYMBOL_TOKEN_DELTA_SECONDS = 52,       /* "delta in seconds"  */
  YYSYMBOL_TOKEN_DELTA_LOGICAL = 53,       /* "logical time delta"  */
  YYSYMBOL_TOKEN_DELTA_ISOTIMESTAMP = 54,  /* "timestamp delta"  */
  YYSYMBOL_TOKEN_IDENTIFIER = 55,          /* "identifier"  */
  YYSYMBOL_TOKEN_TYPE_COMPONENT = 56,      /* "type name"  */
  YYSYMBOL_TOKEN_UNSPEC_IDENTIFIER = 57,   /* "unspec identifier"  */
  YYSYMBOL_TOKEN_SHEBANG_LINE = 58,        /* "shebang line"  */
  YYSYMBOL_59_ = 59,                       /* '<'  */
  YYSYMBOL_60_ = 60,                       /* '>'  */
  YYSYMBOL_61_ = 61,                       /* '('  */
  YYSYMBOL_62_ = 62,                       /* ')'  */
  YYSYMBOL_63_ = 63,                       /* '{'  */
  YYSYMBOL_64_ = 64,                       /* '}'  */
  YYSYMBOL_YYACCEPT = 65,                  /* $accept  */
  YYSYMBOL_bsqontypel = 66,                /* bsqontypel  */
  YYSYMBOL_bsqontypel_entry = 67,          /* bsqontypel_entry  */
  YYSYMBOL_bsqonnametypel = 68,            /* bsqonnametypel  */
  YYSYMBOL_bsqonnametypel_entry = 69,      /* bsqonnametypel_entry  */
  YYSYMBOL_bsqonnominaltype = 70,          /* bsqonnominaltype  */
  YYSYMBOL_bsqontermslist = 71,            /* bsqontermslist  */
  YYSYMBOL_bsqonelisttype = 72,            /* bsqonelisttype  */
  YYSYMBOL_bsqontype = 73,                 /* bsqontype  */
  YYSYMBOL_bsqonliteral = 74,              /* bsqonliteral  */
  YYSYMBOL_bsqonunspecvar = 75,            /* bsqonunspecvar  */
  YYSYMBOL_bsqonidentifier = 76,           /* bsqonidentifier  */
  YYSYMBOL_bsqonscopedidentifier = 77,     /* bsqonscopedidentifier  */
  YYSYMBOL_bsqontypeliteral = 78,          /* bsqontypeliteral  */
  YYSYMBOL_bsqonenvaccess = 79,            /* bsqonenvaccess  */
  YYSYMBOL_bsqonterminal = 80,             /* bsqonterminal  */
  YYSYMBOL_bsqon_mapentry = 81,            /* bsqon_mapentry  */
  YYSYMBOL_bsqonvall = 82,                 /* bsqonvall  */
  YYSYMBOL_bsqonl_entry = 83,              /* bsqonl_entry  */
  YYSYMBOL_bsqonbracketvalue = 84,         /* bsqonbracketvalue  */
  YYSYMBOL_bsqonnamevall = 85,             /* bsqonnamevall  */
  YYSYMBOL_bsqon_braceval = 86,            /* bsqon_braceval  */
  YYSYMBOL_bsqonnameval_entry = 87,        /* bsqonnameval_entry  */
  YYSYMBOL_bsqonbracevalue = 88,           /* bsqonbracevalue  */
  YYSYMBOL_bsqonbracketbracevalue = 89,    /* bsqonbracketbracevalue  */
  YYSYMBOL_bsqontypedvalue = 90,           /* bsqontypedvalue  */
  YYSYMBOL_bsqonstructvalue = 91,          /* bsqonstructvalue  */
  YYSYMBOL_bsqonspecialcons = 92,          /* bsqonspecialcons  */
  YYSYMBOL_bsqonval = 93,                  /* bsqonval  */
  YYSYMBOL_bsqonletexp = 94,               /* bsqonletexp  */
  YYSYMBOL_bsqonref = 95,                  /* bsqonref  */
  YYSYMBOL_bsqonaccess = 96,               /* bsqonaccess  */
  YYSYMBOL_bsqonenvlist = 97,              /* bsqonenvlist  */
  YYSYMBOL_bsqoncomponent = 98,            /* bsqoncomponent  */
  YYSYMBOL_bsqonroot = 99                  /* bsqonroot  */
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

#if 1

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
#endif /* 1 */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

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
#define YYFINAL  110
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1149

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  65
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  35
/* YYNRULES -- Number of rules.  */
#define YYNRULES  139
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  240

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   313


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
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      61,    62,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      59,     2,    60,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    63,     2,    64,     2,     2,     2,     2,
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
      55,    56,    57,    58
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   147,   147,   148,   152,   153,   157,   158,   162,   163,
     167,   168,   169,   173,   174,   175,   176,   180,   181,   182,
     183,   184,   188,   189,   190,   191,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,   205,   206,   207,   208,
     209,   210,   211,   212,   213,   214,   215,   216,   217,   218,
     219,   220,   221,   222,   223,   224,   225,   229,   233,   234,
     235,   236,   237,   241,   245,   249,   250,   254,   254,   254,
     254,   254,   254,   258,   259,   260,   261,   265,   266,   270,
     271,   275,   276,   277,   278,   279,   283,   284,   288,   288,
     292,   293,   294,   295,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   311,   311,   315,   316,   317,   318,   322,
     322,   326,   327,   328,   329,   330,   331,   335,   335,   335,
     335,   335,   339,   343,   343,   343,   347,   348,   349,   353,
     354,   355,   356,   357,   361,   362,   363,   364,   368,   369
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
  "\"end of file\"", "error", "\"invalid token\"", "\"true\"",
  "\"false\"", "\"none\"", "\"some\"", "\"ok\"", "\"err\"", "\"$npos\"",
  "\"$i\"", "\"$key\"", "\"$src\"", "\"let\"", "\"in\"", "\"env\"",
  "\"::\"", "\"=>\"", "\":\"", "\",\"", "\"=\"", "\".\"", "\"(|\"",
  "\"|)\"", "\"[|\"", "\"|]\"", "\"nat literal\"", "\"int literal\"",
  "\"big nat literal\"", "\"big int literal\"", "\"rational literal\"",
  "\"float literal\"", "\"decimal literal\"", "\"decimal degree literal\"",
  "\"complex literal\"", "\"geo coordinate literal\"", "\"numberino\"",
  "\"byte buffer\"", "\"uuid (v4)\"", "\"uuid (v7)\"",
  "\"sha3 hashcode (512 bits)\"", "\"string\"", "\"cstring\"",
  "\"regular expression\"", "\"path item\"",
  "\"date & time with timezone\"", "\"date & time in TIA\"",
  "\"plain date\"", "\"plain time\"", "\"logical time\"",
  "\"ISO timestamp\"", "\"date & time delta\"", "\"delta in seconds\"",
  "\"logical time delta\"", "\"timestamp delta\"", "\"identifier\"",
  "\"type name\"", "\"unspec identifier\"", "\"shebang line\"", "'<'",
  "'>'", "'('", "')'", "'{'", "'}'", "$accept", "bsqontypel",
  "bsqontypel_entry", "bsqonnametypel", "bsqonnametypel_entry",
  "bsqonnominaltype", "bsqontermslist", "bsqonelisttype", "bsqontype",
  "bsqonliteral", "bsqonunspecvar", "bsqonidentifier",
  "bsqonscopedidentifier", "bsqontypeliteral", "bsqonenvaccess",
  "bsqonterminal", "bsqon_mapentry", "bsqonvall", "bsqonl_entry",
  "bsqonbracketvalue", "bsqonnamevall", "bsqon_braceval",
  "bsqonnameval_entry", "bsqonbracevalue", "bsqonbracketbracevalue",
  "bsqontypedvalue", "bsqonstructvalue", "bsqonspecialcons", "bsqonval",
  "bsqonletexp", "bsqonref", "bsqonaccess", "bsqonenvlist",
  "bsqoncomponent", "bsqonroot", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-98)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-126)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     299,   -14,   -98,   -98,   -98,   -37,   -34,   -32,   -98,   -98,
     -98,   -98,   118,   360,   -98,   -98,   -98,   -98,   -98,   -98,
     -98,   -98,   -98,   -98,   -98,   -98,   -98,   -98,   -98,   -98,
     -98,   -98,   -98,   -98,   -98,   -98,   -98,   -98,   -98,   -98,
     -98,   -98,   -98,    17,   -98,   421,   134,    68,   235,   -15,
      78,   -98,   168,   170,   -98,   -98,   -98,   -98,   -98,   -98,
     -98,   -98,   -98,   -98,   -98,   172,   181,   482,   -98,   158,
      83,   -98,   543,   604,   665,   137,   134,   -46,    87,   106,
     -98,   -98,   726,   -98,   151,    -6,     9,   -98,   -14,   -98,
     482,     3,    22,   167,   -98,   127,   143,    -3,   186,   -98,
     787,    -7,   -98,   190,   148,   -98,   152,   154,  1092,   -98,
     -98,   150,    92,   153,   113,   155,   123,   156,   185,   159,
     193,   -98,   161,   -98,   848,   -98,   -98,   110,   -98,     5,
     -98,   909,   -98,   101,    31,   -98,   120,   -98,    15,   -98,
      32,    89,   160,   162,   157,   164,   196,   -98,   -98,   970,
      80,   200,    11,   -98,   -98,   -98,   -98,   -98,    26,   -98,
     198,   210,   -98,   -98,   202,   -98,   -98,   -98,   -98,   -98,
     -98,   -98,   -98,   204,    40,   211,   -98,   -14,   -98,   -98,
     -98,   -14,   -98,   -98,   -98,   128,   -98,   140,   -98,   -98,
     -98,   100,   149,   -98,   -98,   -98,    83,   134,    99,    16,
     -98,  1031,   -98,   -98,   -98,   -98,   189,    20,    21,    55,
     -98,   -98,   -98,   -98,   171,   212,   -98,   -98,   -98,   -98,
     103,    72,   224,   -98,   -98,   -98,   -98,    76,    88,   -98,
     482,   -98,   -98,   -98,   -98,   -98,   220,   482,   173,   -98
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,   139,    27,    28,    26,     0,     0,     0,    59,    60,
      61,    58,     0,     0,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      56,    55,    62,    10,    57,     0,     0,     0,     0,     0,
      67,    68,    69,    70,    71,    72,   117,   103,   104,   109,
     110,   119,   118,   134,   120,     0,   121,     0,   138,     0,
       0,   108,     0,     0,     0,     0,     0,     0,     0,     0,
      81,    89,     0,    78,     0,    88,     0,    11,     0,   136,
       0,     0,     0,    22,    23,     0,     0,     0,    62,    94,
       0,     0,    87,    88,     0,   106,     0,     0,     0,   135,
       1,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   129,     0,     7,     0,    80,    84,     0,    77,    88,
      79,     0,    82,     0,     0,     3,     0,   137,     0,    17,
       0,     0,     0,     0,     0,     0,     0,    93,   101,     0,
       0,    62,     0,    86,    92,    99,    63,    12,     0,   126,
       0,     0,    69,    70,     0,   107,   112,   111,   114,   113,
     116,   115,    65,     0,     0,     0,     6,    76,    74,    85,
      83,    75,    73,     5,    15,     0,     2,     0,     4,    13,
      20,     0,     0,    18,    25,    24,     0,     0,     0,     0,
     102,     0,   100,    64,   127,   128,     0,     0,     0,     0,
      16,    14,    21,    19,     0,     0,    91,    97,    90,    95,
       0,     0,     0,     9,   132,     8,   130,     0,     0,   105,
       0,    98,    96,    66,   133,   131,     0,     0,     0,   122
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -98,   163,   -97,   -98,   129,   -40,   -98,   -98,   -71,   -98,
     -98,   141,   144,   -98,   -98,   145,   -98,   -98,   174,   -98,
     -98,   -45,   195,   -98,     1,   -98,   -98,   -98,     0,   -98,
     -98,   -98,   213,   -98,   -98
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,   134,   135,   122,   123,    49,    87,    94,    95,    50,
      51,    52,    53,    54,    55,    56,    81,    82,    83,    57,
     100,    84,   102,    58,    59,    60,    61,    62,   103,    64,
      65,    66,    67,    68,    69
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      63,   104,    71,   101,   138,   119,    93,    13,    13,   120,
     133,   131,   154,    85,   124,   136,   147,   132,   121,    13,
     141,   143,   131,   142,    72,    91,   139,    73,   180,    74,
     154,    91,   185,   191,   183,   218,    93,   186,   190,   223,
     225,   207,   144,   186,    91,    89,    93,    70,    48,    48,
     105,    93,    93,    91,    91,   152,   227,   155,    70,    43,
      48,   148,    91,   187,    92,    43,   158,   109,   161,   192,
      92,   111,   113,   115,   117,   202,    86,    91,    43,    71,
     219,    96,   129,    92,   224,   226,   203,    43,    43,    71,
     137,   218,    92,    92,    93,   223,    43,   124,    71,   147,
      93,    92,    13,   208,   124,    13,   125,   225,   188,    13,
     126,    43,   193,    71,    13,    71,    92,    71,   216,   183,
     183,    13,   216,   212,   178,    13,   215,   124,    71,   125,
      75,   182,    13,   179,    93,    13,   232,   106,   228,   188,
     234,    70,    75,    48,   200,    13,    48,   183,    70,   199,
      48,    71,   235,    70,   166,    48,    91,    93,   110,   188,
      70,   184,    48,   217,    70,    76,    48,   231,   188,    93,
     130,    70,   213,    48,    70,   168,    48,    76,    71,   118,
     189,    77,    71,   144,    70,   170,    48,   145,   210,  -123,
      43,  -124,  -123,   107,  -124,    92,   108,   214,   146,    71,
     211,   221,  -125,   156,   157,  -125,   149,   131,    43,   159,
     172,   174,   165,   157,   197,   167,   175,   169,   171,   173,
     201,    71,   194,   204,   195,   196,   104,   205,   206,   209,
     236,   222,   230,   229,   237,   239,    97,   238,     2,     3,
       4,     5,     6,     7,     8,     9,    10,    11,   233,   162,
      79,   176,   163,   164,   140,     0,   128,    13,    90,     0,
       0,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,     0,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      98,    43,    44,     0,    46,   153,    47,     0,    48,    99,
       1,     0,     2,     3,     4,     5,     6,     7,     8,     9,
      10,    11,     0,     0,    12,     0,     0,     0,     0,     0,
       0,    13,     0,     0,     0,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,     0,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,     0,
      47,    78,    48,     2,     3,     4,     5,     6,     7,     8,
       9,    10,    11,     0,     0,    79,     0,     0,     0,     0,
       0,     0,    13,    80,     0,     0,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,     0,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,     0,    46,
       0,    47,    88,    48,     2,     3,     4,     5,     6,     7,
       8,     9,    10,    11,     0,     0,    12,     0,     0,     0,
       0,     0,     0,    13,     0,     0,     0,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,     0,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,     0,
      46,     0,    47,    88,    48,     2,     3,     4,     5,     6,
       7,     8,     9,    10,    11,     0,     0,    79,     0,     0,
       0,     0,     0,     0,    13,     0,     0,     0,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,     0,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
       0,    46,     0,    47,   112,    48,     2,     3,     4,     5,
       6,     7,     8,     9,    10,    11,     0,     0,    79,     0,
       0,     0,     0,     0,     0,    13,     0,     0,     0,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,     0,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,     0,    46,     0,    47,   114,    48,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,     0,     0,    79,
       0,     0,     0,     0,     0,     0,    13,     0,     0,     0,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
       0,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,     0,    46,     0,    47,   116,    48,     2,     3,
       4,     5,     6,     7,     8,     9,    10,    11,     0,     0,
      79,     0,     0,     0,     0,     0,     0,    13,     0,     0,
       0,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,     0,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,     0,    46,     0,    47,   127,    48,     2,
       3,     4,     5,     6,     7,     8,     9,    10,    11,     0,
       0,    79,     0,     0,     0,     0,     0,     0,    13,     0,
       0,     0,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,     0,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,     0,    46,     0,    47,   150,    48,
       2,     3,     4,     5,     6,     7,     8,     9,    10,    11,
       0,     0,    79,     0,     0,     0,     0,     0,     0,    13,
       0,     0,     0,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,     0,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,   151,    43,    44,     0,    46,     0,    47,   177,
      48,     2,     3,     4,     5,     6,     7,     8,     9,    10,
      11,     0,     0,    79,     0,     0,     0,     0,     0,     0,
      13,     0,     0,     0,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,     0,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,     0,    46,     0,    47,
     181,    48,     2,     3,     4,     5,     6,     7,     8,     9,
      10,    11,     0,     0,    79,     0,     0,     0,     0,     0,
       0,    13,     0,     0,     0,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,     0,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,     0,    46,     0,
      47,   198,    48,     2,     3,     4,     5,     6,     7,     8,
       9,    10,    11,     0,     0,    79,     0,     0,     0,     0,
       0,     0,    13,     0,     0,     0,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,     0,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,     0,    46,
       0,    47,   220,    48,     2,     3,     4,     5,     6,     7,
       8,     9,    10,    11,     0,     0,    79,     0,     0,     0,
       0,     0,     0,    13,     0,     0,     0,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,     0,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,     0,
      46,     0,    47,     0,    48,     2,     3,     4,     0,     0,
       0,     8,     9,    10,    11,     0,     0,    79,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,   160,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44
};

static const yytype_int16 yycheck[] =
{
       0,    16,     1,    48,     1,    76,    46,    22,    22,    55,
       1,    17,    19,    13,    17,    86,    19,    23,    64,    22,
      91,    92,    17,     1,    61,    22,    23,    61,    23,    61,
      19,    22,     1,     1,    19,    19,    76,   134,    23,    19,
      19,     1,    16,   140,    22,    45,    86,    61,    63,    63,
      49,    91,    92,    22,    22,   100,     1,    64,    61,    56,
      63,    64,    22,   134,    61,    56,   106,    67,   108,   140,
      61,    70,    72,    73,    74,    64,    59,    22,    56,    78,
      64,    13,    82,    61,    64,    64,    60,    56,    56,    88,
      90,    19,    61,    61,   134,    19,    56,    17,    97,    19,
     140,    61,    22,   174,    17,    22,    19,    19,    19,    22,
      23,    56,    23,   112,    22,   114,    61,   116,    19,    19,
      19,    22,    19,    23,   124,    22,   197,    17,   127,    19,
      24,   131,    22,    23,   174,    22,    64,    59,   209,    19,
      64,    61,    24,    63,    64,    22,    63,    19,    61,   149,
      63,   150,    64,    61,    62,    63,    22,   197,     0,    19,
      61,    60,    63,    64,    61,    59,    63,    64,    19,   209,
      19,    61,    23,    63,    61,    62,    63,    59,   177,    42,
      60,    63,   181,    16,    61,    62,    63,    60,    60,    21,
      56,    21,    24,    21,    24,    61,    24,   196,    55,   198,
      60,   201,    21,    55,    56,    24,    20,    17,    56,    55,
      25,    18,    62,    56,    18,    62,    55,    62,    62,    60,
      20,   220,    62,    25,    62,    61,    16,    25,    24,    18,
     230,    42,    20,    62,    14,    62,     1,   237,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    24,   108,
      15,   122,   108,   108,    91,    -1,    82,    22,    45,    -1,
      -1,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    -1,    59,   100,    61,    -1,    63,    64,
       1,    -1,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    22,    -1,    -1,    -1,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    -1,
      61,     1,    63,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    23,    -1,    -1,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    -1,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    -1,    59,
      -1,    61,     1,    63,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    22,    -1,    -1,    -1,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    -1,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    -1,
      59,    -1,    61,     1,    63,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    -1,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      -1,    59,    -1,    61,     1,    63,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    -1,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    -1,    59,    -1,    61,     1,    63,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    -1,    59,    -1,    61,     1,    63,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,
      -1,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    -1,    59,    -1,    61,     1,    63,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    -1,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    -1,    59,    -1,    61,     1,    63,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    22,
      -1,    -1,    -1,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    -1,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    -1,    59,    -1,    61,     1,
      63,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      22,    -1,    -1,    -1,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    -1,    59,    -1,    61,
       1,    63,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    22,    -1,    -1,    -1,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    -1,    59,    -1,
      61,     1,    63,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    -1,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    -1,    59,
      -1,    61,     1,    63,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    22,    -1,    -1,    -1,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    -1,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    -1,
      59,    -1,    61,    -1,    63,     3,     4,     5,    -1,    -1,
      -1,     9,    10,    11,    12,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     1,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    15,    22,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    61,    63,    70,
      74,    75,    76,    77,    78,    79,    80,    84,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
      61,    89,    61,    61,    61,    24,    59,    63,     1,    15,
      23,    81,    82,    83,    86,    93,    59,    71,     1,    93,
      97,    22,    61,    70,    72,    73,    13,     1,    55,    64,
      85,    86,    87,    93,    16,    89,    59,    21,    24,    93,
       0,    89,     1,    93,     1,    93,     1,    93,    42,    73,
      55,    64,    68,    69,    17,    19,    23,     1,    83,    93,
      19,    17,    23,     1,    66,    67,    73,    93,     1,    23,
      66,    73,     1,    73,    16,    60,    55,    19,    64,    20,
       1,    55,    86,    87,    19,    64,    55,    56,    70,    55,
      36,    70,    76,    77,    80,    62,    62,    62,    62,    62,
      62,    62,    25,    60,    18,    55,    69,     1,    93,    23,
      23,     1,    93,    19,    60,     1,    67,    73,    19,    60,
      23,     1,    73,    23,    62,    62,    61,    18,     1,    93,
      64,    20,    64,    60,    25,    25,    24,     1,    73,    18,
      60,    60,    23,    23,    89,    73,    19,    64,    19,    64,
       1,    93,    42,    19,    64,    19,    64,     1,    73,    62,
      20,    64,    64,    24,    64,    64,    93,    14,    93,    62
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    65,    66,    66,    67,    67,    68,    68,    69,    69,
      70,    70,    70,    71,    71,    71,    71,    72,    72,    72,
      72,    72,    73,    73,    73,    73,    74,    74,    74,    74,
      74,    74,    74,    74,    74,    74,    74,    74,    74,    74,
      74,    74,    74,    74,    74,    74,    74,    74,    74,    74,
      74,    74,    74,    74,    74,    74,    74,    75,    76,    76,
      76,    76,    76,    77,    78,    79,    79,    80,    80,    80,
      80,    80,    80,    81,    81,    81,    81,    82,    82,    83,
      83,    84,    84,    84,    84,    84,    85,    85,    86,    86,
      87,    87,    87,    87,    88,    88,    88,    88,    88,    88,
      88,    88,    88,    89,    89,    90,    90,    90,    90,    91,
      91,    92,    92,    92,    92,    92,    92,    93,    93,    93,
      93,    93,    94,    95,    95,    95,    96,    96,    96,    97,
      97,    97,    97,    97,    98,    98,    98,    98,    99,    99
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     1,     2,     2,     2,     1,     4,     4,
       1,     2,     3,     3,     4,     3,     4,     2,     3,     4,
       3,     4,     1,     1,     3,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     4,     4,     7,     1,     1,     1,
       1,     1,     1,     3,     3,     3,     3,     2,     1,     2,
       2,     2,     3,     4,     3,     4,     2,     1,     1,     1,
       4,     4,     2,     2,     2,     5,     6,     5,     6,     3,
       4,     3,     4,     1,     1,     6,     2,     4,     2,     1,
       1,     4,     4,     4,     4,     4,     4,     1,     1,     1,
       1,     1,    10,     1,     1,     1,     3,     4,     4,     3,
       6,     7,     6,     7,     1,     2,     2,     3,     1,     1
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

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


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


/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YYLOCATION_PRINT

#  if defined YY_LOCATION_PRINT

   /* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#   define YYLOCATION_PRINT(File, Loc)  YY_LOCATION_PRINT(File, *(Loc))

#  elif defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
}

#   define YYLOCATION_PRINT  yy_location_print_

    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT(File, Loc)  YYLOCATION_PRINT(File, &(Loc))

#  else

#   define YYLOCATION_PRINT(File, Loc) ((void) 0)
    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT  YYLOCATION_PRINT

#  endif
# endif /* !defined YYLOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
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
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YYLOCATION_PRINT (yyo, yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
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
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]));
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
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


/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yysymbol_kind_t yytoken;
  YYLTYPE *yylloc;
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
  int yyn = yypact[+*yyctx->yyssp];
  if (!yypact_value_is_default (yyn))
    {
      /* Start YYX at -YYN if negative to avoid negative indexes in
         YYCHECK.  In other words, skip the first -YYN actions for
         this state because they are default actions.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;
      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yyx;
      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
        if (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_value_is_error (yytable[yyx + yyn]))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = YY_CAST (yysymbol_kind_t, yyx);
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
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
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
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
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
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
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

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  yylsp[0] = yylloc;
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
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
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
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

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
      yyerror_range[1] = yylloc;
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
  *++yylsp = yylloc;

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

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* bsqontypel: bsqontypel bsqontypel_entry  */
#line 147 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                               { (yyval.bsqon_type_list) = BSQON_AST_LIST_OF_TYPES_Push((yyvsp[0].bsqon_type_node), (yyvsp[-1].bsqon_type_list)); }
#line 1929 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 3: /* bsqontypel: bsqontypel_entry  */
#line 148 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                      { (yyval.bsqon_type_list) = BSQON_AST_LIST_OF_TYPES_Singleton((yyvsp[0].bsqon_type_node)); }
#line 1935 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 4: /* bsqontypel_entry: bsqontype ","  */
#line 152 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                       { (yyval.bsqon_type_node) = (yyvsp[-1].bsqon_type_node); }
#line 1941 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 5: /* bsqontypel_entry: error ","  */
#line 153 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                     { (yyval.bsqon_type_node) = BSQON_AST_ERROR(MK_SPOS_S((yylsp[-1]))); yyerrok; }
#line 1947 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 6: /* bsqonnametypel: bsqonnametypel bsqonnametypel_entry  */
#line 157 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                       { (yyval.bsqon_named_type_list) = BSQON_AST_NLIST_OF_TYPES_Push((yyvsp[0].bsqon_named_type_list_entry), (yyvsp[-1].bsqon_named_type_list)); }
#line 1953 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 7: /* bsqonnametypel: bsqonnametypel_entry  */
#line 158 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                          { (yyval.bsqon_named_type_list) = BSQON_AST_NLIST_OF_TYPES_Singleton((yyvsp[0].bsqon_named_type_list_entry)); }
#line 1959 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 8: /* bsqonnametypel_entry: "identifier" ":" bsqontype ","  */
#line 162 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                                  { (yyval.bsqon_named_type_list_entry) = BSQON_AST_NLIST_OF_TYPES_ENTRY_Create((yyvsp[-3].str), (yyvsp[-1].bsqon_type_node)); }
#line 1965 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 9: /* bsqonnametypel_entry: "identifier" ":" error ","  */
#line 163 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                                { (yyval.bsqon_named_type_list_entry) = BSQON_AST_NLIST_OF_TYPES_ENTRY_Create((yyvsp[-3].str), BSQON_AST_ERROR(MK_SPOS_S((yylsp[-1])))); yyerrok; }
#line 1971 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 10: /* bsqonnominaltype: "type name"  */
#line 167 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                        { (yyval.bsqon_type_node) = BSQON_AST_NODE_CONS(NominalType, BSQON_AST_TAG_NominalType, MK_SPOS_S((yylsp[0])), (yyvsp[0].str), NULL); }
#line 1977 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 11: /* bsqonnominaltype: "type name" bsqontermslist  */
#line 168 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                         { (yyval.bsqon_type_node) = BSQON_AST_NODE_CONS(NominalType, BSQON_AST_TAG_NominalType, MK_SPOS_S((yylsp[-1])), (yyvsp[-1].str), (yyvsp[0].bsqon_type_list)); }
#line 1983 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 12: /* bsqonnominaltype: bsqonnominaltype "::" "type name"  */
#line 169 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                                            { (yyval.bsqon_type_node) = BSQON_AST_NODE_CONS(NominalScopedType, BSQON_AST_TAG_NominalScopedType, MK_SPOS_R((yylsp[-2]), (yylsp[0])), (yyvsp[-2].bsqon_type_node), (yyvsp[0].str)); }
#line 1989 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 13: /* bsqontermslist: '<' bsqontype '>'  */
#line 173 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                     { (yyval.bsqon_type_list) = BSQON_AST_LIST_OF_TYPES_Singleton((yyvsp[-1].bsqon_type_node)); }
#line 1995 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 14: /* bsqontermslist: '<' bsqontypel bsqontype '>'  */
#line 174 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                  { (yyval.bsqon_type_list) = BSQON_AST_LIST_OF_TYPES_Reverse(BSQON_AST_LIST_OF_TYPES_Push((yyvsp[-1].bsqon_type_node), (yyvsp[-2].bsqon_type_list))); }
#line 2001 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 15: /* bsqontermslist: '<' error '>'  */
#line 175 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                   { (yyval.bsqon_type_list) = BSQON_AST_LIST_OF_TYPES_Singleton(BSQON_AST_ERROR(MK_SPOS_S((yylsp[-1])))); yyerrok; }
#line 2007 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 16: /* bsqontermslist: '<' bsqontypel error '>'  */
#line 176 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_type_list) = BSQON_AST_LIST_OF_TYPES_Reverse(BSQON_AST_LIST_OF_TYPES_Push(BSQON_AST_ERROR(MK_SPOS_S((yylsp[-1]))), (yyvsp[-2].bsqon_type_list))); yyerrok; }
#line 2013 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 17: /* bsqonelisttype: "(|" "|)"  */
#line 180 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                  { (yyval.bsqon_type_node) = BSQON_AST_NODE_CONS(EListType, BSQON_AST_TAG_EListType, MK_SPOS_R((yylsp[-1]), (yylsp[0])), NULL); }
#line 2019 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 18: /* bsqonelisttype: "(|" bsqontype "|)"  */
#line 181 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                              { (yyval.bsqon_type_node) = BSQON_AST_NODE_CONS(EListType, BSQON_AST_TAG_EListType, MK_SPOS_R((yylsp[-2]), (yylsp[0])), BSQON_AST_LIST_OF_TYPES_Singleton((yyvsp[-1].bsqon_type_node))); }
#line 2025 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 19: /* bsqonelisttype: "(|" bsqontypel bsqontype "|)"  */
#line 182 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                                         { (yyval.bsqon_type_node) = BSQON_AST_NODE_CONS(EListType, BSQON_AST_TAG_EListType, MK_SPOS_R((yylsp[-3]), (yylsp[0])), BSQON_AST_LIST_OF_TYPES_Reverse(BSQON_AST_LIST_OF_TYPES_Push((yyvsp[-1].bsqon_type_node), (yyvsp[-2].bsqon_type_list)))); }
#line 2031 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 20: /* bsqonelisttype: "(|" error "|)"  */
#line 183 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                          { (yyval.bsqon_type_node) = BSQON_AST_NODE_CONS(EListType, BSQON_AST_TAG_EListType, MK_SPOS_R((yylsp[-2]), (yylsp[0])), BSQON_AST_LIST_OF_TYPES_Singleton(BSQON_AST_ERROR(MK_SPOS_S((yylsp[-2]))))); yyerrok; }
#line 2037 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 21: /* bsqonelisttype: "(|" bsqontypel error "|)"  */
#line 184 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                                     { (yyval.bsqon_type_node) = BSQON_AST_NODE_CONS(EListType, BSQON_AST_TAG_EListType, MK_SPOS_R((yylsp[-3]), (yylsp[0])), BSQON_AST_LIST_OF_TYPES_Reverse(BSQON_AST_LIST_OF_TYPES_Push(BSQON_AST_ERROR(MK_SPOS_S((yylsp[-1]))), (yyvsp[-2].bsqon_type_list)))); yyerrok; }
#line 2043 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 22: /* bsqontype: bsqonnominaltype  */
#line 188 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                    { (yyval.bsqon_type_node) = (yyvsp[0].bsqon_type_node); }
#line 2049 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 23: /* bsqontype: bsqonelisttype  */
#line 189 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                    { (yyval.bsqon_type_node) = (yyvsp[0].bsqon_type_node); }
#line 2055 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 24: /* bsqontype: '(' bsqontype ')'  */
#line 190 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                       { (yyval.bsqon_type_node) = (yyvsp[-1].bsqon_type_node); }
#line 2061 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 25: /* bsqontype: '(' error ')'  */
#line 191 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                   { (yyval.bsqon_type_node) = BSQON_AST_ERROR(MK_SPOS_S((yylsp[-1]))); yyerrok; }
#line 2067 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 26: /* bsqonliteral: "none"  */
#line 195 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(SingletonValue, BSQON_AST_TAG_NoneValue, MK_SPOS_S((yylsp[0]))); }
#line 2073 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 27: /* bsqonliteral: "true"  */
#line 196 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(SingletonValue, BSQON_AST_TAG_TrueValue, MK_SPOS_S((yylsp[0]))); }
#line 2079 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 28: /* bsqonliteral: "false"  */
#line 197 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(SingletonValue, BSQON_AST_TAG_FalseValue, MK_SPOS_S((yylsp[0]))); }
#line 2085 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 29: /* bsqonliteral: "nat literal"  */
#line 198 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(LiteralStandardValue, BSQON_AST_TAG_NatValue, MK_SPOS_S((yylsp[0])), (yyvsp[0].str)); }
#line 2091 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 30: /* bsqonliteral: "int literal"  */
#line 199 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(LiteralStandardValue, BSQON_AST_TAG_IntValue, MK_SPOS_S((yylsp[0])), (yyvsp[0].str)); }
#line 2097 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 31: /* bsqonliteral: "big nat literal"  */
#line 200 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(LiteralStandardValue, BSQON_AST_TAG_BigNatValue, MK_SPOS_S((yylsp[0])), (yyvsp[0].str)); }
#line 2103 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 32: /* bsqonliteral: "big int literal"  */
#line 201 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(LiteralStandardValue, BSQON_AST_TAG_BigIntValue, MK_SPOS_S((yylsp[0])), (yyvsp[0].str)); }
#line 2109 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 33: /* bsqonliteral: "rational literal"  */
#line 202 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(LiteralStandardValue, BSQON_AST_TAG_RationalValue, MK_SPOS_S((yylsp[0])), (yyvsp[0].str)); }
#line 2115 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 34: /* bsqonliteral: "float literal"  */
#line 203 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(LiteralStandardValue, BSQON_AST_TAG_FloatValue, MK_SPOS_S((yylsp[0])), (yyvsp[0].str)); }
#line 2121 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 35: /* bsqonliteral: "decimal literal"  */
#line 204 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(LiteralStandardValue, BSQON_AST_TAG_DecimalValue, MK_SPOS_S((yylsp[0])), (yyvsp[0].str)); }
#line 2127 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 36: /* bsqonliteral: "decimal degree literal"  */
#line 205 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(LiteralStandardValue, BSQON_AST_TAG_DecimalDegreeValue, MK_SPOS_S((yylsp[0])), (yyvsp[0].str)); }
#line 2133 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 37: /* bsqonliteral: "complex literal"  */
#line 206 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(LiteralStandardValue, BSQON_AST_TAG_ComplexValue, MK_SPOS_S((yylsp[0])), (yyvsp[0].str)); }
#line 2139 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 38: /* bsqonliteral: "geo coordinate literal"  */
#line 207 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(LiteralStandardValue, BSQON_AST_TAG_LatLongValue, MK_SPOS_S((yylsp[0])), (yyvsp[0].str)); }
#line 2145 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 39: /* bsqonliteral: "byte buffer"  */
#line 208 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(LiteralStandardValue, BSQON_AST_TAG_ByteBufferValue, MK_SPOS_S((yylsp[0])), (yyvsp[0].str)); }
#line 2151 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 40: /* bsqonliteral: "uuid (v4)"  */
#line 209 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(LiteralStandardValue, BSQON_AST_TAG_UUIDv4Value, MK_SPOS_S((yylsp[0])), (yyvsp[0].str)); }
#line 2157 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 41: /* bsqonliteral: "uuid (v7)"  */
#line 210 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(LiteralStandardValue, BSQON_AST_TAG_UUIDv7Value, MK_SPOS_S((yylsp[0])), (yyvsp[0].str)); }
#line 2163 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 42: /* bsqonliteral: "sha3 hashcode (512 bits)"  */
#line 211 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(LiteralStandardValue, BSQON_AST_TAG_SHAHashcodeValue, MK_SPOS_S((yylsp[0])), (yyvsp[0].str)); }
#line 2169 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 43: /* bsqonliteral: "string"  */
#line 212 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(LiteralStringValue, BSQON_AST_TAG_StringValue, MK_SPOS_S((yylsp[0])), (yyvsp[0].bstr)); }
#line 2175 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 44: /* bsqonliteral: "cstring"  */
#line 213 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(LiteralStringValue, BSQON_AST_TAG_CStringValue, MK_SPOS_S((yylsp[0])), (yyvsp[0].bstr)); }
#line 2181 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 45: /* bsqonliteral: "regular expression"  */
#line 214 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(LiteralStringValue, BSQON_AST_TAG_RegexValue, MK_SPOS_S((yylsp[0])), (yyvsp[0].bstr)); }
#line 2187 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 46: /* bsqonliteral: "path item"  */
#line 215 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(LiteralPathValue, BSQON_AST_TAG_PathValue, MK_SPOS_S((yylsp[0])), (yyvsp[0].bstr)); }
#line 2193 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 47: /* bsqonliteral: "date & time with timezone"  */
#line 216 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(LiteralStandardValue, BSQON_AST_TAG_TZDateTimeValue, MK_SPOS_S((yylsp[0])), (yyvsp[0].str)); }
#line 2199 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 48: /* bsqonliteral: "date & time in TIA"  */
#line 217 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(LiteralStandardValue, BSQON_AST_TAG_TIATimeValue, MK_SPOS_S((yylsp[0])), (yyvsp[0].str)); }
#line 2205 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 49: /* bsqonliteral: "plain date"  */
#line 218 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(LiteralStandardValue, BSQON_AST_TAG_PlainDateValue, MK_SPOS_S((yylsp[0])), (yyvsp[0].str)); }
#line 2211 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 50: /* bsqonliteral: "plain time"  */
#line 219 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(LiteralStandardValue, BSQON_AST_TAG_PlainTimeValue, MK_SPOS_S((yylsp[0])), (yyvsp[0].str)); }
#line 2217 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 51: /* bsqonliteral: "logical time"  */
#line 220 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(LiteralStandardValue, BSQON_AST_TAG_LogicalTimeValue, MK_SPOS_S((yylsp[0])), (yyvsp[0].str)); }
#line 2223 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 52: /* bsqonliteral: "ISO timestamp"  */
#line 221 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(LiteralStandardValue, BSQON_AST_TAG_TimestampValue, MK_SPOS_S((yylsp[0])), (yyvsp[0].str)); }
#line 2229 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 53: /* bsqonliteral: "date & time delta"  */
#line 222 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(LiteralStandardValue, BSQON_AST_TAG_DeltaDateTimeValue, MK_SPOS_S((yylsp[0])), (yyvsp[0].str)); }
#line 2235 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 54: /* bsqonliteral: "delta in seconds"  */
#line 223 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(LiteralStandardValue, BSQON_AST_TAG_DeltaSecondsValue, MK_SPOS_S((yylsp[0])), (yyvsp[0].str)); }
#line 2241 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 55: /* bsqonliteral: "timestamp delta"  */
#line 224 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(LiteralStandardValue, BSQON_AST_TAG_DeltaISOTimeStampValue, MK_SPOS_S((yylsp[0])), (yyvsp[0].str)); }
#line 2247 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 56: /* bsqonliteral: "logical time delta"  */
#line 225 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(LiteralStandardValue, BSQON_AST_TAG_DeltaLogicalValue, MK_SPOS_S((yylsp[0])), (yyvsp[0].str)); }
#line 2253 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 57: /* bsqonunspecvar: "unspec identifier"  */
#line 229 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                           { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(NameValue, BSQON_AST_TAG_UnspecIdentifierValue, MK_SPOS_S((yylsp[0])), (yyvsp[0].str)); }
#line 2259 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 58: /* bsqonidentifier: "$src"  */
#line 233 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                      { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(NameValue, BSQON_AST_TAG_IdentifierValue, MK_SPOS_S((yylsp[0])), "$src"); }
#line 2265 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 59: /* bsqonidentifier: "$npos"  */
#line 234 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                      { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(NameValue, BSQON_AST_TAG_IdentifierValue, MK_SPOS_S((yylsp[0])), "$npos"); }
#line 2271 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 60: /* bsqonidentifier: "$i"  */
#line 235 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                      { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(NameValue, BSQON_AST_TAG_IdentifierValue, MK_SPOS_S((yylsp[0])), "$i"); }
#line 2277 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 61: /* bsqonidentifier: "$key"  */
#line 236 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                      { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(NameValue, BSQON_AST_TAG_IdentifierValue, MK_SPOS_S((yylsp[0])), "$key"); }
#line 2283 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 62: /* bsqonidentifier: "identifier"  */
#line 237 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                      { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(NameValue, BSQON_AST_TAG_IdentifierValue, MK_SPOS_S((yylsp[0])), (yyvsp[0].str)); }
#line 2289 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 63: /* bsqonscopedidentifier: bsqonnominaltype "::" "identifier"  */
#line 241 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                                      { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(ScopedNameValue, BSQON_AST_TAG_ScopedNameValue, MK_SPOS_R((yylsp[-2]), (yylsp[0])), (yyvsp[-2].bsqon_type_node), (yyvsp[0].str)); }
#line 2295 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 64: /* bsqontypeliteral: bsqonliteral '<' bsqonnominaltype '>'  */
#line 245 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                         { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(TypedLiteralValue, BSQON_AST_TAG_TypedLiteralValue, MK_SPOS_R((yylsp[-3]), (yylsp[-1])), (yyvsp[-3].bsqon_value_node), (yyvsp[-1].bsqon_type_node)); }
#line 2301 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 65: /* bsqonenvaccess: "env" "[|" "cstring" "|]"  */
#line 249 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                                      { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(EnvAccessValue, BSQON_AST_TAG_EnvAccessValue, MK_SPOS_R((yylsp[-3]), (yylsp[0])), (yyvsp[-1].bstr), NULL); }
#line 2307 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 66: /* bsqonenvaccess: "env" '<' bsqontype '>' "[|" "cstring" "[|"  */
#line 250 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                                                          { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(EnvAccessValue, BSQON_AST_TAG_EnvAccessValue, MK_SPOS_R((yylsp[-6]), (yylsp[0])), (yyvsp[-1].bstr), (yyvsp[-4].bsqon_type_node)); }
#line 2313 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 72: /* bsqonterminal: bsqonenvaccess  */
#line 254 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                                                                                               { (yyval.bsqon_value_node) = (yyvsp[0].bsqon_value_node); }
#line 2319 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 73: /* bsqon_mapentry: bsqonval "=>" bsqonval  */
#line 258 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                               { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(MapEntryValue, BSQON_AST_TAG_MapEntryValue, MK_SPOS_R((yylsp[-2]), (yylsp[0])), (yyvsp[-2].bsqon_value_node), (yyvsp[0].bsqon_value_node)); }
#line 2325 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 74: /* bsqon_mapentry: error "=>" bsqonval  */
#line 259 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(MapEntryValue, BSQON_AST_TAG_MapEntryValue, MK_SPOS_R((yylsp[-2]), (yylsp[0])), BSQON_AST_ERROR(MK_SPOS_S((yylsp[-2]))), (yyvsp[0].bsqon_value_node)); yyerrok; }
#line 2331 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 75: /* bsqon_mapentry: bsqonval "=>" error  */
#line 260 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(MapEntryValue, BSQON_AST_TAG_MapEntryValue, MK_SPOS_R((yylsp[-2]), (yylsp[0])), (yyvsp[-2].bsqon_value_node), BSQON_AST_ERROR(MK_SPOS_S((yylsp[0])))); yyerrok; }
#line 2337 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 76: /* bsqon_mapentry: error "=>" error  */
#line 261 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                           { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(MapEntryValue, BSQON_AST_TAG_MapEntryValue, MK_SPOS_R((yylsp[-2]), (yylsp[0])), BSQON_AST_ERROR(MK_SPOS_S((yylsp[-2]))), BSQON_AST_ERROR(MK_SPOS_S((yylsp[0])))); yyerrok; }
#line 2343 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 77: /* bsqonvall: bsqonvall bsqonl_entry  */
#line 265 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                          { (yyval.bsqon_value_list) = BSQON_AST_LIST_OF_VALUES_Push((yyvsp[0].bsqon_value_node), (yyvsp[-1].bsqon_value_list)); }
#line 2349 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 78: /* bsqonvall: bsqonl_entry  */
#line 266 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                  { (yyval.bsqon_value_list) = BSQON_AST_LIST_OF_VALUES_Singleton((yyvsp[0].bsqon_value_node)); }
#line 2355 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 79: /* bsqonl_entry: bsqon_braceval ","  */
#line 270 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                            { (yyval.bsqon_value_node) = (yyvsp[-1].bsqon_value_node); }
#line 2361 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 80: /* bsqonl_entry: error ","  */
#line 271 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                     { (yyval.bsqon_value_node) = BSQON_AST_ERROR(MK_SPOS_S((yylsp[-1]))); yyerrok; }
#line 2367 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 81: /* bsqonbracketvalue: "(|" "|)"  */
#line 275 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                  { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(BracketValue, BSQON_AST_TAG_BracketValue, MK_SPOS_R((yylsp[-1]), (yylsp[0])), NULL); }
#line 2373 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 82: /* bsqonbracketvalue: "(|" bsqonval "|)"  */
#line 276 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                             { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(BracketValue, BSQON_AST_TAG_BracketValue, MK_SPOS_R((yylsp[-2]), (yylsp[0])), BSQON_AST_LIST_OF_VALUES_Singleton((yyvsp[-1].bsqon_value_node))); }
#line 2379 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 83: /* bsqonbracketvalue: "(|" bsqonvall bsqonval "|)"  */
#line 277 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                                       { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(BracketValue, BSQON_AST_TAG_BracketValue, MK_SPOS_R((yylsp[-3]), (yylsp[0])), BSQON_AST_LIST_OF_VALUES_Reverse(BSQON_AST_LIST_OF_VALUES_Push((yyvsp[-1].bsqon_value_node), (yyvsp[-2].bsqon_value_list)))); }
#line 2385 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 84: /* bsqonbracketvalue: "(|" error "|)"  */
#line 278 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                          { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(BracketValue, BSQON_AST_TAG_BracketValue, MK_SPOS_R((yylsp[-2]), (yylsp[0])), BSQON_AST_LIST_OF_VALUES_Singleton(BSQON_AST_ERROR(MK_SPOS_S((yylsp[-1]))))); yyerrok; }
#line 2391 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 85: /* bsqonbracketvalue: "(|" bsqonvall error "|)"  */
#line 279 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                                    { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(BracketValue, BSQON_AST_TAG_BracketValue, MK_SPOS_R((yylsp[-3]), (yylsp[0])), BSQON_AST_LIST_OF_VALUES_Reverse(BSQON_AST_LIST_OF_VALUES_Push(BSQON_AST_ERROR(MK_SPOS_S((yylsp[-1]))), (yyvsp[-2].bsqon_value_list)))); yyerrok; }
#line 2397 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 86: /* bsqonnamevall: bsqonnamevall bsqonnameval_entry  */
#line 283 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                    { (yyval.bsqon_named_value_list) = BSQON_AST_NLIST_OF_VALUES_Push((yyvsp[0].bsqon_named_value_list_entry), (yyvsp[-1].bsqon_named_value_list)); }
#line 2403 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 87: /* bsqonnamevall: bsqonnameval_entry  */
#line 284 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                        { (yyval.bsqon_named_value_list) = BSQON_AST_NLIST_OF_VALUES_Singleton((yyvsp[0].bsqon_named_value_list_entry)); }
#line 2409 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 89: /* bsqon_braceval: bsqon_mapentry  */
#line 288 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                             { (yyval.bsqon_value_node) = (yyvsp[0].bsqon_value_node); }
#line 2415 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 90: /* bsqonnameval_entry: "identifier" "=" bsqonval ","  */
#line 292 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                                  { (yyval.bsqon_named_value_list_entry) = BSQON_AST_NLIST_OF_VALUES_ENTRY_Create((yyvsp[-3].str), (yyvsp[-1].bsqon_value_node)); }
#line 2421 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 91: /* bsqonnameval_entry: "identifier" "=" error ","  */
#line 293 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                                 { (yyval.bsqon_named_value_list_entry) = BSQON_AST_NLIST_OF_VALUES_ENTRY_Create((yyvsp[-3].str), BSQON_AST_ERROR(MK_SPOS_S((yylsp[-1])))); yyerrok; }
#line 2427 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 92: /* bsqonnameval_entry: bsqon_braceval ","  */
#line 294 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_named_value_list_entry) = BSQON_AST_NLIST_OF_VALUES_ENTRY_Create(NULL, (yyvsp[-1].bsqon_value_node)); }
#line 2433 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 93: /* bsqonnameval_entry: error ","  */
#line 295 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                     { (yyval.bsqon_named_value_list_entry) = BSQON_AST_NLIST_OF_VALUES_ENTRY_Create(NULL, BSQON_AST_ERROR(MK_SPOS_S((yylsp[-1])))); yyerrok; }
#line 2439 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 94: /* bsqonbracevalue: '{' '}'  */
#line 299 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
           { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(BraceValue, BSQON_AST_TAG_BraceValue, MK_SPOS_R((yylsp[-1]), (yylsp[0])), NULL); }
#line 2445 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 95: /* bsqonbracevalue: '{' "identifier" "=" bsqonval '}'  */
#line 300 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                                  { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(BraceValue, BSQON_AST_TAG_BraceValue, MK_SPOS_R((yylsp[-4]), (yylsp[0])), BSQON_AST_NLIST_OF_VALUES_Singleton(BSQON_AST_NLIST_OF_VALUES_ENTRY_Create((yyvsp[-3].str), (yyvsp[-1].bsqon_value_node)))); }
#line 2451 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 96: /* bsqonbracevalue: '{' bsqonnamevall "identifier" "=" bsqonval '}'  */
#line 301 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                                                { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(BraceValue, BSQON_AST_TAG_BraceValue, MK_SPOS_R((yylsp[-5]), (yylsp[0])), BSQON_AST_NLIST_OF_VALUES_Reverse(BSQON_AST_NLIST_OF_VALUES_Push(BSQON_AST_NLIST_OF_VALUES_ENTRY_Create((yyvsp[-3].str), (yyvsp[-1].bsqon_value_node)), (yyvsp[-4].bsqon_named_value_list)))); }
#line 2457 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 97: /* bsqonbracevalue: '{' "identifier" "=" error '}'  */
#line 302 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                               { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(BraceValue, BSQON_AST_TAG_BraceValue, MK_SPOS_R((yylsp[-4]), (yylsp[0])), BSQON_AST_NLIST_OF_VALUES_Singleton(BSQON_AST_NLIST_OF_VALUES_ENTRY_Create((yyvsp[-3].str), BSQON_AST_ERROR(MK_SPOS_S((yylsp[-1])))))); yyerrok; }
#line 2463 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 98: /* bsqonbracevalue: '{' bsqonnamevall "identifier" "=" error '}'  */
#line 303 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                                             { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(BraceValue, BSQON_AST_TAG_BraceValue, MK_SPOS_R((yylsp[-5]), (yylsp[0])), BSQON_AST_NLIST_OF_VALUES_Reverse(BSQON_AST_NLIST_OF_VALUES_Push(BSQON_AST_NLIST_OF_VALUES_ENTRY_Create((yyvsp[-3].str), BSQON_AST_ERROR(MK_SPOS_S((yylsp[-1])))), (yyvsp[-4].bsqon_named_value_list)))); yyerrok; }
#line 2469 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 99: /* bsqonbracevalue: '{' bsqon_braceval '}'  */
#line 304 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                            { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(BraceValue, BSQON_AST_TAG_BraceValue, MK_SPOS_R((yylsp[-2]), (yylsp[0])), BSQON_AST_NLIST_OF_VALUES_Singleton(BSQON_AST_NLIST_OF_VALUES_ENTRY_Create(NULL, (yyvsp[-1].bsqon_value_node)))); }
#line 2475 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 100: /* bsqonbracevalue: '{' bsqonnamevall bsqon_braceval '}'  */
#line 305 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                          { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(BraceValue, BSQON_AST_TAG_BraceValue, MK_SPOS_R((yylsp[-3]), (yylsp[0])), BSQON_AST_NLIST_OF_VALUES_Reverse(BSQON_AST_NLIST_OF_VALUES_Push(BSQON_AST_NLIST_OF_VALUES_ENTRY_Create(NULL, (yyvsp[-1].bsqon_value_node)), (yyvsp[-2].bsqon_named_value_list)))); }
#line 2481 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 101: /* bsqonbracevalue: '{' error '}'  */
#line 306 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                   { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(BraceValue, BSQON_AST_TAG_BraceValue, MK_SPOS_R((yylsp[-2]), (yylsp[0])), BSQON_AST_NLIST_OF_VALUES_Singleton(BSQON_AST_NLIST_OF_VALUES_ENTRY_Create(NULL, BSQON_AST_ERROR(MK_SPOS_S((yylsp[-1])))))); yyerrok; }
#line 2487 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 102: /* bsqonbracevalue: '{' bsqonnamevall error '}'  */
#line 307 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                 { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(BraceValue, BSQON_AST_TAG_BraceValue, MK_SPOS_R((yylsp[-3]), (yylsp[0])), BSQON_AST_NLIST_OF_VALUES_Reverse(BSQON_AST_NLIST_OF_VALUES_Push(BSQON_AST_NLIST_OF_VALUES_ENTRY_Create(NULL, BSQON_AST_ERROR(MK_SPOS_S((yylsp[-1])))), (yyvsp[-2].bsqon_named_value_list)))); yyerrok; }
#line 2493 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 104: /* bsqonbracketbracevalue: bsqonbracevalue  */
#line 311 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                       { (yyval.bsqon_value_node) = (yyvsp[0].bsqon_value_node); }
#line 2499 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 105: /* bsqontypedvalue: '<' bsqontype '>' '(' bsqonbracketbracevalue ')'  */
#line 315 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                                     { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(TypedValue, BSQON_AST_TAG_TypedValue, MK_SPOS_R((yylsp[-5]), (yylsp[-2])), (yyvsp[-1].bsqon_value_node), (yyvsp[-4].bsqon_type_node), true); }
#line 2505 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 106: /* bsqontypedvalue: bsqonnominaltype bsqonbracketbracevalue  */
#line 316 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                             { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(TypedValue, BSQON_AST_TAG_TypedValue, MK_SPOS_R((yylsp[-1]), (yylsp[0])), (yyvsp[0].bsqon_value_node), (yyvsp[-1].bsqon_type_node), false); }
#line 2511 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 107: /* bsqontypedvalue: error '(' bsqonbracketbracevalue ')'  */
#line 317 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                             { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(TypedValue, BSQON_AST_TAG_TypedValue, MK_SPOS_R((yylsp[-3]), (yylsp[0])), (yyvsp[-1].bsqon_value_node), BSQON_AST_ERROR(MK_SPOS_S((yylsp[-2]))), true); }
#line 2517 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 108: /* bsqontypedvalue: error bsqonbracketbracevalue  */
#line 318 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                             { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(TypedValue, BSQON_AST_TAG_TypedValue, MK_SPOS_R((yylsp[-1]), (yylsp[0])), (yyvsp[0].bsqon_value_node), BSQON_AST_ERROR(MK_SPOS_S((yylsp[-1]))), false); }
#line 2523 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 110: /* bsqonstructvalue: bsqontypedvalue  */
#line 322 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                            { (yyval.bsqon_value_node) = (yyvsp[0].bsqon_value_node); }
#line 2529 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 111: /* bsqonspecialcons: "some" '(' bsqonval ')'  */
#line 326 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                             { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(SpecialConsValue, BSQON_AST_TAG_SomeConsValue, MK_SPOS_R((yylsp[-3]), (yylsp[0])), (yyvsp[-1].bsqon_value_node), "some"); }
#line 2535 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 112: /* bsqonspecialcons: "some" '(' error ')'  */
#line 327 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                             { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(SpecialConsValue, BSQON_AST_TAG_SomeConsValue, MK_SPOS_R((yylsp[-3]), (yylsp[0])), BSQON_AST_ERROR(MK_SPOS_S((yylsp[-1]))), "some"); yyerrok; }
#line 2541 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 113: /* bsqonspecialcons: "ok" '(' bsqonval ')'  */
#line 328 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                             { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(SpecialConsValue, BSQON_AST_TAG_OkConsValue, MK_SPOS_R((yylsp[-3]), (yylsp[0])), (yyvsp[-1].bsqon_value_node), "ok"); }
#line 2547 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 114: /* bsqonspecialcons: "ok" '(' error ')'  */
#line 329 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                             { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(SpecialConsValue, BSQON_AST_TAG_OkConsValue, MK_SPOS_R((yylsp[-3]), (yylsp[0])), BSQON_AST_ERROR(MK_SPOS_S((yylsp[-1]))), "ok"); yyerrok; }
#line 2553 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 115: /* bsqonspecialcons: "err" '(' bsqonval ')'  */
#line 330 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                             { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(SpecialConsValue, BSQON_AST_TAG_ErrConsValue, MK_SPOS_R((yylsp[-3]), (yylsp[0])), (yyvsp[-1].bsqon_value_node), "err"); }
#line 2559 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 116: /* bsqonspecialcons: "err" '(' error ')'  */
#line 331 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                             { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(SpecialConsValue, BSQON_AST_TAG_ErrConsValue, MK_SPOS_R((yylsp[-3]), (yylsp[0])), BSQON_AST_ERROR(MK_SPOS_S((yylsp[-1]))), "err"); yyerrok; }
#line 2565 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 121: /* bsqonval: bsqonaccess  */
#line 335 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                                                                  { (yyval.bsqon_value_node) = (yyvsp[0].bsqon_value_node); }
#line 2571 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 122: /* bsqonletexp: '(' "let" "identifier" ":" bsqontype "=" bsqonval "in" bsqonval ')'  */
#line 339 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                                                                         { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(LetInValue, BSQON_AST_TAG_LetInValue, MK_SPOS_R((yylsp[-9]), (yylsp[0])), (yyvsp[-7].str), (yyvsp[-5].bsqon_type_node), (yyvsp[-3].bsqon_value_node), (yyvsp[-1].bsqon_value_node)); }
#line 2577 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 125: /* bsqonref: bsqonaccess  */
#line 343 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                                         { (yyval.bsqon_value_node) = (yyvsp[0].bsqon_value_node); }
#line 2583 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 126: /* bsqonaccess: bsqonref "." "identifier"  */
#line 347 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                                            { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(AccessNameValue, BSQON_AST_TAG_AccessNameValue, MK_SPOS_R((yylsp[-2]), (yylsp[0])), (yyvsp[-2].bsqon_value_node), (yyvsp[0].str)); }
#line 2589 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 127: /* bsqonaccess: bsqonref "[|" "numberino" "|]"  */
#line 348 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                                            { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(AccessIndexValue, BSQON_AST_TAG_AccessIndexValue, MK_SPOS_R((yylsp[-3]), (yylsp[-1])), (yyvsp[-3].bsqon_value_node), (yyvsp[-1].str)); }
#line 2595 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 128: /* bsqonaccess: bsqonref "[|" bsqonterminal "|]"  */
#line 349 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                                            { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(AccessKeyValue, BSQON_AST_TAG_AccessKeyValue, MK_SPOS_R((yylsp[-3]), (yylsp[0])), (yyvsp[-3].bsqon_value_node), (yyvsp[-1].bsqon_value_node)); }
#line 2601 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 129: /* bsqonenvlist: "env" '{' '}'  */
#line 353 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                  { (yyval.bsqon_named_type_list) = NULL; }
#line 2607 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 130: /* bsqonenvlist: "env" '{' "identifier" ":" bsqontype '}'  */
#line 354 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                                         { (yyval.bsqon_named_type_list) = BSQON_AST_NLIST_OF_TYPES_Singleton(BSQON_AST_NLIST_OF_TYPES_ENTRY_Create((yyvsp[-3].str), (yyvsp[-1].bsqon_type_node))); }
#line 2613 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 131: /* bsqonenvlist: "env" '{' bsqonnametypel "identifier" ":" bsqontype '}'  */
#line 355 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                                                        { (yyval.bsqon_named_type_list) = BSQON_AST_NLIST_OF_TYPES_Reverse(BSQON_AST_NLIST_OF_TYPES_Push(BSQON_AST_NLIST_OF_TYPES_ENTRY_Create((yyvsp[-3].str), (yyvsp[-1].bsqon_type_node)), (yyvsp[-4].bsqon_named_type_list))); }
#line 2619 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 132: /* bsqonenvlist: "env" '{' "identifier" ":" error '}'  */
#line 356 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                                     { (yyval.bsqon_named_type_list) = BSQON_AST_NLIST_OF_TYPES_Singleton(BSQON_AST_NLIST_OF_TYPES_ENTRY_Create((yyvsp[-3].str), BSQON_AST_ERROR(MK_SPOS_S((yylsp[-1]))))); yyerrok; }
#line 2625 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 133: /* bsqonenvlist: "env" '{' bsqonnametypel "identifier" ":" error '}'  */
#line 357 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                                                    { (yyval.bsqon_named_type_list) = BSQON_AST_NLIST_OF_TYPES_Reverse(BSQON_AST_NLIST_OF_TYPES_Push(BSQON_AST_NLIST_OF_TYPES_ENTRY_Create((yyvsp[-3].str), BSQON_AST_ERROR(MK_SPOS_S((yylsp[-3])))), (yyvsp[-4].bsqon_named_type_list))); yyerrok; }
#line 2631 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 134: /* bsqoncomponent: bsqonval  */
#line 361 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
            { (yyval.bsqon_decl) = BSQON_AST_NODE_CONS(BsqonDeclBody, BSQON_AST_TAG_BsqonDeclBody, MK_SPOS_S((yylsp[0])), NULL, NULL, (yyvsp[0].bsqon_value_node)); }
#line 2637 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 135: /* bsqoncomponent: bsqonenvlist bsqonval  */
#line 362 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                           { (yyval.bsqon_decl) = BSQON_AST_NODE_CONS(BsqonDeclBody, BSQON_AST_TAG_BsqonDeclBody, MK_SPOS_R((yylsp[-1]), (yylsp[0])), NULL, (yyvsp[-1].bsqon_named_type_list), (yyvsp[0].bsqon_value_node)); }
#line 2643 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 136: /* bsqoncomponent: "shebang line" bsqonval  */
#line 363 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                 { (yyval.bsqon_decl) = BSQON_AST_NODE_CONS(BsqonDeclBody, BSQON_AST_TAG_BsqonDeclBody, MK_SPOS_R((yylsp[-1]), (yylsp[0])), (yyvsp[-1].str), NULL, (yyvsp[0].bsqon_value_node)); }
#line 2649 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 137: /* bsqoncomponent: "shebang line" bsqonenvlist bsqonval  */
#line 364 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                              { (yyval.bsqon_decl) = BSQON_AST_NODE_CONS(BsqonDeclBody, BSQON_AST_TAG_BsqonDeclBody, MK_SPOS_R((yylsp[-2]), (yylsp[0])), (yyvsp[-2].str), (yyvsp[-1].bsqon_named_type_list), (yyvsp[0].bsqon_value_node)); }
#line 2655 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 138: /* bsqonroot: bsqoncomponent  */
#line 368 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                  { yybsqonval = (yyvsp[0].bsqon_decl); (yyval.bsqon_decl) = (yyvsp[0].bsqon_decl); }
#line 2661 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 139: /* bsqonroot: error  */
#line 369 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
           {yybsqonval = BSQON_AST_ERROR(MK_SPOS_S((yylsp[0]))); (yyval.bsqon_decl) = BSQON_AST_ERROR(MK_SPOS_S((yylsp[0]))); }
#line 2667 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;


#line 2671 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"

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
  *++yylsp = yyloc;

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
          = {yyssp, yytoken, &yylloc};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
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
          YYNOMEM;
      }
    }

  yyerror_range[1] = yylloc;
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
                      yytoken, &yylval, &yylloc);
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

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

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
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}

#line 371 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"


extern FILE* yyin;

void yyerror(const char *s, ...)
{
   va_list ap;
   va_start(ap, s);

   if(yylloc.first_line) {
      int ccount = snprintf(errorbuf, MAX_ERROR_LENGTH, "%s @ %d.%d-%d.%d -- %s", s, yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column, filename);

      if(errorcount < MAX_PARSER_ERRORS) {
         errors[errorcount++] = strndup(errorbuf, ccount);
      }
   }
}

#ifndef EXPORT
int main(int argc, char** argv)
{
   if(argc > 1 && !strcmp(argv[1], "-d")) {
      yydebug = 1; argc--; argv++;
   }

    //see page 34 of book

   if(argc == 1) {
      yyin = stdin;
      filename = "<stdin>";
   }
   else {
      if((yyin = fopen(argv[1], "r")) == NULL) {
         perror(argv[1]);
         exit(1);
      }

      filename = argv[1];
   }

   if(!yyparse()) {
      //----------------------------
      BSQON_AST_print(yybsqonval);
      //BSQON_TYPE_AST_print(yybsqonval_type);

      printf("\n");
      fflush(stdout);
   }
      
   for(size_t i = 0; i < errorcount; ++i) {
      printf("%s\n", errors[i]);
      fflush(stdout);
   }
}
#else
const struct BSQON_AST_Node* BSQON_AST_parse_from_stdin()
{
   yyin = stdin;
   filename = "<stdin>";

   if(!yyparse()) {
      return yybsqonval;
   }
   else {
      return NULL;
   }
}

const struct BSQON_AST_Node* BSQON_AST_parse_from_file(const char* file)
{
   if((yyin = fopen(file, "r")) == NULL) {
      perror(file);
      exit(1);
   }
   
   if(!yyparse()) {
      return yybsqonval;
   }
   else {
      return NULL;
   }
}

size_t BSQON_AST_getErrorInfo(char** errorInfo)
{
   for(size_t i = 0; i < errorcount; ++i) {
      errorInfo[i] = errors[i];
   }
   fflush(stdout);
   
   return errorcount;
}
#endif
