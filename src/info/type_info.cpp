
#include "type_info.h"

namespace bsqon 
{
    static AssemblyInfo g_assembly;

    TypeTag convertTagNameToEnum(const std::string& tname)
    {
        TypeTag ttag = TypeTag::TYPE_UNRESOLVED;

        if(tname == "(|...|)") {
            ttag = TypeTag::TYPE_ELIST;
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
        else if(tname == "Some") {
            ttag = TypeTag::TYPE_SOME;
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
        else if(tname == "APIResult::Rejected") {
            ttag = TypeTag::TYPE_APIREJECTED;
        }
        else if(tname == "APIResult::Failed") {
            ttag = TypeTag::TYPE_APIFAILED;
        }
        else if(tname == "APIResult::Error") {
            ttag = TypeTag::TYPE_APIERROR;
        }
        else if(tname == "APIResult::Success") {
            ttag = TypeTag::TYPE_APISUCCESS;
        }
        else if(tname == "APIResult") {
            ttag = TypeTag::TYPE_APIRESULT;
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
        
        std::vector<TypeKey> supertypes;
        if(j.contains("supertypes") && !j["supertypes"].is_null()) {
            std::transform(j["supertypes"].begin(), j["supertypes"].end(), std::back_inserter(supertypes), [](const json& jv) { return jv.get<TypeKey>(); });
        }

        switch(tt) {
            case TypeTag::TYPE_ELIST: {
                std::vector<TypeKey> entries;
                std::transform(j["entries"].begin(), j["entries"].end(), std::back_inserter(entries), [](const json& jv) { return jv.get<TypeKey>(); });
                return new EListType(entries);
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
                return new StdEntityType(j["tkey"].get<TypeKey>(), supertypes, annotations, fields, hasvalidations);
            }
            case TypeTag::TYPE_STD_CONCEPT: {
                return new StdConceptType(j["tkey"].get<TypeKey>(), supertypes, annotations);
            }
            case TypeTag::TYPE_PRIMITIVE: {
                return new PrimitiveType(j["tkey"].get<TypeKey>(), supertypes, annotations);
            }
            case TypeTag::TYPE_ENUM: {
                std::vector<std::string> variants;
                std::transform(j["variants"].begin(), j["variants"].end(), std::back_inserter(variants), [](const json& jv) { return jv.get<std::string>(); });
                return new EnumType(j["tkey"].get<TypeKey>(), supertypes, annotations, variants);
            }
            case TypeTag::TYPE_TYPE_DECL: {
                std::optional<std::vector<std::string>> optOfValidators = std::nullopt;
                if(j.contains("ofvalidators") && !j["ofvalidators"].is_null()) {
                    const std::vector<std::string> ovv;
                    std::transform(j["ofvalidators"].begin(), j["ofvalidators"].end(), std::back_inserter(ovv), [](const json& jv) { return jv.get<std::string>(); });
                    optOfValidators = std::make_optional(ovv);
                }

                bool hasvalidations = j["hasvalidations"].get<bool>();
                return new TypedeclType(j["tkey"].get<TypeKey>(), supertypes, annotations, j["primitivetype"].get<TypeKey>(), optOfValidators, hasvalidations);
            }
            case TypeTag::TYPE_SOME: {
                return new SomeType(j["oftype"].get<TypeKey>(), supertypes, annotations);
            }
            case TypeTag::TYPE_OPTION: {
                return new OptionType(j["oftype"].get<TypeKey>(), supertypes, annotations);
            }
            case TypeTag::TYPE_OK: {
                return new OkType(j["ttype"].get<TypeKey>(), j["etype"].get<TypeKey>(), supertypes, annotations);
            }
            case TypeTag::TYPE_ERROR: {
                return new ErrorType(j["ttype"].get<TypeKey>(), j["etype"].get<TypeKey>(), supertypes, annotations);
            }
            case TypeTag::TYPE_RESULT: {
                return new ResultType(j["ttype"].get<TypeKey>(), j["etype"].get<TypeKey>(), supertypes, annotations);
            }
            case TypeTag::TYPE_APIREJECTED: {
                return new APIRejectedType(j["ttype"].get<TypeKey>(), supertypes, annotations);
            }
            case TypeTag::TYPE_APIFAILED: {
                return new APIFailedType(j["ttype"].get<TypeKey>(), supertypes, annotations);
            }
            case TypeTag::TYPE_APIERROR: {
                return new APIErrorType(j["ttype"].get<TypeKey>(), supertypes, annotations);
            }
            case TypeTag::TYPE_APISUCCESS: {
                return new APISuccessType(j["ttype"].get<TypeKey>(), supertypes, annotations);
            }
            case TypeTag::TYPE_APIRESULT: {
                return new APIResultType(j["ttype"].get<TypeKey>(), supertypes, annotations);
            }
            case TypeTag::TYPE_LIST: {
                return new ListType(j["oftype"].get<TypeKey>(), supertypes, annotations);
            }
            case TypeTag::TYPE_STACK: {
                return new StackType(j["oftype"].get<TypeKey>(), supertypes, annotations);
            }
            case TypeTag::TYPE_QUEUE: {
                return new QueueType(j["oftype"].get<TypeKey>(), supertypes, annotations);
            }
            case TypeTag::TYPE_SET: {
                return new SetType(j["oftype"].get<TypeKey>(), supertypes, annotations);
            }
            case TypeTag::TYPE_MAP_ENTRY: {
                return new MapEntryType(j["ktype"].get<TypeKey>(), j["vtype"].get<TypeKey>(), supertypes, annotations);
            }
            case TypeTag::TYPE_MAP: {
                return new MapType(j["ktype"].get<TypeKey>(), j["vtype"].get<TypeKey>(), supertypes, annotations);
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

    std::vector<TypeKey> g_primitiveTypes = {
        "None", "Bool", 
        "Nat", "Int", "BigInt", "BigNat", "Rational", "Float", "Decimal", 
        "DecimalDegree", "LatLongCoordinate", 
        "Complex",
        "ByteBuffer", 
        "UUIDv4", "UUIDv7", "SHAContentHash", 
        "TZDateTime", "TAITime", "PlainDate", "PlainTime", "LogicalTime", "ISOTimestamp",
        "DeltaDateTime", "DeltaSeconds", "DeltaLogicalTime", "DeltaISOTimestamp",
        "String", "CString", 
        "Regex", "CRegex", "PathRegex",
        "Path", "Fragment", "Glob"
    };

    std::vector<TypeKey> g_primitiveKeyTypes = {
        "Bool", 
        "Nat", "Int", "BigInt", "BigNat",
        "UUIDv4", "UUIDv7", "SHAContentHash", 
        "TZDateTime", "TAITime", "PlainDate", "PlainTime", "LogicalTime", "ISOTimestamp",
        "DeltaDateTime", "DeltaSeconds", "DeltaLogicalTime", "DeltaISOTimestamp",
        "String", "CString", 
        "Path", "Fragment", "Glob"
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
