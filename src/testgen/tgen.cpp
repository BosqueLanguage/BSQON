#include "component.h"
#include "generator_rnd.h"
#include "generator_ai.h"

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

void usage()
{
    printf("Usage: generator <assembly.json> <loadtype> <--rnd|--agent|--combinatorial|--smt|--all>\n");
    exit(1);
}

const bsqon::Type* loadTypeForGenerate(const bsqon::AssemblyInfo* ainfo, const std::string& tkey)
{
    auto t = ainfo->lookupTypeKey(tkey);
    if(t->isUnresolved()) {
        printf("Invalid 'loadtype' -- %s\n", tkey.c_str());
        exit(1);
    }

    return t;
}

std::vector<bsqon::Value*> generateRandomTestSuite(const bsqon::AssemblyInfo* assembly, ValueSetPartition& vspartition, const bsqon::Type* loadtype, size_t count)
{
    TypeGeneratorRandom generator;
    for(size_t i = 0; i < vspartition.components.size(); ++i) {
        generator.generateType(vspartition.components[i]->context.valuetype, vspartition.components[i]);
    }

    TestGenerator tgen(assembly, &vspartition, {});

    std::vector<bsqon::Value*> tests;
    for(size_t i = 0; i < count; ++i) {
        auto res = tgen.generateType(loadtype, "var");
        tests.push_back(res);
    }

    return tests;
}

std::vector<bsqon::Value*> generateAgentTestSuite(const bsqon::AssemblyInfo* assembly, ValueSetPartition& vspartition, const bsqon::Type* loadtype, size_t count)
{
    printf("Agent test suite generation not yet implemented\n");
    exit(1);
}

std::vector<bsqon::Value*> generateCombinatorialTestSuite(const bsqon::AssemblyInfo* assembly, ValueSetPartition& vspartition, const bsqon::Type* loadtype)
{
    std::vector<bsqon::Value*> tests;

    for(size_t i = 0; i < vspartition.components.size(); ++i) {
        for(size_t j = i + 1; j < vspartition.components.size(); ++j) {
        
            auto pi = vspartition.components[i];
            auto pj = vspartition.components[j];

            std::vector<const ValueConstraint*> constraints;
            constraints.insert(constraints.end(), pi->constraints.cbegin(), pi->constraints.cend());
            constraints.insert(constraints.end(), pj->constraints.cbegin(), pj->constraints.cend());

            if(TestGenerator::checkConstraintSatisfiability(constraints)) {
                TestGenerator tgen(assembly, &vspartition, constraints);
                
                auto res = tgen.generateType(loadtype, "var");
                tests.push_back(res);
            }
        }
    }

    return tests;
}

std::vector<bsqon::Value*> generateTestSuite(const bsqon::AssemblyInfo* assembly, ValueSetPartition& vspartition, const bsqon::Type* loadtype)
{
    printf("Full test suite generation not yet implemented\n");
    exit(1);
}

int main(int argc, char** argv, char **envp)
{
    if(argc != 4) {
        usage();
        exit(1);
    }

    std::string metadata, type;
    //TODO: arg processing

    //parse the JSON 
    json jv = nullptr;
    loadAssemblyJSONExplicit(argv[1], jv);

    bsqon::AssemblyInfo assembly;
    bsqon::AssemblyInfo::parse(jv, assembly);

    AIValueSetGenerator aigenerator;
    ValueSetGenerator generator(&assembly);
    const bsqon::Type* loadtype = assembly.lookupTypeKey(argv[2]);

    ValueSetGeneratorEnvironment venv{"var", {}, GenerateContext{}};
    ValueSetGeneratorEnvironment aivenv{"var", {}, GenerateContext{}};
    ValueSetPartition vspartition = generator.generateType(loadtype, venv);
    ValueSetPartition aivspartition = aigenerator.generateType(loadtype, aivenv);

    std::vector<bsqon::Value*> tests;
    auto modestr = std::string(argv[3]);
    if(modestr == "--rnd") {
        tests = generateRandomTestSuite(&assembly, vspartition, loadtype, 10); //TODO: maybe make this a command parameter --rnd=X (or computa as a function of # partitions)
    }
    else if(modestr == "--agent") {
        tests = generateAgentTestSuite(&assembly, vspartition, loadtype, 10); //TODO: maybe make this a command parameter --rnd=X (or computa as a function of # partitions)
    }
    else if(modestr == "--combinatorial") {
        tests = generateCombinatorialTestSuite(&assembly, vspartition, loadtype);
    }
    else if(modestr == "--all") {
        tests = generateTestSuite(&assembly, vspartition, loadtype);
    }
    else {
        usage();
    }

    //Print out tests -- 1 per line
    for(size_t i = 0; i < tests.size(); ++i) {
        std::u8string rstr = tests[i]->toString();
        printf("%s\n", (const char*)rstr.c_str());
    }

    fflush(stdout);
    exit(0);
}
