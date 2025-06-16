#include "smt_utils.h"
#include "smt_extract.h"
#include <cstddef>
#include <cstdio>
#include <string.h>
#include <cstdlib>
#include <string.h>
#include <cstring>
#include <fstream>
#include <string>
#include <vector>
#include <z3_api.h>

int main(int argc, char** argv)
{
    if(argc != 4) {
        badArgs("");
    }

    // BSQ SMT INIT
    const char* smt_path = argv[1];
    if(validPath(smt_path, "smt2") == false) {
        badArgs("Incorrect .smt2 file");
    }

    const char* smt_file = smt_path;
    z3::context c;
    z3::solver s(c);
    s.add(s.ctx().parse_file(smt_file));

    if(s.check() == z3::unsat) {
        badArgs("UNSAT smt file.");
    }
    else if(s.check() == z3::unknown) {
        badArgs("UNKNOWN smt file.");
    }

    // BSQ ASSEMBLY INIT
    const char* asm_path = argv[2];
    if(validPath(asm_path, "json") == false) {
        badArgs("Incorrect .json file");
    }

    json jv;
    bsqon::AssemblyInfo asm_info;

    const char* asm_json = asm_path;
    try {
        std::ifstream infile(asm_json);
        infile >> jv;
    }
    catch(const std::exception& e) {
        printf("Error parsing JSON: %s\n", e.what());
        exit(1);
    }
    bsqon::AssemblyInfo::parse(jv, asm_info);

    ////////////////////////////////////////////////////////////////////////////
    z3::model m = s.get_model();
    for(uint i = 0; i < m.num_funcs(); ++i) {
        z3::func_decl fn = m.get_func_decl(i);
        std::cout << "FUNC" << fn << "\n";
        std::cout << "ARITY" << fn.arity() << "\n";
        for(uint j = 0; j < fn.arity(); ++j) {
            z3::sort arg = fn.domain(j);
            std::cout << "ARG SORT " << arg.name() << "\n";
        }
    }
    return 0;

    // Find Type in asm_info
    // bsqon::Type* bsq_t = asm_info.lookupTypeKey(tar_t);
    // if(bsq_t == nullptr) {
    //     badArgs("Unable to find TypeKey");
    // }
    //
    // ValueSolver sol(&asm_info, bsq_t, s);
    //
    // bsqon::Value* result = sol.solveValue(sol.bsq_t, sol.ex);
    // if(result == NULL) {
    //     printf("solveValue returned NULL \n");
    //     exit(1);
    // }
    //
    // std::u8string rstr = result->toString();
    // printf("%s\n", (const char*)rstr.c_str());
}
