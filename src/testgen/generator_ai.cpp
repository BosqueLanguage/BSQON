#include "generator_ai.h"
#include <curl/curl.h>
#include "json.hpp"

using json = nlohmann::json;

static bsqon::SourcePos g_spos = {0, 0, 0, 0};

static std::string g_std_prompt_instructions = R"(
    Additional Context:
    - Input Space Partition: A systematic decomposition of the input domain into a set of non-overlapping subsets (partitions), where each subset represents a distinct subset of possible input values that can be tested independently to ensure comprehensive coverage of the input space.
    - Domain: The complete set of all possible input values for a specific variable or parameter, defined by its data type, format constraints, and acceptable value ranges.
    - The API that this value is being generated for has the following signature:
    {{signature}}
    - The value is on the path: {{path}}
    Assume the most likely domain-specific interpretation of the variable name and context.
    Strictly follow inferred semantics, excluding technically valid but unrealistic values.
    Inference Guidance (implicit, not rigid):
    - Consider the variable name ( {{path}} ) and function signature ( {{signature}} ) as hints for likely semantics. 
    - Prioritize values that reflect realistic usage while allowing for generic interpretations.

    - Cover variations in length, character composition, and edge cases. 
    Constraints (Self-Imposed):
    - Test values must match the following regular expression: {{expression}}
    - If the above hints suggest a **specific structure**, enforce it strictly.  
    - Never include invalid values 
    

    Requirements:
    - Each value must represent a distinct, non-overlapping partition of the valid input values.
    - For each generated value, mentally walk through how it satisfies a different partition of the input space, and select examples accordingly.
    - You may silently reason through variable semantics, common formats, edge cases, and patterns before outputting the final list.

    Think step-by-step before giving your answer.
    Use a small but semantically diverse set of values. Assume that bugs can often be exposed by minimal input variations, in line with the small model hypothesis
    Your response must ONLY include a valid JSON array structured exactly as follows:
    [valid test value 1, valid test value 2, ...]

    STRICT RULES:
    - NEVER include invalid or malformed test values.
    - NEVER include commentary, explanations, or additional text outside of this JSON.
)";

static std::string g_prompt_naked_arg = R"(
    Act as an expert in API input space analysis for black-box test generation.

    Given a variable named "{{name}}" with data type "{{type}}" with format "{{format}}", generate a JSON array containing ONLY test values strictly matching the specified data type and format, and within acceptable ranges.
    {{one_shot}}
    {{std_reqs}}
    )";

static std::string g_prompt_naked_arg_index = R"(
    Act as an expert in REST API input space analysis.
    
    Given a variable named "{{name}}" that is an array of type "{{type}}" elements with format "{{format}}", generate a JSON array containing ONLY test values strictly matching the specified array element type and format, and within acceptable ranges.
    {{one_shot}}

    {{std_reqs}}
    )";

static std::string g_prompt_member_field = R"(
    Act as an expert in REST API input space analysis.
    
    Given a field, named "{{fname}}" in a type named "{{tname}}", with data type "{{type}}" and format "{{format}}", generate a JSON array containing ONLY test values strictly matching the specified data type and format, and within acceptable ranges.
    {{one_shot}}

    {{std_reqs}}
    )";

static std::string g_prompt_member_field_index = R"(
    Act as an expert in REST API input space analysis.
        
    Given a field, named "{{fname}}" in a type named "{{tname}}", that is an array of type "{{type}}" elements with format "{{format}}", generate a JSON array containing ONLY test values strictly matching the specified array element type and format, and within acceptable ranges.
    {{one_shot}}
    {{std_reqs}}
    )";

static std::string one_shot = R"(Q: Variable named "code" with type CustomID and format uppercase letters and digits (regex: /[A-Z0-9]{9}/c)
A: First, understand the constraint. The regular expression /[A-Z0-9]{9}/c means each value must:
- Be exactly 9 characters long
- Contain only uppercase letters (A–Z) and digits (0–9)
- No lowercase, symbols, or spaces

To create diverse but valid values, we partition based on:
- Character balance: all letters, all digits, mixed
- Position variation: letter/digit sequences at different places
- Common-looking structures vs. random valid ones

Examples:
- "A1B2C3D4E" — alternating letter/digit
- "123456789" — all digits
- "ABCDEFGHI" — all letters
- "Z9Z9Z9Z9Z" — patterned
- "9X8Y7Z6W5" — reverse random

All of these:
- Are exactly 9 characters
- Use only A–Z and 0–9
- Show semantic + structural variety

So the answer is: ["A1B2C3D4E", "123456789", "ABCDEFGHI", "Z9Z9Z9Z9Z", "9X8Y7Z6W5"])";

static std::string g_api_json = R"(
    {
    "contents": 
        [{
        "parts": 
            [{
            "text": 
                "[PROMPT]"
            }] 
        }],
    "generationConfig": 
        { 
            "response_mime_type": "application/json", 
            "response_schema": { "type": "array", "items": "[TYPE_INFO]" }
        }
    }
    )";

static std::unordered_map<std::string, std::string> g_typeFormatInstructions = {
    {"None","null"},
    {"Bool", "'true'|'false'"},
    {"Nat", "Unsigned integer literal (uint32_t)"},
    {"Int", "Signed integer literal (int32_t)"},
    {"BigNat", "Unsigned big integer literal (uint64_t)"},
    {"BigInt", "Signed big integer literal (int64_t)"},
    {"Float", "Floating-point literal"},
    {"Rational", "<num>/<denom>R (1/3R)"},
    {"Decimal", "[-]<digits>.[digits]d (3.1415926535d)"},
    {"DecimalDegree", "<deg>dd (37.7749dd)"},
    {"LatLongCoordinate", "[lat:<lat>, lng:<lng>] ([37.7749, -122.4194])"},
    {"Complex", "(<real>,<imag>i) (3+4i)"},
    {"String", "A string that may contain any unciode characters"},
    {"CString", "A string that consists only of printable ASCII characters"},
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


static std::unordered_map<std::string, std::string> g_typeJSONFormat = {
    {"Nat", "{ \"type\": \"integer\", \"format\": \"int32\" }"},
    {"Int", "{ \"type\": \"integer\", \"format\": \"int32\" }"},
    {"BigNat", "{ \"type\": \"integer\", \"format\": \"int32\" }"},
    {"BigInt", "{ \"type\": \"integer\", \"format\": \"int32\" }"},
    {"Float", "{ \"type\": \"number\", \"format\": \"double\" }"},
    
    {"String", "{ \"type\": \"string\" }"},
    {"CString", "{ \"type\": \"string\" }"}
};

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) 
{
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

std::string makeAPIRequest(std::string model, const std::string& apiKey, const std::string& url, json payload) 
{
    CURL* curl = curl_easy_init();

    std::string responseString;
    std::string jsonPayload = payload.dump();
    
    struct curl_slist* headers = nullptr;

    headers = curl_slist_append(headers, "Content-Type: application/json");
    if (model != "Gemini") {
        headers = curl_slist_append(headers, ("Authorization: Bearer " + apiKey).c_str());
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    } else {
        curl_easy_setopt(curl, CURLOPT_URL, (url + "?key=" + apiKey).c_str());
    }
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

std::string buildPromptFor(const bsqon::PrimitiveType* t, const ValueSetGeneratorEnvironment& env, std::string formatinstructions, std::string signature) 
{
    std::string prompt;

    if(env.context.forfield.has_value()) {
        prompt = env.context.forindex.has_value() ? g_prompt_member_field_index : g_prompt_member_field;

        prompt.replace(prompt.find("{{fname}}"), 9, env.context.forfield.value().fname);
        prompt.replace(prompt.find("{{tname}}"), 9, env.context.intype.value()->tkey);
    }
    else {
        prompt = env.context.forindex.has_value() ? g_prompt_naked_arg_index : g_prompt_naked_arg;

        prompt.replace(prompt.find("{{name}}"), 8, env.path);
    }
    prompt.replace(prompt.find("{{one_shot}}"), 12, one_shot);

    prompt.replace(prompt.find("{{std_reqs}}"), 12, g_std_prompt_instructions);
    
    prompt.replace(prompt.find("{{path}}"), 8, env.path);
    prompt.replace(prompt.find("{{signature}}"), 13, signature);
    prompt.replace(prompt.find("{{path}}"), 8, env.path);
    prompt.replace(prompt.find("{{signature}}"), 13, signature);

    prompt.replace(prompt.find("{{format}}"), 10, formatinstructions);

    if(!env.context.oftype.has_value()) {
        prompt.replace(prompt.find("{{type}}"), 8, t->tkey);
    }
    else {
        auto ttype = env.context.oftype.value();

        auto ttinfo = ttype->tkey;
        if(ttype->optOfValidator.has_value()) {
            auto vinfo = ttype->optOfValidator.value().first;
            ttinfo += " that is accepted by the regular expression " + std::string(vinfo.cbegin(), vinfo.cend());
        }

        prompt.replace(prompt.find("{{type}}"), 8, ttinfo);
    }

    return prompt;
}

json callGeminiAPIWithPrompt(std::string prompt, std::string jsontype)
{
    std::string apiKey = std::string(getenv("GEMINI_API_KEY"));
    std::string url = "https://generativelanguage.googleapis.com/v1beta/models/gemini-2.0-flash:generateContent";

    json requestJson = json::parse(g_api_json);
    requestJson["contents"][0]["parts"][0]["text"] = prompt;
    requestJson["generationConfig"]["response_schema"]["items"] = json::parse(jsontype);

    //TODO: Temp debugging output
    std::cout << "---- Requesting with ----" << std::endl << requestJson.dump(8) << std::endl;

    std::string response = makeAPIRequest("Gemini", apiKey, url, requestJson);
    json responseJson = json::parse(response);

    //TODO: Temp debugging output
    //std::cout << "---- Responded with ----" << std::endl << responseJson.dump(8) << std::endl;

    return json::parse(responseJson["candidates"][0]["content"]["parts"][0]["text"].get<std::string>());
}

json callAPIWithPrompt(std::string prompt)
{
    //TODO: Temp debugging output
    std::cout << "---- Prompting with ----" << std::endl << prompt << std::endl;

    std::string OPENAI_KEY = getenv("OPENAI_CHATGPT_API_KEY") ? getenv("OPENAI_CHATGPT_API_KEY") : "";
    std::string DEEPSEEK_API_KEY = getenv("DEEPSEEK_API_KEY") ? getenv("DEEPSEEK_API_KEY") : "";

    std::string OPENAI_URL = "https://api.openai.com/v1/chat/completions";
    std::string DEEPSEEK_URL = "https://api.deepseek.com/chat/completions";

    std::string OPENAI_MODEL = "gpt-4o";
    std::string DEEPSEEK_MODEL = "deepseek-chat";

    std::string apiKey = DEEPSEEK_API_KEY;
    std::string url = DEEPSEEK_URL;
    std::string model = DEEPSEEK_MODEL;
   
    json requestJson = {
        {"model", model},
        {"messages", {
            {{"role", "user"}, {"content", prompt}}
        }}
    };

    std::string response = makeAPIRequest("DEEPSEEK", apiKey, url, requestJson);
    json responseJson = json::parse(response);
   
    std::string contentString = responseJson["choices"][0]["message"]["content"];
    size_t start = contentString.find("[");
    size_t end = contentString.rfind("]");
    if (start == std::string::npos || end == std::string::npos) {
        throw std::runtime_error("Invalid JSON content format");
    }

    std::string extractedJson = contentString.substr(start, end - start + 1);    
    json extractedData = json::parse(extractedJson);

    //TODO: Temp debugging output
    std::cout << "---- Responded with ----" << std::endl << extractedData.dump(8) << std::endl;

    return extractedData;
}

template <typename T>
std::vector<T> getValuesFromJson(const json& extractedData) {
    std::vector<T> values;
    std::transform(extractedData.begin(), extractedData.end(), std::back_inserter(values), [](const json& jv) { 
        return jv.get<T>(); 
    });

    return values;
}

template <typename T>
std::vector<T> runAIGenCall(const AIValueGenerator* aigen, const ValueSetGeneratorEnvironment& env, const bsqon::PrimitiveType* t) {
    auto prompt = buildPromptFor(t, env, g_typeFormatInstructions[t->tkey], aigen->sigstr);
    auto jfmt = g_typeJSONFormat[t->tkey];

    json res;
    if(aigen->model == AIModelOption::GEMINI) {
        res = callGeminiAPIWithPrompt(prompt, jfmt);
    }
    else {
        res = callAPIWithPrompt(prompt);
    }

    return getValuesFromJson<T>(res);
}

void AIValueGenerator::generateNone(const bsqon::PrimitiveType* t, const ValueSetGeneratorEnvironment& env, ValueComponent* vc)
{
    vc->options.push_back(new bsqon::NoneValue(t, g_spos));
}

void AIValueGenerator::generateBool(const bsqon::PrimitiveType* t, const ValueSetGeneratorEnvironment& env, ValueComponent* vc)
{
    vc->options.push_back(new bsqon::BoolValue(t, g_spos, true));
    vc->options.push_back(new bsqon::BoolValue(t, g_spos, false));
}

void AIValueGenerator::generateNat(const bsqon::PrimitiveType *t, const ValueSetGeneratorEnvironment& env, ValueComponent* vc)
{
    if (!vc->context.forspecial.has_value()) {
        vc->options.push_back(new bsqon::NatNumberValue(t, g_spos, 0));
        vc->options.push_back(new bsqon::NatNumberValue(t, g_spos, 1));
        vc->options.push_back(new bsqon::NatNumberValue(t, g_spos, 2));
        vc->options.push_back(new bsqon::NatNumberValue(t, g_spos, 3));

        std::vector<uint64_t> values = runAIGenCall<uint64_t>(this, env, t);
        std::transform(values.begin(), values.end(), std::back_inserter(vc->options),
                       [t](uint64_t v) { return new bsqon::NatNumberValue(t, g_spos, v); });
    }
    else {
        auto sname = vc->context.forspecial.value();
        if (sname == u8"length") {
            for (size_t i = 0; i <= MAX_TEST_COLLECTION_COUNT; ++i) {
                vc->options.push_back(new bsqon::NatNumberValue(t, g_spos, i));
            }
        }
        else {
            assert(false);
        }
    }
}

void AIValueGenerator::generateInt(const bsqon::PrimitiveType *t, const ValueSetGeneratorEnvironment& env, ValueComponent* vc)
{
    std::vector<int64_t> values = runAIGenCall<int64_t>(this, env, t);
    std::transform(values.begin(), values.end(), std::back_inserter(vc->options), [t](int64_t v) { return new bsqon::IntNumberValue(t, g_spos, v); });
}

void AIValueGenerator::generateBigNat(const bsqon::PrimitiveType *t, const ValueSetGeneratorEnvironment& env, ValueComponent* vc)
{
    std::vector<uint64_t> values = runAIGenCall<uint64_t>(this, env, t);
    std::transform(values.begin(), values.end(), std::back_inserter(vc->options), [t](uint64_t v) { return new bsqon::BigNatNumberValue(t, g_spos, v); });
}

void AIValueGenerator::generateBigInt(const bsqon::PrimitiveType *t, const ValueSetGeneratorEnvironment& env, ValueComponent* vc)
{
    std::vector<int64_t> values = runAIGenCall<int64_t>(this, env, t);
    std::transform(values.begin(), values.end(), std::back_inserter(vc->options), [t](uint64_t v) { return new bsqon::BigIntNumberValue(t, g_spos, v); });
}

void AIValueGenerator::generateFloat(const bsqon::PrimitiveType* t, const ValueSetGeneratorEnvironment& env, ValueComponent* vc)
{
    std::vector<double> values = runAIGenCall<double>(this, env, t);
    std::transform(values.begin(), values.end(), std::back_inserter(vc->options), [t](double v) { return new bsqon::FloatNumberValue(t, g_spos, v); });
}

void AIValueGenerator::generateCString(const bsqon::PrimitiveType* t, const ValueSetGeneratorEnvironment& env, ValueComponent* vc)
{
    std::vector<std::string> values = runAIGenCall<std::string>(this, env, t);
    std::transform(values.begin(), values.end(), std::back_inserter(vc->options), [t](const std::string& v) { 
        return bsqon::CStringValue::createFromGenerator(t, g_spos, v);
    });
}

void AIValueGenerator::generateString(const bsqon::PrimitiveType* t, const ValueSetGeneratorEnvironment& env, ValueComponent* vc)
{
    std::vector<std::string> values = runAIGenCall<std::string>(this, env, t);
    std::transform(values.begin(), values.end(), std::back_inserter(vc->options), [t](const std::string& v) { 
        return bsqon::StringValue::createFromGenerator(t, g_spos, v);
    });
}

void AIValueGenerator::generatePrimitive(const bsqon::PrimitiveType* t, const ValueSetGeneratorEnvironment& env, ValueComponent* vc)
{
    auto tk = t->tkey;
    if(tk == "None") {
        this->generateNone(t, env, vc);
    }
    else if(tk == "Bool") {
        this->generateBool(t, env, vc);
    }
    else if(tk == "Nat") {
        this->generateNat(t, env, vc);
    }
    else if(tk == "Int") {
        this->generateInt(t, env, vc);
    }
    else if(tk == "BigInt") {
        return this->generateBigInt(t, env, vc);
    }
    else if(tk == "BigNat") {
        return this->generateBigNat(t, env, vc);
    }
    /*
        else if(tk == "Rational") {
            return this->parseRational(t, node);
        }
    */
    else if(tk == "Float") {
        return this->generateFloat(t, env, vc);
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
        return this->generateString(t, env, vc);
    }
    else if(tk == "CString") {
        return this->generateCString(t, env, vc);
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

void AIValueGenerator::generateEnum(const bsqon::EnumType* t, ValueComponent* vc)
{
    for(size_t i = 0; i < t->variants.size(); ++i) {
        vc->options.push_back(new bsqon::EnumValue(t, g_spos, t->variants[i], i));
    }
}

void AIValueGenerator::generateType(const bsqon::Type* t, const ValueSetGeneratorEnvironment& env, ValueComponent* vc)
{
    switch (t->tag) {
        case bsqon::TypeTag::TYPE_PRIMITIVE: {
            this->generatePrimitive(static_cast<const bsqon::PrimitiveType *>(t), env, vc);
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
            // Missing type
            assert(false);
            break;
        }
    }
}
