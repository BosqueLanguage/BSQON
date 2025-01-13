%option noyywrap nodefault nounput noinput yylineno

%{
#include "bsqon.tab.h"

#include "../../../src/fbparser/bytestring.h"

#define MAX_STR_CONST 4096

int yycolumn = 1;

uint8_t string_buf[MAX_STR_CONST];
struct ByteString string_buf_bs = { string_buf, 0 };

/* Forward declarations */
void ByteStringStoreChar(char c);
void ByteStringStoreText(const char* c, size_t len);

size_t isSpecialTypedLiteralIdConflict(const char* txt);

void yyerror(const char* s, ...);

/* This is executed before every action. */
#define YY_USER_ACTION                                                   \
  start_line = prev_yylineno; start_column = yycolumn;                   \
  if (yylineno == prev_yylineno) yycolumn += yyleng;                     \
  else {                                                                 \
    for (yycolumn = 1; yytext[yyleng - yycolumn] != '\n'; ++yycolumn) {} \
    prev_yylineno = yylineno;                                            \
  }                                                                      \
  yylloc.first_line = start_line; yylloc.first_column = start_column;    \
  yylloc.last_line = yylineno; yylloc.last_column = yycolumn;              

%}

%x STRING
%x CSTRING
%x REGEX
%x PATH_ITEM
%x SHEBANG_LINE

NON_ZERO_INTEGRAL ([1-9][0-9]*)
NORMAL_FLOAT [+-]?(0|{NON_ZERO_INTEGRAL})"."[0-9]+
EXP	([Ee][-+]?[0-9]+)

DATE ([0-9]{4})-([0-9]{2})-([0-9]{2})
TIME ([0-9]{2}):([0-9]{2}):([0-9]{2})
TZNAME ((\{[a-zA-Z0-9/, _-]+\})|[A-Z]+)

UUID ([a-fA-F0-9]{8}-[a-fA-F0-9]{4}-[a-fA-F0-9]{4}-[a-fA-F0-9]{4}-[a-fA-F0-9]{12})

IDENTIFIER ([_]?[a-z][a-zA-Z0-9_]*)
TYPE_IDENTIFIER ([A-Z][a-zA-Z0-9_]*)

%%

  int start_line, start_column;
  int prev_yylineno = yylineno;

 /* standard parens */
"(" |
")" |
"{" |
"}" |
"[" |
"]" |
"(|" |
"|)" |
"[|" |
"|]" | 
"<" |
">"         { return yytext[0]; }

 /* symbols */
"::"        { return SYM_DOUBLE_COLON; }
"#"         { return SYM_HASH; }
"=>"        { return SYM_ENTRY; }

":"         { return SYM_COLON; }
","         { return SYM_COMMA; }
"="         { return SYM_EQUALS; }
"."         { return SYM_DOT; }

 /* keywords */

"$npos"     { return KW_NPOS; }
"$src"      { return KW_SRC; }
"none"      { return KW_NONE; }
"true"      { return KW_TRUE; } 
"false"     { return KW_FALSE; }

"some"      { return KW_SOME; }
"ok"        { return KW_OK; }
"fail"       { return KW_ERR; }
"let"       { return KW_LET; }
"in"        { return KW_IN; }

"env"        { return KW_ENV; }

 /* numbers */
[-]0[niNIR]                                      { yyerror("Zero cannot be negative"); return YYerror; }
((0|[+-]?{NON_ZERO_INTEGRAL})\/0)R               { yyerror("Zero as divisor"); return YYerror; }

(0|[+]?{NON_ZERO_INTEGRAL})n   { yylval.str = AST_STRDUP(yytext); return TOKEN_NAT; }
(0|[+-]?{NON_ZERO_INTEGRAL})i  { yylval.str = AST_STRDUP(yytext); return TOKEN_INT; }
(0|[+]?{NON_ZERO_INTEGRAL})N   { yylval.str = AST_STRDUP(yytext); return TOKEN_BIG_NAT; }
(0|[+-]?{NON_ZERO_INTEGRAL})I  { yylval.str = AST_STRDUP(yytext); return TOKEN_BIG_INT; }

(0|[+-]?{NON_ZERO_INTEGRAL})R                        { yylval.str = AST_STRDUP(yytext); return TOKEN_RATIONAL; }
((0|[+-]?{NON_ZERO_INTEGRAL})\/{NON_ZERO_INTEGRAL})R { yylval.str = AST_STRDUP(yytext); return TOKEN_RATIONAL; }

[-]"0.0"{EXP}?(f|d|dd)                            { yyerror("Zero cannot be negative"); return YYerror; }

{NORMAL_FLOAT}{EXP}?f                   { yylval.str = AST_STRDUP(yytext); return TOKEN_FLOAT; }
{NORMAL_FLOAT}{EXP}?d                   { yylval.str = AST_STRDUP(yytext); return TOKEN_DECIMAL; }
{NORMAL_FLOAT}dd                        { yylval.str = AST_STRDUP(yytext); return TOKEN_DECIMAL_DEGREE; }


{NORMAL_FLOAT}"lat-0.0long"       { yyerror("Zero cannot be negative"); return YYerror; }
"-0.0lat"[+-]{NORMAL_FLOAT}long   { yyerror("Zero cannot be negative"); return YYerror; }

{NORMAL_FLOAT}"lat"[+-]{NORMAL_FLOAT}"long" { yylval.str = AST_STRDUP(yytext); return TOKEN_LAT_LONG; }

{NORMAL_FLOAT}{EXP}"-0.0"{EXP}?i { yyerror("Zero cannot be negative"); return YYerror; }
"-0.0"[+-]{NORMAL_FLOAT}{EXP}?i   { yyerror("Zero cannot be negative"); return YYerror; }

{NORMAL_FLOAT}{EXP}?[+-]{NORMAL_FLOAT}{EXP}?i { yylval.str = AST_STRDUP(yytext); return TOKEN_COMPLEX; }

 /* numberinos */
[+-](0|{NON_ZERO_INTEGRAL}) { yyerror("Cannot have sign on indexer"); return YYerror; }

0|[+-]?{NON_ZERO_INTEGRAL}       { yylval.str = AST_STRDUP(yytext); return TOKEN_NUMBERINO; }

 /* strings */
0x"["[a-fA-F0-9]*"]"    { yylval.str = AST_STRDUP(yytext); return TOKEN_BYTE_BUFFER; }
uuid4"{"{UUID}"}"       { yylval.str = AST_STRDUP(yytext); return TOKEN_UUID_V4; }
uuid7"{"{UUID}"}"       { yylval.str = AST_STRDUP(yytext); return TOKEN_UUID_V7; }
sha3"{"[a-f0-9]{64}"}"  { yylval.str = AST_STRDUP(yytext); return TOKEN_SHA_HASH; }

0x"["[^\]]*"]"        { yyerror("Invalid buffer contents"); return YYerror; }
uuid(4|7)"{"[^}]*"}" { yyerror("Invalid UUID value"); return YYerror; }
sha3"{"[^}]*"}"      { yyerror("Invalid SHA3 value"); return YYerror; }

0x"["[a-zA-Z0-9]*          { yyerror("Missing close bracket"); return YYerror; }
uuid(4|7)"{"[a-zA-Z0-9-]+  { yyerror("Missing close bracket"); return YYerror; }
sha3"{"[a-zA-Z0-9]+        { yyerror("Missing close bracket"); return YYerror; }

<INITIAL>{
\"            { BEGIN(STRING); string_buf_bs.len = 0; ByteStringStoreChar('"'); }
}
<STRING>{
\"            { BEGIN(INITIAL); ByteStringStoreChar('"'); yylval.bstr = bstrAlloc(string_buf_bs); return TOKEN_STRING; }
<<EOF>>       { BEGIN(INITIAL); yyerror("Unclosed String"); return YYEOF; }
\n[ \t\r]*\\  { ByteStringStoreChar('\n'); /*ignore whitespace till alignment on multi-line string*/ }
[^"]          { ByteStringStoreChar(yytext[0]); }
}

<INITIAL>{
'             { BEGIN(CSTRING); string_buf_bs.len = 0; ByteStringStoreChar('\''); }
}
<CSTRING>{
'             { BEGIN(INITIAL); ByteStringStoreChar('\''); yylval.bstr = bstrAlloc(string_buf_bs); return TOKEN_CSTRING; }
<<EOF>>       { BEGIN(INITIAL); yyerror("Unclosed ASCIIString"); return YYEOF; }
\n[ \t\r]*\\  { ByteStringStoreChar('\n'); /*ignore whitespace till alignment on multi-line string*/ }
[ -~\t\n]     { ByteStringStoreChar(yytext[0]); }
.             { yyerror("Invalid character (ignoring)"); return YYerror;}
}

<INITIAL>{
"/"         { BEGIN(REGEX); string_buf_bs.len = 0; ByteStringStoreChar('/'); }
}
<REGEX>{
"/"[cp]?    { BEGIN(INITIAL); ByteStringStoreText(yytext, yyleng); yylval.bstr = bstrAlloc(string_buf_bs); return TOKEN_REGEX; }
<<EOF>>     { BEGIN(INITIAL); yyerror("Unclosed Regex"); return YYEOF; }
\n          { yyerror("Newline in Regex"); return YYerror; }
[!-~ \t]    { ByteStringStoreChar(yytext[0]); }
.           { yyerror("Invalid character (ignoring)"); return YYerror;}
}

<INITIAL>{
(g|f)?\\     { BEGIN(PATH_ITEM); string_buf_bs.len = 0; ByteStringStoreText(yytext, yyleng); }
}
<PATH_ITEM>{
\\          { BEGIN(INITIAL); ByteStringStoreChar('\\'); yylval.bstr = bstrAlloc(string_buf_bs); return TOKEN_PATH_ITEM; }
<<EOF>>     { BEGIN(INITIAL); yyerror("Unclosed Path Item"); return YYEOF; }
[ \t\n\r]   { yyerror("Whitespace in Path Item"); return YYerror; }
[!-~]       { ByteStringStoreChar(yytext[0]); }
.           { yyerror("Invalid character (ignoring)"); return YYerror;}
}

 /* dates */
{DATE}T{TIME}@{TZNAME} { yylval.str = AST_STRDUP(yytext); return TOKEN_TZ_DATE_TIME; }
{DATE}T{TIME}Z?        { yylval.str = AST_STRDUP(yytext); return TOKEN_TIA_TIME; }
{DATE}                 { yylval.str = AST_STRDUP(yytext); return TOKEN_PLAIN_DATE; }
{TIME}                 { yylval.str = AST_STRDUP(yytext); return TOKEN_PLAIN_TIME; }

(0|{NON_ZERO_INTEGRAL})l { yylval.str = AST_STRDUP(yytext); return TOKEN_LOGICAL_TIME; }

[+-]0[l]                                        { yyerror("Cannot have sign/negative time"); return YYerror; }
[+-]{NON_ZERO_INTEGRAL}[l]                      { yyerror("Cannot have sign/negative time"); return YYerror; }
0+{NON_ZERO_INTEGRAL}[l]                        { yyerror("Leading zero is not allowed"); return YYerror; }
[+-]?00+[l]                                     { yyerror("Leading zero is not allowed"); return YYerror; }

{DATE}T{TIME}("."[0-9]{3})Z { yylval.str = AST_STRDUP(yytext); return TOKEN_TIMESTAMP; }

 /* deltas */
[+-]{DATE}T{TIME}                        { yylval.str = AST_STRDUP(yytext); return TOKEN_DELTA_DATE_TIME; }
[+-]{NON_ZERO_INTEGRAL}"."[0-9]{1,4}"ds" { yylval.str = AST_STRDUP(yytext); return TOKEN_DELTA_SECONDS; }
[+-]{NON_ZERO_INTEGRAL}"dl"              { yylval.str = AST_STRDUP(yytext); return TOKEN_DELTA_LOGICAL; }
[+-]{DATE}T{TIME}("."[0-9]{3})           { yylval.str = AST_STRDUP(yytext); return TOKEN_DELTA_ISOTIMESTAMP; }


<INITIAL>{
"#!"          { BEGIN(SHEBANG_LINE); string_buf_bs.len = 0; ByteStringStoreText(yytext, yyleng); }
}
<SHEBANG_LINE>{
"\n"          { BEGIN(INITIAL); yylval.bstr = bstrAlloc(string_buf_bs); return TOKEN_SHEBANG_LINE; }
<<EOF>>       { BEGIN(INITIAL); yylval.bstr = bstrAlloc(string_buf_bs); return TOKEN_SHEBANG_LINE; }
.             { yyerror("Invalid character (ignoring)"); return YYerror;}
}

 /* names */
{IDENTIFIER}                            { yylval.str = AST_STRDUP(yytext); return TOKEN_IDENTIFIER; } 
"$?"{IDENTIFIER}                        { yylval.str = AST_STRDUP(yytext); return TOKEN_UNSPEC_IDENTIFIER; }
{TYPE_IDENTIFIER}(::{TYPE_IDENTIFIER})* { yylval.str = AST_STRDUP(yytext); return TOKEN_TYPE_COMPONENT; }

 /* comments */
"%%"[^\n]*  { } /* line comment */

[ \t\n\r]   { } /* ignore white space */ 

. { return yytext[0]; } /* catch all and error in parser */

%%

void ByteStringStoreChar(char c)
{
    if (string_buf_bs.len == MAX_STR_CONST) {
        yyerror("String constant too long");
        string_buf_bs.len++;
    }

    if(string_buf_bs.len < MAX_STR_CONST) {
        string_buf[string_buf_bs.len++] = (uint8_t)c;
    }
}

void ByteStringStoreText(const char* c, size_t len)
{
    if (string_buf_bs.len < MAX_STR_CONST && MAX_STR_CONST <= string_buf_bs.len + len) {
        yyerror("String constant too long");
        string_buf_bs.len += len;
    }

    for(size_t i = 0; i < len; i++) {
        string_buf[string_buf_bs.len++] = (uint8_t)c[i];
    }
}
