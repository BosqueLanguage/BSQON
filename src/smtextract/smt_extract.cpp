#include "smt_utils.h"
#include <cstdio>
#include <vector>
#include "smt_extract.h"

bsqon::Value* checkValidEval(const bsqon::PrimitiveType* bsq_t, z3::expr ex)
{
    auto tk = bsq_t->tkey;
    if(ex.is_int() && (tk == "Int")) {
        int64_t eval_int = ex.get_numeral_int64();
        return new bsqon::IntNumberValue(bsq_t, bsqon::SourcePos{0, 0, 0, 0}, eval_int);
    }
    return NULL;
}

bsqon::Value* ValueSolver::solveInt(const bsqon::PrimitiveType* bsq_t, z3::expr ex)
{
    // Check Z3 interpretation
    bsqon::Value* interp = checkValidEval(bsq_t, this->s.get_model().eval(ex, true));
    if(interp != NULL) {
        return interp;
    }

    // Vector SAT value search
    std::vector<int> choices = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 255};
    bsqon::IntNumberValue* res;

    z3::expr_vector args(this->s.ctx());

    for(size_t i = 0; i < choices.size(); i++) {
        this->s.push();
        z3::expr int_tmp = this->s.ctx().int_val(choices.at(i));

        this->s.add(ex == int_tmp);

        z3::check_result rr = this->s.check();
        this->s.pop();

        if(rr == z3::sat) {
            res = new bsqon::IntNumberValue(bsq_t, bsqon::SourcePos{0, 0, 0, 0}, int64_t(choices.at(i)));
            return res;
        }
    }

    // SAT Bin Search for all possible values.
    int min = BSQ_INT_MIN;
    int max = BSQ_INT_MAX;

    while(min < max) {
        int mid = (max / 2) + (min / 2) + (((max % 2) + (min % 2)) / 2);
        std::cout << "TRYING--->" << mid << "\n";
        this->s.push();

        z3::expr int_tmp = this->s.ctx().int_val(mid);
        this->s.add(ex <= int_tmp);

        z3::check_result rr = this->s.check();
        this->s.pop();

        if(rr == z3::check_result::sat) {
            max = mid;
        }
        else if(rr == z3::check_result::unsat) {
            min = mid + 1;
        }
        else {
            return NULL;
        }
    }

    int64_t search_int = min;
    printf("RETURNING %d\n", int(search_int));
    return new bsqon::IntNumberValue(bsq_t, bsqon::SourcePos{0, 0, 0, 0}, search_int);
}

bsqon::Value* ValueSolver::solvePrimitive(bsqon::PrimitiveType* bsq_t, z3::expr ex)
{
    auto tk = bsq_t->tkey;
    if(tk == "Int") {
        // printf("Solving Int\n");
        return solveInt(bsq_t, ex);
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

bsqon::Value* ValueSolver::solveEntity(bsqon::StdEntityType* bsq_t, z3::expr ex)
{

    z3::func_decl_vector constructs = ex.get_sort().constructors();
    if(constructs.size() > 1) {
        printf("Something really bad happened in Bosque\n");
        exit(1);
    }

    z3::func_decl c = constructs[0];
    z3::func_decl_vector c_accs = c.accessors();

    z3::expr_vector fields(this->s.ctx());
    std::vector<bsqon::Value*> fieldvalues;

    for(size_t j = 0; j < c_accs.size(); j++) {
        bsqon::EntityTypeFieldEntry field_tk = bsq_t->fields[j];
        bsqon::Type* field_t = this->asm_info->lookupTypeKey(field_tk.ftype);

        z3::expr field_acc = c_accs[j](ex);
        bsqon::Value* field_val = this->solveValue(field_t, field_acc);
        if(field_val == NULL) {
            printf("GOT NULL: From solveInt\n");
            exit(1);
        }

        z3::expr field_ex = getExprFromVal(field_val);

        fields.push_back(field_ex);
        fieldvalues.push_back(field_val);
    }

    this->s.add(ex == c(fields));
    z3::check_result rr = this->s.check();

    if(rr == z3::sat) {
        return new bsqon::EntityValue(bsq_t, bsqon::SourcePos{0, 0, 0, 0}, std::move(fieldvalues));
    }
    else {
        return NULL;
    }
}

bsqon::Value* ValueSolver::solveValue(bsqon::Type* bsq_t, z3::expr ex)
{
    bsqon::Value* res;

    if(bsq_t->tag == bsqon::TypeTag::TYPE_STD_ENTITY) {
        res = solveEntity(static_cast<bsqon::StdEntityType*>(bsq_t), ex);
    }
    else if(bsq_t->tag == bsqon::TypeTag::TYPE_PRIMITIVE) {
        res = solvePrimitive(static_cast<bsqon::PrimitiveType*>(bsq_t), ex);
    }

    return res;
}

ValueSolver::ValueSolver(bsqon::AssemblyInfo* asm_info, bsqon::Type* bsq_t, z3::solver& solver)
    : asm_info(asm_info), bsq_t(bsq_t), s(solver), fn([&]() {
          auto tmp = getFuncDecl(bsq_t, solver);
          if(!tmp.has_value()) {
              std::cout << "Provided type " << bsq_t->tkey << " not found in .smt2 file" << "\n";
              exit(1);
          }
          return tmp.value();
      }())
{
    ;
}
