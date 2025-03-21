#include "ai_generator.h"
#include <curl/curl.h>
#include "json.hpp"
using json = nlohmann::json;
static bsqon::SourcePos g_spos = {0, 0, 0, 0};

ValueSetPartition AIValueSetGenerator::generateNone(const bsqon::PrimitiveType *t, const ValueSetGeneratorEnvironment& env)
{
    return ValueSetPartition{ {new ValueComponent(env.path, env.constraints, env.context.completeWithValueType(t), {})} };
}

ValueSetPartition AIValueSetGenerator::generateBool(const bsqon::PrimitiveType *t, const ValueSetGeneratorEnvironment& env, std::vector<bool> values)
{
    std::vector<bsqon::Value*> options;
    for (bool element: values) {
        options.push_back(new bsqon::BoolValue(t, g_spos, element));
    }
    return ValueSetPartition{ {new ValueComponent(env.path, env.constraints, env.context.completeWithValueType(t), options)} };
}

ValueSetPartition AIValueSetGenerator::generateNat(const bsqon::PrimitiveType *t, const ValueSetGeneratorEnvironment& env, std::vector<uint64_t> values)
{
    std::vector<bsqon::Value*> options;
    for (uint64_t element: values) {
        options.push_back(new bsqon::NatNumberValue(t, g_spos, element));
    }
    return ValueSetPartition{ {new ValueComponent(env.path, env.constraints, env.context.completeWithValueType(t), options)} };
}

ValueSetPartition AIValueSetGenerator::generateInt(const bsqon::PrimitiveType *t, const ValueSetGeneratorEnvironment& env, std::vector<int64_t> values)
{
    std::vector<bsqon::Value*> options;
    for (int64_t element: values) {
        options.push_back(new bsqon::IntNumberValue(t, g_spos, element));
    }
    return ValueSetPartition{ {new ValueComponent(env.path, env.constraints, env.context.completeWithValueType(t), options)} };
}

ValueSetPartition AIValueSetGenerator::generateBigNat(const bsqon::PrimitiveType *t, const ValueSetGeneratorEnvironment& env, std::vector<int64_t> values)
{
    std::vector<bsqon::Value*> options;
    for (int64_t element: values) {
        options.push_back(new bsqon::IntNumberValue(t, g_spos, element));
    }
    return ValueSetPartition{ {new ValueComponent(env.path, env.constraints, env.context.completeWithValueType(t), options)} };
}

ValueSetPartition AIValueSetGenerator::generateString(const bsqon::PrimitiveType *t, const ValueSetGeneratorEnvironment& env, std::vector<std::string> values)
{
    std::vector<bsqon::Value*> options;
    for (std::string element: values) {
        options.push_back(bsqon::StringValue::createFromParse(t, g_spos, reinterpret_cast<const uint8_t*>(element.c_str()), element.size()));
    }
    return ValueSetPartition{ {new ValueComponent(env.path, env.constraints, env.context.completeWithValueType(t), options)} };
}

ValueSetPartition AIValueSetGenerator::generateFloat(const bsqon::PrimitiveType *t, const ValueSetGeneratorEnvironment& env, std::vector<float> values)
{
    std::vector<bsqon::Value*> options;
    for (int64_t element: values) {
        options.push_back(new bsqon::FloatNumberValue(t, g_spos, element));
    }
    return ValueSetPartition{ {new ValueComponent(env.path, env.constraints, env.context.completeWithValueType(t), options)} };
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

template <typename T>
std::vector<T> getValuesFromJson(const json& extractedData, const std::string& field) {
    if (!extractedData.contains(field)) {
        std::cout<<"INVALID Fields"<<std::endl;
    }
    std::vector<T> values = extractedData[field].get<std::vector<T>>();
   
    if (values.empty()) {
        throw std::runtime_error("No values available for field: " + field);
    }
   
    return values;
}

ValueSetPartition AIValueSetGenerator::generatePrimitive(const bsqon::PrimitiveType *t, const ValueSetGeneratorEnvironment& env)
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

    Given a variable with path named "{{path}}" with data type "{{type}}" and format "{{format}}", generate a JSON object containing ONLY test values strictly matching the specified data type and format, and within acceptable ranges.

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

    std::string fname = env.context.forfield->fname;
    std::string path = env.path;
    prompt.replace(prompt.find("{{path}}"), 8, path);
    prompt.replace(prompt.find("{{name}}"), 8, fname);
    prompt.replace(prompt.find("{{type}}"), 8, tk);

    std::cout<< prompt << std::endl;
    std::string OPENAI_KEY = getenv("OPENAI_CHATGPT_API_KEY") ? getenv("OPENAI_CHATGPT_API_KEY") : "";
    std::string DEEPSEEK_API_KEY = getenv("DEEPSEEK_API_KEY") ? getenv("DEEPSEEK_API_KEY") : "";

    std::string OPENAI_URL = "https://api.openai.com/v1/chat/completions";
    std::string DEEPSEEK_URL = "https://api.deepseek.com/chat/completions";

    std::string OPENAI_MODEL = "gpt-4o";
    std::string DEEPSEEK_MODEL = "deepseek-chat";

    std::string apiKey = OPENAI_KEY;
    std::string url = OPENAI_URL;
    std::string model = OPENAI_MODEL;
   
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

    if (tk == "None")
    {
        return this->generateNone(t, env);
    }
    else if (tk == "Bool")
    {
        auto& data = extractedData[fname];
        for (size_t i = 0; i < data.size(); ++i) {
            std::string value = data[i].get<std::string>();
            std::string lowerStr = value;
            std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
            data[i] = (lowerStr == "true");
            
        }
        std::vector<bool> values = getValuesFromJson<bool>(extractedData, fname);
        return this->generateBool(t, env, values);
    }
    else if (tk == "Nat")
    {
        auto& data = extractedData[fname];
        for (size_t i = 0; i < data.size(); ++i) {
            std::string value = data[i].get<std::string>();
            if (value.empty() || value.back() != 'n') {
                std::cerr << value << " is incorrect formatted" << std::endl;
                continue;
            }
            value.pop_back();
            data[i] = std::stoull(value);
        }
        std::vector<uint64_t> values = getValuesFromJson<uint64_t>(extractedData, fname);
        return this->generateNat(t, env, values);
    }
    else if (tk == "Int")
    {
        auto& data = extractedData[fname];
        for (size_t i = 0; i < data.size(); ++i) {
            std::string value = data[i].get<std::string>();
            if (value.empty() || value.back() != 'i') {
                std::cerr << value << " is incorrect formatted" << std::endl;
                continue;
            }
            value.pop_back();
            data[i] = std::stoll(value);
        }
        std::vector<int64_t> values = getValuesFromJson<int64_t>(extractedData, fname);
        return this->generateInt(t, env, values);
    }
    else if(tk == "String") 
    {
        std::vector<std::string> values = getValuesFromJson<std::string>(extractedData, fname);
        return this->generateString(t, env, values);
    }else if(tk == "Float") 
    {
        auto& data = extractedData[fname];
        for (size_t i = 0; i < data.size(); ++i) {
            std::string value = data[i].get<std::string>();
            if (value.empty() || value.back() != 'f') {
                std::cerr << value << " is incorrect formatted" << std::endl;
                continue;
            }
            value.pop_back();
            data[i] = std::stof(value);
        }
        std::vector<float> values = getValuesFromJson<float>(extractedData, fname);
        return this->generateFloat(t, env, values);
    }else if(tk == "BigNat") {
    
        auto& data = extractedData[fname];
        for (size_t i = 0; i < data.size(); ++i) {
            std::string value = data[i].get<std::string>();
            if (value.empty() || value.back() != 'N') {
                std::cerr << value << " is incorrect formatted" << std::endl;
                continue;
            }
            value.pop_back();
            data[i] = std::stoll(value);
        }
        std::vector<int64_t> values = getValuesFromJson<int64_t>(extractedData, fname);
        return this->generateBigNat(t, env, values);
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
    }
}

// ValueSetPartition AIValueSetGenerator::generateEnum(const bsqon::EnumType *t, const ValueSetGeneratorEnvironment& env)
// {
//     std::uniform_int_distribution<> ev(0, t->variants.size() - 1);
//     auto evpos = ev(rng);
//     auto variant = t->variants[evpos];

//     return new bsqon::EnumValue(t, g_spos, variant, evpos);
// }

std::vector<std::vector<bsqon::Value*>> generateCartesianProduct(
    const std::vector<std::vector<bsqon::Value*>>& lists,
    size_t maxResults = 500
) {
    std::vector<std::vector<bsqon::Value*>> result;
    if (lists.empty()) return result;

    std::vector<size_t> indices(lists.size(), 0);
    bool done = false;

    while (!done && result.size() < maxResults) {
        std::vector<bsqon::Value*> combination;
        for (size_t i = 0; i < lists.size(); ++i) {
            combination.push_back(lists[i][indices[i]]);
        }
        result.push_back(combination);

        for (size_t i = lists.size(); i-- > 0;) {
            if (++indices[i] < lists[i].size()) {
                break;
            }
            indices[i] = 0;
            if (i == 0) done = true;
        }
    }

    return result;
}
ValueSetPartition AIValueSetGenerator::generateStdEntityType(const bsqon::StdEntityType *t, const ValueSetGeneratorEnvironment& env)
{
    auto tctx = env.context.extendWithEnclosingType(t);

    std::vector<std::vector<bsqon::Value*>> optionLists;
    
    for (const auto &f : t->fields) {
        auto tenv = env.step(pathAccessField(env.path, f.fname), env.constraints, tctx.extendForField(f));
        const bsqon::Type* fieldType = this->assembly.lookupTypeKey(f.ftype);
        ValueSetPartition partition = this->generateType(fieldType, tenv);

        std::vector<bsqon::Value*> fieldOptions;
        for (const auto &component : partition.components) {
            fieldOptions.insert(fieldOptions.end(), component->options.begin(), component->options.end());
        }

        optionLists.push_back(fieldOptions);
    }

    std::vector<std::vector<bsqon::Value*>> entityCombinations = generateCartesianProduct(optionLists);

    std::vector<ValueComponent*> newComponents;
    for (const auto &combination : entityCombinations) {
        bsqon::EntityValue* entityValue = new bsqon::EntityValue(t, g_spos, std::move(combination));
        std::vector<bsqon::Value*> entityOptions = {entityValue};  // Wrap in vector

        newComponents.push_back(new ValueComponent(env.path, env.constraints, env.context.completeWithValueType(t), entityOptions));
    }

    return ValueSetPartition{newComponents};
}

ValueSetPartition AIValueSetGenerator::generateType(const bsqon::Type *t, const ValueSetGeneratorEnvironment& env)
{
    switch (t->tag)
    {

    case bsqon::TypeTag::TYPE_PRIMITIVE:
    {
        return this->generatePrimitive(static_cast<const bsqon::PrimitiveType *>(t), env);
    }

    /*
     * TODO: more tags here
     */
    case bsqon::TypeTag::TYPE_STD_ENTITY:
    {
        return this->generateStdEntityType(static_cast<const bsqon::StdEntityType *>(t), env);
    }
    case bsqon::TypeTag::TYPE_LIST:
    {
        const auto listType = static_cast<const bsqon::ListType *>(t);
        const auto loadtype = this->assembly.lookupTypeKey(listType->oftype);
        std::vector<ValueSetPartition> generatedValues;
        for (int i = 0; i < 5; ++i) {
            generatedValues.push_back(this->generateType(loadtype, env));
        }
        return ValueSetPartition::punion(generatedValues);
    }

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
