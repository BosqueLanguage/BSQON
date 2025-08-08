#include "smt_extract.h"
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <z3_api.h>

// Prepare Mock function, validate datatypes and paramaters with the arguments used in bsq.
// Right now the only way to see this is via the smt2 file.
BsqMock::BsqMock(bsqon::AssemblyInfo* asm_info, std::map<std::string, bsqon::Type*>& fn_info, z3::solver& sol)
    : asm_info(asm_info), fn_info(fn_info), s(sol)
{
    if(this->s.check() != z3::sat) {
        perror("Unsat smt2 file when looking for mock\n");
        exit(1);
    }

    z3::model m = this->s.get_model();

    uint fn_nums = m.num_funcs();
    assert(fn_nums > 0);

    // Find Mock.
    z3::func_decl* mock_ptr;
    for(uint i = 0; i < fn_nums; ++i) {
        z3::func_decl fn_ex = m.get_func_decl(i);
        if(fn_ex.name().str().find("getCustomer") != std::string::npos) {
            mock_ptr = &fn_ex;
            break;
        }
    }

    // Prepare Mock
    z3::func_decl mock_fn = *mock_ptr;
    z3::expr_vector args(this->s.ctx());
    for(uint j = 0; j < mock_fn.arity(); ++j) {
        std::string arg_str_sym = "mock_arg_" + std::to_string(j);
        z3::symbol arg_sym = this->s.ctx().str_symbol(arg_str_sym.c_str());

        z3::sort arg_sort = mock_fn.domain(j);
        z3::expr arg_ex = this->s.ctx().constant(arg_sym, arg_sort);
        args.push_back(arg_ex);
    }

    this->s.push();
    for(int i = 0; i < 3; i++) {

        uint c = m.num_funcs();
        z3::expr_vector m_decls(this->s.ctx());

        for(uint i = 0; i < c; ++i) {
            z3::func_decl c_decl = m.get_func_decl(i);
            std::cout << c_decl << "\n";
        }

        this->s.add(z3::mk_or(m_decls));

        if(this->s.check() != z3::sat) {
            printf("UNSAT BLOCKED MODEL\n");
            exit(1);
        }

        z3::model new_m = this->s.get_model();
        for(uint i = 0; i < c; ++i) {
            z3::func_interp old_ex = m.get_func_interp(m.get_func_decl(i));
            z3::func_interp new_ex = m.get_func_interp(m.get_func_decl(i));
            std::cout << "OLD " << old_ex << "\n";
            std::cout << "NEW " << new_ex << "\n";
        }
        std::cout << "---------------------------------------------------" << "\n";
    }
    this->s.pop();
}
