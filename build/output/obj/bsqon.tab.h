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

#ifndef YY_YY_HOME_MARK_CODE_BSQON_BUILD_OUTPUT_OBJ_BSQON_TAB_H_INCLUDED
# define YY_YY_HOME_MARK_CODE_BSQON_BUILD_OUTPUT_OBJ_BSQON_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 27 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"

#include "../../../src/fbparser/bsqon_ast.h"

#line 53 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.h"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    KW_TRUE = 258,                 /* "true"  */
    KW_FALSE = 259,                /* "false"  */
    KW_NONE = 260,                 /* "none"  */
    KW_SOME = 261,                 /* "some"  */
    KW_OK = 262,                   /* "ok"  */
    KW_ERR = 263,                  /* "err"  */
    KW_NPOS = 264,                 /* "$npos"  */
    KW_INDEX = 265,                /* "$i"  */
    KW_KEY = 266,                  /* "$key"  */
    KW_SRC = 267,                  /* "$src"  */
    KW_LET = 268,                  /* "let"  */
    KW_IN = 269,                   /* "in"  */
    KW_ENV = 270,                  /* "env"  */
    SYM_DOUBLE_COLON = 271,        /* "::"  */
    SYM_HASH = 272,                /* "#"  */
    SYM_ENTRY = 273,               /* "=>"  */
    SYM_COLON = 274,               /* ":"  */
    SYM_COMMA = 275,               /* ","  */
    SYM_EQUALS = 276,              /* "="  */
    SYM_DOT = 277,                 /* "."  */
    SYM_ELIST_LEFT = 278,          /* "(|"  */
    SYM_ELIST_RIGHT = 279,         /* "|)"  */
    SYM_LBRACK_BAR = 280,          /* "[|"  */
    SYM_RBRACK_BAR = 281,          /* "|]"  */
    TOKEN_NAT = 282,               /* "nat literal"  */
    TOKEN_INT = 283,               /* "int literal"  */
    TOKEN_BIG_NAT = 284,           /* "big nat literal"  */
    TOKEN_BIG_INT = 285,           /* "big int literal"  */
    TOKEN_RATIONAL = 286,          /* "rational literal"  */
    TOKEN_FLOAT = 287,             /* "float literal"  */
    TOKEN_DECIMAL = 288,           /* "decimal literal"  */
    TOKEN_DECIMAL_DEGREE = 289,    /* "decimal degree literal"  */
    TOKEN_COMPLEX = 290,           /* "complex literal"  */
    TOKEN_LAT_LONG = 291,          /* "geo coordinate literal"  */
    TOKEN_NUMBERINO = 292,         /* "numberino"  */
    TOKEN_BYTE_BUFFER = 293,       /* "byte buffer"  */
    TOKEN_UUID_V4 = 294,           /* "uuid (v4)"  */
    TOKEN_UUID_V7 = 295,           /* "uuid (v7)"  */
    TOKEN_SHA_HASH = 296,          /* "sha3 hashcode (512 bits)"  */
    TOKEN_STRING = 297,            /* "string"  */
    TOKEN_CSTRING = 298,           /* "cstring"  */
    TOKEN_REGEX = 299,             /* "regular expression"  */
    TOKEN_PATH_ITEM = 300,         /* "path item"  */
    TOKEN_TZ_DATE_TIME = 301,      /* "date & time with timezone"  */
    TOKEN_TIA_TIME = 302,          /* "date & time in TIA"  */
    TOKEN_PLAIN_DATE = 303,        /* "plain date"  */
    TOKEN_PLAIN_TIME = 304,        /* "plain time"  */
    TOKEN_LOGICAL_TIME = 305,      /* "logical time"  */
    TOKEN_TIMESTAMP = 306,         /* "ISO timestamp"  */
    TOKEN_DELTA_DATE_TIME = 307,   /* "date & time delta"  */
    TOKEN_DELTA_SECONDS = 308,     /* "delta in seconds"  */
    TOKEN_DELTA_LOGICAL = 309,     /* "logical time delta"  */
    TOKEN_DELTA_ISOTIMESTAMP = 310, /* "timestamp delta"  */
    TOKEN_IDENTIFIER = 311,        /* "identifier"  */
    TOKEN_TYPE_COMPONENT = 312,    /* "type name"  */
    TOKEN_UNSPEC_IDENTIFIER = 313, /* "unspec identifier"  */
    TOKEN_SHEBANG_LINE = 314       /* "shebang line"  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 31 "/home/mark/Code/BSQON/build/../src/fbparser/bsqon.y"

   char* str;
   struct ByteString* bstr;

   struct BSQON_AST_Node* bsqon_type_node;
   struct BSQON_AST_Node* bsqon_value_node;

   struct BSQON_AST_Node* bsqon_decl;

   struct BSQON_AST_LIST_OF_TYPES* bsqon_type_list;

   struct BSQON_AST_NLIST_OF_TYPES_ENTRY bsqon_named_type_list_entry;
   struct BSQON_AST_NLIST_OF_TYPES* bsqon_named_type_list;

   struct BSQON_AST_LIST_OF_VALUES* bsqon_value_list;

   struct BSQON_AST_NLIST_OF_VALUES_ENTRY bsqon_named_value_list_entry;
   struct BSQON_AST_NLIST_OF_VALUES* bsqon_named_value_list;

#line 149 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;

int yyparse (void);


#endif /* !YY_YY_HOME_MARK_CODE_BSQON_BUILD_OUTPUT_OBJ_BSQON_TAB_H_INCLUDED  */
