#pragma once

#include "../common.h"

#include "../info/type_info.h"
#include "../info/bsqon.h"

#include "component.h"

#include <random>

class RandomValueGenerator
{
public:
    std::mt19937_64 rng;

    RandomValueGenerator() : rng(std::random_device{}()) { ; }
    ~RandomValueGenerator() { ; }

    void generateNone(const bsqon::PrimitiveType* t, ValueComponent* vc);

    void generateBool(const bsqon::PrimitiveType* t, ValueComponent* vc);
    void generateNat(const bsqon::PrimitiveType* t, ValueComponent* vc);
    void generateInt(const bsqon::PrimitiveType* t, ValueComponent* vc);
    void generateBigNat(const bsqon::PrimitiveType* t, ValueComponent* vc);
    void generateBigInt(const bsqon::PrimitiveType* t, ValueComponent* vc);

    void generateFloat(const bsqon::PrimitiveType* t, ValueComponent* vc);

    void generateCString(const bsqon::PrimitiveType* t, ValueComponent* vc);
    void generateString(const bsqon::PrimitiveType* t, ValueComponent* vc);


    //TODO: more primitives..

    void generatePrimitive(const bsqon::PrimitiveType* t, ValueComponent* vc);
    void generateEnum(const bsqon::EnumType* t, ValueComponent* vc);

    void generateType(const bsqon::Type* t, ValueComponent* vc);
};
