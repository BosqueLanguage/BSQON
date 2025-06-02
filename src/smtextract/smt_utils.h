#pragma once

#include "../../build/include/z3/z3++.h"
#include "../info/type_info.h"
#include "../info/bsqon.h"

typedef struct SmtFunc
{
    z3::solver& s;
    z3::func_decl decl;
    z3::sort sort;
} SmtFunc;

typedef union Result {
    bsqon::Value* val;
    z3::expr expr;
} Result;

// Utils
void badArgs(const char* msg);
bool validPath(const char* filepath, const char* extension);
bsqon::TypeKey bsqonToSmt(bsqon::TypeKey tk);
std::optional<z3::func_decl> getFuncDecl(bsqon::Type* bsq_t, z3::solver& s);

// Tree Path Utils
bool isDatatype(bsqon::Type* bsq_t, SmtFunc fn);
bool isPrimitive(bsqon::Type* bsq_t, SmtFunc fn);

Result* solveValue(bsqon::Type* bsq_t, SmtFunc fn);
