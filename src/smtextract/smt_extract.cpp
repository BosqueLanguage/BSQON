#include <cstddef>
#include <cstdio>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <optional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include "smt_extract.h"

// Only accept an expr that is of (Seq Int) sort
z3::expr ValueExtractor::extractSequenceLen(z3::expr ex)
{
    // Check Z3 interpretation z3 expr
    if(this->s.check() == z3::sat) {
        z3::expr z3_eval = this->s.get_model().eval(ex.length(), true);
        if(z3_eval.is_int()) {
            this->s.add(z3_eval == ex);
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

std::optional<char> ValueExtractor::BinSearchChar(z3::expr str_exp, z3::expr index, int min, int max)
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

bsqon::Value* ValueExtractor::extractCString(const bsqon::PrimitiveType* bsq_t, z3::expr ex)
{
    if(this->s.check() != z3::sat) {
        std::cout << "GOT: " << this->s.check() << " solver for CString: " << ex.decl() << "\n";
        exit(1);
    }

    // Check Z3 interpretation z3 expr
    z3::expr z3_eval = this->s.get_model().eval(ex, true);
    bsqon::Value* interp = checkValidEval(bsq_t, z3_eval);
    if(interp != NULL) {
        this->s.add(z3_eval == ex);
        return interp;
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

bsqon::Value* ValueExtractor::extractBool(const bsqon::PrimitiveType* bsq_t, z3::expr ex)
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

bsqon::Value* ValueExtractor::extractBigNat(const bsqon::PrimitiveType* bsq_t, z3::expr ex)
{
    // Check Z3 interpretation z3 expr
    if(this->s.check() == z3::sat) {
        z3::expr z3_eval = this->s.get_model().eval(ex, true);
        bsqon::Value* interp = checkValidEval(bsq_t, z3_eval);
        if(interp != NULL) {
            this->s.add(z3_eval == ex);
            return interp;
        }
    }

    // Vector SAT value search
    std::vector<uint> choices = {0, 1, 2, 10, 255};

    for(size_t i = 0; i < choices.size(); i++) {
        this->s.push();
        z3::expr int_tmp = this->s.ctx().int_val(choices.at(i));

        this->s.add(ex == int_tmp);

        z3::check_result rr = this->s.check();
        this->s.pop();

        if(rr == z3::sat) {
            this->s.add(ex == int_tmp);
            return new bsqon::BigNatNumberValue(bsq_t, FILLER_POS, uint64_t(choices.at(i)));
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

bsqon::Value* ValueExtractor::extractNat(const bsqon::PrimitiveType* bsq_t, z3::expr ex)
{

    // Check Z3 interpretation z3 expr
    if(this->s.check() == z3::sat) {
        z3::expr z3_eval = this->s.get_model().eval(ex, true);

        bsqon::Value* interp = checkValidEval(bsq_t, z3_eval);
        if(interp != NULL) {
            this->s.add(z3_eval == ex);
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

bsqon::Value* ValueExtractor::extractBigInt(const bsqon::PrimitiveType* bsq_t, z3::expr ex)
{

    // Check Z3 interpretation z3 expr
    if(this->s.check() == z3::sat) {
        z3::expr z3_eval = this->s.get_model().eval(ex, true);

        bsqon::Value* interp = checkValidEval(bsq_t, z3_eval);
        if(interp != NULL) {
            this->s.add(z3_eval == ex);
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

bsqon::Value* ValueExtractor::extractInt(const bsqon::PrimitiveType* bsq_t, z3::expr ex)
{

    // Check Z3 interpretation z3 expr
    if(this->s.check() == z3::sat) {
        z3::expr z3_eval = this->s.get_model().eval(ex, true);

        bsqon::Value* interp = checkValidEval(bsq_t, z3_eval);
        if(interp != NULL) {
            this->s.add(z3_eval == ex);
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

bsqon::Value* ValueExtractor::extractPrimitive(bsqon::PrimitiveType* bsq_t, z3::expr ex)
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

bsqon::Value* ValueExtractor::extractEntity(bsqon::StdEntityType* bsq_t, z3::expr ex)
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
            std::cerr << "extractEntity ERR: " << e.what() << "\n";
        }
    }

    return new bsqon::EntityValue(bsq_t, FILLER_POS, std::move(fieldvalues));
}

bsqon::Value* ValueExtractor::extractSome(bsqon::SomeType* bsq_t, z3::expr ex)
{
    z3::sort some_sort = ex.get_sort();

    //(declare-fun Some<T>-mk (T) Some<Int>)
    z3::func_decl some_construct = some_sort.constructors()[0];
    //(declare-fun Some<T>-value (Some<T>) Int)
    z3::func_decl some_accesor = some_construct.accessors()[0];

    z3::expr target_some = some_accesor(ex);

    bsqon::Type* some_type = this->asm_info->lookupTypeKey(bsq_t->oftype);
    bsqon::Value* some_val = this->extractValue(some_type, target_some);

    return new bsqon::SomeValue(bsq_t, FILLER_POS, some_val);
}

// The beauty of Cpp
// ex.get_sort().constructors()[1].accessors()[0].range().constructors()[0].range().constructors()[0].accessors();
bsqon::Value* ValueExtractor::extractOption(bsqon::OptionType* bsq_t, z3::expr ex)
{
    // ex.get_sort() is always @Term
    z3::sort term = ex.get_sort();

    z3::func_decl_vector opt_mks = term.constructors();
    z3::func_decl_vector opt_rgs = term.recognizers();
    const std::vector<bsqon::TypeKey>& opt_subtype = this->asm_info->concreteSubtypesMap.at(bsq_t->tkey);

    auto opt_term = findConstruct(opt_mks, opt_rgs, ex).value();
    z3::func_decl opt_mk = opt_term.first;
    z3::func_decl opt_is = opt_term.second;
    this->s.add(opt_is(ex));

    //--------------------------------------------------------------------------
    std::string none_name = tKeyToSmtName("None", STRUCT_TERM_CONSTRUCT);
    if(opt_mk.name().str() == none_name) {
        return new bsqon::NoneValue(bsq_t, FILLER_POS);
    }
    //--------------------------------------------------------------------------
    z3::func_decl some_acc = opt_mk.accessors()[0];
    z3::check_result r_some = this->s.check();

    bsqon::Value* some_val = nullptr;
    if(r_some == z3::sat) {
        z3::expr some_expr = some_acc(ex);

        bsqon::TypeKey some_tk = opt_subtype.at(0);
        bsqon::Type* some_type = this->asm_info->lookupTypeKey(some_tk);

        some_val = this->extractSome(dynamic_cast<bsqon::SomeType*>(some_type), some_expr);
    }

    return some_val;
}

bsqon::Value* ValueExtractor::extractConcept(bsqon::ConceptType* bsq_t, z3::expr ex)
{
    auto tg = bsq_t->tag;
    if(tg == bsqon::TypeTag::TYPE_OPTION) {
        return extractOption(static_cast<bsqon::OptionType*>(bsq_t), ex);
    }

    return nullptr;
}

bsqon::Value* ValueExtractor::extractList(bsqon::ListType* bsq_t, z3::expr ex)
{
    z3::sort list_dt_sort = ex.get_sort();
    z3::func_decl list_dt_construct = list_dt_sort.constructors()[0];

    z3::func_decl list_dt_accs = list_dt_construct.accessors()[0];

    z3::expr list_term_ex = list_dt_accs(ex);
    z3::sort terms = list_term_ex.get_sort();
    z3::func_decl_vector list_mks = terms.constructors();
    z3::func_decl_vector list_recogs = terms.recognizers();

    auto n_list = findConstruct(list_mks, list_recogs, list_term_ex).value();
    z3::func_decl list_n_mk_term = n_list.first;
    z3::func_decl list_n_rg_term = n_list.second;

    this->s.add(list_n_rg_term(list_term_ex));

    z3::expr list_n_ops = list_n_mk_term.accessors()[0](list_term_ex);

    z3::sort list_n_ops_sort = list_n_ops.get_sort();
    z3::func_decl list_n_ops_construct = list_n_ops_sort.constructors()[0];
    z3::func_decl_vector list_n_ops_accs = list_n_ops_construct.accessors();

    std::vector<bsqon::Value*> vals;
    size_t list_size = list_n_ops_construct.arity();
    if(list_size == 0) {
        this->s.add(list_n_ops_construct() == list_n_ops);
        return new bsqon::ListValue(bsq_t, FILLER_POS, std::move(vals));
    }

    z3::sort list_of_type = list_n_ops_accs[0].range();
    z3::expr list_n = this->s.ctx().constant("tmp", list_of_type);

    if(list_size == 1) {
        z3::expr unin_1 = this->s.ctx().constant("unin_1", list_of_type);
        list_n = list_n_ops_construct(unin_1);
    }
    else if(list_size == 2) {
        z3::expr unin_1 = this->s.ctx().constant("unin_1", list_of_type);
        z3::expr unin_2 = this->s.ctx().constant("unin_2", list_of_type);
        list_n = list_n_ops_construct(unin_1, unin_2);
    }
    else if(list_size == 3) {
        z3::expr unin_1 = this->s.ctx().constant("unin_1", list_of_type);
        z3::expr unin_2 = this->s.ctx().constant("unin_2", list_of_type);
        z3::expr unin_3 = this->s.ctx().constant("unin_3", list_of_type);
        list_n = list_n_ops_construct(unin_1, unin_2, unin_3);
    }

    this->s.add(list_n == list_n_ops);

    for(size_t i = 0; i < list_n.args().size(); ++i) {
        bsqon::Type* list_t = this->asm_info->lookupTypeKey(bsq_t->oftype);

        z3::expr list_ith_val = list_n_ops_accs[i](list_n);
        this->s.add(list_n.args()[i] == list_ith_val);
        try {
            bsqon::Value* ith_val = this->extractValue(list_t, list_ith_val);
            vals.push_back(ith_val);
        }
        catch(const std::exception& e) {
            std::cerr << "extractEntity ERR: " << e.what() << "\n";
        }
    }

    return new bsqon::ListValue(bsq_t, FILLER_POS, std::move(vals));
}

bsqon::Value* ValueExtractor::extractTypeDecl(bsqon::TypedeclType* bsq_t, z3::expr ex)
{
    z3::func_decl t_cs = ex.get_sort().constructors()[0];
    z3::func_decl t_accs = t_cs.accessors()[0];

    z3::expr p_ex = t_accs(ex);

    bsqon::Type* ptype = this->asm_info->lookupTypeKey(bsq_t->primitivetype);
    bsqon::Value* val = this->extractValue(ptype, p_ex);

    return new bsqon::TypedeclValue(bsq_t, FILLER_POS, val);
}

z3::expr ValueExtractor::extractAtResultExpr(z3::expr ex)
{
    z3::func_decl_vector at_res_constructs = ex.get_sort().constructors();

    z3::func_decl_vector at_res_recogs = ex.get_sort().recognizers();

    auto at_res = findConstruct(at_res_constructs, at_res_recogs, ex).value();
    z3::func_decl at_res_mk = at_res.first;
    z3::func_decl at_res_is = at_res.second;

    this->s.add(at_res_is(ex));

    z3::func_decl at_res_acc = at_res_mk.accessors()[0];
    z3::expr at_res_val = at_res_acc(ex);

    return at_res_val;
}

bsqon::Value* ValueExtractor::extractValue(bsqon::Type* bsq_t, z3::expr ex)
{
    if(ex.get_sort().to_string().find("@Result") != std::string::npos) {
        z3::expr result_val_ex = extractAtResultExpr(ex);
        return extractValue(bsq_t, result_val_ex);
    }

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
    else if(tg == bsqon::TypeTag::TYPE_LIST) {
        return extractList(static_cast<bsqon::ListType*>(bsq_t), ex);
    }
    else if(tg == bsqon::TypeTag::TYPE_TYPE_DECL) {
        return extractTypeDecl(static_cast<bsqon::TypedeclType*>(bsq_t), ex);
    }

    return nullptr;
}

ValueExtractor::ValueExtractor(bsqon::AssemblyInfo* asm_info, z3::solver& solver) : asm_info(asm_info), s(solver)
{
}

bsqon::Value* ValueExtractor::checkValidEval(const bsqon::PrimitiveType* bsq_t, z3::expr ex)
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

std::string valueToSMTStr(std::string id, bsqon::Value* val)
{
    if(val == nullptr) {
        return "No value has been extracted.";
    }

    std::string sort_key = "";
    if(val->vtype->tag == bsqon::TypeTag::TYPE_OPTION) {
        sort_key = "@Term";
    }
    else {
        sort_key = tKeyToSmtName(val->vtype->tkey, SMT_TYPE);
    }

    std::u8string val_sig = u8"(define-fun " + std::u8string(id.cbegin(), id.cend()) + u8" () " +
                            std::u8string(sort_key.cbegin(), sort_key.cend()) + u8" ";

    std::u8string val_ex = val->toSMTLib();

    std::u8string final = val_sig + val_ex + u8")";

    return std::string(final.begin(), final.end());
}

std::optional<std::pair<z3::func_decl, z3::func_decl>> ValueExtractor::findConstruct(z3::func_decl_vector terms,
                                                                                     z3::func_decl_vector recognizers,
                                                                                     z3::expr ex)
{
    std::optional<std::pair<z3::func_decl, z3::func_decl>> term;
    for(size_t i = 0; i < recognizers.size(); ++i) {
        z3::func_decl recognize = recognizers[i];
        this->s.push();
        this->s.add(recognize(ex));
        z3::check_result rr = this->s.check();
        this->s.pop();

        if(rr == z3::sat) {
            term = std::make_pair(terms[i], recognize);
        }
    }

    if(term.has_value()) {
        return term;
    }
    else {
        return std::nullopt;
    }
};

void badArgs(const char* msg)
{
    const char* usage = "USAGE: smtextract <formula.smt2> <fn_signature.json> <assembly.json> --<MODE>\nMODES:\n"
                        "\t-e|--extract   - Extract Err Values from SMT\n"
                        "\t-g|--generate  - Generate Test Values from SMT\n"
                        "\t-m|--mock      - Run Mock Tests";

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
