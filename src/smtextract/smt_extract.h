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

typedef struct TermType
{
    z3::func_decl mk;
    z3::func_decl rg;
} TermType;

void badArgs(const char* msg);
bool validPath(const char* filepath, const char* extension);

class BsqMock
{
  public:
    bsqon::AssemblyInfo* asm_info;
    json mock_json;
    z3::solver& s;
    bsqon::Value* mock_val;
    z3::expr* mock_fn;

    BsqMock(bsqon::AssemblyInfo* asm_info, json mock_json, z3::solver& sol);
    std::optional<z3::func_decl> getMockFromSMT(std::string mock_name);
    std::optional<z3::expr> addArgsToMock(z3::func_decl mock);
    z3::expr_vector extractArgsFromMock(z3::expr mock);
    std::map<std::string, std::pair<z3::expr, bsqon::Type*>> getArgMap(z3::func_decl mock_fn);
};

class ValueExtractor
{
  public:
    bsqon::AssemblyInfo* asm_info;
    std::string id;
    bsqon::Type* t;
    z3::expr ex;
    z3::solver& s;
    bsqon::Value* value;

    ValueExtractor(bsqon::AssemblyInfo* asm_info, std::string id, bsqon::Type* t, z3::expr ex, z3::solver& solver);
    bsqon::Value* extractValue(bsqon::Type* t, z3::expr ex);
    bsqon::Value* extractBigNat(const bsqon::PrimitiveType* bsq_t, z3::expr ex);
    bsqon::Value* extractNat(const bsqon::PrimitiveType* bsq_t, z3::expr ex);
    bsqon::Value* extractBigInt(const bsqon::PrimitiveType* bsq_t, z3::expr ex);
    bsqon::Value* extractInt(const bsqon::PrimitiveType* bsq_t, z3::expr ex);
    bsqon::Value* extractCString(const bsqon::PrimitiveType* bsq_t, z3::expr ex);
    bsqon::Value* extractBool(const bsqon::PrimitiveType* bsq_t, z3::expr ex);
    bsqon::Value* extractList(bsqon::ListType* bsq_t, z3::expr ex);
    bsqon::Value* extractSome(bsqon::SomeType* bsq_t, z3::expr ex);
    bsqon::Value* extractOption(bsqon::OptionType* bsq_t, z3::expr ex);
    bsqon::Value* extractConcept(bsqon::ConceptType* t, z3::expr ex);
    bsqon::Value* extractError(bsqon::ErrorType* t, z3::expr ex);
    bsqon::Value* extractOk(bsqon::OkType* t, z3::expr ex);
    bsqon::Value* extractResult(bsqon::ResultType* t, z3::expr ex);
    bsqon::Value* extractTypeDecl(bsqon::TypedeclType* bsq_t, z3::expr ex);
    bsqon::Value* extractPrimitive(bsqon::PrimitiveType* t, z3::expr ex);
    bsqon::Value* extractEntity(bsqon::StdEntityType* t, z3::expr ex);

    std::string valueToSMTStr();
    std::optional<char> BinSearchChar(z3::expr str_exp, z3::expr index, int min, int max);
    z3::expr extractSequenceLen(z3::expr ex);
    bsqon::Value* checkValidEval(const bsqon::PrimitiveType* bsq_t, z3::expr ex);
    std::optional<TermType> findConstruct(z3::func_decl_vector terms, z3::func_decl_vector recognizers, z3::expr ex);
};
