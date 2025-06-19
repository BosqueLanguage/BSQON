#pragma once

#include "../../build/include/z3/z3++.h"
#include "../info/type_info.h"
#include "../info/bsqon.h"
#include <cstdio>

// Utils
void badArgs(const char* msg);
bsqon::TypeKey bsqonToSmt(bsqon::TypeKey tk);
bool validPath(const char* filepath, const char* extension);
std::optional<z3::expr> getBsqTypeExpr(bsqon::Type* bsq_t, z3::solver& s, uint ith);
