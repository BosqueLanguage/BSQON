
#pragma once

#include "../common.h"

namespace bsqon
{
    //
    //TODO: this is not very performant (easy to debug though) we should do a numeric ID map or intern to string pointers later
    //
    using TypeKey = std::string;

    enum class TypeTag
    {
        TYPE_UNRESOLVED = 0x0,
        TYPE_TUPLE,
        TYPE_RECORD,
        TYPE_STD_ENTITY,
        TYPE_STD_CONCEPT,
        TYPE_PRIMITIVE,
        TYPE_ENUM,
        TYPE_TYPE_DECL,
        TYPE_VALIDATOR_RE,
        TYPE_VALIDATOR_PTH,
        TYPE_STRING_OF,
        TYPE_ASCII_STRING_OF,
        TYPE_SOMETHING,
        TYPE_OPTION,
        TYPE_OK,
        TYPE_ERROR,
        TYPE_RESULT,
        TYPE_PATH,
        TYPE_PATH_FRAGMENT,
        TYPE_PATH_GLOB,
        TYPE_LIST,
        TYPE_STACK,
        TYPE_QUEUE,
        TYPE_SET,
        TYPE_MAP_ENTRY,
        TYPE_MAP,
        TYPE_CONCEPT_SET,
        TYPE_UNION
    };

    class TypeAnnotationInfo
    {
    public:
        bool isrecursive;
        std::string docstring;
        std::optional<std::vector<std::pair<TypeKey, std::string>>> sensitivetags;

        TypeAnnotationInfo(bool isrecursive, std::string docstring, std::optional<std::vector<std::pair<TypeKey, std::string>>> sensitivetags) : isrecursive(isrecursive), docstring(docstring), sensitivetags(sensitivetags) { ; }

        bool isSensitive() const {
            return this->sensitivetags.has_value();
        }

        static TypeAnnotationInfo parse(json j) {
            bool isrecursive = j.contains("isrecursive") && j["isrecursive"].is_boolean() && j["isrecursive"].get<bool>();
            std::string docstring = j.contains("docstring") && j["docstring"].is_string() ? j["docstring"].get<std::string>() : "";

            std::optional<std::vector<std::pair<TypeKey, std::string>>> sensitivetags = std::nullopt;
            if(j.contains("sensitive") && !j["sensitive"].is_null()) {
                std::vector<std::pair<TypeKey, std::string>> tags;
                std::transform(j["sensitive"].begin(), j["sensitive"].end(), std::back_inserter(tags), [](const json& jv) { 
                    return std::make_pair(jv["tkey"].get<TypeKey>(), jv["ename"].get<std::string>()); 
                });

                sensitivetags = std::make_optional(tags);
            }

            return TypeAnnotationInfo(isrecursive, docstring, sensitivetags);
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

        TypeAnnotationInfo annotations;

        Type(TypeTag tag, TypeKey tkey, TypeAnnotationInfo annotations) : tag(tag), tkey(tkey), annotations(annotations) { ; }
        virtual ~Type() = default;

        static const int64_t MIN_SAFE_INT = -9223372036854775807ll;
        static const int64_t MAX_SAFE_INT = 9223372036854775807ll;

        static const int64_t MAX_SAFE_NAT = 9223372036854775807ll;

        virtual const std::vector<TypeKey>* getPossibleSubtypeKeys() const
        {
            return nullptr;
        }

        static Type* parse(json j);

        bool isUnresolved() const
        {
            return this->tag == TypeTag::TYPE_UNRESOLVED;
        }

        bool isConcreteType() const
        {
            return !(this->tag == TypeTag::TYPE_UNRESOLVED 
                || this->tag == TypeTag::TYPE_OPTION || this->tag == TypeTag::TYPE_RESULT
                || this->tag == TypeTag::TYPE_STD_CONCEPT || this->tag == TypeTag::TYPE_CONCEPT_SET 
                || this->tag == TypeTag::TYPE_UNION);
        }
    };

    class UnresolvedType : public Type
    {
    public:
        UnresolvedType() : Type(TypeTag::TYPE_UNRESOLVED, "[UNRESOLVED]", {false, "[UNRESOLVED]", std::nullopt}) { ; }
        virtual ~UnresolvedType() = default;

        static UnresolvedType* singleton;
    };

    class TupleType : public Type
    {
    public:
        std::vector<TypeKey> entries;

        TupleType(TypeAnnotationInfo annotations, std::vector<TypeKey> entries) : Type(TypeTag::TYPE_TUPLE, "[" + std::accumulate(entries.begin(), entries.end(), std::string(), [](std::string&& a, TypeKey& b) { return (a == "" ? "" : std::move(a) + ", ") + b; }) + "]", annotations), entries(entries) { ; }
        virtual ~TupleType() = default;
    };

    class RecordTypeEntry
    {
    public:
        std::string pname;
        TypeKey ptype;

        RecordTypeEntry(std::string pname, TypeKey ptype) : pname(pname), ptype(ptype) { ; }
    };

    class RecordType : public Type
    {
    public:
        std::vector<RecordTypeEntry> entries;

        RecordType(TypeAnnotationInfo annotations, std::vector<RecordTypeEntry> entries) : Type(TypeTag::TYPE_RECORD, "{" + std::accumulate(entries.begin(), entries.end(), std::string(), [](std::string&& a, RecordTypeEntry& b) { return (a == "" ? "" : std::move(a) + ", ") + b.pname + ": " + b.ptype; }) + "}", annotations), entries(entries) { ; }
        virtual ~RecordType() = default;
    };

    class EntityType : public Type
    {
    public:
        EntityType(TypeTag tag, TypeKey tkey, TypeAnnotationInfo annotations) : Type(tag, tkey, annotations) { ; }
        virtual ~EntityType() = default;
    };

    class ConceptType : public Type
    {
    public:
        std::vector<TypeKey> subtypes;

        ConceptType(TypeTag tag, TypeKey tkey, std::vector<TypeKey> subtypes, TypeAnnotationInfo annotations) : Type(tag, tkey, annotations), subtypes(subtypes)
        {
            std::sort(this->subtypes.begin(), this->subtypes.end());
        }

        virtual ~ConceptType() = default;

        virtual const std::vector<TypeKey>* getPossibleSubtypeKeys() const override final
        {
            return &this->subtypes;
        }
    };

    class EntityTypeFieldEntry
    {
    public:
        std::string fname;
        TypeKey ftype;

        FieldAnnotationInfo annotations;

        EntityTypeFieldEntry(std::string fname, TypeKey ftype, FieldAnnotationInfo annotations) : fname(fname), ftype(ftype), annotations(annotations) { ; }
    };

    class StdEntityType : public EntityType
    {
    public:
        std::vector<EntityTypeFieldEntry> fields;
        bool hasvalidations;

        StdEntityType(TypeKey tkey, TypeAnnotationInfo annotations, std::vector<EntityTypeFieldEntry> fields, bool hasvalidations) : EntityType(TypeTag::TYPE_STD_ENTITY, tkey, annotations), fields(fields), hasvalidations(hasvalidations) { ; }
        virtual ~StdEntityType() = default;
    };

    class StdConceptType : public ConceptType
    {
    public:
        StdConceptType(TypeKey tkey, std::vector<TypeKey> subtypes, TypeAnnotationInfo annotations) : ConceptType(TypeTag::TYPE_STD_CONCEPT, tkey, subtypes, annotations) { ; }
        virtual ~StdConceptType() = default;
    };

    class PrimitiveType : public EntityType
    {
    public:
        PrimitiveType(TypeKey tkey, TypeAnnotationInfo annotations) : EntityType(TypeTag::TYPE_PRIMITIVE, tkey, annotations) { ; }
        virtual ~PrimitiveType() = default;
    };

    class EnumType : public EntityType
    {
    public:
        std::vector<std::string> variants;

        EnumType(TypeKey tkey, TypeAnnotationInfo annotations, std::vector<std::string> variants) : EntityType(TypeTag::TYPE_ENUM, tkey, annotations), variants(variants) { ; }
        virtual ~EnumType() = default;
    };

    class TypedeclType : public EntityType
    {
    public:
        TypeKey basetype;
        TypeKey oftype;

        std::optional<TypeKey> optStringOfValidator;
        std::optional<TypeKey> optPathOfValidator;
        bool hasvalidations;

        TypedeclType(TypeKey tkey, TypeAnnotationInfo annotations, TypeKey basetype, TypeKey oftype, std::optional<TypeKey> optStringOfValidator, std::optional<TypeKey> optPathOfValidator, bool hasvalidations) : EntityType(TypeTag::TYPE_TYPE_DECL, tkey, annotations), basetype(basetype), oftype(oftype), optStringOfValidator(optStringOfValidator), optPathOfValidator(optPathOfValidator), hasvalidations(hasvalidations) { ; }
        virtual ~TypedeclType() = default;
    };

    class ValidatorREType : public EntityType
    {
    public:
        ValidatorREType(TypeKey tkey, TypeAnnotationInfo annotations) : EntityType(TypeTag::TYPE_VALIDATOR_RE, tkey, annotations) { ; }
        virtual ~ValidatorREType() = default;
    };

    class ValidatorPthType : public EntityType
    {
    public: 
        ValidatorPthType(TypeKey tkey, TypeAnnotationInfo annotations) : EntityType(TypeTag::TYPE_VALIDATOR_PTH, tkey, annotations) { ; }
        virtual ~ValidatorPthType() = default;
    };

    class StringOfType : public EntityType
    {
    public:
        TypeKey oftype;

        StringOfType(TypeKey oftype, TypeAnnotationInfo annotations) : EntityType(TypeTag::TYPE_STRING_OF, "StringOf<" + oftype + ">", annotations), oftype(oftype) { ; }
        virtual ~StringOfType() = default;
    };

    class ASCIIStringOfType : public EntityType
    {
    public:
        TypeKey oftype;

        ASCIIStringOfType(TypeKey oftype, TypeAnnotationInfo annotations) : EntityType(TypeTag::TYPE_ASCII_STRING_OF, "ASCIIStringOf<" + oftype + ">", annotations), oftype(oftype) { ; }
        virtual ~ASCIIStringOfType() = default;
    };

    class SomethingType : public EntityType
    {
    public:
        TypeKey oftype;

        SomethingType(TypeKey oftype, TypeAnnotationInfo annotations) : EntityType(TypeTag::TYPE_SOMETHING, "Something<" + oftype + ">", annotations), oftype(oftype) { ; }
        virtual ~SomethingType() = default;
    };

    class OptionType : public ConceptType
    {
    public:
        TypeKey oftype;

        OptionType(TypeKey oftype, TypeAnnotationInfo annotations) : ConceptType(TypeTag::TYPE_OPTION, "Option<" + oftype + ">", { "Nothing", "Something<" + oftype + ">" }, annotations), oftype(oftype) { ; }
        virtual ~OptionType() = default;
    };

    class OkType : public EntityType
    {
    public:
        TypeKey ttype;
        TypeKey etype;

        OkType(TypeKey ttype, TypeKey etype, TypeAnnotationInfo annotations) : EntityType(TypeTag::TYPE_OK, "Result<" + ttype + ", " + etype + ">::Ok", annotations), ttype(ttype), etype(etype) { ; }
        virtual ~OkType() = default;
    };

    class ErrorType : public EntityType
    {
    public:
        TypeKey ttype;
        TypeKey etype;

        ErrorType(TypeKey ttype, TypeKey etype, TypeAnnotationInfo annotations) : EntityType(TypeTag::TYPE_ERROR, "Result<" + ttype + ", " + etype + ">::Err", annotations), ttype(ttype), etype(etype) { ; }
        virtual ~ErrorType() = default;
    };

    class ResultType : public ConceptType
    {
    public:
        TypeKey ttype;
        TypeKey etype;

        ResultType(TypeKey ttype, TypeKey etype, TypeAnnotationInfo annotations) : ConceptType(TypeTag::TYPE_RESULT, "Result<" + ttype + ", " + etype + ">", { "Result<" + ttype + ", " + etype + ">::Ok", "Result<" + ttype + ", " + etype + ">::Err" }, annotations), ttype(ttype), etype(etype) { ; }
        virtual ~ResultType() = default;
    };

    class PathType : public EntityType
    {
    public:
        TypeKey oftype;

        PathType(TypeKey oftype, TypeAnnotationInfo annotations) : EntityType(TypeTag::TYPE_PATH, "Path<" + oftype + ">", annotations), oftype(oftype) { ; }
        virtual ~PathType() = default;
    };

    class PathFragmentType : public EntityType
    {
    public:
        TypeKey oftype;

        PathFragmentType(TypeKey oftype, TypeAnnotationInfo annotations) : EntityType(TypeTag::TYPE_PATH_FRAGMENT, "PathFragment<" + oftype + ">", annotations), oftype(oftype) { ; }
        virtual ~PathFragmentType() = default;
    };

    class PathGlobType : public EntityType
    {
    public:
        TypeKey oftype;

        PathGlobType(TypeKey oftype, TypeAnnotationInfo annotations) : EntityType(TypeTag::TYPE_PATH_GLOB, "PathGlob<" + oftype + ">", annotations), oftype(oftype) { ; }
        virtual ~PathGlobType() = default;
    };

    class ListType : public EntityType
    {
    public:
        TypeKey oftype;

        ListType(TypeKey oftype, TypeAnnotationInfo annotations) : EntityType(TypeTag::TYPE_LIST, "List<" + oftype + ">", annotations), oftype(oftype) { ; }
        virtual ~ListType() = default;
    };

    class StackType : public EntityType
    {
    public:
        TypeKey oftype;

        StackType(TypeKey oftype, TypeAnnotationInfo annotations) : EntityType(TypeTag::TYPE_STACK, "Stack<" + oftype + ">", annotations), oftype(oftype) { ; }
        virtual ~StackType() = default;
    };

    class QueueType : public EntityType
    {
    public:
        TypeKey oftype;

        QueueType(TypeKey oftype, TypeAnnotationInfo annotations) : EntityType(TypeTag::TYPE_QUEUE, "Queue<" + oftype + ">", annotations), oftype(oftype) { ; }
        virtual ~QueueType() = default;
    };

    class SetType : public EntityType
    {
    public:
        TypeKey oftype;

        SetType(TypeKey oftype, TypeAnnotationInfo annotations) : EntityType(TypeTag::TYPE_SET, "Set<" + oftype + ">", annotations), oftype(oftype) { ; }
        virtual ~SetType() = default;
    };

    class MapEntryType : public EntityType
    {
    public:
        TypeKey ktype;
        TypeKey vtype;

        MapEntryType(TypeKey ktype, TypeKey vtype, TypeAnnotationInfo annotations) : EntityType(TypeTag::TYPE_MAP_ENTRY, "MapEntry<" + ktype + ", " + vtype + ">", annotations), ktype(ktype), vtype(vtype) { ; }
        virtual ~MapEntryType() = default;
    };

    class MapType : public EntityType
    {
    public:
        TypeKey ktype;
        TypeKey vtype;

        MapType(TypeKey ktype, TypeKey vtype, TypeAnnotationInfo annotations) : EntityType(TypeTag::TYPE_MAP, "Map<" + ktype + ", " + vtype + ">", annotations), ktype(ktype), vtype(vtype) { ; }
        virtual ~MapType() = default;
    };

    class ConceptSetType : public Type
    {
    public:
        std::vector<TypeKey> concepts;

        ConceptSetType(TypeAnnotationInfo annotations, std::vector<TypeKey> concepts) : Type(TypeTag::TYPE_CONCEPT_SET, std::accumulate(concepts.begin(), concepts.end(), std::string(), [](std::string&& a, TypeKey& b) { return (a == "" ? "" : std::move(a) + "&") + b; }), annotations), concepts(concepts) { ; }
        virtual ~ConceptSetType() = default;
    };

    class UnionType : public Type
    {
    public:
        std::vector<TypeKey> types;

        UnionType(TypeAnnotationInfo annotations, std::vector<TypeKey> types) : Type(TypeTag::TYPE_UNION, std::accumulate(types.begin(), types.end(), std::string(), [](std::string&& a, TypeKey& b) { return (a == "" ? "" : std::move(a) + " | ") + b; }), annotations), types(types) { ; }
        virtual ~UnionType() = default;
    };

    class NamespaceDecl
    {
    public:
        std::string ns;
        std::vector<TypeKey> typenames;

        NamespaceDecl(std::string ns, std::vector<TypeKey> typenames) : ns(ns), typenames(typenames)
        {
            ;
        }

        static NamespaceDecl* parse(json j);

        bool hasTypenameDecl(const std::string& name) const
        {
            return std::binary_search(this->typenames.cbegin(), this->typenames.cend(), name);
        }
    };

    class AssemblyInfo
    {
    public:
        std::map<TypeKey, Type*> aliasmap;
        std::map<std::string, NamespaceDecl*> namespaces;
        std::map<TypeKey, Type*> typerefs;
        std::vector<std::set<TypeKey>> recursiveSets;
        std::map<TypeKey, brex::Regex*> bsqonRegexValidators;
        std::map<TypeKey, bpath::PathGlob*> bsqonPathValidators;

        AssemblyInfo() : aliasmap(), namespaces(), typerefs(), recursiveSets(), bsqonRegexValidators(), bsqonPathValidators()
        { 
            ; 
        }

        ~AssemblyInfo()
        {
            //
            //Should only be one assembly when executing -- so just let it hang out and get collected on shutdown
            //
        }

        static void parse(json j, AssemblyInfo& assembly);

        Type* resolveType(TypeKey tkey)
        {
            auto tt = this->typerefs.find(tkey);
            if(tt != this->typerefs.end()) {
                return tt->second;
            }
            else {
                return UnresolvedType::singleton;
            }
        }

        const Type* resolveType(TypeKey tkey) const
        {
            auto tt = this->typerefs.find(tkey);
            if(tt != this->typerefs.end()) {
                return tt->second;
            }
            else {
                return UnresolvedType::singleton;
            }
        }

        bool checkConcreteSubtype(const Type* t, const Type* oftype) const
        {
            if (t->tkey == oftype->tkey) {
                return true;
            }

            if (oftype->tag == TypeTag::TYPE_UNION) {
                return std::any_of(static_cast<const UnionType*>(oftype)->types.begin(), static_cast<const UnionType*>(oftype)->types.end(), [this, t](const TypeKey& tt) { return this->checkConcreteSubtype(t, this->resolveType(tt)); });
            }
            else if (oftype->tag == TypeTag::TYPE_CONCEPT_SET) {
                return std::all_of(static_cast<const ConceptSetType*>(oftype)->concepts.begin(), static_cast<const ConceptSetType*>(oftype)->concepts.end(), [this, t](const TypeKey& tt) { return this->checkConcreteSubtype(t, this->resolveType(tt)); });
            }
            else {
                auto psubtypes = oftype->getPossibleSubtypeKeys();
                if(psubtypes == nullptr) {
                    return false;
                }
                else {
                    return std::binary_search(psubtypes->begin(), psubtypes->end(), t->tkey);
                }
            }
        }

        bool isKeyType(TypeKey tkey) const;
    };

    void loadAssembly(json j);

    bool isSubtype(TypeKey tkey, TypeKey oftype);
}
