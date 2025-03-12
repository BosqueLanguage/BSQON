#include "generator.h"

static bsqon::SourcePos g_spos = {0, 0, 0, 0};

bsqon::Value *TypeGeneratorRandom::generateNone(const bsqon::PrimitiveType *t)
{
    return new bsqon::NoneValue(t, g_spos);
}

// bsqon::Value *TypeGeneratorRandom::generateBool(const bsqon::PrimitiveType *t)
// {
//     std::uniform_int_distribution<> bv(0, 1);
//     return new bsqon::BoolValue(t, g_spos, bv(rng) == 1);
// }

bsqon::Value *TypeGeneratorRandom::generateBool(const bsqon::PrimitiveType *t)
{
    std::uniform_int_distribution<> bv(0, 1);
    bool random_value = bv(rng) == 1;
    std::cout << "Generated bool value: " << (random_value ? "true" : "false") << std::endl;
    return new bsqon::BoolValue(t, g_spos, random_value);
}

bsqon::Value *TypeGeneratorRandom::generateNat(const bsqon::PrimitiveType *t)
{
    std::uniform_int_distribution<uint64_t> nv(0, bsqon::Type::MAX_SAFE_NAT);
    return new bsqon::NatNumberValue(t, g_spos, nv(rng));
}

bsqon::Value *TypeGeneratorRandom::generateInt(const bsqon::PrimitiveType *t)
{
    std::uniform_int_distribution<int64_t> iv(bsqon::Type::MIN_SAFE_INT, bsqon::Type::MAX_SAFE_INT);
    int64_t random_value = iv(rng); 
    std::cout << "Generated int value: " << random_value << std::endl;
    return new bsqon::IntNumberValue(t, g_spos, random_value);
}

bsqon::Value *TypeGeneratorRandom::generateBigNat(const bsqon::PrimitiveType *t)
{
    std::uniform_int_distribution<int64_t> iv(1, bsqon::Type::MAX_SAFE_INT);  
    int64_t random_value = iv(rng);
    std::cout << "Generated Big nat value: " << random_value << std::endl;
    return new bsqon::IntNumberValue(t, g_spos, random_value);
}

bsqon::Value *TypeGeneratorRandom::generateString(const bsqon::PrimitiveType *t)
{
    const std::string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

    std::uniform_int_distribution<int64_t> length_dist(5, 15);  // Example range: 5 to 15 characters
    int64_t length = length_dist(rng);

    uint8_t* random_bytes = new uint8_t[length];

    for (int64_t i = 0; i < length; ++i) {
        random_bytes[i] = charset[rng() % charset.size()];
    }
    std::string str(reinterpret_cast<char*>(random_bytes), length);
    std::cout << "Converted string: " << str << std::endl;
    return bsqon::StringValue::createFromParse(t, g_spos, random_bytes, length);
}

bsqon::Value *TypeGeneratorRandom::generateFloat(const bsqon::PrimitiveType *t)
{
    std::uniform_real_distribution<float> fv(std::numeric_limits<float>::min(), std::numeric_limits<float>::max());
    float random_value = fv(rng); 
    std::cout << "Generated float value: " << random_value << std::endl;
    return new bsqon::FloatNumberValue(t, g_spos, random_value);
}


bsqon::Value *TypeGeneratorRandom::generatePrimitive(const bsqon::PrimitiveType *t)
{
    auto tk = t->tkey;
    if (tk == "None")
    {
        return this->generateNone(t);
    }
    else if (tk == "Bool")
    {
        return this->generateBool(t);
    }
    else if (tk == "Nat")
    {
        return this->generateNat(t);
    }
    else if (tk == "Int")
    {
        return this->generateInt(t);
    }
     else if(tk == "String") 
     {
        return this->generateString(t);
    }else if(tk == "Float") 
    {
        return this->generateFloat(t);
    }else if(tk == "BigNat") {
        return this->generateBigNat(t);
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
    else
    {
        // Missing primitive type
        assert(false);
        return nullptr;
    }
}

bsqon::Value *TypeGeneratorRandom::generateEnum(const bsqon::EnumType *t)
{
    std::uniform_int_distribution<> ev(0, t->variants.size() - 1);
    auto evpos = ev(rng);
    auto variant = t->variants[evpos];

    return new bsqon::EnumValue(t, g_spos, variant, evpos);
}

bsqon::Value *TypeGeneratorRandom::generateStdEntityType(const bsqon::StdEntityType *t)
{
    std::vector<bsqon::Value *> fieldvals;
    std::transform(t->fields.cbegin(), t->fields.cend(), std::back_inserter(fieldvals), [this](const auto &f)
                   { return this->generateType(this->assembly.lookupTypeKey(f.ftype)); });
    return new bsqon::EntityValue(t, g_spos, std::move(fieldvals));
}

// bsqon::Value *TypeGeneratorRandom::generateListType(const bsqon::ListType *t)
// {
//     const auto oftype = this->assembly.lookupTypeKey(t->oftype);
    

//     return new bsqon::ListValue(t, g_spos, std::move(oftype));
// }

// bsqon::Value *TypeGeneratorRandom::handleConceptType()
// {
//     std::vector<std::string> subtypes;
//     std::map<std::string, std::vector<std::string>> supertypesMap;

//     for (const auto &[key, type] : assembly.typerefs)
//     {
//         // std::cout << "Key: " << key << ", Type Key: " << type->tkey << std::endl;
//         if (!type->supertypes.empty())
//         {
//             for (const auto &supertype : type->supertypes)
//             {
//                 supertypesMap[supertype].push_back(type->tkey);
//             }
//         }
//     }

//     for (const auto &pair : supertypesMap)
//     {
//         for (const auto &item : pair.second)
//         {
//             const bsqon::Type *loadtype = assembly.lookupTypeKey(item);

//             if (loadtype->isUnresolved())
//             {
//                 printf("Invalid 'loadtype' -- %s\n", item);
//                 exit(1);
//             }
//             return this->generateType(loadtype);
//         }
//     }
// }

//Handle partition here

// Enum for Partition Types
typedef enum {
    NEGATIVE_LARGE,
    NEGATIVE_MEDIUM,
    NEGATIVE_SMALL,
    ZERO,
    POSITIVE_SMALL,
    POSITIVE_MEDIUM,
    POSITIVE_LARGE,
    PARTITION_COUNT  
} IntegerPartitionType;

int randomInRange(int min, int max){
    std::mt19937_64 rng; 
    std::uniform_int_distribution<int64_t> iv(min, max);
    int64_t random_value = iv(rng); 
    return random_value;
}

int generateIntValue(IntegerPartitionType type, bool isRandom) {
    switch (type) {
        case NEGATIVE_LARGE:
            return isRandom?randomInRange(-100000, -1000) : -1000;
 
        case NEGATIVE_MEDIUM:
            return isRandom?randomInRange(-999, -100) : -500;
 
        case NEGATIVE_SMALL:
            return isRandom?randomInRange(-99, -1) : -10;
 
        case ZERO:
            return 0; // Zero is fixed
 
        case POSITIVE_SMALL:
            return isRandom?randomInRange(1, 99) : 10;
 
        case POSITIVE_MEDIUM:
            return isRandom?randomInRange(100, 999) : 500;
 
        case POSITIVE_LARGE:
            return isRandom?randomInRange(1000, 100000) : 1000;
 
        default:
            printf("Invalid Partition Type\n");
            return 0;
    }
}

const char* getIntegerPartitionLabel(IntegerPartitionType type) {
    switch (type) {
        case NEGATIVE_LARGE:   return "Negative Large";
        case NEGATIVE_MEDIUM:  return "Negative Medium";
        case NEGATIVE_SMALL:   return "Negative Small";
        case ZERO:             return "Zero";
        case POSITIVE_SMALL:   return "Positive Small";
        case POSITIVE_MEDIUM:  return "Positive Medium";
        case POSITIVE_LARGE:   return "Positive Large";
        default:               return "Unknown";
    }
}

void handleIntegerPartitioning(){
        for (IntegerPartitionType type = NEGATIVE_LARGE; type < PARTITION_COUNT; type = (IntegerPartitionType)(type+1)) {
        // random value for each partition type
        printf("%s (Random): %d\n", getIntegerPartitionLabel(type), generateIntValue(type, true));
 
        // fixed value for each partition type
        printf("%s (Fixed): %d\n", getIntegerPartitionLabel(type), generateIntValue(type, false));
    }
 
}


typedef enum {
    NUMERIC,
    ALPHABETIC,
    ALPHANUMERIC,
    SHORT,
    MEDIUM,
    LONG,
    LOWERCASE,
    UPPERCASE,
    ASCII,
    PARTITION_COUNT_STR
} StringPartitionType;

const char* getStrPartitionLabel(StringPartitionType type) {
    switch (type) {
        case NUMERIC:   return "NUMERIC";
        case ALPHABETIC:  return "ALPHABETIC";
        case ALPHANUMERIC:   return "ALPHANUMERIC";
        case SHORT:  return "SHORT";
        case MEDIUM:   return "MEDIUM";
        case LONG:   return "LONG";
        case LOWERCASE:  return "LOWERCASE";
        case UPPERCASE:   return "UPPERCASE";
        case ASCII:   return "ASCII";
        
        default: return "Unknown";
    }
}

std::string generateRandomStringInRange(const std::string &charset, int minLen, int maxLen)
{

    std::uniform_int_distribution<int64_t> length_dist(minLen, maxLen); 
    std::mt19937_64 rng; 
    int64_t length = length_dist(rng);

    uint8_t* random_bytes = new uint8_t[length];

    for (int64_t i = 0; i < length; ++i) {
        random_bytes[i] = charset[rng() % charset.size()];
    }
    std::string str(reinterpret_cast<char*>(random_bytes), length);
    
    return str;
}


std::string generateStringValue(StringPartitionType type, bool isRandom) {
    std::string charset;
    switch (type) {
        case NUMERIC: 
            charset = "0123456789";
            return isRandom?generateRandomStringInRange(charset, 5, 15):"1000";
 
        case ALPHABETIC: 
            charset = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
            return isRandom?generateRandomStringInRange(charset, 5, 15):"abcRlpdsfsdf";

        case ALPHANUMERIC: 
            charset = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
            return isRandom?generateRandomStringInRange(charset, 5, 15):"abcR10cvcvcxv00";

        case SHORT: 
            charset = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
            return isRandom?generateRandomStringInRange(charset, 1, 5):" ";

        case MEDIUM: 
            charset = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
            return isRandom?generateRandomStringInRange(charset, 10, 10):"abcR1cxv00";

        case LONG: 
            charset = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
            return isRandom?generateRandomStringInRange(charset, 40, 50):"abcR10cvcvcxv00abcR10cvcvcxv00";

        case LOWERCASE: 
            charset = "abcdefghijklmnopqrstuvwxyz";
            return isRandom?generateRandomStringInRange(charset, 5, 15):"abffffhnjkrhjkhr";

        case UPPERCASE: 
            charset = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
            return isRandom?generateRandomStringInRange(charset, 5, 15):"AYRHGHGYYGHGY";


         case ASCII: 
            charset = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
            return isRandom?generateRandomStringInRange(charset, 5, 15):"#$%^&*()?>AYRfdfafsdYGHGY";

        default:
            printf("Invalid Partition Type\n");
            return 0;
    }
}

void handleStringPartitioning(){
        for (StringPartitionType type = NUMERIC; type < PARTITION_COUNT_STR; type = (StringPartitionType)(type+1)) {
        // random value for each partition type
       // printf("%s (Random): %s\n", getStrPartitionLabel(type), generateStringValue(type, true));
        std::cout << getStrPartitionLabel(type) << "(Random):" << generateStringValue(type, true) << std::endl;
 
        // fixed value for each partition type
         std::cout << getStrPartitionLabel(type) << "(Fixed):" << generateStringValue(type, false) << std::endl;
    }
 
}

std::string generateRandomBool()
{
    std::mt19937_64 rng; 
    std::uniform_int_distribution<> bv(0, 1);
    bool random_value = bv(rng) == 1;
  //  std::cout << "Generated bool value: " << (random_value ? "true" : "false") << std::endl;
    return (random_value ? "true" : "false");
}


std::string generateBoolValue(bool isRandom) {
    if(isRandom){
        return generateRandomBool();
    } else {
        return "false";
    }
}

void handleBoolPartitioning(){   
        std::cout << "(Random):" << generateBoolValue(true) << std::endl;
        std::cout << "(Fixed):" << generateBoolValue(false) << std::endl;
}

bsqon::Value *TypeGeneratorRandom::generateType(const bsqon::Type *t)
{
    switch (t->tag)
    {

    case bsqon::TypeTag::TYPE_PRIMITIVE:
    {
        std::cout << "type received: " << t->tkey << std::endl;
        if(t->tkey == "Int"){
            handleIntegerPartitioning();
        } else if(t->tkey == "String"){
            handleStringPartitioning();
        } else if(t->tkey == "Bool"){
            handleBoolPartitioning();
        }
        
        return this->generatePrimitive(static_cast<const bsqon::PrimitiveType *>(t));
    }

    /*
     * TODO: more tags here
     */
    case bsqon::TypeTag::TYPE_STD_ENTITY:
    {
        return this->generateStdEntityType(static_cast<const bsqon::StdEntityType *>(t));
    }
    case bsqon::TypeTag::TYPE_ENUM:
    {
        return this->generateEnum(static_cast<const bsqon::EnumType *>(t));
    }
    case bsqon::TypeTag::TYPE_LIST:
    {
        std::cout << "type received: " << t->tkey << std::endl;
        const auto listType = static_cast<const bsqon::ListType *>(t);
        const auto loadtype = this->assembly.lookupTypeKey(listType->oftype); // Generate two values
    
        std::vector<bsqon::Value *> generatedValues;
        for (int i = 0; i < 5; ++i) {
            generatedValues.push_back(this->generateType(loadtype));
        }
        return new bsqon::EntityValue(t, g_spos, std::move(generatedValues));
    }

    // case bsqon::TypeTag::TYPE_LIST:
    // {
    //     std::cout << "type received: " << t->tkey << std::endl;
    //     const auto listType = static_cast<const bsqon::ListType *>(t);
    //     const auto loadtype = this->assembly.lookupTypeKey(listType->oftype);
    //     return this->generateType(loadtype);
    // }

    // case bsqon::TypeTag::TYPE_STD_CONCEPT:
    // {
    //     //return handleConceptType();
    //     printf("enter here");
    // }
        /*
         * TODO: more tags here
         */
    default:
    {
        // Missing type
        assert(false);
    }
    }
}
