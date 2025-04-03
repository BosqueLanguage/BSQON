#include "component.h"
#include "generator_rnd.h"
#include "generator_ai.h"

#include <filesystem>
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

void loadTestSignatureExplicit(const char* filename, json& jv)
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
    printf("Usage: generator <assembly.json> <api.json> <--rnd|--agent|--combinatorial|--smt|--all> <outdir>\n");
    exit(1);
}

std::vector<bsqon::Value*> generateArgValues(TestGenerator& tgen, const APISignature& testsig)
{
    std::vector<bsqon::Value*> argvalues;
    for(size_t i = 0; i < testsig.args.size(); ++i) {
        auto arg = testsig.args[i];
        auto t = tgen.assembly->lookupTypeKey(arg.second->tkey);

        auto res = tgen.generateType(t, arg.first);
        argvalues.push_back(res);
    }

    return argvalues;
}

std::vector<std::vector<bsqon::Value*>> generateRandomTestSuite(const bsqon::AssemblyInfo* assembly, ValueSetPartition& vspartition, const APISignature& testsig, size_t count)
{
    RandomValueGenerator generator;
    for(size_t i = 0; i < vspartition.components.size(); ++i) {
        generator.generateType(vspartition.components[i]->context.valuetype, vspartition.components[i]);
    }

    TestGenerator tgen(assembly, &vspartition, {});

    std::vector<std::vector<bsqon::Value*>> tests;
    for(size_t i = 0; i < count; ++i) {
        tests.emplace_back(generateArgValues(tgen, testsig));
    }

    return tests;
}

std::vector<std::vector<bsqon::Value*>> generateAgentTestSuite(const bsqon::AssemblyInfo* assembly, ValueSetPartition& vspartition, const APISignature& testsig, size_t count)
{
    AIValueGenerator generator(AIModelOption::GEMINI, &testsig);
    for(size_t i = 0; i < vspartition.components.size(); ++i) {
        auto genv = ValueSetGeneratorEnvironment{vspartition.components[i]->path, vspartition.components[i]->constraints, vspartition.components[i]->context};
        generator.generateType(vspartition.components[i]->context.valuetype, genv, vspartition.components[i]);
    }

    TestGenerator tgen(assembly, &vspartition, {});

    std::vector<std::vector<bsqon::Value*>> tests;
    for(size_t i = 0; i < count; ++i) {
        tests.emplace_back(generateArgValues(tgen, testsig));
    }

    return tests;
}

std::vector<std::vector<bsqon::Value*>> generateCombinatorialTestSuite(const bsqon::AssemblyInfo* assembly, ValueSetPartition& vspartition, const APISignature& testsig)
{
    AIValueGenerator generator(AIModelOption::GEMINI, &testsig);
    for(size_t i = 0; i < vspartition.components.size(); ++i) {
        auto genv = ValueSetGeneratorEnvironment{vspartition.components[i]->path, vspartition.components[i]->constraints, vspartition.components[i]->context};
        generator.generateType(vspartition.components[i]->context.valuetype, genv, vspartition.components[i]);
    }

    std::vector<std::vector<bsqon::Value*>> tests;
    if(vspartition.components.size() == 1) {
        auto p0 = vspartition.components[0];
        
        std::vector<const ValueConstraint*> constraints;
        constraints.insert(constraints.end(), p0->constraints.cbegin(), p0->constraints.cend());
        
        for(size_t m = 0; m < p0->options.size(); ++m) {
            std::vector<const ValueConstraint*> fconstraints;
            fconstraints.insert(fconstraints.end(), constraints.cbegin(), constraints.cend());

            fconstraints.push_back(new FixedValueConstraint(p0->path, p0->options[m]));

            if(TestGenerator::checkConstraintSatisfiability(fconstraints)) {
                TestGenerator tgen(assembly, &vspartition, fconstraints);
                tests.emplace_back(generateArgValues(tgen, testsig));
            }
        }
    }
    else {
        for(size_t i = 0; i < vspartition.components.size(); ++i) {
            for(size_t j = i + 1; j < vspartition.components.size(); ++j) {
        
                auto pi = vspartition.components[i];
                auto pj = vspartition.components[j];

                std::vector<const ValueConstraint*> constraints;
                constraints.insert(constraints.end(), pi->constraints.cbegin(), pi->constraints.cend());
                constraints.insert(constraints.end(), pj->constraints.cbegin(), pj->constraints.cend());

                for(size_t m = 0; m < pi->options.size(); ++m) {
                    for(size_t n = 0; n < pj->options.size(); ++n) {
                        std::vector<const ValueConstraint*> fconstraints;
                        fconstraints.insert(fconstraints.end(), constraints.cbegin(), constraints.cend());

                        fconstraints.push_back(new FixedValueConstraint(pi->path, pi->options[m]));
                        fconstraints.push_back(new FixedValueConstraint(pj->path, pj->options[n]));

                        if(TestGenerator::checkConstraintSatisfiability(fconstraints)) {
                            TestGenerator tgen(assembly, &vspartition, fconstraints);
                            tests.emplace_back(generateArgValues(tgen, testsig));
                        }
                    }
                }

                //TODO: do we want to force/ensure an equality and disequality if the types are the same?
            }
        }
    }

    return tests;
}

std::vector<std::vector<bsqon::Value*>> generateTestSuite(const bsqon::AssemblyInfo* assembly, ValueSetPartition& vspartition, const APISignature& testsig)
{
    printf("Full test suite generation not yet implemented\n");
    exit(1);
}

int main(int argc, char** argv, char **envp)
{
    if(argc != 5) {
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

    ValueSetGenerator generator(&assembly);

    json jssig = nullptr;
    loadTestSignatureExplicit(argv[2], jssig);

    APISignature testsig = APISignature::parse(assembly, jssig);

    std::vector<ValueSetPartition> argpartitions;
    for(size_t i = 0; i < testsig.args.size(); ++i) {
        auto arg = testsig.args[i];
        auto t = assembly.lookupTypeKey(arg.second->tkey);
        ValueSetPartition vspartition = generator.generateType(t, ValueSetGeneratorEnvironment{arg.first, {}, GenerateContext{}});
        argpartitions.push_back(vspartition);
    }
    
    ValueSetPartition vspartition = ValueSetPartition::punion(argpartitions);

    //TODO: need flag on agent... to specify which model to use -- right now defaults to GEMINI

    std::vector<std::vector<bsqon::Value*>> tests;
    if(testsig.args.size() != 0) {
        auto modestr = std::string(argv[3]);
        if(modestr == "--rnd") {
            tests = generateRandomTestSuite(&assembly, vspartition, testsig, 10); //TODO: maybe make this a command parameter --rnd=X (or computa as a function of # partitions)
        }
        else if(modestr == "--agent") {
            tests = generateAgentTestSuite(&assembly, vspartition, testsig, 10); //TODO: maybe make this a command parameter --rnd=X (or computa as a function of # partitions)
        }
        else if(modestr == "--combinatorial") {
            tests = generateCombinatorialTestSuite(&assembly, vspartition, testsig);
        }
        else if(modestr == "--all") {
            tests = generateTestSuite(&assembly, vspartition, testsig);
        }
        else {
            usage();
        }
    }

    //Print out tests -- 1 per line
    std::cout << "---- Tests ----" << std::endl;
    for(size_t i = 0; i < tests.size(); ++i) {
        if(i != 0) {
            std::cout << "," << std::endl;
        }
        std::cout << "[";
        for(size_t j = 0; j < tests[i].size(); ++j) {
            std::u8string rstr = tests[i][j]->toString();
            if(j != 0) {
                std::cout << ",";
            }
            std::cout << std::endl;

            std::cout << "    " << std::string(rstr.cbegin(), rstr.cend());
        }
        std::cout << std::endl << "]";
    }
    std::cout << std::endl;

    //write the json tests, ONE per file, to the output directory
    auto trgtdir = std::string(argv[4]);
    if(std::filesystem::exists(trgtdir)) {
        std::filesystem::remove_all(trgtdir);
    }
    std::filesystem::create_directories(trgtdir);

    for(size_t i = 0; i < tests.size(); ++i) {
        std::string fname = trgtdir + "/test_" + std::to_string(i) + ".json";
        std::ofstream outfile(fname);

        json jargs;
        for(size_t j = 0; j < testsig.args.size(); ++j) {
            jargs[testsig.args[j].first] = tests[i][j]->toJSON();
        }

        outfile << jargs.dump(4);
    }

    fflush(stdout);
    exit(0);
}
