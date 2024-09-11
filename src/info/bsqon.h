#pragma once

#include "../common.h"
#include "type_info.h"

#include <boost/multiprecision/gmp.hpp>
#include <boost/multiprecision/cpp_dec_float.hpp>

namespace bsqon
{
    class SourcePos
    {
    public:
        uint32_t first_line;
        uint32_t first_column;
        uint32_t last_line;
        uint32_t last_column;
    };

    enum class ValueKind
    {
        InvalidKind = 0x0,
        ErrorValueKind,
        NoneValueKind,
        BoolValueKind,
        NatNumberValueKind,
        IntNumberValueKind,
        BigNatNumberValueKind,
        BigIntNumberValueKind,
        FloatNumberValueKind,
        DecimalNumberValueKind,
        RationalNumberValueKind,
        DecimalDegreeNumberValueKind,
        LatLongValueKind,
        ComplexNumberValueKind,
        StringValueKind,
        CStringValueKind,
        ByteBufferValueKind,
        UUIDv4ValueKind,
        UUIDv7ValueKind,
        SHAContentHashValueKind,
        TZDateTimeValueKind,
        TIATimeValueKind,
        PlainDateValueKind,
        PlainTimeValueKind,
        LogicalTimeValueKind,
        ISOTimeStampValueKind,
        DeltaDateTimeValueKind,
        DeltaSecondsValueKind,
        DeltaLogicalValueKind,
        DeltaISOTimeStampValueKind,
        UnicodeRegexValueKind,
        CRegexValueKind,
        PathRegexValueKind,
        SomeValueKind,
        OkValueKind,
        ErrValueKind,
        APIRejectedValueKind,
        APIFailedValueKind,
        APIErrorValueKind,
        APISuccessValueKind,
        PathValueKind,
        PathFragmentValueKind,
        PathGlobValueKind,
        ListValueKind,
        StackValueKind,
        QueueValueKind,
        SetValueKind,
        MapEntryValueKind,
        MapValueKind,
        EnumValueKind,
        TypedeclValueKind,
        EntityValueKind,
        EListValueKind,

        SymbolicValueKind
    };

    class Value
    {
    public:
        const ValueKind kind;
        const Type* vtype;
        const SourcePos spos;

        Value(ValueKind kind, const Type* vtype, SourcePos spos) : kind(kind), vtype(vtype), spos(spos) { ; }
        virtual ~Value() = default;

        virtual std::u8string toString() const = 0;

        virtual bool isErrorValue() const
        {
            return false;
        }

        virtual bool isValidForTypedecl() const
        {
            return false;
        }

        static int keyCompare(const Value* v1, const Value* v2);

        template <typename T>
        static int keyCompareImplScalars(T v1, T v2)
        {
            if(v1 == v2) {
                return 0;
            }
            else if(v1 < v2) {
                return -1;
            }
            else {
                return 1;
            }
        }

        template <typename T>
        static int keyCompareImplStringish(const T& v1, const T& v2)
        {
            if(v1.size() < v2.size()) {
                return -1;
            }
            else if(v1.size() > v2.size()) {
                return 1;
            }
            else {
                auto mmi = std::mismatch(v1.cbegin(), v1.cend(), v2.cbegin(), v2.cend());
                if(mmi.first == v1.cend() && mmi.second == v2.cend()) {
                    return 0;
                }
                else {
                    return *mmi.first < *mmi.second ? -1 : 1;
                }
            }
        }

        static int keyCompareImplTZString(const char* tz1, const char* tz2)
        {
            auto tz1len = strlen(tz1);
            auto tz2len = strlen(tz2);

            if(tz1len < tz2len) {
                return -1;
            }
            else if(tz1len > tz2len) {
                return 1;
            }
            else {
                auto mmi = std::mismatch(tz1, tz1 + tz1len, tz2, tz2len);
                if(mmi.first == (tz1 + tz1len) && mmi.second == (tz2 + tz2len)) {
                    return 0;
                }
                else {
                    return *mmi.first < *mmi.second ? -1 : 1;
                }
            }
        }

        static int keyCompareImplArray(uint16_t* v1, uint16_t* v2, size_t length)
        {
            auto mmi = std::mismatch(v1, v1 + length, v2, v2 + length);
            if(mmi.first == v1 + length && mmi.second == v2 + length) {
                return 0;
            }
            else {
                return *mmi.first < *mmi.second ? -1 : 1;
            }
        }

        static std::u8string tailingFloatZeroHelper(std::u8string sstr, const std::u8string suffix)
        {
            //TODO: deal with E notation

            if(std::find(sstr.cbegin(), sstr.cend(), u8'.') == sstr.cend()) {
                return sstr + u8".0" + suffix;
            }
            else {
                while(sstr.back() == u8'0') {
                   sstr.pop_back();
                }

                if(sstr.back() == u8'.') {
                    sstr.push_back(u8'0');
                }

                return sstr + suffix;
            }
        }
    };

    class ErrorValue : public Value
    {
    public:
        ErrorValue(const Type* vtype, SourcePos spos) : Value(ValueKind::ErrorValueKind, vtype, spos) { ; }
        virtual ~ErrorValue() = default;

        virtual std::u8string toString() const override
        {
            return u8"error";
        }

        virtual bool isErrorValue() const override
        {
            return true;
        }
    };

    class PrimtitiveValue : public Value
    {
    public:
        PrimtitiveValue(ValueKind kind, const Type* vtype, SourcePos spos) : Value(kind, vtype, spos) { ; }
        virtual ~PrimtitiveValue() = default;

        const PrimitiveType* getPrimitiveType() const
        {
            return (const PrimitiveType*)this->vtype;
        }
    };

    class NoneValue : public PrimtitiveValue 
    {
    public:
        NoneValue(const Type* vtype, SourcePos spos) : PrimtitiveValue(ValueKind::NoneValueKind, vtype, spos) { ; }
        virtual ~NoneValue() = default;

        virtual std::u8string toString() const override
        {
            return u8"none";
        }
    };

    class BoolValue : public PrimtitiveValue 
    {
    public:
        const bool tv;
    
        BoolValue(const Type* vtype, SourcePos spos, bool tv) : PrimtitiveValue(ValueKind::BoolValueKind, vtype, spos), tv(tv) { ; }
        virtual ~BoolValue() = default;

        virtual std::u8string toString() const override
        {
            return this->tv ? u8"true" : u8"false";
        }

        virtual bool isValidForTypedecl() const override
        {
            return true;
        }

        static int keyCompare(const BoolValue* v1, const BoolValue* v2)
        {
            if(v1->tv == v2->tv) {
                return 0;
            }
            else if(!v1->tv) {
                return -1;
            }
            else {
                return 1;
            }
        }
    };

    class NatNumberValue : public PrimtitiveValue 
    {
    public:
        const uint64_t cnv;
    
        NatNumberValue(const Type* vtype, SourcePos spos, uint64_t cnv) : PrimtitiveValue(ValueKind::NatNumberValueKind, vtype, spos), cnv(cnv) { ; }
        virtual ~NatNumberValue() = default;

        virtual std::u8string toString() const override
        {
            auto sstr = std::to_string(this->cnv);
            return std::u8string(sstr.cbegin(), sstr.cend()) + u8"n";
        }

        virtual bool isValidForTypedecl() const override
        {
            return true;
        }

        static int keyCompare(const NatNumberValue* v1, const NatNumberValue* v2)
        {
            return Value::keyCompareImplScalars(v1->cnv, v2->cnv);
        }
    };

    class IntNumberValue : public PrimtitiveValue 
    {
    public:
        const int64_t cnv;
        
        IntNumberValue(const Type* vtype, SourcePos spos, int64_t cnv) : PrimtitiveValue(ValueKind::IntNumberValueKind, vtype, spos), cnv(cnv) { ; }
        virtual ~IntNumberValue() = default;

        virtual std::u8string toString() const override
        {
            auto sstr = std::to_string(this->cnv);
            return std::u8string(sstr.cbegin(), sstr.cend()) + u8"i";
        }

        virtual bool isValidForTypedecl() const override
        {
            return true;
        }

        static int keyCompare(const IntNumberValue* v1, const IntNumberValue* v2)
        {
            return Value::keyCompareImplScalars(v1->cnv, v2->cnv);
        }
    };

    class BigNatNumberValue : public PrimtitiveValue 
    {
    public:
        boost::multiprecision::mpz_int cnv;
    
        BigNatNumberValue(const Type* vtype, SourcePos spos, boost::multiprecision::mpz_int cnv) : PrimtitiveValue(ValueKind::BigNatNumberValueKind, vtype, spos), cnv(cnv) { ; }
        virtual ~BigNatNumberValue() = default;

        virtual std::u8string toString() const override
        {
            auto sstr = this->cnv.str();
            return std::u8string(sstr.cbegin(), sstr.cend()) + u8"N";
        }

        virtual bool isValidForTypedecl() const override
        {
            return true;
        }

        static int keyCompare(const BigNatNumberValue* v1, const BigNatNumberValue* v2)
        {
            return Value::keyCompareImplScalars(v1->cnv, v2->cnv);
        }
    };

    class BigIntNumberValue : public PrimtitiveValue 
    {
    public:
        boost::multiprecision::mpz_int cnv;
    
        BigIntNumberValue(const Type* vtype, SourcePos spos, boost::multiprecision::mpz_int cnv) : PrimtitiveValue(ValueKind::BigIntNumberValueKind, vtype, spos), cnv(cnv) { ; }
        virtual ~BigIntNumberValue() = default;

        virtual std::u8string toString() const override
        {
            auto sstr = this->cnv.str();
            return std::u8string(sstr.cbegin(), sstr.cend()) + u8"I";
        }

        virtual bool isValidForTypedecl() const override
        {
            return true;
        }

        static int keyCompare(const BigIntNumberValue* v1, const BigIntNumberValue* v2)
        {
            return Value::keyCompareImplScalars(v1->cnv, v2->cnv);
        }
    };

    class FloatNumberValue : public PrimtitiveValue 
    {
    public:
        const double cnv;
        
        FloatNumberValue(const Type* vtype, SourcePos spos, double cnv) : PrimtitiveValue(ValueKind::FloatNumberValueKind,  vtype, spos), cnv(cnv) { ; }
        virtual ~FloatNumberValue() = default;

        virtual std::u8string toString() const override
        {
            auto sstr = std::to_string(this->cnv);
            return Value::tailingFloatZeroHelper(std::u8string(sstr.cbegin(), sstr.cend()), u8"f");
        }

        virtual bool isValidForTypedecl() const override
        {
            return true;
        }
    };

    class DecimalNumberValue : public PrimtitiveValue 
    {
    public:
        //TODO: We may want to refine the representation range a bit
        boost::multiprecision::cpp_dec_float_50 cnv;
        
        DecimalNumberValue(const Type* vtype, SourcePos spos, boost::multiprecision::cpp_dec_float_50 cnv) : PrimtitiveValue(ValueKind::DecimalNumberValueKind, vtype, spos), cnv(cnv) { ; }
        virtual ~DecimalNumberValue() = default;

        virtual std::u8string toString() const override
        {
            auto sstr = this->cnv.str();
            return Value::tailingFloatZeroHelper(std::u8string(sstr.cbegin(), sstr.cend()), u8"d");
        }

        virtual bool isValidForTypedecl() const override
        {
            return true;
        }
    };

    class RationalNumberValue : public PrimtitiveValue 
    {
    public:
        boost::multiprecision::mpq_rational cnv;

        RationalNumberValue(const Type* vtype, SourcePos spos, boost::multiprecision::mpq_rational cnv) : PrimtitiveValue(ValueKind::RationalNumberValueKind, vtype, spos), cnv(cnv) { ; }
        virtual ~RationalNumberValue() = default;

        virtual std::u8string toString() const override
        {
            auto sstr = this->cnv.str();
            return std::u8string(sstr.cbegin(), sstr.cend()) + u8"R";
        }

        virtual bool isValidForTypedecl() const override
        {
            return true;
        }
    };

    class DecimalDegreeNumberValue : public PrimtitiveValue 
    {
    public:
        //TODO: We may want to refine the representation range a bit
        boost::multiprecision::cpp_dec_float_50 cnv;
    
        DecimalDegreeNumberValue(const Type* vtype, SourcePos spos, boost::multiprecision::cpp_dec_float_50 cnv) : PrimtitiveValue(ValueKind::DecimalDegreeNumberValueKind, vtype, spos), cnv(cnv) { ; }
        virtual ~DecimalDegreeNumberValue() = default;

        virtual std::u8string toString() const override
        {
            auto sstr = this->cnv.str();
            return Value::tailingFloatZeroHelper(std::u8string(sstr.cbegin(), sstr.cend()), u8"dd");
        }

        virtual bool isValidForTypedecl() const override
        {
            return true;
        }
    };

    class LatLongValue : public PrimtitiveValue 
    {
    public:
        boost::multiprecision::cpp_dec_float_50 latitude;
        boost::multiprecision::cpp_dec_float_50 longitude;
    
        LatLongValue(const Type* vtype, SourcePos spos, boost::multiprecision::cpp_dec_float_50 latitude, boost::multiprecision::cpp_dec_float_50 longitude) : PrimtitiveValue(ValueKind::LatLongValueKind, vtype, spos), latitude(latitude), longitude(longitude) { ; }
        virtual ~LatLongValue() = default;

        virtual std::u8string toString() const override
        {
            auto llstr = this->latitude.str();
            auto latstr = Value::tailingFloatZeroHelper(std::u8string(llstr.cbegin(), llstr.cend()), u8"lat");

            auto lostr = this->longitude.str();
            auto longstr = Value::tailingFloatZeroHelper(std::u8string(lostr.cbegin(), lostr.cend()), u8"long");

            if(!longstr.starts_with(u8'-') && !longstr.starts_with(u8'+')) {
                longstr = u8'+' + longstr;
            }

            return latstr + longstr;
        }

        virtual bool isValidForTypedecl() const override
        {
            return true;
        }
    };

    class ComplexNumberValue : public PrimtitiveValue 
    {
    public:
        double real;
        double imag;
    
        ComplexNumberValue(const Type* vtype, SourcePos spos, double real, double imag) : PrimtitiveValue(ValueKind::ComplexNumberValueKind, vtype, spos), real(real), imag(imag) { ; }
        virtual ~ComplexNumberValue() = default;

        virtual std::u8string toString() const override
        {
            auto rrstr = std::to_string(this->real);
            auto rstr = Value::tailingFloatZeroHelper(std::u8string(rrstr.cbegin(), rrstr.cend()), u8"");

            auto iistr = std::to_string(this->imag);
            auto istr = Value::tailingFloatZeroHelper(std::u8string(iistr.cbegin(), iistr.cend()), u8"i");

            if(!istr.starts_with(u8'-') && !istr.starts_with(u8'+')) {
                istr = u8'+' + istr;
            }

            return rstr + istr;
        }

        virtual bool isValidForTypedecl() const override
        {
            return true;
        }
    };

    class StringValue : public PrimtitiveValue 
    {
    public:
        const brex::UnicodeString sv;
    
        virtual ~StringValue() = default;

        static StringValue* createFromParse(const Type* vtype, SourcePos spos, const uint8_t* bytes, size_t length);

        virtual std::u8string toString() const override
        {
            auto ustr = brex::escapeUnicodeString(this->sv);
            return u8"\"" + std::u8string(ustr.begin(), ustr.end()) + u8"\"";
        }

        virtual bool isValidForTypedecl() const override
        {
            return true;
        }

        static int keyCompare(const StringValue* v1, const StringValue* v2)
        {
            return Value::keyCompareImplStringish(v1->sv, v2->sv);
        }

    private:
        StringValue(const Type* vtype, SourcePos spos, brex::UnicodeString&& sv) : PrimtitiveValue(ValueKind::StringValueKind, vtype, spos), sv(std::move(sv)) { ; }
    };

    class CStringValue : public PrimtitiveValue
    {
    public:
        const brex::CString sv;
    
        virtual ~CStringValue() = default;

        static CStringValue* createFromParse(const Type* vtype, SourcePos spos, const uint8_t* bytes, size_t length);

        virtual std::u8string toString() const override
        {
            auto ustr = brex::escapeCString(this->sv);
            return u8"'" + std::u8string(ustr.begin(), ustr.end()) + u8"'";
        }

        virtual bool isValidForTypedecl() const override
        {
            return true;
        }

        static int keyCompare(const CStringValue* v1, const CStringValue* v2)
        {
            return Value::keyCompareImplStringish(v1->sv, v2->sv);
        }

    private:
        CStringValue(const Type* vtype, SourcePos spos, brex::CString&& sv) : PrimtitiveValue(ValueKind::CStringValueKind, vtype, spos), sv(std::move(sv)) { ; }
    };

    class ByteBufferValue : public PrimtitiveValue
    {
    public:
        const std::vector<uint8_t> bytes;

        virtual ~ByteBufferValue() = default;

        static uint8_t extractByteValue(char hb, char lb);
        static ByteBufferValue* createFromParse(const Type* vtype, SourcePos spos, const char* chars);

        virtual std::u8string toString() const override
        {
            std::u8string bstr;
            for(size_t i = 0; i < this->bytes.size(); ++i) {
                char hb = (this->bytes[i] >> 4) & 0x0F;
                char lb = this->bytes[i] & 0x0F;

                bstr.push_back(hb < 10 ? hb + u8'0' : hb - 10 + u8'A');
                bstr.push_back(lb < 10 ? lb + u8'0' : lb - 10 + u8'A');
            }

            return u8"0x[" + bstr + u8"]";
        }

    private:
        ByteBufferValue(const Type* vtype, SourcePos spos, std::vector<uint8_t> bytes) : PrimtitiveValue(ValueKind::ByteBufferValueKind, vtype, spos), bytes(bytes) { ; }
    };

    class UUIDv4Value : public PrimtitiveValue 
    {
    public:
        //TODO: this is currently the uuid as a string -- is the byte representation more useful?
        const std::string uuidstr;
    
        UUIDv4Value(const Type* vtype, SourcePos spos, std::string uuidstr) : PrimtitiveValue(ValueKind::UUIDv4ValueKind, vtype, spos), uuidstr(uuidstr) { ; }
        virtual ~UUIDv4Value() = default;

        virtual std::u8string toString() const override
        {
            return u8"uuid4{" + std::u8string(this->uuidstr.cbegin(), this->uuidstr.cend()) + u8'}';
        }

        virtual bool isValidForTypedecl() const override
        {
            return true;
        }

        static int keyCompare(const UUIDv4Value* v1, const UUIDv4Value* v2)
        {
            return Value::keyCompareImplStringish(v1->uuidstr, v2->uuidstr);
        }
    };

    class UUIDv7Value : public PrimtitiveValue 
    {
    public:
        //TODO: this is currently the uuid as a string -- is the byte representation more useful?
        const std::string uuidstr;
    
        UUIDv7Value(const Type* vtype, SourcePos spos, std::string uuidstr) : PrimtitiveValue(ValueKind::UUIDv7ValueKind, vtype, spos), uuidstr(uuidstr) { ; }
        virtual ~UUIDv7Value() = default;

        virtual std::u8string toString() const override
        {
            return u8"uuid7{" + std::u8string(this->uuidstr.cbegin(), this->uuidstr.cend()) + u8'}';
        }

        virtual bool isValidForTypedecl() const override
        {
            return true;
        }

        static int keyCompare(const UUIDv7Value* v1, const UUIDv7Value* v2)
        {
            return Value::keyCompareImplStringish(v1->uuidstr, v2->uuidstr);
        }
    };

    class SHAContentHashValue : public PrimtitiveValue 
    {
    public:
        //TODO: this is currently the hashcode as a string -- is the byte representation more useful?
        const std::string hashstr;
    
        SHAContentHashValue(const Type* vtype, SourcePos spos, std::string hashstr) : PrimtitiveValue(ValueKind::SHAContentHashValueKind, vtype, spos), hashstr(hashstr) { ; }
        virtual ~SHAContentHashValue() = default;

        virtual std::u8string toString() const override
        {
            return u8"sha3{" + std::u8string(this->hashstr.cbegin(), this->hashstr.cend()) + u8'}';
        }

        virtual bool isValidForTypedecl() const override
        {
            return true;
        }

        static int keyCompare(const SHAContentHashValue* v1, const SHAContentHashValue* v2)
        {
            return Value::keyCompareImplStringish(v1->hashstr, v2->hashstr);
        }
    };

    class TZDateTimeValue : public PrimtitiveValue 
    {
    public:
        const TZDateTime tv;
    
        TZDateTimeValue(const Type* vtype, SourcePos spos, TZDateTime tv) : PrimtitiveValue(ValueKind::TZDateTimeValueKind, vtype, spos), tv(tv) { ; }
        virtual ~TZDateTimeValue() = default;

        virtual std::u8string toString() const override
        {
            char buf[64];
            int ccount = sprintf(buf, "%.4u-%.2u-%.2uT%.2u:%.2u:%.2u{%s}", this->tv.year, this->tv.month, this->tv.day, this->tv.hour, this->tv.min, this->tv.sec, this->tv.tzdata);
            
            return std::u8string(buf, buf + ccount);
        }

        virtual bool isValidForTypedecl() const override
        {
            return true;
        }

        static int keyCompare(const TZDateTimeValue* v1, const TZDateTimeValue* v2)
        {
            auto tzcmp = keyCompareImplTZString(v1->tv.tzdata, v2->tv.tzdata);
            if(tzcmp != 0) {
                return tzcmp;
            }

            uint16_t v1vs[6] = {v1->tv.year, v1->tv.month, v1->tv.day, v1->tv.hour, v1->tv.min, v1->tv.sec};
            uint16_t v2vs[6] = {v2->tv.year, v2->tv.month, v2->tv.day, v2->tv.hour, v2->tv.min, v2->tv.sec};

            return Value::keyCompareImplArray(v1vs, v2vs, 6);
        }
    };

    class TIATimeValue : public PrimtitiveValue 
    {
    public:
        const TIATime tv;
    
        TIATimeValue(const Type* vtype, SourcePos spos, TIATime tv) : PrimtitiveValue(ValueKind::TIATimeValueKind, vtype, spos), tv(tv) { ; }
        virtual ~TIATimeValue() = default;

        virtual std::u8string toString() const override
        {
            char buf[64];
            int ccount = sprintf(buf, "%.4u-%.2u-%.2uT%.2u:%.2u:%.2uZ", this->tv.year, this->tv.month, this->tv.day, this->tv.hour, this->tv.min, this->tv.sec);
            
            return std::u8string(buf, buf + ccount);
        }

        virtual bool isValidForTypedecl() const override
        {
            return true;
        }

        static int keyCompare(const TIATimeValue* v1, const TIATimeValue* v2)
        {
            uint16_t v1vs[6] = {v1->tv.year, v1->tv.month, v1->tv.day, v1->tv.hour, v1->tv.min, v1->tv.sec};
            uint16_t v2vs[6] = {v2->tv.year, v2->tv.month, v2->tv.day, v2->tv.hour, v2->tv.min, v2->tv.sec};

            return Value::keyCompareImplArray(v1vs, v2vs, 6);
        }
    };

    class PlainDateValue : public PrimtitiveValue 
    {
    public:
        const PlainDate tv;
    
        PlainDateValue(const Type* vtype, SourcePos spos, PlainDate tv) : PrimtitiveValue(ValueKind::PlainDateValueKind, vtype, spos), tv(tv) { ; }
        virtual ~PlainDateValue() = default;

        virtual std::u8string toString() const override
        {
            char buf[64];
            int ccount = sprintf(buf, "%.4u-%.2u-%.2u", this->tv.year, this->tv.month, this->tv.day);
            
            return std::u8string(buf, buf + ccount);
        }

        virtual bool isValidForTypedecl() const override
        {
            return true;
        }

        static int keyCompare(const PlainDateValue* v1, const PlainDateValue* v2)
        {
            uint16_t v1vs[3] = {v1->tv.year, v1->tv.month, v1->tv.day};
            uint16_t v2vs[3] = {v2->tv.year, v2->tv.month, v2->tv.day};

            return Value::keyCompareImplArray(v1vs, v2vs, 3);
        }
    };

    class PlainTimeValue : public PrimtitiveValue 
    {
    public:
        const PlainTime tv;
    
        PlainTimeValue(const Type* vtype, SourcePos spos, PlainTime tv) : PrimtitiveValue(ValueKind::PlainTimeValueKind, vtype, spos), tv(tv) { ; }
        virtual ~PlainTimeValue() = default;

        virtual std::u8string toString() const override
        {
            char buf[64];
            int ccount = sprintf(buf, "%.2u:%.2u:%.2u", this->tv.hour, this->tv.min, this->tv.sec);
            
            return std::u8string(buf, buf + ccount);
        }

        virtual bool isValidForTypedecl() const override
        {
            return true;
        }

        static int keyCompare(const PlainTimeValue* v1, const PlainTimeValue* v2)
        {
            uint16_t v1vs[3] = {v1->tv.hour, v1->tv.min, v1->tv.sec};
            uint16_t v2vs[3] = {v2->tv.hour, v2->tv.min, v2->tv.sec};

            return Value::keyCompareImplArray(v1vs, v2vs, 3);
        }
    };

    class LogicalTimeValue : public PrimtitiveValue 
    {
    public:
        const uint64_t tv;
    
        LogicalTimeValue(const Type* vtype, SourcePos spos, uint64_t tv) : PrimtitiveValue(ValueKind::LogicalTimeValueKind, vtype, spos), tv(tv) { ; }
        virtual ~LogicalTimeValue() = default;

        virtual std::u8string toString() const override
        {
            auto sstr = std::to_string(this->tv);
            return std::u8string(sstr.cbegin(), sstr.cend()) + u8"l";
        }

        virtual bool isValidForTypedecl() const override
        {
            return true;
        }

        static int keyCompare(const LogicalTimeValue* v1, const LogicalTimeValue* v2)
        {
            return Value::keyCompareImplScalars(v1->tv, v2->tv);
        }
    };

    class ISOTimeStampValue : public PrimtitiveValue 
    {
    public:
        const ISOTimeStamp tv;
    
        ISOTimeStampValue(const Type* vtype, SourcePos spos, ISOTimeStamp tv) : PrimtitiveValue(ValueKind::ISOTimeStampValueKind, vtype, spos), tv(tv) { ; }
        virtual ~ISOTimeStampValue() = default;

        virtual std::u8string toString() const override
        {
            char buf[64];
            int ccount = sprintf(buf, "%.4u-%.2u-%.2uT%.2u:%.2u:%.2u.%.3uZ", this->tv.year, this->tv.month, this->tv.day, this->tv.hour, this->tv.min, this->tv.sec, this->tv.millis);
            
            return std::u8string(buf, buf + ccount);
        }

        virtual bool isValidForTypedecl() const override
        {
            return true;
        }

        static int keyCompare(const ISOTimeStampValue* v1, const ISOTimeStampValue* v2)
        {
            uint16_t v1vs[7] = {v1->tv.year, v1->tv.month, v1->tv.day, v1->tv.hour, v1->tv.min, v1->tv.sec, v1->tv.millis};
            uint16_t v2vs[7] = {v2->tv.year, v2->tv.month, v2->tv.day, v2->tv.hour, v2->tv.min, v2->tv.sec, v2->tv.millis};

            return Value::keyCompareImplArray(v1vs, v2vs, 7);
        }
    };

    class DeltaDateTimeValue : public PrimtitiveValue 
    {
    public:
        const DeltaDateTime tv;
    
        DeltaDateTimeValue(const Type* vtype, SourcePos spos, DeltaDateTime tv) : PrimtitiveValue(ValueKind::DeltaDateTimeValueKind, vtype, spos), tv(tv) { ; }
        virtual ~DeltaDateTimeValue() = default;

        virtual std::u8string toString() const override
        {
            char buf[64];
            int ccount = sprintf(buf, "%.4u-%.2u-%.2uT%.2u:%.2u:%.2u", this->tv.year, this->tv.month, this->tv.day, this->tv.hour, this->tv.min, this->tv.sec);
            
            return this->tv.sign + std::u8string(buf, buf + ccount);
        }

        virtual bool isValidForTypedecl() const override
        {
            return true;
        }

        xxxx;
    };

    class DeltaSecondsValue : public PrimtitiveValue 
    {
    public:
        const int64_t tv;
    
        DeltaSecondsValue(const Type* vtype, SourcePos spos, int64_t tv) : PrimtitiveValue(ValueKind::DeltaSecondsValueKind, vtype, spos), tv(tv) { ; }
        virtual ~DeltaSecondsValue() = default;

        virtual std::u8string toString() const override
        {
            auto sstr = std::to_string(this->tv);
            if(this->tv >= 0) {
                sstr = "+" + sstr;
            }
            return std::u8string(sstr.cbegin(), sstr.cend()) + u8"ds";
        }

        virtual bool isValidForTypedecl() const override
        {
            return true;
        }

        xxxx;
    };

    class DeltaLogicalValue : public PrimtitiveValue 
    {
    public:
        const int64_t tv;
    
        DeltaLogicalValue(const Type* vtype, SourcePos spos, int64_t tv) : PrimtitiveValue(ValueKind::DeltaLogicalValueKind, vtype, spos), tv(tv) { ; }
        virtual ~DeltaLogicalValue() = default;

        virtual std::u8string toString() const override
        {
            auto sstr = std::to_string(this->tv);
            if(this->tv >= 0) {
                sstr = "+" + sstr;
            }
            return std::u8string(sstr.cbegin(), sstr.cend()) + u8"dl";
        }

        virtual bool isValidForTypedecl() const override
        {
            return true;
        }

        xxxx;
    };

    class DeltaISOTimeStampValue : public PrimtitiveValue 
    {
    public:
        const DeltaISOTimeStamp tv;
    
        DeltaISOTimeStampValue(const Type* vtype, SourcePos spos, DeltaISOTimeStamp tv) : PrimtitiveValue(ValueKind::DeltaISOTimeStampValueKind, vtype, spos), tv(tv) { ; }
        virtual ~DeltaISOTimeStampValue() = default;

        virtual std::u8string toString() const override
        {
            char buf[64];
            int ccount = sprintf(buf, "%.4u-%.2u-%.2uT%.2u:%.2u:%.2u.%.3u", this->tv.year, this->tv.month, this->tv.day, this->tv.hour, this->tv.min, this->tv.sec, this->tv.millis);
            
            return this->tv.sign + std::u8string(buf, buf + ccount);
        }

        virtual bool isValidForTypedecl() const override
        {
            return true;
        }

        xxxx;
    };

    class UnicodeRegexValue : public PrimtitiveValue 
    {
    public:
        const brex::Regex* tv;
        const std::u8string normalizedre;
    
        virtual ~UnicodeRegexValue() = default;

        static UnicodeRegexValue* createFromParse(const Type* vtype, SourcePos spos, const uint8_t* bytes, size_t length);

        virtual std::u8string toString() const override
        {
            return this->normalizedre;
        }

    private:
        UnicodeRegexValue(const Type* vtype, SourcePos spos, brex::Regex* tv, std::u8string normalizedre) : PrimtitiveValue(ValueKind::UnicodeRegexValueKind, vtype, spos), tv(tv), normalizedre(normalizedre) { ; }

    };

    class CRegexValue : public PrimtitiveValue 
    {
    public:
        const brex::Regex* tv;
        const std::u8string normalizedre;
    
        virtual ~CRegexValue() = default;

        static CRegexValue* createFromParse(const Type* vtype, SourcePos spos, const uint8_t* bytes, size_t length);

        virtual std::u8string toString() const override
        {
            return this->normalizedre;
        }

    private:
        CRegexValue(const Type* vtype, SourcePos spos, brex::Regex* tv, std::u8string normalizedre) : PrimtitiveValue(ValueKind::CRegexValueKind, vtype, spos), tv(tv), normalizedre(normalizedre) { ; }
    };

    class PathRegexValue : public PrimtitiveValue 
    {
    public:
        const brex::Regex* tv;
        const std::u8string normalizedre;
    
        virtual ~PathRegexValue() = default;

        static PathRegexValue* createFromParse(const Type* vtype, SourcePos spos, const uint8_t* bytes, size_t length);

        virtual std::u8string toString() const override
        {
            return this->normalizedre;
        }

    private:
        PathRegexValue(const Type* vtype, SourcePos spos, brex::Regex* tv, std::u8string normalizedre) : PrimtitiveValue(ValueKind::PathRegexValueKind, vtype, spos), tv(tv), normalizedre(normalizedre) { ; }
    };

    class SomeValue : public Value
    {
    public:
        const Value* v;

        SomeValue(const Type* vtype, SourcePos spos, const Value* v) : Value(ValueKind::SomeValueKind, vtype, spos), v(v) { ; }
        virtual ~SomeValue() = default;

        virtual std::u8string toString() const override
        {
            return std::u8string(this->vtype->tkey.cbegin(), this->vtype->tkey.cend()) + u8'{' + this->v->toString() + u8'}';
        }
    };

    class OkValue : public Value
    {
    public:
        const Value* v;

        OkValue(const Type* vtype, SourcePos spos, const Value* v) : Value(ValueKind::OkValueKind, vtype, spos), v(v) { ; }
        virtual ~OkValue() = default;

        virtual std::u8string toString() const override
        {
            return std::u8string(this->vtype->tkey.cbegin(), this->vtype->tkey.cend()) + u8'{' + this->v->toString() + u8'}';
        }
    };

    class ErrValue : public Value
    {
    public:
        const Value* v;

        ErrValue(const Type* vtype, SourcePos spos, const Value* v) : Value(ValueKind::ErrValueKind, vtype, spos), v(v) { ; }
        virtual ~ErrValue() = default;

        virtual std::u8string toString() const override
        {
            return std::u8string(this->vtype->tkey.cbegin(), this->vtype->tkey.cend()) + u8'{' + this->v->toString() + u8'}';
        }
    };

    class PathValue : public Value
    {
    public:
        const bpath::Path* path;
        const brex::UnicodeString normalizedpth;

        virtual ~PathValue() = default;

        //null if validator fails
        static PathValue* createFromParse(const Type* vtype, SourcePos spos, const uint8_t* chars, size_t length, const bpath::PathGlob* validator);

        virtual std::u8string toString() const override
        {
            return this->normalizedpth + std::u8string(this->vtype->tkey.cbegin(), this->vtype->tkey.cend());
        }

        const PathType* getPathType() const
        {
            return (const PathType*)this->vtype;
        }

        virtual bool isValidForTypedecl() const override
        {
            return true;
        }

        static int keyCompare(const PathValue* v1, const PathValue* v2)
        {
            return Value::keyCompareImplStringish(v1->normalizedpth, v2->normalizedpth);
        }

    private:
        PathValue(const Type* vtype, SourcePos spos, bpath::Path* path, brex::UnicodeString normalizedpth) : Value(ValueKind::PathValueKind, vtype, spos), path(path), normalizedpth(normalizedpth) { ; }
    };

    class PathFragmentValue : public Value
    {
    public:
        const bpath::PathFragment* fragment;
        const brex::UnicodeString normalizedfrag;

        virtual ~PathFragmentValue() = default;

        //null if validator fails
        static PathFragmentValue* createFromParse(const Type* vtype, SourcePos spos, const uint8_t* chars, size_t length, const bpath::PathGlob* validator);

        virtual std::u8string toString() const override
        {
            return this->normalizedfrag + std::u8string(this->vtype->tkey.cbegin(), this->vtype->tkey.cend());
        }

        const PathFragmentType* getPathFragmentType() const
        {
            return (const PathFragmentType*)this->vtype;
        }

        virtual bool isValidForTypedecl() const override
        {
            return true;
        }

        static int keyCompare(const PathFragmentValue* v1, const PathFragmentValue* v2)
        {
            return Value::keyCompareImplStringish(v1->normalizedfrag, v2->normalizedfrag);
        }

    private:
        PathFragmentValue(const Type* vtype, SourcePos spos, bpath::PathFragment* fragment, brex::UnicodeString normalizedfrag) : Value(ValueKind::PathFragmentValueKind, vtype, spos), fragment(fragment), normalizedfrag(normalizedfrag) { ; }
    };

    class PathGlobValue : public Value
    {
    public:
        const bpath::PathGlob* glob;
        const brex::UnicodeString normalizedglob;

        virtual ~PathGlobValue() = default;

        //null if validator fails
        static PathGlobValue* createFromParse(const Type* vtype, SourcePos spos, const uint8_t* chars, size_t length, const bpath::PathGlob* validator);

        virtual std::u8string toString() const override
        {
            return this->normalizedglob + std::u8string(this->vtype->tkey.cbegin(), this->vtype->tkey.cend());
        }

        const PathGlobType* getPathGlobType() const
        {
            return (const PathGlobType*)this->vtype;
        }

        virtual bool isValidForTypedecl() const override
        {
            return true;
        }

        static int keyCompare(const PathGlobValue* v1, const PathGlobValue* v2)
        {
            return Value::keyCompareImplStringish(v1->normalizedglob, v2->normalizedglob);
        }

    private:
        PathGlobValue(const Type* vtype, SourcePos spos, bpath::PathGlob* glob, brex::UnicodeString normalizedglob) : Value(ValueKind::PathGlobValueKind, vtype, spos), glob(glob), normalizedglob(normalizedglob) { ; }
    };

    class ListValue : public Value
    {
    public:
        const std::vector<Value*> vals;

        ListValue(const Type* vtype, SourcePos spos, std::vector<Value*>&& vals) : Value(ValueKind::ListValueKind, vtype, spos), vals(std::move(vals)) { ; }
        virtual ~ListValue() = default;
        
        virtual std::u8string toString() const override
        {
            auto ltype = std::u8string(this->vtype->tkey.cbegin(), this->vtype->tkey.cend());
            auto lvalues = std::accumulate(this->vals.cbegin(), this->vals.cend(), std::u8string{}, [](std::u8string&& a, const Value* v) { 
                return (a.empty() ? u8"" : std::move(a) + u8", ") + v->toString(); 
            });

            return ltype + u8'{' + lvalues + u8'}';
        }

        const ListType* getListType() const
        {
            return (const ListType*)this->vtype;
        }
    };

    class StackValue : public Value
    {
    public:
        const std::vector<Value*> vals;

        StackValue(const Type* vtype, SourcePos spos, std::vector<Value*>&& vals) : Value(ValueKind::StackValueKind, vtype, spos), vals(std::move(vals)) { ; }
        virtual ~StackValue() = default;
        
        virtual std::u8string toString() const override
        {
            auto stype = std::u8string(this->vtype->tkey.cbegin(), this->vtype->tkey.cend());
            auto svalues = std::accumulate(this->vals.cbegin(), this->vals.cend(), std::u8string{}, [](std::u8string&& a, const Value* v) { 
                return (a.empty() ? u8"" : std::move(a) + u8", ") + v->toString(); 
            });

            return stype + u8'{' + svalues + u8'}';
        }

        const StackType* getStackType() const
        {
            return (const StackType*)this->vtype;
        }
    };

    class QueueValue : public Value
    {
    public:
        const std::vector<Value*> vals;

        QueueValue(const Type* vtype, SourcePos spos, std::vector<Value*>&& vals) : Value(ValueKind::QueueValueKind, vtype, spos), vals(std::move(vals)) { ; }
        virtual ~QueueValue() = default;
        
        virtual std::u8string toString() const override
        {
            auto qtype = std::u8string(this->vtype->tkey.cbegin(), this->vtype->tkey.cend());
            auto qvalues = std::accumulate(this->vals.cbegin(), this->vals.cend(), std::u8string{}, [](std::u8string&& a, const Value* v) { 
                return (a.empty() ? u8"" : std::move(a) + u8", ") + v->toString(); 
            });

            return qtype + u8'{' + qvalues + u8'}';
        }

        const QueueType* getQueueType() const
        {
            return (const QueueType*)this->vtype;
        }
    };

    class SetValue : public Value
    {
    public:
        const std::vector<Value*> vals;

        SetValue(const Type* vtype, SourcePos spos, std::vector<Value*>&& vals) : Value(ValueKind::SetValueKind, vtype, spos), vals(std::move(vals)) { ; }
        virtual ~SetValue() = default;
        
        virtual std::u8string toString() const override
        {
            auto stype = std::u8string(this->vtype->tkey.cbegin(), this->vtype->tkey.cend());
            auto svalues = std::accumulate(this->vals.cbegin(), this->vals.cend(), std::u8string{}, [](std::u8string&& a, const Value* v) { 
                return (a.empty() ? u8"" : std::move(a) + u8", ") + v->toString(); 
            });

            return stype + u8'{' + svalues + u8'}';
        }

        const SetType* getSetType() const
        {
            return (const SetType*)this->vtype;
        }
    };

    class MapEntryValue : public Value
    {
    public:
        Value* key;
        Value* val;

        MapEntryValue(const Type* vtype, SourcePos spos, Value* key, Value* val) : Value(ValueKind::MapEntryValueKind, vtype, spos), key(key), val(val) { ; }
        virtual ~MapEntryValue() = default;
        
        virtual std::u8string toString() const override
        {
            return std::u8string(this->vtype->tkey.cbegin(), this->vtype->tkey.cend()) + u8'{' + this->key->toString() + u8", " + this->val->toString() + u8'}';
        }

        const MapEntryType* getMapEntryType() const
        {
            return (const MapEntryType*)this->vtype;
        }
    };

    class MapValue : public Value
    {
    public:
        const std::vector<MapEntryValue*> vals;

        MapValue(const Type* vtype, SourcePos spos, std::vector<MapEntryValue*>&& vals) : Value(ValueKind::MapValueKind, vtype, spos), vals(std::move(vals)) { ; }
        virtual ~MapValue() = default;
        
        virtual std::u8string toString() const override
        {
            auto mtype = std::u8string(this->vtype->tkey.cbegin(), this->vtype->tkey.cend());
            auto mvalues = std::accumulate(this->vals.cbegin(), this->vals.cend(), std::u8string{}, [](std::u8string&& a, const Value* v) { 
                return (a.empty() ? u8"" : std::move(a) + u8", ") + v->toString(); 
            });

            return mtype + u8'{' + mvalues + u8'}';
        }

        const MapType* getMapType() const
        {
            return (const MapType*)this->vtype;
        }
    };

    class EnumValue : public Value
    {
    public:
        const std::string evname;
        const uint32_t ev;

        EnumValue(const Type* vtype, SourcePos spos, std::string evname, uint32_t ev) : Value(ValueKind::EnumValueKind, vtype, spos), evname(evname), ev(ev) { ; }
        virtual ~EnumValue() = default;
        
        virtual std::u8string toString() const override
        {
            return std::u8string(this->vtype->tkey.cbegin(), this->vtype->tkey.cend()) + u8"::" + std::u8string(this->evname.cbegin(), this->evname.cend());
        }

        const EnumType* getEnumType() const
        {
            return (const EnumType*)this->vtype;
        }

        virtual bool isValidForTypedecl() const override
        {
            return true;
        }

        static int keyCompare(const EnumValue* v1, const EnumValue* v2)
        {
            return Value::keyCompareImplScalars(v1->ev, v2->ev);
        }
    };

    class TypedeclValue : public Value
    {
    public:
        const Value* basevalue;

        TypedeclValue(const Type* vtype, SourcePos spos, const Value* basevalue) : Value(ValueKind::TypedeclValueKind, vtype, spos), basevalue(basevalue) { ; }
        virtual ~TypedeclValue() = default;
        
        virtual std::u8string toString() const override
        {
            return this->basevalue->toString() + u8'_' + std::u8string(this->vtype->tkey.cbegin(), this->vtype->tkey.cend());
        }

        const TypedeclType* getTypedeclType() const
        {
            return (const TypedeclType*)this->vtype;
        }
    };

    class EntityValue : public Value
    {
    public:
        //value is nullptr if we need to use the default constructor
        const std::vector<Value*> fieldvalues;

        EntityValue(const Type* vtype, SourcePos spos, const std::vector<Value*>&& fieldvalues) : Value(ValueKind::EntityValueKind, vtype, spos), fieldvalues(std::move(fieldvalues)) { ; }
        virtual ~EntityValue() = default;
        
        virtual std::u8string toString() const override
        {
            auto etype = std::u8string(this->vtype->tkey.cbegin(), this->vtype->tkey.cend());

            std::vector<Value*> ifields;
            std::copy_if(this->fieldvalues.cbegin(), this->fieldvalues.cend(), std::back_inserter(ifields), [](const Value* v) { return v != nullptr; });

            auto efields = std::accumulate(ifields.cbegin(), ifields.cend(), std::u8string{}, [](std::u8string&& a, const Value* v) { 
                return (a.empty() ? u8"" : std::move(a) + u8", ") + v->toString(); 
            });

            return etype + u8'{' + efields + u8'}';
        }

        const EntityType* getEntityType() const
        {
            return (const EntityType*)this->vtype;
        }
    };

    class TupleValue : public Value
    {
    public:
        const std::vector<Value*> values;

        TupleValue(const Type* vtype, SourcePos spos, const std::vector<Value*>&& values) : Value(ValueKind::TupleValueKind, vtype, spos), values(std::move(values)) { ; }
        virtual ~TupleValue() = default;
        
        virtual std::u8string toString() const override
        {
            auto ttype = std::u8string(this->vtype->tkey.cbegin(), this->vtype->tkey.cend());
            auto tvalues = std::accumulate(this->values.cbegin(), this->values.cend(), std::u8string{}, [](std::u8string&& a, const Value* v) { 
                return (a.empty() ? u8"" : std::move(a) + u8", ") + v->toString(); 
            });

            return u8'<' + ttype + u8">[" + tvalues + u8']';
        }

        const TupleType* getTupleType() const
        {
            return (const TupleType*)this->vtype;
        }
    };

    class RecordValue : public Value
    {
    public:
        const std::vector<Value*> values;

        RecordValue(const Type* vtype, SourcePos spos, const std::vector<Value*>&& values) : Value(ValueKind::RecordValueKind, vtype, spos), values(std::move(values)) { ; }
        virtual ~RecordValue() = default;
        
        virtual std::u8string toString() const override
        {
            auto rtype = std::u8string(this->vtype->tkey.cbegin(), this->vtype->tkey.cend());
            auto rvalues = std::accumulate(this->values.cbegin(), this->values.cend(), std::u8string{}, [](std::u8string&& a, const Value* v) { 
                return (a.empty() ? u8"" : std::move(a) + u8", ") + v->toString(); 
            });

            return u8'<' + rtype + u8">{" + rvalues + u8'}';
        }

        const RecordType* getRecordType() const
        {
            return (const RecordType*)this->vtype;
        }
    };

    enum class SymbolicOffsetTag
    {
        FieldOffset,
        IndexOffset,
        KeyOffset
    };
    
    class SymbolicOffset
    {
    public:
        SymbolicOffsetTag tag;
        std::string offset;

        SymbolicOffset(SymbolicOffsetTag tag, std::string offset) : tag(tag), offset(offset) { ; }

        std::u8string toString() const
        {
            auto xstr = std::u8string(this->offset.cbegin(), this->offset.cend());
            switch(this->tag) {
                case SymbolicOffsetTag::FieldOffset: {
                    return u8"." + xstr;
                }
                case SymbolicOffsetTag::IndexOffset: {
                    return u8"." + xstr;
                }
                case SymbolicOffsetTag::KeyOffset: {
                    return u8"[" + xstr + u8"]";
                }
                default: {
                    return u8"{BAD OFFSET}";
                }
                    
            }
        }
    };

    class SymbolValue : public Value
    {
    public:
        const std::string rootname;
        const std::vector<SymbolicOffset> offsets;

        SymbolValue(const Type* vtype, SourcePos spos, std::string rootname, std::vector<SymbolicOffset> offsets) : Value(ValueKind::SymbolicValueKind, vtype, spos), rootname(rootname), offsets(offsets) { ; }
        virtual ~SymbolValue() = default;
        
        virtual std::u8string toString() const override
        {
            auto sroot = std::u8string(this->vtype->tkey.cbegin(), this->vtype->tkey.cend());
            return std::accumulate(this->offsets.cbegin(), this->offsets.cend(), std::move(sroot), [](std::u8string&& a, const SymbolicOffset& v) { 
                return std::move(a) + v.toString(); 
            });
        }
    };

    class BsqonDecl
    {
    public:
        const std::string assemblypath;
        const Type* oftype;
        const Value* value;

        BsqonDecl(std::string assemblypath, const Type* oftype, const Value* value) : assemblypath(assemblypath), oftype(oftype), value(value) { ; }
        virtual ~BsqonDecl() = default;

        std::u8string toString() const
        {
            auto dd = u8"#!" + std::u8string(this->assemblypath.cbegin(), this->assemblypath.cend()) + u8"?" + std::u8string(this->oftype->tkey.cbegin(), this->oftype->tkey.cend());
            return dd + u8"\n\n" + this->value->toString();
        }
    };
}
