#pragma once 

#include "../common.h"

#include "nfa_machine.h"

namespace brex
{
    template <typename TStr, typename TIter>
    class NFAExecutor
    {
    private:
        NFAMachine* forward; 
        NFAMachine* reverse;

        TIter iter;

        NFAMachine* m;
        NFAState cstates;

        void runIntialStep()
        {
            this->m->intitializeMachine(this->cstates);
        }

        void runStep(RegexChar c)
        {
            this->cstates = this->m->stepMachine(c, this->cstates);
        }

        inline bool accepted() const { return this->m->inAccepted(this->cstates); }
        inline bool rejected() const { return this->m->allRejected(this->cstates); }

    public:
        NFAExecutor(): forward(nullptr), reverse(nullptr), TIter(), m(nullptr), cstates() {;}
        NFAExecutor(NFAMachine* forward, NFAMachine* reverse) : forward(forward), reverse(reverse), iter(), m(nullptr), cstates() {;}
        ~NFAExecutor() = default;

        NFAExecutor(const NFAExecutor& other) = default;
        NFAExecutor(NFAExecutor&& other) = default;

        NFAExecutor& operator=(const NFAExecutor& other) = default;
        NFAExecutor& operator=(NFAExecutor&& other) = default;

        bool test(TStr* sstr, int64_t spos, int64_t epos)
        {
            this->m = this->forward;
            this->iter = TIter{sstr, spos, epos, spos};

            this->runIntialStep();
            while(this->iter.valid()) {
                this->runStep(this->iter.get());
                this->iter.inc();

                if(this->rejected()) {
                    return false;
                }
            }

            return this->accepted();
        }

        bool matchTestForward(TStr* sstr, int64_t spos, int64_t epos)
        {
            this->m = this->forward;
            this->iter = TIter{sstr, spos, epos, spos};

            this->runIntialStep();
            while(this->iter.valid() && !(this->accepted() || this->rejected())) {
                this->runStep(this->iter.get());
                this->iter.inc();
            }

            return this->accepted();
        }

        bool matchTestReverse(TStr* sstr, int64_t spos, int64_t epos)
        {
            this->m = this->reverse;
            this->iter = TIter{sstr, spos, epos, epos};

            this->runIntialStep();
            while(this->iter.valid() && !(this->accepted() || this->rejected())) {
                this->runStep(this->iter.get());
                this->iter.dec();
            }

            return this->accepted();
        }

        std::vector<int64_t> matchForward(TStr* sstr, int64_t spos, int64_t epos)
        {
            this->m = this->forward;
            this->iter = TIter{sstr, spos, epos, spos};

            std::vector<int64_t> matches;
            this->runIntialStep();
            while(this->iter.valid() && !this->rejected()) {
                this->runStep(this->iter.get());

                if(this->accepted()) {
                    matches.push_back(this->iter.curr);
                }

                this->iter.inc();
            }

            return matches;
        }

        std::vector<int64_t> matchReverse(TStr* sstr, int64_t spos, int64_t epos)
        {
            this->m = this->reverse;
            this->iter = TIter{sstr, spos, epos, epos};

            std::vector<int64_t> matches;
            this->runIntialStep();
            while(this->iter.valid() && !this->rejected()) {
                this->runStep(this->iter.get());

                if(this->accepted()) {
                    matches.push_back(this->iter.curr);
                }

                this->iter.dec();
            }

            return matches;
        }
    };
}
