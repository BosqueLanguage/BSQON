#pragma once

#include "../../build/include/z3/z3++.h"
#include "../info/type_info.h"
#include "../info/bsqon.h"
#include <cstdio>

class ValueSolver
{
  public:
    bsqon::AssemblyInfo* asm_info;
    bsqon::Type* bsq_t;
    z3::solver& s;
    z3::func_decl fn;

    ValueSolver(bsqon::AssemblyInfo* asm_info, bsqon::Type* bsq_t, z3::solver& s);

    z3::expr getExprFromVal(bsqon::Value* v);
    bool isDatatype(bsqon::Type* bsq_t, z3::func_decl fn);
    bool isPrimitive(bsqon::Type* bsq_t, z3::func_decl fn);

    bsqon::IntNumberValue* solveInt(const bsqon::PrimitiveType* bsq_t, z3::func_decl fn);
    bsqon::Value* solvePrimitive(bsqon::PrimitiveType* t, z3::func_decl fn);
    bsqon::Value* solveEntity(bsqon::StdEntityType* t, z3::func_decl fn);
    bsqon::Value* solveValue(bsqon::Type* t, z3::func_decl fn);
};
