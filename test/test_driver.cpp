#include "test_driver.h"

#include <filesystem>
#include <iostream>
#include <fstream>

void loadAssemblyJSONExplicit(std::string filename, json& jv)
{
    std::ifstream infile(filename);
    infile >> jv;
}

std::u8string wsnorm(const std::u8string& s)
{
    std::regex reg(R"(\s+)");
    std::string ssuu = std::string(s.cbegin(), s.cend());
    std::string ss = std::regex_replace(ssuu, reg, " ");

    if(ss.starts_with(" ")) {
        ss = ss.substr(1);
    }

    if(ss.ends_with(" ")) {
        ss = ss.substr(0, ss.size() - 1);
    }

    return std::u8string(ss.cbegin(), ss.cend());
}

void loadContents(std::string filename, std::u8string& contents)
{
    std::string line;
    std::ifstream rfile;
    rfile.open(filename);
    while(std::getline(rfile, line)) {
        contents += std::u8string(line.cbegin(), line.cend());
    }
    rfile.close();

    contents = wsnorm(contents);
}

void checkAndReport(const std::u8string& result, const std::u8string& expected)
{
    if(result != expected) {
        std::cout << "Expected: " << std::string(expected.cbegin(), expected.cend()) << std::endl;
        std::cout << "Got: " << std::string(result.cbegin(), result.cend()) << std::endl;
    }

    BOOST_ASSERT(result == expected);
}

#ifndef TEST_PATH
#define TEST_PATH "./"
#endif

std::string createMetaPathName(std::string tcc)
{
    return std::string(TEST_PATH) + "asm_meta/" + tcc + ".json";
}

std::string createBSQONPathName(std::string tcc, std::string bsq)
{
    return std::string(TEST_PATH) + "bsqon/" + tcc + "/" + bsq;
}

std::string createSmtPathName(std::string tcc)
{
    return std::string(TEST_PATH) + "asm_meta/smt/" + tcc + ".smt2";
}

std::string createSmtBSQONPathName(std::string tcc)
{
    return std::string(TEST_PATH) + "asm_meta/smt/" + tcc + ".json";
}

void tround(std::string metafile, const char* type, std::string datafile, std::u8string& contents,
            std::u8string& result)
{
    result = u8"";
    std::string metadata;

    if(!std::filesystem::exists(metafile)) {
        result = u8"Metadata file does not exist";
        return;
    }

    if(!std::filesystem::exists(datafile)) {
        result = std::u8string(u8"Data file does not exist: ") + std::u8string(datafile.cbegin(), datafile.cend());
        return;
    }

    // the property value is the BSQON value (as a JSON string) so parse it
    const BSQON_AST_Node* node = BSQON_AST_parse_from_file(datafile.c_str());
    char** errorInfo = (char**)malloc(sizeof(char*) * 128);
    size_t errorInfoCount = BSQON_AST_getErrorInfo(errorInfo);

    if(node == nullptr) {
        auto sstr = std::string(errorInfo[0]);
        result = std::u8string(sstr.cbegin(), sstr.cend());
        return;
    }

    loadContents(datafile, contents);

    // parse the JSON
    json jv = nullptr;
    loadAssemblyJSONExplicit(metafile, jv);

    // the property assembly is the code so load it
    bsqon::AssemblyInfo assembly;
    bsqon::AssemblyInfo::parse(jv, assembly);

    bsqon::Parser parser(&assembly);

    // the property loadtype is the type so look it up
    std::string typestr = std::string(type);
    auto loadtype = assembly.lookupTypeKey(typestr);

    if(loadtype->isUnresolved()) {
        result = u8"Invalid 'loadtype";
        return;
    }

    auto ccpos = loadtype->tkey.find("::");
    if(ccpos == std::string::npos) {
        parser.defaultns = "Core";
    }
    else {
        parser.defaultns = loadtype->tkey.substr(0, ccpos);
    }

    // load up any environment variables that we need
    auto envkeys = bsqon::Parser::getEnvironmentBindKeys(node);

    // finally parse the value
    bsqon::BsqonDecl* res = parser.parseBSQON(metadata, loadtype, node);

    if(parser.errors.empty() && errorInfoCount == 0) {
        std::u8string rstr = res->toString(true);

        result = wsnorm(rstr);
        return;
    }
    else {
        for(size_t i = 0; i < errorInfoCount; ++i) {
            std::string sstr(errorInfo[i]);
            result += std::u8string(sstr.cbegin(), sstr.cend());
        }

        for(size_t i = 0; i < parser.errors.size(); ++i) {
            const bsqon::ParseError& pe = parser.errors.at(i);
            result = std::u8string(pe.message.cbegin(), pe.message.cend());
        }

        return;
    }
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
    bsqon::Value* res = sol.solveValue(sol.bsq_t, sol.fn);
    if(res == NULL) {
        printf("GOT NULL: From first call of solveValue.\n");
        exit(1);
    }
    std::u8string rstr = res->toString();
    // printf("%s\n", (const char*)rstr.c_str());
    result = rstr;
    return;
}
