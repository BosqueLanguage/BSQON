
#include "type_info.h"

namespace bsqon 
{
    static AssemblyInfo g_assembly;

    TypeTag convertTagNameToEnum(const std::string& tname)
    {
        TypeTag ttag = TypeTag::TYPE_UNRESOLVED;

        if(tname == "[...]") {
            ttag = TypeTag::TYPE_TUPLE;
        }
        else if(tname == "{...}") {
            ttag = TypeTag::TYPE_RECORD;
        }
        else if(tname == "StdEntity") {
            ttag = TypeTag::TYPE_STD_ENTITY;
        }
        else if(tname == "StdConcept") {
            ttag = TypeTag::TYPE_STD_CONCEPT;
        }
        else if(tname == "PrimitiveEntity") {
            ttag = TypeTag::TYPE_PRIMITIVE;
        }
        else if(tname == "EnumEntity") {
            ttag = TypeTag::TYPE_ENUM;
        }
        else if(tname == "TypeDecl") {
            ttag = TypeTag::TYPE_TYPE_DECL;
        }
        else if(tname == "ValidatorRE") {
            ttag = TypeTag::TYPE_VALIDATOR_RE;
        }
        else if(tname == "ValidatorPth") {
            ttag = TypeTag::TYPE_VALIDATOR_PTH;
        }
        else if(tname == "StringOf") {
            ttag = TypeTag::TYPE_STRING_OF;
        }
        else if(tname == "AsciiStringOf") {
            ttag = TypeTag::TYPE_ASCII_STRING_OF;
        }
        else if(tname == "Something") {
            ttag = TypeTag::TYPE_SOMETHING;
        }
        else if(tname == "Option") {
            ttag = TypeTag::TYPE_OPTION;
        }
        else if(tname == "Result::Ok") {
            ttag = TypeTag::TYPE_OK;
        }
        else if(tname == "Result::Err") {
            ttag = TypeTag::TYPE_ERROR;
        }
        else if(tname == "Result") {
            ttag = TypeTag::TYPE_RESULT;
        }
        else if(tname == "Path") {
            ttag = TypeTag::TYPE_PATH;
        }
        else if(tname == "PathFragment") {
            ttag = TypeTag::TYPE_PATH_FRAGMENT;
        }
        else if(tname == "PathGlob") {
            ttag = TypeTag::TYPE_PATH_GLOB;
        }
        else if(tname == "List") {
            ttag = TypeTag::TYPE_LIST;
        }
        else if(tname == "Stack") {
            ttag = TypeTag::TYPE_STACK;
        }
        else if(tname == "Queue") {
            ttag = TypeTag::TYPE_QUEUE;
        }
        else if(tname == "Set") {
            ttag = TypeTag::TYPE_SET;
        }
        else if(tname == "MapEntry") {
            ttag = TypeTag::TYPE_MAP_ENTRY;
        }
        else if(tname == "Map") {
            ttag = TypeTag::TYPE_MAP;
        }
        else if(tname == "ConceptSet") {
            ttag = TypeTag::TYPE_CONCEPT_SET;
        }
        else if(tname == "UnionType") {
            ttag = TypeTag::TYPE_UNION;
        }
        else {
            //Missing tag
            assert(false);
        }

        return ttag;
    }

    Type* Type::parse(json j)
    {
        std::string ttag = j["tag"].get<std::string>();
        TypeTag tt = convertTagNameToEnum(ttag);

        auto annotations = j.contains("annotations") && !j["annotations"].is_null() ? TypeAnnotationInfo::parse(j["annotations"]) : TypeAnnotationInfo(false, "", std::nullopt);

        switch(tt) {
            case TypeTag::TYPE_TUPLE: {
                std::vector<TypeKey> entries;
                std::transform(j["entries"].begin(), j["entries"].end(), std::back_inserter(entries), [](const json& jv) { return jv.get<TypeKey>(); });
                return new TupleType(annotations, entries);
            }
            case TypeTag::TYPE_RECORD: {
                std::vector<RecordTypeEntry> entries;
                std::transform(j["entries"].begin(), j["entries"].end(), std::back_inserter(entries), [](const json& jv) { 
                    return RecordTypeEntry(jv["pname"].get<std::string>(), jv["ptype"].get<TypeKey>()); 
                });
                return new RecordType(annotations, entries);
            }
            case TypeTag::TYPE_STD_ENTITY: {
                std::vector<EntityTypeFieldEntry> fields;
                std::transform(j["fields"].begin(), j["fields"].end(), std::back_inserter(fields), [](const json& jv) {
                    FieldAnnotationInfo fsannotation("", std::nullopt);
                    if(jv.contains("fsannotation") && !jv["ssannotation"].is_null()) {
                        fsannotation = FieldAnnotationInfo::parse(jv["fsannotation"]);
                    }

                    return EntityTypeFieldEntry(jv["fname"].get<std::string>(), jv["ftype"].get<TypeKey>(), fsannotation); 
                });
                bool hasvalidations = j["hasvalidations"].get<bool>();
                return new StdEntityType(j["tkey"].get<TypeKey>(), annotations, fields, hasvalidations);
            }
            case TypeTag::TYPE_STD_CONCEPT: {
                std::vector<TypeKey> subtypes;
                std::transform(j["subtypes"].begin(), j["subtypes"].end(), std::back_inserter(subtypes), [](const json& jv) { return jv.get<TypeKey>(); });
                return new StdConceptType(j["tkey"].get<TypeKey>(), subtypes, annotations);
            }
            case TypeTag::TYPE_PRIMITIVE: {
                return new PrimitiveType(j["tkey"].get<TypeKey>(), annotations);
            }
            case TypeTag::TYPE_ENUM: {
                std::vector<std::string> variants;
                std::transform(j["variants"].begin(), j["variants"].end(), std::back_inserter(variants), [](const json& jv) { return jv.get<std::string>(); });
                return new EnumType(j["tkey"].get<TypeKey>(), annotations, variants);
            }
            case TypeTag::TYPE_TYPE_DECL: {
                std::optional<TypeKey> optStringOfValidator = !j["optStringOfValidator"].is_null() ? std::make_optional(j["optStringOfValidator"].get<std::string>()) : std::nullopt;
                std::optional<TypeKey> optPathOfValidator = !j["optPathOfValidator"].is_null() ? std::make_optional(j["optPathOfValidator"].get<std::string>()) : std::nullopt;
                bool hasvalidations = j["hasvalidations"].get<bool>();
                return new TypedeclType(j["tkey"].get<TypeKey>(), annotations, j["basetype"].get<TypeKey>(), j["oftype"].get<TypeKey>(), optStringOfValidator, optPathOfValidator, hasvalidations);
            }
            case TypeTag::TYPE_VALIDATOR_RE: {
                return new ValidatorREType(j["tkey"].get<TypeKey>(), annotations);
            }
            case TypeTag::TYPE_VALIDATOR_PTH: {
                return new ValidatorPthType(j["tkey"].get<TypeKey>(), annotations);
            }
            case TypeTag::TYPE_STRING_OF: {
                return new StringOfType(j["oftype"].get<TypeKey>(), annotations);
            }
            case TypeTag::TYPE_ASCII_STRING_OF: {
                return new ASCIIStringOfType(j["oftype"].get<TypeKey>(), annotations);
            }
            case TypeTag::TYPE_SOMETHING: {
                return new SomethingType(j["oftype"].get<TypeKey>(), annotations);
            }
            case TypeTag::TYPE_OPTION: {
                return new OptionType(j["oftype"].get<TypeKey>(), annotations);
            }
            case TypeTag::TYPE_OK: {
                return new OkType(j["ttype"].get<TypeKey>(), j["etype"].get<TypeKey>(), annotations);
            }
            case TypeTag::TYPE_ERROR: {
                return new ErrorType(j["ttype"].get<TypeKey>(), j["etype"].get<TypeKey>(), annotations);
            }
            case TypeTag::TYPE_RESULT: {
                return new ResultType(j["ttype"].get<TypeKey>(), j["etype"].get<TypeKey>(), annotations);
            }
            case TypeTag::TYPE_PATH: {
                return new PathType(j["oftype"].get<TypeKey>(), annotations);
            }
            case TypeTag::TYPE_PATH_FRAGMENT: {
                return new PathFragmentType(j["oftype"].get<TypeKey>(), annotations);
            }
            case TypeTag::TYPE_PATH_GLOB: {
                return new PathGlobType(j["oftype"].get<TypeKey>(), annotations);
            }
            case TypeTag::TYPE_LIST: {
                return new ListType(j["oftype"].get<TypeKey>(), annotations);
            }
            case TypeTag::TYPE_STACK: {
                return new StackType(j["oftype"].get<TypeKey>(), annotations);
            }
            case TypeTag::TYPE_QUEUE: {
                return new QueueType(j["oftype"].get<TypeKey>(), annotations);
            }
            case TypeTag::TYPE_SET: {
                return new SetType(j["oftype"].get<TypeKey>(), annotations);
            }
            case TypeTag::TYPE_MAP_ENTRY: {
                return new MapEntryType(j["ktype"].get<TypeKey>(), j["vtype"].get<TypeKey>(), annotations);
            }
            case TypeTag::TYPE_MAP: {
                return new MapType(j["ktype"].get<TypeKey>(), j["vtype"].get<TypeKey>(), annotations);
            }
            case TypeTag::TYPE_CONCEPT_SET: {
                std::vector<TypeKey> concepts;
                std::transform(j["concepts"].begin(), j["concepts"].end(), std::back_inserter(concepts), [](const json& jv) { return jv.get<TypeKey>(); });
                return new ConceptSetType(annotations, concepts);
            }
            case TypeTag::TYPE_UNION: {
                std::vector<TypeKey> types;
                std::transform(j["types"].begin(), j["types"].end(), std::back_inserter(types), [](const json& jv) { return jv.get<TypeKey>(); });
                return new UnionType(annotations, types);
            }
            default: {
                return UnresolvedType::singleton;
            }
        }
    }

    UnresolvedType* UnresolvedType::singleton = new UnresolvedType();

    NamespaceDecl* NamespaceDecl::parse(json j)
    {
        std::vector<TypeKey> typenames;
        std::transform(j["typenames"].begin(), j["typenames"].end(), std::back_inserter(typenames), [](const json& jv) { return jv.get<TypeKey>(); });

        return new NamespaceDecl(j["ns"].get<std::string>(), typenames);
    }

    void AssemblyInfo::parse(json j, AssemblyInfo& assembly)
    {
        std::for_each(j["namespaces"].begin(), j["namespaces"].end(), [&assembly](const json &ns) { 
            auto nsd = NamespaceDecl::parse(ns);
            assembly.namespaces[nsd->ns] = nsd; 
        });

        std::for_each(j["typerefs"].begin(), j["typerefs"].end(), [&assembly](const json &tr) { 
            auto t = Type::parse(tr);
            assembly.typerefs[t->tkey] = t; 
        });

        std::for_each(j["aliasmap"].begin(), j["aliasmap"].end(), [&assembly](const json &a) { 
            assembly.aliasmap[a[0].get<std::string>()] = assembly.typerefs[a[1].get<TypeKey>()];
        });

        std::for_each(j["revalidators"].begin(), j["revalidators"].end(), [&assembly](const json &rv) {
            std::string ss = rv[1].get<std::string>();
            bool isascii = ss.ends_with("a");
            if(isascii) {
                ss.pop_back();
            }

            auto bre = brex::RegexParser::parseRegex((uint8_t*)ss.c_str(), ss.size(), !isascii, false, false, false);
            assembly.bsqonRegexValidators[rv[0].get<TypeKey>()] = bre.first.value();
        });

        std::for_each(j["pthvalidators"].begin(), j["pthvalidators"].end(), [&assembly](const json &pv) {
            //
            //TODO: need to implement path parser
            //
        });

        std::for_each(j["recursiveSets"].begin(), j["recursiveSets"].end(), [&assembly](const json &rs) { 
            std::set<TypeKey> rset;
            std::transform(rs.begin(), rs.end(), std::inserter(rset, rset.end()), [](const json& jv) { return jv.get<TypeKey>(); });
            assembly.recursiveSets.push_back(rset);
        });
    }

    std::vector<TypeKey> g_primitiveKeyTypes = {
        "None",
        "Bool",
        "Int",
        "Nat",
        "BigInt",
        "BigNat",
        "String",
        "AsciiString",
        "UTCDateTime",
        "PlainDate",
        "PlainTime",
        "TickTime",
        "LogicalTime",
        "ISOTimeStamp",
        "UUIDv4",
        "UUIDv7",
        "SHAContentHash",
        "Regex",
        "ASCIIRegex",
        "ResourceRegex",
        "Path",
        "PathFragment",
        "PathGlob"
    };

    bool AssemblyInfo::isKeyType(TypeKey tkey) const
    {
        auto ttype = this->resolveType(tkey);

        if(ttype->tag == TypeTag::TYPE_PRIMITIVE) {
            return std::find(g_primitiveKeyTypes.begin(), g_primitiveKeyTypes.end(), tkey) != g_primitiveKeyTypes.end();
        }
        else if(ttype->tag == TypeTag::TYPE_ENUM) {
            return true;
        }
        else if(ttype->tag == TypeTag::TYPE_STRING_OF || ttype->tag == TypeTag::TYPE_ASCII_STRING_OF) {
            return true;
        }
        else if(ttype->tag == TypeTag::TYPE_TYPE_DECL) {
            return this->isKeyType(static_cast<const TypedeclType*>(ttype)->oftype);
        }
        else {
            return false;
        }
    }

    void loadAssembly(json j)
    {
        AssemblyInfo::parse(j, g_assembly);
    }

    bool isSubtype(TypeKey tkey, TypeKey oftype)
    {
        auto t = g_assembly.typerefs[tkey];
        auto uu = g_assembly.typerefs[oftype];

        return g_assembly.checkConcreteSubtype(t, uu);
    }
}
