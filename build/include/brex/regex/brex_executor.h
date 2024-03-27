#pragma once

#include "../common.h"
#include "nfa_executor.h"

namespace brex
{
    template <typename TStr, typename TIter>
    class ComponentCheckREInfo
    {
    public:
        ComponentCheckREInfo() = default;
        virtual ~ComponentCheckREInfo() = default;

        ComponentCheckREInfo(const ComponentCheckREInfo& other) = default;
        ComponentCheckREInfo(ComponentCheckREInfo&& other) = default;

        ComponentCheckREInfo& operator=(const ComponentCheckREInfo& other) = default;
        ComponentCheckREInfo& operator=(ComponentCheckREInfo&& other) = default;

        //test is the regex accepts the string from spos to epos (inclusive)
        virtual bool test(TStr* sstr, int64_t spos, int64_t epos) = 0;
        
        //test is there is a substring that the regex accepts
        virtual bool testContains(TStr* sstr, int64_t spos, int64_t epos) = 0;

        //test is there is a substring that the regex accepts -- must start at spos
        virtual bool testFront(TStr* sstr, int64_t spos, int64_t epos) = 0;

        //test is there is a substring that the regex accepts -- must end at epos
        virtual bool testBack(TStr* sstr, int64_t spos, int64_t epos) = 0;

        //return the first and last index of the substring that the regex accepts -- spos it the first matching index and epos is the longest matching index (empty if no match exists)
        virtual std::vector<std::pair<int64_t, int64_t>> matchContains(TStr* sstr, int64_t spos, int64_t epos) = 0;
        
        //return the end index of the match -- starting from spos (or empty if no match is exists)
        virtual std::vector<int64_t> matchFront(TStr* sstr, int64_t spos, int64_t epos) = 0;

        //return the start index of the match -- ending at epos (or empty if no match is exists)
        virtual std::vector<int64_t> matchBack(TStr* sstr, int64_t spos, int64_t epos) = 0;
    };

    template <typename TStr, typename TIter>
    class SingleCheckREInfo : public ComponentCheckREInfo<TStr, TIter>
    {
    public:
        NFAExecutor<TStr, TIter> executor;
        bool isNegative;
        bool isFrontCheck;
        bool isBackCheck;

        SingleCheckREInfo() = default;
        SingleCheckREInfo(const NFAExecutor<TStr, TIter>& executor, bool isNegative, bool isFrontCheck, bool isBackCheck) : ComponentCheckREInfo<TStr, TIter>(), executor(executor), isNegative(isNegative), isFrontCheck(isFrontCheck), isBackCheck(isBackCheck) {;}
        virtual ~SingleCheckREInfo() = default;

        SingleCheckREInfo(const SingleCheckREInfo& other) = default;
        SingleCheckREInfo(SingleCheckREInfo&& other) = default;

        SingleCheckREInfo& operator=(const SingleCheckREInfo& other) = default;
        SingleCheckREInfo& operator=(SingleCheckREInfo&& other) = default;

        bool validateSingleOp(TStr* sstr, int64_t spos, int64_t epos)
        {
            bool accepted = false;
            if(this->isFrontCheck) {
                accepted = this->executor.matchTestForward(sstr, spos, epos);
            }
            else if(this->isBackCheck) {
                accepted = this->executor.matchTestReverse(sstr, spos, epos);
            }
            else {
                accepted = this->executor.test(sstr, spos, epos);
            }

            return this->isNegative ? !accepted : accepted;
        }

        bool test(TStr* sstr, int64_t spos, int64_t epos) override final
        {
            bool accepted = false;
            if(this->isFrontCheck) {
                accepted = this->executor.matchTestForward(sstr, spos, epos);
            }
            else if(this->isBackCheck) {
                accepted = this->executor.matchTestReverse(sstr, spos, epos);
            }
            else {
                accepted = this->executor.test(sstr, spos, epos);
            }

            return this->isNegative ? !accepted : accepted;
        }

        bool testContains(TStr* sstr, int64_t spos, int64_t epos) override final
        {
            //by def a single option that is not negative or front/back marked
            for(int64_t ii = spos; ii <= epos; ++ii) {
                if(this->executor.matchTestForward(sstr, ii, epos)) {
                    return true;
                }
            }

            return false;
        }

        bool testFront(TStr* sstr, int64_t spos, int64_t epos) override final
        {
            bool accepts = this->executor.matchTestForward(sstr, spos, epos);
            return this->isNegative ? !accepts : accepts;
        }

        bool testBack(TStr* sstr, int64_t spos, int64_t epos) override final
        {
            bool accepts = this->executor.matchTestReverse(sstr, spos, epos);
            return this->isNegative ? !accepts : accepts;
        }

        std::vector<std::pair<int64_t, int64_t>> matchContains(TStr* sstr, int64_t spos, int64_t epos) override final
        {
            std::vector<std::pair<int64_t, int64_t>> matches;

            for(int64_t ii = spos; ii <= epos; ++ii) {
                auto mm = this->executor.matchForward(sstr, ii, epos);

                if(!mm.empty()) {
                    std::transform(mm.cbegin(), mm.cend(), std::back_inserter(matches), [ii](int64_t epos) {
                        return std::make_pair(ii, epos);
                    });
                }
            }

            return matches;
        }

        std::vector<int64_t> matchFront(TStr* sstr, int64_t spos, int64_t epos) override final
        {
            return this->executor.matchForward(sstr, spos, epos);
        }

        std::vector<int64_t> matchBack(TStr* sstr, int64_t spos, int64_t epos) override final
        {
            return this->executor.matchReverse(sstr, spos, epos);
        }
    };

    template <typename TStr, typename TIter>
    class MultiCheckREInfo : public ComponentCheckREInfo<TStr, TIter>
    {
    public:
        std::vector<SingleCheckREInfo<TStr, TIter>*> checks;

        MultiCheckREInfo(const std::vector<SingleCheckREInfo<TStr, TIter>*>& checks) : ComponentCheckREInfo<TStr, TIter>(), checks(checks) {;}
        virtual ~MultiCheckREInfo() = default;

        void splitBindingOps(std::vector<SingleCheckREInfo<TStr, TIter>*>& bindingopts, std::vector<SingleCheckREInfo<TStr, TIter>*>& checkopts)
        {
            for(auto iter = this->checks.begin(); iter != this->checks.end(); ++iter) {
                SingleCheckREInfo<TStr, TIter>* chk = *iter;
                if(!chk->isNegative && !chk->isFrontCheck && !chk->isBackCheck) {
                    bindingopts.push_back(chk);
                }
                else {
                    checkopts.push_back(chk);
                }
            }
        }

        static std::vector<int64_t> computeSharedMatches(const std::vector<std::vector<int64_t>>& matches)
        {
            if(matches.empty()) {
                return std::vector<int64_t>();
            }

            std::vector<int64_t> sharedmatches = matches[0];
            for(auto iter = matches.cbegin() + 1; iter != matches.cend(); ++iter) {
                std::vector<int64_t> newmatches;
                std::set_intersection(sharedmatches.cbegin(), sharedmatches.cend(), iter->cbegin(), iter->cend(), std::back_inserter(newmatches));
                sharedmatches = std::move(newmatches);
            }

            return sharedmatches;
        }

        static bool validateOpSet(std::vector<SingleCheckREInfo<TStr, TIter>*>& opts, TStr* sstr, int64_t spos, int64_t epos)
        {
            return std::all_of(opts.begin(), opts.end(), [sstr, spos, epos](SingleCheckREInfo<TStr, TIter>* check) {
                return check->validateSingleOp(sstr, spos, epos);
            });
        }

        static std::vector<int64_t> validateMatchSetOptions(const std::vector<int64_t>& opts, std::vector<SingleCheckREInfo<TStr, TIter>*>& checks, TStr* sstr, int64_t spos)
        {
            std::vector<int64_t> matches;
            std::copy_if(opts.begin(), opts.end(), std::back_inserter(matches), [sstr, spos, &checks](int64_t epos) {
                return MultiCheckREInfo::validateOpSet(checks, sstr, spos, epos);
            });

            return matches;
        }

        bool test(TStr* sstr, int64_t spos, int64_t epos) override final
        {
            return MultiCheckREInfo::validateOpSet(this->checks, sstr, spos, epos);
        }

        bool testContains(TStr* sstr, int64_t spos, int64_t epos) override final
        {
            //CANNOT HAPPEN -- by def a matchable is a single option that is not negative or front/back marked
            return false;
        }

        bool testFront(TStr* sstr, int64_t spos, int64_t epos) override final
        {
            std::vector<SingleCheckREInfo<TStr, TIter>*> matchopts;
            std::vector<SingleCheckREInfo<TStr, TIter>*> checkops;
            this->splitBindingOps(matchopts, checkops);

            std::vector<int64_t> realmatches;
            if(matchopts.size() == 1) {
                realmatches = matchopts.front()->executor.matchForward(sstr, spos, epos);
            }
            else {
                std::vector<std::vector<int64_t>> matches;
                std::transform(matchopts.cbegin(), matchopts.cend(), std::back_inserter(matches), [sstr, spos, epos](SingleCheckREInfo<TStr, TIter>* check) {
                    return check->executor.matchForward(sstr, spos, epos);
                });

                realmatches = MultiCheckREInfo::computeSharedMatches(matches);
            }

            auto validmatches = MultiCheckREInfo::validateMatchSetOptions(realmatches, checkops, sstr, spos);
            return !validmatches.empty();
        }

        bool testBack(TStr* sstr, int64_t spos, int64_t epos) override final
        {
            std::vector<SingleCheckREInfo<TStr, TIter>*> matchopts;
            std::vector<SingleCheckREInfo<TStr, TIter>*> checkops;
            this->splitBindingOps(matchopts, checkops);

            std::vector<int64_t> realmatches;
            if(matchopts.size() == 1) {
                realmatches = matchopts.front()->executor.matchReverse(sstr, spos, epos);
            }
            else {
                std::vector<std::vector<int64_t>> matches;
                std::transform(matchopts.cbegin(), matchopts.cend(), std::back_inserter(matches), [sstr, spos, epos](SingleCheckREInfo<TStr, TIter>* check) {
                    return check->executor.matchReverse(sstr, spos, epos);
                });

                realmatches = MultiCheckREInfo::computeSharedMatches(matches);
            }

            auto validmatches = MultiCheckREInfo::validateMatchSetOptions(realmatches, checkops, sstr, spos);
            return !validmatches.empty();
        }

        std::vector<std::pair<int64_t, int64_t>> matchContains(TStr* sstr, int64_t spos, int64_t epos) override final
        {
            //CANNOT HAPPEN -- by def a matchable is a single option that is not negative or front/back marked
            return std::vector<std::pair<int64_t, int64_t>>{};
        }

        std::vector<int64_t> matchFront(TStr* sstr, int64_t spos, int64_t epos) override final
        {
            std::vector<SingleCheckREInfo<TStr, TIter>*> matchopts;
            std::vector<SingleCheckREInfo<TStr, TIter>*> checkops;
            this->splitBindingOps(matchopts, checkops);

            std::vector<int64_t> realmatches;
            if(matchopts.size() == 1) {
                realmatches = matchopts.front()->executor.matchForward(sstr, spos, epos);
            }
            else {
                std::vector<std::vector<int64_t>> matches;
                std::transform(matchopts.cbegin(), matchopts.cend(), std::back_inserter(matches), [sstr, spos, epos](SingleCheckREInfo<TStr, TIter>* check) {
                    return check->executor.matchForward(sstr, spos, epos);
                });

                realmatches = MultiCheckREInfo::computeSharedMatches(matches);
            }

            return MultiCheckREInfo::validateMatchSetOptions(realmatches, checkops, sstr, spos);
        }

        std::vector<int64_t> matchBack(TStr* sstr, int64_t spos, int64_t epos) override final
        {
            std::vector<SingleCheckREInfo<TStr, TIter>*> matchopts;
            std::vector<SingleCheckREInfo<TStr, TIter>*> checkops;
            this->splitBindingOps(matchopts, checkops);

            std::vector<int64_t> realmatches;
            if(matchopts.size() == 1) {
                realmatches = matchopts.front()->executor.matchReverse(sstr, spos, epos);
            }
            else {
                std::vector<std::vector<int64_t>> matches;
                std::transform(matchopts.cbegin(), matchopts.cend(), std::back_inserter(matches), [sstr, spos, epos](SingleCheckREInfo<TStr, TIter>* check) {
                    return check->executor.matchReverse(sstr, spos, epos);
                });

                realmatches = MultiCheckREInfo::computeSharedMatches(matches);
            }

            return MultiCheckREInfo::validateMatchSetOptions(realmatches, checkops, sstr, spos);
        }
    };

    enum ExecutorError
    {
        Ok,
        InvalidRegexStructure
    };

    template <typename TStr, typename TIter>
    class REExecutor
    {
    public:
        const Regex* declre; 

        ComponentCheckREInfo<TStr, TIter>* optPre;
        ComponentCheckREInfo<TStr, TIter>* optPost;
        ComponentCheckREInfo<TStr, TIter>* re;

        REExecutor(const Regex* declre, ComponentCheckREInfo<TStr, TIter>* optPre, ComponentCheckREInfo<TStr, TIter>* optPost, ComponentCheckREInfo<TStr, TIter>* re) : declre(declre), optPre(optPre), optPost(optPost), re(re) {;}
        ~REExecutor() = default;

        bool test(TStr* sstr, int64_t spos, int64_t epos, bool oobPrefix, bool oobPostfix, ExecutorError& error)
        {
            error = ExecutorError::Ok;
            if(!this->declre->canUseInTest(oobPrefix, oobPostfix)) {
                error = ExecutorError::InvalidRegexStructure;
                return false;
            }

            bool rechk = this->re->test(sstr, spos, epos);
            if(!rechk) {
                return false;
            }

            bool prechk = this->optPre == nullptr || this->optPre->testBack(sstr, 0, spos - 1);
            bool postchk = this->optPost == nullptr || this->optPost->testFront(sstr, epos + 1, (int64_t)sstr->size() - 1);

            return prechk && postchk;
        }
        
        bool testContains(TStr* sstr, int64_t spos, int64_t epos, bool oobPrefix, bool oobPostfix, ExecutorError& error)
        {
            error = ExecutorError::Ok;
            if(!this->declre->canUseInContains()) {
                error = ExecutorError::InvalidRegexStructure;
                return false;
            }

            if(this->optPre == nullptr && this->optPost == nullptr) {
                return this->re->testContains(sstr, spos, epos);
            }
            else {
                auto opts = this->re->matchContains(sstr, spos, epos);

                return std::any_of(opts.cbegin(), opts.cend(), [this, sstr, spos, epos, oobPrefix, oobPostfix](const std::pair<int64_t, int64_t>& opt) {
                    bool prechk = this->optPre == nullptr || this->optPre->testBack(sstr, oobPrefix ? 0 : spos, opt.first - 1);
                    bool postchk = this->optPost == nullptr || this->optPost->testFront(sstr, opt.second + 1, oobPostfix ? (int64_t)sstr->size() - 1 : epos);

                    return prechk && postchk;
                });
            }
        }

        bool testFront(TStr* sstr, int64_t spos, int64_t epos, bool oobPrefix, bool oobPostfix, ExecutorError& error)
        {
            error = ExecutorError::Ok;
            if(!this->declre->canStartsWith(oobPrefix)) {
                error = ExecutorError::InvalidRegexStructure;
                return false;
            }

            if(this->optPre == nullptr && this->optPost == nullptr) {
                return this->re->testFront(sstr, spos, epos);
            }
            else {
                auto opts = this->re->matchFront(sstr, spos, epos);

                return std::any_of(opts.cbegin(), opts.cend(), [this, sstr, spos, epos, oobPostfix](int64_t opt) {
                    bool prechk = this->optPre == nullptr || this->optPre->testBack(sstr, 0, opt - 1);
                    bool postchk = this->optPost == nullptr || this->optPost->testFront(sstr, opt + 1, oobPostfix ? (int64_t)sstr->size() - 1 : epos);

                    return prechk && postchk;
                });
            }
        }

        bool testBack(TStr* sstr, int64_t spos, int64_t epos, bool oobPrefix, bool oobPostfix, ExecutorError& error)
        {
            error = ExecutorError::Ok;
            if(!this->declre->canEndsWith(oobPostfix)) {
                error = ExecutorError::InvalidRegexStructure;
                return false;
            }

            if(this->optPre == nullptr && this->optPost == nullptr) {
                return this->re->testBack(sstr, spos, epos);
            }
            else {
                auto opts = this->re->matchBack(sstr, spos, epos);

                return std::any_of(opts.cbegin(), opts.cend(), [this, sstr, spos, epos, oobPrefix](int64_t opt) {
                    bool prechk = this->optPre == nullptr || this->optPre->testBack(sstr, oobPrefix ? 0 : spos, opt - 1);
                    bool postchk = this->optPost == nullptr || this->optPost->testFront(sstr, opt + 1, (int64_t)sstr->size() - 1);

                    return prechk && postchk;
                });
            }
        }

        std::optional<std::pair<int64_t, int64_t>> matchContainsFirst(TStr* sstr, int64_t spos, int64_t epos, bool oobPrefix, bool oobPostfix, ExecutorError& error)
        {
            error = ExecutorError::Ok;
            if(!this->declre->canUseInMatchContains()) {
                error = ExecutorError::InvalidRegexStructure;
                return std::nullopt;
            }

            std::vector<std::pair<int64_t, int64_t>> mmr;
            if(this->optPre == nullptr && this->optPost == nullptr) {
                mmr = this->re->matchContains(sstr, spos, epos);
            }
            else {
                auto opts = this->re->matchContains(sstr, spos, epos);

                std::copy_if(opts.cbegin(), opts.cend(), std::back_inserter(mmr), [this, sstr, spos, epos, oobPrefix, oobPostfix](const std::pair<int64_t, int64_t>& opt) {
                    bool prechk = this->optPre == nullptr || this->optPre->testBack(sstr, oobPrefix ? 0 : spos, opt.first - 1);
                    bool postchk = this->optPost == nullptr || this->optPost->testFront(sstr, opt.second + 1, oobPostfix ? (int64_t)sstr->size() - 1 : epos);

                    return prechk && postchk;
                });
            }

            if(mmr.empty()) {
                return std::nullopt;
            }

            std::sort(mmr.begin(), mmr.end(), [](const std::pair<int64_t, int64_t>& a, const std::pair<int64_t, int64_t>& b) {
                return a.first < b.first;
            });
            auto midx = mmr.front().first;

            std::vector<std::pair<int64_t, int64_t>> minmmr;
            std::copy_if(mmr.cbegin(), mmr.cend(), std::back_inserter(minmmr), [midx](const std::pair<int64_t, int64_t>& opt) {
                return opt.first == midx;
            });

            std::sort(minmmr.begin(), minmmr.end(), [](const std::pair<int64_t, int64_t>& a, const std::pair<int64_t, int64_t>& b) {
                return a.second < b.second;
            });

            return std::make_optional(minmmr.back());
        }

        std::optional<std::pair<int64_t, int64_t>> matchContainsLast(TStr* sstr, int64_t spos, int64_t epos, bool oobPrefix, bool oobPostfix, ExecutorError& error)
        {
            error = ExecutorError::Ok;
            if(!this->declre->canUseInMatchContains()) {
                error = ExecutorError::InvalidRegexStructure;
                return std::nullopt;
            }

            std::vector<std::pair<int64_t, int64_t>> mmr;
            if(this->optPre == nullptr && this->optPost == nullptr) {
                mmr = this->re->matchContains(sstr, spos, epos);
            }
            else {
                auto opts = this->re->matchContains(sstr, spos, epos);

                std::copy_if(opts.cbegin(), opts.cend(), std::back_inserter(mmr), [this, sstr, spos, epos, oobPrefix, oobPostfix](const std::pair<int64_t, int64_t>& opt) {
                    bool prechk = this->optPre == nullptr || this->optPre->testBack(sstr, oobPrefix ? 0 : spos, opt.first - 1);
                    bool postchk = this->optPost == nullptr || this->optPost->testFront(sstr, opt.second + 1, oobPostfix ? (int64_t)sstr->size() - 1 : epos);

                    return prechk && postchk;
                });
            }

            if(mmr.empty()) {
                return std::nullopt;
            }

            std::sort(mmr.begin(), mmr.end(), [](const std::pair<int64_t, int64_t>& a, const std::pair<int64_t, int64_t>& b) {
                return a.second < b.second;
            });
            auto midx = mmr.back().second;

            std::vector<std::pair<int64_t, int64_t>> maxmmr;
            std::copy_if(mmr.cbegin(), mmr.cend(), std::back_inserter(maxmmr), [midx](const std::pair<int64_t, int64_t>& opt) {
                return opt.second == midx;
            });

            std::sort(maxmmr.begin(), maxmmr.end(), [](const std::pair<int64_t, int64_t>& a, const std::pair<int64_t, int64_t>& b) {
                return a.first < b.first;
            });

            return std::make_optional(maxmmr.front());
        }

        std::optional<int64_t> matchFront(TStr* sstr, int64_t spos, int64_t epos, bool oobPrefix, bool oobPostfix, ExecutorError& error)
        {
            error = ExecutorError::Ok;
            if(!this->declre->canUseInMatchStart(oobPrefix)) {
                error = ExecutorError::InvalidRegexStructure;
                return std::nullopt;
            }

            std::vector<int64_t> mmr;
            if(this->optPre == nullptr && this->optPost == nullptr) {
                mmr = this->re->matchFront(sstr, spos, epos);
            }
            else {
                auto opts = this->re->matchFront(sstr, spos, epos);

                std::copy_if(opts.cbegin(), opts.cend(), std::back_inserter(mmr), [this, sstr, spos, epos, oobPostfix](int64_t opt) {
                    bool prechk = this->optPre == nullptr || this->optPre->testBack(sstr, 0, opt - 1);
                    bool postchk = this->optPost == nullptr || this->optPost->testFront(sstr, opt + 1, oobPostfix ? (int64_t)sstr->size() - 1 : epos);

                    return prechk && postchk;
                });
            }

            return !mmr.empty() ? std::make_optional(mmr.back()) : std::nullopt;
        }

        std::optional<int64_t> matchBack(TStr* sstr, int64_t spos, int64_t epos, bool oobPrefix, bool oobPostfix, ExecutorError& error)
        {
            error = ExecutorError::Ok;
            if(!this->declre->canUseInMatchEnd(oobPostfix)) {
                error = ExecutorError::InvalidRegexStructure;
                return std::nullopt;
            }

            std::vector<int64_t> mmr;
            if(this->optPre == nullptr && this->optPost == nullptr) {
                mmr = this->re->matchBack(sstr, spos, epos);
            }
            else {
                auto opts = this->re->matchBack(sstr, spos, epos);

                std::copy_if(opts.cbegin(), opts.cend(), std::back_inserter(mmr), [this, sstr, spos, epos, oobPrefix](int64_t opt) {
                    bool prechk = this->optPre == nullptr || this->optPre->testBack(sstr, oobPrefix ? 0 : spos, opt - 1);
                    bool postchk = this->optPost == nullptr || this->optPost->testFront(sstr, opt + 1, (int64_t)sstr->size() - 1);

                    return prechk && postchk;
                });
            }

            return !mmr.empty() ? std::make_optional(mmr.back()) : std::nullopt;
        }

        bool test(TStr* sstr, ExecutorError& error) { return this->test(sstr, 0, (int64_t)sstr->size() - 1, false, false, error); }

        bool testContains(TStr* sstr, ExecutorError& error) { return this->testContains(sstr, 0, (int64_t)sstr->size() - 1, false, false, error); }
        bool testFront(TStr* sstr, ExecutorError& error) { return this->testFront(sstr, 0, (int64_t)sstr->size() - 1, false, false, error); }
        bool testBack(TStr* sstr, ExecutorError& error) { return this->testBack(sstr, 0, (int64_t)sstr->size() - 1, false, false, error); }

        std::optional<std::pair<int64_t, int64_t>> matchContainsFirst(TStr* sstr, ExecutorError& error) { return this->matchContainsFirst(sstr, 0, (int64_t)sstr->size() - 1, false, false, error); }
        std::optional<std::pair<int64_t, int64_t>> matchContainsLast(TStr* sstr, ExecutorError& error) { return this->matchContainsLast(sstr, 0, (int64_t)sstr->size() - 1, false, false, error); }
        std::optional<int64_t> matchFront(TStr* sstr, ExecutorError& error) { return this->matchFront(sstr, 0, (int64_t)sstr->size() - 1, false, false, error); }
        std::optional<int64_t> matchBack(TStr* sstr, ExecutorError& error) { return this->matchBack(sstr, 0, (int64_t)sstr->size() - 1, false, false, error); }
    };

    typedef REExecutor<UnicodeString, UnicodeRegexIterator> UnicodeRegexExecutor;
    typedef REExecutor<ASCIIString, ASCIIRegexIterator> ASCIIRegexExecutor;
}
