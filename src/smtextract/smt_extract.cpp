#include <cstdio>
#include <cstring>
#include <optional>
#include <vector>
#include "smt_extract.h"
#include "smt_utils.h"

#define FILLER_POS bsqon::SourcePos{0, 0, 0, 0}

bsqon::Value* checkValidEval(const bsqon::PrimitiveType* bsq_t, z3::expr ex)
{
    auto tk = bsq_t->tkey;
    if(tk == "Int" && ex.is_int()) {
        int ival;
        ex.is_numeral_i(ival);
        return new bsqon::IntNumberValue(bsq_t, FILLER_POS, ival);
    }
    else if(tk == "Nat" && ex.is_int()) {
        uint nval;
        ex.is_numeral_u(nval);
        return new bsqon::NatNumberValue(bsq_t, FILLER_POS, nval);
    }
    else if(tk == "BigInt" && ex.is_int()) {
        int64_t Ival;
        ex.is_numeral_i64(Ival);
        return new bsqon::BigIntNumberValue(bsq_t, FILLER_POS, Ival);
    }
    else if(tk == "BigNat" && ex.is_int()) {
        uint64_t Nval;
        ex.is_numeral_u64(Nval);
        return new bsqon::BigNatNumberValue(bsq_t, FILLER_POS, Nval);
    }
    else if(tk == "CString" && ex.is_string_value()) {
        std::string csval = ex.get_string();
        return bsqon::CStringValue::createFromGenerator(bsq_t, FILLER_POS, csval);
    }
    else if(tk == "String" && ex.is_string_value()) {
        std::string sval = ex.get_string();
        return bsqon::StringValue::createFromGenerator(bsq_t, FILLER_POS, sval);
    }

    return nullptr;
}

z3::expr ValueSolver::extractSequenceLen(z3::expr ex)
{
    // Check Z3 interpretation z3 expr
    if(this->s.check() == z3::sat) {
        z3::expr z3_eval = this->s.get_model().eval(ex.length(), true);
        if(z3_eval.is_int()) {
            return z3_eval;
        }
    }

    std::vector<int> choices = {0, 1, 2, 10, 255};
    z3::expr result = this->s.ctx().int_val(0);

    for(int i : choices) {
        this->s.push();
        z3::expr len_tmp = this->s.ctx().int_val(i);

        this->s.add(ex.length() == len_tmp);

        z3::check_result rr = this->s.check();
        this->s.pop();
        if(rr == z3::sat) {
            this->s.add(ex.length() == len_tmp);
            result = len_tmp;
            return result;
        }
    }

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

bsqon::Value* ValueSolver::extractCString(const bsqon::PrimitiveType* bsq_t, z3::expr ex)
{
    // // Check Z3 interpretation z3 expr
    if(this->s.check() == z3::sat) {
        z3::expr z3_eval = this->s.get_model().eval(ex, true);
        bsqon::Value* interp = checkValidEval(bsq_t, z3_eval);
        if(interp != NULL) {
            return interp;
        }
    }

    std::string str_tmp("");

    z3::expr str_len = extractSequenceLen(ex);
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

    return bsqon::CStringValue::createFromGenerator(bsq_t, FILLER_POS, str_tmp);
}

bsqon::Value* ValueSolver::extractBool(const bsqon::PrimitiveType* bsq_t, z3::expr ex)
{
    this->s.push();

    this->s.add(ex);
    z3::check_result rr = this->s.check();

    this->s.pop();

    if(rr == z3::sat) {
        this->s.add(ex);
        return new bsqon::BoolValue(bsq_t, FILLER_POS, true);
    }
    else {
        z3::expr alt = this->s.ctx().bool_val(false);
        this->s.add(ex == alt);
        return new bsqon::BoolValue(bsq_t, FILLER_POS, false);
    }
}

bsqon::Value* ValueSolver::extractBigNat(const bsqon::PrimitiveType* bsq_t, z3::expr ex)
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
            res = new bsqon::BigNatNumberValue(bsq_t, FILLER_POS, uint64_t(choices.at(i)));
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

    return new bsqon::BigNatNumberValue(bsq_t, FILLER_POS, search_int);
}

bsqon::Value* ValueSolver::extractNat(const bsqon::PrimitiveType* bsq_t, z3::expr ex)
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
            res = new bsqon::NatNumberValue(bsq_t, FILLER_POS, uint64_t(choices.at(i)));
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
    return new bsqon::NatNumberValue(bsq_t, FILLER_POS, search_int);
}

bsqon::Value* ValueSolver::extractBigInt(const bsqon::PrimitiveType* bsq_t, z3::expr ex)
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
            res = new bsqon::BigIntNumberValue(bsq_t, FILLER_POS, int64_t(choices.at(i)));
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
    return new bsqon::BigIntNumberValue(bsq_t, FILLER_POS, search_int);
}

bsqon::Value* ValueSolver::extractInt(const bsqon::PrimitiveType* bsq_t, z3::expr ex)
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
            res = new bsqon::IntNumberValue(bsq_t, FILLER_POS, int64_t(choices.at(i)));
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
    return new bsqon::IntNumberValue(bsq_t, FILLER_POS, search_int);
}

bsqon::Value* ValueSolver::extractPrimitive(bsqon::PrimitiveType* bsq_t, z3::expr ex)
{
    auto tk = bsq_t->tkey;
    if(tk == "Int") {
        return extractInt(bsq_t, ex);
    }
    else if(tk == "BigInt") {
        return extractBigInt(bsq_t, ex);
    }
    else if(tk == "Nat") {
        return extractNat(bsq_t, ex);
    }
    else if(tk == "BigNat") {
        return extractBigNat(bsq_t, ex);
    }
    else if(tk == "Bool") {
        return extractBool(bsq_t, ex);
    }
    else if(tk == "CString") {
        return extractCString(bsq_t, ex);
    }

    return nullptr;
}

bsqon::Value* ValueSolver::extractEntity(bsqon::StdEntityType* bsq_t, z3::expr ex)
{
    z3::func_decl_vector constructs = ex.get_sort().constructors();
    assert(constructs.size() == 1);

    z3::func_decl c = constructs[0];
    z3::func_decl_vector c_accs = c.accessors();

    std::vector<bsqon::Value*> fieldvalues;

    for(size_t j = 0; j < c_accs.size(); j++) {
        bsqon::EntityTypeFieldEntry field_tk = bsq_t->fields[j];
        bsqon::Type* field_t = this->asm_info->lookupTypeKey(field_tk.ftype);

        z3::expr field_acc = c_accs[j](ex);
        try {
            bsqon::Value* field_val = this->extractValue(field_t, field_acc);
            fieldvalues.push_back(field_val);
        }
        catch(const std::exception& e) {
            std::cerr << "ERR: " << e.what() << "\n";
        }
    }

    return new bsqon::EntityValue(bsq_t, FILLER_POS, std::move(fieldvalues));
}

// ex.get_sort().constructors()[1].accessors()[0].range().constructors()[0].range().constructors()[0].accessors();
bsqon::Value* ValueSolver::extractSome(bsqon::SomeType* bsq_t, z3::expr ex)
{
    std::cout << "Triggered err?" << ex << "\n";
    z3::sort some_sort = ex.get_sort();

    //(declare-fun Some<Int>-mk (Int) Some<Int>)
    z3::func_decl some_construct = some_sort.constructors()[0];
    //(declare-fun Some<Int>-value (Some<Int>) Int)
    z3::func_decl some_accesor = some_construct.accessors()[0];

    z3::expr target_some = some_accesor(ex);

    bsqon::Type* some_type = this->asm_info->lookupTypeKey(bsq_t->oftype);
    bsqon::Value* some_val = this->extractValue(some_type, target_some);

    return new bsqon::SomeValue(bsq_t, FILLER_POS, some_val);
}

bsqon::Value* ValueSolver::extractOption(bsqon::OptionType* bsq_t, z3::expr ex)
{
    z3::func_decl_vector opt_cs = ex.get_sort().constructors();
    const std::vector<bsqon::TypeKey>& opt_subtype = this->asm_info->concreteSubtypesMap.at(bsq_t->tkey);

    z3::func_decl_vector opt_recogs = ex.get_sort().recognizers();
    z3::func_decl opt_none_is = opt_recogs[0];
    z3::func_decl opt_some_is = opt_recogs[1];

    //--------------------------------------------------------------------------
    auto none_name = tKeyToSmtName("None", STRUCT_TERM_CONSTRUCT);
    std::optional<z3::func_decl> none_mk_fn = findConstruct(opt_cs, none_name.value());

    z3::expr none_mk = none_mk_fn.value()();
    this->s.push();

    this->s.add(opt_none_is(ex));
    z3::check_result r_none = this->s.check();

    this->s.pop();

    // Negation was the only way to get this working.
    if(r_none == z3::sat) {
        this->s.add(opt_none_is(ex));
        return new bsqon::NoneValue(bsq_t, FILLER_POS);
    }
    //--------------------------------------------------------------------------

    bsqon::TypeKey some_tk = opt_subtype.at(0);
    auto some_name = tKeyToSmtName(some_tk, STRUCT_TERM_CONSTRUCT);

    //(declare-fun @Term-Some<Int>-mk (Some<T>) @Term)
    std::optional<z3::func_decl> term_some_mk = findConstruct(opt_cs, some_name.value());
    //(declare-fun @Term-Some<Int>-value (@Term) Some<T>)
    std::cout << "GOT:\n" << term_some_mk.value() << "\n";
    z3::func_decl term_some_acc = term_some_mk.value().accessors()[0];

    this->s.add(opt_some_is(ex));

    z3::check_result r_some = this->s.check();

    bsqon::Value* some_val = nullptr;
    if(r_some == z3::sat) {
        z3::expr some_expr = term_some_acc(ex);
        bsqon::Type* some_type = this->asm_info->lookupTypeKey(some_tk);

        some_val = this->extractSome(dynamic_cast<bsqon::SomeType*>(some_type), some_expr);
    }

    return some_val;
}

bsqon::Value* ValueSolver::extractConcept(bsqon::ConceptType* bsq_t, z3::expr ex)
{
    auto tg = bsq_t->tag;
    if(tg == bsqon::TypeTag::TYPE_OPTION) {
        return extractOption(static_cast<bsqon::OptionType*>(bsq_t), ex);
    }

    return nullptr;
}

bsqon::Value* ValueSolver::extractList(bsqon::ListType* bsq_t, z3::expr ex)
{
    z3::func_decl_vector constructs = ex.get_sort().constructors();
    assert(constructs.size() == 1);
    z3::func_decl c = constructs[0];
    z3::func_decl c_accs = c.accessors()[0];

    z3::expr list_expr = c_accs(ex);
    z3::expr list_len = this->extractSequenceLen(list_expr);

    bsqon::Type* list_t = this->asm_info->lookupTypeKey(bsq_t->oftype);
    std::vector<bsqon::Value*> vals;

    for(int i = 0; i < list_len.get_numeral_int(); ++i) {
        z3::expr idx = this->s.ctx().int_val(i);

        z3::expr ith_val = list_expr.nth(idx);

        bsqon::Type* t = this->asm_info->lookupTypeKey(list_t->tkey);
        bsqon::Value* val = this->extractValue(t, ith_val);
        vals.push_back(val);
    }

    this->s.add(ex == c(list_expr));
    return new bsqon::ListValue(bsq_t, FILLER_POS, std::move(vals));
}

bsqon::Value* ValueSolver::extractValue(bsqon::Type* bsq_t, z3::expr ex)
{

    auto tg = bsq_t->tag;
    if(tg == bsqon::TypeTag::TYPE_STD_ENTITY) {
        return extractEntity(static_cast<bsqon::StdEntityType*>(bsq_t), ex);
    }
    else if(tg == bsqon::TypeTag::TYPE_PRIMITIVE) {
        return extractPrimitive(static_cast<bsqon::PrimitiveType*>(bsq_t), ex);
    }
    else if(tg == bsqon::TypeTag::TYPE_STD_CONCEPT || tg == bsqon::TypeTag::TYPE_OPTION) {
        return extractConcept(static_cast<bsqon::ConceptType*>(bsq_t), ex);
    }
    else
        (tg == bsqon::TypeTag::TYPE_LIST)
        {
            return extractList(static_cast<bsqon::ListType*>(bsq_t), ex);
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
