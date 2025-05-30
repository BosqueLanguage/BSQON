#include "smt_utils.h"
#include <cstddef>
#include <iterator>
#include <cstdio>
#include <string.h>
#include <regex>
#include <cstdlib>
#include <string.h>
#include <cstring>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <z3_api.h>

void badArgs(const char* msg)
{
    const char* usage = "USAGE: smtextract <formula.smt2> <assembly.json> <types>\n\nTYPES:\n  --Int\n  --Entity\n";
    printf("%s\n", usage);
    printf("%s\n", msg);
    exit(1);
}

bool validPath(const char* filepath, const char* extension)
{
    char* og;
    char* tokens;
    char* track;
    bool valid = false;

    og = strdup(filepath);
    track = og;

    while((tokens = strtok_r(track, ".", &track))) {
        if(strcmp(tokens, extension) == 0) {
            valid = true;
        }
    }

    return valid;
}

bsqon::TypeKey bsqonToSmt(bsqon::TypeKey tk)
{
    std::string og = tk;
    std::regex bsq_name("::");
    std::string smt_tk = std::regex_replace(og, bsq_name, "@");

    return bsqon::TypeKey(smt_tk);
}

std::optional<std::string> solvePrimitive(bsqon::PrimitiveType* bsq_t, z3::solver& s)
{
    auto tk = bsq_t->tkey;
    if(tk == "Int") {
        return "GOT INTEGER";
    }
    return std::nullopt;
}

// Use Type* to find the func_decl in the z3::model.
std::optional<z3::func_decl> getFuncDecl(bsqon::Type* bsq_t, z3::solver& s)
{
    bsqon::TypeKey smt_tk = bsqonToSmt(bsq_t->tkey);
    z3::model m = s.get_model();
    for(size_t i = 0; i < m.num_consts(); i++) {
        std::string wth = m.get_const_decl(i).range().name().str();
        if(strcmp(wth.c_str(), smt_tk.c_str()) == 0) {
            return m.get_const_decl(i);
        }
    }
    return std::nullopt;
}

std::optional<std::string> solveEntity(bsqon::StdEntityType* bsq_t, z3::solver& s)
{

    return std::nullopt;
}

std::optional<std::string> solveValue(bsqon::Type* bsq_t, z3::solver& s)
{
    std::optional<std::string> res;

    if(bsq_t->tag == bsqon::TypeTag::TYPE_STD_ENTITY) {
        res = solveEntity(static_cast<bsqon::StdEntityType*>(bsq_t), s);
    }
    else if(bsq_t->tag == bsqon::TypeTag::TYPE_PRIMITIVE) {
        res = solvePrimitive(static_cast<bsqon::PrimitiveType*>(bsq_t), s);
    }

    return std::nullopt;
}

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
    bsqon::AssemblyInfo assembly;
    bsqon::AssemblyInfo::parse(jv, asm_info);

    // TODO: Check type is passed in correct format of --<TYPE>
    const char* tar_t = argv[3] + 2; /*Just move ptr to get past '--'.*/
    if(!(tar_t[0] >= 'A' || tar_t[0] <= 'Z')) {
        badArgs("Incorrect Type format.");
    }

    ////////////////////////////////////////////////////////////////////////////
    // Look for type in assembly.
    bsqon::Type* bsq_t = asm_info.lookupTypeKey(tar_t);
    if(bsq_t == nullptr) {
        badArgs("Unable to find TypeKey");
    }

    // Find for const in smt.
    auto bsq_func = getFuncDecl(bsq_t, s);
    if(bsq_func.has_value()) {
        std::cout << bsq_func.value() << "\n";
    }

    // TODO: Make it return Value*. At the moment just string.
    auto value = solveValue(bsq_t, s);
    if(value.has_value()) {
        std::cout << "GOT VALUE: " << value.value() << "\n";
    }
    else {
        std::cout << "NO VALUE :3" << "\n";
    }
};
