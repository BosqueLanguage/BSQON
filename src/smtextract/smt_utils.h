#pragma once

#include "../../build/include/z3/z3++.h"
#include "../info/type_info.h"
#include "../info/bsqon.h"
#include <cstdio>

// Utils
void badArgs(const char* msg);
bsqon::TypeKey bsqonToSmt(bsqon::TypeKey tk);
bool validPath(const char* filepath, const char* extension);
void initArgs(z3::expr_vector& args, size_t args_len, z3::func_decl f, z3::solver& s);
std::optional<z3::func_decl> getFuncDecl(bsqon::Type* bsq_t, z3::solver& s);
