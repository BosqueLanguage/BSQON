#include "smt_utils.h"
#include <cstddef>
#include <cstdio>
#include <string.h>
#include <cstdlib>
#include <string.h>
#include <cstring>
#include <fstream>
#include <string>
#include <vector>
#include <z3_api.h>

bsqon::IntNumberValue* ValueSolver::solveInt(const bsqon::PrimitiveType* bsq_t, z3::func_decl fn)
{
    std::vector<int> choices = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    bsqon::IntNumberValue* res;

    z3::expr_vector args(this->s.ctx());
    // NOTE:We do not care about the input just the ouput for here.
    initArgs(args, fn.arity(), fn, this->s);

    for(size_t i = 0; i < choices.size(); i++) {
        this->s.push();
        z3::expr int_tmp = this->s.ctx().int_val(i);

        this->s.add(fn(args) == int_tmp);

        z3::check_result rr = this->s.check();
        this->s.pop();

        // TODO: Suspicious unsat ...
        if(rr == z3::sat) {
            res = new bsqon::IntNumberValue(bsq_t, bsqon::SourcePos{0, 0, 0, 0}, int64_t(i));
            return res;
        }
    }

    return NULL;
}

bsqon::Value* ValueSolver::solvePrimitive(bsqon::PrimitiveType* bsq_t, z3::func_decl fn)
{
    auto tk = bsq_t->tkey;
    if(tk == "Int") {
        // printf("Solving Int\n");
        return solveInt(bsq_t, fn);
    }
    else if(tk == "Bool") {
        printf("Solving Bool\n");
        // res = solveBool(fn);
    }
    else if(tk == "CString") {
        printf("Solving CString\n");
        // return  solveCString(fn);
    }
    else if(tk == "String") {
        printf("Solving String\n");
        // return solveString(fn);
    }

    return NULL;
}

bsqon::Value* ValueSolver::solveEntity(bsqon::StdEntityType* bsq_t, z3::func_decl fn)
{

    z3::func_decl_vector constructs = fn.range().constructors();
    if(constructs.size() > 1) {
        printf("Something really bad happened in Bosque\n");
        exit(1);
    }

    z3::func_decl c = constructs[0];
    z3::func_decl_vector c_accs = c.accessors();

    z3::expr_vector args(this->s.ctx());
    size_t args_len = c.accessors().size();
    // const std::vector<Value*>&& fieldvalues

    std::vector<bsqon::Value*> fieldvalues;

    this->s.push();
    for(size_t j = 0; j < args_len; j++) {

        bsqon::EntityTypeFieldEntry arg_tk = bsq_t->fields[j];

        bsqon::Type* arg_t = this->asm_info->lookupTypeKey(arg_tk.ftype);

        bsqon::Value* arg_val = this->solveValue(arg_t, c_accs[j]);
        if(arg_val == NULL) {
            printf("GOT NULL: From solveInt");
            exit(1);
        }

        z3::expr arg_expr = getExprFromVal(arg_val);
        // this->s.add(c_accs[j] == sat_expr);
        args.push_back(arg_expr);
        fieldvalues.push_back(arg_val);
    }
    this->s.pop();

    this->s.add(fn() == c(args));
    z3::check_result rr = this->s.check();

    if(rr == z3::sat) {
        return new bsqon::EntityValue(bsq_t, bsqon::SourcePos{0, 0, 0, 0}, std::move(fieldvalues));
    }
    else {
        return NULL;
    }
}

bsqon::Value* ValueSolver::solveValue(bsqon::Type* bsq_t, z3::func_decl fn)
{
    bsqon::Value* res;

    if(bsq_t->tag == bsqon::TypeTag::TYPE_STD_ENTITY) {
        res = solveEntity(static_cast<bsqon::StdEntityType*>(bsq_t), fn);
    }
    else if(bsq_t->tag == bsqon::TypeTag::TYPE_PRIMITIVE) {
        res = solvePrimitive(static_cast<bsqon::PrimitiveType*>(bsq_t), fn);
    }

    return res;
}

ValueSolver::ValueSolver(bsqon::AssemblyInfo* asm_info, bsqon::Type* bsq_t, z3::solver& solver)
    : asm_info(asm_info), bsq_t(bsq_t), s(solver), fn([&]() {
          auto tmp = getFuncDecl(bsq_t, solver);
          if(!tmp.has_value()) {
              printf("Function Declaration not found with provided bosque type\n");
              exit(1);
          }
          return tmp.value();
      }())
{
    ;
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

    ValueSolver sol(&asm_info, bsq_t, s);
    bsqon::Value* result = sol.solveValue(sol.bsq_t, sol.fn);
    if(result == NULL) {
        printf("GOT NULL: From first call of solveValue.\n");
        exit(1);
    }
    std::u8string rstr = result->toString();
    printf("%s\n", (const char*)rstr.c_str());
};
