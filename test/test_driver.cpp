#include "test_driver.h"

#include <iostream>
#include <fstream>

void loadAssemblyJSONExplicit(const char* filename, json& jv)
{
    std::ifstream infile(filename);
    infile >> jv;
}

std::string wsnorm(const std::string& s) {
    std::regex reg(R"(\s+)");
    std::string ss = std::regex_replace(s, reg, " ");

    if(ss.starts_with(" ")) {
        ss = ss.substr(1);
    }

    if(ss.ends_with(" ")) {
        ss = ss.substr(0, ss.size() - 1);
    }

    return ss;
}

bool compareToOrig(const std::string& rstr, char* datafile, std::string& err) {
    std::string origstr;
    std::ifstream infile(datafile);
    infile >> origstr;

    std::string s = wsnorm(rstr);
    std::string o = wsnorm(origstr);

    if(rstr == origstr) {
        return true;
    }
    else {
        err = "Expected:\n" + o + "\n\nGot:\n" + s;
        
        return false;
    }
}

bool tround(char* metafile, char* type, char* datafile, std::string& err)
{
    err = "";
    std::string metadata, type, data;
    
    //the property value is the BSQON value (as a JSON string) so parse it
    const BSQON_AST_Node* node = BSQON_AST_parse_from_file(datafile);
    char** errorInfo = (char**)malloc(sizeof(char*) * 128);
    size_t errorInfoCount = BSQON_AST_getErrorInfo(errorInfo);

    if(node == nullptr) {
        for(size_t i = 0; i < errorInfoCount; ++i) {
            err += ("++ %s\n", errorInfo[i]);
        }

        return false;
    }

    //parse the JSON 
    json jv = nullptr;
    loadAssemblyJSONExplicit(metafile, jv);

    //the property assembly is the code so load it
    bsqon::AssemblyInfo assembly;
    bsqon::AssemblyInfo::parse(jv, assembly);

    bsqon::Parser parser(&assembly);

    //the property loadtype is the type so look it up
    auto loadtype = assembly.lookupTypeKey(type);

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
    
    //finally parse the value
    bsqon::BsqonDecl* res = parser.parseBSQON(metadata, loadtype, node);

    if(parser.errors.empty() && errorInfoCount == 0) {
        std::u8string rstr = res->toString();
        std::string sstr(rstr.begin(), rstr.end());

        return compareToOrig(sstr, datafile, err);
    }
    else {
        for(size_t i = 0; i < errorInfoCount; ++i) {
            std::string sstr(errorInfo[i]);
            if(!sstr.starts_with("syntax error")) {
                err += ("%s\n", sstr.c_str());
            }
        }

        for(size_t i = 0; i < parser.errors.size(); ++i) {
            const bsqon::ParseError& pe = parser.errors.at(i);
            err += ("%s -- line %u\n", pe.message.c_str(), pe.loc.first_line);
        }

        return false;
   }
}
