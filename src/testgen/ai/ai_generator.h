#pragma once

#include "../../common.h"

#include "../../info/type_info.h"
#include "../../info/bsqon.h"

#include <random>

class Context 
{
public:
    std::string typeName;
    std::string fields;
    Context(const Context& other) {
        this->typeName = other.typeName;
        this->fields = other.fields; 
    }
    Context() {
        this->typeName = "";
        this->fields = "";
    }
    Context clone() const {
        return Context(*this);
    }
};
class AITypeGenerator
{
public:
    std::mt19937_64 rng;

    bsqon::AssemblyInfo assembly;

    AITypeGenerator() : rng(std::random_device{}()), assembly() { ; }
    ~AITypeGenerator() { ; }

    bsqon::Value* generateNone(const bsqon::PrimitiveType* t, Context ctx);

    bsqon::Value* generateBool(const bsqon::PrimitiveType* t, Context ctx, bool random_value);
    bsqon::Value* generateNat(const bsqon::PrimitiveType* t, Context ctx, uint64_t random_value);
    bsqon::Value* generateInt(const bsqon::PrimitiveType* t, Context ctx, int64_t random_value);
    bsqon::Value* generateBigNat(const bsqon::PrimitiveType *t, Context ctx, int64_t random_value);
    bsqon::Value* generateString(const bsqon::PrimitiveType* t, Context ctx, std::string random_value);
    bsqon::Value* generateFloat(const bsqon::PrimitiveType *t, Context ctx, float val);

    bsqon::Value* handleConceptType();
    //TODO: more primitives..

    bsqon::Value* generatePrimitive(const bsqon::PrimitiveType* t, Context ctx);
    bsqon::Value* generateEnum(const bsqon::EnumType* t, Context ctx);

    //More special types here...

    bsqon::Value* generateStdEntityType(const bsqon::StdEntityType* t, Context ctx);
    bsqon::Value* generateStdConceptype(const bsqon::StdEntityType* t, Context ctx);
    bsqon::Value* generateListType(const bsqon::ListType *t, Context ctx);

    bsqon::Value* generateType(const bsqon::Type* t, Context ctx);
};
