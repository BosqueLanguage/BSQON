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
    std::cout << m << "\n";
    for(size_t i = 0; i < m.num_consts(); i++) {
        std::string const_name = m.get_const_decl(i).range().name().str();
        std::cout << "Comparing.. " << smt_tk << " with... " << const_name << "\n";
        if(strcmp(const_name.c_str(), smt_tk.c_str()) == 0) {
            return m.get_const_decl(i);
        }
    }
    return std::nullopt;
}

// Fills function args with constants of their respective sorts.
void initArgs(z3::expr_vector& args, size_t args_len, z3::func_decl f, z3::solver& s)
{
    for(size_t i = 0; i < args_len; i++) {
        std::string sym = "arg_tmp" + std::to_string(i);

        z3::sort arg_t = f.domain(i);
        z3::symbol arg_sym = s.ctx().str_symbol(sym.c_str());

        z3::expr arg_tmp = s.ctx().constant(arg_sym, arg_t);
        args.push_back(arg_tmp);
    }
}

z3::expr ValueSolver::getExprFromVal(bsqon::Value* v)
{

    auto vk = v->kind;
    if(vk == bsqon::ValueKind::IntNumberValueKind) {
        bsqon::IntNumberValue* nv = static_cast<bsqon::IntNumberValue*>(v);
        return this->s.ctx().int_val(nv->cnv);
    }

    return this->s.ctx().int_val(9999);
}
