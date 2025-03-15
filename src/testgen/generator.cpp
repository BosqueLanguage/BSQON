#include "generator.h"

static bsqon::SourcePos g_spos = { 0, 0, 0, 0 };

void TypeGeneratorRandom::generateNone(const bsqon::PrimitiveType* t, ValueComponent* vc)
{
    vc->options.push_back(new bsqon::NoneValue(t, g_spos));
}

void TypeGeneratorRandom::generateBool(const bsqon::PrimitiveType* t, ValueComponent* vc)
{
    vc->options.push_back(new bsqon::BoolValue(t, g_spos, true));
    vc->options.push_back(new bsqon::BoolValue(t, g_spos, false));
}

void TypeGeneratorRandom::generateNat(const bsqon::PrimitiveType* t, ValueComponent* vc)
{
    std::uniform_int_distribution<uint64_t> nv(0, 256);

    if(!vc->context.forspecial.has_value()) {
        vc->options.push_back(new bsqon::NatNumberValue(t, g_spos, 0));
        vc->options.push_back(new bsqon::NatNumberValue(t, g_spos, 1));
        vc->options.push_back(new bsqon::NatNumberValue(t, g_spos, 2));
        vc->options.push_back(new bsqon::NatNumberValue(t, g_spos, 3));

        for(size_t i = 0; i < 3; ++i) {
            vc->options.push_back(new bsqon::NatNumberValue(t, g_spos, nv(rng)));
        }
    }
    else {
        auto sname = vc->context.forspecial.value();
        if(sname == u8"length") {
            for(size_t i = 0; i <= MAX_TEST_COLLECTION_COUNT; ++i) {
                vc->options.push_back(new bsqon::NatNumberValue(t, g_spos, i));
            }
        }
        else {
            assert(false); //unknown special name
        }
    }
}

void TypeGeneratorRandom::generateInt(const bsqon::PrimitiveType* t, ValueComponent* vc)
{
    std::uniform_int_distribution<int64_t> iv(-256, 256);

    vc->options.push_back(new bsqon::IntNumberValue(t, g_spos, -3));
    vc->options.push_back(new bsqon::IntNumberValue(t, g_spos, -1));
    vc->options.push_back(new bsqon::IntNumberValue(t, g_spos, 0));
    vc->options.push_back(new bsqon::IntNumberValue(t, g_spos, 1));
    vc->options.push_back(new bsqon::IntNumberValue(t, g_spos, 3));

    for(size_t i = 0; i < 3; ++i) {
        vc->options.push_back(new bsqon::IntNumberValue(t, g_spos, iv(rng)));
    }
}

void TypeGeneratorRandom::generatePrimitive(const bsqon::PrimitiveType* t, ValueComponent* vc)
{
    auto tk = t->tkey;
    if(tk == "None") {
        this->generateNone(t, vc);
    }
    else if(tk == "Bool") {
        this->generateBool(t, vc);
    }
    else if(tk == "Nat") {
        this->generateNat(t, vc);
    }
    else if(tk == "Int") {
        this->generateInt(t, vc);
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

void TypeGeneratorRandom::generateEnum(const bsqon::EnumType* t, ValueComponent* vc)
{
    for(size_t i = 0; i < t->variants.size(); ++i) {
        vc->options.push_back(new bsqon::EnumValue(t, g_spos, t->variants[i], i));
    }
}

void TypeGeneratorRandom::generateType(const bsqon::Type* t, ValueComponent* vc)
{
    switch(t->tag) {
        case bsqon::TypeTag::TYPE_PRIMITIVE: {
            this->generatePrimitive(static_cast<const bsqon::PrimitiveType*>(t), vc);
            break;
        }
        /*
        * TODO: more tags here
        */
        case bsqon::TypeTag::TYPE_ENUM: {
            this->generateEnum(static_cast<const bsqon::EnumType*>(t), vc);
            break;
        }
        /*
        * TODO: more tags here
        */
       default: {
            //Missing type
            assert(false);
            break;
        }
    }
}


