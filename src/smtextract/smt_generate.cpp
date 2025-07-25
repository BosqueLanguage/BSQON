#include "smt_extract.h"

std::u8string ValueGenerator::generateInt(const bsqon::PrimitiveType* t, bsqon::IntNumberValue* i_val)
{
    if(i_val == NULL) {
        printf("solveValue returned NULL \n");
        exit(1);
    }

    return i_val->toSMTLib();
}

std::u8string ValueGenerator::generateInt(const bsqon::PrimitiveType* t, bsqon::IntNumberValue* b_val)
{
    if(b_val == NULL) {
        printf("solveValue returned NULL \n");
        exit(1);
    }

    return b_val->toSMTLib();
}

std::u8string ValueGenerator::generatePrimitive(bsqon::PrimitiveType* t, bsqon::PrimtitiveValue* v)
{
    auto tk = t->tkey;
    if(tk == "Int") {
        return generateInt(t, static_cast<bsqon::IntNumberValue*>(v));
    }
    // else if(tk == "BigInt") {
    //     return extractBigInt(bsq_t, ex);
    // }
    // else if(tk == "Nat") {
    //     return extractNat(bsq_t, ex);
    // }
    // else if(tk == "BigNat") {
    //     return extractBigNat(bsq_t, ex);
    // }
    else if(tk == "Bool") {
        return generateBool(t, static_cast<bsqon::BoolValue*>(v));
    }
    // else if(tk == "CString") {
    //     return extractCString(bsq_t, ex);
    // }

    return u8"n/a";
}

std::u8string ValueGenerator::generateValue(bsqon::Type* t, bsqon::Value* v)
{
    auto tg = t->tag;
    if(tg == bsqon::TypeTag::TYPE_PRIMITIVE) {
        return generatePrimitive(static_cast<bsqon::PrimitiveType*>(t), dynamic_cast<bsqon::PrimtitiveValue*>(v));
    }
    // else if(tg == bsqon::TypeTag::TYPE_PRIMITIVE) {
    // }
    // else if(tg == bsqon::TypeTag::TYPE_STD_CONCEPT || tg == bsqon::TypeTag::TYPE_OPTION) {
    //     return extractConcept(static_cast<bsqon::ConceptType*>(bsq_t), ex);
    // }
    // else if(tg == bsqon::TypeTag::TYPE_LIST) {
    //     return extractList(static_cast<bsqon::ListType*>(bsq_t), ex);
    // }
    // else if(tg == bsqon::TypeTag::TYPE_TYPE_DECL) {
    //     return extractTypeDecl(static_cast<bsqon::TypedeclType*>(bsq_t), ex);
    // }

    return u8"n/a";
}

ValueGenerator::ValueGenerator(bsqon::AssemblyInfo* asm_info, bsqon::Type* t, std::string key, bsqon::Value* v,
                               z3::solver& solver)
    : asm_info(asm_info), t(t), s(solver), v()
{
    std::u8string val_sig = u8"(declare-fun " + std::u8string(key.cbegin(), key.cend()) + u8" () " +
                            std::u8string(t->tkey.cbegin(), t->tkey.cend()) + u8" ";

    std::u8string val_ex = this->generateValue(t, v);

    std::u8string val = val_sig + val_ex + u8")";

    printf("%s\n", (const char*)val.c_str());
}
