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

ValueSetPartition ValueSetGenerator::generateBigNat(const bsqon::PrimitiveType* t, const ValueSetGeneratorEnvironment& env)
{
    return ValueSetPartition{ {new ValueComponent(env.path, env.constraints, env.context.completeWithValueType(t))} };
}

ValueSetPartition ValueSetGenerator::generateBigInt(const bsqon::PrimitiveType* t, const ValueSetGeneratorEnvironment& env)
{
    return ValueSetPartition{ {new ValueComponent(env.path, env.constraints, env.context.completeWithValueType(t))} };
}

ValueSetPartition ValueSetGenerator::generateFloat(const bsqon::PrimitiveType* t, const ValueSetGeneratorEnvironment& env)
{
    return ValueSetPartition{ {new ValueComponent(env.path, env.constraints, env.context.completeWithValueType(t))} };
}

ValueSetPartition ValueSetGenerator::generateString(const bsqon::PrimitiveType* t, const ValueSetGeneratorEnvironment& env)
{
    return ValueSetPartition{ {new ValueComponent(env.path, env.constraints, env.context.completeWithValueType(t))} };
}

ValueSetPartition ValueSetGenerator::generateCString(const bsqon::PrimitiveType* t, const ValueSetGeneratorEnvironment& env)
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
    else if(tk == "BigInt") {
        return this->generateBigInt(t, env);
    }
    else if(tk == "BigNat") {
        return this->generateBigNat(t, env);
    }
    /*
        else if(tk == "Rational") {
            return this->parseRational(t, node);
        }
    */
    else if(tk == "Float") {
        return this->generateFloat(t, env);
    }
    /*
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
    */
    else if(tk == "String") {
        return this->generateString(t, env);
    }
    else if(tk == "CString") {
        return this->generateCString(t, env);
    }
    /*
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

    auto tenv = env.step(pathAccessSpecial(env.path, "length"), env.constraints, tctx.extendForSpecial(u8"length").completeWithValueType(this->assembly->lookupTypeKey("Nat")));
    auto len = this->generateType(this->assembly->lookupTypeKey("Nat"), tenv);
    partitions.push_back(len);
 
    for(size_t i = 0; i < MAX_TEST_COLLECTION_COUNT; ++i) {
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

ValueSetPartition ValueSetGenerator::generateTypeDeclType(const bsqon::TypedeclType* t, const ValueSetGeneratorEnvironment& env)
{
    auto npath = pathAccessSpecial(env.path, "value");
    auto nenv = env.step(npath, env.constraints, env.context.extendForTypedecl(t));

    return this->generateType(this->assembly->lookupTypeKey(t->primitivetype), nenv);
}

ValueSetPartition ValueSetGenerator::generateStdConceptType(const bsqon::StdConceptType* t, const ValueSetGeneratorEnvironment& env)
{
    const std::vector<bsqon::TypeKey>& supertypes = this->assembly->concreteSubtypesMap.at(t->tkey);

    std::vector<ValueSetPartition> optpartitions;
    std::transform(supertypes.cbegin(), supertypes.cend(), std::back_inserter(optpartitions), [this, &env](const bsqon::TypeKey& st) {
        auto oftype = this->assembly->lookupTypeKey(st);
        
        std::vector<ValueConstraint*> constraints(env.constraints);
        constraints.push_back(new OfTypeConstraint(pathAccessSpecial(env.path, "oftype"), oftype));
        
        auto tenv = env.step(env.path, constraints, env.context);
        return this->generateType(oftype, tenv);
    });

    return ValueSetPartition::punion(optpartitions);
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
        case bsqon::TypeTag::TYPE_LIST: {
            return this->generateList(static_cast<const bsqon::ListType*>(t), env);
        } 
        case bsqon::TypeTag::TYPE_STD_ENTITY: {
            return this->generateStdEntityType(static_cast<const bsqon::StdEntityType*>(t), env);
        }
        case bsqon::TypeTag::TYPE_ENUM: {
            return this->generateEnum(static_cast<const bsqon::EnumType*>(t), env);
        }
        case bsqon::TypeTag::TYPE_TYPE_DECL: {
            return this->generateTypeDeclType(static_cast<const bsqon::TypedeclType*>(t), env);
        }
        case bsqon::TypeTag::TYPE_STD_CONCEPT: {
            return this->generateStdConceptType(static_cast<const bsqon::StdConceptType*>(t), env);
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
    auto ci = std::find_if(this->constraints.cbegin(), this->constraints.cend(), [&currpath](const ValueConstraint* vc) { 
        return vc->path == currpath && dynamic_cast<const FixedValueConstraint*>(vc) != nullptr; 
    });

    if(ci != this->constraints.cend()) {
        auto fvc = dynamic_cast<const FixedValueConstraint*>(*ci);

        value = fvc->value;
        return true;
    }

    value = nullptr;
    return false;
}

bool TestGenerator::isConstrainedLengthValue(const VCPath& currpath, bsqon::Value*& value)
{
    //
    //NOTE: The len constrains are considered before the fixed values!
    //      So, they will override if we are selecting from a partition at index 2 but have selected a fixed size of 0
    //

    auto lc = std::find_if(this->vspartition->components.cbegin(), this->vspartition->components.cend(), [&currpath](const ValueComponent* vc) { return vc->path == currpath; });
    assert(lc != this->vspartition->components.cend()); //we missed something in the partitioning
    
    const std::vector<bsqon::Value*>& lenopts = (*lc)->options;

    std::vector<const ValueConstraint*> lenconstraints;
    std::copy_if(this->constraints.cbegin(), this->constraints.cend(), std::back_inserter(lenconstraints), [&currpath](const ValueConstraint* vc) {
        return vc->path == currpath && dynamic_cast<const MinLengthConstraint*>(vc) != nullptr;
    });

    if(!lenconstraints.empty()) {
        auto mlci = std::max_element(lenconstraints.cbegin(), lenconstraints.cend(), [](const ValueConstraint* vc1, const ValueConstraint* vc2) {
            return dynamic_cast<const MinLengthConstraint*>(vc1)->minlen < dynamic_cast<const MinLengthConstraint*>(vc2)->minlen;
        });
        auto mlc = dynamic_cast<const MinLengthConstraint*>(*mlci);

        std::vector<bsqon::Value*> minlenopts;
        std::copy_if(lenopts.cbegin(), lenopts.cend(), std::back_inserter(minlenopts), [mlc](const bsqon::Value* v) { 
            return static_cast<const bsqon::NatNumberValue*>(v)->cnv >= mlc->minlen;
        });

        if(minlenopts.empty()) {
            //It must be at least this big 
            value = new bsqon::NatNumberValue(this->assembly->lookupTypeKey("Nat"), g_spos, mlc->minlen);
            return true;
        }
        else {
            std::uniform_int_distribution<size_t> unif(0, minlenopts.size() - 1);
            size_t choice = unif(rng);

            value = minlenopts[choice];
            return true;
        }
    }

    auto ci = std::find_if(this->constraints.cbegin(), this->constraints.cend(), [&currpath](const ValueConstraint* vc) { 
        return vc->path == currpath && dynamic_cast<const FixedValueConstraint*>(vc) != nullptr; 
    });
    
    if(ci != this->constraints.cend()) {
        auto fvc = dynamic_cast<const FixedValueConstraint*>(*ci);
        value = fvc->value;
        return true;
    }

    value = nullptr;
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

const bsqon::Type* TestGenerator::resolveSubtypeChoice(const VCPath& currpath, const bsqon::Type* t)
{
    auto oftypepath = pathAccessSpecial(currpath, "oftype");
    auto ci = std::find_if(this->constraints.cbegin(), this->constraints.cend(), [&oftypepath](const ValueConstraint* vc) { 
        return vc->path == oftypepath && dynamic_cast<const OfTypeConstraint*>(vc) != nullptr; 
    });

    if(ci != this->constraints.cend()) {
        auto fvc = dynamic_cast<const OfTypeConstraint*>(*ci);
        return fvc->vtype;
    }
    else {
        const std::vector<bsqon::TypeKey>& supertypes = this->assembly->concreteSubtypesMap.at(t->tkey);

        std::uniform_int_distribution<size_t> unif(0, supertypes.size() - 1);
        size_t choice = unif(rng);

        return this->assembly->lookupTypeKey(supertypes[choice]);
    }
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

bsqon::Value* TestGenerator::generateList(const bsqon::ListType* t, VCPath currpath)
{
    auto lenpath = pathAccessSpecial(currpath, "length");
    bsqon::Value* len = nullptr;
    if(!this->isConstrainedLengthValue(lenpath, len)) {
        len = this->selectFromPartition(lenpath);
    }

    auto rlen = static_cast<bsqon::NatNumberValue*>(len)->cnv;

    std::vector<bsqon::Value*> fieldvals;
    for(uint64_t i = 0; i < rlen; ++i) {
        auto ipath = pathAccessIndex(currpath, i);
        fieldvals.push_back(this->generateType(this->assembly->lookupTypeKey(t->oftype), ipath));
    }

    return new bsqon::ListValue(t, g_spos, std::move(fieldvals));
}

bsqon::Value* TestGenerator::generateStdEntityType(const bsqon::StdEntityType* t, VCPath currpath)
{
    std::vector<bsqon::Value*> fieldvals;
    std::transform(t->fields.cbegin(), t->fields.cend(), std::back_inserter(fieldvals), [this, &currpath](const auto& f) { 
        return this->generateType(this->assembly->lookupTypeKey(f.ftype), pathAccessField(currpath, f.fname));
    });

    return new bsqon::EntityValue(t, g_spos, std::move(fieldvals));
}

bsqon::Value* TestGenerator::generateTypeDeclType(const bsqon::TypedeclType* t, VCPath currpath)
{
    return this->generateType(this->assembly->lookupTypeKey(t->primitivetype), pathAccessSpecial(currpath, "value"));
}

bsqon::Value* TestGenerator::generateStdConceptType(const bsqon::StdConceptType* t, VCPath currpath)
{
    auto tt = this->resolveSubtypeChoice(currpath, t);
    return this->generateType(tt, currpath);
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
        case bsqon::TypeTag::TYPE_LIST: {
            return this->generateList(static_cast<const bsqon::ListType*>(t), currpath);
        }
        case bsqon::TypeTag::TYPE_STD_ENTITY: {
            return this->generateStdEntityType(static_cast<const bsqon::StdEntityType*>(t), currpath);
        }
        case bsqon::TypeTag::TYPE_ENUM: {
            return this->generateEnum(static_cast<const bsqon::EnumType*>(t), currpath);
        }
        case bsqon::TypeTag::TYPE_TYPE_DECL: {
            return this->generateTypeDeclType(static_cast<const bsqon::TypedeclType*>(t), currpath);
        }
        /*
        * TODO: more tags here
        */
        case bsqon::TypeTag::TYPE_STD_CONCEPT: {
            return this->generateStdConceptType(static_cast<const bsqon::StdConceptType*>(t), currpath);
        }
        default: {
            //Missing type
            assert(false);
        }
    }
}

bool TestGenerator::checkConstraintSatisfiability(const std::vector<const ValueConstraint*>& constraints)
{
    //Make sure all the OfType Constaints on any given path are the same
    std::vector<const ValueConstraint*> oftypeconstraints;
    std::copy_if(constraints.cbegin(), constraints.cend(), std::back_inserter(oftypeconstraints), [](const ValueConstraint* vc) { return dynamic_cast<const OfTypeConstraint*>(vc) != nullptr; });
    std::sort(oftypeconstraints.begin(), oftypeconstraints.end(), [](const ValueConstraint* vc1, const ValueConstraint* vc2) {
        return vc1->path.compare(vc2->path) < 0;
    });

    auto conflictingtype = std::adjacent_find(oftypeconstraints.cbegin(), oftypeconstraints.cend(), [](const ValueConstraint* vc1, const ValueConstraint* vc2) {
        auto cc1 = dynamic_cast<const OfTypeConstraint*>(vc1);
        auto cc2 = dynamic_cast<const OfTypeConstraint*>(vc2);

        return (cc1->path == cc2->path) && (cc1->vtype->tkey != cc2->vtype->tkey);
    });

    if(conflictingtype != oftypeconstraints.cend()) {
        return false;
    }

    //Find all the MinLength Constraints on any given path and make sure there isnt a fixed value that is smaller
    std::vector<const ValueConstraint*> minlenconstraints;
    std::copy_if(constraints.cbegin(), constraints.cend(), std::back_inserter(minlenconstraints), [](const ValueConstraint* vc) { return dynamic_cast<const MinLengthConstraint*>(vc) != nullptr; });
    std::sort(minlenconstraints.begin(), minlenconstraints.end(), [](const ValueConstraint* vc1, const ValueConstraint* vc2) {
        return vc1->path.compare(vc2->path) < 0;
    });

    auto badfixed = std::find_if(minlenconstraints.cbegin(), minlenconstraints.cend(), [&constraints](const ValueConstraint* vc) {
        auto mlc = dynamic_cast<const MinLengthConstraint*>(vc);

        auto fvci = std::find_if(constraints.cbegin(), constraints.cend(), [&mlc](const ValueConstraint* vc) {
            return vc->path == mlc->path && dynamic_cast<const FixedValueConstraint*>(vc) != nullptr;
        });

        if(fvci == constraints.cend()) {
            return false;
        }

        auto fvc = dynamic_cast<const FixedValueConstraint*>(*fvci);
        return static_cast<const bsqon::NatNumberValue*>(fvc->value)->cnv < mlc->minlen;
    });

    if(badfixed != minlenconstraints.cend()) {
        return false;
    }

    return true;
}
