#include "smt_utils.h"
#include <cstddef>
#include <cstdio>
#include <string.h>
#include <cstdlib>
#include <string.h>
#include <cstring>
#include <fstream>
#include <optional>
#include <string>
#include <z3_api.h>

Result* solveInt(bsqon::PrimitiveType* bsq_t, SmtFunc fn)
{
}

Result* solvePrimitive(bsqon::PrimitiveType* bsq_t, SmtFunc fn)
{
    auto tk = bsq_t->tkey;
    if(tk == "Int" || fn.sort.is_int()) {
        // auto res = solveInt(fn);
    }
    else if(tk == "Bool" || fn.sort.is_bool()) {
        // auto res = solveBool(fn);
    }
    else if(tk == "CString" && fn.sort.is_seq()) {
        // auto res = solveCString(fn);
    }
    else if(tk == "String" && fn.sort.is_seq()) {
        // auto res = solveString(fn);
    }

    return NULL;
}

z3::expr solveAccessors(SmtFunc acc, size_t total)
{
    acc.s.push();

    Result* res = solveValue(NULL, acc);

    auto rr = acc.s.check();
    acc.s.pop();
    if(rr == z3::sat) {
    }
}

Result* solveEntity(bsqon::StdEntityType* bsq_t, SmtFunc fn)
{
    //-----------NEW Stack
    fn.s.push();

    // uint size = fn.sort.constructors().size();
    z3::func_decl_vector constructs = fn.decl.range().constructors();

    for(size_t i = 0; i < constructs.size(); i++) {
        z3::func_decl c = constructs[i];
        z3::expr_vector args(fn.s.ctx());
        size_t args_len = c.accessors().size();

        for(size_t j = 0; j < args_len; j++) {
            // Should be z3::expr here.
            SmtFunc acc = {
                .s = fn.s,
                .decl = c, // NOTE: This field is not used in solveValue the next time.
                .sort = c.domain(j),
            };
            z3::expr try_arg = solveAccessors(acc, args_len);
            args.push_back(try_arg);
        }
        fn.s.add(c(args));
        fn.s.add(fn.decl() == c());
    }

    z3::check_result rr = fn.s.check();
    //-----------DEL Stack
    fn.s.pop();

    if(rr == z3::sat) {
    }
    else {
    }
}

Result* solveValue(bsqon::Type* bsq_t, SmtFunc fn)
{
    Result* res;

    if(isDatatype(bsq_t, fn)) {
        res = solveEntity(static_cast<bsqon::StdEntityType*>(bsq_t), fn);
    }
    else if(isPrimitive(bsq_t, fn)) {
        res = solvePrimitive(static_cast<bsqon::PrimitiveType*>(bsq_t), fn);
    }

    return res;
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
    // Find Type in asm_info
    bsqon::Type* bsq_t = asm_info.lookupTypeKey(tar_t);
    if(bsq_t == nullptr) {
        badArgs("Unable to find TypeKey");
    }
    // Find for const in smt.
    auto bsq_func = getFuncDecl(bsq_t, s);

    SmtFunc fn = {
        .s = s,
        .decl = bsq_func.value(),
        .sort = bsq_func.value().range(),
    };

    // TODO: Make it return Value*. At the moment just string.
    bsqon::Value* value = solveValue(bsq_t, fn);
};
