#include "smt_extract.h"
#include <cstdio>
#include <fstream>

int main(int argc, char** argv)
{
    if(argc != 5) {
        badArgs("");
    }

    if(argc == 4) {
        badArgs("Missing --<MODE>");
    }

    // Load SMT FILE
    const char* smt_file = argv[1];
    if(validPath(smt_file, "smt2") == false) {
        badArgs("Incorrect .smt2 file");
    }

    z3::context c;
    z3::solver sol(c);
    sol.from_file(smt_file);

    z3::check_result chk_smt = sol.check();
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
    bsqon::loadAssembly(j_type, asm_info);

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

    std::string mode(argv[4]);

    if(mode == "-e" || mode == "--extract") {
        for(const auto& [id, type] : arg_refs) {
            ValueExtractor extract(&asm_info, type, id, sol);
            bsqon::Value* v = extract.value;
            printf("%s\n", (const char*)v->toString().c_str());
        }
    }
    else if(mode == "-g" || mode == "--generate") {
        for(const auto& [id, type] : arg_refs) {
            ValueExtractor extract(&asm_info, type, id, sol);
            std::cout << extract.extractSMTFromValue() << "\n";
        }
    }
    else if(mode == "-m" || mode == "--mock") {
        printf("Starting Mocks... \n");
        BsqMock mock(&asm_info, arg_refs, sol);
    }
    else {
        std::string err = mode + " is not a valid <MODE>.";
        badArgs(err.c_str());
    }

    Z3_finalize_memory();
    return 0;
}
