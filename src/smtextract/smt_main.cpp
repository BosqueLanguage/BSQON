#include "smt_extract.h"
#include "smt_utils.h"
#include <fstream>

int main(int argc, char** argv)
{
    if(argc != 4) {
        badArgs("");
    }

    // Load SMT FILE
    const char* smt_file = argv[1];
    if(validPath(smt_file, "smt2") == false) {
        badArgs("Incorrect .smt2 file");
    }

    z3::context c;
    z3::solver s(c);
    s.from_file(smt_file);

    z3::check_result chk_smt = s.check();
    if(chk_smt == z3::unsat) {
        badArgs("Unsat smt file\n");
    }
    else if(chk_smt == z3::unknown) {
        badArgs("Unknown smt file\n");
    }

    // Load TYPEINFO FILE
    const char* asm_file = argv[3];
    if(validPath(asm_file, "json") == false) {
        badArgs("Incorrect .json file");
    }

    json j_type;
    bsqon::AssemblyInfo asm_info;
    try {
        std::ifstream infile(asm_file);
        infile >> j_type;
    }
    catch(const std::exception& e) {
        printf("Error parsing JSON: %s\n", e.what());
        exit(1);
    }
    bsqon::AssemblyInfo::parse(j_type, asm_info);

    // Load FN INFO FILE
    const char* fn_info_file = argv[2];
    if(validPath(fn_info_file, "json") == false) {
        badArgs("Incorrect .json file");
    }

    std::map<std::string, bsqon::Type*> arg_refs;
    json j_fn;
    try {
        std::ifstream infile(fn_info_file);
        infile >> j_fn;
    }
    catch(const std::exception& e) {
        printf("Error parsing JSON: %s\n", e.what());
        exit(1);
    }

    for(const auto& arg : j_fn["args"]) {
        arg_refs[arg["name"]] = asm_info.lookupTypeKey(arg["type"]);
    }

    for(const auto& [key, value] : arg_refs) {
        ValueSolver sol(&asm_info, key, s);
        bsqon::Value* result = sol.solveValue(value, sol.ex);
        if(result == NULL) {
            printf("solveValue returned NULL \n");
            exit(1);
        }

        std::u8string rstr = result->toString();
        printf("%s\n", (const char*)rstr.c_str());
    }
    return 0;
}
