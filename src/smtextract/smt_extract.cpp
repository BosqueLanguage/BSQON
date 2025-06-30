#include <cstdio>
#include <cstring>
#include <optional>
#include <vector>
#include "smt_extract.h"
#include "smt_utils.h"

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

    return nullptr;
}

// TODO: Make this us the Z3 Interp and Binary Search
z3::expr ValueSolver::FindStringLen(z3::expr ex)
{
    std::vector<int> choices = {0, 1, 2, 10, 255};
    z3::expr result = this->s.ctx().int_val(0);

    for(int i : choices) {
        this->s.push();
        z3::expr len_tmp = this->s.ctx().int_val(i);

        this->s.add(ex.length() == len_tmp);

        z3::check_result rr = this->s.check();
        this->s.pop();
        if(rr == z3::sat) {
            result = len_tmp;
            return result;
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
    int found_str_len = str_len.get_numeral_int();

    for(int i = 0; i < found_str_len; i++) {
        z3::expr index = this->s.ctx().int_val(i);
        std::optional<char> sat_char = BinSearchChar(ex, index, ASCII_MIN, ASCII_MAX);

        if(sat_char.has_value()) {
            str_tmp += sat_char.value();
        }
        else {
            str_tmp.append("{}");
        }
    }

    return bsqon::CStringValue::createFromGenerator(bsq_t, bsqon::SourcePos{0, 0, 0, 0}, str_tmp);
}

bsqon::Value* ValueSolver::solveBool(const bsqon::PrimitiveType* bsq_t, z3::expr ex)
{
    this->s.push();

    this->s.add(ex);
    z3::check_result rr = this->s.check();

    this->s.pop();

    if(rr == z3::sat) {
        this->s.add(ex);
        return new bsqon::BoolValue(bsq_t, bsqon::SourcePos{0, 0, 0, 0}, true);
    }
    else {
        z3::expr alt = this->s.ctx().bool_val(false);
        this->s.add(ex == alt);
        return new bsqon::BoolValue(bsq_t, bsqon::SourcePos{0, 0, 0, 0}, false);
    }
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
    std::vector<uint> choices = {0, 1, 2, 10, 255};
    bsqon::BigNatNumberValue* res;

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

        if(rr == z3::sat) {
            max = mid;
        }
        else if(rr == z3::unsat || rr == z3::unknown) {
            min = mid + 1;
        }
    }

    u_int64_t search_int = min;
    z3::expr int_expr = this->s.ctx().int_val(search_int);

    this->s.add(ex == int_expr);

    return new bsqon::BigNatNumberValue(bsq_t, bsqon::SourcePos{0, 0, 0, 0}, search_int);
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
    std::vector<uint> choices = {0, 1, 2, 10, 255};
    bsqon::NatNumberValue* res;

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
    return new bsqon::NatNumberValue(bsq_t, bsqon::SourcePos{0, 0, 0, 0}, search_int);
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
    std::vector<int> choices = {0, 1, 2, 10, 255};
    bsqon::BigIntNumberValue* res;

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
    return new bsqon::BigIntNumberValue(bsq_t, bsqon::SourcePos{0, 0, 0, 0}, search_int);
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
    std::vector<int> choices = {0, 1, 2, 10, 255};
    bsqon::IntNumberValue* res;

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
    return new bsqon::IntNumberValue(bsq_t, bsqon::SourcePos{0, 0, 0, 0}, search_int);
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

    return nullptr;
}

bsqon::Value* ValueSolver::solveEntity(bsqon::StdEntityType* bsq_t, z3::expr ex)
{

    z3::func_decl_vector constructs = ex.get_sort().constructors();
    assert(constructs.size() <= 1);

    z3::func_decl c = constructs[0];
    z3::func_decl_vector c_accs = c.accessors();

    std::vector<bsqon::Value*> fieldvalues;

    for(size_t j = 0; j < c_accs.size(); j++) {
        bsqon::EntityTypeFieldEntry field_tk = bsq_t->fields[j];
        bsqon::Type* field_t = this->asm_info->lookupTypeKey(field_tk.ftype);

        z3::expr field_acc = c_accs[j](ex);
        try {
            bsqon::Value* field_val = this->solveValue(field_t, field_acc);
            fieldvalues.push_back(field_val);
        }
        catch(const std::exception& e) {
            std::cerr << "ERR: " << e.what() << "\n";
        }
    }

    return new bsqon::EntityValue(bsq_t, bsqon::SourcePos{0, 0, 0, 0}, std::move(fieldvalues));
}

// ex.get_sort().constructors()[1].accessors()[0].range().constructors()[0].range().constructors()[0].accessors();
bsqon::Value* ValueSolver::solveSome(bsqon::SomeType* bsq_t, z3::expr ex)
{
    //"ex" should be at this point a primitive sort.
    z3::sort some_sort = ex.get_sort();
    z3::func_decl some_construct = some_sort.constructors()[0];
    std::cout << some_construct << "\n";

    z3::func_ecl some_accessor = some_construct.accessors()[0];
    std::cout << some_accessor << "\n";

    bsqon::Type* some_t = this->asm_info->lookupTypeKey(bsq_t->oftype);
    bsqon::Value* some_val = nullptr;
    try {
        some_val = this->solveValue(some_t, ex);
    }
    catch(const std::exception& e) {
        std::cerr << "ERR: " << e.what() << "\n";
    }

    return new bsqon::SomeValue(bsq_t, bsqon::SourcePos{0, 0, 0, 0}, some_val);
}

bsqon::Value* ValueSolver::solveOption(bsqon::OptionType* bsq_t, z3::expr ex)
{
    z3::func_decl_vector terms = ex.get_sort().constructors();
    const std::vector<bsqon::TypeKey>& subtypes = this->asm_info->concreteSubtypesMap.at(bsq_t->tkey);

    // Solving if Option is None.
    // this->s.push();
    // z3::func_decl fn_none = findConstruct(terms, tKeyToSmtName("None", NONE).value());
    // z3::expr none_expr = fn_none;
    //
    // this->s.add(ex == none_expr);
    // z3::check_result res_none = this->s.check();
    //
    // this->s.pop();
    //
    // if(res_none == z3::sat) {
    //     this->s.add(ex == none_expr);
    //     return new bsqon::NoneValue(bsq_t, bsqon::SourcePos{0, 0, 0, 0});
    // }

    // Solving if Option is Some.

    // this->s.add();
    //(declare-fun @Term-Some<Int>-mk (Some<Int>) @Term)
    std::optional<z3::func_decl> some_mk =
        findConstruct(terms, tKeyToSmtName(subtypes.at(0), STRUCT_TERM_CONSTRUCT).value());

    // Some<Int>
    z3::sort some_sort = some_mk.value().accessors()[0].range();
    z3::symbol some_sym = this->s.ctx().str_symbol("tmp_some");
    z3::expr some_unin = this->s.ctx().constant(some_sym, some_sort);

    z3::expr some_mk_expr = some_mk.value()(some_unin);

    // Should make some_mk == to whatever the SAT for some_mk_expr be.
    // this->s.add();
    Value* some_val = this->solveSome(dynamic_cast<bsqon::SomeType*>(bsq_t), some_val_expr);
};

bsqon::Value* ValueSolver::solveConcept(bsqon::ConceptType* bsq_t, z3::expr ex)
{
    auto tg = bsq_t->tag;
    if(tg == bsqon::TypeTag::TYPE_OPTION) {
        return solveOption(static_cast<bsqon::OptionType*>(bsq_t), ex);
    }

    return nullptr;
}

bsqon::Value* ValueSolver::solveValue(bsqon::Type* bsq_t, z3::expr ex)
{

    auto tg = bsq_t->tag;
    if(tg == bsqon::TypeTag::TYPE_STD_ENTITY) {
        return solveEntity(static_cast<bsqon::StdEntityType*>(bsq_t), ex);
    }
    else if(tg == bsqon::TypeTag::TYPE_PRIMITIVE) {
        return solvePrimitive(static_cast<bsqon::PrimitiveType*>(bsq_t), ex);
    }
    else if(tg == bsqon::TypeTag::TYPE_STD_CONCEPT || tg == bsqon::TypeTag::TYPE_OPTION) {
        return solveConcept(static_cast<bsqon::ConceptType*>(bsq_t), ex);
    }

    return nullptr;
}

// Use Type* to find the func_decl in the z3::model.
std::optional<z3::expr> getBsqTypeExpr(std::string target, z3::solver& s)
{
    if(s.check() == z3::sat) {
        z3::model m = s.get_model();
        for(uint i = 0; i < m.num_consts(); ++i) {
            z3::func_decl fn = m.get_const_decl(i);
            if(fn.name().str() == target) {
                return fn();
            }
        }
    }

    return std::nullopt;
}

ValueSolver::ValueSolver(bsqon::AssemblyInfo* asm_info, std::string target, z3::solver& solver)
    : asm_info(asm_info), s(solver), ex([&]() {
          auto tmp = getBsqTypeExpr(target, solver);
          if(!tmp.has_value()) {
              std::cout << "Const not found in not found in .smt2 file" << "\n";
              exit(1);
          }
          return tmp.value();
      }())
{
    ;
}
