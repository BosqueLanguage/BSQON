#include "smt_utils.h"
#include "smt_extract.h"
#include <cstdio>
#include <regex>

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

        std::string const_name = m.get_const_decl(i).range().name().str();
        if(strcmp(const_name.c_str(), smt_tk.c_str()) == 0) {
            return m.get_const_decl(i);
        }
    }
    return std::nullopt;
}

std::optional<z3::expr> ValueSolver::getExprFromVal(bsqon::Value* v)
{

    auto vk = v->kind;
    if(vk == bsqon::ValueKind::IntNumberValueKind) {
        bsqon::IntNumberValue* nv = static_cast<bsqon::IntNumberValue*>(v);
        return this->s.ctx().int_val(nv->cnv);
    }
    else if(vk == bsqon::ValueKind::BigIntNumberValueKind) {
        bsqon::BigIntNumberValue* nv = static_cast<bsqon::BigIntNumberValue*>(v);
        return this->s.ctx().int_val(int64_t(nv->cnv));
    }
    else if(vk == bsqon::ValueKind::NatNumberValueKind) {
        bsqon::NatNumberValue* uv = static_cast<bsqon::NatNumberValue*>(v);
        return this->s.ctx().int_val(uv->cnv);
    }
    else if(vk == bsqon::ValueKind::BigNatNumberValueKind) {
        bsqon::BigNatNumberValue* uv = static_cast<bsqon::BigNatNumberValue*>(v);
        return this->s.ctx().int_val(uint64_t(uv->cnv));
    }
    else if(vk == bsqon::ValueKind::BoolValueKind) {
        bsqon::BoolValue* bv = static_cast<bsqon::BoolValue*>(v);
        return this->s.ctx().bool_val(bv->tv);
    }
    else if(vk == bsqon::ValueKind::CStringValueKind) {
        bsqon::CStringValue* Cv = static_cast<bsqon::CStringValue*>(v);
        return this->s.ctx().string_val(Cv->sv);
    }

    return std::nullopt;
}
