#include "../info/type_info.h"
#include "../info/bsqon.h"
#include "bsqon_parse.h"

#include <iostream>
#include <fstream>

bool processArgs(int argc, char** argv, std::string& metadata, std::string& type, std::string& data, bool& loadenv)
{
    if(argc < 2) {
        printf("usage: bsqon [metadata.json] [type] data.bsqon\n");
        return false;
    }

    loadenv = false;
    std::vector<std::string> args;
    for(int i = 1; i < argc; ++i) {
        if(strcmp(argv[i], "--loadenv") == 0) {
            loadenv = true;
        }
        else {
            args.push_back(argv[i]);
        }
    }

    size_t apos = 0;
    if(apos < args.size() && args[apos].ends_with(".json")) {
        metadata = args[apos];
        apos++;
    }
    else {
        metadata = "<implicit>";
    }

    if(apos < args.size() && !args[apos].ends_with(".bsqon")) {
        type = args[apos];
        apos++;
    }
    else {
        type = "<implicit>";
    }

    data = "<unset>";
    if(apos < args.size() && args[apos].ends_with(".bsqon")) {
        data = args[apos];
        apos++;
    }
    else {
        printf("usage: bsqon [metadata.json] [type] data.bsqon\n");
        printf("missing bsqon file\n");
        return false;
    }

    if(apos < args.size()) {
        printf("usage: bsqon [metadata.json] [type] data.bsqon\n");
        printf("too many arguments\n");
        return false;
    }

    return true;
}

std::map<std::string, std::pair<const BSQON_AST_Node*, const uint8_t*>> loadEnvironmentValues(char** envp, std::map<std::string, const BSQON_AST_Node*>& envkeys)
{
    std::map<std::string, std::pair<const BSQON_AST_Node*, const uint8_t*>> envmap;
    for(auto ii = envkeys.cbegin(); ii != envkeys.cend(); ++ii) {
        auto envval = (const uint8_t*)getenv(ii->first.c_str());
        if(envval == nullptr) {
            printf("Missing environment variable '%s'\n", ii->first.c_str());
            exit(1);
        }

        envmap[ii->first] = std::make_pair(ii->second, envval);
    }
    return envmap;
}

void loadAssemblyJSONExplicit(const char* filename, json& jv)
{
    try
    {
        std::ifstream infile(filename);
        infile >> jv;
    }
    catch(const std::exception& e)
    {
        printf("Error parsing JSON: %s\n", e.what());
        exit(1);
    }
}

void loadAssemblyJSONImplicit(std::string resolvestr, json& jv)
{
    if(resolvestr.starts_with("file:///")) {
        std::string filename = resolvestr.substr(8);
        loadAssemblyJSONExplicit(filename.c_str(), jv);
    }
    else {
        //TODO: should handle http: and git as well

        printf("Unhandled or invalid assembly resolver -- \"%s\"\n", resolvestr.c_str());
        exit(1);
    }
}

int main(int argc, char** argv, char **envp)
{
    std::string metadata, type, data;
    bool loadenv = false;
    if(!processArgs(argc, argv, metadata, type, data, loadenv)) {
        return 1;
    }

    //the property value is the BSQON value (as a JSON string) so parse it
    const BSQON_AST_Node* node = argc == 3 ? BSQON_AST_parse_from_stdin() : BSQON_AST_parse_from_file(argv[3]);
    char** errorInfo = (char**)malloc(sizeof(char*) * 128);
    size_t errorInfoCount = BSQON_AST_getErrorInfo(errorInfo);

    if(node == nullptr) {
        for(size_t i = 0; i < errorInfoCount; ++i) {
            printf("++ %s\n", errorInfo[i]);
        }

        fflush(stdout);
        exit(1);
    }

    //parse the JSON 
    json jv = nullptr;
    auto metainfo = bsqon::Parser::getAssemblyAndType(node);
    if(metainfo.first.has_value()) {
        if(metadata != "<implicit>") {
            printf("Assembly info specified on BOTH command line and as she-bang in package -- using she-bang version\n");
        }

        loadAssemblyJSONImplicit(metainfo.first.value(), jv);
    }
    else {
        if(metadata == "<implicit>") {
            printf("Assembly info not specified on command line or as she-bang in BSQON package\n");
            exit(1);
        }

        loadAssemblyJSONExplicit(argv[1], jv);
    }

    //the property assembly is the code so load it
    bsqon::AssemblyInfo assembly;
    bsqon::AssemblyInfo::parse(jv, assembly);

    bsqon::Parser parser(&assembly);

    //the property loadtype is the type so look it up
    const bsqon::Type* loadtype = nullptr;
    if(metainfo.second.has_value()) {
        if(metadata != "<implicit>") {
            printf("Assembly info specified on BOTH command line and as she-bang in package -- using she-bang version\n");
        }

        loadtype = assembly.lookupTypeKey(metainfo.second.value());
    }
    else {
        if(metadata == "<implicit>") {
            printf("Assembly info not specified on command line or as she-bang in BSQON package\n");
            exit(1);
        }

        loadtype = assembly.lookupTypeKey(argv[2]);
    }

    if(loadtype->isUnresolved()) {
        printf("Invalid 'loadtype'\n");
        exit(1);
    }
    
    auto ccpos = loadtype->tkey.find("::");
    if(ccpos == std::string::npos) {
        parser.defaultns = "Core";
    }
    else {
        parser.defaultns = loadtype->tkey.substr(0, ccpos);
    }

    //load up any environment variables that we need
    auto envkeys = bsqon::Parser::getEnvironmentBindKeys(node);
    auto envmap = loadEnvironmentValues(envp, envkeys);
    for(auto ii = envmap.cbegin(); ii != envmap.cend(); ++ii) {
        auto envname = ii->first;
        auto envtype = ii->second.first;
        auto envvalue = ii->second.second;
        auto envlen = 0;
        while(envvalue[envlen] != 0) {
            envlen++;
        }
        
        const bsqon::Type* etype = parser.parseType(envtype);
        if(etype->isUnresolved()) {
            printf("Invalid environment type\n");
            exit(1);
        }

        auto envpos = bsqon::SourcePos{0, 0, 0, 0};
        if(etype->tag == bsqon::TypeTag::TYPE_PRIMITIVE) {
            if(etype->tkey == "String") {
                parser.envbinds[envname] = bsqon::StringValue::createFromParse(etype, envpos, envvalue, envlen);
            }
            else if(etype->tkey == "CString") {
                parser.envbinds[envname] = bsqon::CStringValue::createFromParse(etype, envpos, envvalue, envlen);
            }
            else {
                printf("TODO: want to support most primitives + stringofs, paths, and typedecls\n");
                exit(1);
            }
        }
    }

    //finally parse the value
    bsqon::BsqonDecl* res = parser.parseBSQON(metadata, loadtype, node);

    if(parser.errors.empty() && errorInfoCount == 0) {
        std::u8string rstr = res->toString(false);
        printf("%s\n", (const char*)rstr.c_str());

        fflush(stdout);
        exit(0);
    }
    else {
        for(size_t i = 0; i < errorInfoCount; ++i) {
            std::string sstr(errorInfo[i]);
            if(!sstr.starts_with("syntax error")) {
                printf("%s\n", sstr.c_str());
            }
        }

        for(size_t i = 0; i < parser.errors.size(); ++i) {
            const bsqon::ParseError& pe = parser.errors.at(i);
            printf("%s -- line %u\n", pe.message.c_str(), pe.loc.first_line);
        }

        fflush(stdout);
        exit(1);
    }
}
