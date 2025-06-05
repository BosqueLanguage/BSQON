#include "smt_utils.h"
#include "smt_extract.h"

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
