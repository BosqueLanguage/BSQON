#include "smt_utils.h"
#include "smt_extract.h"
#include <cstdio>
#include <optional>
#include <regex>

void badArgs(const char* msg)
{
    const char* usage = "USAGE: smtextract <formula.smt2> <fn_signature.json> <assembly.json>";

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

// Look for a "-mk" contruct from @Term.
std::optional<z3::func_decl> findConstruct(z3::func_decl_vector terms, std::string target)
{
    for(size_t i = 0; i < terms.size(); ++i) {
        z3::func_decl ith_term = terms[i];

        if(ith_term.name().str() == target) {
            return ith_term;
        }
    }

    return std::nullopt;
};

// SmtNameType Options:
// STRUCT_CONSTRUCT,
// STRUCT_FIELD,
// STRUCT_TERM_CONSTRUCT,
// STRUCT_TERM_FIELD,
// NAMESPACE_NAME,
// TYPE_CONST_NAME,
// TERM_SUBTYPE_FN_NAME,

std::optional<std::string> tKeyToSmtName(bsqon::TypeKey tk, SmtNameType n)
{
    if(n == STRUCT_CONSTRUCT) {
        return tk + "-mk";
    }
    else if(n == STRUCT_FIELD) {
        // TODO
    }
    else if(n == STRUCT_TERM_CONSTRUCT) {
        return "@Term-" + tk + "-mk";
    }
    else if(n == STRUCT_TERM_FIELD) {
        return "@Term-" + tk + "-value";
    }
    else if(n == NAMESPACE_NAME) {
        // TODO
    }
    else if(n == TYPE_CONST_NAME) {
        // TODO
    }
    else if(n == TERM_SUBTYPE_FN_NAME) {
        return "@SubtypeOf-" + tk;
    }

    return std::nullopt;
}
