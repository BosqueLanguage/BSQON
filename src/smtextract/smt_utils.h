#pragma once

#include "../../build/include/z3/z3++.h"
#include "../info/type_info.h"
#include "../info/bsqon.h"
#include <cstdio>

typedef struct TermType
{
    z3::func_decl mk;
    z3::func_decl rg;
} TermType;

enum SmtNameType
{
    NONE_NAME,
    STRUCT_CONSTRUCT,
    STRUCT_FIELD,
    STRUCT_TERM_CONSTRUCT,
    STRUCT_TERM_FIELD,
    NAMESPACE_NAME,
    TYPE_CONST_NAME,
    TERM_SUBTYPE_FN_NAME,
};

// Utils
void badArgs(const char* msg);
bool validPath(const char* filepath, const char* extension);
std::string tKeyToSmtName(bsqon::TypeKey tk, SmtNameType n);
std::optional<TermType> findConstruct(z3::func_decl_vector terms, z3::func_decl_vector recognizers, std::string target);
