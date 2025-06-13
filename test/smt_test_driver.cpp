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

std::string createSmtBSQONPathName(std::string tcc)
{
    return std::string(TEST_PATH) + "asm_meta/smt/" + tcc + ".json";
}

void smt_tround(std::string smt_in, std::string meta_file, const char* t_in, std::u8string& result)
{
    // BSQ SMT INIT
    const char* smt_path = smt_in.c_str();
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
    const char* asm_path = meta_file.c_str();
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

    // TODO: Check type is passed in correct format of --<TYPE>
    const char* tar_t = t_in + 2; /*Just move ptr to get past '--'.*/
    if(!(tar_t[0] >= 'A' || tar_t[0] <= 'Z')) {
        badArgs("Incorrect Type format.");
    }

    ////////////////////////////////////////////////////////////////////////////
    // Find Type in asm_info
    bsqon::Type* bsq_t = asm_info.lookupTypeKey(tar_t);
    if(bsq_t == nullptr) {
        badArgs("Unable to find TypeKey");
    }

    ValueSolver sol(&asm_info, bsq_t, s);
    bsqon::Value* res = sol.solveValue(sol.bsq_t, sol.fn());
    if(res == NULL) {
        printf("GOT NULL: From first call of solveValue.\n");
        exit(1);
    }

    result = res->toString();
    return;
}
