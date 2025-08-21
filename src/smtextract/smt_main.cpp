#include "smt_extract.h"
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <string>

char *ReadFile(const char *filename) {
  FILE *f = fopen(filename, "r");
  if (!f) {
    perror("fopen");
  }
  fseek(f, 0, SEEK_END);
  long length = ftell(f);
  rewind(f);
  char *bfr = (char *)malloc(sizeof(char) * length);

  fread(bfr, sizeof(char), length, f);

  fclose(f);
  return bfr;
}


int main(int argc, char** argv)
{
    if(argc != 5) {
        badArgs("");
    }

    if(argc == 4) {
        badArgs("Missing --<MODE>");
    }

    // Load SMT FILE
    const char* smt_file = argv[1];
    if(validPath(smt_file, "smt2") == false) {
        badArgs("Incorrect .smt2 file");
    }
	//HACK: Fix to prevent z3 to optimize the functions that use define-fun.
	char * smt_bfr = ReadFile(smt_file);
	std::string smt_str(smt_bfr);
	free(smt_bfr);

	std::string helper_fn = ";;;;;;;;;;;;;;;\n (declare-fun MockTest (Int) Int) (assert (> (MockTest 5) 2))";
	std::string smt_formula = smt_str + helper_fn;

    z3::context c;
    z3::solver sol(c);
	sol.from_string(smt_formula.c_str());

    z3::check_result chk_smt = sol.check();
    if(chk_smt == z3::unsat) {
        badArgs("Unsat smt file\n");
    }
    else if(chk_smt == z3::unknown) {
        badArgs("Unknown smt file\n");
    }

    // Load TYPEINFO FILE
    const char* asm_file = argv[3];
    if(validPath(asm_file, "json") == false) {
        badArgs("Incorrect .json file");
    }

    json j_type;
    bsqon::AssemblyInfo asm_info;
    try {
        std::ifstream infile(asm_file);
        infile >> j_type;
    }
    catch(const std::exception& e) {
        printf("Error parsing JSON: %s\n", e.what());
        exit(1);
    }
    bsqon::AssemblyInfo::parse(j_type, asm_info);
    bsqon::loadAssembly(j_type, asm_info);

    // Load FN INFO FILE
    const char* fn_info_file = argv[2];
    if(validPath(fn_info_file, "json") == false) {
        badArgs("Incorrect .json file");
    }

    json fn_json;
    try {
        std::ifstream infile(fn_info_file);
        infile >> fn_json;
    }
    catch(const std::exception& e) {
        printf("Error parsing JSON: %s\n", e.what());
        exit(1);
    }

    std::map<std::string, bsqon::Type*> arg_refs;
    for(const auto& arg : fn_json["args"]) {
        arg_refs[arg["name"]] = asm_info.lookupTypeKey(arg["type"]);
    }

    std::string mode(argv[4]);

    if(mode == "-e" || mode == "--extract") {
        // for(const auto& [id, type] : arg_refs) {
        //     z3::expr ex = getBsqTypeExpr(id, type, sol).value();
        //
        //     ValueExtractor extract(&asm_info, type, ex, sol);
        //     bsqon::Value* v = extract.value;
        //     printf("%s\n", (const char*)v->toString().c_str());
        // }
    }
    else if(mode == "-g" || mode == "--generate") {
        // for(const auto& [id, type] : arg_refs) {
        //     z3::expr ex = getBsqTypeExpr(id, type, sol).value();
        //
        //     ValueExtractor extract(&asm_info, type, ex, sol);
        //     std::cout << extract.valueToSMTExpr() << "\n";
        // }
    }
    else if(mode == "-m" || mode == "--mock") {
        runMock(&asm_info, fn_json, sol);
    }
    else {
        std::string err = mode + " is not a valid <MODE>.";
        badArgs(err.c_str());
    }

    Z3_finalize_memory();
    return 0;
}

void runMock(bsqon::AssemblyInfo* asm_info, json mock_json, z3::solver& sol)
{
    for(auto& arg : mock_json["args"]) {
        std::string arg_id = arg["name"];
        std::string arg_type = arg["type"];

        if(sol.check() != z3::sat) {
            printf("Unsat .smt2 file\n");
            exit(1);
        }

        z3::model m = sol.get_model();
        uint consts = m.num_consts();
        assert(consts > 0);
        for(uint i = 0; i < consts; ++i) {
            z3::func_decl c = m.get_const_decl(i);
            if(c.name().str() == ("@arg-" + arg_id)) {
				ValueExtractor extract(asm_info, sol);

				bsqon::Type* c_type = asm_info->lookupTypeKey(arg_type);
				z3::expr c_ex = c();
				
				bsqon::Value* c_val = extract.extractValue(c_type, c_ex);
				printf("Type: %s\n", (const char*)c_type->tkey.c_str());
				printf("Value: %s\n", (const char*)c_val->toString().c_str());
            }
        }
    }
}
