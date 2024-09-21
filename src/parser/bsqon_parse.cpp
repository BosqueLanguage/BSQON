#include "bsqon_parse.h"

#include <inttypes.h>

namespace bsqon
{
    std::vector<std::string> s_coreTypes = {
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

    bool Parser::processTimeInfo(const std::string& ds, uint8_t& hh, uint8_t& mm, uint8_t& ss, bool allow60sec)
    {
        auto pp = sscanf(ds.c_str(), "%2" SCNu8 ":%2" SCNu8 ":%2" SCNu8, &hh, &mm, &ss);

        return pp == 3 && hh < 24 && mm < 60 && ss < (allow60sec ? 61 : 60);
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

    std::optional<std::vector<Value*>> Parser::processEntriesForEList(const EListType* ttype, const BSQON_AST_NODE(BracketValue)* node)
    {
        std::vector<Value*> elems;

        auto titer = ttype->entries.cbegin();
        for(auto curr = node->values; curr != NULL; curr = curr->next) {
            elems.push_back(this->parseValue(this->assembly->lookupTypeKey(*titer++), curr->value));
        }

        if(elems.size() != ttype->entries.size()) {
            this->addError("Tuple value has incorrect number of elements", Parser::convertSrcPos(node->base.pos));
            return std::nullopt;
        }
        else {
            return std::make_optional(std::move(elems));
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
                        vals[idx] = this->parseValue(this->assembly->lookupTypeKey(fiter->ftype), curr->entry.value);
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

                    Value* vv = this->parseValue(this->assembly->lookupTypeKey(fentry.ftype), curr->entry.value);
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

    Value* Parser::processAPIXValue(const Type* t, const char* name, const BSQON_AST_Node* node, const Type* etype)
    {
        if(node->tag != BSQON_AST_TAG_TypedValue) {
            this->addError("Expected APIResult value", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        auto tnode = BSQON_AST_NODE_AS(TypedValue, node);
        if(tnode->type->tag != BSQON_AST_TAG_NominalScopedType || tnode->istagged) {
            this->addError("Expected APIResult value", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        auto oftypenode = BSQON_AST_NODE_AS(NominalScopedType, tnode->type);
        if(oftypenode->root->tag != BSQON_AST_TAG_NominalType) {
            this->addError("Expected APIResult value", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        auto roottype = BSQON_AST_NODE_AS(NominalType, oftypenode->root);
        if(roottype->terms != NULL) {
            const Type* ttype = this->parseTypeRoot(tnode->type);
            if(ttype->tkey != t->tkey) {
                this->addError("Expected APIResult value but got type " + ttype->tkey, Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }
        }

        if(strcmp(roottype->name, "APIResult") != 0 || strcmp(oftypenode->ext, name) != 0) {
            this->addError("Expected APIResult value but got type " + std::string(roottype->name) + "::" + std::string(oftypenode->ext), Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }
            
        if(tnode->value->tag != BSQON_AST_TAG_BraceValue) {
            this->addError("Expected constructor arg list", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        std::optional<Value*> ofval = this->processPropertiesForSpecialCons(etype, BSQON_AST_NODE_AS(BraceValue, tnode->value));
        if(!ofval.has_value()) {
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        return ofval.value();
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
        const MapEntryType* metype = static_cast<const MapEntryType*>(this->assembly->lookupTypeKey("MapEntry<" + keytype->tkey + ", " + valtype->tkey + ">"));

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
        if (this->assembly->namespaces.at("Core")->types.contains(basenominal)) {
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
        if (titer == this->assembly->typerefs.end()) {
            return UnresolvedType::singleton;
        }
        else {
            return titer->second;
        }
    }

    const Type* Parser::resolveAndCheckType(TypeKey tkey, SourcePos spos)
    {
        auto tt = this->assembly->lookupTypeKey(tkey);
        if(tt->isUnresolved()) {
            this->addError("Could not resolve type " + tkey, spos);
        }
            
        return tt;
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

    const Type* Parser::parseTemplateTypeHelper_API(const Type* tresult, std::string sname, SourcePos spos)
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
        else if (tname == "Some") {
            return this->parseSomeType(Parser::convertSrcPos(node->base.pos), node->terms);
        }
        else if (tname == "Option") {
            return this->parseOptionType(Parser::convertSrcPos(node->base.pos), node->terms);
        }
        else if (tname == "Result") {
            return this->parseResultType(Parser::convertSrcPos(node->base.pos), node->terms);
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
        else if(etype == "Fail") {
            return this->parseTemplateTypeHelper_OkErr(roottype, "Fail", Parser::convertSrcPos(node->base.pos));
        }
        else if(etype == "Rejected") {
            return this->parseTemplateTypeHelper_API(roottype, "Rejected", Parser::convertSrcPos(node->base.pos));
        }
        else if(etype == "Failed") {
            return this->parseTemplateTypeHelper_API(roottype, "Failed", Parser::convertSrcPos(node->base.pos));
        }
        else if(etype == "Error") {
            return this->parseTemplateTypeHelper_API(roottype, "Error", Parser::convertSrcPos(node->base.pos));
        }
        else if(etype == "Success") {
            return this->parseTemplateTypeHelper_API(roottype, "Success", Parser::convertSrcPos(node->base.pos));
        }
        else {
            this->addError("Expected Result::Ok or Result::Err type", Parser::convertSrcPos(node->root->pos));
            return UnresolvedType::singleton;
        }
    }

    const Type* Parser::parseEListType(const BSQON_AST_NODE(EListType)* node)
    {
        std::vector<const Type*> types;
        this->parseTemplateTermList(node->types, types);
                
        if(std::any_of(types.cbegin(), types.cend(), [](const Type* tt) { return tt->isUnresolved(); })) {
            return UnresolvedType::singleton;
        }
        else {
            std::vector<TypeKey> kk;
            std::transform(types.cbegin(), types.cend(), std::back_inserter(kk), [](const Type* tt){ return tt->tkey; });

            auto tkey = "(|" + std::accumulate(kk.begin(), kk.end(), std::string(), [](std::string&& a, TypeKey& b) { return (a == "" ? "" : std::move(a) + ", ") + b; }) + "|)";
            return this->resolveAndCheckType(tkey, Parser::convertSrcPos(node->base.pos));
        }
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
            case BSQON_AST_TAG_EListType:
                return this->parseEListType(BSQON_AST_NODE_AS(EListType, node));
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

    Value* Parser::parseCString(const PrimitiveType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_CStringValue) {
            this->addError("Expected CString literal", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        auto bstr = BSQON_AST_NODE_AS(LiteralStringValue, node)->data;
        CStringValue* svopt = CStringValue::createFromParse(t, Parser::convertSrcPos(node->pos), bstr->bytes, bstr->len);

        if(svopt == nullptr) {
            this->addError("Invalid escape characters in ascii-string", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        return svopt;
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

    Value* Parser::parseTZDateTime(const PrimitiveType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_TZDateTimeValue) {
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

        if(!Parser::processDateInfo(dstr, year, month, day) || !Parser::processTimeInfo(tstr, hour, minute, second, true) || !Parser::processTZInfo(tzstr, &tz)) {
            this->addError("Invalid component in DateTime value", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        TZDateTime dv = {year, month, day, hour, minute, second, (char*)tz};
        return new TZDateTimeValue(t, Parser::convertSrcPos(node->pos), dv);
    }

    Value* Parser::parseTIATime(const PrimitiveType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_TIATimeValue) {
            this->addError("Expected UTCDateTime literal", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        auto dstr = std::string(BSQON_AST_NODE_AS(LiteralStandardValue, node)->data);
        auto tstr = dstr.substr(dstr.find('T') + 1);
        auto tzstr = dstr.substr(dstr.find('@'));

        uint16_t year;
        uint8_t month, day;
        uint8_t hour, minute, second;

        if(!Parser::processDateInfo(dstr, year, month, day) || !Parser::processTimeInfo(tstr, hour, minute, second, false)) {
            this->addError("Invalid component in TIATime value", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        TIATime dv = {year, month, day, hour, minute, second};
        return new TIATimeValue(t, Parser::convertSrcPos(node->pos), dv);
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

        if(!Parser::processTimeInfo(tstr, hour, minute, second, true)) {
            this->addError("Invalid component in PlainTime literal", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        PlainTime dv = {hour, minute, second};
        return new PlainTimeValue(t, Parser::convertSrcPos(node->pos), dv);
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
        
        if(!Parser::processDateInfo(dstr, year, month, day) || !Parser::processTimeInfo(tstr, hour, minute, second, true) || !Parser::processMillisInfo(tstr, millis)) {
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

        return new DeltaLogicalTimeValue(t, Parser::convertSrcPos(node->pos), vv);
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

    Value* Parser::parseCRegex(const PrimitiveType* t, const BSQON_AST_Node* node)
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

        auto rparse = CRegexValue::createFromParse(t, Parser::convertSrcPos(node->pos), strnode->data->bytes, strnode->data->len);
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

    Value* Parser::parsePath(const PrimitiveType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_PathValue) {
            this->addError("Expected path literal", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        auto pnode = BSQON_AST_NODE_AS(LiteralPathValue, node);

        if(*pnode->data->bytes != '\\') {
            this->addError("Expected Path literal", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        PathValue* popt = PathValue::createFromParse(t, Parser::convertSrcPos(node->pos), pnode->data->bytes, pnode->data->len);

        if(popt == nullptr) {
            this->addError("Invalid characters in path (does not validate)", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        return popt;
    }

    Value* Parser::parsePathItem(const PrimitiveType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_PathValue) {
            this->addError("Expected path literal", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        auto pnode = BSQON_AST_NODE_AS(LiteralPathValue, node);

        if(*pnode->data->bytes != 'f') {
            this->addError("Expected PathItem literal", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        PathItemValue* popt = PathItemValue::createFromParse(t, Parser::convertSrcPos(node->pos), pnode->data->bytes, pnode->data->len);

        if(popt == nullptr) {
            this->addError("Invalid characters in path (does not validate)", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        return popt;
    }

    Value* Parser::parseGlob(const PrimitiveType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_PathValue) {
            this->addError("Expected glob literal", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        auto pnode = BSQON_AST_NODE_AS(LiteralPathValue, node);

        if(*pnode->data->bytes != 'g') {
            this->addError("Expected PathItem literal", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        GlobValue* popt = GlobValue::createFromParse(t, Parser::convertSrcPos(node->pos), pnode->data->bytes, pnode->data->len);

        if(popt == nullptr) {
            this->addError("Invalid characters in pathGlob (does not validate)", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        return popt;
    }

    Value* Parser::parseSome(const SomeType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_SomeConsValue && node->tag != BSQON_AST_TAG_TypedValue) {
            this->addError("Expected Some value", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        if(node->tag == BSQON_AST_TAG_SomeConsValue) {
            Value* ofval = this->parseValue(this->assembly->lookupTypeKey(t->oftype), BSQON_AST_NODE_AS(SpecialConsValue, node)->value);
            return new SomeValue(t, Parser::convertSrcPos(node->pos), ofval);
        }
        else {
            auto tnode = BSQON_AST_NODE_AS(TypedValue, node);
            if(tnode->type->tag != BSQON_AST_TAG_NominalType || tnode->istagged) {
                this->addError("Expected Some value", Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            auto oftypenode = BSQON_AST_NODE_AS(NominalType, tnode->type);
            if(oftypenode->terms != NULL) {
                const Type* ttype = this->parseTypeRoot(tnode->type);
                if(ttype->tkey != t->tkey) {
                    this->addError("Expected Some value but got type " + ttype->tkey, Parser::convertSrcPos(node->pos));
                    return new ErrorValue(t, Parser::convertSrcPos(node->pos));
                }
            }
        
            if(tnode->value->tag != BSQON_AST_TAG_BraceValue) {
                this->addError("Expected constructor arg list", Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            std::optional<Value*> ofval = this->processPropertiesForSpecialCons(this->assembly->lookupTypeKey(t->oftype), BSQON_AST_NODE_AS(BraceValue, tnode->value));
            if(!ofval.has_value()) {
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            return new SomeValue(t, Parser::convertSrcPos(node->pos), ofval.value());
        }
    }

    Value* Parser::parseOk(const OkType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_OkConsValue && node->tag != BSQON_AST_TAG_TypedValue) {
            this->addError("Expected Ok value", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        if(node->tag == BSQON_AST_TAG_OkConsValue) {
            Value* ofval = this->parseValue(this->assembly->lookupTypeKey(t->ttype), BSQON_AST_NODE_AS(SpecialConsValue, node)->value);
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

            std::optional<Value*> ofval = this->processPropertiesForSpecialCons(this->assembly->lookupTypeKey(t->ttype), BSQON_AST_NODE_AS(BraceValue, tnode->value));
            if(!ofval.has_value()) {
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            return new OkValue(t, Parser::convertSrcPos(node->pos), ofval.value());
        }
    }

    Value* Parser::parseErr(const ErrorType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_ErrConsValue && node->tag != BSQON_AST_TAG_TypedValue) {
            this->addError("Expected Fail value", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        if(node->tag == BSQON_AST_TAG_ErrConsValue) {
            Value* ofval = this->parseValue(this->assembly->lookupTypeKey(t->etype), BSQON_AST_NODE_AS(SpecialConsValue, node)->value);
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
                this->addError("Expected Fail value", Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            auto roottype = BSQON_AST_NODE_AS(NominalType, oftypenode->root);
            if(roottype->terms != NULL) {
                const Type* ttype = this->parseTypeRoot(tnode->type);
                if(ttype->tkey != t->tkey) {
                    this->addError("Expected Fail value but got type " + ttype->tkey, Parser::convertSrcPos(node->pos));
                    return new ErrorValue(t, Parser::convertSrcPos(node->pos));
                }
            }
            
            if(strcmp(roottype->name, "Result") != 0 || strcmp(oftypenode->ext, "Fail") != 0) {
                this->addError("Expected Result::Fail value but got type " + std::string(roottype->name) + "::" + std::string(oftypenode->ext), Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            if(tnode->value->tag != BSQON_AST_TAG_BraceValue) {
                this->addError("Expected constructor arg list", Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            std::optional<Value*> ofval = this->processPropertiesForSpecialCons(this->assembly->lookupTypeKey(t->etype), BSQON_AST_NODE_AS(BraceValue, tnode->value));
            if(!ofval.has_value()) {
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            return new ErrValue(t, Parser::convertSrcPos(node->pos), ofval.value());
        }
    }

    Value* Parser::parseAPIRejected(const APIRejectedType* t, const BSQON_AST_Node* node)
    {
        auto ofval = this->processAPIXValue(t, "Rejected", node, this->assembly->lookupTypeKey(t->ttype));
        return new APIRejectedValue(t, Parser::convertSrcPos(node->pos), ofval);
    }

    Value* Parser::parseAPIFailed(const APIFailedType* t, const BSQON_AST_Node* node)
    {
        auto ofval = this->processAPIXValue(t, "Failed", node, this->assembly->lookupTypeKey(t->ttype));
        return new APIFailedValue(t, Parser::convertSrcPos(node->pos), ofval);
    }

    Value* Parser::parseAPIError(const APIErrorType* t, const BSQON_AST_Node* node)
    {
        auto ofval = this->processAPIXValue(t, "Error", node, this->assembly->lookupTypeKey(t->ttype));
        return new APIErrorValue(t, Parser::convertSrcPos(node->pos), ofval);
    }

    Value* Parser::parseAPISuccess(const APISuccessType* t, const BSQON_AST_Node* node)
    {
        auto ofval = this->processAPIXValue(t, "Success", node, this->assembly->lookupTypeKey(t->ttype));
        return new APISuccessValue(t, Parser::convertSrcPos(node->pos), ofval);
    }

    Value* Parser::parseMapEntry(const MapEntryType* t, const BSQON_AST_Node* node, bool implicitkey)
    {
        if(node->tag != BSQON_AST_TAG_TypedValue && node->tag != BSQON_AST_TAG_MapEntryValue) {
            this->addError("Expected MapEntry value", Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        if(node->tag == BSQON_AST_TAG_MapEntryValue) {
            auto tnode = BSQON_AST_NODE_AS(MapEntryValue, node);
            auto kv = this->parseValue(this->assembly->lookupTypeKey(t->ktype), tnode->key);

            if(implicitkey) {
                this->containerstack.back().kval = kv;
            }

            auto vv = this->parseValue(this->assembly->lookupTypeKey(t->vtype), tnode->value);

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

            auto vv = this->processPropertiesForMapEntry(this->assembly->lookupTypeKey(t->ktype), this->assembly->lookupTypeKey(t->vtype), BSQON_AST_NODE_AS(BraceValue, tnode->value));
            if(!vv.has_value()) {
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            return new MapEntryValue(t, Parser::convertSrcPos(node->pos), vv.value().first, vv.value().second);
        }
    }
        
    Value* Parser::parseEList(const EListType* t, const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_BracketValue && node->tag != BSQON_AST_TAG_TypedValue) {
            this->addError("Expected EList value", Parser::convertSrcPos(node->pos));
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

        auto vvals = this->processEntriesForEList(t, BSQON_AST_NODE_AS(BracketValue, node));
        if(!vvals.has_value()) {
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }
            
        return new EListValue(t, Parser::convertSrcPos(node->pos), std::move(vvals.value()));
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
        
        const Type* ptype = this->assembly->lookupTypeKey(t->primitivetype);
        Value* pval = this->parseValue(ptype, vnode);

        if(pval->vtype->tkey != ptype->tkey) {
            this->addError("Incompatible value and type alias -- got " + pval->vtype->tkey + " but expected " + ptype->tkey, Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        if(!pval->isValidForTypedecl()) {
            this->addError("Value cannot be used in a type alias " + pval->vtype->tkey, Parser::convertSrcPos(vnode->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        if(t->optOfValidators.has_value()) {
            const std::vector<std::u8string>& validators = t->optOfValidators.value();

            std::string nodefault("[TODO -- no default resolve]");
            for(size_t i = 0; i < validators.size(); ++i) {
                auto vv = validators[i];

                if(ptype->tkey == "String") {
                    StringValue* sval = static_cast<StringValue*>(pval);
                    bool isok = this->assembly->validateString(vv, &sval->sv, nodefault);

                    if(!isok) {
                        this->addError("Value does not validate against the validator " + std::string(vv.cbegin(), vv.cend()), Parser::convertSrcPos(node->pos));
                        return new ErrorValue(t, Parser::convertSrcPos(node->pos));
                    }
                }
                else {
                    CStringValue* sval = static_cast<CStringValue*>(pval);
                    bool isok = this->assembly->validateCString(vv, &sval->sv, nodefault);

                    if(!isok) {
                        this->addError("Value does not validate against the validator " + std::string(vv.cbegin(), vv.cend()), Parser::convertSrcPos(node->pos));
                        return new ErrorValue(t, Parser::convertSrcPos(node->pos));
                    }
                }
            }
        }

        if(t->hasvalidations) {
            this->validatecallbacks.push_front({ t->tkey, {pval} });
        }

        return new TypedeclValue(t, Parser::convertSrcPos(node->pos), pval);
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
        
        if(t->hasvalidations) {
            this->validatecallbacks.push_front({ t->tkey, {vvals.value().begin(), vvals.value().end()} });
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

        const Type* tkey = this->assembly->lookupTypeKey(t->ktype);
        const Type* tvalue = this->assembly->lookupTypeKey(t->vtype);

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
        else if(tk == "CString") {
            return this->parseCString(t, node);
        }
        else if(tk == "ByteBuffer") {
            return this->parseByteBuffer(t, node);
        }
        else if(tk == "TZDateTime") {
            return this->parseTZDateTime(t, node);
        }
        else if(tk == "TIATime") {
            return this->parseTIATime(t, node);
        }
        else if(tk == "PlainDate") {
            return this->parsePlainDate(t, node);
        }
        else if(tk == "PlainTime") {
            return this->parsePlainTime(t, node);
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
        else if(tk == "SecondsDelta") {
            return this->parseDeltaSeconds(t, node);
        }
        else if(tk == "ISOTimestampDelta") {
            return this->parseDeltaISOTimeStamp(t, node);
        }
        else if(tk == "LogicalTimeDelta") {
            return this->parseDeltaLogical(t, node);
        }      
        else if(tk == "UnicodeRegex") {
            return this->parseUnicodeRegex(t, node);
        }
        else if(tk == "CRegex") {
            return this->parseCRegex(t, node);
        }
        else if(tk == "PathRegex") {
            return this->parsePathRegex(t, node);
        }
        else if(tk == "Path") {
            return this->parsePath(t, node);
        }
        else if(tk == "PathItem") {
            return this->parsePathItem(t, node);
        }
        else if(tk == "Glob") {
            return this->parseGlob(t, node);
        }
        else {
            this->addError("Unknown primitive type " + tk, Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }
    }

    Value* Parser::parseValueDirect(const Type* t, const BSQON_AST_Node* node)
    {
        switch(t->tag) {
            case TypeTag::TYPE_ELIST: {
                return this->parseEList(static_cast<const EListType*>(t), node);
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
            case TypeTag::TYPE_SOME: {
                return this->parseSome(static_cast<const SomeType*>(t), node);
            }
            case TypeTag::TYPE_OK: {
                return this->parseOk(static_cast<const OkType*>(t), node);
            }
            case TypeTag::TYPE_ERROR: {
                return this->parseErr(static_cast<const ErrorType*>(t), node);
            }
            case TypeTag::TYPE_APIREJECTED: {
                return this->parseAPIRejected(static_cast<const APIRejectedType*>(t), node);
            }
            case TypeTag::TYPE_APIFAILED: {
                return this->parseAPIFailed(static_cast<const APIFailedType*>(t), node);
            }
            case TypeTag::TYPE_APIERROR: {
                return this->parseAPIError(static_cast<const APIErrorType*>(t), node);
            }
            case TypeTag::TYPE_APISUCCESS: {
                return this->parseAPISuccess(static_cast<const APISuccessType*>(t), node);
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
                
            if(node->tag == BSQON_AST_TAG_NoneValue) {
                return this->parseNone(static_cast<const PrimitiveType*>(this->assembly->lookupTypeKey("None")), node);
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

                return this->parseSome(static_cast<const SomeType*>(this->assembly->lookupTypeKey("Some<" + otype->oftype + ">")), node);    
            }
        }
        else if(t->tag == TypeTag::TYPE_RESULT) {
            const ResultType* rtype = static_cast<const ResultType*>(t);
            const OkType* oktype = static_cast<const OkType*>(this->assembly->lookupTypeKey(rtype->tkey + "::Ok"));
            const ErrorType* errtype = static_cast<const ErrorType*>(this->assembly->lookupTypeKey(rtype->tkey + "::Fail"));

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

                if(!this->assembly->checkSubtype(oftype, t)) {
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
        else if(t->tag == TypeTag::TYPE_APIRESULT) {
            if(node->tag != BSQON_AST_TAG_TypedValue) {
                this->addError("Values of APIResult<T> type must be tagged", Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            const Type* oftype = this->parseTypeRoot(BSQON_AST_NODE_AS(TypedValue, node)->type);
            if(oftype->isUnresolved()) {
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            if(!this->assembly->checkSubtype(oftype, t)) {
                this->addError("Expected result of type " + t->tkey + " but got " + oftype->tkey, Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            if(oftype->tag == TypeTag::TYPE_APIREJECTED) {
                return this->parseAPIRejected(static_cast<const APIRejectedType*>(oftype), node);
            }
            else if(oftype->tag == TypeTag::TYPE_APIFAILED) {
                return this->parseAPIFailed(static_cast<const APIFailedType*>(oftype), node);
            }
            else if(oftype->tag == TypeTag::TYPE_APIERROR) {
                return this->parseAPIError(static_cast<const APIErrorType*>(oftype), node);
            }
            else {
                return this->parseAPISuccess(static_cast<const APISuccessType*>(oftype), node);
            }
               
        }
        else if(t->tag == TypeTag::TYPE_STD_CONCEPT) {
            if(node->tag != BSQON_AST_TAG_TypedValue) {
                this->addError("Values of Concept type must be tagged", Parser::convertSrcPos(node->pos));
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            const Type* oftype = this->parseTypeRoot(BSQON_AST_NODE_AS(TypedValue, node)->type);
            if(oftype->isUnresolved()) {
                return new ErrorValue(t, Parser::convertSrcPos(node->pos));
            }

            if(!this->assembly->checkSubtype(oftype, t)) {
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
        else if ((t->tag == TypeTag::TYPE_STD_CONCEPT)) {
            return this->parseValueConcept(t, node);
        }
        else {
            return this->parseValueDirect(t, node);
        }
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

            if(!this->assembly->checkSubtype(oftype, t)) {
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

    Value* Parser::parseAccessInternal(const BSQON_AST_Node* node)
    {
        if(node->tag == BSQON_AST_TAG_IdentifierValue) {
            return this->parseStrictIdentifier(node);
        }
        else if(node->tag == BSQON_AST_TAG_AccessNameValue) {
            return this->parseAccessName(node);
        }
        else if(node->tag == BSQON_AST_TAG_AccessKeyValue) {
            return this->parseAccessKey(node);
        }
        else {
            this->addError("Expected access value", Parser::convertSrcPos(node->pos));
            return nullptr;
        }
    }
        
    Value* Parser::parseStrictIdentifier(const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_IdentifierValue) {
            this->addError("Expected Identifier value", Parser::convertSrcPos(node->pos));
            return nullptr;
        }

        std::string vname = BSQON_AST_NODE_AS(NameValue, node)->data;

        if(!this->vbinds.contains(vname)) {
            this->addError("Unknown let binding " + vname, Parser::convertSrcPos(node->pos));
            return nullptr;
        }

        return this->vbinds[vname];
    }
        
    Value* Parser::parseAccessName(const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_AccessNameValue) {
            this->addError("Expected access index value", Parser::convertSrcPos(node->pos));
            return nullptr;
        }
        auto nnode = BSQON_AST_NODE_AS(AccessNameValue, node);
        auto anxstr = std::string(nnode->name);

        Value* lvalue = this->parseAccessInternal(nnode->value);
        if(lvalue == nullptr || lvalue->kind != ValueKind::EntityValueKind) {
            this->addError("Expected record or entity value (non-symbolic)", Parser::convertSrcPos(node->pos));
            return nullptr;
        }

        auto avalue = static_cast<EntityValue*>(lvalue);
        auto atype = static_cast<const StdEntityType*>(avalue->vtype);

        auto eeiter = std::find_if(atype->fields.cbegin(), atype->fields.cend(), [&anxstr](const EntityTypeFieldEntry& entry) { return entry.fname == anxstr; });
        if(eeiter == atype->fields.cend()) {
            this->addError("Unknown field " + anxstr, Parser::convertSrcPos(node->pos));
            return nullptr;
        }
 
        return avalue->fieldvalues[std::distance(atype->fields.cbegin(), eeiter)]; 
    }

    Value* Parser::parseAccessKey(const BSQON_AST_Node* node)
    {
        if(node->tag != BSQON_AST_TAG_AccessKeyValue) {
            this->addError("Expected access index value", Parser::convertSrcPos(node->pos));
            return nullptr;
        }

        auto knode = BSQON_AST_NODE_AS(AccessKeyValue, node);
        Value* lvalue = this->parseAccessInternal(knode->value);

        if(lvalue == nullptr || (lvalue->kind != ValueKind::ListValueKind && lvalue->kind != ValueKind::MapValueKind)) {
            this->addError("Expected list or map value (non-symbolic)", Parser::convertSrcPos(node->pos));
            return nullptr;
        }

        if(lvalue->kind == ValueKind::ListValueKind) {
            auto avalue = static_cast<ListValue*>(lvalue);

            auto akey = this->parseValue(this->resolveAndCheckType("Nat", Parser::convertSrcPos(node->pos)), knode->kk);
            uint64_t aindex = static_cast<NatNumberValue*>(akey)->cnv;
            if(aindex >= avalue->vals.size()) {
                this->addError("Index out of range", Parser::convertSrcPos(node->pos));
                return nullptr;
            }

            return avalue->vals[aindex];
        }
        else {
            auto avalue = static_cast<MapValue*>(lvalue);
            auto atype = static_cast<const MapType*>(avalue->vtype);

            auto akey = this->parseValue(this->resolveAndCheckType(atype->ktype, Parser::convertSrcPos(node->pos)), knode->kk);      
            auto aiter = std::lower_bound(avalue->vals.begin(), avalue->vals.end(), akey, [](const MapEntryValue* v1, const Value* v2) { return Value::keyCompare(v1->key, v2) < 0; });

            if(aiter == avalue->vals.end() || Value::keyCompare((*aiter)->key, akey) != 0) {
                this->addError("Key not found in map", Parser::convertSrcPos(node->pos));
                return nullptr;
            }

            return (*aiter)->val;
        }
    }

    Value* Parser::parseAccess(const Type* t, const BSQON_AST_Node* node)
    {
        Value* avv = this->parseAccessInternal(node);

        if(avv == nullptr) {
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        if(!this->assembly->checkSubtype(avv->vtype, t)) {
            this->addError("Expected result of type " + t->tkey + " but got " + avv->vtype->tkey, Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        return avv;
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
        if(!this->assembly->checkSubtype(evalue->vtype, dtype)) {
            this->addError("Expected result of type " + dtype->tkey + " but got " + evalue->vtype->tkey, Parser::convertSrcPos(node->pos));
            return new ErrorValue(t, Parser::convertSrcPos(node->pos));
        }

        if(!this->assembly->checkSubtype(evalue->vtype, t)) {
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
        else if(node->tag == BSQON_AST_TAG_AccessNameValue || node->tag == BSQON_AST_TAG_AccessKeyValue) {
            return this->parseAccess(t, node);
        }
        else if(node->tag == BSQON_AST_TAG_LetInValue) {
            return this->parseLetIn(t, node);
        }
        else {
           return this->parseValueSimple(t, node);
        }
    }
}
