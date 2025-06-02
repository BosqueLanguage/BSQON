#include "smt_utils.h"
#include <iterator>
#include <regex>
#include <iostream>

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

bool isDatatype(bsqon::Type* bsq_t, SmtFunc fn)
{
    if(bsq_t->tag == bsqon::TypeTag::TYPE_STD_ENTITY) {
        return true;
    }

    if(fn.decl().is_datatype()) {
        return true;
    }

    return false;
}

bool isPrimitive(bsqon::Type* bsq_t, SmtFunc fn)
{
    if(bsq_t->tag == bsqon::TypeTag::TYPE_PRIMITIVE) {
        return true;
    }

    if(fn.sort.is_seq()) {
        return true;
    }
    else if(fn.sort.is_bool()) {
        return true;
    }
    else if(fn.sort.is_int()) {
        return true;
    }

    return false;
}
