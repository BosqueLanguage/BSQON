#pragma once

#include "../../build/include/z3/z3++.h"
#include <cstdio>
#include <optional>
#include "../info/bsqon.h"

typedef struct TermType
{
    z3::func_decl mk;
    z3::func_decl rg;
} TermType;

// Utils
void badArgs(const char* msg);
bool validPath(const char* filepath, const char* extension);
std::optional<TermType> findConstruct(z3::func_decl_vector terms, z3::func_decl_vector recognizers, std::string target);
