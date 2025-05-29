#include "smt_utils.h"
#include <cstdio>
#include <cstdlib>
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

std::optional<std::string> solvePrimitive(bsqon::PrimitiveType* bsq_t, z3::solver& s)
{
    // z3::expr entity_tmp = s.ctx().constant();
    // std::cout << bsq_t->fields[1].ftype << "\n";
    auto tk = bsq_t->tkey;
    if(tk == "Int") {
        return "GOT INTEGER";
    }
}

std::optional<std::string> solveEntity(bsqon::StdEntityType* bsq_t, z3::solver& s)
{
    // z3::expr entity_tmp = s.ctx().constant();
    std::cout << bsq_t->fields[1].ftype << "\n";
}

std::optional<std::string> solveValue(bsqon::AssemblyInfo info, z3::solver& s, std::string target)
{
    bsqon::Type* bsq_t = nullptr;
    std::optional<std::string> res;

    bsq_t = info.lookupTypeKey(target);

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

    // Solve for values start
    // TODO: Parse --Int to Int.
    const char* tar_t = argv[3] + 2; /*Just move ptr to get past '--'.*/
    if(!(tar_t[0] >= 'A' || tar_t[0] <= 'Z')) {
        badArgs("Incorrect Type.");
    }

    auto value = solveValue(asm_info, s, tar_t);
    if(value.has_value()) {
        std::cout << "GOT VALUE: " << value.value() << "\n";
    }
    else {
        std::cout << "NO VALUE :3" << "\n";
    }
};
