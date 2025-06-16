#include "smt_utils.h"
#include <cstdio>
#include <vector>
#include <z3_api.h>
#include "smt_extract.h"

bsqon::Value* checkValidEval(const bsqon::PrimitiveType* bsq_t, z3::expr ex)
{
    auto tk = bsq_t->tkey;
    if(tk == "Int" && ex.is_int()) {
        int ival;
        ex.is_numeral_i(ival);
        return new bsqon::IntNumberValue(bsq_t, bsqon::SourcePos{0, 0, 0, 0}, ival);
    }
    else if(tk == "Nat" && ex.is_int()) {
        uint nval;
        ex.is_numeral_u(nval);
        return new bsqon::NatNumberValue(bsq_t, bsqon::SourcePos{0, 0, 0, 0}, nval);
    }
    else if(tk == "BigInt" && ex.is_int()) {
        int64_t Ival;
        ex.is_numeral_i64(Ival);
        return new bsqon::BigIntNumberValue(bsq_t, bsqon::SourcePos{0, 0, 0, 0}, Ival);
    }
    else if(tk == "BigNat" && ex.is_int()) {
        uint64_t Nval;
        ex.is_numeral_u64(Nval);
        return new bsqon::BigNatNumberValue(bsq_t, bsqon::SourcePos{0, 0, 0, 0}, Nval);
    }
    else if(tk == "CString" && ex.is_string_value()) {
        std::string csval = ex.get_string();
        return bsqon::CStringValue::createFromGenerator(bsq_t, bsqon::SourcePos{0, 0, 0, 0}, csval);
    }
    else if(tk == "String" && ex.is_string_value()) {
        std::string sval = ex.get_string();
        return bsqon::StringValue::createFromGenerator(bsq_t, bsqon::SourcePos{0, 0, 0, 0}, sval);
    }

    return NULL;
}

// TODO: Make this us the Z3 Interp and Binary Search
z3::expr ValueSolver::FindStringLen(z3::expr ex)
{
    std::vector<int> choices = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    z3::expr result = this->s.ctx().int_val(0);

    for(int i : choices) {
        this->s.push();
        z3::expr len_tmp = this->s.ctx().int_val(i);

        this->s.add(ex.length() == len_tmp);

        z3::check_result rr = this->s.check();
        this->s.pop();
        if(rr == z3::sat) {
            result = len_tmp;
            break;
        }
    }

    // SAT Bin Search for all possible values.
    int min = BSQ_INT_MIN;
    int max = BSQ_INT_MAX;

    while(min < max) {
        int mid = (max / 2) + (min / 2) + (((max % 2) + (min % 2)) / 2);
        this->s.push();

        z3::expr int_tmp = this->s.ctx().int_val(mid);
        this->s.add(ex.length() <= int_tmp);

        z3::check_result rr = this->s.check();
        this->s.pop();

        if(rr == z3::check_result::sat) {
            max = mid;
        }
        else if(rr == z3::check_result::unsat) {
            min = mid + 1;
        }
        else {
            return result;
        }
    }

    int64_t search_int = min;
    result = this->s.ctx().int_val(search_int);

    this->s.add(ex.length() == result);

    return result;
}

std::optional<char> ValueSolver::BinSearchChar(z3::expr str_exp, z3::expr index, int min, int max)
{
    while(min < max) {
        char mid = (max / 2) + (min / 2) + (((max % 2) + (min % 2)) / 2);

        this->s.push();

        Z3_ast ast_char = Z3_mk_char(this->s.ctx(), mid);
        z3::expr char_exp = z3::expr(this->s.ctx(), ast_char);

        this->s.add(str_exp.nth(index).char_to_int() <= char_exp.char_to_int());

        z3::check_result rr = this->s.check();
        this->s.pop();

        if(rr == z3::check_result::sat) {
            max = mid;
        }
        else if(rr == z3::check_result::unsat) {
            min = mid + 1;
        }
        else {
            return std::nullopt;
        }
    }

    char const_char = max;
    Z3_ast ast_char = Z3_mk_char(this->s.ctx(), const_char);
    z3::expr char_exp = z3::expr(this->s.ctx(), ast_char);

    this->s.add(str_exp.nth(index) == char_exp);

    return const_char;
}

bsqon::Value* ValueSolver::solveCString(const bsqon::PrimitiveType* bsq_t, z3::expr ex)
{
    // Check Z3 interpretation z3 expr
    if(this->s.check() == z3::sat) {
        z3::expr z3_eval = this->s.get_model().eval(ex, true);
        bsqon::Value* interp = checkValidEval(bsq_t, z3_eval);
        if(interp != NULL) {
            return interp;
        }
    }

    std::string str_tmp("");

    z3::expr str_len = FindStringLen(ex);

    for(int i = 0; i < str_len.get_numeral_int(); i++) {
        z3::expr index = this->s.ctx().int_val(i);
        std::optional<char> sat_char = BinSearchChar(ex, index, ASCII_MIN, ASCII_MAX);

        if(sat_char.has_value()) {
            str_tmp += sat_char.value();
        }
        else {
            str_tmp.append("{}");
        }
    }

    this->s.push();
    z3::expr str_expr = this->s.ctx().string_val(str_tmp);
    this->s.add(ex == str_expr);
    z3::check_result rr = this->s.check();
    this->s.pop();

    if(rr == z3::sat) {
        this->s.add(ex == str_expr);
        return bsqon::CStringValue::createFromGenerator(bsq_t, bsqon::SourcePos{0, 0, 0, 0}, str_tmp);
    }

    return NULL;
}

bsqon::Value* ValueSolver::solveBool(const bsqon::PrimitiveType* bsq_t, z3::expr ex)
{
    // Check Z3 interpretation z3 expr
    if(this->s.check() == z3::sat) {
        z3::expr z3_eval = this->s.get_model().eval(ex, true);
        bsqon::Value* interp = checkValidEval(bsq_t, z3_eval);
        if(interp != NULL) {
            return interp;
        }
    }

    std::vector<bool> choices = {true, false};

    z3::expr_vector args(this->s.ctx());

    for(size_t i = 0; i < choices.size(); i++) {
        this->s.push();
        z3::expr bool_tmp = this->s.ctx().bool_val(choices.at(i));

        this->s.add(ex == bool_tmp);

        z3::check_result rr = this->s.check();
        this->s.pop();

        if(rr == z3::sat) {
            this->s.add(ex == bool_tmp);
            return new bsqon::BoolValue(bsq_t, bsqon::SourcePos{0, 0, 0, 0}, choices.at(i));
        }
    }
    return NULL;
}

bsqon::Value* ValueSolver::solveBigNat(const bsqon::PrimitiveType* bsq_t, z3::expr ex)
{
    // Check Z3 interpretation z3 expr
    if(this->s.check() == z3::sat) {
        z3::expr z3_eval = this->s.get_model().eval(ex, true);
        bsqon::Value* interp = checkValidEval(bsq_t, z3_eval);
        if(interp != NULL) {
            return interp;
        }
    }

    // Vector SAT value search
    std::vector<uint> choices = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 255};
    bsqon::BigNatNumberValue* res;

    z3::expr_vector args(this->s.ctx());

    for(size_t i = 0; i < choices.size(); i++) {
        this->s.push();
        z3::expr int_tmp = this->s.ctx().int_val(choices.at(i));

        this->s.add(ex == int_tmp);

        z3::check_result rr = this->s.check();
        this->s.pop();

        if(rr == z3::sat) {
            this->s.add(ex == int_tmp);
            res = new bsqon::BigNatNumberValue(bsq_t, bsqon::SourcePos{0, 0, 0, 0}, uint64_t(choices.at(i)));
            return res;
        }
    }

    // SAT Bin Search for all possible values.
    int min = BSQ_BIGNAT_MIN;
    int max = BSQ_BIGNAT_MAX;

    while(min < max) {
        uint mid = (max / 2) + (min / 2) + (((max % 2) + (min % 2)) / 2);
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

    u_int64_t search_int = min;
    z3::expr int_expr = this->s.ctx().int_val(search_int);

    this->s.add(ex == int_expr);
    z3::check_result rr = this->s.check();
    if(rr == z3::sat) {
        return new bsqon::BigNatNumberValue(bsq_t, bsqon::SourcePos{0, 0, 0, 0}, search_int);
    }

    return NULL;
}

bsqon::Value* ValueSolver::solveNat(const bsqon::PrimitiveType* bsq_t, z3::expr ex)
{
    // Check Z3 interpretation z3 expr
    if(this->s.check() == z3::sat) {
        z3::expr z3_eval = this->s.get_model().eval(ex, true);
        bsqon::Value* interp = checkValidEval(bsq_t, z3_eval);
        if(interp != NULL) {
            return interp;
        }
    }

    // Vector SAT value search
    std::vector<uint> choices = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 255};
    bsqon::NatNumberValue* res;

    z3::expr_vector args(this->s.ctx());

    for(size_t i = 0; i < choices.size(); i++) {
        this->s.push();
        z3::expr int_tmp = this->s.ctx().int_val(choices.at(i));

        this->s.add(ex == int_tmp);

        z3::check_result rr = this->s.check();
        this->s.pop();

        if(rr == z3::sat) {
            this->s.add(ex == int_tmp);
            res = new bsqon::NatNumberValue(bsq_t, bsqon::SourcePos{0, 0, 0, 0}, uint64_t(choices.at(i)));
            return res;
        }
    }

    // SAT Bin Search for all possible values.
    int min = BSQ_NAT_MIN;
    int max = BSQ_NAT_MAX;

    while(min < max) {
        uint mid = (max / 2) + (min / 2) + (((max % 2) + (min % 2)) / 2);
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

    u_int64_t search_int = min;
    z3::expr int_expr = this->s.ctx().int_val(search_int);

    this->s.add(ex == int_expr);
    z3::check_result rr = this->s.check();
    if(rr == z3::sat) {
        return new bsqon::NatNumberValue(bsq_t, bsqon::SourcePos{0, 0, 0, 0}, search_int);
    }

    return NULL;
}

bsqon::Value* ValueSolver::solveBigInt(const bsqon::PrimitiveType* bsq_t, z3::expr ex)
{
    // Check Z3 interpretation z3 expr
    if(this->s.check() == z3::sat) {
        z3::expr z3_eval = this->s.get_model().eval(ex, true);
        bsqon::Value* interp = checkValidEval(bsq_t, z3_eval);
        if(interp != NULL) {
            return interp;
        }
    }

    // Vector SAT value search
    std::vector<int> choices = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 255};
    bsqon::BigIntNumberValue* res;

    z3::expr_vector args(this->s.ctx());

    for(size_t i = 0; i < choices.size(); i++) {
        this->s.push();
        z3::expr int_tmp = this->s.ctx().int_val(choices.at(i));

        this->s.add(ex == int_tmp);

        z3::check_result rr = this->s.check();
        this->s.pop();

        if(rr == z3::sat) {
            this->s.add(ex == int_tmp);
            res = new bsqon::BigIntNumberValue(bsq_t, bsqon::SourcePos{0, 0, 0, 0}, int64_t(choices.at(i)));
            return res;
        }
    }

    // SAT Bin Search for all possible values.
    int min = BSQ_BIGINT_MIN;
    int max = BSQ_BIGINT_MAX;

    while(min < max) {
        int mid = (max / 2) + (min / 2) + (((max % 2) + (min % 2)) / 2);
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
    z3::expr int_expr = this->s.ctx().int_val(search_int);

    this->s.add(ex == int_expr);
    z3::check_result rr = this->s.check();
    if(rr == z3::sat) {
        return new bsqon::BigIntNumberValue(bsq_t, bsqon::SourcePos{0, 0, 0, 0}, search_int);
    }

    return NULL;
}

bsqon::Value* ValueSolver::solveInt(const bsqon::PrimitiveType* bsq_t, z3::expr ex)
{
    // Check Z3 interpretation z3 expr
    if(this->s.check() == z3::sat) {
        z3::expr z3_eval = this->s.get_model().eval(ex, true);
        bsqon::Value* interp = checkValidEval(bsq_t, z3_eval);
        if(interp != NULL) {
            return interp;
        }
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
            this->s.add(ex == int_tmp);
            res = new bsqon::IntNumberValue(bsq_t, bsqon::SourcePos{0, 0, 0, 0}, int64_t(choices.at(i)));
            return res;
        }
    }

    // SAT Bin Search for all possible values.
    int min = BSQ_INT_MIN;
    int max = BSQ_INT_MAX;

    while(min < max) {
        int mid = (max / 2) + (min / 2) + (((max % 2) + (min % 2)) / 2);
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
    z3::expr int_expr = this->s.ctx().int_val(search_int);

    this->s.add(ex == int_expr);
    z3::check_result rr = this->s.check();
    if(rr == z3::sat) {
        return new bsqon::IntNumberValue(bsq_t, bsqon::SourcePos{0, 0, 0, 0}, search_int);
    }

    return NULL;
}

bsqon::Value* ValueSolver::solvePrimitive(bsqon::PrimitiveType* bsq_t, z3::expr ex)
{
    auto tk = bsq_t->tkey;
    if(tk == "Int") {
        return solveInt(bsq_t, ex);
    }
    else if(tk == "BigInt") {
        return solveBigInt(bsq_t, ex);
    }
    else if(tk == "Nat") {
        return solveNat(bsq_t, ex);
    }
    else if(tk == "BigNat") {
        return solveBigNat(bsq_t, ex);
    }
    else if(tk == "Bool") {
        return solveBool(bsq_t, ex);
    }
    else if(tk == "CString") {
        return solveCString(bsq_t, ex);
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
            std::cout << "EXPR" << field_acc << "\n";
            exit(1);
        }

        std::optional<z3::expr> field_ex = getExprFromVal(field_val);
        if(field_ex.has_value() == false) {
            printf("getExprFromVal Failed\n");
            return NULL;
        }

        fields.push_back(field_ex.value());
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

    if(bsq_t->tag == bsqon::TypeTag::TYPE_STD_ENTITY) {
        return solveEntity(static_cast<bsqon::StdEntityType*>(bsq_t), ex);
    }
    else if(bsq_t->tag == bsqon::TypeTag::TYPE_PRIMITIVE) {
        return solvePrimitive(static_cast<bsqon::PrimitiveType*>(bsq_t), ex);
    }

    return NULL;
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
