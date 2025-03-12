#pragma once

#include "../common.h"

#include "../info/type_info.h"
#include "../info/bsqon.h"

#include <random>

class TypeGeneratorRandom
{
public:
    std::mt19937_64 rng;

    bsqon::AssemblyInfo assembly;

    TypeGeneratorRandom() : rng(std::random_device{}()), assembly() { ; }
    ~TypeGeneratorRandom() { ; }

    bsqon::Value* generateNone(const bsqon::PrimitiveType* t);

    bsqon::Value* generateBool(const bsqon::PrimitiveType* t);
    bsqon::Value* generateNat(const bsqon::PrimitiveType* t);
    bsqon::Value* generateInt(const bsqon::PrimitiveType* t);
    bsqon::Value* generateBigNat(const bsqon::PrimitiveType *t) ;
    bsqon::Value* generateString(const bsqon::PrimitiveType* t);
    bsqon::Value* generateFloat(const bsqon::PrimitiveType *t);

    bsqon::Value* handleConceptType();
    //TODO: more primitives..

    bsqon::Value* generatePrimitive(const bsqon::PrimitiveType* t);
    bsqon::Value* generateEnum(const bsqon::EnumType* t);

    //More special types here...

    bsqon::Value* generateStdEntityType(const bsqon::StdEntityType* t);
    bsqon::Value* generateStdConceptype(const bsqon::StdEntityType* t);
    bsqon::Value* generateListType(const bsqon::ListType *t);

    bsqon::Value* generateType(const bsqon::Type* t);
};
