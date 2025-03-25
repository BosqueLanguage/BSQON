#pragma once

#include "../common.h"

#include "../info/type_info.h"
#include "../info/bsqon.h"
#include "./component.h"

#include <random>

enum class AIModelOption
{
    OPEN_AI,
    DEEP_SEEK,
    GEMINI
};

class AIValueGenerator
{
public:
    std::mt19937_64 rng;
    AIModelOption model;

    AIValueGenerator(AIModelOption model) : rng(std::random_device{}()), model(model) { ; }
    ~AIValueGenerator() { ; }

    void generateNone(const bsqon::PrimitiveType* t, const ValueSetGeneratorEnvironment& env, ValueComponent* vc);

    void generateBool(const bsqon::PrimitiveType* t, const ValueSetGeneratorEnvironment& env, ValueComponent* vc);
    void generateNat(const bsqon::PrimitiveType* t, const ValueSetGeneratorEnvironment& env, ValueComponent* vc);
    void generateInt(const bsqon::PrimitiveType* t, const ValueSetGeneratorEnvironment& env, ValueComponent* vc);
    void generateBigNat(const bsqon::PrimitiveType* t, const ValueSetGeneratorEnvironment& env, ValueComponent* vc);
    void generateBigInt(const bsqon::PrimitiveType* t, const ValueSetGeneratorEnvironment& env, ValueComponent* vc);

    void generateFloat(const bsqon::PrimitiveType* t, const ValueSetGeneratorEnvironment& env, ValueComponent* vc);

    void generateCString(const bsqon::PrimitiveType* t, const ValueSetGeneratorEnvironment& env, ValueComponent* vc);
    void generateString(const bsqon::PrimitiveType* t, const ValueSetGeneratorEnvironment& env, ValueComponent* vc);

    //TODO: more primitives..

    void generatePrimitive(const bsqon::PrimitiveType* t, const ValueSetGeneratorEnvironment& env, ValueComponent* vc);
    void generateEnum(const bsqon::EnumType* t, ValueComponent* vc);

    void generateType(const bsqon::Type* t, const ValueSetGeneratorEnvironment& env, ValueComponent* vc);
};
