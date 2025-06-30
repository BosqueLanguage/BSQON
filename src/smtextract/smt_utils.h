#pragma once

#include "../../build/include/z3/z3++.h"
#include "../info/type_info.h"
#include "../info/bsqon.h"
#include <cstdio>

enum SmtNameType
{
    NONE,
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
std::optional<z3::expr> getBsqTypeExpr(bsqon::Type* bsq_t, z3::solver& s);
std::optional<std::string> tKeyToSmtName(bsqon::TypeKey tk, SmtNameType n);
std::optional<z3::func_decl> findConstruct(z3::func_decl_vector terms, std::string target);
