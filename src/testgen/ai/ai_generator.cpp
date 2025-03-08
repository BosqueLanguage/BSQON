#include "ai_generator.h"
#include <curl/curl.h>
#include "json.hpp"
using json = nlohmann::json;
static bsqon::SourcePos g_spos = {0, 0, 0, 0};

size_t getRandom(size_t size) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dis(0, size - 1);
    return dis(gen);
}

bsqon::Value *AITypeGenerator::generateNone(const bsqon::PrimitiveType *t, Context ctx)
{
    return new bsqon::NoneValue(t, g_spos);
}

bsqon::Value *AITypeGenerator::generateBool(const bsqon::PrimitiveType *t, Context ctx, bool random_value)
{
    return new bsqon::BoolValue(t, g_spos, random_value);
}

bsqon::Value *AITypeGenerator::generateNat(const bsqon::PrimitiveType *t, Context ctx, uint64_t random_value)
{
    std::uniform_int_distribution<uint64_t> nv(0, bsqon::Type::MAX_SAFE_NAT);
    return new bsqon::NatNumberValue(t, g_spos, random_value);
}

bsqon::Value *AITypeGenerator::generateInt(const bsqon::PrimitiveType *t, Context ctx, int64_t random_value)
{
    return new bsqon::IntNumberValue(t, g_spos, random_value);
}

bsqon::Value *AITypeGenerator::generateBigNat(const bsqon::PrimitiveType *t, Context ctx, int64_t random_value)
{
    return new bsqon::IntNumberValue(t, g_spos, random_value);
}

bsqon::Value *AITypeGenerator::generateString(const bsqon::PrimitiveType *t, Context ctx, std::string random_value)
{
    return bsqon::StringValue::createFromParse(t, g_spos, reinterpret_cast<const uint8_t*>(random_value.c_str()), random_value.size());
}

bsqon::Value *AITypeGenerator::generateFloat(const bsqon::PrimitiveType *t, Context ctx, float val)
{
    return new bsqon::FloatNumberValue(t, g_spos, val);
}

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

std::string makeAPIRequest(const std::string& apiKey, const std::string& url, const std::string& model, const std::string& prompt) {
    CURL* curl = curl_easy_init();

    std::string responseString;
    json requestJson = {
        {"model", model},
        {"messages", {
            {{"role", "user"}, {"content", prompt}}
        }}
    };

    std::string jsonPayload = requestJson.dump();
    
    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, ("Authorization: Bearer " + apiKey).c_str());

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonPayload.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseString);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "cURL Error: " << curl_easy_strerror(res) << std::endl;
    }

    // Cleanup
    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);

    return responseString;
}

int selectRandomValue(const std::vector<int>& values) {
    if (values.empty()) {
        throw std::runtime_error("No values available to select.");
    }
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dist(0, values.size() - 1);

    return values[dist(gen)];
}

std::string combineJsonObjects(const std::vector<json>& jsonArray) {
    json combined;
    for (const auto&obj:jsonArray) {
        for (auto it = obj.begin(); it != obj.end(); ++it) {
            combined[it.key()] = it.value();
        }
    }
    return combined.dump();
}

template <typename T>
T getRandomValueFromJson(const json& extractedData, const std::string& field) {
    if (!extractedData.contains(field)) {
        std::cout<<"INVALID Fields"<<std::endl;
    }
    std::vector<T> values = extractedData[field].get<std::vector<T>>();
   
    if (values.empty()) {
        throw std::runtime_error("No values available for field: " + field);
    }
    size_t idx = getRandom(values.size());
    return values[idx];
}

bsqon::Value *AITypeGenerator::generatePrimitive(const bsqon::PrimitiveType *t, Context ctx)
{
    auto tk = t->tkey;
    std::unordered_map<std::string, std::string> typeFormatInstructions = {
        {"None","null"},
        {"Bool", "'true'|'false'"},
        {"Nat", "Unsigned integer literal (uint64_t) ending with 'n' (range: '0n' to '18446744073709551615n')"},
        {"Int", "Signed integer literal (int64_t), ending with 'i' (range: '-9223372036854775808i' to '9223372036854775807i')."},
        {"BigNat", "Signed integer literal (int64_t), ending with 'N' (range: '-9223372036854775808i' to '9223372036854775807i')."},
        {"BigInt", "Signed integer literal (int64_t), ending with 'N' (range: '-9223372036854775808N' to '9223372036854775807N')."},
        {"Float", "Floating-point literal (double), within standard C++ double range, ending with 'f' ('3.14f')"},
        {"Rational", "<num>/<denom>R (1/3R)"},
        {"Decimal", "[-]<digits>.[digits]d (3.1415926535d)"},
        {"DecimalDegree", "<deg>dd (37.7749dd)"},
        {"LatLongCoordinate", "[lat:<lat>, lng:<lng>] ([37.7749, -122.4194])"},
        {"Complex", "(<real>,<imag>i) (3+4i)"},
        {"String", "\"<text>\" (\"hello\")"},
        {"CString", "c\"<text>\" (c\"hello\\0\")"},
        {"ByteBuffer", "0x<hex> (0x1A3F)"},
        {"UUIDv4", "uuid4\"...\""},
        {"UUIDv7", "uuid7\"...\""},
        {"SHAContentHash", "sha256\"...\""},
        {"TZDateTime", "<ISO8601> (2023-03-28T14:30:00Z)"},
        {"TAITime", "<TAI-format>"},
        {"PlainDate", "YYYY-MM-DD (2023-09-27)"},
        {"PlainTime", "HH:MM:SS (14:30:00)"},
        {"LogicalTime", "<timestamp>l (1633024800l)"},
        {"ISOTimestamp", "<ISO8601> (2023-09-27T14:30:00Z)"},
        {"DeltaDateTime", "P<date>T<time> (P3DT12H)"},
        {"DeltaSeconds", "<seconds>ds (3600ds)"},
        {"DeltaLogicalTime", "<diff>dl (100dl)"},
        {"DeltaISOTimestamp", "P<date>T<time> (PT1H)"},
        {"UnicodeRegex", "/pattern/u (/\\w+/u)"},
        {"CRegex", "a/pattern/ (a/^[a-z]+$/)"},
        {"PathRegex", "p/pattern/ (p/.*\\.txt/)"},
        {"Path", "p\"/path\" (p\"/home/user\")"},
        {"PathItem", "f\"name\" (f\"file.txt\")"},
        {"Glob", "g\"pattern\" (g\"*.txt\")"}
    };

   const std::string INPUT_SPACE_TEST_PROMPT = R"(
    Act as an expert in REST API input space analysis.

    Given a variable named "{{name}}" with data type "{{type}}" and format "{{format}}", generate a JSON object containing ONLY test values strictly matching the specified data type and format, and within acceptable ranges.

    Requirements:
    - Each value must represent a distinct, non-overlapping valid input scenario.
    - Do NOT explicitly label or describe scenarios; demonstrate implicit scenario coverage through diverse values.

    Your response must ONLY include a valid JSON object structured exactly as follows:
    {
        "{{name}}": ["valid test value 1", "valid test value 2", ...]
    }

    STRICT RULES:
    - NEVER include invalid or edge-case test values.
    - NEVER include commentary, explanations, or additional text outside of this JSON.
    )";

    std::string prompt = INPUT_SPACE_TEST_PROMPT;
    if (typeFormatInstructions.find(tk) != typeFormatInstructions.end()) {
        prompt.replace(prompt.find("{{format}}"), 10, typeFormatInstructions[tk]);
    } else {
        std::cerr << "Failed to determine type format" << std::endl;
    }

    prompt.replace(prompt.find("{{name}}"), 8, ctx.fields);
    prompt.replace(prompt.find("{{name}}"), 8, ctx.fields);
    prompt.replace(prompt.find("{{type}}"), 8, tk);

    std::cout<< prompt << std::endl;
    std::string OPENAI_KEY = getenv("OPENAI_CHATGPT_API_KEY") ? getenv("OPENAI_CHATGPT_API_KEY") : "";
    std::string DEEPSEEK_API_KEY = getenv("DEEPSEEK_API_KEY") ? getenv("DEEPSEEK_API_KEY") : "";

    std::string OPENAI_URL = "https://api.openai.com/v1/chat/completions";
    std::string DEEPSEEK_URL = "https://api.deepseek.com/chat/completions";

    std::string OPENAI_MODEL = "gpt-4o";
    std::string DEEPSEEK_MODEL = "deepseek-chat";

    // Choose which API to use
    std::string apiKey = OPENAI_KEY;
    std::string url = OPENAI_URL;
    std::string model = OPENAI_MODEL;
   
    // Make the API request
    std::string response = makeAPIRequest(apiKey, url, model, prompt);
    std::cout<<response<<std::endl;
    json responseJson = json::parse(response);
   
    std::string contentString = responseJson["choices"][0]["message"]["content"];
    size_t start = contentString.find("{");
    size_t end = contentString.rfind("}");
    if (start == std::string::npos || end == std::string::npos) {
        throw std::runtime_error("Invalid JSON content format");
    }

    std::string extractedJson = contentString.substr(start, end - start + 1);
    
    json extractedData = json::parse(extractedJson);
    std::cout<<extractedData.dump(8)<<std::endl;
    // std::cout << "Response: " << response << std::endl;
    // std::vector<int> totalItems = extractTotalItems(response, ctx);
    // if (!totalItems.empty()) {
    //     int randomValue = selectRandomValue(totalItems);
    //     std::cout << "Randomly Selected Value: " << randomValue << std::endl;
    // } else {
    //     std::cerr << "Failed to extract totalItems." << std::endl;
    // }
    if (tk == "None")
    {
        return this->generateNone(t, ctx);
    }
    else if (tk == "Bool")
    {
        auto& data = extractedData[ctx.fields];
        for (size_t i = 0; i < data.size(); ++i) {
            std::string value = data[i].get<std::string>();
            std::string lowerStr = value;
            std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
            data[i] = (lowerStr == "true");
            
        }
        bool random_val = getRandomValueFromJson<bool>(extractedData, ctx.fields);
        return this->generateBool(t, ctx, random_val);
    }
    else if (tk == "Nat")
    {
        auto& data = extractedData[ctx.fields];
        for (size_t i = 0; i < data.size(); ++i) {
            std::string value = data[i].get<std::string>();
            if (value.empty() || value.back() != 'n') {
                std::cerr << value << " is incorrect formatted" << std::endl;
                continue;
            }
            value.pop_back();
            data[i] = std::stoull(value);
        }
        uint64_t random_val = getRandomValueFromJson<uint64_t>(extractedData, ctx.fields);
        return this->generateNat(t, ctx, random_val);
    }
    else if (tk == "Int")
    {
        auto& data = extractedData[ctx.fields];
        for (size_t i = 0; i < data.size(); ++i) {
            std::string value = data[i].get<std::string>();
            if (value.empty() || value.back() != 'i') {
                std::cerr << value << " is incorrect formatted" << std::endl;
                continue;
            }
            value.pop_back();
            data[i] = std::stoll(value);
        }
        int64_t random_val = getRandomValueFromJson<int64_t>(extractedData, ctx.fields);
        return this->generateInt(t, ctx, random_val);
    }
    else if(tk == "String") 
    {
        std::string random_val = getRandomValueFromJson<std::string>(extractedData, ctx.fields);
        return this->generateString(t, ctx, random_val);
    }else if(tk == "Float") 
    {
        auto& data = extractedData[ctx.fields];
        for (size_t i = 0; i < data.size(); ++i) {
            std::string value = data[i].get<std::string>();
            if (value.empty() || value.back() != 'f') {
                std::cerr << value << " is incorrect formatted" << std::endl;
                continue;
            }
            value.pop_back();
            data[i] = std::stof(value);
        }
        float random_val = getRandomValueFromJson<float>(extractedData, ctx.fields);
        return this->generateFloat(t, ctx, random_val);
    }else if(tk == "BigNat") {
    
        auto& data = extractedData[ctx.fields];
        for (size_t i = 0; i < data.size(); ++i) {
            std::string value = data[i].get<std::string>();
            if (value.empty() || value.back() != 'N') {
                std::cerr << value << " is incorrect formatted" << std::endl;
                continue;
            }
            value.pop_back();
            data[i] = std::stoll(value);
        }
        int64_t random_val = getRandomValueFromJson<int64_t>(extractedData, ctx.fields);
        return this->generateBigNat(t, ctx, random_val);
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

bsqon::Value *AITypeGenerator::generateEnum(const bsqon::EnumType *t, Context ctx)
{
    std::uniform_int_distribution<> ev(0, t->variants.size() - 1);
    auto evpos = ev(rng);
    auto variant = t->variants[evpos];

    return new bsqon::EnumValue(t, g_spos, variant, evpos);
}

bsqon::Value *AITypeGenerator::generateStdEntityType(const bsqon::StdEntityType *t, Context ctx)
{
    std::vector<bsqon::Value *> fieldvals;

    std::transform(t->fields.cbegin(), t->fields.cend(), std::back_inserter(fieldvals),
                   [this, ctx](const auto &f) mutable
                   {
                       Context newCtx = ctx.clone();
                       newCtx.fields = newCtx.fields + f.fname;

                       return this->generateType(this->assembly.lookupTypeKey(f.ftype), newCtx);
                   });

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

bsqon::Value *AITypeGenerator::generateType(const bsqon::Type *t, Context ctx)
{
    

    switch (t->tag)
    {

    case bsqon::TypeTag::TYPE_PRIMITIVE:
    {
        std::cout << "type received: " << t->tkey << std::endl;
        return this->generatePrimitive(static_cast<const bsqon::PrimitiveType *>(t), ctx);
    }

    /*
     * TODO: more tags here
     */
    case bsqon::TypeTag::TYPE_STD_ENTITY:
    {
        return this->generateStdEntityType(static_cast<const bsqon::StdEntityType *>(t), ctx);
    }
    case bsqon::TypeTag::TYPE_ENUM:
    {
        return this->generateEnum(static_cast<const bsqon::EnumType *>(t), ctx);
    }
    case bsqon::TypeTag::TYPE_LIST:
    {
        std::cout << "type received: " << t->tkey << std::endl;
        const auto listType = static_cast<const bsqon::ListType *>(t);
        const auto loadtype = this->assembly.lookupTypeKey(listType->oftype);
        std::vector<bsqon::Value *> generatedValues;
        for (int i = 0; i < 5; ++i) {
            generatedValues.push_back(this->generateType(loadtype, ctx));
        }
        return new bsqon::ListValue(t, g_spos, std::move(generatedValues));
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
