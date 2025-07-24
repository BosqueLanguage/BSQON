#include "smt_extract.h"

// public function getCustomer(id: Int): UserDTO {
//     return UserDTO {
//         some('jch270@uky.edu'<EMAIL>),
//         '12345678'<ALPHANUMERIC>,
//         some('James Chen'<USER_NAME>),
//         some('+12 111-111-1111'<USER_PHONE>),
//         some('123 Fake Street'<USER_ADDRESS>),
//         some(id),
//     };
// }

z3::expr ValueGenerator::generateInt(const bsqon::PrimitiveType* t, bsqon::Value* v)
{
}

ValueExtractor::ValueExtractor(bsqon::AssemblyInfo* asm_info, bsqon::Type* type, std::string key, z3::solver& solver)
    : asm_info(asm_info), t(type), s(solver), ex([&]() {
          auto tmp = getBsqTypeExpr(key, solver);
          if(!tmp.has_value()) {
              std::cout << "ARG: " << key << " not in .smt2 file" << "\n";
              exit(1);
          }
          return tmp.value();
      }())
{
    bsqon::Value* result = this->extractValue(this->t, this->ex);
    if(result == NULL) {
        printf("solveValue returned NULL \n");
        exit(1);
    }
    printf("%s\n", (const char*)result->toString().c_str());
}

ValueGenerator::ValueGenerator(bsqon::AssemblyInfo* asm_info, bsqon::Type* t, std::string key, z3::solver& solver)
    : asm_info(asm_info), t(t), s(solver), v([&]() {
          /// INIT
      }())
{
}
