
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
        else if(tname == "Result::Fail") {
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
                std::optional<std::vector<std::u8string>> optOfValidators = std::nullopt;
                if(j.contains("ofvalidators") && !j["ofvalidators"].is_null()) {
                    std::vector<std::u8string> ovv;
                    std::transform(j["ofvalidators"].begin(), j["ofvalidators"].end(), std::back_inserter(ovv), [](const json& jv) { 
                        auto vv = jv.get<std::string>(); 
                        return std::u8string{vv.cbegin(), vv.cend()};
                    });
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
        bool istoplevel = j["istoplevel"].get<bool>();

        std::map<std::string, std::string> imports;
        std::transform(j["imports"].begin(), j["imports"].end(), std::inserter(imports, imports.end()), [](const json& jv) { return std::make_pair(jv[0].get<std::string>(), jv[1].get<std::string>()); });

        FullyQualifiedNamespace fullns;
        std::transform(j["fullns"].begin(), j["fullns"].end(), std::back_inserter(fullns), [](const json& jv) { return jv.get<std::string>(); });

        std::string ns = j["ns"].get<std::string>();

        std::map<std::string, FullyQualifiedNamespace> subns;
        std::transform(j["subns"].begin(), j["subns"].end(), std::inserter(subns, subns.end()), [](const json& jv) { 
            FullyQualifiedNamespace fqn;
            std::transform(jv[1].begin(), jv[1].end(), std::back_inserter(fqn), [](const json& jv) { return jv.get<std::string>(); });
            return std::make_pair(jv[0].get<std::string>(), fqn);
        });

        std::map<std::string, TypeKey> types;
        std::transform(j["types"].begin(), j["types"].end(), std::inserter(types, types.end()), [](const json& jv) { return std::make_pair(jv[0].get<std::string>(), jv[1].get<TypeKey>()); });
        
        return new NamespaceDecl(istoplevel, imports, fullns, ns, subns, types);
    }

    void AssemblyInfo::parseRESystem(json j, AssemblyInfo& assembly)
    {
        assert(j.is_array());

        std::vector<brex::RENSInfo> ninfos;
        for(size_t i = 0; i < j.size(); ++i) {
            auto ninfoentry = j[i];
            assert(ninfoentry.is_object() && ninfoentry.contains("nsinfo") && ninfoentry["nsinfo"].is_object() && ninfoentry.contains("reinfos") && ninfoentry["reinfos"].is_array());

            auto remapinfo = ninfoentry["nsinfo"];
            assert(remapinfo.contains("inns") && remapinfo["inns"].is_string() && remapinfo.contains("nsmappings") && remapinfo["nsmappings"].is_array());
            
            std::vector<std::pair<std::string, std::string>> nsmappings;
            auto mappingarr = remapinfo["nsmappings"];

            size_t mappinglen = mappingarr.size();
            for(size_t j = 0; j < mappinglen; ++j) {
                auto nsmappingentry = mappingarr[j];
                assert(nsmappingentry.is_array() && nsmappingentry.size() == 2 && nsmappingentry[0].is_string() && nsmappingentry[1].is_string());

                nsmappings.push_back({nsmappingentry[0].get<std::string>(), nsmappingentry[1].get<std::string>()});
            }

            auto reinfos = ninfoentry["reinfos"];
            std::vector<brex::REInfo> reinfovec;

            size_t reinfoslen = reinfos.size();
            for(size_t j = 0; j < reinfoslen; ++j) {
                auto reinfoentry = reinfos[j];
                assert(reinfoentry.is_object() && reinfoentry.contains("name") && reinfoentry["name"].is_string() && reinfoentry.contains("restr") && reinfoentry["restr"].is_string());

                std::string rename = reinfoentry["name"].get<std::string>();
                std::string restring = reinfoentry["restr"].get<std::string>();

                std::u8string u8restring = std::u8string(restring.cbegin(), restring.cend());

                reinfovec.push_back({rename, u8restring});
            }

            ninfos.push_back({{remapinfo["inns"].get<std::string>(), nsmappings}, reinfovec});
        }

        std::vector<std::u8string> errors;
        brex::ReSystem rsystem = brex::ReSystem::processSystem(ninfos, errors);

        assert(errors.empty());

        for(size_t i = 0; i < rsystem.entries.size(); ++i) {
            if(rsystem.entries[i]->isUnicode()) {
                auto reentry = static_cast<brex::ReSystemUnicodeEntry*>(rsystem.entries[i]);

                if(reentry->re->isValidNamedRegexComponent()) {
                    assembly.namedUnicodeRegexMap[reentry->fullname] = static_cast<const brex::RegexSingleComponent*>(reentry->re->re)->entry.opt;
                }
            }
            else {
                auto reentry = static_cast<brex::ReSystemCEntry*>(rsystem.entries[i]);

                if(reentry->re->isValidNamedRegexComponent()) {
                    assembly.namedCRegexMap[reentry->fullname] = static_cast<const brex::RegexSingleComponent*>(reentry->re->re)->entry.opt;
                }
            }
        }

        assembly.nsremapper = rsystem.remapper;
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

        std::for_each(j["recursiveSets"].begin(), j["recursiveSets"].end(), [&assembly](const json &rs) { 
            std::set<TypeKey> rset;
            std::transform(rs.begin(), rs.end(), std::inserter(rset, rset.end()), [](const json& jv) { return jv.get<TypeKey>(); });
            assembly.recursiveSets.push_back(rset);
        });

        if(j.contains("resystem") && !j["resystem"].is_null()) {
            auto resystem = j["resystem"];
            AssemblyInfo::parseRESystem(resystem, assembly);
        }
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
        "Path", "PathItem", "Glob"
    };

    std::vector<TypeKey> g_primitiveKeyTypes = {
        "Bool", 
        "Nat", "Int", "BigInt", "BigNat",
        "UUIDv4", "UUIDv7", "SHAContentHash", 
        "TZDateTime", "TAITime", "PlainDate", "PlainTime", "LogicalTime", "ISOTimestamp",
        "DeltaDateTime", "DeltaSeconds", "DeltaLogicalTime", "DeltaISOTimestamp",
        "String", "CString", 
        "Path", "PathItem", "Glob"
    };

    std::u8string AssemblyInfo::resolveConstantRegexValue(const std::u8string& reexp, bool isutf) const {
        if(reexp.starts_with(u8'/')) {
            return reexp;
        }
        else {
            return u8"/${" + reexp + u8"}/" + (isutf ? u8"" : u8"c");
        }
    }

    void AssemblyInfo::processUnicodeRegex(const std::string& inns, const std::u8string& regex) 
    {
        auto pr = brex::RegexParser::parseUnicodeRegex(regex, false);
        assert(pr.first.has_value());

        std::vector<brex::RegexCompileError> compileerror;
        auto rmp = brex::ReSystemResolverInfo(inns, &nsremapper);
        auto executor = brex::RegexCompiler::compileUnicodeRegexToExecutor(pr.first.value(), this->namedUnicodeRegexMap, {}, false, &rmp, &brex::ReSystem::resolveREName, compileerror);
        assert(compileerror.empty());

        this->executableUnicodeRegexMap[regex] = executor;
    }

    void AssemblyInfo::processCRegex(const std::string& inns, const std::u8string& regex) 
    {
        auto pr = brex::RegexParser::parseCRegex(regex, false);
        assert(pr.first.has_value());

        std::vector<brex::RegexCompileError> compileerror;
        auto rmp = brex::ReSystemResolverInfo(inns, &nsremapper);
        auto executor = brex::RegexCompiler::compileCRegexToExecutor(pr.first.value(), this->namedCRegexMap, {}, false, &rmp, &brex::ReSystem::resolveREName, compileerror);
        assert(compileerror.empty());

        this->executableCRegexMap[regex] = executor;
    }

    bool AssemblyInfo::validateString(const std::u8string& regex, brex::UnicodeString* ustr, const std::string& inns)
    {
        auto uregex = this->resolveConstantRegexValue(regex, true);
        this->processUnicodeRegex(inns, uregex);
    
        brex::ExecutorError err;
        bool accepts = this->executableUnicodeRegexMap[uregex]->test(ustr, err);
    
        assert(err == brex::ExecutorError::Ok);
        return accepts;
    }

    bool AssemblyInfo::validateCString(const std::u8string& regex, brex::CString* cstr, const std::string& inns)
    {
        auto cregex = this->resolveConstantRegexValue(regex, false);
        this->processCRegex(inns, cregex);

        brex::ExecutorError err;
        bool accepts = this->executableCRegexMap[cregex]->test(cstr, err);
    
        assert(err == brex::ExecutorError::Ok);
        return accepts;
    }

    bool AssemblyInfo::isKeyType(TypeKey tkey) const
    {
        auto ttype = this->lookupTypeKey(tkey);

        if(ttype->tag == TypeTag::TYPE_PRIMITIVE) {
            return std::find(g_primitiveKeyTypes.begin(), g_primitiveKeyTypes.end(), tkey) != g_primitiveKeyTypes.end();
        }
        else if(ttype->tag == TypeTag::TYPE_ENUM) {
            return true;
        }
        else if(ttype->tag == TypeTag::TYPE_TYPE_DECL) {
            return this->isKeyType(static_cast<const TypedeclType*>(ttype)->primitivetype);
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

        return g_assembly.checkSubtype(t, uu);
    }
}
