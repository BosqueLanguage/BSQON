#include <napi.h>

#include "../include/brex/regex/brex.h"
#include "../include/brex/regex/brex_parser.h"
#include "../include/brex/regex/brex_compiler.h"
#include "../include/brex/regex/brex_system.h"

using namespace Napi;

//maps from literal regex representations to their executable forms
static std::map<std::u8string, brex::UnicodeRegexExecutor*> g_executableUnicodeRegexMap;
static std::map<std::u8string, brex::CRegexExecutor*> g_executableCRegexMap;

//maps from namespace const named Foo::bar regex representations to their executable forms AND their named regex opt forms
static std::map<std::string, const brex::RegexOpt*> g_namedUnicodeRegexMap;
static std::map<std::string, brex::UnicodeRegexExecutor*> g_namedExecutableUnicodeRegexMap;

static std::map<std::string, const brex::RegexOpt*> g_namedCRegexMap;
static std::map<std::string, brex::CRegexExecutor*> g_namedExecutableCRegexMap;

//The active lexing string for lexing operations in the compiler
static std::u8string g_lexstring;

static brex::ReNSRemapper nsremapper;

///////////////////////////////////////////////////////////////////////////
//Initialization and processing helper functions

std::string processUnicodeRegex(std::string& inns, std::u8string regex) 
{
    auto pr = brex::RegexParser::parseUnicodeRegex(regex, false);
    if(!pr.first.has_value() || !pr.second.empty()) {
        return !pr.second.empty() ? std::string(pr.second[0].msg.cbegin(), pr.second[0].msg.cend()) : "Invalid Regex";
    }

    std::vector<brex::RegexCompileError> compileerror;
    auto rmp = brex::ReSystemResolverInfo(inns, &nsremapper);
    auto executor = brex::RegexCompiler::compileUnicodeRegexToExecutor(pr.first.value(), g_namedUnicodeRegexMap, {}, false, &rmp, &brex::ReSystem::resolveREName, compileerror);
    if(!compileerror.empty()) {
        return std::string(compileerror[0].msg.cbegin(), compileerror[0].msg.cend());
    }

    g_executableUnicodeRegexMap[regex] = executor;
    return "";
}

std::string processCRegex(std::string& inns, std::u8string regex) 
{
    auto pr = brex::RegexParser::parseCRegex(regex, false);
    if(!pr.first.has_value() || !pr.second.empty()) {
        return !pr.second.empty() ? std::string(pr.second[0].msg.cbegin(), pr.second[0].msg.cend()) : "Invalid Regex";
    }

    std::vector<brex::RegexCompileError> compileerror;
    auto rmp = brex::ReSystemResolverInfo(inns, &nsremapper);
    auto executor = brex::RegexCompiler::compileCRegexToExecutor(pr.first.value(), g_namedCRegexMap, {}, false, &rmp, &brex::ReSystem::resolveREName, compileerror);
    if(!compileerror.empty()) {
        return std::string(compileerror[0].msg.cbegin(), compileerror[0].msg.cend());
    }

    g_executableCRegexMap[regex] = executor;
    return "";
}

bool isUnicodeRegex(std::u8string regex)
{
    return regex.ends_with(u8"/");
}

std::string processRegexAsNeeded(std::string& inns, std::u8string regex)
{
    if(regex.size() < 2) {
        return "Invalid regex";
    }

    if(isUnicodeRegex(regex)) {
        if(g_executableUnicodeRegexMap.find(regex) == g_executableUnicodeRegexMap.end()) {
            return processUnicodeRegex(inns, regex);
        }
    }
    else {
        if(g_executableCRegexMap.find(regex) == g_executableCRegexMap.end()) {
            return processCRegex(inns, regex);
        }
    }

    return "";
}

Napi::Value LoadConstAndValidateRESystem(const Napi::CallbackInfo& info) 
{
    Napi::Env env = info.Env();

    if (info.Length() != 1 || !info[0].IsArray()) {
        Napi::TypeError::New(env, "Expected a regex system (array of nsinfos)").ThrowAsJavaScriptException();
        return env.Null();
    }

    Napi::Array systemarray = info[0].As<Napi::Array>();
    std::vector<brex::RENSInfo> ninfos;
    for(size_t i = 0; i < systemarray.Length(); ++i) {
        if(!systemarray.Get(i).IsObject()) {
            Napi::TypeError::New(env, "Expected a regex system namepsace entry").ThrowAsJavaScriptException();
            return env.Null();
        }

        Napi::Object ninfoentry = systemarray.Get(i).As<Napi::Object>();
        if(!ninfoentry.Has("nsinfo") || !ninfoentry.Get("nsinfo").IsObject() || !ninfoentry.Has("reinfos") || !ninfoentry.Get("reinfos").IsArray()) {
            Napi::TypeError::New(env, "Expected fields nsinfo (object) and reinfos (array)").ThrowAsJavaScriptException();
            return env.Null();
        }

        Napi::Object remapinfo = ninfoentry.Get("nsinfo").As<Napi::Object>();
        if(!remapinfo.Has("inns") || !remapinfo.Get("inns").IsString() || !remapinfo.Has("nsmappings") || !remapinfo.Get("nsmappings").IsArray()) {
            Napi::TypeError::New(env, "Expected fields inns (string) and nsmappings (array)").ThrowAsJavaScriptException();
            return env.Null();
        }

        std::vector<std::pair<std::string, std::string>> nsmappings;
        Napi::Array mappingarr = remapinfo.Get("nsmappings").As<Napi::Array>();
        size_t mappinglen = mappingarr.Length();
        for(size_t j = 0; j < mappinglen; ++j) {
            Napi::Value nsmappingentry = mappingarr.Get(j);
            if(!nsmappingentry.IsArray()){
                Napi::TypeError::New(env, "Expected namespace remapping pair").ThrowAsJavaScriptException();
                return env.Null();
            }

            Napi::Array nsmappingarr = nsmappingentry.As<Napi::Array>();
            if(nsmappingarr.Length() != 2 || !nsmappingarr.Get((uint32_t)0).IsString() || !nsmappingarr.Get((uint32_t)1).IsString()) {
                Napi::TypeError::New(env, "Expected namespace remapping pair").ThrowAsJavaScriptException();
                return env.Null();
            }

            nsmappings.push_back({nsmappingarr.Get((uint32_t)0).As<Napi::String>().Utf8Value(), nsmappingarr.Get((uint32_t)1).As<Napi::String>().Utf8Value()});
        }

        Napi::Array reinfos = ninfoentry.Get("reinfos").As<Napi::Array>();
        size_t reinfoslen = reinfos.Length();
        std::vector<brex::REInfo> reinfovec;
        for(size_t j = 0; j < reinfoslen; ++j) {
            if(!reinfos.Get(j).IsObject()) {
                Napi::TypeError::New(env, "Expected a regex entry").ThrowAsJavaScriptException();
                return env.Null();
            }

            Napi::Object reinfoentry = reinfos.Get(j).As<Napi::Object>();
            if(!reinfoentry.Has("name") || !reinfoentry.Get("name").IsString() || !reinfoentry.Has("restr") || !reinfoentry.Get("restr").IsString()) {
                Napi::TypeError::New(env, "Expected fields name (string) and regex (string)").ThrowAsJavaScriptException();
                return env.Null();
            }

            std::string rename = reinfoentry.Get("name").As<Napi::String>().Utf8Value();
            std::string restring = reinfoentry.Get("restr").As<Napi::String>().Utf8Value();

            std::u8string u8restring = std::u8string(restring.cbegin(), restring.cend());

            reinfovec.push_back({rename, u8restring});
        }

        ninfos.push_back({{remapinfo.Get("inns").As<Napi::String>().Utf8Value(), nsmappings}, reinfovec});
    }

    std::vector<std::u8string> errors;
    brex::ReSystem rsystem = brex::ReSystem::processSystem(ninfos, errors);

    if(!errors.empty()) {
        auto arr = Napi::Array::New(env);
        for(size_t i = 0; i < errors.size(); ++i) {
            arr.Set(i, Napi::String::New(env, std::string(errors[i].cbegin(), errors[i].cend())));
        }

        return arr;
    }

    for(size_t i = 0; i < rsystem.entries.size(); ++i) {
        if(rsystem.entries[i]->isUnicode()) {
            auto reentry = static_cast<brex::ReSystemUnicodeEntry*>(rsystem.entries[i]);

            g_namedExecutableUnicodeRegexMap[reentry->fullname] = reentry->executor;
            if(reentry->re->isValidNamedRegexComponent()) {
                g_namedUnicodeRegexMap[reentry->fullname] = static_cast<const brex::RegexSingleComponent*>(reentry->re->re)->entry.opt;
            }
        }
        else {
            auto reentry = static_cast<brex::ReSystemCEntry*>(rsystem.entries[i]);

            g_namedExecutableCRegexMap[reentry->fullname] = reentry->executor;
            if(reentry->re->isValidNamedRegexComponent()) {
                g_namedCRegexMap[reentry->fullname] = static_cast<const brex::RegexSingleComponent*>(reentry->re->re)->entry.opt;
            }
        }
    }

    nsremapper = rsystem.remapper;

    return env.Null();
}

///////////////////////////////////////////////////////////////////////////

Napi::Value ValidateStringLiteral(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();

    if(info.Length() != 1) {
        Napi::TypeError::New(env, "Expected a String to validate").ThrowAsJavaScriptException();
        return env.Null();
    }

    if(!info[0].IsString()) {
        Napi::TypeError::New(env, "Expected a String to validate").ThrowAsJavaScriptException();
        return env.Null();
    }

    std::string ustr = info[0].As<Napi::String>().Utf8Value();
    std::u8string str(ustr.cbegin(), ustr.cend());

    std::pair<std::optional<brex::UnicodeString>, std::optional<std::u8string>> uestr = brex::unescapeUnicodeStringLiteralInclMultiline((uint8_t*)str.c_str(), str.size());
    if(!uestr.first.has_value()) {
        Napi::Error::New(env, std::string(uestr.second.value().cbegin(), uestr.second.value().cend())).ThrowAsJavaScriptException();
        return env.Null();
    }

    std::string rstr = std::string(uestr.first.value().cbegin(), uestr.first.value().cend());
    return Napi::String::From<std::string>(env, rstr);
}

Napi::Value ValidateCStringLiteral(const Napi::CallbackInfo& info) 
{
    Napi::Env env = info.Env();

    if(info.Length() != 1) {
        Napi::TypeError::New(env, "Expected a String to validate").ThrowAsJavaScriptException();
        return env.Null();
    }

    if(!info[0].IsString()) {
        Napi::TypeError::New(env, "Expected a String to validate").ThrowAsJavaScriptException();
        return env.Null();
    }

    std::string ustr = info[0].As<Napi::String>().Utf8Value();
    std::u8string str(ustr.cbegin(), ustr.cend());

    std::pair<std::optional<brex::CString>, std::optional<std::u8string>> uestr = brex::unescapeCStringLiteralInclMultiline((uint8_t*)str.c_str(), str.size());
    if(!uestr.first.has_value()) {
        Napi::Error::New(env, std::string(uestr.second.value().cbegin(), uestr.second.value().cend())).ThrowAsJavaScriptException();
        return env.Null();
    }

    std::string rstr = std::string(uestr.first.value().cbegin(), uestr.first.value().cend());
    return Napi::String::From<std::string>(env, rstr);
}

Napi::Value EscapeStringLiteral(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();

    if(info.Length() != 1) {
        Napi::TypeError::New(env, "Expected a String to validate").ThrowAsJavaScriptException();
        return env.Null();
    }

    if(!info[0].IsString()) {
        Napi::TypeError::New(env, "Expected a String to validate").ThrowAsJavaScriptException();
        return env.Null();
    }

    std::string ustr = info[0].As<Napi::String>().Utf8Value();
    std::u8string str(ustr.cbegin(), ustr.cend());

    std::vector<uint8_t> uestr = brex::escapeUnicodeString(str);
    std::string rstr = std::string(uestr.cbegin(), uestr.cend());
    return Napi::String::From<std::string>(env, rstr);
}

Napi::Value EscapeCStringLiteral(const Napi::CallbackInfo& info) 
{
    Napi::Env env = info.Env();

    if(info.Length() != 1) {
        Napi::TypeError::New(env, "Expected a String to validate").ThrowAsJavaScriptException();
        return env.Null();
    }

    if(!info[0].IsString()) {
        Napi::TypeError::New(env, "Expected a String to validate").ThrowAsJavaScriptException();
        return env.Null();
    }

    std::string str = info[0].As<Napi::String>().Utf8Value();

    std::vector<uint8_t> uestr = brex::escapeCString(str);
    std::string rstr = std::string(uestr.cbegin(), uestr.cend());
    return Napi::String::From<std::string>(env, rstr);
}

///////////////////////////////////////////////////////////////////////////

std::string checkArgs(const Napi::CallbackInfo& info)
{
    if (info.Length() != 3) {
        return "Expected regex and a string as arguments + current namespace";
    }

    if (!info[0].IsString() || !info[1].IsString() || !info[2].IsString()) {
        return "Expected all arguments as strings";
    }

    return "";
}

Napi::Value Accepts(const Napi::CallbackInfo& info) 
{
    Napi::Env env = info.Env();

    std::string aok = checkArgs(info);
    if(aok != "") {
        Napi::TypeError::New(env, aok).ThrowAsJavaScriptException();
        return env.Undefined();
    }

    std::string uregex = info[0].As<Napi::String>().Utf8Value();
    std::u8string regex(uregex.cbegin(), uregex.cend());

    auto inns = info[2].As<Napi::String>().Utf8Value();
    std::string rrok = processRegexAsNeeded(inns, regex);
    if(rrok != "") {
        Napi::Error::New(env, rrok).ThrowAsJavaScriptException();
        return env.Undefined();
    }

    std::string ustr = info[1].As<Napi::String>().Utf8Value();

    bool accepts = false;
    brex::ExecutorError err;
    if(!isUnicodeRegex(regex)) {
        accepts = g_executableCRegexMap[regex]->test(&ustr, err);        
    }
    else {
        std::u8string str(ustr.cbegin(), ustr.cend());
        accepts = g_executableUnicodeRegexMap[regex]->test(&str, err);
    }
    
    if(err != brex::ExecutorError::Ok) {
        Napi::Error::New(env, "Error executing regex").ThrowAsJavaScriptException();
        return env.Undefined();
    }

    return Napi::Boolean::From<bool>(env, accepts);
}

Napi::Value StartsWith(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();

    std::string aok = checkArgs(info);
    if(aok != "") {
        Napi::TypeError::New(env, aok).ThrowAsJavaScriptException();
        return env.Undefined();
    }

    std::string uregex = info[0].As<Napi::String>().Utf8Value();
    std::u8string regex(uregex.cbegin(), uregex.cend());

    auto inns = info[2].As<Napi::String>().Utf8Value();
    std::string rrok = processRegexAsNeeded(inns, regex);
    if(rrok != "") {
        Napi::Error::New(env, rrok).ThrowAsJavaScriptException();
        return env.Undefined();
    }

    std::string ustr = info[1].As<Napi::String>().Utf8Value();

    bool accepts = false;
    brex::ExecutorError err;
    if(!isUnicodeRegex(regex)) {
        accepts = g_executableCRegexMap[regex]->testFront(&ustr, err);        
    }
    else {
        std::u8string str(ustr.cbegin(), ustr.cend());
        accepts = g_executableUnicodeRegexMap[regex]->testFront(&str, err);
    }
    
    if(err != brex::ExecutorError::Ok) {
        Napi::Error::New(env, "Error executing regex").ThrowAsJavaScriptException();
        return env.Undefined();
    }

    return Napi::Boolean::From<bool>(env, accepts);
}

Napi::Value EndsWith(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();

    std::string aok = checkArgs(info);
    if(aok != "") {
        Napi::TypeError::New(env, aok).ThrowAsJavaScriptException();
        return env.Undefined();
    }

    std::string uregex = info[0].As<Napi::String>().Utf8Value();
    std::u8string regex(uregex.cbegin(), uregex.cend());

    auto inns = info[2].As<Napi::String>().Utf8Value();
    std::string rrok = processRegexAsNeeded(inns, regex);
    if(rrok != "") {
        Napi::Error::New(env, rrok).ThrowAsJavaScriptException();
        return env.Undefined();
    }

    std::string ustr = info[1].As<Napi::String>().Utf8Value();

    bool accepts = false;
    brex::ExecutorError err;
    if(!isUnicodeRegex(regex)) {
        accepts = g_executableCRegexMap[regex]->testBack(&ustr, err);        
    }
    else {
        std::u8string str(ustr.cbegin(), ustr.cend());
        accepts = g_executableUnicodeRegexMap[regex]->testBack(&str, err);
    }
    
    if(err != brex::ExecutorError::Ok) {
        Napi::Error::New(env, "Error executing regex").ThrowAsJavaScriptException();
        return env.Undefined();
    }

    return Napi::Boolean::From<bool>(env, accepts);
}

std::string checkInfoArgs(const Napi::CallbackInfo& info)
{
    if (info.Length() != 2) {
        return "Expected regex + current namespace";
    }

    if (!info[0].IsString() || !info[1].IsString()) {
        return "Expected all arguments as strings";
    }

    return "";
}

Napi::Value GetBSQIRForm(const Napi::CallbackInfo& info) 
{
    Napi::Env env = info.Env();

    std::string aok = checkInfoArgs(info);
    if(aok != "") {
        Napi::TypeError::New(env, aok).ThrowAsJavaScriptException();
        return env.Undefined();
    }

    std::string uregex = info[0].As<Napi::String>().Utf8Value();
    std::u8string regex(uregex.cbegin(), uregex.cend());

    auto inns = info[1].As<Napi::String>().Utf8Value();
    std::string rrok = processRegexAsNeeded(inns, regex);
    if(rrok != "") {
        Napi::Error::New(env, rrok).ThrowAsJavaScriptException();
        return env.Undefined();
    }

    std::string rebsqir;
    if(!isUnicodeRegex(regex)) {
        rebsqir = g_executableCRegexMap[regex]->getBSQIRInfo().first;        
    }
    else {
        rebsqir = g_executableUnicodeRegexMap[regex]->getBSQIRInfo().first;
    }
    
    return Napi::String::From<std::string>(env, rebsqir);
}

Napi::Value GetSMTForm(const Napi::CallbackInfo& info) 
{
     Napi::Env env = info.Env();

    std::string aok = checkInfoArgs(info);
    if(aok != "") {
        Napi::TypeError::New(env, aok).ThrowAsJavaScriptException();
        return env.Undefined();
    }

    std::string uregex = info[0].As<Napi::String>().Utf8Value();
    std::u8string regex(uregex.cbegin(), uregex.cend());

    auto inns = info[1].As<Napi::String>().Utf8Value();
    std::string rrok = processRegexAsNeeded(inns, regex);
    if(rrok != "") {
        Napi::Error::New(env, rrok).ThrowAsJavaScriptException();
        return env.Undefined();
    }

    std::string rebsqir;
    if(!isUnicodeRegex(regex)) {
        rebsqir = g_executableCRegexMap[regex]->getBSQIRInfo().second;        
    }
    else {
        rebsqir = g_executableUnicodeRegexMap[regex]->getBSQIRInfo().second;
    }
    
    return Napi::String::From<std::string>(env, rebsqir);
}

///////////////////////////////////////////////////////////////////////////

Napi::Object Init(Napi::Env env, Napi::Object exports) 
{
    exports.Set(Napi::String::New(env, "loadConstAndValidateRESystem"), Napi::Function::New(env, LoadConstAndValidateRESystem));

    exports.Set(Napi::String::New(env, "validateStringLiteral"), Napi::Function::New(env, ValidateStringLiteral));
    exports.Set(Napi::String::New(env, "validateCStringLiteral"), Napi::Function::New(env, ValidateCStringLiteral));

    exports.Set(Napi::String::New(env, "accepts"), Napi::Function::New(env, Accepts));
    exports.Set(Napi::String::New(env, "startsWith"), Napi::Function::New(env, StartsWith));
    exports.Set(Napi::String::New(env, "endsWith"), Napi::Function::New(env, EndsWith));

    exports.Set(Napi::String::New(env, "escapeStringLiteral"), Napi::Function::New(env, EscapeStringLiteral));
    exports.Set(Napi::String::New(env, "escapeCStringLiteral"), Napi::Function::New(env, EscapeCStringLiteral));

    exports.Set(Napi::String::New(env, "getBSQIRForm"), Napi::Function::New(env, GetBSQIRForm));
    exports.Set(Napi::String::New(env, "getSMTForm"), Napi::Function::New(env, GetSMTForm));

    return exports;
}

NODE_API_MODULE(addon, Init)
