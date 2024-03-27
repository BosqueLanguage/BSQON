#pragma once

#include "../common.h"

namespace brex
{
    typedef size_t StateID;
    
    uint16_t saturateNFATokenIncrement(uint16_t count);

    class NFASimpleStateToken
    {
    public:
        StateID cstate;

        NFASimpleStateToken() : cstate(0) {;}
        NFASimpleStateToken(StateID cstate) : cstate(cstate) {;}
        ~NFASimpleStateToken() {;}

        NFASimpleStateToken(const NFASimpleStateToken& other) = default;
        NFASimpleStateToken(NFASimpleStateToken&& other) = default;

        NFASimpleStateToken& operator=(const NFASimpleStateToken& other) = default;
        NFASimpleStateToken& operator=(NFASimpleStateToken&& other) = default;

        bool operator==(const NFASimpleStateToken& other) const
        {
            return this->cstate == other.cstate;
        }

        bool operator!=(const NFASimpleStateToken& other) const
        {
            return this->cstate != other.cstate;
        }

        static bool cmp(const NFASimpleStateToken& t1, const NFASimpleStateToken& t2)
        {
            return t1.cstate < t2.cstate;
        }

        inline NFASimpleStateToken toNextState(StateID next) const
        {
            return NFASimpleStateToken(next);
        }
    };

    class NFASingleStateToken
    {
    public:
        StateID cstate;
        std::pair<StateID, uint16_t> rangecount;

        NFASingleStateToken() : cstate(0), rangecount(0, 0) {;}
        NFASingleStateToken(StateID cstate, std::pair<StateID, uint16_t> rangecount) : cstate(cstate), rangecount(rangecount) {;}
        ~NFASingleStateToken() {;}

        NFASingleStateToken(const NFASingleStateToken& other) = default;
        NFASingleStateToken(NFASingleStateToken&& other) = default;

        NFASingleStateToken& operator=(const NFASingleStateToken& other) = default;
        NFASingleStateToken& operator=(NFASingleStateToken&& other) = default;

        bool operator==(const NFASingleStateToken& other) const
        {
            return this->cstate == other.cstate && this->rangecount == other.rangecount;
        }

        bool operator!=(const NFASingleStateToken& other) const
        {
            return this->cstate != other.cstate && this->rangecount != other.rangecount;
        }

        static bool cmp(const NFASingleStateToken& t1, const NFASingleStateToken& t2)
        {
            if(t1.cstate < t2.cstate) {
                return true;
            }
            else if(t1.cstate > t2.cstate) {
                return false;
            }
            else {
                return t1.rangecount < t2.rangecount;
            }
        }

        inline NFASingleStateToken toNextState(StateID next) const
        {
            return NFASingleStateToken(next, this->rangecount);
        }

        inline static NFASingleStateToken toNextStateWithInitialize(StateID next, StateID incState)
        {
            return NFASingleStateToken(next, std::make_pair(incState, 1));
        }

        inline NFASingleStateToken toNextStateWithIncrement(StateID next) const
        {
            return NFASingleStateToken(next, std::make_pair(this->rangecount.first, saturateNFATokenIncrement(this->rangecount.second)));
        }

        inline NFASimpleStateToken toNextStateWithDoneRange(StateID next) const
        {
            return NFASimpleStateToken(next);
        }
    };

    class NFAFullStateToken
    {
    public:
        StateID cstate;
        std::vector<std::pair<StateID, uint16_t>> rangecounts;
        
        NFAFullStateToken() : cstate(0), rangecounts() {;}
        NFAFullStateToken(StateID cstate, std::vector<std::pair<StateID, uint16_t>> rangecounts) : cstate(cstate), rangecounts(rangecounts) {;}
        ~NFAFullStateToken() {;}

        NFAFullStateToken(const NFAFullStateToken& other) = default;
        NFAFullStateToken(NFAFullStateToken&& other) = default;

        NFAFullStateToken& operator=(const NFAFullStateToken& other) = default;
        NFAFullStateToken& operator=(NFAFullStateToken&& other) = default;

        bool operator==(const NFAFullStateToken& other) const
        {
            return this->cstate == other.cstate && std::equal(this->rangecounts.cbegin(), this->rangecounts.cend(), other.rangecounts.cbegin(), other.rangecounts.cend());
        }

        bool operator!=(const NFAFullStateToken& other) const
        {
            return this->cstate != other.cstate || !std::equal(this->rangecounts.cbegin(), this->rangecounts.cend(), other.rangecounts.cbegin(), other.rangecounts.cend());
        }

        static bool cmp(const NFAFullStateToken& t1, const NFAFullStateToken& t2)
        {
            if(t1.cstate < t2.cstate) {
                return true;
            }
            else if(t1.cstate > t2.cstate) {
                return false;
            }
            else {
                return std::lexicographical_compare(t1.rangecounts.cbegin(), t1.rangecounts.cend(), t2.rangecounts.cbegin(), t2.rangecounts.cend());
            }
        }

        inline NFAFullStateToken toNextState(StateID next) const
        {
            return NFAFullStateToken(next, this->rangecounts);
        }

        static inline NFAFullStateToken toNextStateWithInitialize(StateID next, std::pair<StateID, uint16_t> existingrng, StateID incState)
        {
            std::vector<std::pair<StateID, uint16_t>> newrangecounts = {existingrng, std::make_pair(incState, 1)};
            return NFAFullStateToken(next, newrangecounts);
        }

        inline NFAFullStateToken toNextStateWithIncrement(StateID next, StateID incState) const
        {
            std::vector<std::pair<StateID, uint16_t>> newrangecounts(this->rangecounts.size());
            std::transform(this->rangecounts.cbegin(), this->rangecounts.cend(), newrangecounts.begin(), [incState](const std::pair<StateID, uint16_t>& rc) {
                return rc.first == incState ? std::make_pair(rc.first, saturateNFATokenIncrement(rc.second)) : rc;
            });

            return NFAFullStateToken(next, newrangecounts);
        }
    };

    enum class NFAOptTag
    {
        Accept = 0x0,
        CharCode,
        CharRange,
        Dot,
        AnyOf,
        Star,
        RangeK
    };

    class NFAOpt
    {
    public:
        const NFAOptTag tag;
        const StateID stateid;

        NFAOpt(NFAOptTag tag, StateID stateid) : tag(tag), stateid(stateid) {;}
        virtual ~NFAOpt() {;}

        inline bool concreteTransition() const
        {
            return this->tag <= NFAOptTag::Dot;
        }
    };

    class NFAOptAccept : public NFAOpt
    {
    public:
        NFAOptAccept(StateID stateid) : NFAOpt(NFAOptTag::Accept, stateid) {;}
        virtual ~NFAOptAccept() {;}
    };

    class NFAOptCharCode : public NFAOpt
    {
    public:
        const RegexChar c;
        const StateID follow;

        NFAOptCharCode(StateID stateid, RegexChar c, StateID follow) : NFAOpt(NFAOptTag::CharCode, stateid), c(c), follow(follow) {;}
        virtual ~NFAOptCharCode() {;}
    };

    class NFAOptRange : public NFAOpt
    {
    public:
        const bool compliment;
        const std::vector<SingleCharRange> ranges;
        const StateID follow;

        NFAOptRange(StateID stateid, bool compliment, std::vector<SingleCharRange> ranges, StateID follow) : NFAOpt(NFAOptTag::CharRange, stateid), compliment(compliment), ranges(ranges), follow(follow) {;}
        virtual ~NFAOptRange() {;}
    };

    class NFAOptDot : public NFAOpt
    {
    public:
        const StateID follow;

        NFAOptDot(StateID stateid, StateID follow) : NFAOpt(NFAOptTag::Dot, stateid), follow(follow) {;}
        virtual ~NFAOptDot() {;}
    };

    class NFAOptAnyOf : public NFAOpt
    {
    public:
        const std::vector<StateID> follows;

        NFAOptAnyOf(StateID stateid, std::vector<StateID> follows) : NFAOpt(NFAOptTag::AnyOf, stateid), follows(follows) {;}
        virtual ~NFAOptAnyOf() {;}
    };

    class NFAOptStar : public NFAOpt
    {
    public:
        const StateID matchfollow;
        const StateID skipfollow;

        NFAOptStar(StateID stateid, StateID matchfollow, StateID skipfollow) : NFAOpt(NFAOptTag::Star, stateid), matchfollow(matchfollow), skipfollow(skipfollow) {;}
        virtual ~NFAOptStar() {;}
    };

    class NFAOptRangeK : public NFAOpt
    {
    public:
        const StateID infollow;
        const StateID outfollow;
        const uint16_t mink;
        const uint16_t maxk;

        NFAOptRangeK(StateID stateid, uint16_t mink, uint16_t maxk, StateID infollow, StateID outfollow) : NFAOpt(NFAOptTag::RangeK, stateid), infollow(infollow), outfollow(outfollow), mink(mink), maxk(maxk) {;}
        virtual ~NFAOptRangeK() {;}
    };

    class NFAState
    {
    public:
        typedef std::set<NFASimpleStateToken, decltype(&NFASimpleStateToken::cmp)> TSimpleStates;
        typedef std::set<NFASingleStateToken, decltype(&NFASingleStateToken::cmp)> TSingleStates;
        typedef std::set<NFAFullStateToken, decltype(&NFAFullStateToken::cmp)> TFullStates;

        TSimpleStates simplestates;
        TSingleStates singlestates;
        TFullStates fullstates;

        NFAState() : simplestates(&NFASimpleStateToken::cmp), singlestates(&NFASingleStateToken::cmp), fullstates(&NFAFullStateToken::cmp) {;}
        ~NFAState() {;}

        NFAState(const NFAState& other) = default;
        NFAState(NFAState&& other) = default;

        NFAState& operator=(const NFAState& other) = default;
        NFAState& operator=(NFAState&& other) = default;

        inline size_t stateSize() const
        {
            return this->simplestates.size() + this->singlestates.size() + this->fullstates.size();
        }

        void intitialize() {
            this->simplestates.clear();
            this->singlestates.clear();
            this->fullstates.clear();
        }

        void reset() {
            this->simplestates.clear();
            this->singlestates.clear();
            this->fullstates.clear();
        }
    };

    class NFAEpsilonWorkSet
    {
    public:
        typedef std::set<NFASimpleStateToken, decltype(&NFASimpleStateToken::cmp)> TSimpleStates;
        typedef std::set<NFASingleStateToken, decltype(&NFASingleStateToken::cmp)> TSingleStates;
        typedef std::set<NFAFullStateToken, decltype(&NFAFullStateToken::cmp)> TFullStates;

        TSimpleStates simplestates;
        TSingleStates singlestates;
        TFullStates fullstates;

        NFAEpsilonWorkSet() : simplestates(&NFASimpleStateToken::cmp), singlestates(&NFASingleStateToken::cmp), fullstates(&NFAFullStateToken::cmp) {;}
        ~NFAEpsilonWorkSet() {;}

        bool done() const
        {
            return this->simplestates.empty() && this->singlestates.empty() && this->fullstates.empty();
        }

        bool hasSimpleStates() const 
        { 
            return !this->simplestates.empty(); 
        }
        NFASimpleStateToken getNextSimpleState() 
        { 
            auto it = this->simplestates.begin();
            
            NFASimpleStateToken t = *it;
            this->simplestates.erase(it);

            return t;
        }

        bool hasSingleStates() const 
        { 
            return !this->singlestates.empty(); 
        }
        NFASingleStateToken getNextSingleState() 
        { 
            auto it = this->singlestates.begin();
            
            NFASingleStateToken t = *it;
            this->singlestates.erase(it);

            return t;
        }

        bool hasFullStates() const 
        { 
            return !this->fullstates.empty(); 
        }
        NFAFullStateToken getNextFullState() 
        { 
            auto it = this->fullstates.begin();
            
            NFAFullStateToken t = *it;
            this->fullstates.erase(it);

            return t;
        }
    };

    class NFAEpsilonFixpointSet
    {
    public:
        typedef std::set<NFASimpleStateToken, decltype(&NFASimpleStateToken::cmp)> TSimpleStates;
        typedef std::set<NFASingleStateToken, decltype(&NFASingleStateToken::cmp)> TSingleStates;
        typedef std::set<NFAFullStateToken, decltype(&NFAFullStateToken::cmp)> TFullStates;

        TSimpleStates simplestates;
        TSingleStates singlestates;
        TFullStates fullstates;

        NFAEpsilonFixpointSet() : simplestates(&NFASimpleStateToken::cmp), singlestates(&NFASingleStateToken::cmp), fullstates(&NFAFullStateToken::cmp) {;}
        ~NFAEpsilonFixpointSet() {;}

        NFAEpsilonFixpointSet(const NFAEpsilonWorkSet& iworkset) : simplestates(iworkset.simplestates), singlestates(iworkset.singlestates), fullstates(iworkset.fullstates) {;}
    };

    class NFAMachine
    {
    private:
        void addNextSimpleState(NFAState& nstates, NFAEpsilonWorkSet& workset, const NFASimpleStateToken& t) const
        {
            if(this->nfaopts[t.cstate]->concreteTransition()) {
                nstates.simplestates.insert(t);
            }
            else {
                workset.simplestates.insert(t);
            }
        }
        void addNextSingleState(NFAState& nstates, NFAEpsilonWorkSet& workset, const NFASingleStateToken& t) const
        {
            if(this->nfaopts[t.cstate]->concreteTransition()) {
                nstates.singlestates.insert(t);
            }
            else {
                workset.singlestates.insert(t);
            }
        }
        void addNextFullState(NFAState& nstates, NFAEpsilonWorkSet& workset, const NFAFullStateToken& t) const
        {
            if(this->nfaopts[t.cstate]->concreteTransition()) {
                nstates.fullstates.insert(t);
            }
            else {
                workset.fullstates.insert(t);
            }
        }

        void processSimpleStateEpsilonTransition(NFAState& nstates, NFAEpsilonFixpointSet& fixpoint, NFAEpsilonWorkSet& workset, const NFASimpleStateToken& t) const
        {
            if(this->nfaopts[t.cstate]->concreteTransition()) {
                nstates.simplestates.insert(t);
            }
            else {
                if(!fixpoint.simplestates.contains(t)) {
                    fixpoint.simplestates.insert(t);
                    workset.simplestates.insert(t);
                }
            }
        }
        void processSingleStateEpsilonTransition(NFAState& nstates, NFAEpsilonFixpointSet& fixpoint, NFAEpsilonWorkSet& workset, const NFASingleStateToken& t) const
        {
            if(this->nfaopts[t.cstate]->concreteTransition()) {
                nstates.singlestates.insert(t);
            }
            else {
                if(!fixpoint.singlestates.contains(t)) {
                    fixpoint.singlestates.insert(t);
                    workset.singlestates.insert(t);
                }
            }
        }
        void processFullStateEpsilonTransition(NFAState& nstates, NFAEpsilonFixpointSet& fixpoint, NFAEpsilonWorkSet& workset, const NFAFullStateToken& t) const
        {
            if(this->nfaopts[t.cstate]->concreteTransition()) {
                nstates.fullstates.insert(t);
            }
            else {
                if(!fixpoint.fullstates.contains(t)) {
                    fixpoint.fullstates.insert(t);
                    workset.fullstates.insert(t);
                }
            }
        }

        //process a single char and compute the new state
        void advanceCharForSimpleStates(RegexChar c, const NFAState& ostates, NFAEpsilonWorkSet& workset, NFAState& nstates) const;
        void advanceCharForSingleStates(RegexChar c, const NFAState& ostates, NFAEpsilonWorkSet& workset, NFAState& nstates) const;
        void advanceCharForFullStates(RegexChar c, const NFAState& ostates, NFAEpsilonWorkSet& workset, NFAState& nstates) const;

        //process all the epsilon transitions and compute the new state
        void advanceEpsilonForSimpleStates(NFAEpsilonFixpointSet& fixpoint, NFAEpsilonWorkSet& workset, NFAState& nstates) const;
        void advanceEpsilonForSingleStates(NFAEpsilonFixpointSet& fixpoint, NFAEpsilonWorkSet& workset, NFAState& nstates) const;
        void advanceEpsilonForFullStates(NFAEpsilonFixpointSet& fixpoint, NFAEpsilonWorkSet& workset, NFAState& nstates) const;

    public:
        const StateID startstate;
        const StateID acceptstate;

        const std::vector<NFAOpt*> nfaopts;
        NFASimpleStateToken acceptStateRepr;

        NFAMachine(StateID startstate, StateID acceptstate, std::vector<NFAOpt*> nfaopts) : startstate(startstate), acceptstate(acceptstate), nfaopts(nfaopts), acceptStateRepr(acceptstate) { ; }
        ~NFAMachine() = default;

        //true if the machine has accepted or all paths are rejected
        bool inAccepted(const NFAState& ostates) const;
        bool allRejected(const NFAState& ostates) const;

        void advanceChar(RegexChar c, const NFAState& ostates, NFAEpsilonWorkSet& workset, NFAState& nstates) const
        {
            this->advanceCharForSimpleStates(c, ostates, workset, nstates);
            this->advanceCharForSingleStates(c, ostates, workset, nstates);
            this->advanceCharForFullStates(c, ostates, workset, nstates);
        }

        void advanceEpsilon(NFAEpsilonFixpointSet& fixpoint, NFAEpsilonWorkSet& workset, NFAState& nstates) const
        {
            while(!workset.done()) {
                if(workset.hasSimpleStates()) {
                    this->advanceEpsilonForSimpleStates(fixpoint, workset, nstates);
                }

                if(workset.hasSingleStates()) {
                    this->advanceEpsilonForSingleStates(fixpoint, workset, nstates);
                }

                if(workset.hasFullStates()) {
                    this->advanceEpsilonForFullStates(fixpoint, workset, nstates);
                }
            }
        }

        void intitializeMachine(NFAState& nstates) const
        {
            nstates.intitialize();
            NFAEpsilonWorkSet workset;
            this->addNextSimpleState(nstates, workset, NFASimpleStateToken{this->startstate});

            NFAEpsilonFixpointSet fixpoint(workset);
            while(!workset.done()) {
                this->advanceEpsilon(fixpoint, workset, nstates);
            }
        }

        NFAState stepMachine(RegexChar c, const NFAState& ostates) const
        {
            NFAState nstates;
            NFAEpsilonWorkSet workset;
            this->advanceChar(c, ostates, workset, nstates);

            NFAEpsilonFixpointSet fixpoint(workset);
            while(!workset.done()) {
                this->advanceEpsilon(fixpoint, workset, nstates);
            }

            return nstates;
        }
    };
}