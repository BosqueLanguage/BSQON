#include "smt_extract.h"
#include <cassert>
#include <cstdio>
#include <string>

// Prepare Mock function, validate datatypes and paramaters with the arguments used in bsq.
// Right now the only way to see this is via the smt2 file.
BsqMock::BsqMock(bsqon::AssemblyInfo* asm_info, std::map<std::string, bsqon::Type*>& fn_info, z3::solver& sol)
    : asm_info(asm_info), fn_info(fn_info), s(sol)
{
    z3::model m = this->s.get_model();
    uint fn_nums = m.num_funcs();
    assert(fn_nums > 0);

    this->s.push();
    for(uint i = 0; i < fn_nums; ++i) {
        z3::func_decl fn_ex = m.get_func_decl(i);
        if(fn_ex.name().str().find("getCustomer") != std::string::npos) {
            // Build fn expr
            z3::expr_vector args(this->s.ctx());
            for(uint j = 0; j < fn_ex.arity(); ++j) {
                std::string arg_str_sym = "mock_arg_" + std::to_string(j);
                z3::symbol arg_sym = this->s.ctx().str_symbol(arg_str_sym.c_str());

                z3::sort arg_sort = fn_ex.domain(j);
                z3::expr arg_ex = this->s.ctx().constant(arg_sym, arg_sort);
                args.push_back(arg_ex);
            }

            z3::expr mock_ex = fn_ex(args);
            z3::expr mock_res = mockTest(mock_ex, fn_ex.range());
        }
    }
    this->s.pop();
}

// Setup $return constraints.
z3::expr BsqMock::mockTest(z3::expr mock, z3::sort return_type)
{
    // TODO: Read the .json for the specific mock function and try to extract the pre and post values.
    std::cout << "MOCK EX: " << mock << "RETURN: " << return_type << "\n";
    return this->s.ctx().bool_const("heyo");
}
