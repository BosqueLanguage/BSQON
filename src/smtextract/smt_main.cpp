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

bsqon::IntNumberValue* solveInt(bsqon::PrimitiveType* bsq_t, SmtFunc fn)
{
    std::vector<int> choices = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    bsqon::IntNumberValue* res;

    for(int i : choices) {
        fn.s.push();
        z3::expr int_tmp = fn.s.ctx().int_val(i);

        fn.s.add(fn.decl() == int_tmp);

        z3::check_result rr = fn.s.check();
        fn.s.pop();

        if(rr == z3::sat) {
            res = new bsqon::IntNumberValue();
            return res;
        }
    }

    return NULL;
}

bsqon::Value* solvePrimitive(bsqon::PrimitiveType* bsq_t, SmtFunc fn)
{
    bsqon::Value* res;
    auto tk = bsq_t->tkey;
    if(tk == "Int" || fn.sort.is_int()) {
        printf("Solving Int\n");
        res = solveInt(bsq_t, fn);
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

bsqon::Value* solveEntity(bsqon::StdEntityType* bsq_t, SmtFunc fn)
{
    // TODO: Only handles one constructor at the moment.
    fn.s.push();

    // uint size = fn.sort.constructors().size();
    z3::func_decl_vector constructs = fn.decl.range().constructors();

    if(constructs.size() > 1) {
        // Not Possible
    }
    z3::func_decl c = constructs[0];

    z3::expr_vector args(fn.s.ctx());
    size_t args_len = c.accessors().size();
    initArgs(args, args_len, c, fn.s);

    for(size_t j = 0; j < args_len; j++) {
        fn.s.push();

        fn.s.add(args);
        //  Should be z3::expr here.
        SmtFunc acc = {
            .s = fn.s,
            .decl = c,
            .sort = c.domain(j),
        };

        bsqon::EntityTypeFieldEntry* arg_tk = bsq_t->fields[j];
        std::cout << arg_tk->ftype << "\n";

        bsqon::Value* sat_arg = solveValue(arg_tk, acc);
        // args[j] = sat_arg.expr;
        // args[j] = bsqonValueToExpr(try_arg);

        fn.s.pop();
    }

    fn.s.add(fn.decl() == c(args));
    z3::check_result rr = fn.s.check();

    fn.s.pop();

    if(rr == z3::sat) {
        return NULL;
    }
    else {
        return NULL;
    }
}

bsqon::Value* solveValue(bsqon::Type* bsq_t, SmtFunc fn)
{
    bsqon::Value* res;

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
    ValueSolver sol = {
        .asm_info = asm_info,
        .s = s,
        .decl = bsq_func.value(),
        .sort = bsq_func.value().range(),
    };

    // TODO: Should assert that result here should be value.
    // bsqon::Value* value =
    solveValue(bsq_t, fn);
    // std::cout << value->val->toString() << "\n";
};
