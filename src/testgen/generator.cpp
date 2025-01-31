#include "generator.h"

static bsqon::SourcePos g_spos = { 0, 0, 0, 0 };

bsqon::Value* TypeGeneratorRandom::generateNone(const bsqon::PrimitiveType* t)
{
    return new bsqon::NoneValue(t, g_spos);
}

bsqon::Value* TypeGeneratorRandom::generateBool(const bsqon::PrimitiveType* t)
{
    std::uniform_int_distribution<> bv(0, 1);
    return new bsqon::BoolValue(t, g_spos, bv(rng) == 1);
}

bsqon::Value* TypeGeneratorRandom::generateNat(const bsqon::PrimitiveType* t)
{
    std::uniform_int_distribution<uint64_t> nv(0, bsqon::Type::MAX_SAFE_NAT);
    return new bsqon::NatNumberValue(t, g_spos, nv(rng));
}

bsqon::Value* TypeGeneratorRandom::generateInt(const bsqon::PrimitiveType* t)
{
    std::uniform_int_distribution<int64_t> iv(bsqon::Type::MIN_SAFE_INT, bsqon::Type::MAX_SAFE_INT);
    return new bsqon::IntNumberValue(t, g_spos, iv(rng));
}

bsqon::Value* TypeGeneratorRandom::generatePrimitive(const bsqon::PrimitiveType* t)
{
    auto tk = t->tkey;
    if(tk == "None") {
        return this->generateNone(t);
    }
    else if(tk == "Bool") {
        return this->generateBool(t);
    }
    else if(tk == "Nat") {
        return this->generateNat(t);
    }
    else if(tk == "Int") {
        return this->generateInt(t);
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
        return nullptr;
    }   
}

bsqon::Value* TypeGeneratorRandom::generateEnum(const bsqon::EnumType* t)
{
    std::uniform_int_distribution<> ev(0, t->variants.size() - 1);
    auto evpos = ev(rng);
    auto variant = t->variants[evpos];

    return new bsqon::EnumValue(t, g_spos, variant, evpos);
}

bsqon::Value* TypeGeneratorRandom::generateStdEntityType(const bsqon::StdEntityType* t)
{
    std::vector<bsqon::Value*> fieldvals;
    std::transform(t->fields.cbegin(), t->fields.cend(), std::back_inserter(fieldvals), [this](const auto& f) { 
        return this->generateType(this->assembly.lookupTypeKey(f.ftype));
    });

    return new bsqon::EntityValue(t, g_spos, std::move(fieldvals));
}

bsqon::Value* TypeGeneratorRandom::generateType(const bsqon::Type* t)
{
    switch(t->tag) {
        case bsqon::TypeTag::TYPE_PRIMITIVE: {
            return this->generatePrimitive(static_cast<const bsqon::PrimitiveType*>(t));
        }
        /*
        * TODO: more tags here
        */
        case bsqon::TypeTag::TYPE_STD_ENTITY:
        {
            return this->generateStdEntityType(static_cast<const bsqon::StdEntityType*>(t));
        }
        case bsqon::TypeTag::TYPE_ENUM: {
            return this->generateEnum(static_cast<const bsqon::EnumType*>(t));
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


