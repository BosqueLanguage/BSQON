#include "generator_ai.h"
#include <curl/curl.h>
#include "json.hpp"
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <cstdlib>
#include <cctype>
#include <type_traits>
using json = nlohmann::json;

static bsqon::SourcePos g_spos = {0, 0, 0, 0};
std::string mock_data;
bool mock_loaded = false;
bool isDevMode = false;
bool final_prompt_logged = false;
static int g_llm_prompt_log_call_count = 0;
static bool g_llm_prompt_log_file_started = false;
static bool g_llm_prompt_log_file_announced = false;
static bool g_failure_aware_mock_flow_enabled = false;
static bool g_failure_aware_input_generation_enabled = false;
static json g_failure_prompt_context = json::object();

static bool isEnvFlagEnabled(const char* name)
{
    const char* rawValue = std::getenv(name);
    if (rawValue == nullptr) {
        return false;
    }

    std::string value(rawValue);
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c) {
        return static_cast<char>(std::tolower(c));
    });

    return !value.empty() && value != "0" && value != "false" && value != "off" && value != "no";
}

static std::string getEnvValueOrEmpty(const char* name)
{
    const char* rawValue = std::getenv(name);
    return rawValue == nullptr ? "" : std::string(rawValue);
}

static std::string getLLMPromptLogFilePath()
{
    std::string configuredPath = getEnvValueOrEmpty("LLM_PROMPT_LOG_FILE");
    if (!configuredPath.empty()) {
        return configuredPath;
    }

    return "build/tgen-llm-prompts.log";
}

static void writeLLMPromptLogFile(const std::string& logFilePath, const std::string& header, const std::string& prompt)
{
    try {
        std::filesystem::path path(logFilePath);
        auto parent = path.parent_path();
        if (!parent.empty()) {
            std::filesystem::create_directories(parent);
        }

        std::ofstream file(
            logFilePath,
            std::ios::out | (g_llm_prompt_log_file_started ? std::ios::app : std::ios::trunc)
        );
        if (!file.is_open()) {
            std::cerr << "Failed to open LLM prompt log file: " << logFilePath << std::endl;
            return;
        }

        file << header << std::endl << prompt << std::endl << std::endl;
        g_llm_prompt_log_file_started = true;
    }
    catch (const std::exception& ex) {
        std::cerr << "Failed to write LLM prompt log file: " << ex.what() << std::endl;
    }
}

static void logLLMPrompt(const std::string& provider, const std::string& prompt)
{
    const bool logToStdout = isEnvFlagEnabled("LLM_LOG_PROMPTS");
    const std::string logFilePath = getLLMPromptLogFilePath();

    ++g_llm_prompt_log_call_count;
    const std::string callHeader = "---- Final Prompt (call " + std::to_string(g_llm_prompt_log_call_count) + ", " + provider + ") ----";

    if (logToStdout) {
        std::cout << callHeader << std::endl << prompt << std::endl;
    }
    else if (!final_prompt_logged) {
        std::cout << "---- Final Prompt (one-time) ----" << std::endl << prompt << std::endl;
        final_prompt_logged = true;
    }

    if (!g_llm_prompt_log_file_announced) {
        std::cout << "---- LLM prompt log file: " << logFilePath << " ----" << std::endl;
        g_llm_prompt_log_file_announced = true;
    }
    writeLLMPromptLogFile(logFilePath, callHeader, prompt);
}

void setFailureAwareMockFlowEnabled(bool enabled)
{
    g_failure_aware_mock_flow_enabled = enabled;
}

void setFailureAwareInputGenerationEnabled(bool enabled)
{
    g_failure_aware_input_generation_enabled = enabled;
}

void clearFailurePromptContext()
{
    g_failure_prompt_context = json::object();
}

void setFailurePromptContext(const json& context)
{
    if (context.is_object()) {
        g_failure_prompt_context = context;
    }
    else {
        g_failure_prompt_context = json::object();
    }
}

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
    {{reexpression}}
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

static std::string g_mockgen_db_query_prompt = R"(
    Now also act as a database query expert. You must generate values(must maintain the data type and format) that are valid for the database query with the input values.
    - Raw DB query for this endpoint (with runtime input values): {{sql_hint}}
    - If the query contains a concrete literal value for the current field/argument, include that exact value in the generated JSON array, preferably first.
    - If the query has no concrete literal value, or the literal does not correspond to the current field/argument, do not force any query-derived value.
)";

static std::string g_failure_path_prompt = R"(
    Act as an expert in database failure simulation for repository-level mock replay.

    You must generate ONLY DB failure mock entries for this repository call context.
    Return ONLY valid JSON array (no markdown, no explanation, no extra text).

    Context:
    - repository: {{repository}}
    - method: {{method}}
    - args_json: {{args_json}}
    - arg_types_json: {{arg_types_json}}
    - signature: {{signature}}
    - path: {{path}}
    - value_type: {{value_type}}
    - sql_hint: {{sql_hint}}
    - allowed_error_types_json: {{allowed_error_types_json}}
    - context_summary:
    {{context_summary}}

    Generate exactly {{count}} DB failure mock entries as a JSON array.

    Output schema (array of objects):
    [
      {
        "mockflow": {
          "version": "1.0",
          "mode": "error"
        },
        "match": {
          "repository": "{{repository}}",
          "method": "{{method}}"
        },
        "error": {
          "type": "DB_TIMEOUT",
          "message": "Simulated DB timeout during {{repository}}.{{method}}"
        }
      }
    ]

    Strict rules:
    - match.repository must equal {{repository}} exactly
    - match.method must equal {{method}} exactly
    - Never invent repository/method names outside provided context
    - error.type must be one value from allowed_error_types_json
    - Do NOT output exceptionClass or errorType
    - Do NOT output success payloads
    - Keep message concise and realistic
)";

static std::string g_generic_failure_path_prompt = R"(
    Act as an expert in generic failure-aware mock replay.

    You must generate failure mock entries for this repository/dependency call context.
    Include database/storage failures as one category, but do not limit the scenarios to database failures.
    Return ONLY valid JSON array (no markdown, no explanation, no extra text).

    Context:
    - failure_role: {{failure_role}}
    - repository: {{repository}}
    - method: {{method}}
    - args_json: {{args_json}}
    - arg_types_json: {{arg_types_json}}
    - signature: {{signature}}
    - path: {{path}}
    - value_type: {{value_type}}
    - sql_hint: {{sql_hint}}
    - allowed_error_types_json: {{allowed_error_types_json}}
    - context_summary:
    {{context_summary}}

    Generate exactly {{count}} failure mock entries as a JSON array.

    Diversify scenarios across realistic failure styles when possible:
    - database/storage failures: timeout, unavailable storage, permission denied, invalid query, deadlock, constraint violation
    - missing-data failures: absent record, empty result, stale reference, unresolved identifier
    - external dependency failures: downstream timeout, downstream unavailable, downstream 404, downstream 500, rate limit
    - data-shape failures: malformed dependency response, schema mismatch, missing required field, unsupported enum/value
    - transformation failures: parse failure, serialization failure, incompatible nested structure
    - consistency failures: duplicate/conflict, stale data, transaction conflict, partial data

    Output schema (array of objects):
    [
      {
        "mockflow": {
          "version": "1.0",
          "mode": "error"
        },
        "match": {
          "repository": "{{repository}}",
          "method": "{{method}}"
        },
        "error": {
          "role": "{{failure_role}}",
          "style": "timeout",
          "type": "DB_TIMEOUT",
          "message": "Simulated timeout during {{repository}}.{{method}}"
        }
      }
    ]

    Strict rules:
    - match.repository must equal {{repository}} exactly
    - match.method must equal {{method}} exactly
    - Never invent repository/method names outside provided context
    - error.role must equal {{failure_role}} exactly
    - error.style must describe the generic failure behavior, such as timeout, unavailable, not_found, empty_result, http_404, http_500, malformed_response, schema_mismatch, permission_denied, conflict, or parse_failure
    - error.type must be one value from allowed_error_types_json
    - If allowed_error_types_json is narrow or database-oriented, choose the closest allowed type and express the specific failure style in error.message
    - Do NOT output exceptionClass or errorType
    - Do NOT output success payloads
    - Do NOT add extra top-level keys beyond mockflow, match, and error
    - Keep message concise, realistic, and specific to the selected failure style
)";

static std::string resolveDbQueryContext()
{
    const char* sqlHint = std::getenv("MOCKGEN_HINT_QUERY");

    if (sqlHint == nullptr || std::string(sqlHint).empty()) {
        return "";
    }

    std::string section = g_mockgen_db_query_prompt;
    const std::string placeholder = "{{sql_hint}}";
    size_t pos = section.find(placeholder);
    if (pos != std::string::npos) {
        section.replace(pos, placeholder.size(), std::string(sqlHint));
    }
    return section;
}

static std::string contextStringOr(const std::string& key, const std::string& fallback)
{
    if (!g_failure_prompt_context.is_object()) {
        return fallback;
    }

    auto it = g_failure_prompt_context.find(key);
    if (it == g_failure_prompt_context.end() || !it->is_string()) {
        return fallback;
    }

    auto v = it->get<std::string>();
    return v.empty() ? fallback : v;
}

static std::string contextArrayDumpOr(const std::string& key, const std::string& fallback)
{
    if (!g_failure_prompt_context.is_object()) {
        return fallback;
    }

    auto it = g_failure_prompt_context.find(key);
    if (it == g_failure_prompt_context.end() || !it->is_array()) {
        return fallback;
    }

    try {
        return it->dump();
    } catch (...) {
        return fallback;
    }
}

static int contextIntOr(const std::string& key, int fallback)
{
    if (!g_failure_prompt_context.is_object()) {
        return fallback;
    }

    auto it = g_failure_prompt_context.find(key);
    if (it == g_failure_prompt_context.end() || !it->is_number_integer()) {
        return fallback;
    }

    try {
        return it->get<int>();
    } catch (...) {
        return fallback;
    }
}

static std::string g_prompt_naked_arg = R"(
    Act as an expert in API input space analysis for black-box test generation.

    Given a variable named "{{name}}" with data type "{{type}}" with format "{{format}}", generate a JSON array containing ONLY test values strictly matching the specified data type and format, and within acceptable ranges.
    {{one_shot}}
    {{mock_generate}}
    {{std_reqs}}
    )";

static std::string g_prompt_naked_arg_index = R"(
    Act as an expert in REST API input space analysis.
    
    Given a variable named "{{name}}" that is an array of type "{{type}}" elements with format "{{format}}", generate a JSON array containing ONLY test values strictly matching the specified array element type and format, and within acceptable ranges.
    {{one_shot}}
    {{mock_generate}}
    {{std_reqs}}
    )";

static std::string g_prompt_member_field = R"(
    Act as an expert in REST API input space analysis.
    
    Given a field, named "{{fname}}" in a type named "{{tname}}", with data type "{{type}}" and format "{{format}}", generate a JSON array containing ONLY test values strictly matching the specified data type and format, and within acceptable ranges.
    {{one_shot}}
    {{mock_generate}}
    {{std_reqs}}
    )";

static std::string g_prompt_member_field_index = R"(
    Act as an expert in REST API input space analysis.
        
    Given a field, named "{{fname}}" in a type named "{{tname}}", that is an array of type "{{type}}" elements with format "{{format}}", generate a JSON array containing ONLY test values strictly matching the specified array element type and format, and within acceptable ranges.
    {{one_shot}}
    {{mock_generate}}
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

static std::string mock_generate = R"(
    Act as an expert in data analysis and synthetic data generation.
    
    You will be given a field named "{{fname}}" in a type named "{{tname}}", with data type "{{type}}" and format "{{format}}", found in a larger input text or file content. Your job is to:
    1. Extract the value of that field from the provided data.
    2. Analyze the format, pattern, and characteristics of that value.
    3. Generate some values by repeating a few of the extracted ones, along with additional values that are syntactically and structurally similar to them.
    4. test values strictly matching the specified data type and format
    
    Be careful to preserve:
    - Character type (letters, digits, symbols)
    - Pattern (e.g., alternating characters, fixed prefix/suffix, etc.)
    - Length
    - Casing (uppercase/lowercase)
    
    --- START DATA SAMPLE ---
    {{mock_data}}
    --- END DATA SAMPLE ---
    
    The field name to extract is: "{{fname}}"
    
    Now generate similar values in this format:
    ["value1", "value2", "value3"]
    )";



static std::string failure_aware_input_partition = R"(
    --- FAILURE-AWARE INPUT PARTITIONING ---

    You must ensure generated values cover scenarios that can trigger different categories of failures during application–database interaction:

    1. Input/Validation Errors:
    - Boundary-valid values (e.g., minimum, maximum, near-empty but valid)
    - Values that are syntactically valid but may challenge validation logic

    2. Application/Query Errors:
    - Values that are valid but may expose parameter binding or query assumptions
    - Edge-length, uncommon but valid formats

    3. Business Logic Errors:
    - Valid values that may or may not exist in the dataset (e.g., existing vs non-existing IDs)
    - Values that exercise different logical states (found vs not found)

    4. Database Interaction Errors (Input-Triggerable):
    - Values that may interact with constraints or lookup behavior:
    - Existing values (possible duplicates)
    - Valid but non-existing values
    - Boundary numeric values within type limits
    - Values that differentiate indexed vs non-indexed lookup behavior

    5. System-Level Fault Awareness:
    - Select values that remain useful when executed under fault-injected conditions such as:
    - timeout, temporary unavailability, retry, rollback, replica lag, connection exhaustion
    - Do NOT encode faults as invalid values
    - Focus on values that help distinguish environment-level failures from input, query, database, and business logic failures

    IMPORTANT:
    - All values MUST strictly conform to the required data type and format
    - Do NOT generate syntactically invalid values
    - Trigger failures through semantic and domain variation, not malformed input
    )";

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

json callAPIWithPrompt(std::string prompt);

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

void replaceAll(std::string& str, const std::string& from, const std::string& to) {
    size_t pos = 0;
    while ((pos = str.find(from, pos)) != std::string::npos) {
        str.replace(pos, from.length(), to);
        pos += to.length();
    }
}

std::string buildPromptForFailurePath(const bsqon::PrimitiveType* t, const ValueSetGeneratorEnvironment& env, const std::string& signature)
{
    std::string prompt = g_generic_failure_path_prompt;

    const std::string repository = contextStringOr("repository", "UnknownRepository");
    const std::string method = contextStringOr(
        "method_name",
        contextStringOr("method", "unknownMethod")
    );
    const std::string argsJson = contextArrayDumpOr("args", "[]");
    const std::string argTypesJson = contextArrayDumpOr("arg_types", "[]");
    const std::string allowedErrorTypesJson = contextArrayDumpOr(
        "allowed_error_types",
        contextArrayDumpOr("allowed_exception_classes", "[]")
    );
    const std::string contextSummary = contextStringOr(
        "context_summary",
        contextStringOr("schema_summary", "<none>")
    );
    const std::string failureRole = contextStringOr("failure_role", "storage");
    const int countValue = std::max(1, contextIntOr("count", 50));
    const std::string count = std::to_string(countValue);
    const std::string sqlHint = contextStringOr("sql_hint", "");

    replaceAll(prompt, "{{failure_role}}", failureRole);
    replaceAll(prompt, "{{repository}}", repository);
    replaceAll(prompt, "{{method}}", method);
    replaceAll(prompt, "{{args_json}}", argsJson);
    replaceAll(prompt, "{{arg_types_json}}", argTypesJson);
    replaceAll(prompt, "{{signature}}", signature);
    replaceAll(prompt, "{{path}}", env.path);
    replaceAll(prompt, "{{value_type}}", t == nullptr ? "UnknownType" : t->tkey);
    replaceAll(prompt, "{{sql_hint}}", sqlHint.empty() ? "<none>" : sqlHint);
    replaceAll(prompt, "{{allowed_error_types_json}}", allowedErrorTypesJson);
    replaceAll(prompt, "{{context_summary}}", contextSummary);
    replaceAll(prompt, "{{count}}", count);

    return prompt;
}

static bool isValidFailureMockEntry(const json& item)
{
    if (!item.is_object()) {
        return false;
    }
    if (!item.contains("mockflow") || !item["mockflow"].is_object()) {
        return false;
    }
    if (!item["mockflow"].contains("mode") || !item["mockflow"]["mode"].is_string()) {
        return false;
    }
    if (item["mockflow"]["mode"].get<std::string>() != "error") {
        return false;
    }
    if (!item.contains("match") || !item["match"].is_object()) {
        return false;
    }
    if (!item.contains("error") || !item["error"].is_object()) {
        return false;
    }
    if (!item["error"].contains("type") || !item["error"]["type"].is_string()) {
        return false;
    }
    if (!item["error"].contains("message") || !item["error"]["message"].is_string()) {
        return false;
    }
    return true;
}

json generateFailureMockEntries(const std::string& signature)
{
    ValueSetGeneratorEnvironment env{"failure.entry", {}, GenerateContext{}};
    auto prompt = buildPromptForFailurePath(nullptr, env, signature);

    json raw = callAPIWithPrompt(prompt);
    if (!raw.is_array()) {
        throw std::runtime_error("Failure mock generation must return a JSON array");
    }

    json filtered = json::array();
    for (const auto& item : raw) {
        if (isValidFailureMockEntry(item)) {
            filtered.push_back(item);
        }
    }

    if (filtered.empty()) {
        throw std::runtime_error("No valid failure mock entries were returned by LLM");
    }

    return filtered;
}

void loadMockSection(std::string& prompt, const ValueSetGeneratorEnvironment& env) {
    namespace fs = std::filesystem;
    if (!mock_loaded) {
        const std::string filename = "mock.text";
        std::ifstream file(filename);

        if (fs::exists(filename) && fs::file_size(filename) > 0) {
            if (file.is_open()) {
                std::getline(file, mock_data, '\0');
                std::cout << "✅ mock.text loaded successfully.\n";
                mock_loaded = true;
            } else {
                std::cerr << "❌ Failed to open mock.text.\n";
                return;
            }
        } else {
            std::cerr << "⚠️ mock.text does not exist or is empty.\n";
            return;
        }
    }

    std::string resolved_mock_generate = mock_generate;

    size_t md_pos = resolved_mock_generate.find("{{mock_data}}");
    if (md_pos != std::string::npos) {
        resolved_mock_generate.replace(md_pos, 13, mock_data);
    } else {
        std::cerr << "⚠️ {{mock_data}} not found in mock_generate.\n";
    }

    if (env.context.forfield.has_value()) {
        replaceAll(resolved_mock_generate, "{{fname}}", env.context.forfield.value().fname);
    } else {
        replaceAll(resolved_mock_generate, "{{fname}}", env.path);
    }

    size_t mg_pos = prompt.find("{{mock_generate}}");
    if (mg_pos != std::string::npos) {
        prompt.replace(mg_pos, 17, resolved_mock_generate);
    } else {
        std::cerr << "⚠️ {{mock_generate}} not found in prompt.\n";
    }
}

std::string buildPromptFor(const bsqon::PrimitiveType* t, const ValueSetGeneratorEnvironment& env, std::string formatinstructions, std::string signature) 
{
    if (g_failure_aware_mock_flow_enabled) {
        return buildPromptForFailurePath(t, env, signature);
    }

    std::string prompt;
    std::string resolvedFname = env.context.forfield.has_value() ? env.context.forfield.value().fname : env.path;
    std::string resolvedTname = env.context.intype.has_value() ? env.context.intype.value()->tkey : "UnknownType";
    std::string resolvedType = t == nullptr ? "UnknownType" : t->tkey;
    std::string resolvedReexpression = "";

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

    loadMockSection(prompt, env);  // Inject mock_data into mock_generate and into prompt

    prompt.replace(prompt.find("{{std_reqs}}"), 12, g_std_prompt_instructions);
    std::string dbQueryContext = resolveDbQueryContext();
    if (!dbQueryContext.empty()) {
        prompt += "\n";
        prompt += dbQueryContext;
    }
    
    prompt.replace(prompt.find("{{path}}"), 8, env.path);
    prompt.replace(prompt.find("{{signature}}"), 13, signature);
    prompt.replace(prompt.find("{{path}}"), 8, env.path);
    prompt.replace(prompt.find("{{signature}}"), 13, signature);

    prompt.replace(prompt.find("{{format}}"), 10, formatinstructions);

    if (prompt.find("{{path}}") != std::string::npos) prompt.replace(prompt.find("{{path}}"), 8, env.path);
    if (prompt.find("{{signature}}") != std::string::npos) prompt.replace(prompt.find("{{signature}}"), 13, signature);
    if (prompt.find("{{path}}") != std::string::npos) prompt.replace(prompt.find("{{path}}"), 8, env.path);
    if (prompt.find("{{signature}}") != std::string::npos) prompt.replace(prompt.find("{{signature}}"), 13, signature);

    if (prompt.find("{{format}}") != std::string::npos) prompt.replace(prompt.find("{{format}}"), 10, formatinstructions);

    // Add dev mode instruction to generate fewer values
    if (isDevMode) {
        prompt += "\nIMPORTANT: Since this is in development mode, please generate exactly 2 test values.";
        //prompt += "\nCRITICAL: You must respond with ONLY a valid JSON array - no other text, no explanations.";
        //prompt += "\n📂 Current Working Directory: " + std::filesystem::current_path().string();
    }
    if(!env.context.oftype.has_value()) {
        prompt.replace(prompt.find("{{type}}"), 8, t->tkey);
    }
    else {
        auto ttype = env.context.oftype.value();

        std::string ttinfo = ttype->tkey;
        std::string reexpression = "";
        if(ttype->optOfValidator.has_value()) {
            auto vinfo = ttype->optOfValidator.value().first;
            ttinfo += " that is accepted by the regular expression " + std::string(vinfo.cbegin(), vinfo.cend());

            reexpression = "- Test values must match the following regular expression: " + std::string(vinfo.cbegin(), vinfo.cend());
        }

        resolvedType = ttinfo;
        resolvedReexpression = reexpression;

        prompt.replace(prompt.find("{{type}}"), 8, ttinfo);
        prompt.replace(prompt.find("{{reexpression}}"), 16, reexpression);
    }

    if (g_failure_aware_input_generation_enabled) {
        prompt += "\n";
        prompt += failure_aware_input_partition;
    }

    // Final template pass: resolve tokens that may appear multiple times (including injected mock_generate section).
    replaceAll(prompt, "{{fname}}", resolvedFname);
    replaceAll(prompt, "{{tname}}", resolvedTname);
    replaceAll(prompt, "{{type}}", resolvedType);
    replaceAll(prompt, "{{format}}", formatinstructions);
    replaceAll(prompt, "{{path}}", env.path);
    replaceAll(prompt, "{{signature}}", signature);
    replaceAll(prompt, "{{reexpression}}", resolvedReexpression);

    return prompt;
}

json callGeminiAPIWithPrompt(std::string prompt, std::string jsontype)
{
    logLLMPrompt("Gemini", prompt);

    std::string apiKey = std::string(getenv("GEMINI_API_KEY"));
    std::string url = "https://generativelanguage.googleapis.com/v1beta/models/gemini-2.0-flash:generateContent";

    json requestJson = json::parse(g_api_json);
    requestJson["contents"][0]["parts"][0]["text"] = prompt;
    requestJson["generationConfig"]["response_schema"]["items"] = json::parse(jsontype);

    //TODO: Temp debugging output
    //std::cout << "---- Requesting with ----" << std::endl << requestJson.dump(8) << std::endl;

    std::string response = makeAPIRequest("Gemini", apiKey, url, requestJson);
    json responseJson = json::parse(response);

    //TODO: Temp debugging output
    //std::cout << "---- Responded with ----" << std::endl << responseJson.dump(8) << std::endl;

    return json::parse(responseJson["candidates"][0]["content"]["parts"][0]["text"].get<std::string>());
}

json callAPIWithPrompt(std::string prompt)
{
    static const std::string OPENAI_URL   = "https://api.openai.com/v1/chat/completions";
    static const std::string OPENAI_MODEL = "gpt-5";

    static const std::string DEEPSEEK_URL   = "https://api.deepseek.com/chat/completions";
    static const std::string DEEPSEEK_MODEL = "deepseek-chat";

    static const std::string LMSTUDIO_API_KEY = "lm-studio";
    static const std::string LMSTUDIO_URL     = "http://127.0.0.1:1234/v1/chat/completions";
    static const std::string LMSTUDIO_MODEL   = "openai-community_-_gpt2-medium";

    std::string provider = "deepseek"; // "openai" | "deepseek" | "lmstudio"

    std::string apiKey, url, model;

    if (provider == "deepseek") {
        apiKey = std::getenv("DEEPSEEK_API_KEY") ? std::getenv("DEEPSEEK_API_KEY") : "";
        url    = DEEPSEEK_URL;
        model  = DEEPSEEK_MODEL;
    } else if (provider == "lmstudio") {
        apiKey = LMSTUDIO_API_KEY; 
        url    = LMSTUDIO_URL;
        model  = LMSTUDIO_MODEL;
    } else { // "openai"
        apiKey = std::getenv("OPENAI_CHATGPT_API_KEY") ? std::getenv("OPENAI_CHATGPT_API_KEY") : "";
        url    = OPENAI_URL;
        model  = OPENAI_MODEL;
    }

    logLLMPrompt(provider, prompt);
   
    json requestJson = {
        {"model", model},
        {"messages", {
            {{"role", "user"}, {"content", prompt}}
        }}
    };

    //std::string response = makeAPIRequest("DEEPSEEK", apiKey, url, requestJson);
    std::string response = makeAPIRequest(provider, apiKey, url, requestJson);
    json responseJson = json::parse(response);
    //std::cout << "---- Raw Response ----" << std::endl << responseJson.dump(8) << std::endl;

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
static T parseNumericFromString(const std::string& rawValue) {
    auto begin = std::find_if_not(rawValue.begin(), rawValue.end(), [](unsigned char c) { return std::isspace(c) != 0; });
    auto end = std::find_if_not(rawValue.rbegin(), rawValue.rend(), [](unsigned char c) { return std::isspace(c) != 0; }).base();
    if (begin >= end) {
        throw std::runtime_error("Numeric string is empty");
    }

    std::string value(begin, end);
    size_t idx = 0;

    if constexpr (std::is_integral_v<T> && std::is_signed_v<T>) {
        long long parsed = std::stoll(value, &idx, 10);
        if (idx != value.size()) {
            throw std::runtime_error("Invalid signed integer token: " + value);
        }
        return static_cast<T>(parsed);
    } else if constexpr (std::is_integral_v<T> && !std::is_signed_v<T>) {
        if (!value.empty() && value[0] == '-') {
            throw std::runtime_error("Invalid unsigned integer token: " + value);
        }
        unsigned long long parsed = std::stoull(value, &idx, 10);
        if (idx != value.size()) {
            throw std::runtime_error("Invalid unsigned integer token: " + value);
        }
        return static_cast<T>(parsed);
    } else if constexpr (std::is_floating_point_v<T>) {
        long double parsed = std::stold(value, &idx);
        if (idx != value.size()) {
            throw std::runtime_error("Invalid floating-point token: " + value);
        }
        return static_cast<T>(parsed);
    } else {
        static_assert(!sizeof(T*), "parseNumericFromString only supports numeric types");
    }
}

template <typename T>
std::vector<T> getValuesFromJson(const json& extractedData) {
    std::vector<T> values;
    for (const auto& jv : extractedData) {
        if (jv.is_null())
            continue;  // skip nulls to avoid type_error.302 (e.g. AI returning null in string/date array)
        if constexpr (std::is_integral_v<T> || std::is_floating_point_v<T>) {
            if (jv.is_number()) {
                values.push_back(jv.get<T>());
            }
            else if (jv.is_string()) {
                values.push_back(parseNumericFromString<T>(jv.get<std::string>()));
            }
            else {
                throw std::runtime_error("Expected numeric JSON value or numeric string");
            }
        }
        else {
            values.push_back(jv.get<T>());
        }
    }
    return values;
}

template <typename T>
std::vector<T> runAIGenCall(const AIValueGenerator* aigen, const ValueSetGeneratorEnvironment& env, const bsqon::PrimitiveType* t) {
    auto prompt = buildPromptFor(t, env, g_typeFormatInstructions[t->tkey], aigen->sigstr);
    auto jfmt = g_typeJSONFormat[t->tkey];

    json res;
    if(aigen->model == AIModelOption::GEMINI) {
        //std::cout << "---- gemini with ----------------------\n";
        res = callGeminiAPIWithPrompt(prompt, jfmt);
    }
    else {
        std::cout << "---- deepseek with ----------------------\n";
        //std::cout << prompt << "\n";
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
