#include "smt_extract.h"
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

char* ReadFile(const char* filename)
{
    FILE* f = fopen(filename, "r");
    if(!f) {
        perror("fopen");
    }
    fseek(f, 0, SEEK_END);
    long length = ftell(f);
    rewind(f);
    char* bfr = (char*)malloc(sizeof(char) * length);

    fread(bfr, sizeof(char), length, f);

    fclose(f);
    return bfr;
}

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
    // HACK: Fix to prevent z3 to optimize the functions that use define-fun.
    char* smt_bfr = ReadFile(smt_file);
    std::string smt_str(smt_bfr);
    free(smt_bfr);

    std::string helper_fn = ";;;;;;;;;;;;;;;\n (declare-fun MockTest (Int) Int) (assert (> (MockTest 5) 2))";
    std::string smt_formula = smt_str + helper_fn;

    z3::context c;
    z3::solver sol(c);
    sol.from_string(smt_formula.c_str());

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

    json fn_json;
    try {
        std::ifstream infile(fn_info_file);
        infile >> fn_json;
    }
    catch(const std::exception& e) {
        printf("Error parsing JSON: %s\n", e.what());
        exit(1);
    }

    std::string mode(argv[4]);

    if(mode == "-t" || mode == "--test") {
        runMock(&asm_info, fn_json, sol, "test");
    }
    else if(mode == "-m" || mode == "--mock") {
        runMock(&asm_info, fn_json, sol, "mock");
    }
    else {
        std::string err = mode + " is not a valid <MODE>.";
        badArgs(err.c_str());
    }

    Z3_finalize_memory();
    return 0;
}

void extractReturnValue(bsqon::AssemblyInfo* asm_info, ExtractSig ret, z3::solver& sol)
{
    ValueExtractor extractor(asm_info, sol);
    bsqon::Value* c_val = extractor.extractValue(ret.type, ret.ex);

    printf("Type: %s\n", (const char*)ret.type->tkey.c_str());
    printf("Value: %s\n", (const char*)c_val->toString().c_str());
}

void extractErrorArgsValues(bsqon::AssemblyInfo* asm_info, std::vector<ExtractSig> args, z3::solver& sol)
{
    ValueExtractor extractor(asm_info, sol);
    for(ExtractSig arg : args) {
        bsqon::Value* c_val = extractor.extractValue(arg.type, arg.ex);

        printf("Type: %s\n", (const char*)arg.type->tkey.c_str());
        printf("Value: %s\n", (const char*)c_val->toString().c_str());
    }
}

std::optional<z3::expr> FindConstantInModel(z3::model m, std::string id)
{
    uint n_consts = m.num_consts();
    for(uint i = 0; i < n_consts; ++i) {
        z3::func_decl fn_const = m.get_const_decl(i);
        std::string fn_id = fn_const.name().str();

        if(fn_id == id) {
            return fn_const();
        }
    }

    return std::nullopt;
}

void runMock(bsqon::AssemblyInfo* asm_info, json mock_json, z3::solver& sol, std::string mode)
{
    // Extract Return value of mock.
    if(mode == "mock") {
        std::string ret_id = "";
        bsqon::TypeKey tkey = mock_json["return"];
        bsqon::Type* ret_t = asm_info->lookupTypeKey(tkey);
        ret_id = "@retMock-" + tKeyToSmtName(tkey, SMT_TYPE);

        z3::check_result cr = sol.check();
        if(cr != z3::sat) {
            std::cout << "Got " << cr << " from .smt2 file";
            exit(1);
        }

        z3::model m = sol.get_model();

        auto const_ex = FindConstantInModel(m, ret_id);
        if(!const_ex.has_value()) {
            std::cout << "Unable to find " << ret_id << " in z3 model.\n";
            exit(1);
        }

        ExtractSig sig = {
            .type = ret_t,
            .ex = const_ex.value(),
        };

        extractReturnValue(asm_info, sig, sol);
    }

    // Extract Arg values of mock.
    if(mode == "test") {
        std::vector<ExtractSig> arguments;
        for(auto arg : mock_json["args"]) {
            std::string id = arg["name"];
            bsqon::TypeKey tkey = arg["type"];
            bsqon::Type* arg_t = asm_info->lookupTypeKey(tkey);

            z3::model m = sol.get_model();
            z3::expr arg_ex(sol.ctx());

            ExtractSig sig = {
                .type = arg_t,
                .ex = arg_ex,
            };

            std::string arg_id = "@arg-" + id;
            auto const_ex = FindConstantInModel(m, arg_id);
            if(!const_ex.has_value()) {
                std::cout << "Unable to find " << arg_id << " in z3 model.\n";
                exit(1);
            }
            arguments.push_back(sig);
        }

        extractErrorArgsValues(asm_info, arguments, sol);
    }
}
