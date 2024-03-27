#include "bsqon_parse.h"

#include <inttypes.h>

namespace bsqon
{
    std::vector<std::string> s_coreTypes = {
        "None", "Nothing", "Bool", 
        "Int", "Nat", "BigInt", "BigNat", "Rational", "Float", "Decimal", "DecimalDegree", "LatLongCoordinate", "Complex",
        "String", "ASCIIString", "StringView", "ASCIIStringView" "ByteBuffer", 
        "DateTime", "UTCDateTime", "PlainDate", "PlainTime", "TickTime", "LogicalTime", "ISOTimeStamp", 
        "UUIDv4", "UUIDv7", "SHAContentHash", 
        "DataTimeDelta", "PlainDateDelta", "PlainTimeDelta", "ISOTimestampDelta", "SecondsDelta", "TickTimeDelta", "LogicalTimeDelta",
        "UnicodeRegex", "ASCIIRegex", "PathRegex" 
    };

    std::vector<TypeTag> s_okTypeTaggedTags = {
        TypeTag::TYPE_STD_ENTITY,
        TypeTag::TYPE_RECORD, TypeTag::TYPE_TUPLE,
        TypeTag::TYPE_SOMETHING, TypeTag::TYPE_OK, TypeTag::TYPE_ERROR,
        TypeTag::TYPE_LIST, TypeTag::TYPE_STACK, TypeTag::TYPE_QUEUE, TypeTag::TYPE_SET, TypeTag::TYPE_MAP_ENTRY, TypeTag::TYPE_MAP
    };

    bool s_isLeapYear(uint16_t y)
    {
        if(y == 1900 || y == 2100 || y == 2200) {
            return false;
        }
        else {
            return y % 4 == 0;
        }
    }

    bool s_dayInMonth(uint8_t d, uint8_t m, uint16_t y)
    {
        if(m == 1) {
            if(s_isLeapYear(y)) {
                return d <= 29;
            }
            else {
                return d <= 28;
            }
        }
        else {
            if(m == 3 || m == 5 || m == 8 || m == 10) {
                return d <= 30;
            }
            else {
                return d <= 31;
            }
        }
    }

    class ContainerParseStackEntryManager
    {
    public:
        Parser* parser;

        ContainerParseStackEntryManager(ContainerParseStackEntryTag tag, Parser* p) : parser(p) 
        {
            parser->containerstack.push_front(ContainerParseStackEntry(tag));
        }

        ContainerParseStackEntryManager(ContainerParseStackEntryTag tag, Parser* p, int64_t npos) : parser(p) 
        {
            parser->containerstack.push_front(ContainerParseStackEntry(tag, npos));
        }

        ~ContainerParseStackEntryManager() 
        {
            parser->containerstack.pop_front();
        }

        static std::optional<int64_t> resolveNPOS(std::list<ContainerParseStackEntry>& stack)
        {
            auto ii = std::find_if(stack.cbegin(), stack.cend(), [](const ContainerParseStackEntry& pp) { return pp.tag == ContainerParseStackEntryTag::Slice; });
            return ii != stack.cend() ? std::make_optional(ii->ival) : std::nullopt;
        }

        static std::optional<uint64_t> resolveI(std::list<ContainerParseStackEntry>& stack) 
        {
            auto ii = std::find_if(stack.cbegin(), stack.cend(), [](const ContainerParseStackEntry& pp) { return pp.tag == ContainerParseStackEntryTag::List; });
            return ii != stack.cend() ? std::make_optional(ii->ival) : std::nullopt;
        }

        static Value* resolveKey(std::list<ContainerParseStackEntry>& stack) 
        {
            auto ii = std::find_if(stack.cbegin(), stack.cend(), [](const ContainerParseStackEntry& pp) { return pp.tag == ContainerParseStackEntryTag::Map; });
            return ii != stack.cend() ? ii->kval : nullptr;
        }
    };

    bool Parser::isValidNat(const std::string nv, int64_t& vv)
    {
        auto ecount = sscanf(nv.c_str(), "%" SCNu64, &vv);
        return ecount == 1 && 0 <= vv && vv <= Type::MAX_SAFE_NAT; 
    }

    bool Parser::isValidInt(const std::string nv, int64_t& vv)
    {
        auto ecount = sscanf(nv.c_str(), "%" SCNd64, &vv);
        return ecount == 1 && Type::MIN_SAFE_INT <= vv && vv <= Type::MAX_SAFE_INT;
    }

    bool Parser::isValidFloat(const std::string nv, double& vv)
    {
        auto ecount = sscanf(nv.c_str(), "%lf", &vv);
        return ecount == 1;
    }

    bool Parser::isValidWCTime(const std::string nv, uint64_t& vv)
    {
        auto ecount = sscanf(nv.c_str(), "%" SCNu64, &vv);
        return ecount == 1;
    }

    bool Parser::isValidWCDelta(const std::string nv, int64_t& vv)
    {
        auto ecount = sscanf(nv.c_str(), "%" SCNi64, &vv);
        return ecount == 1;
    }

    bool Parser::processDateInfo(const std::string& ds, uint16_t& yy, uint8_t& mm, uint8_t& dd)
    {
        auto pp = sscanf(ds.c_str(), "%4" SCNu16 "-%2" SCNu8 "-%2" SCNu8, &yy, &mm, &dd);

        return pp == 3 && (1900 <= yy && yy <= 2200) && mm < 12 && s_dayInMonth(dd, mm, yy);
    }

    bool Parser::processTimeInfo(const std::string& ds, uint8_t& hh, uint8_t& mm, uint8_t& ss)
    {
        auto pp = sscanf(ds.c_str(), "%2" SCNu8 ":%2" SCNu8 ":%2" SCNu8, &hh, &mm, &ss);

        return pp == 3 && hh < 24 && mm < 60 && ss < 61;
    }

    bool Parser::processMillisInfo(const std::string& ds, uint16_t& millis)
    {
        auto pp = sscanf(ds.c_str(), ".%3" SCNu16, &millis);

        return pp == 1 && millis < 1000;
    }

    bool Parser::processTZInfo(const std::string& ds, const char** tz)
    {
        std::string ttz;
        if(ds.starts_with("{")) {
            ttz = ds.substr(1, ds.size() - 2); //trim {...}
        }
        else {
            ttz = ds;
        }

        auto tzii = this->tzset.insert(ttz);
        *tz = tzii.first->c_str();
        
        return true;
    }

    bool Parser::processDeltaDateInfo(const std::string& ds, uint16_t& yy, uint16_t& mm, uint32_t& dd)
    {
        auto pp = sscanf(ds.c_str(), "%4" SCNu16 "-%2" SCNu16 "-%2" SCNu32, &yy, &mm, &dd);
        return pp == 3;
    }

    bool Parser::processDeltaTimeInfo(const std::string& ds, uint32_t& hh, uint32_t& mm, uint32_t& ss)
    {
        auto pp = sscanf(ds.c_str(), "%2" SCNu32 ":%2" SCNu32 ":%2" SCNu32, &hh, &mm, &ss);
        return pp == 3;
    }

    bool Parser::processDeltaMillisInfo(const std::string& ds, uint32_t& millis)
    {
        auto pp = sscanf(ds.c_str(), ".%3" SCNu32, &millis);
        return pp == 1;
    }

    std::optional<std::vector<Value*>> Parser::processEntriesForTuple(const TupleType* ttype, const BSQON_AST_NODE(BracketValue)* node)
    {
        std::vector<Value*> elems;

        auto titer = ttype->entries.cbegin();
        for(auto curr = node->values; curr != NULL; curr = curr->next) {
            elems.push_back(this->parseValue(this->assembly->resolveType(*titer++), curr->value));
        }

        if(elems.size() != ttype->entries.size()) {
            this->addError("Tuple value has incorrect number of elements", Parser::convertSrcPos(node->base.pos));
            return std::nullopt;
        }
        else {
            return std::make_optional(std::move(elems));
        }
    }

    std::optional<std::vector<std::pair<std::string, Value*>>> Parser::processPropertiesForRecord(const RecordType* ttype, const BSQON_AST_NODE(BraceValue)* node)
    {
        std::vector<std::pair<std::string, Value*>> props;

        for(auto curr = node->entries; curr != NULL; curr = curr->next) {
            if(curr->entry.name == NULL) {
                this->addError("Record value has unnamed property", Parser::convertSrcPos(node->base.pos));
            }
            else {
                std::string pname(curr->entry.name);

                auto ptypeiter = std::find_if(ttype->entries.cbegin(), ttype->entries.cend(), [&pname](const RecordTypeEntry& rr) { return rr.pname == pname; });
                if(ptypeiter == ttype->entries.cend()) {
                    this->addError("Unknown property name " + pname, Parser::convertSrcPos(node->base.pos));
                }
                else {
                    if(std::find_if(props.cbegin(), props.cend(), [&pname](const std::pair<std::string, Value*>& pp) { return pp.first == pname; }) != props.cend()) {
                        this->addError("Duplicate property name " + pname, Parser::convertSrcPos(node->base.pos));
                    }
                    else {
                        props.push_back(std::make_pair(pname, this->parseValue(this->assembly->resolveType(ptypeiter->ptype), curr->entry.value)));
                    }
                }
            }
        }

        if(props.size() != ttype->entries.size()) {
            this->addError("Record value has mismatched properties", Parser::convertSrcPos(node->base.pos));
            return std::nullopt;
        }
        else {
            std::sort(props.begin(), props.end(), [](const std::pair<std::string, Value*>& a, const std::pair<std::string, Value*>& b) { return a.first < b.first; });
            return std::make_optional(std::move(props));
        }
    }

    std::optional<std::vector<Value*>> Parser::processPropertiesForEntity(const StdEntityType* ttype, const BSQON_AST_NODE(BraceValue)* node)
    {
        std::vector<Value*> vals(ttype->fields.size(), nullptr);

        //pass over values to set named fields
        for(auto curr = node->entries; curr != NULL; curr = curr->next) {
            if(curr->entry.name != NULL) {
                std::string pname(curr->entry.name);

                auto fiter = std::find_if(ttype->fields.cbegin(), ttype->fields.cend(), [&pname](const EntityTypeFieldEntry& pp) { return pp.fname == pname; });
                if(fiter == ttype->fields.cend()) {
                    this->addError("Unknown field name " + pname, Parser::convertSrcPos(node->base.pos));
                }
                else {
                    auto idx = std::distance(ttype->fields.cbegin(), fiter);
                    if(vals[idx] != nullptr) {
                        this->addError("Duplicate field entry " + pname, Parser::convertSrcPos(node->base.pos));
                    }
                    else {
                        vals[idx] = this->parseValue(this->assembly->resolveType(fiter->ftype), curr->entry.value);
                    }
                }
            }
        }

        //pass over values to fill in positional fields
        auto positer = std::find_if(vals.begin(), vals.end(), [](Value* vv) { return vv == nullptr; });
        for(auto curr = node->entries; curr != NULL; curr = curr->next) {
            if(curr->entry.name == NULL) {
                if(positer == vals.cend()) {
                    this->addError("Too many values for type", Parser::convertSrcPos(node->base.pos));
                }
                else {
                    auto fpos = std::distance(vals.begin(), positer);
                    const EntityTypeFieldEntry& fentry = ttype->fields[fpos];

                    Value* vv = this->parseValue(this->assembly->resolveType(fentry.ftype), curr->entry.value);
                    *positer = vv;

                    positer = std::find_if(positer++, vals.end(), [](Value* vv) { return vv == nullptr; });
                }
            }
        }

        if(positer != vals.cend()) {
            this->addError("Too few values for type", Parser::convertSrcPos(node->base.pos));
            return std::nullopt;
        }
        else {
            return std::make_optional(std::move(vals));
        }
    }

    std::optional<Value*> Parser::processPropertiesForSpecialCons(const Type* etype, const BSQON_AST_NODE(BraceValue)* node)
    {
        if(node->entries == NULL) {
            this->addError("Missing argument", Parser::convertSrcPos(node->base.pos));
            return std::nullopt;
        }
        else if(node->entries->next != NULL) {
            this->addError("Too many arguments", Parser::convertSrcPos(node->base.pos));
            return std::nullopt;
        }
        else if(node->entries->entry.name != NULL) {
            this->addError("Special constructors for value does not accept named fields", Parser::convertSrcPos(node->base.pos));
            return std::nullopt;
        }
        else {
            return std::make_optional(this->parseValue(etype, node->entries->entry.value));
        }
    }

    std::optional<std::pair<Value*, Value*>> Parser::processPropertiesForMapEntry(const Type* ktype, const Type* vtype, const BSQON_AST_NODE(BraceValue)* node)
    {
        if(node->entries == NULL || node->entries->next == NULL) {
            this->addError("Missing argument", Parser::convertSrcPos(node->base.pos));
            return std::nullopt;
        }
        else if(node->entries->next->next != NULL) {
            this->addError("Too many arguments", Parser::convertSrcPos(node->base.pos));
            return std::nullopt;
        }
        else if(node->entries->entry.name != NULL || node->entries->next->entry.name != NULL) {
            this->addError("MapEntry value does not accept named fields", Parser::convertSrcPos(node->base.pos));
            return std::nullopt;
        }
        else {
            return std::make_optional(std::make_pair(this->parseValue(ktype, node->entries->entry.value), this->parseValue(vtype, node->entries->next->entry.value)));
        }
    }

    void Parser::processEntriesForSequence(const Type* etype, const BSQON_AST_Node* node, std::vector<Value*>& vals, bool advanceimplicitindex)
    {
        if(node->tag == BSQON_AST_TAG_BracketValue) {
            auto bnode = BSQON_AST_NODE_AS(BracketValue, node);
            for(auto curr = bnode->values; curr != NULL; curr = curr->next) {
                vals.push_back(this->parseValue(etype, curr->value));

                if(advanceimplicitindex) {
                    this->containerstack.back().ival++;
                }
            }
        }
        else {
            auto bnode = BSQON_AST_NODE_AS(BraceValue, node);
            for(auto curr = bnode->entries; curr != NULL; curr = curr->next) {
                if(curr->entry.name != NULL) {
                    this->addError("Sequence value cannot have named property", Parser::convertSrcPos(node->pos));
                }
                vals.push_back(this->parseValue(etype, curr->entry.value));

                if(advanceimplicitindex) {
                    this->containerstack.back().ival++;
                }
            }
        }
    }
        
    void Parser::processEntriesForMap(const Type* keytype, const Type* valtype, const BSQON_AST_NODE(BraceValue)* node, std::vector<MapEntryValue*>& entries)
    {
        const MapEntryType* metype = static_cast<const MapEntryType*>(this->assembly->resolveType("MapEntry<" + keytype->tkey + ", " + valtype->tkey + ">"));

        for(auto curr = node->entries; curr != NULL; curr = curr->next) {
            if(curr->entry.name != NULL) {
                this->addError("Map value has named property", Parser::convertSrcPos(node->base.pos));
            }
            else {
                if(curr->entry.value->tag == BSQON_AST_TAG_MapEntryValue) {
                    entries.push_back(static_cast<MapEntryValue*>(this->parseMapEntry(metype, curr->entry.value, true)));
                }
                else {
                    entries.push_back(static_cast<MapEntryValue*>(this->parseValue(metype, curr->entry.value)));
                }
            }
        }
    }

    const Type* Parser::resolveTypeFromNameList(std::string basenominal, std::vector<const Type*> terms)
    {
        std::string baseprefix = basenominal.substr(0, basenominal.find("::"));

        std::string scopedname;
        if (this->assembly->namespaces.at("Core")->hasTypenameDecl(basenominal)) {
            scopedname = basenominal;
        }
        else if (this->importmap.find(baseprefix) != this->importmap.end()) {
            scopedname = this->importmap.at(baseprefix) + basenominal.substr(baseprefix.size());
        }
        else {
            if (this->assembly->namespaces.find(baseprefix) != this->assembly->namespaces.end()) {
                scopedname = basenominal;
            }
            else {
                scopedname = this->defaultns + "::" + basenominal;
            }
        }

        if (!terms.empty()) {
            scopedname = scopedname + "<" + std::accumulate(terms.cbegin(), terms.cend(), std::string(), [](std::string&& a, const Type* b) { return a + ", " + b->tkey; }) + ">";
        }

        auto titer = this->assembly->typerefs.find(scopedname);
        if (titer != this->assembly->typerefs.end()) {
            return titer->second;
        }
        else {
            auto aiter = this->assembly->aliasmap.find(scopedname);
            if(aiter != this->assembly->aliasmap.end()) {
                return aiter->second;
            }
            else {
                return UnresolvedType::singleton;
            }
        }
    }

    const Type* Parser::resolveAndCheckType(TypeKey tkey, SourcePos spos)
    {
        auto tt = this->assembly->resolveType(tkey);
        if(tt->isUnresolved()) {
            this->addError("Could not resolve type " + tkey, spos);
        }
            
        return tt;
    }

    std::string Parser::resolveTypeForRegexLookup(const std::string& tkey, void* assembly)
    {
        return ((AssemblyInfo*)assembly)->resolveType(tkey)->tkey;
    }

    const Type* Parser::processCoreType(std::string tname) 
    {
        return this->resolveTypeFromNameList(tname, {});
    }

    bool Parser::parseTemplateTermList(const BSQON_AST_LIST_OF_TYPES* tlist, std::vector<const Type*>& terms) 
    {
        for(auto curr = tlist; curr != NULL; curr = curr->next) {
            terms.push_back(this->parseType(curr->value));
        }
     
        return std::find_if(terms.begin(), terms.end(), [](const Type* tt) { return tt->isUnresolved(); }) == terms.end();
    }

    const Type* Parser::parseTemplateTermList_One(SourcePos spos, const BSQON_AST_LIST_OF_TYPES* tlist) 
    {
        std::vector<const Type*> terms;
        bool ok = this->parseTemplateTermList(tlist, terms);

        if(ok && terms.size() == 1) {
            return terms[0];
        }
        else {
            if(terms.size() != 1) {
                this->addError("Expected 1 template arg but got " + std::to_string(terms.size()), spos);
            }
            return UnresolvedType::singleton;
        }
    }

    std::pair<const Type*, const Type*> Parser::parseTemplateTermList_Two(SourcePos spos, const BSQON_AST_LIST_OF_TYPES* tlist) 
    {
        std::vector<const Type*> terms;
        bool ok = this->parseTemplateTermList(tlist, terms);

        if(ok && terms.size() == 2) {
            return std::make_pair(terms[0], terms[1]);
        }
        else {
            if(terms.size() != 2) {
                this->addError("Expected 2 template arg but got " + std::to_string(terms.size()), spos);
            }
            return std::make_pair(UnresolvedType::singleton, UnresolvedType::singleton);
        }
    }

    const Type* Parser::parseTemplateTypeHelper_One(std::string sname, SourcePos spos, const BSQON_AST_LIST_OF_TYPES* tlist)
    {
        const Type* oftype = this->parseTemplateTermList_One(spos, tlist);
            
        if(oftype->isUnresolved()) {
            return UnresolvedType::singleton;
        } 
        else {
            return this->resolveAndCheckType(sname + "<" + oftype->tkey + ">", spos);
        }
    }

    const Type* Parser::parseTemplateTypeHelper_Two(std::string sname, SourcePos spos, const BSQON_AST_LIST_OF_TYPES* tlist)
    {
        std::pair<const Type*, const Type*> oftype = this->parseTemplateTermList_Two(spos, tlist);
            
        if(oftype.first->isUnresolved() || oftype.second->isUnresolved()) {
            return UnresolvedType::singleton;
        } 
        else {
            return this->resolveAndCheckType(sname + "<" + oftype.first->tkey + ", " + oftype.second->tkey + ">", spos);
        }
    }

    const Type* Parser::parseTemplateTypeHelper_OkErr(const Type* tresult, std::string sname, SourcePos spos)
    {
        if(tresult->isUnresolved()) {
            return UnresolvedType::singleton;
        } 
        else {
            return this->resolveAndCheckType(tresult->tkey + "::" + sname , spos);
        }
    }

    const Type* Parser::parseNominalType(const BSQON_AST_NODE(NominalType)* node)
    {
        std::string tname(node->name);

        bool iscore = std::find(s_coreTypes.begin(), s_coreTypes.end(), tname) != s_coreTypes.end();
        if (iscore) {
            return this->processCoreType(tname);
        }
        else if (tname == "StringOf") {
            return this->parseStringOfType(Parser::convertSrcPos(node->base.pos), node->terms);
        }
        else if (tname == "ASCIIStringOf") {
            return this->parseASCIIStringOfType(Parser::convertSrcPos(node->base.pos), node->terms);
        }
        else if (tname == "Something") {
            return this->parseSomethingType(Parser::convertSrcPos(node->base.pos), node->terms);
        }
        else if (tname == "Option") {
            return this->parseOptionType(Parser::convertSrcPos(node->base.pos), node->terms);
        }
        else if (tname == "Result") {
            return this->parseResultType(Parser::convertSrcPos(node->base.pos), node->terms);
        }
        else if (tname == "Path") {
            return this->parsePathType(Parser::convertSrcPos(node->base.pos), node->terms);
        }
        else if (tname == "PathFragment") {
            return this->parsePathFragmentType(Parser::convertSrcPos(node->base.pos), node->terms);
        }
        else if (tname == "PathGlob") {
            return this->parsePathGlobType(Parser::convertSrcPos(node->base.pos), node->terms);
        }
        else if (tname == "List") {
            return this->parseListType(Parser::convertSrcPos(node->base.pos), node->terms);
        }
        else if (tname == "Stack") {
            return this->parseStackType(Parser::convertSrcPos(node->base.pos), node->terms);
        }
        else if (tname == "Queue") {
            return this->parseQueueType(Parser::convertSrcPos(node->base.pos), node->terms);
        }
        else if (tname == "Set") {
            return this->parseSetType(Parser::convertSrcPos(node->base.pos), node->terms);
        }
        else if (tname == "MapEntry") {
            return this->parseMapEntryType(Parser::convertSrcPos(node->base.pos), node->terms);
        }
        else if (tname == "Set") {
            return this->parseMapType(Parser::convertSrcPos(node->base.pos), node->terms);
        }
        else {
            std::vector<const Type*> terms;
            this->parseTemplateTermList(node->terms, terms);
                
            if(std::any_of(terms.begin(), terms.end(), [](const Type* tt) { return tt->isUnresolved(); })) {
                return UnresolvedType::singleton;
            }
            else {
                return this->resolveTypeFromNameList(tname, terms);
            }
        }
    }

    const Type* Parser::parseNominalTemplateType(const BSQON_AST_NODE(NominalScopedType)* node)
    {
        const Type* roottype = this->parseType(node->root);
        if(!roottype->tkey.starts_with("Result<")) {
            this->addError("Expected Result type", Parser::convertSrcPos(node->root->pos));
            return UnresolvedType::singleton;
        }
        
        std::string etype = std::string(node->ext);
        if(etype == "Ok") {
            return this->parseTemplateTypeHelper_OkErr(roottype, "Ok", Parser::convertSrcPos(node->base.pos));
        }
        else if(etype == "Err") {
            return this->parseTemplateTypeHelper_OkErr(roottype, "Err", Parser::convertSrcPos(node->base.pos));
        }
        else {
            this->addError("Expected Result::Ok or Result::Err type", Parser::convertSrcPos(node->root->pos));
            return UnresolvedType::singleton;
        }
    }

    const Type* Parser::parseTupleType(const BSQON_AST_NODE(TupleType)* node)
    {
        std::vector<const Type*> types;
        this->parseTemplateTermList(node->types, types);
                
        if(std::any_of(types.cbegin(), types.cend(), [](const Type* tt) { return tt->isUnresolved(); })) {
            return UnresolvedType::singleton;
        }
        else {
            std::vector<TypeKey> kk;
            std::transform(types.cbegin(), types.cend(), std::back_inserter(kk), [](const Type* tt){ return tt->tkey; });

            auto tkey = "[" + std::accumulate(kk.begin(), kk.end(), std::string(), [](std::string&& a, TypeKey& b) { return (a == "" ? "" : std::move(a) + ", ") + b; }) + "]";
            return this->resolveAndCheckType(tkey, Parser::convertSrcPos(node->base.pos));
        }
    }

    const Type* Parser::parseRecordType(const BSQON_AST_NODE(RecordType)* node) 
    {
        std::vector<RecordTypeEntry> entries;
        for(auto curr = node->entries; curr != NULL; curr = curr->next) {
            auto pname = std::string(curr->entry.name);
            auto ptype = this->parseType(curr->entry.value);
            
            if(ptype->isUnresolved()) {
                return UnresolvedType::singleton;
            }
            else {
                entries.push_back(RecordTypeEntry{pname, ptype->tkey});
            }
        }

        std::sort(entries.begin(), entries.end(), [](const RecordTypeEntry& a, const RecordTypeEntry& b) { return a.pname < b.pname; });
        auto tkey = "{" + std::accumulate(entries.cbegin(), entries.cend(), std::string(), [](std::string&& a, const RecordTypeEntry& b) { return (a == "" ? "" : std::move(a) + ", ") + b.pname + ": " + b.ptype; }) + "}";
        return this->resolveAndCheckType(tkey, Parser::convertSrcPos(node->base.pos));
    }

    void Parser::parseConceptSetType_Helper(const BSQON_AST_Node* node, std::vector<const Type*>& tlist)
    {
        if(node->tag != BSQON_AST_TAG_ConjunctionType) {
            tlist.push_back(this->parseType(node));
        }
        else {
            const BSQON_AST_NODE(ConjunctionType)* cnode = BSQON_AST_NODE_AS(ConjunctionType, node);
            this->parseConceptSetType_Helper(cnode->left, tlist);
            this->parseConceptSetType_Helper(cnode->right, tlist);
        }
    }

    const Type* Parser::parseConceptSetType(const BSQON_AST_NODE(ConjunctionType)* node) 
    {
        std::vector<const Type*> conjs;
        this->parseConceptSetType_Helper(node->left, conjs);
        this->parseConceptSetType_Helper(node->right, conjs);
                
        if(std::any_of(conjs.cbegin(), conjs.cend(), [](const Type* tt) { return tt->isUnresolved(); })) {
            return UnresolvedType::singleton;
        }

        //
        //TODO: Assume that there is no subsumption here -- later we will want to check for this 
        //  Add a subtype relation in the Assembly and check/sort here.

        std::vector<TypeKey> concepts;
        std::transform(conjs.cbegin(), conjs.cend(), std::back_inserter(concepts), [](const Type* tt){ return tt->tkey; });

        std::sort(concepts.begin(), concepts.end());
        auto tkey = std::accumulate(concepts.cbegin(), concepts.cend(), std::string(), [](std::string&& a, const TypeKey& b) { return (a == "" ? "" : std::move(a) + "&") + b; });
        return this->resolveAndCheckType(tkey, Parser::convertSrcPos(node->base.pos));
    }

    void Parser::parseUnionType_Helper(const BSQON_AST_Node* node, std::vector<const Type*>& tlist)
    {
        if(node->tag != BSQON_AST_TAG_UnionType) {
            tlist.push_back(this->parseType(node));
        }
        else {
            const BSQON_AST_NODE(UnionType)* unode = BSQON_AST_NODE_AS(UnionType, node);
            this->parseUnionType_Helper(unode->left, tlist);
            this->parseUnionType_Helper(unode->right, tlist);
        }
    }

    const Type* Parser::parseUnionType(const BSQON_AST_NODE(UnionType)* node)
    {
        std::vector<const Type*> opts;
        this->parseUnionType_Helper(node->left, opts);
        this->parseUnionType_Helper(node->right, opts);
                
        if(std::any_of(opts.cbegin(), opts.cend(), [](const Type* tt) { return tt->isUnresolved(); })) {
            return UnresolvedType::singleton;
        }

        //
        //TODO: Assume that there is no subsumption here -- later we will want to check for this 
        //  Add a subtype relation in the Assembly and check/sort here.
        
        std::vector<TypeKey> disjuncts;
        std::transform(opts.cbegin(), opts.cend(), std::back_inserter(disjuncts), [](const Type* tt){ return tt->tkey; });

        std::sort(disjuncts.begin(), disjuncts.end());
        auto tkey = std::accumulate(disjuncts.cbegin(), disjuncts.cend(), std::string(), [](std::string&& a, const TypeKey& b) { return (a == "" ? "" : std::move(a) + " | ") + b; });
        return this->resolveAndCheckType(tkey, Parser::convertSrcPos(node->base.pos));
    }

    const Type* Parser::parseType(const BSQON_AST_Node* node)
    {
        switch(node->tag)
        {
            case BSQON_AST_TAG_Error:
                return UnresolvedType::singleton;
            case BSQON_AST_TAG_NominalType:
                return this->parseNominalType(BSQON_AST_NODE_AS(NominalType, node));
            case BSQON_AST_TAG_NominalScopedType:
                return this->parseNominalTemplateType(BSQON_AST_NODE_AS(NominalScopedType, node));
            case BSQON_AST_TAG_TupleType:
                return this->parseTupleType(BSQON_AST_NODE_AS(TupleType, node));
            case BSQON_AST_TAG_RecordType:
                return this->parseRecordType(BSQON_AST_NODE_AS(RecordType, node));
            case BSQON_AST_TAG_ConjunctionType:
                return this->parseConceptSetType(BSQON_AST_NODE_AS(ConjunctionType, node));
            case BSQON_AST_TAG_UnionType:
                return this->parseUnionType(BSQON_AST_NODE_AS(UnionType, node));
            default: {
                assert(false);
                return UnresolvedType::singleton;
            }
        }
    }

    const Type* Parser::parseTypeRoot(const BSQON_AST_Node* node)
    {
        auto ftype = this->parseType(node);

        if(this->assembly->typerefs.find(ftype->tkey) == this->assembly->typerefs.end()) {
            this->addError("Could not resolve type " + ftype->tkey, Parser::convertSrcPos(node->pos));
            return UnresolvedType::singleton;
        }

        return ftype;
    }

    Value* Parser::parseNone(const PrimitiveType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_NoneValue) {
            this->addError("Expected none", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        return new NoneValue(t, Parser::convertSrcPos(node->pos));
    }
    
    Value* Parser::parseNothing(const PrimitiveType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_NothingValue) {
            this->addError("Expected nothing", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        return new NothingValue(t, Parser::convertSrcPos(node->pos));
    }

    Value* Parser::parseBool(const PrimitiveType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_TrueValue && node->tag != BSQON_AST_TAG_FalseValue) {
            this->addError("Expected Boolean literal", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        return new BoolValue(t, Parser::convertSrcPos(node->pos), node->tag == BSQON_AST_TAG_TrueValue);
    }

    Value* Parser::parseNat(const PrimitiveType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_NatValue) {
            this->addError("Expected Nat literal", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        int64_t vv;
        std::string nv = std::string(BSQON_AST_NODE_AS(LiteralStandardValue, node)->data);
        nv.pop_back(); //remove the trailing 'n'

        if(!Parser::isValidNat(nv, vv)) {
            this->addError("Nat value outside of valid range", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        return new NatNumberValue(t, Parser::convertSrcPos(node->pos), (uint64_t)vv);
    }

    Value* Parser::parseInt(const PrimitiveType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_IntValue) {
            this->addError("Expected Int literal", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        int64_t vv;
        std::string nv = std::string(BSQON_AST_NODE_AS(LiteralStandardValue, node)->data);
        nv.pop_back(); //remove the trailing 'i'

        if(!Parser::isValidInt(nv, vv)) {
            this->addError("Int value outside of valid range", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        return new IntNumberValue(t, Parser::convertSrcPos(node->pos), vv);
    }

    Value* Parser::parseBigNat(const PrimitiveType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_BigNatValue) {
            this->addError("Expected BigNat literal", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        std::string nv = std::string(BSQON_AST_NODE_AS(LiteralStandardValue, node)->data);
        nv.pop_back(); //remove the trailing 'N'

        if(nv.front() == '+') {
            nv = nv.substr(1);
        }

        boost::multiprecision::mpz_int pv(nv);

        return new BigNatNumberValue(t, Parser::convertSrcPos(node->pos), pv);
    }

    Value* Parser::parseBigInt(const PrimitiveType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_BigIntValue) {
            this->addError("Expected BigInt literal", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        std::string nv = std::string(BSQON_AST_NODE_AS(LiteralStandardValue, node)->data);
        nv.pop_back(); //remove the trailing 'I'

        if(nv.front() == '+') {
            nv = nv.substr(1);
        }

        boost::multiprecision::mpz_int pv(nv);

        return new BigIntNumberValue(t, Parser::convertSrcPos(node->pos), pv);
    }

    Value* Parser::parseRational(const PrimitiveType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_RationalValue) {
            this->addError("Expected Rational literal", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        std::string nv = std::string(BSQON_AST_NODE_AS(LiteralStandardValue, node)->data);
        nv.pop_back(); //remove the trailing 'R'

        if(nv.front() == '+') {
            nv = nv.substr(1);
        }

        if(nv.find('/') == std::string::npos) {
            boost::multiprecision::mpz_int rv(nv);

            return new RationalNumberValue(t, Parser::convertSrcPos(node->pos), boost::multiprecision::mpq_rational(rv, 1));
        }
        else {
            boost::multiprecision::mpz_int numerator(nv.substr(0, nv.find('/')));
            auto denominator = nv.substr(nv.find('/') + 1);

            int64_t denomv;
            if(!Parser::isValidNat(denominator, denomv)) {
                this->addError("Rational numerator outside of valid range", Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            return new RationalNumberValue(t, Parser::convertSrcPos(node->pos), boost::multiprecision::mpq_rational(numerator, (uint64_t)denomv));
        }
    }

    Value* Parser::parseFloat(const PrimitiveType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_FloatValue) {
            this->addError("Expected Float literal", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        double vv;
        std::string nv = std::string(BSQON_AST_NODE_AS(LiteralStandardValue, node)->data);
        nv.pop_back(); //remove the trailing 'f'

        if(!Parser::isValidFloat(nv, vv)) {
            this->addError("Invalid Float literal", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        return new FloatNumberValue(t, Parser::convertSrcPos(node->pos), vv);
    }

    Value* Parser::parseDecimal(const PrimitiveType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_DecimalValue) {
            this->addError("Expected Decimal literal", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        std::string nv = std::string(BSQON_AST_NODE_AS(LiteralStandardValue, node)->data);
        nv.pop_back(); //remove the trailing 'd'

        boost::multiprecision::cpp_dec_float_50 pv(nv);
        return new DecimalNumberValue(t, Parser::convertSrcPos(node->pos), pv);
    }

    Value* Parser::parseDecimalDegree(const PrimitiveType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_DecimalDegreeValue) {
            this->addError("Expected Decimal Degree literal", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        std::string nv = std::string(BSQON_AST_NODE_AS(LiteralStandardValue, node)->data);
        nv.pop_back(); //remove the trailing 'dd'
        nv.pop_back();
        
        boost::multiprecision::cpp_dec_float_50 pv(nv);
        return new DecimalDegreeNumberValue(t, Parser::convertSrcPos(node->pos), pv);
    }
    
    Value* Parser::parseLatLong(const PrimitiveType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_LatLongValue) {
            this->addError("Expected LatLong literal", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        std::string nv = std::string(BSQON_AST_NODE_AS(LiteralStandardValue, node)->data);

        auto split = nv.find("lat") + 3;

        auto latstr = nv.substr(0, split);
        if(latstr.front() == '+') {
            latstr = latstr.substr(1);
        }

        auto longstr = nv.substr(split);
        if(longstr.front() == '+') {
            longstr = longstr.substr(1);
        }

        boost::multiprecision::cpp_dec_float_50 latv(latstr.substr(0, latstr.size() - 3));
        boost::multiprecision::cpp_dec_float_50 longv(longstr.substr(0, longstr.size() - 4));

        return new LatLongValue(t, Parser::convertSrcPos(node->pos), latv, longv);
    }
    
    Value* Parser::parseComplex(const PrimitiveType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_ComplexValue) {
            this->addError("Expected Complex number literal", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        std::string nv = std::string(BSQON_AST_NODE_AS(LiteralStandardValue, node)->data);
        nv.pop_back(); //remove the trailing 'i'

        auto split = std::min(nv.find('+'), nv.find("-"));

        auto realstr = nv.substr(0, split);
        double realval;
        if(!Parser::isValidFloat(realstr, realval)) {
            this->addError("Invalid Complex literal", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        auto imagstr = nv.substr(split);
        double imagval;
        if(!Parser::isValidFloat(imagstr, imagval)) {
            this->addError("Invalid Complex literal", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        return new ComplexNumberValue(t, Parser::convertSrcPos(node->pos), realval, imagval);
    }

    Value* Parser::parseString(const PrimitiveType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_StringValue) {
            this->addError("Expected String literal", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        auto bstr = BSQON_AST_NODE_AS(LiteralStringValue, node)->data;
        StringValue* svopt = StringValue::createFromParse(t, Parser::convertSrcPos(node->pos), bstr->bytes, bstr->len);

        if(svopt == nullptr) {
            this->addError("Invalid escape characters in string", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        return svopt;
    }

    Value* Parser::parseASCIIString(const PrimitiveType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_ASCIIStringValue) {
            this->addError("Expected ASCIIString literal", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        auto bstr = BSQON_AST_NODE_AS(LiteralStringValue, node)->data;
        ASCIIStringValue* svopt = ASCIIStringValue::createFromParse(t, Parser::convertSrcPos(node->pos), bstr->bytes, bstr->len);

        if(svopt == nullptr) {
            this->addError("Invalid escape characters in ascii-string", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        return svopt;
    }

    Value* Parser::parseStringSlice(const PrimitiveType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_StringSliceValue) {
            this->addError("Expected StringView literal", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        auto sl = BSQON_AST_NODE_AS(StringSliceValue, node);
        auto sval = this->parseValue(this->assembly->resolveType("String"), sl->data);
        if(sval->vtype->tkey != "String") {
            this->addError("Invalid type in StringView literal", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        {
            ContainerParseStackEntryManager mgr(ContainerParseStackEntryTag::Slice, this, static_cast<StringValue*>(sval)->sv.size());

            auto startval = sl->start != NULL ? this->parseValue(this->assembly->resolveType("Int"), sl->start) : nullptr;
            auto endval = sl->end != NULL ? this->parseValue(this->assembly->resolveType("Int"), sl->end) : nullptr;
    
            if((startval != nullptr && startval->vtype->tkey != "Int") || (endval != nullptr && endval->vtype->tkey != "Int")) {
                this->addError("Invalid type in StringView literal", Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            auto sstr = &static_cast<StringValue*>(sval)->sv;
            int64_t start = startval != nullptr ? static_cast<IntNumberValue*>(startval)->cnv : 0;
            int64_t end = endval != nullptr ? static_cast<IntNumberValue*>(endval)->cnv : (int64_t)sstr->size();

            //convert to 0 based front indexing -- check bounds
            if(start < 0) {
                start = sstr->size() + start;
            }
            if(end < 0) {
                end = sstr->size() + end;
            }
        
            if(start < 0 || start > end || end > (int64_t)sstr->size()) {
                this->addError("Invalid bounds in StringView literal", Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            return new StringSliceValue(t, Parser::convertSrcPos(node->pos), sstr, start, end);
        }
    }

    Value* Parser::parseASCIIStringSlice(const PrimitiveType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_StringSliceValue) {
            this->addError("Expected StringView literal", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        auto sl = BSQON_AST_NODE_AS(StringSliceValue, node);
        auto sval = this->parseValue(this->assembly->resolveType("ASCIIString"), sl->data);
        if(sval->vtype->tkey != "ASCIIString") {
            this->addError("Invalid type in ASCIIStringView literal", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        {
            ContainerParseStackEntryManager mgr(ContainerParseStackEntryTag::Slice, this, static_cast<ASCIIStringValue*>(sval)->sv.size());

            auto startval = sl->start != NULL ? this->parseValue(this->assembly->resolveType("Int"), sl->start) : nullptr;
            auto endval = sl->end != NULL ? this->parseValue(this->assembly->resolveType("Int"), sl->end) : nullptr;

            if((startval != nullptr && startval->vtype->tkey != "Int") || (endval != nullptr && endval->vtype->tkey != "Int")) {
                this->addError("Invalid type in ASCIIStringView literal", Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            auto sstr = &static_cast<ASCIIStringValue*>(sval)->sv;
            int64_t start = startval != nullptr ? static_cast<IntNumberValue*>(startval)->cnv : 0;
            int64_t end = endval != nullptr ? static_cast<IntNumberValue*>(endval)->cnv : (int64_t)sstr->size();

            //convert to 0 based front indexing -- check bounds
            if(start < 0) {
                start = sstr->size() + start;
            }
            if(end < 0) {
                end = sstr->size() + end;
            }
        
            if(start < 0 || start > end || end > (int64_t)sstr->size()) {
                this->addError("Invalid bounds in StringView literal", Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            return new ASCIIStringSliceValue(t, Parser::convertSrcPos(node->pos), sstr, start, end);
        }
    }

    Value* Parser::parseByteBuffer(const PrimitiveType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_ByteBufferValue) {
            this->addError("Expected ByteBuffer literal", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        auto buf = BSQON_AST_NODE_AS(LiteralStandardValue, node)->data;
        ByteBufferValue* bvopt = ByteBufferValue::createFromParse(t, Parser::convertSrcPos(node->pos), buf);

        if(bvopt == nullptr) {
            this->addError("Truncated byte value in buffer", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        return bvopt;
    }

    Value* Parser::parseUUIDv4(const PrimitiveType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_UUIDv4Value) {
            this->addError("Expected UUIDv4 literal", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        auto uuid = std::string(BSQON_AST_NODE_AS(LiteralStandardValue, node)->data);
        if(!uuid.starts_with("uuid4")) {
            this->addError("Invalid UUIDv4 value", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        return new UUIDv4Value(t, Parser::convertSrcPos(node->pos), uuid.substr(6, uuid.size() - 7));
    }

    Value* Parser::parseUUIDv7(const PrimitiveType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_UUIDv7Value) {
            this->addError("Expected UUIDv7 literal", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        auto uuid = std::string(BSQON_AST_NODE_AS(LiteralStandardValue, node)->data);
        if(!uuid.starts_with("uuid7")) {
            this->addError("Invalid UUIDv7 value", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        return new UUIDv7Value(t, Parser::convertSrcPos(node->pos), uuid.substr(6, uuid.size() - 7));
    }

    Value* Parser::parseSHAHashcode(const PrimitiveType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_SHAHashcodeValue) {
            this->addError("Expected SHAContentHash literal", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        auto hash = std::string(BSQON_AST_NODE_AS(LiteralStandardValue, node)->data);
        return new SHAContentHashValue(t, Parser::convertSrcPos(node->pos), hash.substr(4, hash.size() - 5));
    }

    Value* Parser::parseDateTime(const PrimitiveType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_DateTimeValue) {
            this->addError("Expected DateTime literal", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        auto dstr = std::string(BSQON_AST_NODE_AS(LiteralStandardValue, node)->data);
        auto tstr = dstr.substr(dstr.find('T') + 1);
        auto tzstr = dstr.substr(dstr.find('@'));

        uint16_t year;
        uint8_t month, day;
        uint8_t hour, minute, second;
        const char* tz;

        if(!Parser::processDateInfo(dstr, year, month, day) || !Parser::processTimeInfo(tstr, hour, minute, second) || !Parser::processTZInfo(tzstr, &tz)) {
            this->addError("Invalid component in DateTime value", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        DateTime dv = {year, month, day, hour, minute, second, (char*)tz};
        return new DateTimeValue(t, Parser::convertSrcPos(node->pos), dv);
    }

    Value* Parser::parseUTCDateTime(const PrimitiveType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_UTCDateTimeValue) {
            this->addError("Expected UTCDateTime literal", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        auto dstr = std::string(BSQON_AST_NODE_AS(LiteralStandardValue, node)->data);
        auto tstr = dstr.substr(dstr.find('T') + 1);
        auto tzstr = dstr.substr(dstr.find('@'));

        uint16_t year;
        uint8_t month, day;
        uint8_t hour, minute, second;

        if(!Parser::processDateInfo(dstr, year, month, day) || !Parser::processTimeInfo(tstr, hour, minute, second)) {
            this->addError("Invalid component in UTCDateTime value", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        UTCDateTime dv = {year, month, day, hour, minute, second};
        return new UTCDateTimeValue(t, Parser::convertSrcPos(node->pos), dv);
    }

    Value* Parser::parsePlainDate(const PrimitiveType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_PlainDateValue) {
            this->addError("Expected PlainDate literal", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        auto dstr = std::string(BSQON_AST_NODE_AS(LiteralStandardValue, node)->data);

        uint16_t year;
        uint8_t month, day;

        if(!Parser::processDateInfo(dstr, year, month, day)) {
            this->addError("Invalid component in PlainDate literal", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        PlainDate dv = {year, month, day};
        return new PlainDateValue(t, Parser::convertSrcPos(node->pos), dv);
    }

    Value* Parser::parsePlainTime(const PrimitiveType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_PlainTimeValue) {
            this->addError("Expected PlainTime literal", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        auto tstr = std::string(BSQON_AST_NODE_AS(LiteralStandardValue, node)->data);

        uint8_t hour, minute, second;

        if(!Parser::processTimeInfo(tstr, hour, minute, second)) {
            this->addError("Invalid component in PlainTime literal", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        PlainTime dv = {hour, minute, second};
        return new PlainTimeValue(t, Parser::convertSrcPos(node->pos), dv);
    }

    Value* Parser::parseTickTime(const PrimitiveType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_TickTimeValue) {
            this->addError("Expected TickTime literal", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        uint64_t vv;
        std::string nv = std::string(BSQON_AST_NODE_AS(LiteralStandardValue, node)->data);
        nv.pop_back(); //remove the trailing 't'

        if(!Parser::isValidWCTime(nv, vv)) {
            this->addError("TickTime value outside of valid range", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        return new TickTimeValue(t, Parser::convertSrcPos(node->pos), vv);
    }

    Value* Parser::parseLogicalTime(const PrimitiveType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_LogicalTimeValue) {
            this->addError("Expected LogicalTime literal", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        uint64_t vv;
        std::string nv = std::string(BSQON_AST_NODE_AS(LiteralStandardValue, node)->data);
        nv.pop_back(); //remove the trailing 'l'

        if(!Parser::isValidWCTime(nv, vv)) {
            this->addError("LogicalTime value outside of valid range", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        return new LogicalTimeValue(t, Parser::convertSrcPos(node->pos), vv);
    }

    Value* Parser::parseISOTimeStamp(const PrimitiveType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_TimestampValue) {
            this->addError("Expected ISOTimeStamp literal", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        auto dstr = std::string(BSQON_AST_NODE_AS(LiteralStandardValue, node)->data);
        auto tstr = dstr.substr(dstr.find('T') + 1);
        
        uint16_t year;
        uint8_t month, day;
        uint8_t hour, minute, second;
        uint16_t millis;
        
        if(!Parser::processDateInfo(dstr, year, month, day) || !Parser::processTimeInfo(tstr, hour, minute, second) || !Parser::processMillisInfo(tstr, millis)) {
            this->addError("Invalid component in ISOTimeStamp value", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        ISOTimeStamp dv = {year, month, day, hour, minute, second, millis};
        return new ISOTimeStampValue(t, Parser::convertSrcPos(node->pos), dv);
    }

    Value* Parser::parseDeltaDateTime(const PrimitiveType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_DeltaDateTimeValue) {
            this->addError("Expected DeltaDateTime literal", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        auto dstr = std::string(BSQON_AST_NODE_AS(LiteralStandardValue, node)->data);
        char8_t sign = dstr.front();
        dstr = dstr.substr(1);

        auto tstr = dstr.substr(dstr.find('T') + 1);

        uint16_t year, month;
        uint32_t day, hour, minute, second;

        if(!Parser::processDeltaDateInfo(dstr, year, month, day) || !Parser::processDeltaTimeInfo(tstr, hour, minute, second)) {
            this->addError("Invalid component in DeltaDateTime value", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        DeltaDateTime dv = {year, month, day, hour, minute, second, sign};
        return new DeltaDateTimeValue(t, Parser::convertSrcPos(node->pos), dv);
    }

    Value* Parser::parseDeltaPlainDate(const PrimitiveType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_DeltaPlainDateValue) {
            this->addError("Expected DeltaPlainDate literal", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        auto dstr = std::string(BSQON_AST_NODE_AS(LiteralStandardValue, node)->data);
        char8_t sign = dstr.front();
        dstr = dstr.substr(1);

        uint16_t year, month;
        uint32_t day;

        if(!Parser::processDeltaDateInfo(dstr, year, month, day)) {
            this->addError("Invalid component in DeltaPlainDate value", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        DeltaPlainDate dv = {year, month, day, sign};
        return new DeltaPlainDateValue(t, Parser::convertSrcPos(node->pos), dv);
    }

    Value* Parser::parseDeltaPlainTime(const PrimitiveType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_DeltaPlainTimeValue) {
            this->addError("Expected DeltaPlainTime literal", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        auto tstr = std::string(BSQON_AST_NODE_AS(LiteralStandardValue, node)->data);
        char8_t sign = tstr.front();
        tstr = tstr.substr(1);

        uint32_t hour, minute, second;

        if(!Parser::processDeltaTimeInfo(tstr, hour, minute, second)) {
            this->addError("Invalid component in DeltaPlainTime value", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        DeltaPlainTime dv = {hour, minute, second, sign};
        return new DeltaPlainTimeValue(t, Parser::convertSrcPos(node->pos), dv);
    }

    Value* Parser::parseDeltaISOTimeStamp(const PrimitiveType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_DeltaISOTimeStampValue) {
            this->addError("Expected DeltaISOTimestamp literal", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        auto dstr = std::string(BSQON_AST_NODE_AS(LiteralStandardValue, node)->data);
        char8_t sign = dstr.front();
        dstr = dstr.substr(1);

        auto tstr = dstr.substr(dstr.find('T') + 1);
        
        uint16_t year, month;
        uint32_t day, hour, minute, second;
        uint32_t millis;
        
        if(!Parser::processDeltaDateInfo(dstr, year, month, day) || !Parser::processDeltaTimeInfo(tstr, hour, minute, second) || !Parser::processDeltaMillisInfo(tstr, millis)) {
            this->addError("Invalid component in DeltaISOTimestamp value", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        DeltaISOTimeStamp dv = {year, month, day, hour, minute, second, millis, sign};
        return new DeltaISOTimeStampValue(t, Parser::convertSrcPos(node->pos), dv);
    }

    Value* Parser::parseDeltaSeconds(const PrimitiveType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_DeltaSecondsValue) {
            this->addError("Expected DeltaSeconds literal", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        double vv;
        std::string nv = std::string(BSQON_AST_NODE_AS(LiteralStandardValue, node)->data);
        nv.pop_back(); //remove the trailing 's'
        nv.pop_back(); //remove the trailing 'd'

        if(!Parser::isValidFloat(nv, vv)) {
            this->addError("DeltaSeconds value outside of valid range", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        return new DeltaSecondsValue(t, Parser::convertSrcPos(node->pos), vv);
    }

    Value* Parser::parseDeltaTick(const PrimitiveType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_DeltaTickValue) {
            this->addError("Expected DeltaTick literal", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        int64_t vv;
        std::string nv = std::string(BSQON_AST_NODE_AS(LiteralStandardValue, node)->data);
        nv.pop_back(); //remove the trailing 't'
        nv.pop_back(); //remove the trailing 'd'

        if(!Parser::isValidWCDelta(nv, vv)) {
            this->addError("DeltaTick value outside of valid range", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        return new DeltaTickValue(t, Parser::convertSrcPos(node->pos), vv);
    }

    Value* Parser::parseDeltaLogical(const PrimitiveType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_DeltaLogicalValue) {
            this->addError("Expected DeltaLogical literal", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        int64_t vv;
        std::string nv = std::string(BSQON_AST_NODE_AS(LiteralStandardValue, node)->data);
        nv.pop_back(); //remove the trailing 'l'
        nv.pop_back(); //remove the trailing 'd'

        if(!Parser::isValidWCDelta(nv, vv)) {
            this->addError("DeltaLogical value outside of valid range", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        return new DeltaLogicalValue(t, Parser::convertSrcPos(node->pos), vv);
    }

    Value* Parser::parseUnicodeRegex(const PrimitiveType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_RegexValue) {
            this->addError("Expected Regex literal", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        auto strnode = BSQON_AST_NODE_AS(LiteralStringValue, node);
        if(strnode->data->bytes[strnode->data->len - 1] != '/') {
            this->addError("Expected Unicode Regex value", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        auto rparse = UnicodeRegexValue::createFromParse(t, Parser::convertSrcPos(node->pos), strnode->data->bytes, strnode->data->len);
        if(rparse == nullptr) {
            this->addError("Invalid Regex value", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }
        
        return rparse; 
    }

    Value* Parser::parseASCIIRegex(const PrimitiveType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_RegexValue) {
            this->addError("Expected Regex literal", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        auto strnode = BSQON_AST_NODE_AS(LiteralStringValue, node);
        if(strnode->data->bytes[strnode->data->len - 1] != 'a') {
            this->addError("Expected ASCIIRegex value", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        auto rparse = ASCIIRegexValue::createFromParse(t, Parser::convertSrcPos(node->pos), strnode->data->bytes, strnode->data->len);
        if(rparse == nullptr) {
            this->addError("Invalid Regex value", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }
        
        return rparse;
    }

    Value* Parser::parsePathRegex(const PrimitiveType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_RegexValue) {
            this->addError("Expected Regex literal", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        auto strnode = BSQON_AST_NODE_AS(LiteralStringValue, node);
        if(strnode->data->bytes[strnode->data->len - 1] != 'p') {
            this->addError("Expected PathRegex value", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        auto rparse = PathRegexValue::createFromParse(t, Parser::convertSrcPos(node->pos), strnode->data->bytes, strnode->data->len);
        if(rparse == nullptr) {
            this->addError("Invalid Regex value", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }
        
        return rparse;
    }

    Value* Parser::parseStringOf(const StringOfType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_StringOfValue && node->tag != BSQON_AST_TAG_StringValue) {
            this->addError("Expected StringOf literal", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        const brex::Regex* vre = this->assembly->bsqonRegexValidators.at(t->oftype);
        if(!this->reunicodebinds.contains(t->oftype)) {
            std::vector<brex::RegexCompileError> compileerror;
            auto uexecutor = brex::RegexCompiler::compileUnicodeRegexToExecutor(vre, this->namedRegexes, {}, false, (brex::NameResolverState*)this->assembly, Parser::resolveTypeForRegexLookup, compileerror);

            if(compileerror.size() > 0) {
                this->addError("Invalid Regex value: " + std::string(compileerror.front().msg.cbegin(), compileerror.front().msg.cend()), Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }
            this->reunicodebinds[t->oftype] = uexecutor;
        }
        brex::UnicodeRegexExecutor* ure = this->reunicodebinds.at(t->oftype);

        struct ByteString* sstr;
        if(node->tag == BSQON_AST_TAG_StringValue) {
            sstr = BSQON_AST_NODE_AS(LiteralStringValue, node)->data;
        }
        else {
            sstr = BSQON_AST_NODE_AS(StringOfValue, node)->data;

            const Type* oftype = this->parseTypeRoot(BSQON_AST_NODE_AS(StringOfValue, node)->type);
            if(oftype->tkey != t->oftype) {
                this->addError("Mismatch between expected StringOf type " + t->oftype + " and given type " + oftype->tkey, Parser::convertSrcPos(node->pos));
            }
        }

        StringOfValue* svopt = StringOfValue::createFromParse(t, Parser::convertSrcPos(node->pos), sstr->bytes, sstr->len, vre, ure);

        if(svopt == nullptr) {
            this->addError("Invalid characters in string (does not validate)", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        return svopt;
    }

    Value* Parser::parseASCIIStringOf(const ASCIIStringOfType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_ASCIIStringOfValue && node->tag != BSQON_AST_TAG_ASCIIStringValue) {
            this->addError("Expected ASCIIStringOf literal", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        const brex::Regex* vre = this->assembly->bsqonRegexValidators.at(t->oftype);
        if(!this->reasciibinds.contains(t->oftype)) {
            std::vector<brex::RegexCompileError> compileerror;
            auto aexecutor = brex::RegexCompiler::compileASCIIRegexToExecutor(vre, this->namedRegexes, {}, false, (brex::NameResolverState*)this->assembly, Parser::resolveTypeForRegexLookup, compileerror);

            if(compileerror.size() > 0) {
                this->addError("Invalid Regex value: " + std::string(compileerror.front().msg.cbegin(), compileerror.front().msg.cend()), Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }
            this->reasciibinds[t->oftype] = aexecutor;
        }
        brex::ASCIIRegexExecutor* are = this->reasciibinds.at(t->oftype);

        struct ByteString* sstr;
        if(node->tag == BSQON_AST_TAG_ASCIIStringValue) {
            sstr = BSQON_AST_NODE_AS(LiteralStringValue, node)->data;
        }
        else {
            sstr = BSQON_AST_NODE_AS(StringOfValue, node)->data;

            const Type* oftype = this->parseTypeRoot(BSQON_AST_NODE_AS(StringOfValue, node)->type);
            if(oftype->tkey != t->oftype) {
                this->addError("Mismatch between expected ASCIIStringOf type " + t->oftype + " and given type " + oftype->tkey, Parser::convertSrcPos(node->pos));
            }
        }

        ASCIIStringOfValue* svopt = ASCIIStringOfValue::createFromParse(t, Parser::convertSrcPos(node->pos), sstr->bytes, sstr->len, vre, are);

        if(svopt == nullptr) {
            this->addError("Invalid characters in string (does not validate)", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        return svopt;
    }

    Value* Parser::parsePathNaked(const PathType* t, SourcePos spos, const BSQON_AST_NODE(LiteralStringValue)* node)
    {
        if(*node->data->bytes != '\\') {
            this->addError("Expected Path literal", spos);
            return new ErrorValue(t, spos);
        }

        //const BSQPath* vpath = this->assembly->pthvalidators.at(t->oftype);
        //
        //TODO: need to add path validation here
        //
        bpath::PathGlob* validator = nullptr;
        this->addError("TODO: path validators!!!", spos);

        PathValue* popt = PathValue::createFromParse(t, spos, node->data->bytes, node->data->len, validator);

        if(popt == nullptr) {
            this->addError("Invalid characters in path (does not validate)", spos);
            return new ErrorValue(t, spos);
        }

        return popt;
    }

    Value* Parser::parsePath(const PathType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_PathValue) {
            this->addError("Expected Path literal", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        auto pnode = BSQON_AST_NODE_AS(PathValue, node);
        if(pnode->data->tag != BSQON_AST_TAG_NakedPathValue) {
            this->addError("Expected Path literal", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        auto ptype = this->parseTypeRoot(pnode->type);
        if(ptype->tkey != t->tkey) {
            this->addError("Mismatch between expected Path type " + t->oftype + " and given type " + ptype->tkey, Parser::convertSrcPos(node->pos));
        }

        auto pstrnode = BSQON_AST_NODE_AS(LiteralStringValue, pnode->data);
        return this->parsePathNaked(t, Parser::convertSrcPos(node->pos), pstrnode);
    }

    Value* Parser::parsePathFragmentNaked(const PathFragmentType* t, SourcePos spos, const BSQON_AST_NODE(LiteralStringValue)* node)
    {
        if(*node->data->bytes != 'g') {
            this->addError("Expected PathFragment literal", spos);
            return new ErrorValue(t, spos);
        }

        //const BSQPath* vpath = this->assembly->pthvalidators.at(t->oftype);
        //
        //TODO: need to add path validation here
        //
        bpath::PathGlob* validator = nullptr;
        this->addError("TODO: path validators!!!", spos);

        PathFragmentValue* popt = PathFragmentValue::createFromParse(t, spos, node->data->bytes, node->data->len, validator);

        if(popt == nullptr) {
            this->addError("Invalid characters in path (does not validate)", spos);
            return new ErrorValue(t, spos);
        }

        return popt;
    }

    Value* Parser::parsePathFragment(const PathFragmentType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_PathValue) {
            this->addError("Expected PathFragment literal", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        auto pnode = BSQON_AST_NODE_AS(PathValue, node);
        if(pnode->data->tag != BSQON_AST_TAG_NakedPathValue) {
            this->addError("Expected PathFragment literal", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        auto ptype = this->parseTypeRoot(pnode->type);
        if(ptype->tkey != t->tkey) {
            this->addError("Mismatch between expected PathFragment type " + t->oftype + " and given type " + ptype->tkey, Parser::convertSrcPos(node->pos));
        }

        auto pstrnode = BSQON_AST_NODE_AS(LiteralStringValue, pnode->data);
        return this->parsePathFragmentNaked(t, Parser::convertSrcPos(node->pos), pstrnode);
    }

    Value* Parser::parsePathGlobNaked(const PathGlobType* t, SourcePos spos, const BSQON_AST_NODE(LiteralStringValue)* node)
    {
        //const BSQPath* vpath = this->assembly->pthvalidators.at(t->oftype);
        //
        //TODO: need to add path validation here
        //
        bpath::PathGlob* validator = nullptr;
        this->addError("TODO: path validators!!!", spos);

        PathGlobValue* popt = PathGlobValue::createFromParse(t, spos, node->data->bytes, node->data->len, validator);

        if(popt == nullptr) {
            this->addError("Invalid characters in pathGlob (does not validate)", spos);
            return new ErrorValue(t, spos);
        }

        return popt;
    }

    Value* Parser::parsePathGlob(const PathGlobType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_PathValue) {
            this->addError("Expected PathGlob literal", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        auto pnode = BSQON_AST_NODE_AS(PathValue, node);
        if(pnode->data->tag != BSQON_AST_TAG_NakedPathValue) {
            this->addError("Expected PathGlob literal", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        auto ptype = this->parseTypeRoot(pnode->type);
        if(ptype->tkey != t->tkey) {
            this->addError("Mismatch between expected PathGlob type " + t->oftype + " and given type " + ptype->tkey, Parser::convertSrcPos(node->pos));
        }

        auto pstrnode = BSQON_AST_NODE_AS(LiteralStringValue, pnode->data);
        return this->parsePathGlobNaked(t, Parser::convertSrcPos(node->pos), pstrnode);
    }

    Value* Parser::parseSomething(const SomethingType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_SomethingConsValue && node->tag != BSQON_AST_TAG_TypedValue) {
            this->addError("Expected Something value", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        if(node->tag == BSQON_AST_TAG_SomethingConsValue) {
            Value* ofval = this->parseValue(this->assembly->resolveType(t->oftype), BSQON_AST_NODE_AS(SpecialConsValue, node)->value);
            return new SomethingValue(t, Parser::convertSrcPos(node->pos), ofval);
        }
        else {
            auto tnode = BSQON_AST_NODE_AS(TypedValue, node);
            if(tnode->type->tag != BSQON_AST_TAG_NominalType || tnode->istagged) {
                this->addError("Expected Something value", Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            auto oftypenode = BSQON_AST_NODE_AS(NominalType, tnode->type);
            if(oftypenode->terms != NULL) {
                const Type* ttype = this->parseTypeRoot(tnode->type);
                if(ttype->tkey != t->tkey) {
                    this->addError("Expected Something value but got type " + ttype->tkey, Parser::convertSrcPos(node->pos));
                    return new ErrorValue(t, Parser::convertSrcPos(node->pos));
                }
            }
        
            if(strcmp(oftypenode->name, "Something") != 0) {
                this->addError("Expected Something value but got type " + std::string(oftypenode->name), Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }
            
            if(tnode->value->tag != BSQON_AST_TAG_BraceValue) {
                this->addError("Expected constructor arg list", Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            std::optional<Value*> ofval = this->processPropertiesForSpecialCons(this->assembly->resolveType(t->oftype), BSQON_AST_NODE_AS(BraceValue, tnode->value));
            if(!ofval.has_value()) {
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            return new SomethingValue(t, Parser::convertSrcPos(node->pos), ofval.value());
        }
    }

    Value* Parser::parseOk(const OkType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_OkConsValue && node->tag != BSQON_AST_TAG_TypedValue) {
            this->addError("Expected Ok value", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        if(node->tag == BSQON_AST_TAG_OkConsValue) {
            Value* ofval = this->parseValue(this->assembly->resolveType(t->ttype), BSQON_AST_NODE_AS(SpecialConsValue, node)->value);
            return new OkValue(t, Parser::convertSrcPos(node->pos), ofval);
        }
        else {
            auto tnode = BSQON_AST_NODE_AS(TypedValue, node);
            if(tnode->type->tag != BSQON_AST_TAG_NominalScopedType || tnode->istagged) {
                this->addError("Expected Ok value", Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            auto oftypenode = BSQON_AST_NODE_AS(NominalScopedType, tnode->type);
            if(oftypenode->root->tag != BSQON_AST_TAG_NominalType) {
                this->addError("Expected Ok value", Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            auto roottype = BSQON_AST_NODE_AS(NominalType, oftypenode->root);
            if(roottype->terms != NULL) {
                const Type* ttype = this->parseTypeRoot(tnode->type);
                if(ttype->tkey != t->tkey) {
                    this->addError("Expected Ok value but got type " + ttype->tkey, Parser::convertSrcPos(node->pos));
                    return new ErrorValue(t, Parser::convertSrcPos(node->pos));
                }
            }

            if(strcmp(roottype->name, "Result") != 0 || strcmp(oftypenode->ext, "Ok") != 0) {
                this->addError("Expected Result::Ok value but got type " + std::string(roottype->name) + "::" + std::string(oftypenode->ext), Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }
            
            if(tnode->value->tag != BSQON_AST_TAG_BraceValue) {
                this->addError("Expected constructor arg list", Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            std::optional<Value*> ofval = this->processPropertiesForSpecialCons(this->assembly->resolveType(t->ttype), BSQON_AST_NODE_AS(BraceValue, tnode->value));
            if(!ofval.has_value()) {
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            return new OkValue(t, Parser::convertSrcPos(node->pos), ofval.value());
        }
    }

    Value* Parser::parseErr(const ErrorType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_ErrConsValue && node->tag != BSQON_AST_TAG_TypedValue) {
            this->addError("Expected Ok value", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        if(node->tag == BSQON_AST_TAG_ErrConsValue) {
            Value* ofval = this->parseValue(this->assembly->resolveType(t->etype), BSQON_AST_NODE_AS(SpecialConsValue, node)->value);
            return new ErrValue(t, Parser::convertSrcPos(node->pos), ofval);
        }
        else {
            auto tnode = BSQON_AST_NODE_AS(TypedValue, node);
            if(tnode->type->tag != BSQON_AST_TAG_NominalScopedType || tnode->istagged) {
                this->addError("Expected Err value", Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            auto oftypenode = BSQON_AST_NODE_AS(NominalScopedType, tnode->type);
            if(oftypenode->root->tag != BSQON_AST_TAG_NominalType) {
                this->addError("Expected Err value", Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            auto roottype = BSQON_AST_NODE_AS(NominalType, oftypenode->root);
            if(roottype->terms != NULL) {
                const Type* ttype = this->parseTypeRoot(tnode->type);
                if(ttype->tkey != t->tkey) {
                    this->addError("Expected Err value but got type " + ttype->tkey, Parser::convertSrcPos(node->pos));
                    return new ErrorValue(t, Parser::convertSrcPos(node->pos));
                }
            }
            
            if(strcmp(roottype->name, "Result") != 0 || strcmp(oftypenode->ext, "Err") != 0) {
                this->addError("Expected Result::Err value but got type " + std::string(roottype->name) + "::" + std::string(oftypenode->ext), Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            if(tnode->value->tag != BSQON_AST_TAG_BraceValue) {
                this->addError("Expected constructor arg list", Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            std::optional<Value*> ofval = this->processPropertiesForSpecialCons(this->assembly->resolveType(t->etype), BSQON_AST_NODE_AS(BraceValue, tnode->value));
            if(!ofval.has_value()) {
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            return new ErrValue(t, Parser::convertSrcPos(node->pos), ofval.value());
        }
    }

    Value* Parser::parseMapEntry(const MapEntryType* t, const BSQON_AST_Node* node, bool implicitkey)
    {
        if(node->tag != BSQON_AST_TAG_TypedValue && node->tag != BSQON_AST_TAG_MapEntryValue) {
            this->addError("Expected MapEntry value", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        if(node->tag == BSQON_AST_TAG_MapEntryValue) {
            auto tnode = BSQON_AST_NODE_AS(MapEntryValue, node);
            auto kv = this->parseValue(this->assembly->resolveType(t->ktype), tnode->key);

            if(implicitkey) {
                this->containerstack.back().kval = kv;
            }

            auto vv = this->parseValue(this->assembly->resolveType(t->vtype), tnode->value);

            if(implicitkey) {
                this->containerstack.back().kval = nullptr;
            }

            return new MapEntryValue(t, Parser::convertSrcPos(node->pos), kv, vv);
        }
        else {
            auto tnode = BSQON_AST_NODE_AS(TypedValue, node);
            if(tnode->type->tag != BSQON_AST_TAG_NominalType || tnode->istagged) {
                this->addError("Expected MapEntry value", Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            auto oftypenode = BSQON_AST_NODE_AS(NominalType, tnode->type);
            if(oftypenode->terms != NULL) {
                const Type* ttype = this->parseTypeRoot(tnode->type);
                if(ttype->tkey != t->tkey) {
                    this->addError("Expected MapEntry value but got type " + ttype->tkey, Parser::convertSrcPos(node->pos));
                    return new ErrorValue(t, Parser::convertSrcPos(node->pos));
                }
            }

            if(strcmp(oftypenode->name, "MapEntry") != 0) {
                this->addError("Expected MapEntry value but got type " + std::string(oftypenode->name), Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            if(tnode->value->tag != BSQON_AST_TAG_BraceValue) {
                this->addError("Expected constructor arg list", Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            auto vv = this->processPropertiesForMapEntry(this->assembly->resolveType(t->ktype), this->assembly->resolveType(t->vtype), BSQON_AST_NODE_AS(BraceValue, tnode->value));
            if(!vv.has_value()) {
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            return new MapEntryValue(t, Parser::convertSrcPos(node->pos), vv.value().first, vv.value().second);
        }
    }
        
    Value* Parser::parseTuple(const TupleType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_BracketValue && node->tag != BSQON_AST_TAG_TypedValue) {
            this->addError("Expected Tuple value", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        if(node->tag == BSQON_AST_TAG_TypedValue) {
            auto tnode = BSQON_AST_NODE_AS(TypedValue, node);
            const Type* ttype = this->parseTypeRoot(tnode->type);
            if(ttype->tkey != t->tkey) {
                this->addError("Expected " + t->tkey + " value but got " + ttype->tkey, Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            if(tnode->value->tag != BSQON_AST_TAG_BracketValue) {
                this->addError("Expected tuple value", Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            node = tnode->value;
        }

        auto vvals = this->processEntriesForTuple(t, BSQON_AST_NODE_AS(BracketValue, node));
        if(!vvals.has_value()) {
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }
            
        return new TupleValue(t, Parser::convertSrcPos(node->pos), std::move(vvals.value()));
    }

    Value* Parser::parseRecord(const RecordType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_BraceValue && node->tag != BSQON_AST_TAG_TypedValue) {
            this->addError("Expected Record value", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        if(node->tag == BSQON_AST_TAG_TypedValue) {
            auto tnode = BSQON_AST_NODE_AS(TypedValue, node);
            const Type* ttype = this->parseTypeRoot(tnode->type);
            if(ttype->tkey != t->tkey) {
                this->addError("Expected " + t->tkey + " value but got " + ttype->tkey, Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            if(tnode->value->tag != BSQON_AST_TAG_BraceValue) {
                this->addError("Expected record value", Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            node = tnode->value;
        }

        auto vvals = this->processPropertiesForRecord(t, BSQON_AST_NODE_AS(BraceValue, node));
        if(!vvals.has_value()) {
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }
        
        std::vector<Value*> rvals(vvals.value().size(), nullptr);
        std::transform(vvals.value().begin(), vvals.value().end(), rvals.begin(), [](const std::pair<std::string, Value*>& pp) { return pp.second; });

        return new RecordValue(t, Parser::convertSrcPos(node->pos), std::move(rvals));
    }

    Value* Parser::parseEnum(const EnumType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_ScopedNameValue) {
            this->addError("Expected Enum value", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        auto snode = BSQON_AST_NODE_AS(ScopedNameValue, node);
        const Type* ttype = this->parseTypeRoot(snode->root);
        if(ttype->tkey != t->tkey) {
            this->addError("Expected " + t->tkey + " value but got " + ttype->tkey, Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        std::string ename(snode->identifier);
        auto eiter = std::find(t->variants.cbegin(), t->variants.cend(), ename);

        if(eiter == t->variants.cend()) {
            this->addError("Name " + ename + " not defined in the enumeration", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        return new EnumValue(t, Parser::convertSrcPos(node->pos), ename, (uint32_t)std::distance(t->variants.cbegin(), eiter));
    }
    
    Value* Parser::parseTypeDecl(const TypedeclType* t, const BSQON_AST_Node* node)
    {
        auto vnode = node;
        if(node->tag == BSQON_AST_TAG_TypedLiteralValue) {
            vnode = BSQON_AST_NODE_AS(TypedLiteralValue, node)->data;
            const Type* tdtype = this->parseTypeRoot(BSQON_AST_NODE_AS(TypedLiteralValue, node)->type);

            if(tdtype->tkey != t->tkey) {
                this->addError("Expected " + t->tkey + " value but got " + tdtype->tkey, Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }
        }
        
        const Type* btype = this->assembly->resolveType(t->basetype);
        if(btype->tkey == "None" || btype->tkey == "Nothing") {
            this->addError("Cannot have literal value of none/nothing", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        Value* bval = nullptr;
        if(node->tag == BSQON_AST_TAG_NakedPathValue) {
            auto npnode = BSQON_AST_NODE_AS(LiteralStringValue, vnode);
            if(*npnode->data->bytes == '\\') {
                bval = this->parsePathNaked(static_cast<const PathType*>(btype), Parser::convertSrcPos(vnode->pos), npnode);
            }
            else if(*npnode->data->bytes == 'f') {
                bval = this->parsePathFragmentNaked(static_cast<const PathFragmentType*>(btype), Parser::convertSrcPos(vnode->pos), npnode);
            }
            else {
                bval = this->parsePathGlobNaked(static_cast<const PathGlobType*>(btype), Parser::convertSrcPos(vnode->pos), npnode);
            }
        }
        else {
            bval = this->parseValue(btype, vnode);

            if(bval->vtype->tkey != t->basetype) {
                this->addError("Incompatible value and typedecl -- got " + bval->vtype->tkey + " but expected " + t->basetype, Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }
        }

        if(!bval->isValidForTypedecl()) {
            this->addError("Value cannot be used in a typedecl " + bval->vtype->tkey, Parser::convertSrcPos(vnode->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        return new TypedeclValue(t, Parser::convertSrcPos(node->pos), bval);
    }

    Value* Parser::parseStdEntity(const StdEntityType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_BraceValue && node->tag != BSQON_AST_TAG_TypedValue) {
            this->addError("Expected Entity value", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        if(node->tag == BSQON_AST_TAG_TypedValue) {
            auto tnode = BSQON_AST_NODE_AS(TypedValue, node);
            const Type* ttype = this->parseTypeRoot(tnode->type);
            if(ttype->tkey != t->tkey) {
                this->addError("Expected " + t->tkey + " value but got " + ttype->tkey, Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            if(tnode->value->tag != BSQON_AST_TAG_BraceValue || tnode->istagged) {
                this->addError("Expected entity value", Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            node = tnode->value;
        }

        auto vvals = this->processPropertiesForEntity(t, BSQON_AST_NODE_AS(BraceValue, node));
        if(!vvals.has_value()) {
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }
        
        return new EntityValue(t, Parser::convertSrcPos(node->pos), std::move(vvals.value()));
    }

    Value* Parser::parseList(const ListType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_BracketValue && node->tag != BSQON_AST_TAG_TypedValue) {
            this->addError("Expected List value", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        {
            ContainerParseStackEntryManager mgr(ContainerParseStackEntryTag::List, this);

            std::vector<Value*> vv;
            if(node->tag == BSQON_AST_TAG_BracketValue) {
                 this->processEntriesForSequence(t, node, vv, true);
            }
            else {
                auto tnode = BSQON_AST_NODE_AS(TypedValue, node);
                if(tnode->type->tag != BSQON_AST_TAG_NominalType || tnode->istagged) {
                    this->addError("Expected List value", Parser::convertSrcPos(node->pos));
                    return new ErrorValue(t, Parser::convertSrcPos(node->pos));
                }

                auto oftypenode = BSQON_AST_NODE_AS(NominalType, tnode->type);
                if(oftypenode->terms != NULL) {
                    const Type* ttype = this->parseTypeRoot(tnode->type);
                    if(ttype->tkey != t->tkey) {
                        this->addError("Expected List value but got type " + ttype->tkey, Parser::convertSrcPos(node->pos));
                        return new ErrorValue(t, Parser::convertSrcPos(node->pos));
                    }
                }

                if(strcmp(oftypenode->name, "List") != 0) {
                    this->addError("Expected List value but got type " + std::string(oftypenode->name), Parser::convertSrcPos(node->pos));
                    return new ErrorValue(t, Parser::convertSrcPos(node->pos));
                }

                if(tnode->value->tag != BSQON_AST_TAG_BraceValue) {
                    this->addError("Expected constructor arg list", Parser::convertSrcPos(node->pos));
                    return new ErrorValue(t, Parser::convertSrcPos(node->pos));
                }

                this->processEntriesForSequence(t, tnode->value, vv, true);
            }

            return new ListValue(t, Parser::convertSrcPos(node->pos), std::move(vv));
        }
    }

    Value* Parser::parseStack(const StackType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_BracketValue && node->tag != BSQON_AST_TAG_TypedValue) {
            this->addError("Expected Stack value", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        std::vector<Value*> vv;
        if(node->tag == BSQON_AST_TAG_BracketValue) {
             this->processEntriesForSequence(t, node, vv, false);
        }
        else {
            auto tnode = BSQON_AST_NODE_AS(TypedValue, node);
            if(tnode->type->tag != BSQON_AST_TAG_NominalType || tnode->istagged) {
                this->addError("Expected Stack value", Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            auto oftypenode = BSQON_AST_NODE_AS(NominalType, tnode->type);
            if(oftypenode->terms != NULL) {
                const Type* ttype = this->parseTypeRoot(tnode->type);
                if(ttype->tkey != t->tkey) {
                    this->addError("Expected Stack value but got type " + ttype->tkey, Parser::convertSrcPos(node->pos));
                    return new ErrorValue(t, Parser::convertSrcPos(node->pos));
                }
            }

            if(strcmp(oftypenode->name, "Stack") != 0) {
                this->addError("Expected Stack value but got type " + std::string(oftypenode->name), Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            if(tnode->value->tag != BSQON_AST_TAG_BraceValue) {
                this->addError("Expected constructor arg list", Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            this->processEntriesForSequence(t, tnode->value, vv, false);
        }

        return new StackValue(t, Parser::convertSrcPos(node->pos), std::move(vv));
    }

    Value* Parser::parseQueue(const QueueType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_BracketValue && node->tag != BSQON_AST_TAG_TypedValue) {
            this->addError("Expected Queue value", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        std::vector<Value*> vv;
        if(node->tag == BSQON_AST_TAG_BracketValue) {
             this->processEntriesForSequence(t, node, vv, false);
        }
        else {
            auto tnode = BSQON_AST_NODE_AS(TypedValue, node);
            if(tnode->type->tag != BSQON_AST_TAG_NominalType || tnode->istagged) {
                this->addError("Expected Queue value", Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            auto oftypenode = BSQON_AST_NODE_AS(NominalType, tnode->type);
            if(oftypenode->terms != NULL) {
                const Type* ttype = this->parseTypeRoot(tnode->type);
                if(ttype->tkey != t->tkey) {
                    this->addError("Expected Queue value but got type " + ttype->tkey, Parser::convertSrcPos(node->pos));
                    return new ErrorValue(t, Parser::convertSrcPos(node->pos));
                }
            }

            if(strcmp(oftypenode->name, "Queue") != 0) {
                this->addError("Expected Queue value but got type " + std::string(oftypenode->name), Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            if(tnode->value->tag != BSQON_AST_TAG_BraceValue) {
                this->addError("Expected constructor arg list", Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            this->processEntriesForSequence(t, tnode->value, vv, false);
        }

        return new QueueValue(t, Parser::convertSrcPos(node->pos), std::move(vv));
    }

    Value* Parser::parseSet(const SetType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_BraceValue && node->tag != BSQON_AST_TAG_TypedValue) {
            this->addError("Expected Set value", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        std::vector<Value*> vv;
        if(node->tag == BSQON_AST_TAG_BraceValue) {
             this->processEntriesForSequence(t, node, vv, false);
        }
        else {
            auto tnode = BSQON_AST_NODE_AS(TypedValue, node);
            if(tnode->type->tag != BSQON_AST_TAG_NominalType || tnode->istagged) {
                this->addError("Expected Set value", Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            auto oftypenode = BSQON_AST_NODE_AS(NominalType, tnode->type);
            if(oftypenode->terms != NULL) {
                const Type* ttype = this->parseTypeRoot(tnode->type);
                if(ttype->tkey != t->tkey) {
                    this->addError("Expected Set value but got type " + ttype->tkey, Parser::convertSrcPos(node->pos));
                    return new ErrorValue(t, Parser::convertSrcPos(node->pos));
                }
            }

            if(strcmp(oftypenode->name, "Set") != 0) {
                this->addError("Expected Set value but got type " + std::string(oftypenode->name), Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            if(tnode->value->tag != BSQON_AST_TAG_BraceValue) {
                this->addError("Expected constructor arg list", Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            this->processEntriesForSequence(t, tnode->value, vv, false);
        }

        std::sort(vv.begin(), vv.end(), [](const Value* v1, const Value* v2) { return Value::keyCompare(v1, v2) < 0; });
        auto hasdup = std::adjacent_find(vv.cbegin(), vv.cend(), [](const Value* v1, const Value* v2){ return Value::keyCompare(v1, v2) == 0; });
        if(hasdup != vv.cend()) {
            this->addError("Duplicate value in Set", (*(hasdup + 1))->spos);
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        return new SetValue(t, Parser::convertSrcPos(node->pos), std::move(vv));
    }

    Value* Parser::parseMap(const MapType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_BraceValue && node->tag != BSQON_AST_TAG_TypedValue) {
            this->addError("Expected Map value", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        const Type* tkey = this->assembly->resolveType(t->ktype);
        const Type* tvalue = this->assembly->resolveType(t->vtype);

        {
            ContainerParseStackEntryManager mgr(ContainerParseStackEntryTag::Map, this);

            std::vector<MapEntryValue*> vv;
            if(node->tag == BSQON_AST_TAG_BraceValue) {
                this->processEntriesForMap(tkey, tvalue, BSQON_AST_NODE_AS(BraceValue, node), vv);
            }
            else {
                auto tnode = BSQON_AST_NODE_AS(TypedValue, node);
                if(tnode->type->tag != BSQON_AST_TAG_NominalType || tnode->istagged) {
                    this->addError("Expected Map value", Parser::convertSrcPos(node->pos));
                    return new ErrorValue(t, Parser::convertSrcPos(node->pos));
                }

                auto oftypenode = BSQON_AST_NODE_AS(NominalType, tnode->type);
                if(oftypenode->terms != NULL) {
                    const Type* ttype = this->parseTypeRoot(tnode->type);
                    if(ttype->tkey != t->tkey) {
                        this->addError("Expected Map value but got type " + ttype->tkey, Parser::convertSrcPos(node->pos));
                        return new ErrorValue(t, Parser::convertSrcPos(node->pos));
                    }
                }

                if(strcmp(oftypenode->name, "Map") != 0) {
                    this->addError("Expected Map value but got type " + std::string(oftypenode->name), Parser::convertSrcPos(node->pos));
                    return new ErrorValue(t, Parser::convertSrcPos(node->pos));
                }

                if(tnode->value->tag != BSQON_AST_TAG_BraceValue) {
                    this->addError("Expected constructor arg list", Parser::convertSrcPos(node->pos));
                    return new ErrorValue(t, Parser::convertSrcPos(node->pos));
                }

                this->processEntriesForMap(tkey, tvalue, BSQON_AST_NODE_AS(BraceValue, tnode->value), vv);
            }

            std::sort(vv.begin(), vv.end(), [](const MapEntryValue* v1, const MapEntryValue* v2) { return Value::keyCompare(v1->key, v2->key) < 0; });
            auto hasdup = std::adjacent_find(vv.cbegin(), vv.cend(), [](const MapEntryValue* v1, const MapEntryValue* v2){ return Value::keyCompare(v1->key, v2->key) == 0; });
            if(hasdup != vv.cend()) {
                this->addError("Duplicate keys in Map", (*(hasdup + 1))->spos);
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            return new MapValue(t, Parser::convertSrcPos(node->pos), std::move(vv));
        }
    }

    Value* Parser::parseValuePrimitive(const PrimitiveType* t, const BSQON_AST_Node* node)
    {
        auto tk = t->tkey;
        if(tk == "None") {
            return this->parseNone(t, node);
        }
        else if(tk == "Nothing") {
            return this->parseNothing(t, node);
        }
        else if(tk == "Bool") {
            return this->parseBool(t, node);
        }
        else if(tk == "Int") {
            return this->parseInt(t, node);
        }
        else if(tk == "Nat") {
            return this->parseNat(t, node);
        }
        else if(tk == "BigInt") {
            return this->parseBigInt(t, node);
        }
        else if(tk == "BigNat") {
            return this->parseBigNat(t, node);
        }
        else if(tk == "Rational") {
            return this->parseRational(t, node);
        }
        else if(tk == "Float") {
            return this->parseFloat(t, node);
        }
        else if(tk == "Decimal") {
            return this->parseDecimal(t, node);
        }
        else if(tk == "DecimalDegree") {
            return this->parseDecimalDegree(t, node);
        }
        else if(tk == "LatLongCoordinate") {
            return this->parseLatLong(t, node);
        }
        else if(tk == "Complex") {
            return this->parseComplex(t, node);
        }
        else if(tk == "String") {
            return this->parseString(t, node);
        }
        else if(tk == "ASCIIString") {
            return this->parseASCIIString(t, node);
        }
        else if(tk == "StringView") {
            return this->parseStringSlice(t, node);
        }
        else if(tk == "ASCIIStringView") {
            return this->parseASCIIStringSlice(t, node);
        }
        else if(tk == "ByteBuffer") {
            return this->parseByteBuffer(t, node);
        }
        else if(tk == "DateTime") {
            return this->parseDateTime(t, node);
        }
        else if(tk == "UTCDateTime") {
            return this->parseUTCDateTime(t, node);
        }
        else if(tk == "PlainDate") {
            return this->parsePlainDate(t, node);
        }
        else if(tk == "PlainTime") {
            return this->parsePlainTime(t, node);
        }
        else if(tk == "TickTime") {
            return this->parseTickTime(t, node);
        }
        else if(tk == "LogicalTime") {
            return this->parseLogicalTime(t, node);
        }
        else if(tk == "ISOTimeStamp") {
            return this->parseISOTimeStamp(t, node);
        }
        else if(tk == "UUIDv4") {
            return this->parseUUIDv4(t, node);
        }
        else if(tk == "UUIDv7") {
            return this->parseUUIDv7(t, node);
        }
        else if(tk == "SHAContentHash") {
            return this->parseSHAHashcode(t, node);
        }
        else if(tk == "DataTimeDelta") {
            return this->parseDeltaDateTime(t, node);
        }
        else if(tk == "PlainDateDelta") {
            return this->parseDeltaPlainDate(t, node);
        }
        else if(tk == "PlainTimeDelta") {
            return this->parseDeltaPlainTime(t, node);
        }
        else if(tk == "ISOTimestampDelta") {
            return this->parseDeltaISOTimeStamp(t, node);
        }
        else if(tk == "SecondsDelta") {
            return this->parseDeltaSeconds(t, node);
        }
        else if(tk == "TickTimeDelta") {
            return this->parseDeltaTick(t, node);
        }
        else if(tk == "LogicalTimeDelta") {
            return this->parseDeltaLogical(t, node);
        }
        else if(tk == "UnicodeRegex") {
            return this->parseUnicodeRegex(t, node);
        }
        else if(tk == "ASCIIRegex") {
            return this->parseASCIIRegex(t, node);
        }
        else if(tk == "PathRegex") {
            return this->parsePathRegex(t, node);
        }
        else {
            this->addError("Unknown primitive type " + tk, Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }
    }

    Value* Parser::parseValueDirect(const Type* t, const BSQON_AST_Node* node)
    {
        switch(t->tag) {
            case TypeTag::TYPE_TUPLE: {
                return this->parseTuple(static_cast<const TupleType*>(t), node);
            }
            case TypeTag::TYPE_RECORD: {
                return this->parseRecord(static_cast<const RecordType*>(t), node);
            }
            case TypeTag::TYPE_STD_ENTITY: {
                return this->parseStdEntity(static_cast<const StdEntityType*>(t), node);
            }
            case TypeTag::TYPE_ENUM: {
                return this->parseEnum(static_cast<const EnumType*>(t), node);
            }
            case TypeTag::TYPE_TYPE_DECL: {
                return this->parseTypeDecl(static_cast<const TypedeclType*>(t), node);
            }
            case TypeTag::TYPE_STRING_OF: {
                return this->parseStringOf(static_cast<const StringOfType*>(t), node);
            }
            case TypeTag::TYPE_ASCII_STRING_OF: {
                return this->parseASCIIStringOf(static_cast<const ASCIIStringOfType*>(t), node);
            }
            case TypeTag::TYPE_SOMETHING: {
                return this->parseSomething(static_cast<const SomethingType*>(t), node);
            }
            case TypeTag::TYPE_OK: {
                return this->parseOk(static_cast<const OkType*>(t), node);
            }
            case TypeTag::TYPE_ERROR: {
                return this->parseErr(static_cast<const ErrorType*>(t), node);
            }
            case TypeTag::TYPE_PATH: {
                return this->parsePath(static_cast<const PathType*>(t), node);
            }
            case TypeTag::TYPE_PATH_FRAGMENT: {
                return this->parsePathFragment(static_cast<const PathFragmentType*>(t), node);
            }
            case TypeTag::TYPE_PATH_GLOB: {
                return this->parsePathGlob(static_cast<const PathGlobType*>(t), node);
            }
            case TypeTag::TYPE_LIST: {
                return this->parseList(static_cast<const ListType*>(t), node);
            }
            case TypeTag::TYPE_STACK: {
                return this->parseStack(static_cast<const StackType*>(t), node);
            }
            case TypeTag::TYPE_QUEUE: {
                return this->parseQueue(static_cast<const QueueType*>(t), node);
            }
            case TypeTag::TYPE_SET: {
                return this->parseSet(static_cast<const SetType*>(t), node);
            }
            case TypeTag::TYPE_MAP_ENTRY: {
                return this->parseMapEntry(static_cast<const MapEntryType*>(t), node, false);
            }
            case TypeTag::TYPE_MAP: {
                return this->parseMap(static_cast<const MapType*>(t), node);
            }
            default: {
                this->addError("Unknown type " + t->tkey, Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }
        }
    }

    Value* Parser::parseValueConcept(const Type* t, const BSQON_AST_Node* node)
    {
        if(t->tag == TypeTag::TYPE_OPTION) {
            return nullptr;
            const OptionType* otype = static_cast<const OptionType*>(t);
                
            if(node->tag == BSQON_AST_TAG_NothingValue) {
                return this->parseNothing(static_cast<const PrimitiveType*>(this->assembly->resolveType("Nothing")), node);
            }
            else {
                if(node->tag != BSQON_AST_TAG_TypedValue) {
                    this->addError("Values of Option<T> type must be tagged", Parser::convertSrcPos(node->pos));
                    return new ErrorValue(t, Parser::convertSrcPos(node->pos));
                }

                const Type* oftype = this->parseTypeRoot(BSQON_AST_NODE_AS(TypedValue, node)->type);
                if(oftype->isUnresolved()) {
                    return new ErrorValue(t, Parser::convertSrcPos(node->pos));
                }

                if(oftype->tkey == "Nothing") {
                    return this->parseNothing(static_cast<const PrimitiveType*>(this->assembly->resolveType("Nothing")), node);
                }
                else {
                    return this->parseSomething(static_cast<const SomethingType*>(this->assembly->resolveType("Something<" + otype->oftype + ">")), node);    
                }
            }
        }
        else if(t->tag == TypeTag::TYPE_RESULT) {
            const ResultType* rtype = static_cast<const ResultType*>(t);
            const OkType* oktype = static_cast<const OkType*>(this->assembly->resolveType(rtype->tkey + "::Ok"));
            const ErrorType* errtype = static_cast<const ErrorType*>(this->assembly->resolveType(rtype->tkey + "::Err"));

            if(node->tag == BSQON_AST_TAG_OkConsValue) {
                return this->parseOk(oktype, node);
            }
            else if(node->tag == BSQON_AST_TAG_ErrConsValue) {
                return this->parseErr(errtype, node);
            }
            else {
                if(node->tag != BSQON_AST_TAG_TypedValue) {
                    this->addError("Values of Result<T, E> type must be tagged", Parser::convertSrcPos(node->pos));
                    return new ErrorValue(t, Parser::convertSrcPos(node->pos));
                }

                const Type* oftype = this->parseTypeRoot(BSQON_AST_NODE_AS(TypedValue, node)->type);
                if(oftype->isUnresolved()) {
                    return new ErrorValue(t, Parser::convertSrcPos(node->pos));
                }

                if(!this->assembly->checkConcreteSubtype(oftype, t)) {
                    this->addError("Expected result of type " + t->tkey + " but got " + oftype->tkey, Parser::convertSrcPos(node->pos));
                    return new ErrorValue(t, Parser::convertSrcPos(node->pos));
                }

                if(oftype->tag == TypeTag::TYPE_OK) {
                    return this->parseOk(static_cast<const OkType*>(oftype), node);
                }
                else {
                    return this->parseErr(static_cast<const ErrorType*>(errtype), node);
                }
            }
        }
        else if(t->tag == TypeTag::TYPE_STD_CONCEPT || t->tag == TypeTag::TYPE_CONCEPT_SET) {
            if(node->tag != BSQON_AST_TAG_TypedValue) {
                this->addError("Values of Concept type must be tagged", Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            const Type* oftype = this->parseTypeRoot(BSQON_AST_NODE_AS(TypedValue, node)->type);
            if(oftype->isUnresolved()) {
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            if(!this->assembly->checkConcreteSubtype(oftype, t)) {
                this->addError("Expected result of type " + t->tkey + " but got " + oftype->tkey, Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }
            
            return this->parseStdEntity(static_cast<const StdEntityType*>(oftype), node);
        }
        else {
            this->addError("Unknown type " + t->tkey, Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }
    }

    Value* Parser::parseValueSimple(const Type* t, const BSQON_AST_Node* node)
    {
        if (t->tag == TypeTag::TYPE_PRIMITIVE) {
            return this->parseValuePrimitive(static_cast<const PrimitiveType*>(t), node);
        }
        else if ((t->tag == TypeTag::TYPE_STD_CONCEPT) || (t->tag == TypeTag::TYPE_CONCEPT_SET)) {
            return this->parseValueConcept(t, node);
        }
        else {
            return this->parseValueDirect(t, node);
        }
    }

    Value* Parser::parseValueUnion(const UnionType* t, const BSQON_AST_Node* node)
    {
        //everyone has a none special format option
        if(node->tag == BSQON_AST_TAG_NoneValue) {
            if(std::find(t->types.cbegin(), t->types.cend(), "None") == t->types.cend()) {
                this->addError("Expected result of type " + t->tkey + " but got none", Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            return this->parseNone(static_cast<const PrimitiveType*>(this->assembly->resolveType("None")), node);
        }

        //Check for special nonable form as well "T | none"
        if(this->isNoneableParse(t)) {
            //from previous check we know that the type is not none
            return this->parseValueSimple(this->getNoneableRealType(t), node);
        }

        //it isn't none so now we start looking at tags
        auto tk = node->tag;
        Value* vv = nullptr;
        const Type* tt = nullptr;
        if(tk == BSQON_AST_TAG_TypedValue) {
            tt = this->parseTypeRoot(BSQON_AST_NODE_AS(TypedValue, node)->type);
            if(std::find(s_okTypeTaggedTags.cbegin(), s_okTypeTaggedTags.cend(), tt->tag) == s_okTypeTaggedTags.cend()) {
                this->addError("Invalid tagged value " + tt->tkey, Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }
            vv = this->parseValue(tt, BSQON_AST_NODE_AS(TypedValue, node)->value);
        }
        else if(tk == BSQON_AST_TAG_StringOfValue) {
            const Type* oftype = this->parseTypeRoot(BSQON_AST_NODE_AS(StringOfValue, node)->type);
            if(oftype->isUnresolved()) {
                this->addError("Invalid StringOf value " + oftype->tkey, Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            tt = this->assembly->resolveType("StringOf<" + oftype->tkey + ">");
            if(tt->tag != TypeTag::TYPE_STRING_OF) {
                this->addError("Invalid StringOf  value " + tt->tkey, Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }
            vv = this->parseStringOf(static_cast<const StringOfType*>(tt), node);
        }
        else if(tk == BSQON_AST_TAG_ASCIIStringOfValue) {
            const Type* oftype = this->parseTypeRoot(BSQON_AST_NODE_AS(StringOfValue, node)->type);
            if(oftype->isUnresolved()) {
                this->addError("Invalid ASCIIStringOf value " + oftype->tkey, Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            tt = this->assembly->resolveType("ASCIIStringOf<" + oftype->tkey + ">");
            if(tt->tag != TypeTag::TYPE_ASCII_STRING_OF) {
                this->addError("Invalid ASCIIStringOf value " + tt->tkey, Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }
            vv = this->parseASCIIStringOf(static_cast<const ASCIIStringOfType*>(tt), node);
        }
        else if(tk == BSQON_AST_TAG_StringSliceValue) {
            auto ssnode = BSQON_AST_NODE_AS(StringSliceValue, node);
            auto ssbase = this->parseValue(this->assembly->resolveType("Any"), ssnode->data); //really want String | ASCIIString but may not be registed

            if(ssbase->kind != ValueKind::StringValueKind && ssbase->kind != ValueKind::ASCIIStringValueKind) {
                this->addError("Invalid StringSlice value base type", Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            int64_t sstrsize = ssbase->kind == ValueKind::StringValueKind ? static_cast<StringValue*>(ssbase)->sv.size() : static_cast<ASCIIStringValue*>(ssbase)->sv.size();

            {
                ContainerParseStackEntryManager mgr(ContainerParseStackEntryTag::Slice, this, sstrsize);

                auto startval = ssnode->start != NULL ? this->parseValue(this->assembly->resolveType("Int"), ssnode->start) : nullptr;
                auto endval = ssnode->end != NULL ? this->parseValue(this->assembly->resolveType("Int"), ssnode->end) : nullptr;
            
                if((startval != nullptr && startval->vtype->tkey != "Int") || (endval != nullptr && endval->vtype->tkey != "Int")) {
                    this->addError("Invalid type in ASCIIStringView literal", Parser::convertSrcPos(node->pos));
                    return new ErrorValue(t, Parser::convertSrcPos(node->pos));
                }

                int64_t start = startval != nullptr ? static_cast<IntNumberValue*>(startval)->cnv : 0;
                int64_t end = endval != nullptr ? static_cast<IntNumberValue*>(endval)->cnv : sstrsize;

                //convert to 0 based front indexing -- check bounds
                if(start < 0) {
                    start = sstrsize + start;
                }
                if(end < 0) {
                    end = sstrsize + end;
                }
        
                if(start < 0 || start > end || end > sstrsize) {
                    this->addError("Invalid bounds in StringView literal", Parser::convertSrcPos(node->pos));
                    return new ErrorValue(t, Parser::convertSrcPos(node->pos));
                }

                if(ssbase->kind == ValueKind::StringValueKind) {
                    tt = this->assembly->resolveType("StringView");
                    vv = new StringSliceValue(tt, Parser::convertSrcPos(node->pos), &static_cast<StringValue*>(ssbase)->sv, start, end);
                }
                else {
                    tt = this->assembly->resolveType("ASCIIStringView");
                    vv = new ASCIIStringSliceValue(tt, Parser::convertSrcPos(node->pos), &static_cast<ASCIIStringValue*>(ssbase)->sv, start, end);
                }
            }
        }
        else if(tk == BSQON_AST_TAG_RegexValue) {
            auto rnode = BSQON_AST_NODE_AS(LiteralStringValue, node);
            if(*rnode->data->bytes == '\\') {
                tt = this->assembly->resolveType("UnicodeRegex");
                vv = this->parseUnicodeRegex(static_cast<const PrimitiveType*>(tt), node);
            }
            else if (*rnode->data->bytes == 'a') {
                tt = this->assembly->resolveType("ASCIIRegex");
                vv = this->parseASCIIRegex(static_cast<const PrimitiveType*>(tt), node);
            }
            else if (*rnode->data->bytes == 'p') {
                tt = this->assembly->resolveType("PathRegex");
                vv = this->parsePathRegex(static_cast<const PrimitiveType*>(tt), node);
            }
            else {
                this->addError("Invalid regex value", Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }
        }
        else if(tk == BSQON_AST_TAG_PathValue) {
            auto pnode = BSQON_AST_NODE_AS(PathValue, node);

            const Type* oftype = this->parseTypeRoot(pnode->type);
            if(oftype->isUnresolved()) {
                this->addError("Invalid path value " + oftype->tkey, Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            char pfx = *BSQON_AST_NODE_AS(LiteralStringValue, pnode->data)->data->bytes;
            if(pfx == 'g') {
                tt = this->assembly->resolveType("PathGlob<" + oftype->tkey + ">");
                if(tt->tag != TypeTag::TYPE_PATH_GLOB) {
                    this->addError("Invalid PathGlob value " + tt->tkey, Parser::convertSrcPos(node->pos));
                    return new ErrorValue(t, Parser::convertSrcPos(node->pos));
                }
                vv = this->parsePathGlob(static_cast<const PathGlobType*>(tt), node);
            }
            else if(pfx == 'f') {
                tt = this->assembly->resolveType("PathFragment<" + oftype->tkey + ">");
                if(tt->tag != TypeTag::TYPE_PATH_FRAGMENT) {
                    this->addError("Invalid PathFragment value " + tt->tkey, Parser::convertSrcPos(node->pos));
                    return new ErrorValue(t, Parser::convertSrcPos(node->pos));
                }
                vv = this->parsePathFragment(static_cast<const PathFragmentType*>(tt), node);
            }
            else {
                tt = this->assembly->resolveType("Path<" + oftype->tkey + ">");
                if(tt->tag != TypeTag::TYPE_PATH) {
                    this->addError("Invalid Path value " + tt->tkey, Parser::convertSrcPos(node->pos));
                    return new ErrorValue(t, Parser::convertSrcPos(node->pos));
                }
                vv = this->parsePath(static_cast<const PathType*>(tt), node);
            }
        }
        else if(tk == BSQON_AST_TAG_TypedLiteralValue) {
            tt = this->parseTypeRoot(BSQON_AST_NODE_AS(TypedLiteralValue, node)->type);
            if(tt->isUnresolved()) {
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            vv = this->parseValueSimple(tt, BSQON_AST_NODE_AS(TypedLiteralValue, node)->data);
        }
        else {
            if(tk == BSQON_AST_TAG_TrueValue || tk == BSQON_AST_TAG_FalseValue) {
                tt = this->assembly->resolveType("Bool");
                vv = this->parseBool(static_cast<const PrimitiveType*>(tt), node);
            }
            else if(tk == BSQON_AST_TAG_NatValue) {
                tt = this->assembly->resolveType("Nat");
                vv = this->parseNat(static_cast<const PrimitiveType*>(tt), node);
            }
            else if(tk == BSQON_AST_TAG_IntValue) {
                tt = this->assembly->resolveType("Int");
                vv = this->parseInt(static_cast<const PrimitiveType*>(tt), node);
            }
            else if(tk == BSQON_AST_TAG_BigNatValue) {
                tt = this->assembly->resolveType("BigNat");
                vv = this->parseBigNat(static_cast<const PrimitiveType*>(tt), node);
            }
            else if(tk == BSQON_AST_TAG_BigIntValue) {
                tt = this->assembly->resolveType("BigInt");
                vv = this->parseBigInt(static_cast<const PrimitiveType*>(tt), node);
            }
            else if(tk == BSQON_AST_TAG_FloatValue) {
                tt = this->assembly->resolveType("Float");
                vv = this->parseFloat(static_cast<const PrimitiveType*>(tt), node);
            }
            else if(tk == BSQON_AST_TAG_DecimalValue) {
                tt = this->assembly->resolveType("Decimal");
                vv = this->parseDecimal(static_cast<const PrimitiveType*>(tt), node);
            }
            else if(tk == BSQON_AST_TAG_RationalValue) {
                tt = this->assembly->resolveType("Rational");
                vv = this->parseRational(static_cast<const PrimitiveType*>(tt), node);
            }
            else if(tk == BSQON_AST_TAG_DecimalDegreeValue) {
                tt = this->assembly->resolveType("DecimalDegree");
                vv = this->parseDecimalDegree(static_cast<const PrimitiveType*>(tt), node);
            }
            else if(tk == BSQON_AST_TAG_LatLongValue) {
                tt = this->assembly->resolveType("LatLongCoordinate");
                vv = this->parseLatLong(static_cast<const PrimitiveType*>(tt), node);
            }
            else if(tk == BSQON_AST_TAG_ComplexValue) {
                tt = this->assembly->resolveType("Complex");
                vv = this->parseComplex(static_cast<const PrimitiveType*>(tt), node);
            }
            else if(tk == BSQON_AST_TAG_StringValue) {
                tt = this->assembly->resolveType("String");
                vv = this->parseString(static_cast<const PrimitiveType*>(tt), node);
            }
            else if(tk == BSQON_AST_TAG_ASCIIStringValue) {
                tt = this->assembly->resolveType("ASCIIString");
                vv = this->parseASCIIString(static_cast<const PrimitiveType*>(tt), node);
            }
            else if(tk == BSQON_AST_TAG_ByteBufferValue) {
                tt = this->assembly->resolveType("ByteBuffer");
                vv = this->parseByteBuffer(static_cast<const PrimitiveType*>(tt), node);
            }
            else if(tk == BSQON_AST_TAG_DateTimeValue) {
                tt = this->assembly->resolveType("DateTime");
                vv = this->parseDateTime(static_cast<const PrimitiveType*>(tt), node);
            }
            else if(tk == BSQON_AST_TAG_UTCDateTimeValue) {
                tt = this->assembly->resolveType("UTCDateTime");
                vv = this->parseUTCDateTime(static_cast<const PrimitiveType*>(tt), node);
            }
            else if(tk == BSQON_AST_TAG_PlainDateValue) {
                tt = this->assembly->resolveType("PlainDate");
                vv = this->parsePlainDate(static_cast<const PrimitiveType*>(tt), node);
            }
            else if(tk == BSQON_AST_TAG_PlainTimeValue) {
                tt = this->assembly->resolveType("PlainTime");
                vv = this->parsePlainTime(static_cast<const PrimitiveType*>(tt), node);
            }
            else if(tk == BSQON_AST_TAG_TickTimeValue) {
                tt = this->assembly->resolveType("TickTime");
                vv = this->parseTickTime(static_cast<const PrimitiveType*>(tt), node);
            }
            else if(tk == BSQON_AST_TAG_LogicalTimeValue) {
                tt = this->assembly->resolveType("LogicalTime");
                vv = this->parseLogicalTime(static_cast<const PrimitiveType*>(tt), node);
            }
            else if(tk == BSQON_AST_TAG_TimestampValue) {
                tt = this->assembly->resolveType("ISOTimeStamp");
                vv = this->parseISOTimeStamp(static_cast<const PrimitiveType*>(tt), node);
            }
            else if(tk == BSQON_AST_TAG_UUIDv4Value) {
                tt = this->assembly->resolveType("UUIDv4");
                vv = this->parseUUIDv4(static_cast<const PrimitiveType*>(tt), node);
            }
            else if(tk == BSQON_AST_TAG_UUIDv7Value) {
                tt = this->assembly->resolveType("UUIDv7");
                vv = this->parseUUIDv7(static_cast<const PrimitiveType*>(tt), node);
            }
            else if(tk == BSQON_AST_TAG_SHAHashcodeValue) {
                tt = this->assembly->resolveType("SHAContentHash");
                vv = this->parseSHAHashcode(static_cast<const PrimitiveType*>(tt), node);
            }
            else if(tk == BSQON_AST_TAG_DeltaDateTimeValue) {
                tt = this->assembly->resolveType("DataTimeDelta");
                vv = this->parseDeltaDateTime(static_cast<const PrimitiveType*>(tt), node);
            }
            else if(tk == BSQON_AST_TAG_DeltaPlainDateValue) {
                tt = this->assembly->resolveType("PlainDateDelta");
                vv = this->parseDeltaPlainDate(static_cast<const PrimitiveType*>(tt), node);
            }
            else if(tk == BSQON_AST_TAG_DeltaPlainTimeValue) {
                tt = this->assembly->resolveType("PlainTimeDelta");
                vv = this->parseDeltaPlainTime(static_cast<const PrimitiveType*>(tt), node);
            }
            else if(tk == BSQON_AST_TAG_DeltaISOTimeStampValue) {
                tt = this->assembly->resolveType("ISOTimestampDelta");
                vv = this->parseDeltaISOTimeStamp(static_cast<const PrimitiveType*>(tt), node);
            }
            else if(tk == BSQON_AST_TAG_DeltaSecondsValue) {
                tt = this->assembly->resolveType("SecondsDelta");
                vv = this->parseDeltaSeconds(static_cast<const PrimitiveType*>(tt), node);
            }
            else if(tk == BSQON_AST_TAG_DeltaTickValue) {
                tt = this->assembly->resolveType("TickTimeDelta");
                vv = this->parseDeltaTick(static_cast<const PrimitiveType*>(tt), node);
            }
            else if(tk == BSQON_AST_TAG_DeltaLogicalValue) {
                tt = this->assembly->resolveType("LogicalTimeDelta");
                vv = this->parseDeltaLogical(static_cast<const PrimitiveType*>(tt), node);
            }
            else {
                this->addError("Cannot implicitly resolve ", Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }
        }

        if(!this->assembly->checkConcreteSubtype(tt, t)) {
            this->addError("Expected result of type " + t->tkey + " but got " + tt->tkey, Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }
            
        return vv;
    }

    Value* Parser::parseIdentifier(const Type* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_IdentifierValue) {
            this->addError("Expected Identifier value", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        std::string vname = BSQON_AST_NODE_AS(NameValue, node)->data;

        if(vname == "$npos") {
            if(t->tkey != "Int") {
                this->addError("Expected result of type " + t->tkey + " but got Int ($npos)", Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            auto vv = ContainerParseStackEntryManager::resolveNPOS(this->containerstack);
            if(!vv.has_value()) {
                this->addError("Cannot resolve $npos in this context", Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            return new IntNumberValue(t, Parser::convertSrcPos(node->pos), vv.value());
        }
        else if(vname == "$i") {
            if(t->tkey != "Nat") {
                this->addError("Expected result of type " + t->tkey + " but got Nat ($i)", Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            auto vv = ContainerParseStackEntryManager::resolveI(this->containerstack);
            if(!vv.has_value()) {
                this->addError("Cannot resolve $i in this context", Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            return new NatNumberValue(t, Parser::convertSrcPos(node->pos), vv.value());
        }
        else if(vname == "$key") {
            if(t->tkey != "String") {
                this->addError("Expected result of type " + t->tkey + " but got String ($key)", Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            auto vv = ContainerParseStackEntryManager::resolveKey(this->containerstack);
            if(vv == nullptr) {
                this->addError("Cannot resolve $key in this context", Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            return vv;
        }
        else {
            if(!this->vbinds.contains(vname)) {
                this->addError("Unknown let binding " + vname, Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            const Type* oftype = this->vbinds[vname]->vtype;

            if(oftype->isUnresolved()) {
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            if(!this->assembly->checkConcreteSubtype(oftype, t)) {
                this->addError("Expected result of type " + t->tkey + " but got " + oftype->tkey, Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            return this->vbinds[vname];
        }
    }

    Value* Parser::parseScopedName(const Type* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_ScopedNameValue) {
            this->addError("Expected ScopedName value", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        auto snode = BSQON_AST_NODE_AS(ScopedNameValue, node);
        
        auto simplerootname = std::string(snode->root->tag == BSQON_AST_TAG_NominalType ? BSQON_AST_NODE_AS(NominalType, snode->root)->name : "[COMPOUND]");
        if(this->assembly->namespaces.contains(simplerootname)) {
            auto ns = this->assembly->namespaces.at(simplerootname);

            this->addError("TODO: consts in namespace!!! " + ns->ns, Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }
        else {
            const Type* ttype = this->parseTypeRoot(snode->root);
            if(ttype->tag == TypeTag::TYPE_ENUM) {
                return this->parseEnum(static_cast<const EnumType*>(ttype), node);
            }
            else {
                this->addError("TODO: consts in type!!! " + ttype->tkey, Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            } 
        }
    }

    Value* Parser::parseLetIn(const Type* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_LetInValue) {
            this->addError("Expected LetIn value", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        auto lnode = BSQON_AST_NODE_AS(LetInValue, node);

        std::string vname = lnode->vname;
        if(this->vbinds.contains(vname)) {
            this->addError("Duplicate let binding " + vname, Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        const Type* vtype = this->parseTypeRoot(lnode->vtype);
        if(vtype->isUnresolved()) {
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        Value* vvalue = this->parseValue(vtype, lnode->value);
        this->vbinds[vname] = vvalue;

        Value* res = this->parseValue(t, lnode->exp);

        this->vbinds.erase(vname);
        return res;
    }

    Value* Parser::parseAccessIndex(const Type* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_AccessIndexValue) {
            this->addError("Expected access index value", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        auto innode = BSQON_AST_NODE_AS(AccessIndexValue, node);

        auto atype = this->resolveAndCheckType("Tuple", Parser::convertSrcPos(node->pos));
        auto aidxstr = std::string(innode->idx);

        int64_t aindex = 0;
        this->isValidNat(aidxstr, aindex);
        
        Value* lvalue = this->parseValue(atype, innode->value);
        if(lvalue->kind != ValueKind::TupleValueKind) {
            this->addError("Expected a Tuple value (non-symbolic)", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        auto avalue = static_cast<TupleValue*>(lvalue);

        if(aindex < 0 || aindex >= (int64_t)avalue->values.size()) {
            this->addError("Index out of range", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        auto res = avalue->values[aindex];
        if(!this->assembly->checkConcreteSubtype(res->vtype, t)) {
            this->addError("Expected result of type " + t->tkey + " but got " + res->vtype->tkey, Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        return res;
    }

    Value* Parser::parseAccessName(const Type* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_AccessNameValue) {
            this->addError("Expected access index value", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }
        auto nnode = BSQON_AST_NODE_AS(AccessNameValue, node);

        auto atype = this->resolveAndCheckType("Any", Parser::convertSrcPos(node->pos));
        auto anxstr = std::string(nnode->name);
        
        Value* lvalue = this->parseValue(atype, nnode->value);
        if(lvalue->kind != ValueKind::RecordValueKind && lvalue->kind != ValueKind::EntityValueKind) {
            this->addError("Expected record or entity value (non-symbolic)", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        Value* res = nullptr;
        if(lvalue->kind == ValueKind::RecordValueKind) {
            auto avalue = static_cast<RecordValue*>(lvalue);
            auto atype = static_cast<const RecordType*>(avalue->vtype);

            auto eeiter = std::find_if(atype->entries.cbegin(), atype->entries.cend(), [&anxstr](const RecordTypeEntry& entry) { return entry.pname == anxstr; });
            if(eeiter == atype->entries.cend()) {
                this->addError("Unknown property " + anxstr, Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }
 
            res = avalue->values[std::distance(atype->entries.cbegin(), eeiter)];    
        }
        else {
            auto avalue = static_cast<EntityValue*>(lvalue);
            auto atype = static_cast<const StdEntityType*>(avalue->vtype);

            auto eeiter = std::find_if(atype->fields.cbegin(), atype->fields.cend(), [&anxstr](const EntityTypeFieldEntry& entry) { return entry.fname == anxstr; });
            if(eeiter == atype->fields.cend()) {
                this->addError("Unknown field " + anxstr, Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }
 
            res = avalue->fieldvalues[std::distance(atype->fields.cbegin(), eeiter)]; 
        }
        
        if(!this->assembly->checkConcreteSubtype(res->vtype, t)) {
            this->addError("Expected result of type " + t->tkey + " but got " + res->vtype->tkey, Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        return res;
    }

    Value* Parser::parseAccessKey(const Type* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_AccessKeyValue) {
            this->addError("Expected access index value", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        auto knode = BSQON_AST_NODE_AS(AccessKeyValue, node);

        auto atype = this->resolveAndCheckType("Any", Parser::convertSrcPos(node->pos)); 
        Value* lvalue = this->parseValue(atype, knode->value);

        if(lvalue->kind != ValueKind::ListValueKind && lvalue->kind != ValueKind::MapValueKind) {
            this->addError("Expected list or map value (non-symbolic)", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        if(lvalue->kind == ValueKind::ListValueKind) {
            auto avalue = static_cast<ListValue*>(lvalue);

            auto akey = this->parseValue(this->resolveAndCheckType("Nat", Parser::convertSrcPos(node->pos)), knode->kk);
            uint64_t aindex = static_cast<NatNumberValue*>(akey)->cnv;
            if(aindex >= avalue->vals.size()) {
                this->addError("Index out of range", Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            auto res = avalue->vals[aindex];
            if(!this->assembly->checkConcreteSubtype(res->vtype, t)) {
                this->addError("Expected result of type " + t->tkey + " but got " + res->vtype->tkey, Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            return res;
        }
        else {
            auto avalue = static_cast<MapValue*>(lvalue);
            auto atype = static_cast<const MapType*>(avalue->vtype);

            auto akey = this->parseValue(this->resolveAndCheckType(atype->ktype, Parser::convertSrcPos(node->pos)), knode->kk);      
            auto aiter = std::lower_bound(avalue->vals.begin(), avalue->vals.end(), akey, [](const MapEntryValue* v1, const Value* v2) { return Value::keyCompare(v1->key, v2) < 0; });

            if(aiter == avalue->vals.end() || Value::keyCompare((*aiter)->key, akey) != 0) {
                this->addError("Key not found in map", Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            auto res = (*aiter)->val;
            if(!this->assembly->checkConcreteSubtype(res->vtype, t)) {
                this->addError("Expected result of type " + t->tkey + " but got " + res->vtype->tkey, Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            return res;
        }
    }

    Value* Parser::parseEnvAccess(const Type* t, const BSQON_AST_Node* node) 
    {
        if(node->tag != BSQON_AST_TAG_EnvAccessValue) {
            this->addError("Expected env access value", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        auto enode = BSQON_AST_NODE_AS(EnvAccessValue, node);
        auto ename = std::string(enode->data->bytes, enode->data->bytes + enode->data->len);
        auto dtype = enode->type != NULL ? this->parseType(enode->type) : t;
        
        if(!this->envbinds.contains(ename)) {
            this->addError("Unknown env binding " + ename, Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }
        
        auto evalue = this->envbinds[ename];
        if(!this->assembly->checkConcreteSubtype(evalue->vtype, dtype)) {
            this->addError("Expected result of type " + dtype->tkey + " but got " + evalue->vtype->tkey, Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        if(!this->assembly->checkConcreteSubtype(evalue->vtype, t)) {
            this->addError("Expected result of type " + t->tkey + " but got " + evalue->vtype->tkey, Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        return evalue;
    }

    Value* Parser::parseUnspecIdentifierValue(const Type* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_UnspecIdentifierValue) {
            this->addError("Expected unspec identifier value", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        auto unode = BSQON_AST_NODE_AS(NameValue, node);
        auto uname = std::string(unode->data).substr(2);

        return new SymbolValue(t, Parser::convertSrcPos(node->pos), uname, {});
    }

    Value* Parser::parseValue(const Type* t, const BSQON_AST_Node* node)
    {
        if(node->tag == BSQON_AST_TAG_IdentifierValue) {
            return this->parseIdentifier(t, node);
        }
        else if(node->tag == BSQON_AST_TAG_ScopedNameValue) {
            return this->parseScopedName(t, node);
        }
        else if(node->tag == BSQON_AST_TAG_UnspecIdentifierValue) {
            return this->parseUnspecIdentifierValue(t, node);
        }
        else if(node->tag == BSQON_AST_TAG_EnvAccessValue) {
            return this->parseEnvAccess(t, node);
        }
        else if(node->tag == BSQON_AST_TAG_AccessIndexValue) {
            return this->parseAccessIndex(t, node);
        }
        else if(node->tag == BSQON_AST_TAG_AccessNameValue) {
            return this->parseAccessName(t, node);
        }
        else if(node->tag == BSQON_AST_TAG_AccessKeyValue) {
            return this->parseAccessKey(t, node);
        }
        else if(node->tag == BSQON_AST_TAG_LetInValue) {
            return this->parseLetIn(t, node);
        }
        else {
            if (t->tag == TypeTag::TYPE_UNION) {
                return this->parseValueUnion(static_cast<const UnionType*>(t), node);
            }
            else {
                return this->parseValueSimple(t, node);
            }
        }
    }
}
