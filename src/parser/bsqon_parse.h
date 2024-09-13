#pragma once

#include "../common.h"

#include "../info/type_info.h"
#include "../info/bsqon.h"

#include "../fbparser/bsqon_ast.h"

namespace bsqon
{
    class ParseError
    {
    public:
        std::string message;
        SourcePos loc;

        ParseError(std::string message, SourcePos loc) : message(message), loc(loc) {;}
    };

    enum class ContainerParseStackEntryTag
    {
        Slice,
        List,
        Map
    };

    class ContainerParseStackEntry
    {
    public:
        ContainerParseStackEntryTag tag;
        int64_t ival;
        Value* kval;

        ContainerParseStackEntry(ContainerParseStackEntryTag tag) : tag(tag), ival(0), kval(nullptr) {;}
        ContainerParseStackEntry(ContainerParseStackEntryTag tag, int64_t ival) : tag(tag), ival(ival), kval(nullptr) {;}
    };

    class Parser
    {
    private:
        static SourcePos convertSrcPos(struct AST_SourcePos spos)
        {
            return SourcePos(spos.first_line, spos.first_column, spos.last_line, spos.last_column);
        }

        void addError(std::string message, SourcePos loc)
        {
            this->errors.push_back(ParseError(message, loc));
        }

        static bool isValidNat(const std::string nv, int64_t& vv);
        static bool isValidInt(const std::string nv, int64_t& vv);
        static bool isValidFloat(const std::string nv, double& vv);
        static bool isValidWCTime(const std::string nv, uint64_t& vv);
        static bool isValidWCDelta(const std::string nv, int64_t& vv);

        static bool processDateInfo(const std::string& ds, uint16_t& yy, uint8_t& mm, uint8_t& dd);
        static bool processTimeInfo(const std::string& ds, uint8_t& hh, uint8_t& mm, uint8_t& ss, bool allow60sec);
        static bool processMillisInfo(const std::string& ds, uint16_t& millis);

        bool processTZInfo(const std::string& ds, const char** tz);
        
        bool processDeltaDateInfo(const std::string& ds, uint16_t& yy, uint16_t& mm, uint32_t& dd);
        bool processDeltaTimeInfo(const std::string& ds, uint32_t& hh, uint32_t& mm, uint32_t& ss);
        bool processDeltaMillisInfo(const std::string& ds, uint32_t& millis);

        std::optional<std::vector<Value*>> processEntriesForEList(const EListType* ttype, const BSQON_AST_NODE(BracketValue)* node);
        std::optional<std::vector<Value*>> processPropertiesForEntity(const StdEntityType* ttype, const BSQON_AST_NODE(BraceValue)* node);

        std::optional<Value*> processPropertiesForSpecialCons(const Type* etype, const BSQON_AST_NODE(BraceValue)* node);
        std::optional<std::pair<Value*, Value*>> processPropertiesForMapEntry(const Type* ktype, const Type* vtype, const BSQON_AST_NODE(BraceValue)* node);
        void processEntriesForSequence(const Type* etype, const BSQON_AST_Node* node, std::vector<Value*>& vals, bool advanceimplicitindex);
        void processEntriesForMap(const Type* keytype, const Type* valtype, const BSQON_AST_NODE(BraceValue)* node, std::vector<MapEntryValue*>& entries);

    public:
        const AssemblyInfo* assembly;

        const std::string defaultns;
        std::map<std::string, std::string> importmap;
        std::set<std::string> tzset;

        std::vector<ParseError> errors;

        std::map<std::string, Value*> envbinds;
        std::map<std::string, Value*> vbinds;

        std::list<ContainerParseStackEntry> containerstack;

        Parser(const AssemblyInfo* assembly) {;}
        virtual ~Parser() = default;

        const Type* resolveTypeFromNameList(std::string basenominal, std::vector<const Type*> terms);
        const Type* resolveAndCheckType(TypeKey tkey, SourcePos spos);

        static std::string resolveTypeForRegexLookup(const std::string& tkey, void* assembly);

        const Type* processCoreType(std::string tname);

        bool parseTemplateTermList(const BSQON_AST_LIST_OF_TYPES* tlist, std::vector<const Type*>& terms);
        const Type* parseTemplateTermList_One(SourcePos spos, const BSQON_AST_LIST_OF_TYPES* tlist); 
        std::pair<const Type*, const Type*> parseTemplateTermList_Two(SourcePos spos, const BSQON_AST_LIST_OF_TYPES* tlist);

        const Type* parseTemplateTypeHelper_One(std::string sname, SourcePos spos, const BSQON_AST_LIST_OF_TYPES* tlist);
        const Type* parseTemplateTypeHelper_Two(std::string sname, SourcePos spos, const BSQON_AST_LIST_OF_TYPES* tlist);

        const Type* parseTemplateTypeHelper_OkErr(const Type* tresult, std::string sname, SourcePos spos);
        const Type* parseTemplateTypeHelper_API(const Type* tresult, std::string sname, SourcePos spos);

        const Type* parseSomeType(SourcePos spos, const BSQON_AST_LIST_OF_TYPES* tlist)
        {
            return this->parseTemplateTypeHelper_One("Something", spos, tlist);
        }

        const Type* parseOptionType(SourcePos spos, const BSQON_AST_LIST_OF_TYPES* tlist)
        {
            return this->parseTemplateTypeHelper_One("Option", spos, tlist);
        }

        const Type* parseResultType(SourcePos spos, const BSQON_AST_LIST_OF_TYPES* tlist)
        {
            return this->parseTemplateTypeHelper_Two("Result", spos, tlist);
        }

        const Type* parseListType(SourcePos spos, const BSQON_AST_LIST_OF_TYPES* tlist)
        {
            return this->parseTemplateTypeHelper_One("List", spos, tlist);
        }

        const Type* parseStackType(SourcePos spos, const BSQON_AST_LIST_OF_TYPES* tlist)
        {
            return this->parseTemplateTypeHelper_One("Stack", spos, tlist);
        }

        const Type* parseQueueType(SourcePos spos, const BSQON_AST_LIST_OF_TYPES* tlist)
        {
            return this->parseTemplateTypeHelper_One("Queue", spos, tlist);
        }

        const Type* parseSetType(SourcePos spos, const BSQON_AST_LIST_OF_TYPES* tlist)
        {
            return this->parseTemplateTypeHelper_One("Set", spos, tlist);
        }

        const Type* parseMapEntryType(SourcePos spos, const BSQON_AST_LIST_OF_TYPES* tlist)
        {
            return this->parseTemplateTypeHelper_Two("MapEntry", spos, tlist);
        }

        const Type* parseMapType(SourcePos spos, const BSQON_AST_LIST_OF_TYPES* tlist)
        {
            return this->parseTemplateTypeHelper_Two("Map", spos, tlist);
        }

        const Type* parseNominalType(const BSQON_AST_NODE(NominalType)* node);
        const Type* parseNominalTemplateType(const BSQON_AST_NODE(NominalScopedType)* node);
        const Type* parseEListType(const BSQON_AST_NODE(EListType)* node);
        const Type* parseType(const BSQON_AST_Node* node);

        const Type* parseTypeRoot(const BSQON_AST_Node* node);

        Value* parseNone(const PrimitiveType* t, const BSQON_AST_Node* node);

        Value* parseBool(const PrimitiveType* t, const BSQON_AST_Node* node);
        Value* parseNat(const PrimitiveType* t, const BSQON_AST_Node* node);
        Value* parseInt(const PrimitiveType* t, const BSQON_AST_Node* node);
        Value* parseBigNat(const PrimitiveType* t, const BSQON_AST_Node* node);
        Value* parseBigInt(const PrimitiveType* t, const BSQON_AST_Node* node);

        Value* parseRational(const PrimitiveType* t, const BSQON_AST_Node* node);
        Value* parseFloat(const PrimitiveType* t, const BSQON_AST_Node* node);
        Value* parseDecimal(const PrimitiveType* t, const BSQON_AST_Node* node);
        Value* parseDecimalDegree(const PrimitiveType* t, const BSQON_AST_Node* node);
        Value* parseLatLong(const PrimitiveType* t, const BSQON_AST_Node* node);
        Value* parseComplex(const PrimitiveType* t, const BSQON_AST_Node* node);

        Value* parseString(const PrimitiveType* t, const BSQON_AST_Node* node);
        Value* parseCString(const PrimitiveType* t, const BSQON_AST_Node* node);
        Value* parseByteBuffer(const PrimitiveType* t, const BSQON_AST_Node* node);
        Value* parseUUIDv4(const PrimitiveType* t, const BSQON_AST_Node* node);
        Value* parseUUIDv7(const PrimitiveType* t, const BSQON_AST_Node* node);
        Value* parseSHAHashcode(const PrimitiveType* t, const BSQON_AST_Node* node);

        Value* parseTZDateTime(const PrimitiveType* t, const BSQON_AST_Node* node);
        Value* parseTIATime(const PrimitiveType* t, const BSQON_AST_Node* node);
        Value* parsePlainDate(const PrimitiveType* t, const BSQON_AST_Node* node);
        Value* parsePlainTime(const PrimitiveType* t, const BSQON_AST_Node* node);
        Value* parseLogicalTime(const PrimitiveType* t, const BSQON_AST_Node* node);
        Value* parseISOTimeStamp(const PrimitiveType* t, const BSQON_AST_Node* node);

        Value* parseDeltaDateTime(const PrimitiveType* t, const BSQON_AST_Node* node);
        Value* parseDeltaSeconds(const PrimitiveType* t, const BSQON_AST_Node* node);
        Value* parseDeltaLogical(const PrimitiveType* t, const BSQON_AST_Node* node);
        Value* parseDeltaISOTimeStamp(const PrimitiveType* t, const BSQON_AST_Node* node);

        Value* parseUnicodeRegex(const PrimitiveType* t, const BSQON_AST_Node* node);
        Value* parseCRegex(const PrimitiveType* t, const BSQON_AST_Node* node);
        Value* parsePathRegex(const PrimitiveType* t, const BSQON_AST_Node* node);

        Value* parsePath(const PrimitiveType* t, SourcePos spos, const BSQON_AST_NODE(LiteralPathValue)* node);
        Value* parsePathFragment(const PrimitiveType* t, SourcePos spos, const BSQON_AST_NODE(LiteralPathValue)* node);
        Value* parsePathGlob(const PrimitiveType* t, SourcePos spos, const BSQON_AST_NODE(LiteralPathValue)* node);

        Value* parseSome(const SomeType* t, const BSQON_AST_Node* node);
        Value* parseOk(const OkType* t, const BSQON_AST_Node* node);
        Value* parseErr(const ErrorType* t, const BSQON_AST_Node* node);
        Value* parseMapEntry(const MapEntryType* t, const BSQON_AST_Node* node, bool implicitkey);
        
        Value* parseEList(const EListType* t, const BSQON_AST_Node* node);

        Value* parseEnum(const EnumType* t, const BSQON_AST_Node* node);
        Value* parseTypeDecl(const TypedeclType* t, const BSQON_AST_Node* node);
        Value* parseStdEntity(const StdEntityType* t, const BSQON_AST_Node* node);

        Value* parseList(const ListType* t, const BSQON_AST_Node* node);
        Value* parseStack(const StackType* t, const BSQON_AST_Node* node);
        Value* parseQueue(const QueueType* t, const BSQON_AST_Node* node);
        Value* parseSet(const SetType* t, const BSQON_AST_Node* node);
        Value* parseMap(const MapType* t, const BSQON_AST_Node* node);

        Value* parseValuePrimitive(const PrimitiveType* t, const BSQON_AST_Node* node);
        Value* parseValueDirect(const Type* t, const BSQON_AST_Node* node);

        Value* parseValueConcept(const Type* t, const BSQON_AST_Node* node);
        
        bool isNoneableParse(const UnionType* t)
        {
            return t->types.size() == 2 && std::find(t->types.cbegin(), t->types.cend(), "None") != t->types.cend();
        }

        const Type* getNoneableRealType(const UnionType* t)
        {
            auto tii = std::find_if(t->types.cbegin(), t->types.cend(), [](const TypeKey& tt){ return tt != "None"; });
            return this->assembly->resolveType(*tii);
        }        

        Value* parseValueSimple(const Type* t, const BSQON_AST_Node* node);
        Value* parseValueUnion(const UnionType* t, const BSQON_AST_Node* node);

        Value* parseIdentifier(const Type* t, const BSQON_AST_Node* node);
        Value* parseScopedName(const Type* t, const BSQON_AST_Node* node);
        Value* parseLetIn(const Type* t, const BSQON_AST_Node* node);
        Value* parseAccessIndex(const Type* t, const BSQON_AST_Node* node);
        Value* parseAccessName(const Type* t, const BSQON_AST_Node* node);
        Value* parseAccessKey(const Type* t, const BSQON_AST_Node* node);

        Value* parseUnspecIdentifierValue(const Type* t, const BSQON_AST_Node* node);
        Value* parseEnvAccess(const Type* t, const BSQON_AST_Node* node);

        Value* parseValue(const Type* t, const BSQON_AST_Node* node);

        BsqonDecl* parseBSQON(std::string asmpath, const Type* t, const BSQON_AST_Node* node)
        {
            if(node->tag != BSQON_AST_TAG_BsqonDeclBody) {
                this->addError("Expected a BSQON declaration", convertSrcPos(node->pos));
                return nullptr;
            }

            auto dnoa = BSQON_AST_NODE_AS(BsqonDeclBody, node);
            auto vv = this->parseValue(t, dnoa->value);

            return new BsqonDecl(asmpath, t, vv);
        }

        void tryLoadEnvValue(std::string ename, std::map<std::string, const BSQON_AST_Node*>& envtypes, const BSQON_AST_Node* node)
        {
            auto t = this->parseType(envtypes.at(ename));
            auto vv = this->parseValue(t, node);

            if(vv != nullptr) {
                this->envbinds[ename] = vv;
            }
        }

        static std::map<std::string, const BSQON_AST_Node*> getEnvironmentBindKeys(const BSQON_AST_Node* node)
        {
            std::map<std::string, const BSQON_AST_Node*> envkeys;

            if(node->tag != BSQON_AST_TAG_BsqonDeclBody) {
                return envkeys;
            }

            auto dnoa = BSQON_AST_NODE_AS(BsqonDeclBody, node);
            struct BSQON_AST_NLIST_OF_TYPES* curr = dnoa->envtypes;
            while(curr != nullptr) {
                envkeys[std::string(curr->entry.name)] = curr->entry.value;
            }

            return envkeys;
        }

        static std::pair<std::optional<std::string>, std::optional<std::string>> getAssemblyAndType(const BSQON_AST_Node* node)
        {
            if(node->tag != BSQON_AST_TAG_BsqonDeclBody) {
                return std::make_pair(std::nullopt, std::nullopt);
            }

            auto dnoa = BSQON_AST_NODE_AS(BsqonDeclBody, node);
            if(dnoa->shebangmeta == NULL) {
                return std::make_pair(std::nullopt, std::nullopt);
            }

            std::string smeta = std::string(dnoa->shebangmeta).substr(2);
            auto qidx = smeta.find('?');
            if(qidx == std::string::npos) {
                return std::make_pair(std::optional<std::string>(smeta), std::optional<std::string>());
            }
            else {
                auto asmpath = smeta.substr(0, qidx);
                auto tname = smeta.substr(qidx + 1);
                return std::make_pair(std::optional<std::string>(asmpath), std::optional<std::string>(tname));
            }
        }
    };
}
