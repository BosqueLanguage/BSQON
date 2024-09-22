#include "bsqon.h"

namespace bsqon
{
    StringValue* StringValue::createFromParse(const Type* vtype, SourcePos spos, const uint8_t* bytes, size_t length)
    {
        auto sv = brex::unescapeUnicodeString(bytes, length);
        if(!sv.first.has_value()) {
            return nullptr;
        }

        return new StringValue(vtype, spos, std::move(sv.first.value()));
    }

    CStringValue* CStringValue::createFromParse(const Type* vtype, SourcePos spos, const uint8_t* bytes, size_t length)
    {
        auto sv = brex::unescapeCString(bytes, length);
        if(!sv.first.has_value()) {
            return nullptr;
        }

        return new CStringValue(vtype, spos, std::move(sv.first.value()));
    }

    uint8_t ByteBufferValue::extractByteValue(char hb, char lb)
    {
        uint8_t h = ('0' <= hb && hb <= '9') ? (hb - '0') : (hb - 'a' + 10);
        uint8_t l = ('0' <= lb && lb <= '9') ? (lb - '0') : (lb - 'a' + 10);

        return (h << 4) | l;
    }

    ByteBufferValue* ByteBufferValue::createFromParse(const Type* vtype, SourcePos spos, const char* chars)
    {
        auto bblen = strlen(chars) - 4; //0x[...]
        
        const char* curr = chars + 3;
        const char* bbend = chars + bblen;

        if(bblen % 2 != 0) {
            return nullptr;
        }

        std::vector<uint8_t> buff;
        buff.reserve(bblen / 2);

        while(curr != bbend) {
            auto hb = *curr++;
            auto lb = *curr++;
            
            uint8_t bv = ByteBufferValue::extractByteValue(hb, lb);
            buff.push_back(bv);
        }

        return new ByteBufferValue(vtype, spos, std::move(buff));
    }

    UnicodeRegexValue* UnicodeRegexValue::createFromParse(const Type* vtype, SourcePos spos, const uint8_t* bytes, size_t length)
    {
        auto re = brex::RegexParser::parseRegex((uint8_t*)bytes, length, true, false, false);
        if(!re.first.has_value()) {
            return nullptr;
        }

        auto normalizedre = re.first.value()->toBSQONFormat();
        return new UnicodeRegexValue(vtype, spos, re.first.value(), normalizedre);
    }

    CRegexValue* CRegexValue::createFromParse(const Type* vtype, SourcePos spos, const uint8_t* bytes, size_t length)
    {
        auto re = brex::RegexParser::parseRegex((uint8_t*)bytes, length, false, true, false);
        if(!re.first.has_value()) {
            return nullptr;
        }

        auto normalizedre = re.first.value()->toBSQONFormat();
        return new CRegexValue(vtype, spos, re.first.value(), normalizedre);
    }

    PathRegexValue* PathRegexValue::createFromParse(const Type* vtype, SourcePos spos, const uint8_t* bytes, size_t length)
    {
        auto re = brex::RegexParser::parseRegex((uint8_t*)bytes, length, false, true, true);
        if(!re.first.has_value()) {
            return nullptr;
        }

        auto normalizedre = re.first.value()->toBSQONFormat();
        return new PathRegexValue(vtype, spos, re.first.value(), normalizedre);
    }

    PathValue* PathValue::createFromParse(const Type* vtype, SourcePos spos, const uint8_t* chars, size_t length)
    {
        //
        //TODO: path parser and validator implementation needed
        //
        return nullptr;
    }

    PathItemValue* PathItemValue::createFromParse(const Type* vtype, SourcePos spos, const uint8_t* chars, size_t length)
    {
        //
        //TODO: path parser and validator implementation needed
        //
        return nullptr;
    }

    GlobValue* GlobValue::createFromParse(const Type* vtype, SourcePos spos, const uint8_t* chars, size_t length)
    {
        //
        //TODO: path parser and validator implementation needed
        //
        return nullptr;
    }

    std::vector<TypeKey> s_known_key_order = {
        "Bool", 
        "Nat", 
        "Int", 
        "BigInt", 
        "BigNat",
        "UUIDv4", 
        "UUIDv7", 
        "SHAContentHash", 
        "TZDateTime", 
        "TAITime", 
        "PlainDate", 
        "PlainTime", 
        "LogicalTime", 
        "ISOTimestamp",
        "DeltaDateTime", 
        "DeltaSeconds", 
        "DeltaLogicalTime", 
        "DeltaISOTimestamp",
        "String", 
        "CString", 
        "Path", 
        "PathItem", 
        "Glob"
    };

    int Value::keyCompare(const Value* v1, const Value* v2)
    {
        if(v1->vtype->tkey != v2->vtype->tkey) {
            auto iter1 = std::find(s_known_key_order.cbegin(), s_known_key_order.cend(), v1->vtype->tkey);
            auto iter2 = std::find(s_known_key_order.cbegin(), s_known_key_order.cend(), v2->vtype->tkey);

            if(iter1 == s_known_key_order.cend() && iter2 == s_known_key_order.cend()) {
                return (v1->vtype->tkey < v2->vtype->tkey) ? -1 : 1;
            }
            else if (iter1 == s_known_key_order.cend()) {
                return 1;
            }
            else if (iter2 == s_known_key_order.cend()) {
                return -1;
            }
            else {
                return iter1 < iter2 ? -1 : 1;
            }
        }
        else {
            const std::string dtype = v1->vtype->tkey;

            if(v1->vtype->tag == TypeTag::TYPE_PRIMITIVE) {
                if(dtype == "Bool") {
                    return BoolValue::keyCompare(static_cast<const BoolValue*>(v1), static_cast<const BoolValue*>(v2));
                }
                else if(dtype == "Nat") {
                    return NatNumberValue::keyCompare(static_cast<const NatNumberValue*>(v1), static_cast<const NatNumberValue*>(v2));
                }
                else if(dtype == "Int") {
                    return IntNumberValue::keyCompare(static_cast<const IntNumberValue*>(v1), static_cast<const IntNumberValue*>(v2));
                }
                else if(dtype == "BigNat") {
                    return BigNatNumberValue::keyCompare(static_cast<const BigNatNumberValue*>(v1), static_cast<const BigNatNumberValue*>(v2));
                }
                else if(dtype == "BigInt") {
                    return BigIntNumberValue::keyCompare(static_cast<const BigIntNumberValue*>(v1), static_cast<const BigIntNumberValue*>(v2));
                }
                else if(dtype == "UUIDv4") {
                    return UUIDv4Value::keyCompare(static_cast<const UUIDv4Value*>(v1), static_cast<const UUIDv4Value*>(v2));
                }
                else if(dtype == "UUIDv7") {
                    return UUIDv7Value::keyCompare(static_cast<const UUIDv7Value*>(v1), static_cast<const UUIDv7Value*>(v2));
                }
                else if(dtype == "SHAContentHash") {
                    return SHAContentHashValue::keyCompare(static_cast<const SHAContentHashValue*>(v1), static_cast<const SHAContentHashValue*>(v2));
                }
                else if(dtype == "TZDateTime") {
                    return TZDateTimeValue::keyCompare(static_cast<const TZDateTimeValue*>(v1), static_cast<const TZDateTimeValue*>(v2));
                }
                else if(dtype == "PlainDate") {
                    return PlainDateValue::keyCompare(static_cast<const PlainDateValue*>(v1), static_cast<const PlainDateValue*>(v2));
                }
                else if(dtype == "PlainTime") {
                    return PlainTimeValue::keyCompare(static_cast<const PlainTimeValue*>(v1), static_cast<const PlainTimeValue*>(v2));
                }
                else if(dtype == "LogicalTime") {
                    return LogicalTimeValue::keyCompare(static_cast<const LogicalTimeValue*>(v1), static_cast<const LogicalTimeValue*>(v2));
                }
                else if(dtype == "ISOTimestamp") {
                    return ISOTimeStampValue::keyCompare(static_cast<const ISOTimeStampValue*>(v1), static_cast<const ISOTimeStampValue*>(v2));
                }
                else if(dtype == "DeltaDateTime") {
                    return DeltaDateTimeValue::keyCompare(static_cast<const DeltaDateTimeValue*>(v1), static_cast<const DeltaDateTimeValue*>(v2));
                }
                else if(dtype == "DeltaSeconds") {
                    return DeltaSecondsValue::keyCompare(static_cast<const DeltaSecondsValue*>(v1), static_cast<const DeltaSecondsValue*>(v2));
                }
                else if(dtype == "DeltaLogicalTime") {
                    return DeltaLogicalTimeValue::keyCompare(static_cast<const DeltaLogicalTimeValue*>(v1), static_cast<const DeltaLogicalTimeValue*>(v2));
                }
                else if(dtype == "DeltaISOTimestamp") {
                    return DeltaISOTimeStampValue::keyCompare(static_cast<const DeltaISOTimeStampValue*>(v1), static_cast<const DeltaISOTimeStampValue*>(v2));
                }
                else if(dtype == "String") {
                    return StringValue::keyCompare(static_cast<const StringValue*>(v1), static_cast<const StringValue*>(v2));
                }
                else if(dtype == "CString") {
                    return CStringValue::keyCompare(static_cast<const CStringValue*>(v1), static_cast<const CStringValue*>(v2));
                }
                else if(dtype == "Path") {
                    return PathValue::keyCompare(static_cast<const PathValue*>(v1), static_cast<const PathValue*>(v2));
                }
                else if(dtype == "PathItem") {
                    return PathItemValue::keyCompare(static_cast<const PathItemValue*>(v1), static_cast<const PathItemValue*>(v2));
                }
                else {
                    //should be Glob
                    return GlobValue::keyCompare(static_cast<const GlobValue*>(v1), static_cast<const GlobValue*>(v2));
                }
            }
            else {
                switch (v1->vtype->tag)
                {
                case TypeTag::TYPE_ENUM:
                    return EnumValue::keyCompare(static_cast<const EnumValue*>(v1), static_cast<const EnumValue*>(v2));
                default:
                    //it must be a typedecl
                    return Value::keyCompare(static_cast<const TypedeclValue*>(v1)->pvalue, static_cast<const TypedeclValue*>(v2)->pvalue);
                }
            }
        }
    }
}

