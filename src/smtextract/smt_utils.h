#pragma once

#include "../../build/include/z3/z3++.h"
#include "../info/type_info.h"
#include "../info/bsqon.h"

class ValueSolver
{
  public:
    z3::solver& s;
    bsqon::AssemblyInfo asm_info;
    bsqon::Type* bsq_t;
    z3::func_decl decl;
    z3::func_decl sort;
};

// Utils
void badArgs(const char* msg);
bsqon::TypeKey bsqonToSmt(bsqon::TypeKey tk);
bool validPath(const char* filepath, const char* extension);
std::optional<z3::func_decl> getFuncDecl(bsqon::Type* bsq_t, z3::solver& s);
void initArgs(z3::expr_vector& args, size_t args_len, z3::func_decl f, z3::solver& s);

// Tree Path Utils
bool isDatatype(bsqon::Type* bsq_t, SmtFunc fn);
bool isPrimitive(bsqon::Type* bsq_t, SmtFunc fn);

bsqon::Value* solveValue(bsqon::Type* bsq_t, SmtFunc fn);
