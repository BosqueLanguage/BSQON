#pragma once

#include "../common.h"

#include "../info/type_info.h"
#include "../info/bsqon.h"

#include <random>

typedef std::string VCPath;

bool vcpathCMP(const VCPath& p1, const VCPath& p2);

VCPath pathAccessField(const VCPath& p, const std::string& f);
VCPath pathAccessIndex(const VCPath& p, size_t i);
VCPath pathAccessSpecial(const VCPath& p, const std::string& name);

class ValueConstraint
{
public:
    const VCPath path;

    ValueConstraint(VCPath path) : path(path) { ; }
    virtual ~ValueConstraint() = default;

    virtual std::u8string toString() const = 0;
};

class MinLengthConstraint : public ValueConstraint
{
public:
    const size_t minlen;

    MinLengthConstraint(VCPath path, size_t minlen) : ValueConstraint(path), minlen(minlen) { ; }

    virtual std::u8string toString() const override
    {
        auto sstr = std::to_string(this->minlen);
        return u8"|" + std::u8string(this->path.cbegin(), this->path.cend()) + u8"| >= " + std::u8string(sstr.cbegin(), sstr.cend());
    }
};

class OfTypeConstraint : public ValueConstraint
{
public:
    const bsqon::Type* vtype;

    OfTypeConstraint(VCPath path, const bsqon::Type* vtype) : ValueConstraint(path), vtype(vtype) { ; }

    virtual std::u8string toString() const override
    {
        return std::u8string(this->path.cbegin(), this->path.cend()) + u8"?" + std::u8string(this->vtype->tkey.cbegin(), this->vtype->tkey.cend());
    }
};

class FixedValueConstraint : public ValueConstraint
{
public:
    bsqon::Value* value;

    FixedValueConstraint(VCPath path, bsqon::Value* value) : ValueConstraint(path), value(value) { ; }

    virtual std::u8string toString() const override
    {
        return std::u8string(this->path.cbegin(), this->path.cend()) + u8" == " + this->value->toString();
    }
};

class GenerateContext
{
private:
    GenerateContext(const std::optional<const bsqon::Type*> intype, std::optional<bsqon::EntityTypeFieldEntry> forfield, std::optional<size_t> forindex, std::optional<std::u8string> forspecial, const std::optional<const bsqon::TypedeclType*> oftype, const bsqon::Type* valuetype) : intype(intype), forfield(forfield), forindex(forindex), forspecial(forspecial), oftype(oftype), valuetype(valuetype) { ; }

public:
    std::optional<const bsqon::Type*> intype;
    std::optional<bsqon::EntityTypeFieldEntry> forfield;
    std::optional<size_t> forindex;
    std::optional<std::u8string> forspecial;
    std::optional<const bsqon::TypedeclType*> oftype;
    const bsqon::Type* valuetype;

    GenerateContext() : intype(), forfield(), forindex(), forspecial(), oftype(), valuetype(nullptr) { ; }
    GenerateContext(const GenerateContext& other) : intype(other.intype), forfield(other.forfield), forindex(other.forindex), forspecial(other.forspecial), oftype(other.oftype), valuetype(other.valuetype) { ; }

    GenerateContext& operator=(const GenerateContext& other)
    {
        if(this == &other) {
            return *this;
        }

        this->intype = other.intype;
        this->forfield = other.forfield;
        this->forindex = other.forindex;
        this->forspecial = other.forspecial;
        this->oftype = other.oftype;
        this->valuetype = other.valuetype;
        return *this;
    }

    std::u8string toString() const
    {
        std::u8string its;
        if(this->intype.has_value()) {
            its = u8", type=" + std::u8string(this->intype.value()->tkey.cbegin(), this->intype.value()->tkey.cend());
        }

        std::u8string ffs;
        if(this->forfield.has_value()) {
            ffs = u8", field=" + std::u8string(this->forfield.value().fname.cbegin(), this->forfield.value().fname.cend());
        }
        if(this->forspecial.has_value()) {
            ffs = u8", special=" + this->forspecial.value();
        }
        if(this->forindex.has_value()) {
            auto sstr = std::to_string(this->forindex.value());
            ffs = u8", index=" + std::u8string(sstr.cbegin(), sstr.cend());
        }

        std::u8string ots;
        if(this->oftype.has_value()) {
            ots = u8", oftype=" + std::u8string(this->oftype.value()->tkey.cbegin(), this->oftype.value()->tkey.cend());
        }

        std::u8string vts = u8"valuetype=" + (this->valuetype == nullptr ? u8"[UNSET]" : std::u8string(this->valuetype->tkey.cbegin(), this->valuetype->tkey.cend()));

        return u8"{ " + vts + its + ffs + ots + u8" }";

    }

    GenerateContext extendWithEnclosingType(const bsqon::Type* t) const
    {
        return GenerateContext{std::make_optional(t), std::nullopt, std::nullopt, std::nullopt, std::nullopt, nullptr};
    }

    GenerateContext extendForField(const bsqon::EntityTypeFieldEntry& f) const
    {
        return GenerateContext{this->intype, std::make_optional(f), std::nullopt, std::nullopt, std::nullopt, nullptr};
    }

    GenerateContext extendForIndex(size_t i) const
    {
        return GenerateContext{this->intype, std::nullopt, std::make_optional(i), std::nullopt, std::nullopt, nullptr};
    }

    GenerateContext extendForSpecial(std::u8string special) const
    {
        return GenerateContext{this->intype, std::nullopt, std::nullopt, std::make_optional(special), std::nullopt, nullptr};
    }

    GenerateContext extendForTypedecl(const bsqon::TypedeclType* t) const
    {
        return GenerateContext{this->intype, this->forfield, this->forindex, std::nullopt, std::make_optional(t), nullptr};
    }

    GenerateContext completeWithValueType(const bsqon::Type* t) const
    {
        return GenerateContext{this->intype, this->forfield, this->forindex, this->forspecial, this->oftype, t};
    }
};

class ValueComponent
{
public:
    const VCPath path;
    const std::vector<ValueConstraint*> constraints;

    const GenerateContext context;
    std::vector<bsqon::Value*> options;

    ValueComponent(const VCPath path, const std::vector<ValueConstraint*>& constraints, const GenerateContext& context)
        : path(path), constraints(constraints), context(context) { }
    ValueComponent(const VCPath path, const std::vector<ValueConstraint*>& constraints, const GenerateContext& context, std::vector<bsqon::Value*> options)
        : path(path), constraints(constraints), context(context), options(std::move(options)) { }

    std::u8string toString() const
    {
        auto pathstr = std::u8string(this->path.cbegin(), this->path.cend());

        auto cstr = std::accumulate(this->constraints.cbegin(), this->constraints.cend(), std::u8string{}, [](std::u8string&& a, const ValueConstraint* v) { 
            return (a.empty() ? u8"" : std::move(a) + u8", ") + v->toString();
        });

        auto ctx = this->context.toString();

        auto ostr = std::accumulate(this->options.cbegin(), this->options.cend(), std::u8string{}, [](std::u8string&& a, const bsqon::Value* v) { 
            return (a.empty() ? u8"" : std::move(a) + u8", ") + v->toString(); 
        });

        auto withstr = cstr.empty() ? u8"" : u8"\n    with { " + ostr + u8" }";
        auto ctxstr = ctx.empty() ? u8"" : u8" in context" + ctx;
        
        return pathstr + u8" => { " + ostr + u8" }" + ctxstr + withstr;
    }
};

class ValueSetGeneratorEnvironment
{
public:
    VCPath path;
    std::vector<ValueConstraint*> constraints;
    GenerateContext context;

    ValueSetGeneratorEnvironment(const VCPath& path, const std::vector<ValueConstraint*>& constraints, const GenerateContext& context) : path(path), constraints(constraints), context(context) { ; }
    ValueSetGeneratorEnvironment(const ValueSetGeneratorEnvironment& other) : path(other.path), constraints(other.constraints), context(other.context) { ; }  

    ValueSetGeneratorEnvironment& operator=(const ValueSetGeneratorEnvironment& other)
    {
        if(this == &other) {
            return *this;
        }

        this->path = other.path;
        this->constraints = other.constraints;
        this->context = other.context;
        return *this;
    }

    ValueSetGeneratorEnvironment step(const std::string& npath, const std::vector<ValueConstraint*>& newconstraints, const GenerateContext& updatedcontext) const
    {
        std::vector<ValueConstraint*> nconstraints(this->constraints);
        nconstraints.insert(nconstraints.end(), newconstraints.cbegin(), newconstraints.cend());
        std::stable_sort(nconstraints.begin(), nconstraints.end(), [](const ValueConstraint* v1, const ValueConstraint* v2) { return vcpathCMP(v1->path, v2->path); });

        return ValueSetGeneratorEnvironment{npath, nconstraints, updatedcontext};
    }
};

class ValueSetPartition
{
public:
    std::vector<ValueComponent*> components;

    ValueSetPartition() : components() { ; }
    ValueSetPartition(const std::vector<ValueComponent*>& components) : components(components) { ; }

    ValueSetPartition(const ValueSetPartition& other) : components(other.components) { ; }
    ValueSetPartition(const ValueSetPartition&& other) : components(std::move(other.components)) { ; }

    ValueSetPartition& operator=(const ValueSetPartition& other)
    {
        if(this == &other) {
            return *this;
        }

        this->components = other.components;
        return *this;
    }

    ValueSetPartition& operator=(const ValueSetPartition&& other)
    {
        if(this == &other) {
            return *this;
        }

        this->components = std::move(other.components);
        return *this;
    }

    std::u8string toString() const
    {
        auto cstr = std::accumulate(this->components.cbegin(), this->components.cend(), std::u8string{}, [](std::u8string&& a, const ValueComponent* v) { 
            return (a.empty() ? u8"" : std::move(a) + u8"\n") + v->toString();
        });

        return cstr;
    }

    static ValueSetPartition punion(const std::vector<ValueSetPartition>& partitions)
    {
        std::vector<ValueComponent*> components;
        for(size_t i = 0; i < partitions.size(); ++i) {
            components.insert(components.end(), partitions[i].components.cbegin(), partitions[i].components.cend());
        }
        std::stable_sort(components.begin(), components.end(), [](const ValueComponent* v1, const ValueComponent* v2) { return vcpathCMP(v1->path, v2->path); });

        return ValueSetPartition{std::move(components)};
    }
};

class ValueSetGenerator
{
public:
    const bsqon::AssemblyInfo* assembly;

    ValueSetGenerator(const bsqon::AssemblyInfo* assembly) : assembly(assembly) { ; }
    ~ValueSetGenerator() { ; }

    ValueSetPartition generateNone(const bsqon::PrimitiveType* t, const ValueSetGeneratorEnvironment& env);

    ValueSetPartition generateBool(const bsqon::PrimitiveType* t, const ValueSetGeneratorEnvironment& env);
    ValueSetPartition generateNat(const bsqon::PrimitiveType* t, const ValueSetGeneratorEnvironment& env);
    ValueSetPartition generateInt(const bsqon::PrimitiveType* t, const ValueSetGeneratorEnvironment& env);
    ValueSetPartition generateBigNat(const bsqon::PrimitiveType* t, const ValueSetGeneratorEnvironment& env);
    ValueSetPartition generateBigInt(const bsqon::PrimitiveType* t, const ValueSetGeneratorEnvironment& env);

    ValueSetPartition generateFloat(const bsqon::PrimitiveType* t, const ValueSetGeneratorEnvironment& env);

    ValueSetPartition generateString(const bsqon::PrimitiveType* t, const ValueSetGeneratorEnvironment& env);
    ValueSetPartition generateCString(const bsqon::PrimitiveType* t, const ValueSetGeneratorEnvironment& env);

    //TODO: more primitives..
    ValueSetPartition generatePrimitive(const bsqon::PrimitiveType* t, const ValueSetGeneratorEnvironment& env);
    ValueSetPartition generateEnum(const bsqon::EnumType* t, const ValueSetGeneratorEnvironment& env);

    ValueSetPartition generateList(const bsqon::ListType* t, const ValueSetGeneratorEnvironment& env);
    //More special types here...

    ValueSetPartition generateStdEntityType(const bsqon::StdEntityType* t, const ValueSetGeneratorEnvironment& env);
    ValueSetPartition generateTypeDeclType(const bsqon::TypedeclType* t, const ValueSetGeneratorEnvironment& env);

    ValueSetPartition generateStdConceptType(const bsqon::StdConceptType* t, const ValueSetGeneratorEnvironment& env);
    ValueSetPartition generateOption(const bsqon::OptionType* t, const ValueSetGeneratorEnvironment& env);
    ValueSetPartition generateSome(const bsqon::SomeType* t, const ValueSetGeneratorEnvironment& env);

    ValueSetPartition generateType(const bsqon::Type* t, const ValueSetGeneratorEnvironment& env);
};

class APISignature
{
public:
    std::string fname;
    std::vector<std::pair<std::string, const bsqon::Type*>> args;
    const bsqon::Type* rettype;

    std::vector<std::string> preconds;
    std::vector<std::string> postconds;

    APISignature(std::string fname, std::vector<std::pair<std::string, const bsqon::Type*>> args, const bsqon::Type* rettype, std::vector<std::string> preconds, std::vector<std::string> postconds) : fname(fname), args(args), rettype(rettype), preconds(preconds), postconds(postconds) { ; }
    ~APISignature() { ; }

    std::string toString() const
    {
        auto argstr = std::accumulate(this->args.cbegin(), this->args.cend(), std::string{}, [](std::string&& a, const std::pair<std::string, const bsqon::Type*>& v) { 
            return (a.empty() ? "" : std::move(a) + ", ") + v.first + ": " + v.second->tkey;
        });

        auto ssig = "api " + this->fname + "(" + argstr + ") : " + this->rettype->tkey;

        auto precondstr = std::accumulate(this->preconds.cbegin(), this->preconds.cend(), std::string{}, [](std::string&& a, const std::string& v) { 
            return (a.empty() ? "" : std::move(a) + "\n") + ("    requires " + v + ";");
        });
        auto postcondstr = std::accumulate(this->postconds.cbegin(), this->postconds.cend(), std::string{}, [](std::string&& a, const std::string& v) { 
            return (a.empty() ? "" : std::move(a) + ", ") + ("    ensures " + v + ";");
        });
        
        if(precondstr.empty() && postcondstr.empty()) {
            return ssig + ";";
        }
        else {
            if(!precondstr.empty()) {
                ssig += "\n" + precondstr;
            }
            if(!postcondstr.empty()) {
                ssig += "\n" + postcondstr;
            }

            return ssig + "\n;";
        }
    }

    static APISignature parse(const bsqon::AssemblyInfo& assembly, const json& j)
    {
        std::string fname = j["fname"].get<std::string>();

        std::vector<std::pair<std::string, const bsqon::Type*>> args;
        std::transform(j["args"].begin(), j["args"].end(), std::back_inserter(args), [&assembly](const json& jv) { 
            return std::make_pair(jv["name"].get<std::string>(), assembly.lookupTypeKey(jv["type"].get<std::string>()));
        });

        const bsqon::Type* rettype = assembly.lookupTypeKey(j["return"].get<std::string>());

        std::vector<std::string> preconds;
        if(j.contains("preconds") && !j["preconds"].is_null()) {
            std::transform(j["preconds"].begin(), j["preconds"].end(), std::back_inserter(preconds), [](const json& jv) { return jv.get<std::string>(); });
        }

        std::vector<std::string> postconds;
        if(j.contains("postconds") && !j["postconds"].is_null()) {
            std::transform(j["postconds"].begin(), j["postconds"].end(), std::back_inserter(postconds), [](const json& jv) { return jv.get<std::string>(); });
        }

        return APISignature(fname, args, rettype, preconds, postconds);
    }
};

class TestGenerator
{
private:
    bool isRequiredValue(const VCPath& currpath, bsqon::Value*& value);
    bool isConstrainedLengthValue(const VCPath& currpath, bsqon::Value*& value);
    bsqon::Value* selectFromPartition(const VCPath& currpath);

    const bsqon::Type* resolveSubtypeChoice(const VCPath& currpath, const bsqon::Type* t);

public:
    const bsqon::AssemblyInfo* assembly;
    const ValueSetPartition* vspartition;

    std::vector<const ValueConstraint*> constraints;
    std::mt19937_64 rng;

    TestGenerator(const bsqon::AssemblyInfo* assembly, const ValueSetPartition* vspartition, const std::vector<const ValueConstraint*>& constraints) : assembly(assembly), vspartition(vspartition), constraints(constraints), rng(std::random_device{}()) { ; }
    ~TestGenerator() { ; }

    bsqon::Value* generateNone(const bsqon::PrimitiveType* t, VCPath currpath);

    bsqon::Value* generateBool(const bsqon::PrimitiveType* t, VCPath currpath);
    bsqon::Value* generateNat(const bsqon::PrimitiveType* t, VCPath currpath);
    bsqon::Value* generateInt(const bsqon::PrimitiveType* t, VCPath currpath);

    //TODO: more primitives..

    bsqon::Value* generatePrimitive(const bsqon::PrimitiveType* t, VCPath currpath);
    bsqon::Value* generateEnum(const bsqon::EnumType* t, VCPath currpath);

    bsqon::Value* generateList(const bsqon::ListType* t, VCPath currpath);

    //More special types here...

    bsqon::Value* generateStdEntityType(const bsqon::StdEntityType* t, VCPath currpath);
    bsqon::Value* generateTypeDeclType(const bsqon::TypedeclType* t, VCPath currpath);

    bsqon::Value* generateStdConceptType(const bsqon::StdConceptType* t, VCPath currpath);
    bsqon::Value* generateOption(const bsqon::OptionType* t, VCPath currpath);
    bsqon::Value* generateSome(const bsqon::SomeType* t, VCPath currpath);

    bsqon::Value* generateType(const bsqon::Type* t, VCPath currpath);

    static bool checkConstraintSatisfiability(const std::vector<const ValueConstraint*>& constraints);
};
