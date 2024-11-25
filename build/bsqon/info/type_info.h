
#pragma once

#include "../common.h"

#include "../../build/include/brex/regex/brex_system.h"

namespace bsqon
{
    //
    //TODO: this is not very performant (easy to debug though) we should do a numeric ID map or intern to string pointers later
    //
    using TypeKey = std::string;

    using FullyQualifiedNamespace = std::vector<std::string>;

    enum class TypeTag
    {
        TYPE_UNRESOLVED = 0x0,
        TYPE_ELIST,
        TYPE_STD_ENTITY,
        TYPE_STD_CONCEPT,
        TYPE_PRIMITIVE,
        TYPE_ENUM,
        TYPE_TYPE_DECL,
        TYPE_SOME,
        TYPE_OPTION,
        TYPE_OK,
        TYPE_ERROR,
        TYPE_RESULT,
        TYPE_APIREJECTED,
        TYPE_APIFAILED,
        TYPE_APIERROR,
        TYPE_APISUCCESS,
        TYPE_APIRESULT,
        TYPE_LIST,
        TYPE_STACK,
        TYPE_QUEUE,
        TYPE_SET,
        TYPE_MAP_ENTRY,
        TYPE_MAP
    };

    class TypeAnnotationInfo
    {
    public:
        std::string docstring;
        std::optional<std::vector<std::pair<TypeKey, std::string>>> sensitivetags;

        TypeAnnotationInfo(std::string docstring, std::optional<std::vector<std::pair<TypeKey, std::string>>> sensitivetags) : docstring(docstring), sensitivetags(sensitivetags) { ; }

        bool isSensitive() const {
            return this->sensitivetags.has_value();
        }

        static TypeAnnotationInfo parse(json j) {
            std::string docstring = j.contains("docstring") && j["docstring"].is_string() ? j["docstring"].get<std::string>() : "";

            std::optional<std::vector<std::pair<TypeKey, std::string>>> sensitivetags = std::nullopt;
            if(j.contains("sensitive") && !j["sensitive"].is_null()) {
                std::vector<std::pair<TypeKey, std::string>> tags;
                std::transform(j["sensitive"].begin(), j["sensitive"].end(), std::back_inserter(tags), [](const json& jv) { 
                    return std::make_pair(jv["tkey"].get<TypeKey>(), jv["ename"].get<std::string>()); 
                });

                sensitivetags = std::make_optional(tags);
            }

            return TypeAnnotationInfo(docstring, sensitivetags);
        }
    };

    class FieldAnnotationInfo
    {
    public:
        std::string docstring;
        std::optional<std::vector<std::pair<TypeKey, std::string>>> sensitivetags;

        FieldAnnotationInfo(std::string docstring, std::optional<std::vector<std::pair<TypeKey, std::string>>> sensitivetags) : docstring(docstring), sensitivetags(sensitivetags) { ; }

        bool isSensitive() const {
            return this->sensitivetags.has_value();
        }

        static FieldAnnotationInfo parse(json j) {
            std::string docstring = j.contains("docstring") && j["docstring"].is_string() ? j["docstring"].get<std::string>() : "";

            std::optional<std::vector<std::pair<TypeKey, std::string>>> sensitivetags = std::nullopt;
            if(j.contains("sensitive") && !j["sensitive"].is_null()) {
                std::vector<std::pair<TypeKey, std::string>> tags;
                std::transform(j["sensitive"].begin(), j["sensitive"].end(), std::back_inserter(tags), [](const json& jv) { 
                    return std::make_pair(jv["tkey"].get<TypeKey>(), jv["ename"].get<std::string>()); 
                });

                sensitivetags = std::make_optional(tags);
            }

            return FieldAnnotationInfo(docstring, sensitivetags);
        }
    };

    class Type
    {
    public:
        TypeTag tag;
        TypeKey tkey;

        std::vector<TypeKey> supertypes;

        TypeAnnotationInfo annotations;

        Type(TypeTag tag, TypeKey tkey, std::vector<TypeKey> supertypes, TypeAnnotationInfo annotations) : tag(tag), tkey(tkey), supertypes(supertypes), annotations(annotations) 
        {
            std::sort(this->supertypes.begin(), this->supertypes.end());
        }

        virtual ~Type() = default;

        static const int64_t MIN_SAFE_INT = -9223372036854775807ll;
        static const int64_t MAX_SAFE_INT = 9223372036854775807ll;

        static const int64_t MAX_SAFE_NAT = 9223372036854775807ll;

        static Type* parse(json j);

        bool isUnresolved() const
        {
            return this->tag == TypeTag::TYPE_UNRESOLVED;
        }

        bool isConcreteType() const
        {
            return !(this->tag == TypeTag::TYPE_UNRESOLVED 
                || this->tag == TypeTag::TYPE_OPTION || this->tag == TypeTag::TYPE_RESULT
                || this->tag == TypeTag::TYPE_STD_CONCEPT || this->tag == TypeTag::TYPE_APIRESULT);
        }
    };

    class UnresolvedType : public Type
    {
    public:
        UnresolvedType() : Type(TypeTag::TYPE_UNRESOLVED, "[UNRESOLVED]", {}, {"[UNRESOLVED]", std::nullopt}) { ; }
        virtual ~UnresolvedType() = default;

        static UnresolvedType* singleton;
    };

    class EListType : public Type
    {
    public:
        std::vector<TypeKey> entries;

        EListType(std::vector<TypeKey> entries) : Type(TypeTag::TYPE_ELIST, "(|" + std::accumulate(entries.begin(), entries.end(), std::string(), [](std::string&& a, TypeKey& b) { return (a == "" ? "" : std::move(a) + ", ") + b; }) + "|)", {}, {"", {}}), entries(entries) { ; }
        virtual ~EListType() = default;
    };

    class EntityType : public Type
    {
    public:
        EntityType(TypeTag tag, TypeKey tkey, std::vector<TypeKey> supertypes, TypeAnnotationInfo annotations) : Type(tag, tkey, supertypes, annotations) { ; }
        virtual ~EntityType() = default;
    };

    class ConceptType : public Type
    {
    public:
        ConceptType(TypeTag tag, TypeKey tkey, std::vector<TypeKey> supertypes, TypeAnnotationInfo annotations) : Type(tag, tkey, supertypes, annotations) { ; }
        virtual ~ConceptType() = default;
    };

    class EntityTypeFieldEntry
    {
    public:
        std::string fname;
        TypeKey ftype;
        bool isOptional;

        FieldAnnotationInfo annotations;

        EntityTypeFieldEntry(std::string fname, TypeKey ftype, bool isOptional, FieldAnnotationInfo annotations) : fname(fname), ftype(ftype), isOptional(isOptional), annotations(annotations) { ; }
    };

    class StdEntityType : public EntityType
    {
    public:
        std::vector<EntityTypeFieldEntry> fields;
        bool hasvalidations;

        StdEntityType(TypeKey tkey, std::vector<TypeKey> supertypes, TypeAnnotationInfo annotations, std::vector<EntityTypeFieldEntry> fields, bool hasvalidations) : EntityType(TypeTag::TYPE_STD_ENTITY, tkey, supertypes, annotations), fields(fields), hasvalidations(hasvalidations) { ; }
        virtual ~StdEntityType() = default;
    };

    class StdConceptType : public ConceptType
    {
    public:
        StdConceptType(TypeKey tkey, std::vector<TypeKey> supertypes, TypeAnnotationInfo annotations) : ConceptType(TypeTag::TYPE_STD_CONCEPT, tkey, supertypes, annotations) { ; }
        virtual ~StdConceptType() = default;
    };

    class PrimitiveType : public EntityType
    {
    public:
        PrimitiveType(TypeKey tkey, std::vector<TypeKey> supertypes, TypeAnnotationInfo annotations) : EntityType(TypeTag::TYPE_PRIMITIVE, tkey, supertypes, annotations) { ; }
        virtual ~PrimitiveType() = default;
    };

    class EnumType : public EntityType
    {
    public:
        std::vector<std::string> variants;

        EnumType(TypeKey tkey, std::vector<TypeKey> supertypes, TypeAnnotationInfo annotations, std::vector<std::string> variants) : EntityType(TypeTag::TYPE_ENUM, tkey, supertypes, annotations), variants(variants) { ; }
        virtual ~EnumType() = default;
    };

    class TypedeclType : public EntityType
    {
    public:
        TypeKey primitivetype;

        std::optional<std::pair<std::u8string, std::string>> optOfValidator;
        bool hasvalidations;

        TypedeclType(TypeKey tkey, std::vector<TypeKey> supertypes, TypeAnnotationInfo annotations, TypeKey primitivetype, std::optional<std::pair<std::u8string, std::string>> optOfValidator, bool hasvalidations) : EntityType(TypeTag::TYPE_TYPE_DECL, tkey, supertypes, annotations), primitivetype(primitivetype), optOfValidator(optOfValidator), hasvalidations(hasvalidations) { ; }
        virtual ~TypedeclType() = default;
    };

    class SomeType : public EntityType
    {
    public:
        TypeKey oftype;

        SomeType(TypeKey oftype, std::vector<TypeKey> supertypes, TypeAnnotationInfo annotations) : EntityType(TypeTag::TYPE_SOME, "Some<" + oftype + ">", supertypes, annotations), oftype(oftype) { ; }
        virtual ~SomeType() = default;
    };

    class OptionType : public ConceptType
    {
    public:
        TypeKey oftype;

        OptionType(TypeKey oftype, std::vector<TypeKey> supertypes, TypeAnnotationInfo annotations) : ConceptType(TypeTag::TYPE_OPTION, "Option<" + oftype + ">", supertypes, annotations), oftype(oftype) { ; }
        virtual ~OptionType() = default;
    };

    class OkType : public EntityType
    {
    public:
        TypeKey ttype;
        TypeKey etype;

        OkType(TypeKey ttype, TypeKey etype, std::vector<TypeKey> supertypes, TypeAnnotationInfo annotations) : EntityType(TypeTag::TYPE_OK, "Result<" + ttype + ", " + etype + ">::Ok", supertypes, annotations), ttype(ttype), etype(etype) { ; }
        virtual ~OkType() = default;
    };

    class ErrorType : public EntityType
    {
    public:
        TypeKey ttype;
        TypeKey etype;

        ErrorType(TypeKey ttype, TypeKey etype, std::vector<TypeKey> supertypes, TypeAnnotationInfo annotations) : EntityType(TypeTag::TYPE_ERROR, "Result<" + ttype + ", " + etype + ">::Err", supertypes, annotations), ttype(ttype), etype(etype) { ; }
        virtual ~ErrorType() = default;
    };

    class ResultType : public ConceptType
    {
    public:
        TypeKey ttype;
        TypeKey etype;

        ResultType(TypeKey ttype, TypeKey etype, std::vector<TypeKey> supertypes, TypeAnnotationInfo annotations) : ConceptType(TypeTag::TYPE_RESULT, "Result<" + ttype + ", " + etype + ">", supertypes, annotations), ttype(ttype), etype(etype) { ; }
        virtual ~ResultType() = default;
    };

    class APIRejectedType : public EntityType
    {
    public:
        TypeKey ttype;

        APIRejectedType(TypeKey ttype, std::vector<TypeKey> supertypes, TypeAnnotationInfo annotations) : EntityType(TypeTag::TYPE_APIREJECTED, "APIResult<" + ttype + ">::Rejected", supertypes, annotations), ttype(ttype) { ; }
        virtual ~APIRejectedType() = default;
    };

    class APIFailedType : public EntityType
    {
    public:
        TypeKey ttype;

        APIFailedType(TypeKey ttype, std::vector<TypeKey> supertypes, TypeAnnotationInfo annotations) : EntityType(TypeTag::TYPE_APIFAILED, "APIResult<" + ttype + ">::Failed", supertypes, annotations), ttype(ttype) { ; }
        virtual ~APIFailedType() = default;
    };

    class APIErrorType : public EntityType
    {
    public:
        TypeKey ttype;

        APIErrorType(TypeKey ttype, std::vector<TypeKey> supertypes, TypeAnnotationInfo annotations) : EntityType(TypeTag::TYPE_APIERROR, "APIResult<" + ttype + ">::Error", supertypes, annotations), ttype(ttype) { ; }
        virtual ~APIErrorType() = default;
    };

    class APISuccessType : public EntityType
    {
    public:
        TypeKey ttype;

        APISuccessType(TypeKey ttype, std::vector<TypeKey> supertypes, TypeAnnotationInfo annotations) : EntityType(TypeTag::TYPE_APISUCCESS, "APIResult<" + ttype + ">::Success", supertypes, annotations), ttype(ttype) { ; }
        virtual ~APISuccessType() = default;
    };

    class APIResultType : public ConceptType
    {
    public:
        TypeKey ttype;

        APIResultType(TypeKey ttype, std::vector<TypeKey> supertypes, TypeAnnotationInfo annotations) : ConceptType(TypeTag::TYPE_APIRESULT, "APIResult<" + ttype + ">", supertypes, annotations), ttype(ttype) { ; }
        virtual ~APIResultType() = default;
    };

    class ListType : public EntityType
    {
    public:
        TypeKey oftype;

        ListType(TypeKey oftype, std::vector<TypeKey> supertypes, TypeAnnotationInfo annotations) : EntityType(TypeTag::TYPE_LIST, "List<" + oftype + ">", supertypes, annotations), oftype(oftype) { ; }
        virtual ~ListType() = default;
    };

    class StackType : public EntityType
    {
    public:
        TypeKey oftype;

        StackType(TypeKey oftype, std::vector<TypeKey> supertypes, TypeAnnotationInfo annotations) : EntityType(TypeTag::TYPE_STACK, "Stack<" + oftype + ">", supertypes, annotations), oftype(oftype) { ; }
        virtual ~StackType() = default;
    };

    class QueueType : public EntityType
    {
    public:
        TypeKey oftype;

        QueueType(TypeKey oftype, std::vector<TypeKey> supertypes, TypeAnnotationInfo annotations) : EntityType(TypeTag::TYPE_QUEUE, "Queue<" + oftype + ">", supertypes, annotations), oftype(oftype) { ; }
        virtual ~QueueType() = default;
    };

    class SetType : public EntityType
    {
    public:
        TypeKey oftype;

        SetType(TypeKey oftype, std::vector<TypeKey> supertypes, TypeAnnotationInfo annotations) : EntityType(TypeTag::TYPE_SET, "Set<" + oftype + ">", supertypes, annotations), oftype(oftype) { ; }
        virtual ~SetType() = default;
    };

    class MapEntryType : public EntityType
    {
    public:
        TypeKey ktype;
        TypeKey vtype;

        MapEntryType(TypeKey ktype, TypeKey vtype, std::vector<TypeKey> supertypes, TypeAnnotationInfo annotations) : EntityType(TypeTag::TYPE_MAP_ENTRY, "MapEntry<" + ktype + ", " + vtype + ">", supertypes, annotations), ktype(ktype), vtype(vtype) { ; }
        virtual ~MapEntryType() = default;
    };

    class MapType : public EntityType
    {
    public:
        TypeKey ktype;
        TypeKey vtype;

        MapType(TypeKey ktype, TypeKey vtype, std::vector<TypeKey> supertypes, TypeAnnotationInfo annotations) : EntityType(TypeTag::TYPE_MAP, "Map<" + ktype + ", " + vtype + ">", supertypes, annotations), ktype(ktype), vtype(vtype) { ; }
        virtual ~MapType() = default;
    };

    class NamespaceDecl
    {
    public:
        bool istoplevel;
        std::map<std::string, std::string> imports; //copy over if not top-level

        FullyQualifiedNamespace fullns;
        std::string ns;

        std::map<std::string, FullyQualifiedNamespace> subns;
        std::map<std::string, TypeKey> types;

        NamespaceDecl(bool istoplevel, std::map<std::string, std::string> imports, FullyQualifiedNamespace fullns, std::string ns, std::map<std::string, FullyQualifiedNamespace> subns, std::map<std::string, TypeKey> types) : istoplevel(istoplevel), imports(imports), fullns(fullns), ns(ns), subns(subns), types(types) { ; }
        static NamespaceDecl* parse(json j);

        bool lookupNSReference(const std::string& name, std::string& out) const
        {
            auto ii = this->imports.find(name);
            if(ii == this->imports.end()) {
                return false;
            }
            else {
                out = ii->second;
                return true;
            }
        }
    };

    class AssemblyInfo
    {
    public:
        std::map<std::string, NamespaceDecl*> namespaces;
        std::map<TypeKey, Type*> typerefs;

        std::vector<std::set<TypeKey>> recursiveSets;

        //maps from literal regex representations to their executable forms
        std::map<std::u8string, brex::UnicodeRegexExecutor*> executableUnicodeRegexMap;
        std::map<std::u8string, brex::CRegexExecutor*> executableCRegexMap;

        //maps from namespace const named Foo::bar regex representations to their named regex opt forms
        std::map<std::string, const brex::RegexOpt*> namedUnicodeRegexMap;
        std::map<std::string, const brex::RegexOpt*> namedCRegexMap;
        brex::ReNSRemapper nsremapper;

        AssemblyInfo() : namespaces(), typerefs(), recursiveSets(), executableUnicodeRegexMap(), executableCRegexMap(), namedUnicodeRegexMap(), namedCRegexMap(), nsremapper()
        { 
            ; 
        }

        ~AssemblyInfo()
        {
            //
            //Should only be one assembly when executing -- so just let it hang out and get collected on shutdown
            //
        }

        static void parseRESystem(json j, AssemblyInfo& assembly);
        static void parse(json j, AssemblyInfo& assembly);

        Type* lookupTypeKey(TypeKey tkey)
        {
            auto tt = this->typerefs.find(tkey);
            if(tt != this->typerefs.end()) {
                return tt->second;
            }
            else {
                return UnresolvedType::singleton;
            }
        }

        const Type* lookupTypeKey(TypeKey tkey) const
        {
            auto tt = this->typerefs.find(tkey);
            if(tt != this->typerefs.end()) {
                return tt->second;
            }
            else {
                return UnresolvedType::singleton;
            }
        }

        bool checkSubtype(const Type* t, const Type* oftype) const
        {
            if (t->tkey == oftype->tkey) {
                return true;
            }
            else {
                return std::binary_search(t->supertypes.cbegin(), t->supertypes.cend(), oftype->tkey);
            }
        }

        std::u8string resolveConstantRegexValue(const std::u8string& reexp, bool isutf) const;

        void processUnicodeRegex(const std::string& inns, const std::u8string& regex);
        void processCRegex(const std::string& inns, const std::u8string& regex);

        bool validateString(const std::u8string& regex, brex::UnicodeString* ustr, const std::string& inns);
        bool validateCString(const std::u8string& regex, brex::CString* cstr, const std::string& inns);

        bool isKeyType(TypeKey tkey) const;
    };

    void loadAssembly(json j);

    bool isSubtype(TypeKey tkey, TypeKey oftype);
}
