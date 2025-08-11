#include "smt_extract.h"
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <map>
#include <optional>
#include <string>
#include <utility>
#include <vector>
#include <z3_api.h>

// Prepare Mock function, validate datatypes and paramaters with the arguments used in bsq.
// Right now the only way to see this is via the smt2 file.
BsqMock::BsqMock(bsqon::AssemblyInfo* asm_info, json mock_json, z3::solver& sol)
    : asm_info(asm_info), mock_json(mock_json), s(sol)
{
    std::string mock_name = tKeyToSmtName(mock_json["fname"], SMT_TYPE);
    z3::func_decl mock_fn = getMockFromSMT(mock_name).value();

    z3::expr mock_ex = addArgsToMock(mock_fn).value();
    z3::expr_vector arg_values(this->s.ctx());

    std::vector<bsqon::Value*> arg_bsq_values;

    std::map<std::string, std::pair<z3::expr, bsqon::Type*>> arg_map = getArgMap(mock_fn);
    uint ith_arg = 0;
    for(const auto& [id, arg_sig] : arg_map) {
        z3::expr ex = arg_sig.first;

        this->s.add(ex == mock_ex.arg(ith_arg++));

        bsqon::Type* t = arg_sig.second;
        ValueExtractor extract(asm_info, id, t, ex, sol);
        arg_values.push_back(extract.ex);
        arg_bsq_values.push_back(extract.value);
    }

    z3::expr mock_fn_result = mock_fn(arg_values);

    z3::sort return_sort = mock_fn.range();
    z3::symbol return_sym = this->s.ctx().str_symbol("$return");

    // TODO: Add Validator
    z3::expr return_ex = this->s.ctx().constant(return_sym, return_sort);
    this->s.add(return_ex == mock_fn_result);

    bsqon::Type* return_type = this->asm_info->lookupTypeKey(mock_json["return"]);

    ValueExtractor extract_return(asm_info, return_sym.str(), return_type, return_ex, sol);

    std::cout << "Mocking " << mock_name << ":" << "\n";
    std::cout << "Arguments:\n";
    uint i = 0;
    for(bsqon::Value* v : arg_bsq_values) {
        const char* v_res = (const char*)v->toString().c_str();
        std::cout << "ARG " << i << " : " << v_res << "\n";
    }
    const char* result = (const char*)extract_return.value->toString().c_str();
    std::cout << "Result: " << result << "\n";
}

std::map<std::string, std::pair<z3::expr, bsqon::Type*>> BsqMock::getArgMap(z3::func_decl mock_fn)
{
    std::map<std::string, std::pair<z3::expr, bsqon::Type*>> arg_map;

    for(uint i = 0; i < mock_fn.arity(); ++i) {
        for(const auto& arg : this->mock_json["args"]) {
            bsqon::Type* type = this->asm_info->lookupTypeKey(arg["type"]);

            z3::sort arg_sort = mock_fn.domain(i);
            if(tKeyToSmtName(type->tkey, SMT_TYPE) == arg_sort.to_string()) {
                std::string arg_id = arg["name"];
                // std::string arg_str = "mock_" + arg_id;
                z3::symbol arg_sym = this->s.ctx().str_symbol(arg_id.c_str());
                z3::expr arg_ex = this->s.ctx().constant(arg_sym, arg_sort);

                arg_map.insert(std::make_pair(arg_id, std::make_pair(arg_ex, type)));
            }
        }
    }

    return arg_map;
}

std::optional<z3::expr> BsqMock::addArgsToMock(z3::func_decl mock)
{
    z3::expr_vector args(this->s.ctx());
    for(uint i = 0; i < mock.arity(); ++i) {
        z3::sort arg_sort = mock.domain(i);
        std::string arg_str = "mock_arg_" + std::to_string(i);
        z3::symbol arg_sym = this->s.ctx().str_symbol(arg_str.c_str());

        z3::expr arg_ex = this->s.ctx().constant(arg_sym, arg_sort);
        this->s.add(arg_ex == this->s.ctx().int_val(5));
        args.push_back(arg_ex);
    }

    return mock(args);
}

std::optional<z3::func_decl> BsqMock::getMockFromSMT(std::string mock_name)
{
    std::optional<z3::func_decl> fn_sig_opt;
    z3::model m = this->s.get_model();

    for(uint i = 0; i < m.num_funcs(); ++i) {
        z3::func_decl fn = m.get_func_decl(i);
        if(fn.name().str() == mock_name) {
            return fn;
        }
    }

    return std::nullopt;
}
