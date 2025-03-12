#include "component.h"

bool vcpathCMP(const VCPath& p1, const VCPath& p2)
{
    if(p1.size() < p2.size()) {
        return true;
    }
    else if(p1.size() > p2.size()) {
        return false;
    }
    else {
        return p1.compare(p2) < 0;
    }
}

VCPath pathAccessField(const VCPath& p, const std::string& f)
{
    return p + "." + f;
}

VCPath pathAccessIndex(const VCPath& p, size_t i)
{
    return p + "[" + std::to_string(i) + "]";
}

VCPath pathAccessSpecial(const VCPath& p, const std::string& name)
{
    return p + "@" + name;
}

ValueSetPartition ValueSetGenerator::generateNone(const bsqon::PrimitiveType* t, const ValueSetGeneratorEnvironment& env)
{
    return ValueSetPartition{ {new ValueComponent(env.path, env.constraints, env.context.completeWithValueType(t))} };
}

ValueSetPartition ValueSetGenerator::generateBool(const bsqon::PrimitiveType* t, const ValueSetGeneratorEnvironment& env)
{
    return ValueSetPartition{ {new ValueComponent(env.path, env.constraints, env.context.completeWithValueType(t))} };
}

ValueSetPartition ValueSetGenerator::generateNat(const bsqon::PrimitiveType* t, const ValueSetGeneratorEnvironment& env)
{
    return ValueSetPartition{ {new ValueComponent(env.path, env.constraints, env.context.completeWithValueType(t))} };
}

ValueSetPartition ValueSetGenerator::generateInt(const bsqon::PrimitiveType* t, const ValueSetGeneratorEnvironment& env)
{
    return ValueSetPartition{ {new ValueComponent(env.path, env.constraints, env.context.completeWithValueType(t))} };
}

//TODO: more primitives..

ValueSetPartition ValueSetGenerator::generatePrimitive(const bsqon::PrimitiveType* t, const ValueSetGeneratorEnvironment& env)
{
    auto tk = t->tkey;
    if(tk == "None") {
        return this->generateNone(t, env);
    }
    else if(tk == "Bool") {
        return this->generateBool(t, env);
    }
    else if(tk == "Nat") {
        return this->generateNat(t, env);
    }
    else if(tk == "Int") {
        return this->generateInt(t, env);
    }
        /*
        else if(tk == "BigInt") {
            return this->parseBigInt(t, node);
        }
        else if(tk == "BigNat") {
            return this->parseBigNat(t, node);
        }
        else if(tk == "Rational") {
            return this->parseRational(t, node);
        }
        else if(tk == "Float") {
            return this->parseFloat(t, node);
        }
        else if(tk == "Decimal") {
            return this->parseDecimal(t, node);
        }
        else if(tk == "DecimalDegree") {
            return this->parseDecimalDegree(t, node);
        }
        else if(tk == "LatLongCoordinate") {
            return this->parseLatLong(t, node);
        }
        else if(tk == "Complex") {
            return this->parseComplex(t, node);
        }
        else if(tk == "String") {
            return this->parseString(t, node);
        }
        else if(tk == "CString") {
            return this->parseCString(t, node);
        }
        else if(tk == "ByteBuffer") {
            return this->parseByteBuffer(t, node);
        }
        else if(tk == "TZDateTime") {
            return this->parseTZDateTime(t, node);
        }
        else if(tk == "TIATime") {
            return this->parseTIATime(t, node);
        }
        else if(tk == "PlainDate") {
            return this->parsePlainDate(t, node);
        }
        else if(tk == "PlainTime") {
            return this->parsePlainTime(t, node);
        }
        else if(tk == "LogicalTime") {
            return this->parseLogicalTime(t, node);
        }
        else if(tk == "ISOTimeStamp") {
            return this->parseISOTimeStamp(t, node);
        }
        else if(tk == "UUIDv4") {
            return this->parseUUIDv4(t, node);
        }
        else if(tk == "UUIDv7") {
            return this->parseUUIDv7(t, node);
        }
        else if(tk == "SHAContentHash") {
            return this->parseSHAHashcode(t, node);
        }
        else if(tk == "DataTimeDelta") {
            return this->parseDeltaDateTime(t, node);
        }
        else if(tk == "SecondsDelta") {
            return this->parseDeltaSeconds(t, node);
        }
        else if(tk == "ISOTimestampDelta") {
            return this->parseDeltaISOTimeStamp(t, node);
        }
        else if(tk == "LogicalTimeDelta") {
            return this->parseDeltaLogical(t, node);
        }      
        else if(tk == "UnicodeRegex") {
            return this->parseUnicodeRegex(t, node);
        }
        else if(tk == "CRegex") {
            return this->parseCRegex(t, node);
        }
        else if(tk == "PathRegex") {
            return this->parsePathRegex(t, node);
        }
        else if(tk == "Path") {
            return this->parsePath(t, node);
        }
        else if(tk == "PathItem") {
            return this->parsePathItem(t, node);
        }
        else if(tk == "Glob") {
            return this->parseGlob(t, node);
        }
        */
    else {
        //Missing primitive type
        assert(false);
    }   
}
    
ValueSetPartition ValueSetGenerator::generateEnum(const bsqon::EnumType* t, const ValueSetGeneratorEnvironment& env)
{
    return ValueSetPartition{ {new ValueComponent(env.path, env.constraints, env.context.completeWithValueType(t))} };
}

ValueSetPartition ValueSetGenerator::generateList(const bsqon::ListType* t, const ValueSetGeneratorEnvironment& env)
{
    auto tctx = env.context.extendWithEnclosingType(t);
    std::vector<ValueSetPartition> partitions;

    auto tenv = env.step(pathAccessSpecial(env.path, "length"), env.constraints, tctx.completeWithValueType(this->assembly->lookupTypeKey("Nat")));
    auto len = this->generateType(this->assembly->lookupTypeKey("Nat"), tenv);
    partitions.push_back(len);
 
    for(size_t i = 0; i < MAX_COLLECTION_COUNT - 1; ++i) {
        std::vector<ValueConstraint*> constraints(env.constraints);
        constraints.push_back(new MinLengthConstraint(pathAccessSpecial(env.path, "length"), i + 1));
        
        auto tenv = env.step(pathAccessIndex(env.path, i), constraints, tctx.extendForIndex(i));
        auto pp = this->generateType(this->assembly->lookupTypeKey(t->oftype), tenv);

        partitions.push_back(pp);
    }

    return ValueSetPartition::punion(partitions);
}

//More special types here...

ValueSetPartition ValueSetGenerator::generateStdEntityType(const bsqon::StdEntityType* t, const ValueSetGeneratorEnvironment& env)
{
    auto tctx = env.context.extendWithEnclosingType(t);

    std::vector<ValueSetPartition> fieldpaths;
    std::transform(t->fields.cbegin(), t->fields.cend(), std::back_inserter(fieldpaths), [this, &tctx, &env](const bsqon::EntityTypeFieldEntry& f) {
        auto tenv = env.step(pathAccessField(env.path, f.fname), env.constraints, tctx.extendForField(f));

        return this->generateType(this->assembly->lookupTypeKey(f.ftype), tenv);
    });

    return ValueSetPartition::punion(fieldpaths);
}

ValueSetPartition ValueSetGenerator::generateType(const bsqon::Type* t, const ValueSetGeneratorEnvironment& env)
{
    switch(t->tag) {
        case bsqon::TypeTag::TYPE_PRIMITIVE: {
            return this->generatePrimitive(static_cast<const bsqon::PrimitiveType*>(t), env);
        }
        /*
        * TODO: more tags here
        */
        case bsqon::TypeTag::TYPE_STD_ENTITY:
        {
            return this->generateStdEntityType(static_cast<const bsqon::StdEntityType*>(t), env);
        }
        case bsqon::TypeTag::TYPE_ENUM: {
            return this->generateEnum(static_cast<const bsqon::EnumType*>(t), env);
        }
        /*
        * TODO: more tags here
        */
       default: {
            //Missing type
            assert(false);
        }
    }
}

static bsqon::SourcePos g_spos = { 0, 0, 0, 0 };

bool TestGenerator::isRequiredValue(const VCPath& currpath, bsqon::Value*& value)
{
    auto ci = std::find_if(this->constraints.cbegin(), this->constraints.cend(), [&currpath](const ValueConstraint* vc) { return vc->path == currpath; });
    if(ci == this->constraints.cend()) {
        value = nullptr;
        return false;
    }
    else {
        auto fvc = dynamic_cast<const FixedValueConstraint*>(*ci);

        value = fvc->value;
        return true;
    }

    return false;
}

bsqon::Value* TestGenerator::selectFromPartition(const VCPath& currpath)
{
    auto ci = std::find_if(this->vspartition->components.cbegin(), this->vspartition->components.cend(), [&currpath](const ValueComponent* vc) { return vc->path == currpath; });
    assert(ci != this->vspartition->components.cend()); //we missed something in the partitioning
    
    std::uniform_int_distribution<size_t> unif(0, (*ci)->options.size() - 1);
    size_t choice = unif(rng);

    return (*ci)->options[choice];
}

bsqon::Value* TestGenerator::generatePrimitive(const bsqon::PrimitiveType* t, VCPath currpath)
{
    bsqon::Value* res = nullptr;
    if(this->isRequiredValue(currpath, res)) {
        return res;
    }

    return this->selectFromPartition(currpath);
}

bsqon::Value* TestGenerator::generateEnum(const bsqon::EnumType* t, VCPath currpath)
{
    bsqon::Value* res = nullptr;
    if(this->isRequiredValue(currpath, res)) {
        return res;
    }

    return this->selectFromPartition(currpath);
}

bsqon::Value* TestGenerator::generateStdEntityType(const bsqon::StdEntityType* t, VCPath currpath)
{
    std::vector<bsqon::Value*> fieldvals;
    std::transform(t->fields.cbegin(), t->fields.cend(), std::back_inserter(fieldvals), [this, &currpath](const auto& f) { 
        return this->generateType(this->assembly->lookupTypeKey(f.ftype), pathAccessField(currpath, f.fname));
    });

    return new bsqon::EntityValue(t, g_spos, std::move(fieldvals));
}

bsqon::Value* TestGenerator::generateType(const bsqon::Type* t, VCPath currpath)
{
    switch(t->tag) {
        case bsqon::TypeTag::TYPE_PRIMITIVE: {
            return this->generatePrimitive(static_cast<const bsqon::PrimitiveType*>(t), currpath);
        }
        /*
        * TODO: more tags here
        */
        case bsqon::TypeTag::TYPE_STD_ENTITY:
        {
            return this->generateStdEntityType(static_cast<const bsqon::StdEntityType*>(t), currpath);
        }
        case bsqon::TypeTag::TYPE_ENUM: {
            return this->generateEnum(static_cast<const bsqon::EnumType*>(t), currpath);
        }
        /*
        * TODO: more tags here
        */
       default: {
            //Missing type
            assert(false);
        }
    }
}
