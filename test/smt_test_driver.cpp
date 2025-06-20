#include "smt_test_driver.h"

#include <fstream>

#ifndef TEST_PATH
#define TEST_PATH "./"
#endif

void checkAndReport(const std::u8string& result, const std::u8string& expected)
{
    if(result != expected) {
        std::cout << "\033[33;1mExpected:\033[0m " << std::string(expected.cbegin(), expected.cend()) << std::endl;
        std::cout << "\033[33;1mGot:\033[0m      " << std::string(result.cbegin(), result.cend()) << std::endl;
    }

    BOOST_ASSERT(result == expected);
}

std::string createSmtPathName(std::string tcc)
{
    return std::string(TEST_PATH) + "asm_meta/smt/" + tcc + ".smt2";
}

std::string createSmtJSONPathName(std::string tcc)
{
    return std::string(TEST_PATH) + "asm_meta/smt/" + tcc + ".json";
}

void smt_tround(std::string smt_in, std::string fn_in, std::string tref_in, std::u8string& result)
{
    // Load SMT FILE
    const char* smt_file = smt_in.c_str();
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
    const char* asm_file = tref_in.c_str();
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
    const char* fn_info_file = fn_in.c_str();
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

    // FIND VALUES FOR ALL FN ARGUMENTS
    for(const auto& [key, value] : arg_refs) {
        ValueSolver sol(&asm_info, key, s);
        bsqon::Value* res = sol.solveValue(value, sol.ex);
        if(res == NULL) {
            printf("solveValue returned NULL \n");
            exit(1);
        }

        result += res->toString() + u8"\n";
    }
}
