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
  YYSYMBOL_SYM_BAR = 3,                    /* SYM_BAR  */
  YYSYMBOL_4_ = 4,                         /* "|"  */
  YYSYMBOL_SYM_AMP = 5,                    /* SYM_AMP  */
  YYSYMBOL_6_ = 6,                         /* "&"  */
  YYSYMBOL_KW_NONE = 7,                    /* "none"  */
  YYSYMBOL_KW_NOTHING = 8,                 /* "nothing"  */
  YYSYMBOL_KW_TRUE = 9,                    /* "true"  */
  YYSYMBOL_KW_FALSE = 10,                  /* "false"  */
  YYSYMBOL_KW_SOMETHING = 11,              /* "something"  */
  YYSYMBOL_KW_OK = 12,                     /* "ok"  */
  YYSYMBOL_KW_ERR = 13,                    /* "err"  */
  YYSYMBOL_KW_NPOS = 14,                   /* "$npos"  */
  YYSYMBOL_KW_INDEX = 15,                  /* "$i"  */
  YYSYMBOL_KW_KEY = 16,                    /* "$key"  */
  YYSYMBOL_KW_SRC = 17,                    /* "$src"  */
  YYSYMBOL_KW_LET = 18,                    /* "let"  */
  YYSYMBOL_KW_IN = 19,                     /* "in"  */
  YYSYMBOL_KW_ENV = 20,                    /* "env"  */
  YYSYMBOL_SYM_DOUBLE_COLON = 21,          /* "::"  */
  YYSYMBOL_SYM_ENTRY = 22,                 /* "=>"  */
  YYSYMBOL_SYM_COLON = 23,                 /* ":"  */
  YYSYMBOL_SYM_COMMA = 24,                 /* ","  */
  YYSYMBOL_SYM_EQUALS = 25,                /* "="  */
  YYSYMBOL_SYM_UNDERSCORE = 26,            /* "_"  */
  YYSYMBOL_TOKEN_NAT = 27,                 /* "nat literal"  */
  YYSYMBOL_TOKEN_INT = 28,                 /* "int literal"  */
  YYSYMBOL_TOKEN_BIG_NAT = 29,             /* "big nat literal"  */
  YYSYMBOL_TOKEN_BIG_INT = 30,             /* "big int literal"  */
  YYSYMBOL_TOKEN_RATIONAL = 31,            /* "rational literal"  */
  YYSYMBOL_TOKEN_FLOAT = 32,               /* "float literal"  */
  YYSYMBOL_TOKEN_DECIMAL = 33,             /* "decimal literal"  */
  YYSYMBOL_TOKEN_DECIMAL_DEGREE = 34,      /* "decimal degree literal"  */
  YYSYMBOL_TOKEN_COMPLEX = 35,             /* "complex literal"  */
  YYSYMBOL_TOKEN_LAT_LONG = 36,            /* "geo coordinate literal"  */
  YYSYMBOL_TOKEN_NUMBERINO = 37,           /* "numberino"  */
  YYSYMBOL_TOKEN_BYTE_BUFFER = 38,         /* "byte buffer"  */
  YYSYMBOL_TOKEN_UUID_V4 = 39,             /* "uuid (v4)"  */
  YYSYMBOL_TOKEN_UUID_V7 = 40,             /* "uuid (v7)"  */
  YYSYMBOL_TOKEN_SHA_HASH = 41,            /* "sha3 hashcode (512 bits)"  */
  YYSYMBOL_TOKEN_STRING = 42,              /* "string"  */
  YYSYMBOL_TOKEN_ASCII_STRING = 43,        /* "ascii string"  */
  YYSYMBOL_TOKEN_REGEX = 44,               /* "regular expression"  */
  YYSYMBOL_TOKEN_PATH_ITEM = 45,           /* "path item"  */
  YYSYMBOL_TOKEN_DATE_TIME = 46,           /* "date & time with timezone"  */
  YYSYMBOL_TOKEN_UTC_DATE_TIME = 47,       /* "date & time in UTC"  */
  YYSYMBOL_TOKEN_PLAIN_DATE = 48,          /* "plain date"  */
  YYSYMBOL_TOKEN_PLAIN_TIME = 49,          /* "plain time"  */
  YYSYMBOL_TOKEN_DELTA_DATE_TIME = 50,     /* "date & time delta"  */
  YYSYMBOL_TOKEN_DELTA_PLAIN_DATE = 51,    /* "plain date delta"  */
  YYSYMBOL_TOKEN_DELTA_PLAIN_TIME = 52,    /* "plain time delta"  */
  YYSYMBOL_TOKEN_LOGICAL_TIME = 53,        /* "logical time"  */
  YYSYMBOL_TOKEN_TICK_TIME = 54,           /* "tick time"  */
  YYSYMBOL_TOKEN_TIMESTAMP = 55,           /* "ISO timestamp"  */
  YYSYMBOL_TOKEN_DELTA_ISOTIMESTAMP = 56,  /* "timestamp delta"  */
  YYSYMBOL_TOKEN_DELTA_SECONDS = 57,       /* "delta in seconds"  */
  YYSYMBOL_TOKEN_DELTA_TICK = 58,          /* "tick delta"  */
  YYSYMBOL_TOKEN_DELTA_LOGICAL = 59,       /* "logical time delta"  */
  YYSYMBOL_TOKEN_IDENTIFIER = 60,          /* "identifier"  */
  YYSYMBOL_TOKEN_TYPE_COMPONENT = 61,      /* "type name"  */
  YYSYMBOL_TOKEN_UNSPEC_IDENTIFIER = 62,   /* "unspec identifier"  */
  YYSYMBOL_TOKEN_SHEBANG_LINE = 63,        /* "shebang line"  */
  YYSYMBOL_SYM_DOT = 64,                   /* "."  */
  YYSYMBOL_65_ = 65,                       /* '<'  */
  YYSYMBOL_66_ = 66,                       /* '>'  */
  YYSYMBOL_67_ = 67,                       /* '['  */
  YYSYMBOL_68_ = 68,                       /* ']'  */
  YYSYMBOL_69_ = 69,                       /* '{'  */
  YYSYMBOL_70_ = 70,                       /* '}'  */
  YYSYMBOL_71_ = 71,                       /* '('  */
  YYSYMBOL_72_ = 72,                       /* ')'  */
  YYSYMBOL_YYACCEPT = 73,                  /* $accept  */
  YYSYMBOL_bsqontypel = 74,                /* bsqontypel  */
  YYSYMBOL_bsqontypel_entry = 75,          /* bsqontypel_entry  */
  YYSYMBOL_bsqonnametypel = 76,            /* bsqonnametypel  */
  YYSYMBOL_bsqonnametypel_entry = 77,      /* bsqonnametypel_entry  */
  YYSYMBOL_bsqonnominaltype = 78,          /* bsqonnominaltype  */
  YYSYMBOL_bsqontermslist = 79,            /* bsqontermslist  */
  YYSYMBOL_bsqontupletype = 80,            /* bsqontupletype  */
  YYSYMBOL_bsqonrecordtype = 81,           /* bsqonrecordtype  */
  YYSYMBOL_bsqontype = 82,                 /* bsqontype  */
  YYSYMBOL_bsqontspec = 83,                /* bsqontspec  */
  YYSYMBOL_bsqonliteral = 84,              /* bsqonliteral  */
  YYSYMBOL_bsqonunspecvar = 85,            /* bsqonunspecvar  */
  YYSYMBOL_bsqonidentifier = 86,           /* bsqonidentifier  */
  YYSYMBOL_bsqonscopedidentifier = 87,     /* bsqonscopedidentifier  */
  YYSYMBOL_bsqonstringof = 88,             /* bsqonstringof  */
  YYSYMBOL_bsqonpath = 89,                 /* bsqonpath  */
  YYSYMBOL_bsqontypeliteral = 90,          /* bsqontypeliteral  */
  YYSYMBOL_bsqonenvaccess = 91,            /* bsqonenvaccess  */
  YYSYMBOL_bsqonterminal = 92,             /* bsqonterminal  */
  YYSYMBOL_bsqon_mapentry = 93,            /* bsqon_mapentry  */
  YYSYMBOL_bsqonvall = 94,                 /* bsqonvall  */
  YYSYMBOL_bsqonl_entry = 95,              /* bsqonl_entry  */
  YYSYMBOL_bsqonbracketvalue = 96,         /* bsqonbracketvalue  */
  YYSYMBOL_bsqonnamevall = 97,             /* bsqonnamevall  */
  YYSYMBOL_bsqon_braceval = 98,            /* bsqon_braceval  */
  YYSYMBOL_bsqonnameval_entry = 99,        /* bsqonnameval_entry  */
  YYSYMBOL_bsqonbracevalue = 100,          /* bsqonbracevalue  */
  YYSYMBOL_bsqonbracketbracevalue = 101,   /* bsqonbracketbracevalue  */
  YYSYMBOL_bsqontypedvalue = 102,          /* bsqontypedvalue  */
  YYSYMBOL_bsqonstructvalue = 103,         /* bsqonstructvalue  */
  YYSYMBOL_bsqonspecialcons = 104,         /* bsqonspecialcons  */
  YYSYMBOL_bsqonval = 105,                 /* bsqonval  */
  YYSYMBOL_bsqonletexp = 106,              /* bsqonletexp  */
  YYSYMBOL_bsqonref = 107,                 /* bsqonref  */
  YYSYMBOL_bsqonidx = 108,                 /* bsqonidx  */
  YYSYMBOL_bsqonaccess = 109,              /* bsqonaccess  */
  YYSYMBOL_bsqonenvlist = 110,             /* bsqonenvlist  */
  YYSYMBOL_bsqoncomponent = 111,           /* bsqoncomponent  */
  YYSYMBOL_bsqonroot = 112                 /* bsqonroot  */
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
#define YYFINAL  121
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1337

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  73
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  40
/* YYNRULES -- Number of rules.  */
#define YYNRULES  167
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  284

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   319


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
      71,    72,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      65,     2,    66,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    67,     2,    68,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    69,     2,    70,     2,     2,     2,     2,
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
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   153,   153,   154,   158,   159,   163,   164,   168,   169,
     173,   174,   175,   179,   180,   181,   182,   186,   187,   188,
     189,   190,   194,   195,   196,   197,   198,   202,   203,   204,
     205,   206,   207,   208,   212,   213,   214,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   257,   261,   262,   263,   264,   265,   269,
     273,   274,   278,   282,   286,   287,   291,   291,   291,   291,
     291,   291,   291,   291,   295,   296,   297,   298,   302,   303,
     307,   308,   312,   313,   314,   315,   316,   320,   321,   325,
     325,   329,   330,   331,   332,   336,   337,   338,   339,   340,
     341,   342,   343,   344,   348,   348,   352,   353,   354,   355,
     359,   359,   363,   364,   365,   366,   367,   368,   372,   372,
     372,   372,   372,   376,   380,   380,   380,   380,   384,   384,
     384,   388,   389,   390,   391,   392,   393,   397,   398,   399,
     400,   401,   405,   406,   407,   408,   412,   413
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
  "\"end of file\"", "error", "\"invalid token\"", "SYM_BAR", "\"|\"",
  "SYM_AMP", "\"&\"", "\"none\"", "\"nothing\"", "\"true\"", "\"false\"",
  "\"something\"", "\"ok\"", "\"err\"", "\"$npos\"", "\"$i\"", "\"$key\"",
  "\"$src\"", "\"let\"", "\"in\"", "\"env\"", "\"::\"", "\"=>\"", "\":\"",
  "\",\"", "\"=\"", "\"_\"", "\"nat literal\"", "\"int literal\"",
  "\"big nat literal\"", "\"big int literal\"", "\"rational literal\"",
  "\"float literal\"", "\"decimal literal\"", "\"decimal degree literal\"",
  "\"complex literal\"", "\"geo coordinate literal\"", "\"numberino\"",
  "\"byte buffer\"", "\"uuid (v4)\"", "\"uuid (v7)\"",
  "\"sha3 hashcode (512 bits)\"", "\"string\"", "\"ascii string\"",
  "\"regular expression\"", "\"path item\"",
  "\"date & time with timezone\"", "\"date & time in UTC\"",
  "\"plain date\"", "\"plain time\"", "\"date & time delta\"",
  "\"plain date delta\"", "\"plain time delta\"", "\"logical time\"",
  "\"tick time\"", "\"ISO timestamp\"", "\"timestamp delta\"",
  "\"delta in seconds\"", "\"tick delta\"", "\"logical time delta\"",
  "\"identifier\"", "\"type name\"", "\"unspec identifier\"",
  "\"shebang line\"", "\".\"", "'<'", "'>'", "'['", "']'", "'{'", "'}'",
  "'('", "')'", "$accept", "bsqontypel", "bsqontypel_entry",
  "bsqonnametypel", "bsqonnametypel_entry", "bsqonnominaltype",
  "bsqontermslist", "bsqontupletype", "bsqonrecordtype", "bsqontype",
  "bsqontspec", "bsqonliteral", "bsqonunspecvar", "bsqonidentifier",
  "bsqonscopedidentifier", "bsqonstringof", "bsqonpath",
  "bsqontypeliteral", "bsqonenvaccess", "bsqonterminal", "bsqon_mapentry",
  "bsqonvall", "bsqonl_entry", "bsqonbracketvalue", "bsqonnamevall",
  "bsqon_braceval", "bsqonnameval_entry", "bsqonbracevalue",
  "bsqonbracketbracevalue", "bsqontypedvalue", "bsqonstructvalue",
  "bsqonspecialcons", "bsqonval", "bsqonletexp", "bsqonref", "bsqonidx",
  "bsqonaccess", "bsqonenvlist", "bsqoncomponent", "bsqonroot", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-165)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-151)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     255,   192,  -165,  -165,  -165,  -165,   -66,   -54,   -16,  -165,
    -165,  -165,  -165,   185,  -165,  -165,  -165,  -165,  -165,  -165,
    -165,  -165,  -165,  -165,  -165,  -165,  -165,  -165,    29,    29,
    -165,    29,  -165,  -165,  -165,  -165,  -165,  -165,  -165,  -165,
    -165,  -165,  -165,  -165,  -165,  -165,  -165,    41,  -165,   450,
      56,   320,   385,   122,   118,   -11,  -165,   -27,   152,  -165,
    -165,  -165,  -165,  -165,  -165,  -165,  -165,  -165,  -165,  -165,
    -165,  -165,   169,   177,   515,  -165,   155,  -165,   580,   645,
     710,   182,   103,    78,   138,   138,   138,    31,  -165,   192,
    -165,   515,   102,    15,    91,   138,  -165,  -165,   108,   170,
     258,  -165,  -165,   775,  -165,   157,    25,   100,   175,  -165,
     840,   206,  -165,   196,   214,   282,  -165,    29,   -18,  1220,
    -165,  -165,   -46,   163,   272,   207,   337,   247,    40,   138,
    -165,  -165,    60,   277,   323,  -165,   324,  -165,   322,   -21,
      43,  -165,    30,  -165,   192,   -22,  -165,    74,    47,   367,
    -165,   348,   192,   905,  -165,  -165,   179,  -165,   209,  -165,
     970,  -165,  -165,  -165,  1035,   158,   386,   208,  -165,  -165,
    -165,   387,  -165,  -165,   138,  -165,  -165,  1276,   427,   219,
     426,   446,   403,   451,  -165,  -165,  -165,  -165,  -165,  -165,
     401,    17,   182,   182,   408,  -165,   117,   453,  -165,  -165,
    -165,    19,  -165,   174,  -165,  -165,  -165,  -165,   125,    63,
    -165,   128,   464,  -165,   192,  -165,  -165,  -165,   192,  -165,
     141,   210,  -165,  1100,  -165,   182,  -165,  -165,  -165,  -165,
    -165,  -165,   445,  -165,  1165,  -165,  -165,   509,  -165,   477,
     268,     7,   135,  -165,  -165,  -165,  -165,   333,    24,   146,
    -165,  -165,  -165,  -165,   188,   383,    33,  -165,  -165,   465,
     466,  -165,  -165,  -165,  -165,   398,   139,  -165,  -165,   448,
     151,  -165,  -165,   515,  -165,  -165,  -165,  -165,  -165,  -165,
     517,   515,   467,  -165
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,   167,    37,    38,    39,    40,     0,     0,     0,    75,
      76,    77,    74,     0,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      58,    57,    59,    60,    61,    62,    66,    67,    68,    63,
      64,    65,    69,    70,    71,    72,    78,    10,    73,     0,
       0,     0,     0,     0,     0,    86,    87,    88,    89,    90,
      91,    92,    93,   138,   124,   125,   130,   131,   140,   139,
     162,   141,     0,   142,     0,   166,     0,   129,     0,     0,
       0,     0,     0,     0,    80,    81,    82,     0,    11,     0,
     164,     0,     0,     0,     0,    34,    35,    36,     0,     0,
       0,   102,   110,     0,    99,     0,   109,     0,    78,   115,
       0,     0,   108,   109,     0,     0,   127,     0,     0,     0,
     163,     1,     0,     0,     0,     0,     0,     0,     0,    27,
      28,    29,     0,     0,     0,   157,     0,     7,     0,     0,
       0,     3,     0,   165,     0,     0,    17,     0,     0,     0,
      22,     0,     0,     0,   101,   105,     0,    98,   109,   100,
       0,   103,   114,   122,     0,     0,    78,     0,   107,   113,
     120,     0,    79,    12,    83,   152,   151,     0,     0,    86,
      88,    89,     0,     0,   133,   132,   135,   134,   137,   136,
       0,     0,     0,     0,     0,    84,     0,     0,     6,     5,
      15,     0,     2,     0,     4,    13,   128,    20,     0,     0,
      18,     0,     0,   126,    97,    95,   106,   104,    96,    94,
       0,     0,   123,     0,   121,     0,    55,    56,    57,   148,
     149,   150,     0,   153,     0,    33,    32,    31,    30,     0,
       0,     0,     0,    16,    14,    21,    19,     0,     0,     0,
     112,   118,   111,   116,     0,     0,     0,   155,   154,     0,
       0,     9,   160,     8,   158,     0,     0,    25,    23,     0,
       0,   119,   117,     0,   156,    85,   161,   159,    26,    24,
       0,     0,     0,   143
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -165,   444,  -122,   447,  -112,   -20,  -165,   488,   490,   -59,
    -165,  -118,  -165,  -115,  -113,  -165,  -165,  -165,  -165,   433,
    -165,  -165,   475,  -165,  -165,   -38,   469,  -165,     6,  -165,
    -165,  -165,     0,  -165,  -165,  -164,  -165,   534,  -165,  -165
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,   140,   141,   136,   137,    54,    88,   130,   131,   132,
      98,    55,    56,    57,    58,    59,    60,    61,    62,    63,
     102,   103,   104,    64,   110,   105,   112,    65,    66,    67,
      68,    69,   113,    71,    72,   183,    73,    74,    75,    76
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      70,   179,   199,   199,   180,    78,   181,    77,    84,    85,
     192,    86,   193,   232,   111,   117,   145,    79,   202,   175,
     192,    51,   193,    52,   198,   202,   184,   192,   142,   193,
      95,   263,   139,   192,   148,   193,   192,  -145,   193,   198,
    -145,   190,   176,   199,   201,   200,   207,   160,   263,    90,
     192,   106,   193,  -144,   204,    80,  -144,    92,   273,   229,
     116,   129,   230,   192,   231,   193,   192,   129,   193,   191,
     259,   204,   167,   129,   120,   208,    47,   264,   123,   125,
     127,   203,    93,   146,    94,   243,   128,   204,   209,   236,
      47,   143,    47,   161,   268,    77,   205,   174,    93,   178,
      94,    47,   128,   158,    47,    77,    87,    93,   129,    94,
      93,   128,    94,    77,   128,   210,   229,    47,   240,   230,
     129,   231,   153,    93,   162,    94,   194,   129,    77,   247,
      77,   246,    77,   237,   238,    47,   265,   241,   134,   115,
     114,    93,   192,    94,   193,   128,   133,   269,   135,   199,
     206,   149,   248,   215,   192,   121,   193,   178,   213,   138,
     219,   150,    77,   263,   221,   250,   256,    51,   144,    52,
     163,    77,   129,   129,   152,   263,   129,   192,    47,   193,
     153,   159,   162,   266,    93,    51,    94,    52,   128,    47,
     270,   129,   153,   245,   154,    93,    47,    94,   204,   128,
     164,   153,    93,   154,    94,   129,   128,    47,    51,   277,
      52,   251,   250,    93,   178,    94,  -146,   128,   160,  -146,
      77,   279,   129,   255,    77,    51,    77,    52,   222,   129,
     169,   160,   169,   118,   252,   185,   119,    51,   155,    52,
     244,  -147,  -148,    47,  -147,   117,    51,   216,    52,    93,
      81,    94,    82,   128,    83,    51,     1,    52,   271,    51,
      77,    52,     2,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,   280,   171,    13,   170,   217,   224,   187,
     253,   282,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,   261,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,   189,
      50,    99,    51,    81,    52,    82,    53,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,   262,    51,
     100,    52,   172,   173,   186,   195,   196,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,   261,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,   173,   197,    50,   107,    51,   101,    52,
     211,    53,     2,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,   267,    51,   100,    52,   252,   212,   188,
     225,   223,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,   261,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,   108,    47,    48,   115,  -149,
      50,    89,    51,   272,    52,   109,    53,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,   276,  -150,
      13,   233,   261,   235,   234,   239,   242,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,   249,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,   257,   193,    50,    89,    51,   278,    52,
     260,    53,     2,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,   274,   275,   100,   281,   147,    96,   283,
      97,   151,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,   182,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,   157,   168,
      50,   122,    51,    91,    52,     0,    53,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
     100,     0,     0,     0,     0,     0,     0,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,     0,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,     0,     0,    50,   124,    51,     0,    52,
       0,    53,     2,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,   100,     0,     0,     0,     0,
       0,     0,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,     0,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,     0,     0,
      50,   126,    51,     0,    52,     0,    53,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
     100,     0,     0,     0,     0,     0,     0,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,     0,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,     0,     0,    50,   156,    51,     0,    52,
       0,    53,     2,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,   100,     0,     0,     0,     0,
       0,     0,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,     0,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,     0,     0,
      50,   165,    51,     0,    52,     0,    53,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
     100,     0,     0,     0,     0,     0,     0,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,     0,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
     166,    47,    48,     0,     0,    50,   214,    51,     0,    52,
       0,    53,     2,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,   100,     0,     0,     0,     0,
       0,     0,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,     0,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,     0,     0,
      50,   218,    51,     0,    52,     0,    53,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
     100,     0,     0,     0,     0,     0,     0,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,     0,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,     0,     0,    50,   220,    51,     0,    52,
       0,    53,     2,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,   100,     0,     0,     0,     0,
       0,     0,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,     0,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,     0,     0,
      50,   254,    51,     0,    52,     0,    53,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
     100,     0,     0,     0,     0,     0,     0,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,     0,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,     0,     0,    50,     0,    51,     0,    52,
       0,    53,     2,     3,     4,     5,     0,     0,     0,     9,
      10,    11,    12,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,     0,    24,    25,    26,    27,   226,   227,    30,
     228,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,     2,     3,     4,
       5,     0,     0,   258,     9,    10,    11,    12,     0,     0,
     100,     0,     0,   177,     0,     0,     0,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,     0,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,     2,     3,     4,     5,     0,     0,     0,
       9,    10,    11,    12,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,     0,    24,    25,    26,    27,   226,   227,
      30,   228,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47
};

static const yytype_int16 yycheck[] =
{
       0,   119,    24,    24,   119,    71,   119,     1,    28,    29,
       3,    31,     5,   177,    52,    26,     1,    71,   140,    37,
       3,    67,     5,    69,   136,   147,    72,     3,    87,     5,
      50,    24,     1,     3,    93,     5,     3,    64,     5,   151,
      67,     1,    60,    24,     1,    66,    68,    22,    24,    49,
       3,    51,     5,    64,    24,    71,    67,     1,    25,   177,
      54,    81,   177,     3,   177,     5,     3,    87,     5,   128,
     234,    24,   110,    93,    74,     1,    61,    70,    78,    79,
      80,   140,    67,    68,    69,    66,    71,    24,   147,    72,
      61,    91,    61,    68,    70,    89,    66,   117,    67,   119,
      69,    61,    71,   103,    61,    99,    65,    67,   128,    69,
      67,    71,    69,   107,    71,    68,   234,    61,     1,   234,
     140,   234,    22,    67,    24,    69,    66,   147,   122,     1,
     124,    68,   126,   192,   193,    61,     1,   196,    60,    21,
      18,    67,     3,    69,     5,    71,    43,     1,    70,    24,
     144,    60,   211,   153,     3,     0,     5,   177,   152,    21,
     160,    70,   156,    24,   164,    24,   225,    67,    66,    69,
      70,   165,   192,   193,    66,    24,   196,     3,    61,     5,
      22,    24,    24,   242,    67,    67,    69,    69,    71,    61,
     249,   211,    22,    68,    24,    67,    61,    69,    24,    71,
      25,    22,    67,    24,    69,   225,    71,    61,    67,    70,
      69,    70,    24,    67,   234,    69,    64,    71,    22,    67,
     214,    70,   242,   223,   218,    67,   220,    69,    70,   249,
      24,    22,    24,    64,    24,    72,    67,    67,    68,    69,
      66,    64,    23,    61,    67,    26,    67,    68,    69,    67,
      65,    69,    67,    71,    69,    67,     1,    69,    70,    67,
     254,    69,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,   273,    60,    20,    70,    68,    70,    72,
      70,   281,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    24,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    72,
      65,     1,    67,    65,    69,    67,    71,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    70,    67,
      20,    69,    60,    61,    72,    68,    23,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    24,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    61,    60,    65,     1,    67,    68,    69,
      23,    71,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    70,    67,    20,    69,    24,    60,    72,
      23,    25,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    24,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    21,    23,
      65,     1,    67,    70,    69,    70,    71,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    70,    23,
      20,    68,    24,    72,    23,    67,    23,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    23,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    68,     5,    65,     1,    67,    70,    69,
      43,    71,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    68,    68,    20,    19,    93,    50,    72,
      50,    94,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,   119,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,   103,   110,
      65,     1,    67,    49,    69,    -1,    71,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    -1,    -1,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    -1,    -1,    65,     1,    67,    -1,    69,
      -1,    71,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    -1,    -1,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    -1,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    -1,    -1,
      65,     1,    67,    -1,    69,    -1,    71,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    -1,    -1,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    -1,    -1,    65,     1,    67,    -1,    69,
      -1,    71,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    -1,    -1,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    -1,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    -1,    -1,
      65,     1,    67,    -1,    69,    -1,    71,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    -1,    -1,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    -1,    -1,    65,     1,    67,    -1,    69,
      -1,    71,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    -1,    -1,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    -1,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    -1,    -1,
      65,     1,    67,    -1,    69,    -1,    71,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    -1,    -1,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    -1,    -1,    65,     1,    67,    -1,    69,
      -1,    71,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    -1,    -1,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    -1,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    -1,    -1,
      65,     1,    67,    -1,    69,    -1,    71,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    -1,    -1,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    -1,    -1,    65,    -1,    67,    -1,    69,
      -1,    71,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      15,    16,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    -1,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,     7,     8,     9,
      10,    -1,    -1,    68,    14,    15,    16,    17,    -1,    -1,
      20,    -1,    -1,    23,    -1,    -1,    -1,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     1,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    20,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      65,    67,    69,    71,    78,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    96,   100,   101,   102,   103,   104,
     105,   106,   107,   109,   110,   111,   112,   101,    71,    71,
      71,    65,    67,    69,    78,    78,    78,    65,    79,     1,
     105,   110,     1,    67,    69,    78,    80,    81,    83,     1,
      20,    68,    93,    94,    95,    98,   105,     1,    60,    70,
      97,    98,    99,   105,    18,    21,   101,    26,    64,    67,
     105,     0,     1,   105,     1,   105,     1,   105,    71,    78,
      80,    81,    82,    43,    60,    70,    76,    77,    21,     1,
      74,    75,    82,   105,    66,     1,    68,    74,    82,    60,
      70,    76,    66,    22,    24,    68,     1,    95,   105,    24,
      22,    68,    24,    70,    25,     1,    60,    98,    99,    24,
      70,    60,    60,    61,    78,    37,    60,    23,    78,    84,
      86,    87,    92,   108,    72,    72,    72,    72,    72,    72,
       1,    82,     3,     5,    66,    68,    23,    60,    77,    24,
      66,     1,    75,    82,    24,    66,   101,    68,     1,    82,
      68,    23,    60,   101,     1,   105,    68,    68,     1,   105,
       1,   105,    70,    25,    70,    23,    42,    43,    45,    84,
      86,    87,   108,    68,    23,    72,    72,    82,    82,    67,
       1,    82,    23,    66,    66,    68,    68,     1,    82,    23,
      24,    70,    24,    70,     1,   105,    82,    68,    68,   108,
      43,    24,    70,    24,    70,     1,    82,    70,    70,     1,
      82,    70,    70,    25,    68,    68,    70,    70,    70,    70,
     105,    19,   105,    72
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    73,    74,    74,    75,    75,    76,    76,    77,    77,
      78,    78,    78,    79,    79,    79,    79,    80,    80,    80,
      80,    80,    81,    81,    81,    81,    81,    82,    82,    82,
      82,    82,    82,    82,    83,    83,    83,    84,    84,    84,
      84,    84,    84,    84,    84,    84,    84,    84,    84,    84,
      84,    84,    84,    84,    84,    84,    84,    84,    84,    84,
      84,    84,    84,    84,    84,    84,    84,    84,    84,    84,
      84,    84,    84,    85,    86,    86,    86,    86,    86,    87,
      88,    88,    89,    90,    91,    91,    92,    92,    92,    92,
      92,    92,    92,    92,    93,    93,    93,    93,    94,    94,
      95,    95,    96,    96,    96,    96,    96,    97,    97,    98,
      98,    99,    99,    99,    99,   100,   100,   100,   100,   100,
     100,   100,   100,   100,   101,   101,   102,   102,   102,   102,
     103,   103,   104,   104,   104,   104,   104,   104,   105,   105,
     105,   105,   105,   106,   107,   107,   107,   107,   108,   108,
     108,   109,   109,   109,   109,   109,   109,   110,   110,   110,
     110,   110,   111,   111,   111,   111,   112,   112
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     1,     2,     2,     2,     1,     4,     4,
       1,     2,     3,     3,     4,     3,     4,     2,     3,     4,
       3,     4,     2,     5,     6,     5,     6,     1,     1,     1,
       3,     3,     3,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       2,     2,     2,     3,     4,     7,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     3,     3,     3,     2,     1,
       2,     2,     2,     3,     4,     3,     4,     2,     1,     1,
       1,     4,     4,     2,     2,     2,     5,     6,     5,     6,
       3,     4,     3,     4,     1,     1,     4,     2,     4,     2,
       1,     1,     4,     4,     4,     4,     4,     4,     1,     1,
       1,     1,     1,    10,     1,     1,     1,     1,     1,     1,
       1,     3,     3,     4,     5,     5,     6,     3,     6,     7,
       6,     7,     1,     2,     2,     3,     1,     1
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
#line 153 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                               { (yyval.bsqon_type_list) = BSQON_AST_LIST_OF_TYPES_Push((yyvsp[0].bsqon_type_node), (yyvsp[-1].bsqon_type_list)); }
#line 2006 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 3: /* bsqontypel: bsqontypel_entry  */
#line 154 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                      { (yyval.bsqon_type_list) = BSQON_AST_LIST_OF_TYPES_Singleton((yyvsp[0].bsqon_type_node)); }
#line 2012 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 4: /* bsqontypel_entry: bsqontype ","  */
#line 158 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                       { (yyval.bsqon_type_node) = (yyvsp[-1].bsqon_type_node); }
#line 2018 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 5: /* bsqontypel_entry: error ","  */
#line 159 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                     { (yyval.bsqon_type_node) = BSQON_AST_ERROR(MK_SPOS_S((yylsp[-1]))); yyerrok; }
#line 2024 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 6: /* bsqonnametypel: bsqonnametypel bsqonnametypel_entry  */
#line 163 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                       { (yyval.bsqon_named_type_list) = BSQON_AST_NLIST_OF_TYPES_Push((yyvsp[0].bsqon_named_type_list_entry), (yyvsp[-1].bsqon_named_type_list)); }
#line 2030 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 7: /* bsqonnametypel: bsqonnametypel_entry  */
#line 164 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                          { (yyval.bsqon_named_type_list) = BSQON_AST_NLIST_OF_TYPES_Singleton((yyvsp[0].bsqon_named_type_list_entry)); }
#line 2036 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 8: /* bsqonnametypel_entry: "identifier" ":" bsqontype ","  */
#line 168 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                                  { (yyval.bsqon_named_type_list_entry) = BSQON_AST_NLIST_OF_TYPES_ENTRY_Create((yyvsp[-3].str), (yyvsp[-1].bsqon_type_node)); }
#line 2042 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 9: /* bsqonnametypel_entry: "identifier" ":" error ","  */
#line 169 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                                { (yyval.bsqon_named_type_list_entry) = BSQON_AST_NLIST_OF_TYPES_ENTRY_Create((yyvsp[-3].str), BSQON_AST_ERROR(MK_SPOS_S((yylsp[-1])))); yyerrok; }
#line 2048 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 10: /* bsqonnominaltype: "type name"  */
#line 173 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                        { (yyval.bsqon_type_node) = BSQON_AST_NODE_CONS(NominalType, BSQON_AST_TAG_NominalType, MK_SPOS_S((yylsp[0])), (yyvsp[0].str), NULL); }
#line 2054 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 11: /* bsqonnominaltype: "type name" bsqontermslist  */
#line 174 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                         { (yyval.bsqon_type_node) = BSQON_AST_NODE_CONS(NominalType, BSQON_AST_TAG_NominalType, MK_SPOS_S((yylsp[-1])), (yyvsp[-1].str), (yyvsp[0].bsqon_type_list)); }
#line 2060 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 12: /* bsqonnominaltype: bsqonnominaltype "::" "type name"  */
#line 175 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                                            { (yyval.bsqon_type_node) = BSQON_AST_NODE_CONS(NominalScopedType, BSQON_AST_TAG_NominalScopedType, MK_SPOS_R((yylsp[-2]), (yylsp[0])), (yyvsp[-2].bsqon_type_node), (yyvsp[0].str)); }
#line 2066 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 13: /* bsqontermslist: '<' bsqontype '>'  */
#line 179 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                     { (yyval.bsqon_type_list) = BSQON_AST_LIST_OF_TYPES_Singleton((yyvsp[-1].bsqon_type_node)); }
#line 2072 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 14: /* bsqontermslist: '<' bsqontypel bsqontype '>'  */
#line 180 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                  { (yyval.bsqon_type_list) = BSQON_AST_LIST_OF_TYPES_Reverse(BSQON_AST_LIST_OF_TYPES_Push((yyvsp[-1].bsqon_type_node), (yyvsp[-2].bsqon_type_list))); }
#line 2078 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 15: /* bsqontermslist: '<' error '>'  */
#line 181 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                   { (yyval.bsqon_type_list) = BSQON_AST_LIST_OF_TYPES_Singleton(BSQON_AST_ERROR(MK_SPOS_S((yylsp[-1])))); yyerrok; }
#line 2084 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 16: /* bsqontermslist: '<' bsqontypel error '>'  */
#line 182 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_type_list) = BSQON_AST_LIST_OF_TYPES_Reverse(BSQON_AST_LIST_OF_TYPES_Push(BSQON_AST_ERROR(MK_SPOS_S((yylsp[-1]))), (yyvsp[-2].bsqon_type_list))); yyerrok; }
#line 2090 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 17: /* bsqontupletype: '[' ']'  */
#line 186 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
           { (yyval.bsqon_type_node) = BSQON_AST_NODE_CONS(TupleType, BSQON_AST_TAG_TupleType, MK_SPOS_R((yylsp[-1]), (yylsp[0])), NULL); }
#line 2096 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 18: /* bsqontupletype: '[' bsqontype ']'  */
#line 187 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                       { (yyval.bsqon_type_node) = BSQON_AST_NODE_CONS(TupleType, BSQON_AST_TAG_TupleType, MK_SPOS_R((yylsp[-2]), (yylsp[0])), BSQON_AST_LIST_OF_TYPES_Singleton((yyvsp[-1].bsqon_type_node))); }
#line 2102 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 19: /* bsqontupletype: '[' bsqontypel bsqontype ']'  */
#line 188 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                  { (yyval.bsqon_type_node) = BSQON_AST_NODE_CONS(TupleType, BSQON_AST_TAG_TupleType, MK_SPOS_R((yylsp[-3]), (yylsp[0])), BSQON_AST_LIST_OF_TYPES_Reverse(BSQON_AST_LIST_OF_TYPES_Push((yyvsp[-1].bsqon_type_node), (yyvsp[-2].bsqon_type_list)))); }
#line 2108 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 20: /* bsqontupletype: '[' error ']'  */
#line 189 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                   { (yyval.bsqon_type_node) = BSQON_AST_NODE_CONS(TupleType, BSQON_AST_TAG_TupleType, MK_SPOS_R((yylsp[-2]), (yylsp[0])), BSQON_AST_LIST_OF_TYPES_Singleton(BSQON_AST_ERROR(MK_SPOS_S((yylsp[-2]))))); yyerrok; }
#line 2114 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 21: /* bsqontupletype: '[' bsqontypel error ']'  */
#line 190 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_type_node) = BSQON_AST_NODE_CONS(TupleType, BSQON_AST_TAG_TupleType, MK_SPOS_R((yylsp[-3]), (yylsp[0])), BSQON_AST_LIST_OF_TYPES_Reverse(BSQON_AST_LIST_OF_TYPES_Push(BSQON_AST_ERROR(MK_SPOS_S((yylsp[-1]))), (yyvsp[-2].bsqon_type_list)))); yyerrok; }
#line 2120 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 22: /* bsqonrecordtype: '{' '}'  */
#line 194 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
           { (yyval.bsqon_type_node) = BSQON_AST_NODE_CONS(RecordType, BSQON_AST_TAG_RecordType, MK_SPOS_R((yylsp[-1]), (yylsp[0])), NULL); }
#line 2126 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 23: /* bsqonrecordtype: '{' "identifier" ":" bsqontype '}'  */
#line 195 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                                  { (yyval.bsqon_type_node) = BSQON_AST_NODE_CONS(RecordType, BSQON_AST_TAG_RecordType, MK_SPOS_R((yylsp[-4]), (yylsp[0])), BSQON_AST_NLIST_OF_TYPES_Singleton(BSQON_AST_NLIST_OF_TYPES_ENTRY_Create((yyvsp[-3].str), (yyvsp[-1].bsqon_type_node)))); }
#line 2132 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 24: /* bsqonrecordtype: '{' bsqonnametypel "identifier" ":" bsqontype '}'  */
#line 196 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                                                 { (yyval.bsqon_type_node) = BSQON_AST_NODE_CONS(RecordType, BSQON_AST_TAG_RecordType, MK_SPOS_R((yylsp[-5]), (yylsp[0])), BSQON_AST_NLIST_OF_TYPES_Reverse(BSQON_AST_NLIST_OF_TYPES_Push(BSQON_AST_NLIST_OF_TYPES_ENTRY_Create((yyvsp[-3].str), (yyvsp[-1].bsqon_type_node)), (yyvsp[-4].bsqon_named_type_list)))); }
#line 2138 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 25: /* bsqonrecordtype: '{' "identifier" ":" error '}'  */
#line 197 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                              { (yyval.bsqon_type_node) = BSQON_AST_NODE_CONS(RecordType, BSQON_AST_TAG_RecordType, MK_SPOS_R((yylsp[-4]), (yylsp[0])), BSQON_AST_NLIST_OF_TYPES_Singleton(BSQON_AST_NLIST_OF_TYPES_ENTRY_Create((yyvsp[-3].str), BSQON_AST_ERROR(MK_SPOS_S((yylsp[-1])))))); yyerrok; }
#line 2144 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 26: /* bsqonrecordtype: '{' bsqonnametypel "identifier" ":" error '}'  */
#line 198 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                                             { (yyval.bsqon_type_node) = BSQON_AST_NODE_CONS(RecordType, BSQON_AST_TAG_RecordType, MK_SPOS_R((yylsp[-5]), (yylsp[0])), BSQON_AST_NLIST_OF_TYPES_Reverse(BSQON_AST_NLIST_OF_TYPES_Push(BSQON_AST_NLIST_OF_TYPES_ENTRY_Create((yyvsp[-3].str), BSQON_AST_ERROR(MK_SPOS_S((yylsp[-3])))), (yyvsp[-4].bsqon_named_type_list)))); yyerrok; }
#line 2150 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 27: /* bsqontype: bsqonnominaltype  */
#line 202 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                    { (yyval.bsqon_type_node) = (yyvsp[0].bsqon_type_node); }
#line 2156 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 28: /* bsqontype: bsqontupletype  */
#line 203 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                    { (yyval.bsqon_type_node) = (yyvsp[0].bsqon_type_node); }
#line 2162 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 29: /* bsqontype: bsqonrecordtype  */
#line 204 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                     { (yyval.bsqon_type_node) = (yyvsp[0].bsqon_type_node); }
#line 2168 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 30: /* bsqontype: bsqontype SYM_AMP bsqontype  */
#line 205 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                 { (yyval.bsqon_type_node) = BSQON_AST_NODE_CONS(ConjunctionType, BSQON_AST_TAG_ConjunctionType, MK_SPOS_R((yylsp[-2]), (yylsp[0])), (yyvsp[-2].bsqon_type_node), (yyvsp[0].bsqon_type_node)); }
#line 2174 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 31: /* bsqontype: bsqontype SYM_BAR bsqontype  */
#line 206 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                 { (yyval.bsqon_type_node) = BSQON_AST_NODE_CONS(UnionType, BSQON_AST_TAG_UnionType, MK_SPOS_R((yylsp[-2]), (yylsp[0])), (yyvsp[-2].bsqon_type_node), (yyvsp[0].bsqon_type_node)); }
#line 2180 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 32: /* bsqontype: '(' bsqontype ')'  */
#line 207 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                       { (yyval.bsqon_type_node) = (yyvsp[-1].bsqon_type_node); }
#line 2186 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 33: /* bsqontype: '(' error ')'  */
#line 208 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                   { (yyval.bsqon_type_node) = BSQON_AST_ERROR(MK_SPOS_S((yylsp[-1]))); yyerrok; }
#line 2192 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 34: /* bsqontspec: bsqonnominaltype  */
#line 212 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                    { (yyval.bsqon_type_node) = (yyvsp[0].bsqon_type_node); }
#line 2198 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 35: /* bsqontspec: bsqontupletype  */
#line 213 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                    { (yyval.bsqon_type_node) = (yyvsp[0].bsqon_type_node); }
#line 2204 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 36: /* bsqontspec: bsqonrecordtype  */
#line 214 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                     { (yyval.bsqon_type_node) = (yyvsp[0].bsqon_type_node); }
#line 2210 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 37: /* bsqonliteral: "none"  */
#line 218 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(SingletonValue, BSQON_AST_TAG_NoneValue, MK_SPOS_S((yylsp[0]))); }
#line 2216 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 38: /* bsqonliteral: "nothing"  */
#line 219 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(SingletonValue, BSQON_AST_TAG_NothingValue, MK_SPOS_S((yylsp[0]))); }
#line 2222 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 39: /* bsqonliteral: "true"  */
#line 220 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(SingletonValue, BSQON_AST_TAG_TrueValue, MK_SPOS_S((yylsp[0]))); }
#line 2228 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 40: /* bsqonliteral: "false"  */
#line 221 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(SingletonValue, BSQON_AST_TAG_FalseValue, MK_SPOS_S((yylsp[0]))); }
#line 2234 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 41: /* bsqonliteral: "nat literal"  */
#line 222 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(LiteralStandardValue, BSQON_AST_TAG_NatValue, MK_SPOS_S((yylsp[0])), (yyvsp[0].str)); }
#line 2240 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 42: /* bsqonliteral: "int literal"  */
#line 223 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(LiteralStandardValue, BSQON_AST_TAG_IntValue, MK_SPOS_S((yylsp[0])), (yyvsp[0].str)); }
#line 2246 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 43: /* bsqonliteral: "big nat literal"  */
#line 224 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(LiteralStandardValue, BSQON_AST_TAG_BigNatValue, MK_SPOS_S((yylsp[0])), (yyvsp[0].str)); }
#line 2252 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 44: /* bsqonliteral: "big int literal"  */
#line 225 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(LiteralStandardValue, BSQON_AST_TAG_BigIntValue, MK_SPOS_S((yylsp[0])), (yyvsp[0].str)); }
#line 2258 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 45: /* bsqonliteral: "rational literal"  */
#line 226 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(LiteralStandardValue, BSQON_AST_TAG_RationalValue, MK_SPOS_S((yylsp[0])), (yyvsp[0].str)); }
#line 2264 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 46: /* bsqonliteral: "float literal"  */
#line 227 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(LiteralStandardValue, BSQON_AST_TAG_FloatValue, MK_SPOS_S((yylsp[0])), (yyvsp[0].str)); }
#line 2270 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 47: /* bsqonliteral: "decimal literal"  */
#line 228 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(LiteralStandardValue, BSQON_AST_TAG_DecimalValue, MK_SPOS_S((yylsp[0])), (yyvsp[0].str)); }
#line 2276 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 48: /* bsqonliteral: "decimal degree literal"  */
#line 229 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(LiteralStandardValue, BSQON_AST_TAG_DecimalDegreeValue, MK_SPOS_S((yylsp[0])), (yyvsp[0].str)); }
#line 2282 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 49: /* bsqonliteral: "complex literal"  */
#line 230 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(LiteralStandardValue, BSQON_AST_TAG_ComplexValue, MK_SPOS_S((yylsp[0])), (yyvsp[0].str)); }
#line 2288 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 50: /* bsqonliteral: "geo coordinate literal"  */
#line 231 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(LiteralStandardValue, BSQON_AST_TAG_LatLongValue, MK_SPOS_S((yylsp[0])), (yyvsp[0].str)); }
#line 2294 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 51: /* bsqonliteral: "byte buffer"  */
#line 232 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(LiteralStandardValue, BSQON_AST_TAG_ByteBufferValue, MK_SPOS_S((yylsp[0])), (yyvsp[0].str)); }
#line 2300 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 52: /* bsqonliteral: "uuid (v4)"  */
#line 233 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(LiteralStandardValue, BSQON_AST_TAG_UUIDv4Value, MK_SPOS_S((yylsp[0])), (yyvsp[0].str)); }
#line 2306 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 53: /* bsqonliteral: "uuid (v7)"  */
#line 234 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(LiteralStandardValue, BSQON_AST_TAG_UUIDv7Value, MK_SPOS_S((yylsp[0])), (yyvsp[0].str)); }
#line 2312 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 54: /* bsqonliteral: "sha3 hashcode (512 bits)"  */
#line 235 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(LiteralStandardValue, BSQON_AST_TAG_SHAHashcodeValue, MK_SPOS_S((yylsp[0])), (yyvsp[0].str)); }
#line 2318 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 55: /* bsqonliteral: "string"  */
#line 236 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(LiteralStringValue, BSQON_AST_TAG_StringValue, MK_SPOS_S((yylsp[0])), (yyvsp[0].bstr)); }
#line 2324 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 56: /* bsqonliteral: "ascii string"  */
#line 237 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(LiteralStringValue, BSQON_AST_TAG_ASCIIStringValue, MK_SPOS_S((yylsp[0])), (yyvsp[0].bstr)); }
#line 2330 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 57: /* bsqonliteral: "path item"  */
#line 238 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(LiteralStringValue, BSQON_AST_TAG_NakedPathValue, MK_SPOS_S((yylsp[0])), (yyvsp[0].bstr)); }
#line 2336 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 58: /* bsqonliteral: "regular expression"  */
#line 239 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(LiteralStringValue, BSQON_AST_TAG_RegexValue, MK_SPOS_S((yylsp[0])), (yyvsp[0].bstr)); }
#line 2342 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 59: /* bsqonliteral: "date & time with timezone"  */
#line 240 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(LiteralStandardValue, BSQON_AST_TAG_DateTimeValue, MK_SPOS_S((yylsp[0])), (yyvsp[0].str)); }
#line 2348 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 60: /* bsqonliteral: "date & time in UTC"  */
#line 241 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(LiteralStandardValue, BSQON_AST_TAG_UTCDateTimeValue, MK_SPOS_S((yylsp[0])), (yyvsp[0].str)); }
#line 2354 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 61: /* bsqonliteral: "plain date"  */
#line 242 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(LiteralStandardValue, BSQON_AST_TAG_PlainDateValue, MK_SPOS_S((yylsp[0])), (yyvsp[0].str)); }
#line 2360 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 62: /* bsqonliteral: "plain time"  */
#line 243 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(LiteralStandardValue, BSQON_AST_TAG_PlainTimeValue, MK_SPOS_S((yylsp[0])), (yyvsp[0].str)); }
#line 2366 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 63: /* bsqonliteral: "logical time"  */
#line 244 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(LiteralStandardValue, BSQON_AST_TAG_LogicalTimeValue, MK_SPOS_S((yylsp[0])), (yyvsp[0].str)); }
#line 2372 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 64: /* bsqonliteral: "tick time"  */
#line 245 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(LiteralStandardValue, BSQON_AST_TAG_TickTimeValue, MK_SPOS_S((yylsp[0])), (yyvsp[0].str)); }
#line 2378 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 65: /* bsqonliteral: "ISO timestamp"  */
#line 246 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(LiteralStandardValue, BSQON_AST_TAG_TimestampValue, MK_SPOS_S((yylsp[0])), (yyvsp[0].str)); }
#line 2384 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 66: /* bsqonliteral: "date & time delta"  */
#line 247 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(LiteralStandardValue, BSQON_AST_TAG_DeltaDateTimeValue, MK_SPOS_S((yylsp[0])), (yyvsp[0].str)); }
#line 2390 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 67: /* bsqonliteral: "plain date delta"  */
#line 248 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(LiteralStandardValue, BSQON_AST_TAG_DeltaPlainDateValue, MK_SPOS_S((yylsp[0])), (yyvsp[0].str)); }
#line 2396 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 68: /* bsqonliteral: "plain time delta"  */
#line 249 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(LiteralStandardValue, BSQON_AST_TAG_DeltaPlainTimeValue, MK_SPOS_S((yylsp[0])), (yyvsp[0].str)); }
#line 2402 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 69: /* bsqonliteral: "timestamp delta"  */
#line 250 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(LiteralStandardValue, BSQON_AST_TAG_DeltaISOTimeStampValue, MK_SPOS_S((yylsp[0])), (yyvsp[0].str)); }
#line 2408 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 70: /* bsqonliteral: "delta in seconds"  */
#line 251 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(LiteralStandardValue, BSQON_AST_TAG_DeltaSecondsValue, MK_SPOS_S((yylsp[0])), (yyvsp[0].str)); }
#line 2414 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 71: /* bsqonliteral: "tick delta"  */
#line 252 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(LiteralStandardValue, BSQON_AST_TAG_DeltaTickValue, MK_SPOS_S((yylsp[0])), (yyvsp[0].str)); }
#line 2420 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 72: /* bsqonliteral: "logical time delta"  */
#line 253 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(LiteralStandardValue, BSQON_AST_TAG_DeltaLogicalValue, MK_SPOS_S((yylsp[0])), (yyvsp[0].str)); }
#line 2426 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 73: /* bsqonunspecvar: "unspec identifier"  */
#line 257 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                           { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(NameValue, BSQON_AST_TAG_UnspecIdentifierValue, MK_SPOS_S((yylsp[0])), (yyvsp[0].str)); }
#line 2432 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 74: /* bsqonidentifier: "$src"  */
#line 261 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(NameValue, BSQON_AST_TAG_IdentifierValue, MK_SPOS_S((yylsp[0])), "$src"); }
#line 2438 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 75: /* bsqonidentifier: "$npos"  */
#line 262 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                 { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(NameValue, BSQON_AST_TAG_IdentifierValue, MK_SPOS_S((yylsp[0])), "$npos"); }
#line 2444 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 76: /* bsqonidentifier: "$i"  */
#line 263 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                  { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(NameValue, BSQON_AST_TAG_IdentifierValue, MK_SPOS_S((yylsp[0])), "$i"); }
#line 2450 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 77: /* bsqonidentifier: "$key"  */
#line 264 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(NameValue, BSQON_AST_TAG_IdentifierValue, MK_SPOS_S((yylsp[0])), "$key"); }
#line 2456 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 78: /* bsqonidentifier: "identifier"  */
#line 265 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                      { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(NameValue, BSQON_AST_TAG_IdentifierValue, MK_SPOS_S((yylsp[0])), (yyvsp[0].str)); }
#line 2462 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 79: /* bsqonscopedidentifier: bsqonnominaltype "::" "identifier"  */
#line 269 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                                      { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(ScopedNameValue, BSQON_AST_TAG_ScopedNameValue, MK_SPOS_R((yylsp[-2]), (yylsp[0])), (yyvsp[-2].bsqon_type_node), (yyvsp[0].str)); }
#line 2468 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 80: /* bsqonstringof: "string" bsqonnominaltype  */
#line 273 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                 { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(StringOfValue, BSQON_AST_TAG_StringOfValue, MK_SPOS_R((yylsp[-1]), (yylsp[0])), (yyvsp[-1].bstr), (yyvsp[0].bsqon_type_node)); }
#line 2474 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 81: /* bsqonstringof: "ascii string" bsqonnominaltype  */
#line 274 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                         { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(StringOfValue, BSQON_AST_TAG_ASCIIStringOfValue, MK_SPOS_R((yylsp[-1]), (yylsp[0])), (yyvsp[-1].bstr), (yyvsp[0].bsqon_type_node)); }
#line 2480 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 82: /* bsqonpath: "path item" bsqonnominaltype  */
#line 278 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                    { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(PathValue, BSQON_AST_TAG_PathValue, MK_SPOS_R((yylsp[-1]), (yylsp[0])), BSQON_AST_NODE_CONS(LiteralStringValue, BSQON_AST_TAG_NakedPathValue, MK_SPOS_S((yylsp[-1])), (yyvsp[-1].bstr)), (yyvsp[0].bsqon_type_node)); }
#line 2486 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 83: /* bsqontypeliteral: bsqonliteral "_" bsqonnominaltype  */
#line 282 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                                { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(TypedLiteralValue, BSQON_AST_TAG_TypedLiteralValue, MK_SPOS_R((yylsp[-2]), (yylsp[0])), (yyvsp[-2].bsqon_value_node), (yyvsp[0].bsqon_type_node)); }
#line 2492 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 84: /* bsqonenvaccess: "env" '[' "ascii string" ']'  */
#line 286 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                     { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(EnvAccessValue, BSQON_AST_TAG_EnvAccessValue, MK_SPOS_R((yylsp[-3]), (yylsp[0])), (yyvsp[-1].bstr), NULL); }
#line 2498 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 85: /* bsqonenvaccess: "env" '<' bsqontype '>' '[' "ascii string" ']'  */
#line 287 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                                         { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(EnvAccessValue, BSQON_AST_TAG_EnvAccessValue, MK_SPOS_R((yylsp[-6]), (yylsp[0])), (yyvsp[-1].bstr), (yyvsp[-4].bsqon_type_node)); }
#line 2504 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 93: /* bsqonterminal: bsqonenvaccess  */
#line 291 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                                                                                                                           { (yyval.bsqon_value_node) = (yyvsp[0].bsqon_value_node); }
#line 2510 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 94: /* bsqon_mapentry: bsqonval "=>" bsqonval  */
#line 295 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                               { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(MapEntryValue, BSQON_AST_TAG_MapEntryValue, MK_SPOS_R((yylsp[-2]), (yylsp[0])), (yyvsp[-2].bsqon_value_node), (yyvsp[0].bsqon_value_node)); }
#line 2516 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 95: /* bsqon_mapentry: error "=>" bsqonval  */
#line 296 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(MapEntryValue, BSQON_AST_TAG_MapEntryValue, MK_SPOS_R((yylsp[-2]), (yylsp[0])), BSQON_AST_ERROR(MK_SPOS_S((yylsp[-2]))), (yyvsp[0].bsqon_value_node)); yyerrok; }
#line 2522 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 96: /* bsqon_mapentry: bsqonval "=>" error  */
#line 297 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(MapEntryValue, BSQON_AST_TAG_MapEntryValue, MK_SPOS_R((yylsp[-2]), (yylsp[0])), (yyvsp[-2].bsqon_value_node), BSQON_AST_ERROR(MK_SPOS_S((yylsp[0])))); yyerrok; }
#line 2528 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 97: /* bsqon_mapentry: error "=>" error  */
#line 298 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                           { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(MapEntryValue, BSQON_AST_TAG_MapEntryValue, MK_SPOS_R((yylsp[-2]), (yylsp[0])), BSQON_AST_ERROR(MK_SPOS_S((yylsp[-2]))), BSQON_AST_ERROR(MK_SPOS_S((yylsp[0])))); yyerrok; }
#line 2534 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 98: /* bsqonvall: bsqonvall bsqonl_entry  */
#line 302 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                          { (yyval.bsqon_value_list) = BSQON_AST_LIST_OF_VALUES_Push((yyvsp[0].bsqon_value_node), (yyvsp[-1].bsqon_value_list)); }
#line 2540 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 99: /* bsqonvall: bsqonl_entry  */
#line 303 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                  { (yyval.bsqon_value_list) = BSQON_AST_LIST_OF_VALUES_Singleton((yyvsp[0].bsqon_value_node)); }
#line 2546 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 100: /* bsqonl_entry: bsqon_braceval ","  */
#line 307 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                            { (yyval.bsqon_value_node) = (yyvsp[-1].bsqon_value_node); }
#line 2552 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 101: /* bsqonl_entry: error ","  */
#line 308 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                     { (yyval.bsqon_value_node) = BSQON_AST_ERROR(MK_SPOS_S((yylsp[-1]))); yyerrok; }
#line 2558 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 102: /* bsqonbracketvalue: '[' ']'  */
#line 312 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
           { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(BracketValue, BSQON_AST_TAG_BracketValue, MK_SPOS_R((yylsp[-1]), (yylsp[0])), NULL); }
#line 2564 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 103: /* bsqonbracketvalue: '[' bsqonval ']'  */
#line 313 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                      { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(BracketValue, BSQON_AST_TAG_BracketValue, MK_SPOS_R((yylsp[-2]), (yylsp[0])), BSQON_AST_LIST_OF_VALUES_Singleton((yyvsp[-1].bsqon_value_node))); }
#line 2570 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 104: /* bsqonbracketvalue: '[' bsqonvall bsqonval ']'  */
#line 314 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(BracketValue, BSQON_AST_TAG_BracketValue, MK_SPOS_R((yylsp[-3]), (yylsp[0])), BSQON_AST_LIST_OF_VALUES_Reverse(BSQON_AST_LIST_OF_VALUES_Push((yyvsp[-1].bsqon_value_node), (yyvsp[-2].bsqon_value_list)))); }
#line 2576 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 105: /* bsqonbracketvalue: '[' error ']'  */
#line 315 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                   { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(BracketValue, BSQON_AST_TAG_BracketValue, MK_SPOS_R((yylsp[-2]), (yylsp[0])), BSQON_AST_LIST_OF_VALUES_Singleton(BSQON_AST_ERROR(MK_SPOS_S((yylsp[-1]))))); yyerrok; }
#line 2582 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 106: /* bsqonbracketvalue: '[' bsqonvall error ']'  */
#line 316 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                             { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(BracketValue, BSQON_AST_TAG_BracketValue, MK_SPOS_R((yylsp[-3]), (yylsp[0])), BSQON_AST_LIST_OF_VALUES_Reverse(BSQON_AST_LIST_OF_VALUES_Push(BSQON_AST_ERROR(MK_SPOS_S((yylsp[-1]))), (yyvsp[-2].bsqon_value_list)))); yyerrok; }
#line 2588 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 107: /* bsqonnamevall: bsqonnamevall bsqonnameval_entry  */
#line 320 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                    { (yyval.bsqon_named_value_list) = BSQON_AST_NLIST_OF_VALUES_Push((yyvsp[0].bsqon_named_value_list_entry), (yyvsp[-1].bsqon_named_value_list)); }
#line 2594 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 108: /* bsqonnamevall: bsqonnameval_entry  */
#line 321 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                        { (yyval.bsqon_named_value_list) = BSQON_AST_NLIST_OF_VALUES_Singleton((yyvsp[0].bsqon_named_value_list_entry)); }
#line 2600 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 110: /* bsqon_braceval: bsqon_mapentry  */
#line 325 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                             { (yyval.bsqon_value_node) = (yyvsp[0].bsqon_value_node); }
#line 2606 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 111: /* bsqonnameval_entry: "identifier" "=" bsqonval ","  */
#line 329 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                                  { (yyval.bsqon_named_value_list_entry) = BSQON_AST_NLIST_OF_VALUES_ENTRY_Create((yyvsp[-3].str), (yyvsp[-1].bsqon_value_node)); }
#line 2612 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 112: /* bsqonnameval_entry: "identifier" "=" error ","  */
#line 330 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                                 { (yyval.bsqon_named_value_list_entry) = BSQON_AST_NLIST_OF_VALUES_ENTRY_Create((yyvsp[-3].str), BSQON_AST_ERROR(MK_SPOS_S((yylsp[-1])))); yyerrok; }
#line 2618 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 113: /* bsqonnameval_entry: bsqon_braceval ","  */
#line 331 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                              { (yyval.bsqon_named_value_list_entry) = BSQON_AST_NLIST_OF_VALUES_ENTRY_Create(NULL, (yyvsp[-1].bsqon_value_node)); }
#line 2624 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 114: /* bsqonnameval_entry: error ","  */
#line 332 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                     { (yyval.bsqon_named_value_list_entry) = BSQON_AST_NLIST_OF_VALUES_ENTRY_Create(NULL, BSQON_AST_ERROR(MK_SPOS_S((yylsp[-1])))); yyerrok; }
#line 2630 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 115: /* bsqonbracevalue: '{' '}'  */
#line 336 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
           { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(BraceValue, BSQON_AST_TAG_BraceValue, MK_SPOS_R((yylsp[-1]), (yylsp[0])), NULL); }
#line 2636 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 116: /* bsqonbracevalue: '{' "identifier" "=" bsqonval '}'  */
#line 337 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                                  { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(BraceValue, BSQON_AST_TAG_BraceValue, MK_SPOS_R((yylsp[-4]), (yylsp[0])), BSQON_AST_NLIST_OF_VALUES_Singleton(BSQON_AST_NLIST_OF_VALUES_ENTRY_Create((yyvsp[-3].str), (yyvsp[-1].bsqon_value_node)))); }
#line 2642 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 117: /* bsqonbracevalue: '{' bsqonnamevall "identifier" "=" bsqonval '}'  */
#line 338 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                                                { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(BraceValue, BSQON_AST_TAG_BraceValue, MK_SPOS_R((yylsp[-5]), (yylsp[0])), BSQON_AST_NLIST_OF_VALUES_Reverse(BSQON_AST_NLIST_OF_VALUES_Push(BSQON_AST_NLIST_OF_VALUES_ENTRY_Create((yyvsp[-3].str), (yyvsp[-1].bsqon_value_node)), (yyvsp[-4].bsqon_named_value_list)))); }
#line 2648 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 118: /* bsqonbracevalue: '{' "identifier" "=" error '}'  */
#line 339 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                               { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(BraceValue, BSQON_AST_TAG_BraceValue, MK_SPOS_R((yylsp[-4]), (yylsp[0])), BSQON_AST_NLIST_OF_VALUES_Singleton(BSQON_AST_NLIST_OF_VALUES_ENTRY_Create((yyvsp[-3].str), BSQON_AST_ERROR(MK_SPOS_S((yylsp[-1])))))); yyerrok; }
#line 2654 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 119: /* bsqonbracevalue: '{' bsqonnamevall "identifier" "=" error '}'  */
#line 340 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                                             { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(BraceValue, BSQON_AST_TAG_BraceValue, MK_SPOS_R((yylsp[-5]), (yylsp[0])), BSQON_AST_NLIST_OF_VALUES_Reverse(BSQON_AST_NLIST_OF_VALUES_Push(BSQON_AST_NLIST_OF_VALUES_ENTRY_Create((yyvsp[-3].str), BSQON_AST_ERROR(MK_SPOS_S((yylsp[-1])))), (yyvsp[-4].bsqon_named_value_list)))); yyerrok; }
#line 2660 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 120: /* bsqonbracevalue: '{' bsqon_braceval '}'  */
#line 341 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                            { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(BraceValue, BSQON_AST_TAG_BraceValue, MK_SPOS_R((yylsp[-2]), (yylsp[0])), BSQON_AST_NLIST_OF_VALUES_Singleton(BSQON_AST_NLIST_OF_VALUES_ENTRY_Create(NULL, (yyvsp[-1].bsqon_value_node)))); }
#line 2666 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 121: /* bsqonbracevalue: '{' bsqonnamevall bsqon_braceval '}'  */
#line 342 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                          { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(BraceValue, BSQON_AST_TAG_BraceValue, MK_SPOS_R((yylsp[-3]), (yylsp[0])), BSQON_AST_NLIST_OF_VALUES_Reverse(BSQON_AST_NLIST_OF_VALUES_Push(BSQON_AST_NLIST_OF_VALUES_ENTRY_Create(NULL, (yyvsp[-1].bsqon_value_node)), (yyvsp[-2].bsqon_named_value_list)))); }
#line 2672 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 122: /* bsqonbracevalue: '{' error '}'  */
#line 343 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                   { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(BraceValue, BSQON_AST_TAG_BraceValue, MK_SPOS_R((yylsp[-2]), (yylsp[0])), BSQON_AST_NLIST_OF_VALUES_Singleton(BSQON_AST_NLIST_OF_VALUES_ENTRY_Create(NULL, BSQON_AST_ERROR(MK_SPOS_S((yylsp[-1])))))); yyerrok; }
#line 2678 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 123: /* bsqonbracevalue: '{' bsqonnamevall error '}'  */
#line 344 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                 { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(BraceValue, BSQON_AST_TAG_BraceValue, MK_SPOS_R((yylsp[-3]), (yylsp[0])), BSQON_AST_NLIST_OF_VALUES_Reverse(BSQON_AST_NLIST_OF_VALUES_Push(BSQON_AST_NLIST_OF_VALUES_ENTRY_Create(NULL, BSQON_AST_ERROR(MK_SPOS_S((yylsp[-1])))), (yyvsp[-2].bsqon_named_value_list)))); yyerrok; }
#line 2684 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 125: /* bsqonbracketbracevalue: bsqonbracevalue  */
#line 348 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                       { (yyval.bsqon_value_node) = (yyvsp[0].bsqon_value_node); }
#line 2690 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 126: /* bsqontypedvalue: '<' bsqontspec '>' bsqonbracketbracevalue  */
#line 352 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                             { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(TypedValue, BSQON_AST_TAG_TypedValue, MK_SPOS_R((yylsp[-3]), (yylsp[0])), (yyvsp[0].bsqon_value_node), (yyvsp[-2].bsqon_type_node), true); }
#line 2696 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 127: /* bsqontypedvalue: bsqonnominaltype bsqonbracketbracevalue  */
#line 353 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                             { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(TypedValue, BSQON_AST_TAG_TypedValue, MK_SPOS_R((yylsp[-1]), (yylsp[0])), (yyvsp[0].bsqon_value_node), (yyvsp[-1].bsqon_type_node), false); }
#line 2702 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 128: /* bsqontypedvalue: '<' error '>' bsqonbracketbracevalue  */
#line 354 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                          { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(TypedValue, BSQON_AST_TAG_TypedValue, MK_SPOS_R((yylsp[-3]), (yylsp[0])), (yyvsp[0].bsqon_value_node), BSQON_AST_ERROR(MK_SPOS_S((yylsp[-2]))), true); }
#line 2708 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 129: /* bsqontypedvalue: error bsqonbracketbracevalue  */
#line 355 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                  { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(TypedValue, BSQON_AST_TAG_TypedValue, MK_SPOS_R((yylsp[-1]), (yylsp[0])), (yyvsp[0].bsqon_value_node), BSQON_AST_ERROR(MK_SPOS_S((yylsp[-1]))), false); }
#line 2714 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 131: /* bsqonstructvalue: bsqontypedvalue  */
#line 359 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                            { (yyval.bsqon_value_node) = (yyvsp[0].bsqon_value_node); }
#line 2720 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 132: /* bsqonspecialcons: "something" '(' bsqonval ')'  */
#line 363 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                 { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(SpecialConsValue, BSQON_AST_TAG_SomethingConsValue, MK_SPOS_R((yylsp[-3]), (yylsp[0])), (yyvsp[-1].bsqon_value_node), "some"); }
#line 2726 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 133: /* bsqonspecialcons: "something" '(' error ')'  */
#line 364 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(SpecialConsValue, BSQON_AST_TAG_SomethingConsValue, MK_SPOS_R((yylsp[-3]), (yylsp[0])), BSQON_AST_ERROR(MK_SPOS_S((yylsp[-1]))), "some"); yyerrok; }
#line 2732 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 134: /* bsqonspecialcons: "ok" '(' bsqonval ')'  */
#line 365 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                            { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(SpecialConsValue, BSQON_AST_TAG_OkConsValue, MK_SPOS_R((yylsp[-3]), (yylsp[0])), (yyvsp[-1].bsqon_value_node), "ok"); }
#line 2738 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 135: /* bsqonspecialcons: "ok" '(' error ')'  */
#line 366 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                         { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(SpecialConsValue, BSQON_AST_TAG_OkConsValue, MK_SPOS_R((yylsp[-3]), (yylsp[0])), BSQON_AST_ERROR(MK_SPOS_S((yylsp[-1]))), "ok"); yyerrok; }
#line 2744 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 136: /* bsqonspecialcons: "err" '(' bsqonval ')'  */
#line 367 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                             { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(SpecialConsValue, BSQON_AST_TAG_ErrConsValue, MK_SPOS_R((yylsp[-3]), (yylsp[0])), (yyvsp[-1].bsqon_value_node), "err"); }
#line 2750 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 137: /* bsqonspecialcons: "err" '(' error ')'  */
#line 368 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                          { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(SpecialConsValue, BSQON_AST_TAG_ErrConsValue, MK_SPOS_R((yylsp[-3]), (yylsp[0])), BSQON_AST_ERROR(MK_SPOS_S((yylsp[-1]))), "err"); yyerrok; }
#line 2756 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 142: /* bsqonval: bsqonaccess  */
#line 372 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                                                                  { (yyval.bsqon_value_node) = (yyvsp[0].bsqon_value_node); }
#line 2762 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 143: /* bsqonletexp: '(' "let" "identifier" ":" bsqontype "=" bsqonval "in" bsqonval ')'  */
#line 376 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                                                                         { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(LetInValue, BSQON_AST_TAG_LetInValue, MK_SPOS_R((yylsp[-9]), (yylsp[0])), (yyvsp[-7].str), (yyvsp[-5].bsqon_type_node), (yyvsp[-3].bsqon_value_node), (yyvsp[-1].bsqon_value_node)); }
#line 2768 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 147: /* bsqonref: bsqonaccess  */
#line 380 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                                                        { (yyval.bsqon_value_node) = (yyvsp[0].bsqon_value_node); }
#line 2774 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 150: /* bsqonidx: bsqonscopedidentifier  */
#line 384 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                                          { (yyval.bsqon_value_node) = (yyvsp[0].bsqon_value_node); }
#line 2780 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 151: /* bsqonaccess: bsqonref "." "identifier"  */
#line 388 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                     { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(AccessNameValue, BSQON_AST_TAG_AccessNameValue, MK_SPOS_R((yylsp[-2]), (yylsp[0])), (yyvsp[-2].bsqon_value_node), (yyvsp[0].str)); }
#line 2786 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 152: /* bsqonaccess: bsqonref "." "numberino"  */
#line 389 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                      { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(AccessIndexValue, BSQON_AST_TAG_AccessIndexValue, MK_SPOS_R((yylsp[-2]), (yylsp[0])), (yyvsp[-2].bsqon_value_node), (yyvsp[0].str)); }
#line 2792 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 153: /* bsqonaccess: bsqonref '[' bsqonterminal ']'  */
#line 390 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                    { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(AccessKeyValue, BSQON_AST_TAG_AccessKeyValue, MK_SPOS_R((yylsp[-3]), (yylsp[0])), (yyvsp[-3].bsqon_value_node), (yyvsp[-1].bsqon_value_node)); }
#line 2798 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 154: /* bsqonaccess: bsqonref '[' bsqonidx ":" ']'  */
#line 391 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                          { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(StringSliceValue, BSQON_AST_TAG_StringSliceValue, MK_SPOS_R((yylsp[-4]), (yylsp[0])), (yyvsp[-4].bsqon_value_node), (yyvsp[-2].bsqon_value_node), NULL); }
#line 2804 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 155: /* bsqonaccess: bsqonref '[' ":" bsqonidx ']'  */
#line 392 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                          { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(StringSliceValue, BSQON_AST_TAG_StringSliceValue, MK_SPOS_R((yylsp[-4]), (yylsp[0])), (yyvsp[-4].bsqon_value_node), NULL, (yyvsp[-1].bsqon_value_node)); }
#line 2810 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 156: /* bsqonaccess: bsqonref '[' bsqonidx ":" bsqonidx ']'  */
#line 393 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                                  { (yyval.bsqon_value_node) = BSQON_AST_NODE_CONS(StringSliceValue, BSQON_AST_TAG_StringSliceValue, MK_SPOS_R((yylsp[-5]), (yylsp[0])), (yyvsp[-5].bsqon_value_node), (yyvsp[-3].bsqon_value_node), (yyvsp[-1].bsqon_value_node)); }
#line 2816 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 157: /* bsqonenvlist: "env" '{' '}'  */
#line 397 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                  { (yyval.bsqon_named_type_list) = NULL; }
#line 2822 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 158: /* bsqonenvlist: "env" '{' "identifier" ":" bsqontype '}'  */
#line 398 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                                         { (yyval.bsqon_named_type_list) = BSQON_AST_NLIST_OF_TYPES_Singleton(BSQON_AST_NLIST_OF_TYPES_ENTRY_Create((yyvsp[-3].str), (yyvsp[-1].bsqon_type_node))); }
#line 2828 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 159: /* bsqonenvlist: "env" '{' bsqonnametypel "identifier" ":" bsqontype '}'  */
#line 399 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                                                        { (yyval.bsqon_named_type_list) = BSQON_AST_NLIST_OF_TYPES_Reverse(BSQON_AST_NLIST_OF_TYPES_Push(BSQON_AST_NLIST_OF_TYPES_ENTRY_Create((yyvsp[-3].str), (yyvsp[-1].bsqon_type_node)), (yyvsp[-4].bsqon_named_type_list))); }
#line 2834 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 160: /* bsqonenvlist: "env" '{' "identifier" ":" error '}'  */
#line 400 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                                     { (yyval.bsqon_named_type_list) = BSQON_AST_NLIST_OF_TYPES_Singleton(BSQON_AST_NLIST_OF_TYPES_ENTRY_Create((yyvsp[-3].str), BSQON_AST_ERROR(MK_SPOS_S((yylsp[-1]))))); yyerrok; }
#line 2840 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 161: /* bsqonenvlist: "env" '{' bsqonnametypel "identifier" ":" error '}'  */
#line 401 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                                                    { (yyval.bsqon_named_type_list) = BSQON_AST_NLIST_OF_TYPES_Reverse(BSQON_AST_NLIST_OF_TYPES_Push(BSQON_AST_NLIST_OF_TYPES_ENTRY_Create((yyvsp[-3].str), BSQON_AST_ERROR(MK_SPOS_S((yylsp[-3])))), (yyvsp[-4].bsqon_named_type_list))); yyerrok; }
#line 2846 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 162: /* bsqoncomponent: bsqonval  */
#line 405 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
            { (yyval.bsqon_decl) = BSQON_AST_NODE_CONS(BsqonDeclBody, BSQON_AST_TAG_BsqonDeclBody, MK_SPOS_S((yylsp[0])), NULL, NULL, (yyvsp[0].bsqon_value_node)); }
#line 2852 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 163: /* bsqoncomponent: bsqonenvlist bsqonval  */
#line 406 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                           { (yyval.bsqon_decl) = BSQON_AST_NODE_CONS(BsqonDeclBody, BSQON_AST_TAG_BsqonDeclBody, MK_SPOS_R((yylsp[-1]), (yylsp[0])), NULL, (yyvsp[-1].bsqon_named_type_list), (yyvsp[0].bsqon_value_node)); }
#line 2858 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 164: /* bsqoncomponent: "shebang line" bsqonval  */
#line 407 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                 { (yyval.bsqon_decl) = BSQON_AST_NODE_CONS(BsqonDeclBody, BSQON_AST_TAG_BsqonDeclBody, MK_SPOS_R((yylsp[-1]), (yylsp[0])), (yyvsp[-1].str), NULL, (yyvsp[0].bsqon_value_node)); }
#line 2864 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 165: /* bsqoncomponent: "shebang line" bsqonenvlist bsqonval  */
#line 408 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                                              { (yyval.bsqon_decl) = BSQON_AST_NODE_CONS(BsqonDeclBody, BSQON_AST_TAG_BsqonDeclBody, MK_SPOS_R((yylsp[-2]), (yylsp[0])), (yyvsp[-2].str), (yyvsp[-1].bsqon_named_type_list), (yyvsp[0].bsqon_value_node)); }
#line 2870 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 166: /* bsqonroot: bsqoncomponent  */
#line 412 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
                  { yybsqonval = (yyvsp[0].bsqon_decl); (yyval.bsqon_decl) = (yyvsp[0].bsqon_decl); }
#line 2876 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;

  case 167: /* bsqonroot: error  */
#line 413 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"
           {yybsqonval = BSQON_AST_ERROR(MK_SPOS_S((yylsp[0]))); (yyval.bsqon_decl) = BSQON_AST_ERROR(MK_SPOS_S((yylsp[0]))); }
#line 2882 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"
    break;


#line 2886 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.c"

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

#line 415 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"


extern FILE* yyin;

size_t isSpecialTypedLiteralIdConflict(const char* txt)
{
   size_t tlen = strlen(txt);
   if(strncmp("none_", txt, 5) == 0  && tlen >= 6 && isupper(txt[5])) {
      return tlen - 4;
   }
   else if(strncmp("true_", txt, 5) == 0  && tlen >= 6 && isupper(txt[5])) {
      return tlen - 4;
   }
   else if(strncmp("false_", txt, 6) == 0  && tlen >= 7 && isupper(txt[6])) {
      return tlen - 5;
   }
   else if(strncmp("nothing_", txt, 8) == 0  && tlen >= 9 && isupper(txt[8])) {
      return tlen - 7;
   }
   else {
      return 0;
   }
}

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
