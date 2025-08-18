#include "smt_extract.h"
#include <cassert>
#include <codecvt>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <map>
#include <optional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include <z3_api.h>

// Prepare Mock function, validate datatypes and paramaters with the arguments used in bsq.
// Right now the only way to see this is via the smt2 file.
BsqMock::BsqMock(bsqon::AssemblyInfo* asm_info, json mock_json, z3::solver& sol)
    : asm_info(asm_info), mock_json(mock_json), s(sol)
{
	std::unordered_map<std::string, z3::func_decl> fn_map = buildMockMap();
    this->fn_map = fn_map;

    std::string mock_name = tKeyToSmtName(mock_json["fname"], SMT_TYPE);
    if(fn_map.find(mock_name) == fn_map.end()){
		printf("%s, not present in .smt2 file.\n",mock_name.c_str());
		exit(1);
	}
	z3::func_decl mock_fn = fn_map.at(mock_name);

    std::map<std::string, std::pair<z3::expr, bsqon::Type*>> arg_map = getArgMap(mock_fn);

    z3::expr mock_ex = addArgsToMock(mock_fn, arg_map);

    z3::expr_vector arg_values(this->s.ctx());
    std::vector<bsqon::Value*> arg_bsq_values;

    uint ith_arg = 0;
	printf("| MOCK %s |\n",mock_name.c_str());
    printf("Arguments:\n");
    for(const auto& [id, arg_sig] : arg_map) {
        z3::expr arg_ex = mock_ex.arg(ith_arg++);

        ValueExtractor extract(asm_info, sol);

        bsqon::Type* arg_type = arg_sig.second;
        bsqon::Value* extracted_val = extract.extractValue(arg_type, arg_ex);
        if(extracted_val == nullptr) {
            printf("ERROR: NULL Extracted arg value | TYPE: %s \n",arg_type->tkey.c_str());
        }
        else {
			//TODO: Find a way to print the type tree.
			printf("Type: %s\n", (const char*)arg_type->tkey.c_str());
			printf("Value: %s\n", (const char*)extracted_val->toString().c_str());
        }

        arg_values.push_back(arg_ex);
        arg_bsq_values.push_back(extracted_val);
    }

    z3::expr mock_fn_result = mock_fn(arg_values);

    z3::sort return_sort = mock_fn.range();
    z3::symbol return_sym = this->s.ctx().str_symbol("$return");

    // TODO: Add Validator
    z3::expr return_ex = this->s.ctx().constant(return_sym, return_sort);
    this->s.add(return_ex == mock_fn_result);

    bsqon::Type* return_type = this->asm_info->lookupTypeKey(mock_json["return"]);

    ValueExtractor extract_return(asm_info, sol);
    bsqon::Value* return_value = extract_return.extractValue(return_type, return_ex);
    if(return_value == nullptr) {
        printf("ERROR: NULL Extracted $return value");
    }
    else {
        printf("Return:\n");
        printf("Type: %s\nValue: %s\n", (const char*)return_value->vtype->tkey.c_str(),
               (const char*)return_value->toString().c_str());
    }
}

std::map<std::string, std::pair<z3::expr, bsqon::Type*>> BsqMock::getArgMap(z3::func_decl mock_fn)
{
    std::map<std::string, std::pair<z3::expr, bsqon::Type*>> arg_map;

    // NOTE: Assumes z3 domain order is the same as json args order.
    uint ith = 0;
    for(const auto& arg : this->mock_json["args"]) {
        z3::sort arg_sort = mock_fn.domain(ith++);
        std::string arg_id = arg["name"];
        z3::symbol arg_sym = this->s.ctx().str_symbol(arg_id.c_str());
        z3::expr arg_ex = this->s.ctx().constant(arg_sym, arg_sort);

        bsqon::Type* arg_type = this->asm_info->lookupTypeKey(arg["type"]);

        arg_map.insert({arg_id, std::make_pair(arg_ex, arg_type)});
    }

    return arg_map;
}

z3::expr BsqMock::addArgsToMock(z3::func_decl mock, std::map<std::string, std::pair<z3::expr, bsqon::Type*>> arg_map)
{
    z3::expr_vector args(this->s.ctx());

    for(const auto& [id, arg_sig] : arg_map) {
        z3::expr arg_ex = arg_sig.first;
        bsqon::Type* arg_type = arg_sig.second;

        z3::func_decl validate_fn = findValidator(arg_type).value();
        this->s.add(validate_fn(arg_ex));

        args.push_back(arg_ex);
    }

    return mock(args);
}

std::optional<z3::func_decl> BsqMock::findValidator(bsqon::Type* t)
{
    if(this->s.check() != z3::sat) {
        printf("Got UNSAT when looking for validators.");
        exit(1);
    }
    z3::model m = this->s.get_model();

    std::string root_validator_name = "@ValidateRoot-" + tKeyToSmtName(t->tkey, SMT_TYPE);
    std::string validator_name = "@Validate-" + tKeyToSmtName(t->tkey, SMT_TYPE);

    if(auto it = this->fn_map.find(root_validator_name); it != this->fn_map.end()) {
        return it->second;
    }

    if(auto it = this->fn_map.find(validator_name); it != this->fn_map.end()) {
        return it->second;
    }

    return std::nullopt;
    ;
}

std::unordered_map<std::string, z3::func_decl> BsqMock::buildMockMap()
{
    std::unordered_map<std::string, z3::func_decl> fn_map;
    if(this->s.check() != z3::sat) {
        printf("Unsat .smt2 file\n");
        exit(1);
    }

    z3::model m = this->s.get_model();
    uint fns = m.num_funcs();
    assert(fns > 0);

    for(uint i = 0; i < fns; ++i) {
        z3::func_decl fn = m.get_func_decl(i);
        fn_map.insert({fn.name().str(), fn});
    }

    return fn_map;
};
