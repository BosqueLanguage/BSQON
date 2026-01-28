#pragma once

#include "../../build/include/z3/z3++.h"
#include "../../build/include/json/json.hpp"
#include "../info/type_info.h"
#include "../info/bsqon.h"
#include <cstdio>
#include <string>
#include <map>
#include <unordered_map>

#define BSQ_INT_MIN -500
#define BSQ_INT_MAX 500
#define BSQ_BIGINT_MIN -1000
#define BSQ_BIGINT_MAX 1000

#define BSQ_NAT_MIN 0
#define BSQ_NAT_MAX 500

#define BSQ_BIGNAT_MIN 0
#define BSQ_BIGNAT_MAX 1000

#define ASCII_MIN 0
#define ASCII_MAX 255

#define FILLER_POS bsqon::SourcePos{0, 0, 0, 0}

void badArgs(const char* msg);
bool validPath(const char* filepath, const char* extension);
std::string valueToSMTStr(std::string id, bsqon::Value* val);
void runMock(bsqon::AssemblyInfo* asm_info, json mock_json, z3::solver& sol, std::string mode);

typedef struct _ExtractSig{
	bsqon::Type* type;
	z3::expr ex;
} ExtractSig;

class ValueExtractor 
{
  public:
    bsqon::AssemblyInfo* asm_info;
    z3::solver& s;

    ValueExtractor(bsqon::AssemblyInfo* asm_info, z3::solver& solver);
    bsqon::Value* extractValue(bsqon::Type* t, z3::expr ex);

  private:
    bsqon::Value* extractBigNat(const bsqon::PrimitiveType* bsq_t, z3::expr ex);
    bsqon::Value* extractNat(const bsqon::PrimitiveType* bsq_t, z3::expr ex);
    bsqon::Value* extractBigInt(const bsqon::PrimitiveType* bsq_t, z3::expr ex);
    bsqon::Value* extractInt(const bsqon::PrimitiveType* bsq_t, z3::expr ex);
    bsqon::Value* extractCString(const bsqon::PrimitiveType* bsq_t, z3::expr ex);
    bsqon::Value* extractBool(const bsqon::PrimitiveType* bsq_t, z3::expr ex);
	bsqon::Value* extractEList(bsqon::EListType* bsq_t, z3::expr ex);
    bsqon::Value* extractList(bsqon::ListType* bsq_t, z3::expr ex);
    bsqon::Value* extractSome(bsqon::SomeType* bsq_t, z3::expr ex);
    bsqon::Value* extractOption(bsqon::OptionType* bsq_t, z3::expr ex);
    bsqon::Value* extractStdConcept(bsqon::StdConceptType* bsq_t, z3::expr ex);
    bsqon::Value* extractTypeDecl(bsqon::TypedeclType* bsq_t, z3::expr ex);
    bsqon::Value* extractPrimitive(bsqon::PrimitiveType* t, z3::expr ex);
    bsqon::Value* extractEntity(bsqon::StdEntityType* t, z3::expr ex);
	bsqon::Value* extractEnum(bsqon::Type* bsq_t, z3::expr ex);

	std::optional<z3::expr> extractAtResultExpr(bsqon::Type* t, z3::expr ex);
    std::optional<char> BinSearchChar(z3::expr str_exp, z3::expr index, int min, int max);
    z3::expr extractSequenceLen(z3::expr ex);
    bsqon::Value* checkValidEval(const bsqon::PrimitiveType* bsq_t, z3::expr ex);
    std::optional<std::pair<z3::func_decl, z3::func_decl>> findConstruct(z3::func_decl_vector terms,
                                                                         z3::func_decl_vector recognizers, z3::expr ex);
};
