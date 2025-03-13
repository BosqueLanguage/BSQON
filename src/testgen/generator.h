#pragma once

#include "../common.h"

#include "../info/type_info.h"
#include "../info/bsqon.h"

#include "component.h"

#include <random>

class TypeGeneratorRandom
{
public:
    std::mt19937_64 rng;

    TypeGeneratorRandom() : rng(std::random_device{}()) { ; }
    ~TypeGeneratorRandom() { ; }

    void generateNone(const bsqon::PrimitiveType* t, ValueComponent* vc);

    void generateBool(const bsqon::PrimitiveType* t, ValueComponent* vc);
    void generateNat(const bsqon::PrimitiveType* t, ValueComponent* vc);
    void generateInt(const bsqon::PrimitiveType* t, ValueComponent* vc);

    //TODO: more primitives..

    void generatePrimitive(const bsqon::PrimitiveType* t, ValueComponent* vc);
    void generateEnum(const bsqon::EnumType* t, ValueComponent* vc);

    //More special types here...

    void generateType(const bsqon::Type* t, ValueComponent* vc);
};
