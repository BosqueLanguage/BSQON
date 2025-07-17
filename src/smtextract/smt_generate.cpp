#include "./smt_extract.h"
#include "./smt_utils.h"

bsqon::Value* ValueGenerator::generateInt(const bsqon::PrimitiveType* bsq_t, z3::expr ex)
{
    z3::expr unin_i = this->s.ctx().constant("i_val", ex.get_sort());
    int ival = 0;
    this->s.push();
    this->s.add(unin_i == ex);
    if(ex.is_numeral_i(ival)) {
        return new bsqon::IntNumberValue(bsq_t, FILLER_POS, ival);
    }
    this->s.pop();

    return new bsqon::IntNumberValue(bsq_t, FILLER_POS, ival);
}

bsqon::Value* ValueGenerator::generatePrimitive(bsqon::PrimitiveType* bsq_t, z3::expr ex)
{
    auto tk = bsq_t->tkey;
    if(tk == "Int") {
        return generateInt(bsq_t, ex);
    }
    return nullptr;
}

bsqon::Value* ValueGenerator::generateValue(bsqon::Type* bsq_t, z3::expr ex)
{
    auto tg = bsq_t->tag;
    if(tg == bsqon::TypeTag::TYPE_PRIMITIVE) {
        return generatePrimitive(static_cast<bsqon::PrimitiveType*>(bsq_t), ex);
    }
    // if(tg == bsqon::TypeTag::TYPE_STD_ENTITY) {
    //     return extractEntity(static_cast<bsqon::StdEntityType*>(bsq_t), ex);
    // }
    // else if(tg == bsqon::TypeTag::TYPE_PRIMITIVE) {
    //     return extractPrimitive(static_cast<bsqon::PrimitiveType*>(bsq_t), ex);
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

    return nullptr;
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
    bsqon::Value* result = this->generateValue(this->t, this->ex);
    if(result == NULL) {
        printf("solveValue returned NULL \n");
        exit(1);
    }
    printf("%s\n", (const char*)result->toString().c_str());
}
