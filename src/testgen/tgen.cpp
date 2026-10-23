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
    printf("Usage: generator <assembly.json> <api.json> <--rnd|--agent|--combinatorial|--onewise|--smt|--all> <outdir> [--failure-aware-input-generation] [--failure-aware-mock-flow --failure-context <file>]\n");
    exit(1);
}

void loadFailureContextExplicit(const char* filename, json& jv)
{
    try
    {
        std::ifstream infile(filename);
        infile >> jv;
    }
    catch(const std::exception& e)
    {
        printf("Error parsing failure context JSON: %s\n", e.what());
        exit(1);
    }
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
    AIValueGenerator generator(AIModelOption::DEEP_SEEK, &testsig);
    //AIValueGenerator generator(AIModelOption::GEMINI, &testsig);
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
     AIValueGenerator generator(AIModelOption::DEEP_SEEK, &testsig);
    //AIValueGenerator generator(AIModelOption::GEMINI, &testsig);
    for(size_t i = 0; i < vspartition.components.size(); ++i) {
        std::cout << "Processing component " << (i+1) << "/" << vspartition.components.size() << std::endl;
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

/** 1-wise: for each partition, enumerate all its options; other partitions use random selection. */
std::vector<std::vector<bsqon::Value*>> generateOneWiseTestSuite(const bsqon::AssemblyInfo* assembly, ValueSetPartition& vspartition, const APISignature& testsig)
{
    AIValueGenerator generator(AIModelOption::DEEP_SEEK, &testsig);
    for(size_t i = 0; i < vspartition.components.size(); ++i) {
        std::cout << "Processing component " << (i+1) << "/" << vspartition.components.size() << " (1-wise)" << std::endl;
        auto genv = ValueSetGeneratorEnvironment{vspartition.components[i]->path, vspartition.components[i]->constraints, vspartition.components[i]->context};
        generator.generateType(vspartition.components[i]->context.valuetype, genv, vspartition.components[i]);
    }

    std::vector<std::vector<bsqon::Value*>> tests;
    for(size_t i = 0; i < vspartition.components.size(); ++i) {
        auto pi = vspartition.components[i];
        std::vector<const ValueConstraint*> base_constraints(pi->constraints.cbegin(), pi->constraints.cend());

        for(size_t m = 0; m < pi->options.size(); ++m) {
            std::vector<const ValueConstraint*> fconstraints(base_constraints.cbegin(), base_constraints.cend());
            fconstraints.push_back(new FixedValueConstraint(pi->path, pi->options[m]));

            if(TestGenerator::checkConstraintSatisfiability(fconstraints)) {
                TestGenerator tgen(assembly, &vspartition, fconstraints);
                tests.emplace_back(generateArgValues(tgen, testsig));
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
    if(argc < 5) {
        usage();
        exit(1);
    }

    bool failureAwareMockFlowEnabled = false;
    bool failureAwareInputGenerationEnabled = false;
    std::optional<std::string> failureContextPath = std::nullopt;
    for (int i = 5; i < argc; ++i) {
        std::string opt = std::string(argv[i]);
        if (opt == "--failure-aware-mock-flow") {
            failureAwareMockFlowEnabled = true;
        }
        else if (opt == "--failure-aware-input-generation") {
            failureAwareInputGenerationEnabled = true;
        }
        else if (opt == "--failure-context") {
            if (i + 1 >= argc) {
                usage();
                exit(1);
            }
            failureContextPath = std::string(argv[++i]);
        }
        else {
            usage();
            exit(1);
        }
    }
    if (failureAwareMockFlowEnabled && !failureContextPath.has_value()) {
        printf("Error: --failure-context <file> is required when --failure-aware-mock-flow is enabled.\n");
        usage();
        exit(1);
    }
    if (!failureAwareMockFlowEnabled && failureContextPath.has_value()) {
        printf("Error: --failure-context requires --failure-aware-mock-flow.\n");
        usage();
        exit(1);
    }

    setFailureAwareMockFlowEnabled(failureAwareMockFlowEnabled);
    setFailureAwareInputGenerationEnabled(failureAwareInputGenerationEnabled);
    clearFailurePromptContext();
    if (failureAwareMockFlowEnabled) {
        json failureContext = nullptr;
        loadFailureContextExplicit(failureContextPath.value().c_str(), failureContext);
        setFailurePromptContext(failureContext);
    }

    std::string metadata, type;
    //TODO: arg processing

    //parse the JSON 
    json jv = nullptr;
    loadAssemblyJSONExplicit(argv[1], jv);

    bsqon::AssemblyInfo assembly;
    bsqon::loadAssembly(jv, assembly);

    ValueSetGenerator generator(&assembly);

    json jssig = nullptr;
    loadTestSignatureExplicit(argv[2], jssig);

    APISignature testsig = APISignature::parse(assembly, jssig);

    if (failureAwareMockFlowEnabled) {
        json failureEntries = generateFailureMockEntries(testsig.toString());

        auto trgtdir = std::string(argv[4]);
        if (std::filesystem::exists(trgtdir)) {
            std::filesystem::remove_all(trgtdir);
        }
        std::filesystem::create_directories(trgtdir);

        std::cout << "---- Failure Mocks ----" << std::endl;
        std::cout << failureEntries.dump(2) << std::endl;

        for (size_t i = 0; i < failureEntries.size(); ++i) {
            std::string fname = trgtdir + "/test_" + std::to_string(i) + ".json";
            std::ofstream outfile(fname);
            outfile << failureEntries[i].dump(4);
        }

        fflush(stdout);
        exit(0);
    }

    std::vector<ValueSetPartition> argpartitions;
    for(size_t i = 0; i < testsig.args.size(); ++i) {
        auto arg = testsig.args[i];
        auto t = assembly.lookupTypeKey(arg.second->tkey);
        ValueSetPartition vspartition = generator.generateType(t, ValueSetGeneratorEnvironment{arg.first, {}, GenerateContext{}});
        argpartitions.push_back(vspartition);
    }
    
    ValueSetPartition vspartition = ValueSetPartition::punion(argpartitions);

    //TODO: need flag on agent... to specify which model to use -- right now defaults to OPEN_AI

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
        else if(modestr == "--onewise") {
            tests = generateOneWiseTestSuite(&assembly, vspartition, testsig);
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
            // std::string rstr = tests[i][j]->toString();
            auto str = tests[i][j]->toString();
            std::string rstr(str.begin(), str.end());

            if(j != 0) {
                std::cout << ",";
            }
            std::cout << std::endl;

            std::cout << "    " << rstr;
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
