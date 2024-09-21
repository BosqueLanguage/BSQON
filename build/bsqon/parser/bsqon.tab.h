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
    SYM_ENTRY = 272,               /* "=>"  */
    SYM_COLON = 273,               /* ":"  */
    SYM_COMMA = 274,               /* ","  */
    SYM_EQUALS = 275,              /* "="  */
    SYM_DOT = 276,                 /* "."  */
    SYM_ELIST_LEFT = 277,          /* "(|"  */
    SYM_ELIST_RIGHT = 278,         /* "|)"  */
    SYM_LBRACK_BAR = 279,          /* "[|"  */
    SYM_RBRACK_BAR = 280,          /* "|]"  */
    TOKEN_NAT = 281,               /* "nat literal"  */
    TOKEN_INT = 282,               /* "int literal"  */
    TOKEN_BIG_NAT = 283,           /* "big nat literal"  */
    TOKEN_BIG_INT = 284,           /* "big int literal"  */
    TOKEN_RATIONAL = 285,          /* "rational literal"  */
    TOKEN_FLOAT = 286,             /* "float literal"  */
    TOKEN_DECIMAL = 287,           /* "decimal literal"  */
    TOKEN_DECIMAL_DEGREE = 288,    /* "decimal degree literal"  */
    TOKEN_COMPLEX = 289,           /* "complex literal"  */
    TOKEN_LAT_LONG = 290,          /* "geo coordinate literal"  */
    TOKEN_NUMBERINO = 291,         /* "numberino"  */
    TOKEN_BYTE_BUFFER = 292,       /* "byte buffer"  */
    TOKEN_UUID_V4 = 293,           /* "uuid (v4)"  */
    TOKEN_UUID_V7 = 294,           /* "uuid (v7)"  */
    TOKEN_SHA_HASH = 295,          /* "sha3 hashcode (512 bits)"  */
    TOKEN_STRING = 296,            /* "string"  */
    TOKEN_CSTRING = 297,           /* "cstring"  */
    TOKEN_REGEX = 298,             /* "regular expression"  */
    TOKEN_PATH_ITEM = 299,         /* "path item"  */
    TOKEN_TZ_DATE_TIME = 300,      /* "date & time with timezone"  */
    TOKEN_TIA_TIME = 301,          /* "date & time in TIA"  */
    TOKEN_PLAIN_DATE = 302,        /* "plain date"  */
    TOKEN_PLAIN_TIME = 303,        /* "plain time"  */
    TOKEN_LOGICAL_TIME = 304,      /* "logical time"  */
    TOKEN_TIMESTAMP = 305,         /* "ISO timestamp"  */
    TOKEN_DELTA_DATE_TIME = 306,   /* "date & time delta"  */
    TOKEN_DELTA_SECONDS = 307,     /* "delta in seconds"  */
    TOKEN_DELTA_LOGICAL = 308,     /* "logical time delta"  */
    TOKEN_DELTA_ISOTIMESTAMP = 309, /* "timestamp delta"  */
    TOKEN_IDENTIFIER = 310,        /* "identifier"  */
    TOKEN_TYPE_COMPONENT = 311,    /* "type name"  */
    TOKEN_UNSPEC_IDENTIFIER = 312, /* "unspec identifier"  */
    TOKEN_SHEBANG_LINE = 313       /* "shebang line"  */
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

#line 148 "/home/mark/Code/BSQON/build/output/obj/bsqon.tab.h"

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
