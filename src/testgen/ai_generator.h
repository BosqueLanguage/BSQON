#pragma once

#include "../common.h"

#include "../info/type_info.h"
#include "../info/bsqon.h"
#include "./component.h"

#include <random>

class AIValueSetGenerator
{
public:
    std::mt19937_64 rng;

    bsqon::AssemblyInfo assembly;

    AIValueSetGenerator() : rng(std::random_device{}()), assembly() { ; }
    ~AIValueSetGenerator() { ; }

    ValueSetPartition generateNone(const bsqon::PrimitiveType* t, const ValueSetGeneratorEnvironment& env);

    ValueSetPartition generateBool(const bsqon::PrimitiveType* t, const ValueSetGeneratorEnvironment& env, std::vector<bool> values);
    ValueSetPartition generateNat(const bsqon::PrimitiveType* t, const ValueSetGeneratorEnvironment& env, std::vector<uint64_t> values);
    ValueSetPartition generateInt(const bsqon::PrimitiveType* t, const ValueSetGeneratorEnvironment& env, std::vector<int64_t> values);
    ValueSetPartition generateBigNat(const bsqon::PrimitiveType *t, const ValueSetGeneratorEnvironment& env, std::vector<int64_t> values);
    ValueSetPartition generateString(const bsqon::PrimitiveType* t, const ValueSetGeneratorEnvironment& env, std::vector<std::string> values);
    ValueSetPartition generateFloat(const bsqon::PrimitiveType *t, const ValueSetGeneratorEnvironment& env, std::vector<float> values);

    ValueSetPartition handleConceptType();
    //TODO: more primitives..

    ValueSetPartition generatePrimitive(const bsqon::PrimitiveType* t, const ValueSetGeneratorEnvironment& env);
    ValueSetPartition generateEnum(const bsqon::EnumType* t, const ValueSetGeneratorEnvironment& env);

    //More special types here...

    ValueSetPartition generateStdEntityType(const bsqon::StdEntityType* t, const ValueSetGeneratorEnvironment& env);
    ValueSetPartition generateStdConceptype(const bsqon::StdEntityType* t, const ValueSetGeneratorEnvironment& env);
    ValueSetPartition generateListType(const bsqon::ListType *t, const ValueSetGeneratorEnvironment& env);

    ValueSetPartition generateType(const bsqon::Type* t, const ValueSetGeneratorEnvironment& env);
};
