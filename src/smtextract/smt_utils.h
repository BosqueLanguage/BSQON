#pragma once

#include "../../build/include/z3/z3++.h"
#include "../info/type_info.h"

typedef struct smt_func
{
    z3::solver& s;
    z3::func_decl decl;
    Z3_sort_kind sort;
} smt_func;
