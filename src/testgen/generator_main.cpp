#include "generator.h"

#include <iostream>
#include <fstream>


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


int main(int argc, char** argv, char **envp)
{
    std::string metadata, type;
    //TODO: arg processing
    
    TypeGeneratorRandom generator;

    //parse the JSON 
    json jv = nullptr;
    loadAssemblyJSONExplicit(argv[1], jv);
    bsqon::AssemblyInfo::parse(jv, generator.assembly);

    //the property loadtype is the type so look it up
    const bsqon::Type* loadtype = generator.assembly.lookupTypeKey(argv[2]);

    if(loadtype->isUnresolved()) {
        printf("Invalid 'loadtype'\n");
        exit(1);
    }

    //finally parse the value
    bsqon::Value* res = generator.generateType(loadtype);

    std::u8string rstr = res->toString();
    printf("%s\n", (const char*)rstr.c_str());

    fflush(stdout);
    exit(0);
}
