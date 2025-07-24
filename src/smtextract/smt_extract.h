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

std::optional<z3::expr> getBsqTypeExpr(std::string target, z3::solver& s);

class ValueExtractor
{
  public:
    bsqon::AssemblyInfo* asm_info;
    bsqon::Type* t;
    z3::solver& s;
    z3::expr ex;

    ValueExtractor(bsqon::AssemblyInfo* asm_info, bsqon::Type* t, std::string key, z3::solver& solver);
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
    bsqon::Value* extractTypeDecl(bsqon::TypedeclType* bsq_t, z3::expr ex);
    bsqon::Value* extractPrimitive(bsqon::PrimitiveType* t, z3::expr ex);
    bsqon::Value* extractEntity(bsqon::StdEntityType* t, z3::expr ex);

    std::optional<char> BinSearchChar(z3::expr str_exp, z3::expr index, int min, int max);

    std::optional<z3::expr> getExprFromVal(bsqon::Value* v);
    z3::expr extractSequenceLen(z3::expr ex);
};

class ValueGenerator
{
  public:
    bsqon::AssemblyInfo* asm_info;
    bsqon::Type* t;
    z3::solver& s;
    bsqon::Value* v;

    ValueGenerator(bsqon::AssemblyInfo* asm_info, bsqon::Type* t, std::string key, z3::solver& solver);

    z3::expr generateBigNat(const bsqon::PrimitiveType* t, bsqon::Value* v);
    z3::expr generateNat(const bsqon::PrimitiveType* t, bsqon::Value* v);
    z3::expr generateBigInt(const bsqon::PrimitiveType* t, bsqon::Value* v);
    z3::expr generateInt(const bsqon::PrimitiveType* t, bsqon::Value* v);
    z3::expr generateBool(const bsqon::PrimitiveType* t, bsqon::Value* v);
    z3::expr generateCString(const bsqon::PrimitiveType* t, bsqon::Value* v);
    z3::expr generateSome(bsqon::SomeType* t, bsqon::Value* v);
    z3::expr generateOption(bsqon::OptionType* t, bsqon::Value* v);
    z3::expr generateEntity(bsqon::StdEntityType* t, bsqon::Value* v);
    z3::expr generateList(bsqon::ListType* t, bsqon::Value* v);
    z3::expr generateTypeDecl(bsqon::TypedeclType* t, bsqon::Value* v);
    z3::expr generatePrimitive(bsqon::PrimitiveType* t, bsqon::Value* v);
    z3::expr generateValue(bsqon::Type* t, bsqon::Value* v);
};
