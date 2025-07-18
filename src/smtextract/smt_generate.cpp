#include "./smt_extract.h"
#include "./smt_utils.h"
#include <cstdio>
#include <optional>
#include <sys/types.h>

bsqon::Value* ValueGenerator::generateBigNat(const bsqon::PrimitiveType* t, z3::expr ex)
{
    u_int16_t N = 0;

    if(this->s.check() == z3::sat) {
        z3::expr z3_eval = this->s.get_model().eval(ex, true);
        bsqon::Value* interp = checkValidEval(t, z3_eval);
        if(interp != NULL) {
            this->s.add(z3_eval == ex);
            return interp;
        }
    }

    return new bsqon::BigNatNumberValue(t, FILLER_POS, N);
}

bsqon::Value* ValueGenerator::generateNat(const bsqon::PrimitiveType* t, z3::expr ex)
{
    uint n = 0;

    if(this->s.check() == z3::sat) {
        z3::expr z3_eval = this->s.get_model().eval(ex, true);
        bsqon::Value* interp = checkValidEval(t, z3_eval);
        if(interp != NULL) {
            this->s.add(z3_eval == ex);
            return interp;
        }
    }

    return new bsqon::BigNatNumberValue(t, FILLER_POS, n);
}

bsqon::Value* ValueGenerator::generateBigInt(const bsqon::PrimitiveType* t, z3::expr ex)
{
    int ival = 0;

    if(this->s.check() == z3::sat) {
        z3::expr z3_eval = this->s.get_model().eval(ex, true);
        bsqon::Value* interp = checkValidEval(t, z3_eval);
        if(interp != NULL) {
            this->s.add(z3_eval == ex);
            return interp;
        }
    }

    return new bsqon::BigIntNumberValue(t, FILLER_POS, ival);
}

bsqon::Value* ValueGenerator::generateInt(const bsqon::PrimitiveType* t, z3::expr ex)
{
    int ival = 0;

    if(this->s.check() == z3::sat) {
        z3::expr z3_eval = this->s.get_model().eval(ex, true);
        bsqon::Value* interp = checkValidEval(t, z3_eval);
        if(interp != NULL) {
            this->s.add(z3_eval == ex);
            return interp;
        }
    }

    return new bsqon::IntNumberValue(t, FILLER_POS, ival);
}

bsqon::Value* ValueGenerator::generateBool(const bsqon::PrimitiveType* t, z3::expr ex)
{
    this->s.push();

    this->s.add(ex);
    z3::check_result rr = this->s.check();

    this->s.pop();

    if(rr == z3::sat) {
        this->s.add(ex);
        return new bsqon::BoolValue(t, FILLER_POS, true);
    }
    else {
        z3::expr alt = this->s.ctx().bool_val(false);
        this->s.add(ex == alt);
        return new bsqon::BoolValue(t, FILLER_POS, false);
    }
}

bsqon::Value* ValueGenerator::generateCString(const bsqon::PrimitiveType* t, z3::expr ex)
{
    std::string str_val = "";

    if(this->s.check() == z3::sat) {
        z3::expr z3_eval = this->s.get_model().eval(ex, true);
        bsqon::Value* interp = checkValidEval(t, z3_eval);
        if(interp != NULL) {
            this->s.add(z3_eval == ex);
            return interp;
        }
    }

    return bsqon::CStringValue::createFromGenerator(t, FILLER_POS, str_val);
}

bsqon::Value* ValueGenerator::generatePrimitive(bsqon::PrimitiveType* t, z3::expr ex)
{
    auto tk = t->tkey;
    if(tk == "Int") {
        return generateInt(t, ex);
    }
    else if(tk == "CString") {
        return generateCString(t, ex);
    }
    return nullptr;
}

z3::expr ValueGenerator::generateSequenceLen(z3::expr ex)
{
    int seq_len = 0;
    z3::expr seq_expr = this->s.ctx().int_val(seq_len);
    // Check Z3 interpretation z3 expr

    if(this->s.check() == z3::sat) {
        z3::expr z3_eval = this->s.get_model().eval(ex.length(), true);
        if(z3_eval.is_int()) {
            this->s.add(z3_eval == ex);
            return z3_eval;
        }
    }

    return seq_expr;
}

bsqon::Value* ValueGenerator::generateEntity(bsqon::StdEntityType* t, z3::expr ex)
{
    z3::sort srt = ex.get_sort();
    // TODO: Try this first, if no worky just use ex.

    z3::func_decl_vector constructs = srt.constructors();
    assert(constructs.size() == 1);

    z3::func_decl c = constructs[0];
    z3::func_decl_vector c_accs = c.accessors();

    std::vector<bsqon::Value*> fieldvalues;

    for(size_t j = 0; j < c_accs.size(); j++) {
        bsqon::EntityTypeFieldEntry field_tk = t->fields[j];
        bsqon::Type* field_t = this->asm_info->lookupTypeKey(field_tk.ftype);

        z3::expr field_acc = c_accs[j](ex);
        try {
            bsqon::Value* field_val = this->generateValue(field_t, field_acc);
            fieldvalues.push_back(field_val);
        }
        catch(const std::exception& e) {
            std::cerr << "extractEntity ERR: " << e.what() << " FOR: " << field_acc.decl() << "\n";
        }
    }

    return new bsqon::EntityValue(t, FILLER_POS, std::move(fieldvalues));
}

bsqon::Value* ValueGenerator::generateSome(bsqon::SomeType* t, z3::expr ex)
{
    z3::sort some_sort = ex.get_sort();

    //(declare-fun Some<T>-mk (Int) Some<T>)
    z3::func_decl some_construct = some_sort.constructors()[0];
    //(declare-fun Some<T>-value (Some<T>) Int)
    z3::func_decl some_accesor = some_construct.accessors()[0];

    z3::expr target_some = some_accesor(ex);

    bsqon::Type* some_type = this->asm_info->lookupTypeKey(t->oftype);
    bsqon::Value* some_val = this->generateValue(some_type, target_some);

    return new bsqon::SomeValue(t, FILLER_POS, some_val);
}

bsqon::Value* ValueGenerator::generateOption(bsqon::OptionType* t, z3::expr ex)
{ // ex.get_sort() is always @Term
    z3::sort term = ex.get_sort();

    z3::func_decl_vector opt_mks = term.constructors();
    z3::func_decl_vector opt_rgs = term.recognizers();
    const std::vector<bsqon::TypeKey>& opt_subtype = this->asm_info->concreteSubtypesMap.at(t->tkey);

    //--------------------------------------------------------------------------
    std::string none_name = tKeyToSmtName("None", STRUCT_TERM_CONSTRUCT);
    std::optional<TermType> none_term = findConstruct(opt_mks, opt_rgs, none_name);
    z3::func_decl none_mk = none_term.value().mk;
    z3::func_decl none_is = none_term.value().rg;

    this->s.push();

    this->s.add(none_is(ex));
    z3::check_result r_none = this->s.check();

    this->s.pop();

    if(r_none == z3::sat) {
        this->s.add(none_is(ex));
        return new bsqon::NoneValue(t, FILLER_POS);
    }
    //--------------------------------------------------------------------------

    bsqon::TypeKey some_tk = opt_subtype.at(0);
    std::string some_name = tKeyToSmtName(some_tk, STRUCT_TERM_CONSTRUCT);

    //(declare-fun @Term-Some<Int>-mk (Some<T>) @Term)
    std::optional<TermType> some_term = findConstruct(opt_mks, opt_rgs, some_name);
    z3::func_decl some_mk = some_term.value().mk;
    z3::func_decl some_is = some_term.value().rg;

    //(declare-fun @Term-Some<Int>-value (@Term) Some<T>)
    z3::func_decl some_acc = some_mk.accessors()[0];

    this->s.add(some_is(ex));

    z3::check_result r_some = this->s.check();

    bsqon::Value* some_val = nullptr;
    if(r_some == z3::sat) {
        z3::expr some_expr = some_acc(ex);
        bsqon::Type* some_type = this->asm_info->lookupTypeKey(some_tk);

        some_val = this->generateSome(dynamic_cast<bsqon::SomeType*>(some_type), some_expr);
    }

    return some_val;
}

bsqon::Value* ValueGenerator::generateList(bsqon::ListType* t, z3::expr ex)
{
    z3::func_decl_vector constructs = ex.get_sort().constructors();
    assert(constructs.size() == 1);
    z3::func_decl c = constructs[0];
    z3::func_decl c_accs = c.accessors()[0];

    z3::expr list_expr = c_accs(ex);
    z3::expr list_len = this->generateSequenceLen(list_expr);

    bsqon::Type* list_t = this->asm_info->lookupTypeKey(t->oftype);
    std::vector<bsqon::Value*> vals;

    for(int i = 0; i < list_len.get_numeral_int(); ++i) {
        z3::expr idx = this->s.ctx().int_val(i);

        z3::expr ith_val = list_expr.nth(idx);

        bsqon::Type* t = this->asm_info->lookupTypeKey(list_t->tkey);
        bsqon::Value* val = this->generateValue(t, ith_val);
        vals.push_back(val);
    }

    this->s.add(ex == c(list_expr));
    return new bsqon::ListValue(t, FILLER_POS, std::move(vals));
}

bsqon::Value* ValueGenerator::generateTypeDecl(bsqon::TypedeclType* t, z3::expr ex)
{
    z3::sort srt = ex.get_sort();
    z3::func_decl t_cs = srt.constructors()[0];
    z3::func_decl t_accs = t_cs.accessors()[0];

    z3::expr p_ex = t_accs(ex);

    bsqon::Type* ptype = this->asm_info->lookupTypeKey(t->primitivetype);
    return this->generateValue(ptype, p_ex);
}

bsqon::Value* ValueGenerator::generateValue(bsqon::Type* t, z3::expr ex)
{
    auto tg = t->tag;
    if(tg == bsqon::TypeTag::TYPE_PRIMITIVE) {
        return generatePrimitive(static_cast<bsqon::PrimitiveType*>(t), ex);
    }
    else if(tg == bsqon::TypeTag::TYPE_STD_ENTITY) {
        return generateEntity(static_cast<bsqon::StdEntityType*>(t), ex);
    }
    else if(tg == bsqon::TypeTag::TYPE_OPTION) {
        return generateOption(static_cast<bsqon::OptionType*>(t), ex);
    }
    else if(tg == bsqon::TypeTag::TYPE_SOME) {
        return generateSome(static_cast<bsqon::SomeType*>(t), ex);
    }
    else if(tg == bsqon::TypeTag::TYPE_LIST) {
        return generateList(static_cast<bsqon::ListType*>(t), ex);
    }
    else if(tg == bsqon::TypeTag::TYPE_TYPE_DECL) {
        return generateTypeDecl(static_cast<bsqon::TypedeclType*>(t), ex);
    }

    return nullptr;
}

std::optional<z3::func_decl> ValueGenerator::GetValidatorFor(z3::sort srt)
{
    if(this->s.check() != z3::sat) {
        return std::nullopt;
    }

    std::string target = "@ValidateRoot-" + srt.name().str();
    z3::model m = this->s.get_model();

    for(uint i = 0; i < m.num_funcs(); ++i) {
        z3::func_decl fn = m.get_func_decl(i);

        if(target == fn.name().str()) {
            return fn;
        }
    }

    return std::nullopt;
}

ValueGenerator::ValueGenerator(bsqon::AssemblyInfo* asm_info, bsqon::Type* type, std::string key, z3::solver& solver)
    : asm_info(asm_info), t(type), s(solver), ex([&]() {
          auto tmp = getBsqTypeExpr(key, solver);
          if(!tmp.has_value()) {
              std::cout << "ARG: " << key << " not in .smt2 file" << "\n";
              exit(1);
          }
          return tmp.value();
      }())
{
    std::string test_name = "test_" + key;
    z3::expr test_ex = this->s.ctx().constant(test_name.c_str(), this->ex.get_sort());

    auto validate = GetValidatorFor(this->ex.get_sort());
    this->s.add(validate.value()(test_ex));

    if(this->s.check() != z3::sat) {
        std::cout << "No SAT for generating value for" << this->ex.decl() << "\n";
        exit(1);
    }

    bsqon::Value* result = this->generateValue(type, test_ex);
    if(result == NULL) {
        printf("solveValue returned NULL \n");
        exit(1);
    }
    printf("%s\n", (const char*)result->toString().c_str());
}
