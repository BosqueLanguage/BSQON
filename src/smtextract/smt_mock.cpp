#include "smt_extract.h"

// Prepare Mock function, validate datatypes and paramaters with the arguments used in bsq.
// Right now the only way to see this is via the smt2 file.
BsqMock::BsqMock(bsqon::AssemblyInfo* asm_info, std::map<std::string, bsqon::Type*>& fn_info, z3::solver& s)
    : asm_info(asm_info), fn_info(fn_info), s(s)
{
    std::cout << "SATISFIABLE? -> " << this->s.check() << "\n";
    std::cout << "MODEL: " << this->s.get_model() << "\n";

    // Find the caller of the mock,if there is one.
    // Find the mock sign in the model.
    // Find the argument values that were passed to the mock.
}

// Setup $return constraints.
z3::expr BsqMock::mockTest(z3::expr mock)
{
    return this->s.ctx().bool_const("heyo");
}
