#pragma once

#include "../../build/include/z3/z3++.h"
#include "../../build/include/json/json.hpp"
#include "../info/type_info.h"
#include "../info/bsqon.h"
#include <cstdio>

#define BSQ_INT_MIN -500
#define BSQ_INT_MAX 500
#define BSQ_BIGINT_MIN -1000
#define BSQ_BIGINT_MAX 1000

#define BSQ_NAT_MIN 0
#define BSQ_NAT_MAX 500

#define BSQ_BIGNAT_MIN 0
#define BSQ_BIGNAT_MAX 1000

#define ASCII_MIN 0
#define ASCII_MAX 255

class ValueSolver
{
  public:
    bsqon::AssemblyInfo* asm_info;
    z3::solver& s;
    z3::expr ex;

    ValueSolver(bsqon::AssemblyInfo* asm_info, std::string target, z3::solver& s);

    bsqon::Value* solveCString(const bsqon::PrimitiveType* bsq_t, z3::expr ex);
    bsqon::Value* solveBool(const bsqon::PrimitiveType* bsq_t, z3::expr ex);
    bsqon::Value* solveBigNat(const bsqon::PrimitiveType* bsq_t, z3::expr ex);
    bsqon::Value* solveNat(const bsqon::PrimitiveType* bsq_t, z3::expr ex);
    bsqon::Value* solveBigInt(const bsqon::PrimitiveType* bsq_t, z3::expr ex);
    bsqon::Value* solveInt(const bsqon::PrimitiveType* bsq_t, z3::expr ex);
    bsqon::Value* solvePrimitive(bsqon::PrimitiveType* t, z3::expr ex);
    bsqon::Value* solveEntity(bsqon::StdEntityType* t, z3::expr ex);
    bsqon::Value* solveValue(bsqon::Type* t, z3::expr ex);

    std::optional<z3::expr> getExprFromVal(bsqon::Value* v);
    std::optional<char> BinSearchChar(z3::expr str_exp, z3::expr index, int min, int max);
    z3::expr FindStringLen(z3::expr ex);
    bool isDatatype(bsqon::Type* bsq_t, z3::func_decl fn);
    bool isPrimitive(bsqon::Type* bsq_t, z3::func_decl fn);
};
