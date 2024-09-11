#pragma once

#include "../common.h"

namespace brex
{
    enum class RegexOptTag
    {
        Literal,
        CharRange,
        CharClassDot,
        NamedRegex,
        EnvRegex,
        StarRepeat,
        PlusRepeat,
        RangeRepeat,
        Optional,
        AnyOf,
        Sequence
    };

    class RegexOpt
    {
    public:
        const RegexOptTag tag;

        RegexOpt(RegexOptTag tag): tag(tag) {;}
        virtual ~RegexOpt() {;}

        virtual bool needsParens() const { return false; }
        virtual bool needsSequenceParens() const { return false; }
        virtual std::u8string toBSQONFormat() const = 0;

        static RegexOpt* jparse(json j);
    };

    class LiteralOpt : public RegexOpt
    {
    public:
        const std::vector<RegexChar> codes;
        const bool isunicode;

        LiteralOpt(std::vector<RegexChar> codes, bool isunicode) : RegexOpt(RegexOptTag::Literal), codes(codes), isunicode(isunicode) {;}
        virtual ~LiteralOpt() = default;

        virtual std::u8string toBSQONFormat() const override final
        {
            if(this->isunicode) {
                auto bbytes = escapeRegexUnicodeLiteralCharBuffer(this->codes);
                return u8'"' + std::u8string(bbytes.cbegin(), bbytes.cend()) + u8'"';
            }
            else {
                auto bbytes = escapeRegexCLiteralCharBuffer(this->codes);
                return u8"'" + std::u8string(bbytes.cbegin(), bbytes.cend()) + u8"'";
            }
        }

        static LiteralOpt* jparse(json j)
        {
            std::vector<RegexChar> codes;
            auto jcodes = j["charcodes"];
            std::transform(jcodes.cbegin(), jcodes.cend(), std::back_inserter(codes), [](const json& rv) {
                return rv.get<RegexChar>();
            });

            const bool isunicode = j["isunicode"].get<bool>();

            return new LiteralOpt(codes, isunicode);
        }
    };

    class CharRangeOpt : public RegexOpt
    {
    public:
        const bool compliment;
        const std::vector<SingleCharRange> ranges;
        const bool isunicode;

        CharRangeOpt(bool compliment, std::vector<SingleCharRange> ranges, bool isunicode) : RegexOpt(RegexOptTag::CharRange), compliment(compliment), ranges(ranges), isunicode(isunicode) {;}
        virtual ~CharRangeOpt() = default;

        virtual std::u8string toBSQONFormat() const override final
        {
            std::u8string rngs = u8"[";
            if(this->compliment) {
                rngs.push_back('^');
            }

            for(auto ii = this->ranges.cbegin(); ii != this->ranges.cend(); ++ii) {
                auto cr = *ii;

                auto lowbytes = this->isunicode ? escapeSingleUnicodeRegexChar(cr.low) : escapeSingleCRegexChar(cr.low);
                rngs.append(lowbytes.cbegin(), lowbytes.cend());

                if(cr.low != cr.high) {
                    rngs.push_back('-');
                    
                    auto highbytes = this->isunicode ? escapeSingleUnicodeRegexChar(cr.high) : escapeSingleCRegexChar(cr.high);
                    rngs.append(highbytes.cbegin(), highbytes.cend());
                }
            }
            rngs.push_back(']');

            return rngs;
        }

        static CharRangeOpt* jparse(json j)
        {
            const bool compliment = j["compliment"].get<bool>();

            std::vector<SingleCharRange> ranges;
            auto jranges = j["range"];
            std::transform(jranges.cbegin(), jranges.cend(), std::back_inserter(ranges), [](const json& rv) {
                auto lb = rv["lb"].get<RegexChar>();
                auto ub = rv["ub"].get<RegexChar>();

                return SingleCharRange{lb, ub};
            });

            const bool isunicode = j["isunicode"].get<bool>();

            return new CharRangeOpt(compliment, ranges, isunicode);
        }
    };

    class CharClassDotOpt : public RegexOpt
    {
    public:
        CharClassDotOpt() : RegexOpt(RegexOptTag::CharClassDot) {;}
        virtual ~CharClassDotOpt() = default;

        virtual std::u8string toBSQONFormat() const override final
        {
            return std::u8string{u8'.'};
        }

        static CharClassDotOpt* jparse(json j)
        {
            return new CharClassDotOpt();
        }
    };

    class NamedRegexOpt : public RegexOpt
    {
    public:
        //The namespace of the regex including scope (but not resolved)
        const std::string rname;

        NamedRegexOpt(const std::string& rname) : RegexOpt(RegexOptTag::NamedRegex), rname(rname) {;}
        virtual ~NamedRegexOpt() = default;

        virtual std::u8string toBSQONFormat() const override final
        {
            return u8'{' + std::u8string(this->rname.cbegin(), this->rname.cend()) + u8'}';
        }

        static NamedRegexOpt* jparse(json j)
        {
            const std::string rname = j["rname"].get<std::string>();

            return new NamedRegexOpt(rname);
        }
    };

    class EnvRegexOpt : public RegexOpt
    {
    public:
        const std::string ename;

        EnvRegexOpt(const std::string& ename) : RegexOpt(RegexOptTag::EnvRegex), ename(ename) {;}
        virtual ~EnvRegexOpt() = default;

        virtual std::u8string toBSQONFormat() const override final
        {
            return u8'{' + std::u8string(this->ename.cbegin(), this->ename.cend()) + u8'}';
        }

        static EnvRegexOpt* jparse(json j)
        {
            const std::string ename = j["ename"].get<std::string>();

            return new EnvRegexOpt(ename);
        }
    };

    class StarRepeatOpt : public RegexOpt
    {
    public:
        const RegexOpt* repeat;

        StarRepeatOpt(const RegexOpt* repeat) : RegexOpt(RegexOptTag::StarRepeat), repeat(repeat) {;}
        virtual ~StarRepeatOpt() = default;

        virtual bool needsParens() const override final { return true; }
        virtual std::u8string toBSQONFormat() const override final
        {
            if(!this->repeat->needsParens()) {
                return this->repeat->toBSQONFormat() + u8'*';
            }
            else {
                return u8'(' + this->repeat->toBSQONFormat() + u8")*";
            }
        }

        static StarRepeatOpt* jparse(json j)
        {
            auto repeat = RegexOpt::jparse(j["repeat"]);
            return new StarRepeatOpt(repeat);
        }
    };

    class PlusRepeatOpt : public RegexOpt
    {
    public:
        const RegexOpt* repeat;

        PlusRepeatOpt(const RegexOpt* repeat) : RegexOpt(RegexOptTag::PlusRepeat), repeat(repeat) {;}
        virtual ~PlusRepeatOpt() = default;

        virtual bool needsParens() const override final { return true; }
        virtual std::u8string toBSQONFormat() const override final
        {
            if (!this->repeat->needsParens()) {
                return this->repeat->toBSQONFormat() + u8'+';
            }
            else {
                return u8'(' + this->repeat->toBSQONFormat() + u8")+";
            }
        }

        static PlusRepeatOpt* jparse(json j)
        {
            auto repeat = RegexOpt::jparse(j["repeat"]);
            return new PlusRepeatOpt(repeat);
        }
    };

    class RangeRepeatOpt : public RegexOpt
    {
    public:
        const RegexOpt* repeat;
        const uint16_t low;
        const uint16_t high; //if high == INT16_MAX then this is an unbounded repeat

        RangeRepeatOpt(uint16_t low, uint16_t high, const RegexOpt* repeat) : RegexOpt(RegexOptTag::RangeRepeat), repeat(repeat), low(low), high(high) {;}
        virtual ~RangeRepeatOpt() = default;

        virtual bool needsParens() const override final { return true; }
        virtual std::u8string toBSQONFormat() const override final
        {
            std::u8string repeatstr;
            if(!this->repeat->needsParens()) {
                repeatstr = this->repeat->toBSQONFormat();
            }
            else {
                repeatstr = u8'(' + this->repeat->toBSQONFormat() + u8')';
            }

            std::string iterstr{'{'};
            if(this->low == this->high) {
                iterstr += std::to_string(this->low) + std::string{'}'};
            }
            else {
                if(this->low == 0) {
                    iterstr += std::string{','} + std::to_string(this->high) + std::string{'}'};
                }
                else if(this->high == UINT16_MAX) {
                    iterstr += std::to_string(this->low) + std::string{','} + std::string{'}'};
                }
                else {
                    iterstr += std::to_string(this->low) + std::string{','} + std::to_string(this->high) + std::string{'}'};
                }   
            }

            return repeatstr + std::u8string(iterstr.cbegin(), iterstr.cend());
        }

        static RangeRepeatOpt* jparse(json j)
        {
            auto repeat = RegexOpt::jparse(j["repeat"]);
            auto low = j["low"].get<uint16_t>();
            auto high = (!j.contains("high") || j["high"].is_null()) ? UINT16_MAX : j["high"].get<uint16_t>();

            return new RangeRepeatOpt(low, high, repeat);
        }
    };

    class OptionalOpt : public RegexOpt
    {
    public:
        const RegexOpt* opt;

        OptionalOpt(const RegexOpt* opt) : RegexOpt(RegexOptTag::Optional), opt(opt) {;}
        virtual ~OptionalOpt() = default;

        virtual bool needsParens() const override final { return true; }
        virtual std::u8string toBSQONFormat() const override final
        {
            if (!this->opt->needsParens()) {
                return this->opt->toBSQONFormat() + u8'?';
            }
            else {
                return u8'(' + this->opt->toBSQONFormat() + u8")?";
            }
        }

        static OptionalOpt* jparse(json j)
        {
            auto opt = RegexOpt::jparse(j["opt"]);
            return new OptionalOpt(opt);
        }
    };

    class AnyOfOpt : public RegexOpt
    {
    public:
        const std::vector<const RegexOpt*> opts;

        AnyOfOpt(std::vector<const RegexOpt*> opts) : RegexOpt(RegexOptTag::AnyOf), opts(opts) {;}
        virtual ~AnyOfOpt() = default;

        virtual bool needsParens() const override final { return true; }
        virtual bool needsSequenceParens() const override final { return true; }
        virtual std::u8string toBSQONFormat() const override final
        {
            std::u8string optstr;
            for(auto ii = this->opts.cbegin(); ii != this->opts.cend(); ++ii) {
                if(ii != this->opts.cbegin()) {
                    optstr += u8'|';
                }

                if(!(*ii)->needsParens()) {
                    optstr += (*ii)->toBSQONFormat();
                }
                else {
                    optstr += u8'(' + (*ii)->toBSQONFormat() + u8')';
                }
            }
        
            return optstr;
        }

        static AnyOfOpt* jparse(json j)
        {
            std::vector<const RegexOpt*> opts;
            auto jopts = j["opts"];
            std::transform(jopts.cbegin(), jopts.cend(), std::back_inserter(opts), [](json arg) {
                return RegexOpt::jparse(arg);
            });

            return new AnyOfOpt(opts);
        }
    };

    class SequenceOpt : public RegexOpt
    {
    public:
        const std::vector<const RegexOpt*> regexs;

        SequenceOpt(std::vector<const RegexOpt*> regexs) : RegexOpt(RegexOptTag::Sequence), regexs(regexs) {;}
        virtual ~SequenceOpt() = default;

        virtual bool needsParens() const override final { return true; }
        virtual std::u8string toBSQONFormat() const override final
        {
            std::u8string regexstr;
            for(auto ii = this->regexs.cbegin(); ii != this->regexs.cend(); ++ii) {
                if(!(*ii)->needsSequenceParens()) {
                    regexstr += (*ii)->toBSQONFormat();
                }
                else {
                    regexstr += u8'(' + (*ii)->toBSQONFormat() + u8')';
                }
            }
            
            return regexstr;
        }

        static SequenceOpt* jparse(json j)
        {
            std::vector<const RegexOpt*> regexs;
            auto jregexs = j["regexs"];
            std::transform(jregexs.cbegin(), jregexs.cend(), std::back_inserter(regexs), [](json arg) {
                return RegexOpt::jparse(arg);
            });

            return new SequenceOpt(regexs);
        }
    };

    class RegexToplevelEntry
    {
    public:
        bool isNegated;
        bool isFrontCheck;
        bool isBackCheck;

        const RegexOpt* opt;

        RegexToplevelEntry() : isNegated(false), isFrontCheck(false), isBackCheck(false), opt(nullptr) {;}
        RegexToplevelEntry(bool isNegated, bool isFrontCheck, bool isBackCheck, const RegexOpt* opt) : isNegated(isNegated), isFrontCheck(isFrontCheck), isBackCheck(isBackCheck), opt(opt) {;}
        ~RegexToplevelEntry() = default;

        RegexToplevelEntry(const RegexToplevelEntry& other) = default;
        RegexToplevelEntry(RegexToplevelEntry&& other) = default;

        RegexToplevelEntry& operator=(const RegexToplevelEntry& other) = default;
        RegexToplevelEntry& operator=(RegexToplevelEntry&& other) = default;

        std::u8string toBSQONFormat() const
        {
            std::u8string fstr;
            if(this->isNegated) {
                fstr += u8'!';
            }

            if(this->isFrontCheck) {
                fstr += u8'^';
            }
            
            std::u8string tstr;
            if(this->isBackCheck) {
                tstr = u8'$';
            }

            std::u8string opstr;
            if((fstr.empty() && tstr.empty()) || !this->opt->needsParens()) {
                opstr = this->opt->toBSQONFormat();
            }
            else {
                opstr = u8'(' + this->opt->toBSQONFormat() + u8')';
            }

            return fstr + opstr + tstr;
        }

        static RegexToplevelEntry jparse(json j)
        {
            bool isNegated = j["isNegated"].get<bool>();
            bool isFrontCheck = j["isFrontCheck"].get<bool>();
            bool isBackCheck = j["isBackCheck"].get<bool>();

            auto opt = RegexOpt::jparse(j["opt"]);
            return RegexToplevelEntry(isNegated, isFrontCheck, isBackCheck, opt);
        }
    };

    enum class RegexComponentTag
    {
        Single,
        AllOf
    };

    class RegexComponent
    {
    public:
        RegexComponentTag tag;

        RegexComponent(RegexComponentTag tag) : tag(tag) {;}
        virtual ~RegexComponent() = default;

        virtual std::u8string toBSQONFormat() const = 0;

        static RegexComponent* jparse(json j);

        virtual bool isContainsable() const = 0;
        virtual bool isMatchable() const = 0;

        virtual bool validPreAnchor() const = 0;
        virtual bool validPostAnchor() const = 0;

        virtual bool isValidNamedRegexComponent() const 
        {
            return false;
        }
    };

    class RegexSingleComponent : public RegexComponent
    {
    public:
        const RegexToplevelEntry entry;

        RegexSingleComponent(const RegexToplevelEntry& entry) : RegexComponent(RegexComponentTag::Single), entry(entry) {;}
        ~RegexSingleComponent() = default;

        virtual std::u8string toBSQONFormat() const override final
        {
            return this->entry.toBSQONFormat();
        }

        static RegexSingleComponent* jparse(json j)
        {
            auto entry = RegexToplevelEntry::jparse(j["entry"]);
            return new RegexSingleComponent(entry);
        }

        virtual bool isContainsable() const override final
        {
            if(this->entry.isFrontCheck || this->entry.isBackCheck || this->entry.isNegated) {
                return false;
            }
            else {
                //
                //TODO: This gives us a nice way to know that we can quickly find the contains using Boyer-Moore or some other fast search
                //      Maybe make this warnable (and with ranges) for possible ReDOS issues -- what do bounds look like?
                //

                return true;
            }
        }

        virtual bool isMatchable() const override final
        {
            return !this->entry.isFrontCheck && !this->entry.isBackCheck && !this->entry.isNegated;
        }

        virtual bool validPreAnchor() const override final
        {
            return !this->entry.isFrontCheck && !this->entry.isBackCheck;
        }

        virtual bool validPostAnchor() const override final
        {
            return !this->entry.isBackCheck && !this->entry.isBackCheck;
        }

        virtual bool isValidNamedRegexComponent() const override final
        {
           return !this->entry.isFrontCheck && !this->entry.isBackCheck && !this->entry.isNegated;
        }
    };

    class RegexAllOfComponent : public RegexComponent
    {
    public:
        std::vector<RegexToplevelEntry> musts;

        RegexAllOfComponent(const std::vector<RegexToplevelEntry>& musts) : RegexComponent(RegexComponentTag::AllOf), musts(musts) {;}
        virtual ~RegexAllOfComponent() = default;

        std::u8string toBSQONFormat() const override final
        {
            std::u8string muststr;
            for(auto ii = this->musts.cbegin(); ii != this->musts.cend(); ++ii) {
                if(ii != this->musts.cbegin()) {
                    muststr += u8" & ";
                }

                muststr += ii->toBSQONFormat();
            }
            
            return muststr;
        }

        static RegexAllOfComponent* jparse(json j)
        {
            auto jmusts = j["musts"];
            std::vector<RegexToplevelEntry> musts;

            std::transform(jmusts.cbegin(), jmusts.cend(), std::back_inserter(musts), [](json arg) {
                return RegexToplevelEntry::jparse(arg);
            });

            return new RegexAllOfComponent(musts);
        }

        virtual bool isContainsable() const override final
        {
            return false;
        }

        virtual bool isMatchable() const override final
        {
            return std::any_of(this->musts.cbegin(), this->musts.cend(), [](const RegexToplevelEntry& opt) { 
                return !opt.isNegated && !opt.isFrontCheck && !opt.isBackCheck;
            });
        }

        virtual bool validPreAnchor() const override final
        {
            bool allfree = std::all_of(this->musts.cbegin(), this->musts.cend(), [](const RegexToplevelEntry& opt) { 
                return !opt.isFrontCheck && !opt.isBackCheck;
            });

            return this->isMatchable() && allfree;
        }

        virtual bool validPostAnchor() const override final
        {
            bool allfree = std::all_of(this->musts.cbegin(), this->musts.cend(), [](const RegexToplevelEntry& opt) { 
                return !opt.isFrontCheck && !opt.isBackCheck;
            });

            return this->isMatchable() && allfree;
        }
    };

    enum class RegexKindTag
    {
        Std,
        Path
    };

    enum class RegexCharInfoTag
    {
        Unicode,
        Char
    };

    class Regex
    {
    public:
        const RegexKindTag rtag;
        const RegexCharInfoTag ctag;

        const RegexComponent* preanchor;
        const RegexComponent* postanchor;
        const RegexComponent* re;

        Regex(RegexKindTag rtag, RegexCharInfoTag ctag, const RegexComponent* preanchor, const RegexComponent* postanchor, const RegexComponent* re): rtag(rtag), ctag(ctag), preanchor(preanchor), postanchor(postanchor), re(re) {;}
        ~Regex() = default;

        static Regex* jparse(json j)
        {
            auto rtag = (!j.contains("isPath") || j["isPath"].is_null()) ? RegexKindTag::Std : RegexKindTag::Path;
            auto ctag = (!j.contains("isChar") || !j["isChar"].get<bool>()) ? RegexCharInfoTag::Char : RegexCharInfoTag::Unicode;

            auto preanchor = (!j.contains("preanchor") || j["preanchor"].is_null()) ? nullptr : RegexComponent::jparse(j["preanchor"]);
            auto postanchor = (!j.contains("postanchor") || j["postanchor"].is_null()) ? nullptr : RegexComponent::jparse(j["postanchor"]);
            auto re = RegexComponent::jparse(j["re"]);

            return new Regex(rtag, ctag, preanchor, postanchor, re);
        }

        std::u8string toBSQONFormat() const
        {
            std::u8string fstr;
            if(this->preanchor != nullptr) {
                fstr += this->preanchor->toBSQONFormat() + u8"^";
            }

            if(this->preanchor != nullptr || this->postanchor != nullptr) {
                fstr += u8"<";
            }
            fstr += this->re->toBSQONFormat();
            if(this->preanchor != nullptr || this->postanchor != nullptr) {
                fstr += u8">";
            }

            if(this->postanchor != nullptr) {
                fstr += u8"$" + this->postanchor->toBSQONFormat();
            }

            std::u8string fchar = u8"";
            if(this->rtag == RegexKindTag::Path) {
                fchar = u8"p";
            }
            else {
                if(this->ctag == RegexCharInfoTag::Char) {
                    fchar = u8"c";
                }
            }

            return u8'/' + fstr + u8'/' + fchar;
        }

        std::u8string toBSQONGlobFormat() const 
        {
            BREX_ASSERT(this->ctag == RegexCharInfoTag::Char, "only char regexes can be converted to glob format");
            BREX_ASSERT(this->preanchor == nullptr, "only regexes without a preanchor can be converted to glob format");
            BREX_ASSERT(this->postanchor == nullptr, "only regexes without a postanchor can be converted to glob format");

            return u8'<' + this->re->toBSQONFormat() + u8'>';
        }

        bool canUseInTestOperation() const
        {
            if(this->preanchor == nullptr && this->postanchor == nullptr) {
                return true;
            }
            else {
                return this->re->isContainsable();
            }
        }

        bool canStartsOperation() const
        {
            return this->preanchor == nullptr && this->re->isMatchable();
        }

        bool canEndOperation() const
        {
            return this->postanchor == nullptr && this->re->isMatchable();
        }

        bool canUseInContains() const
        {
            return this->re->isContainsable();
        }

        bool isValidNamedRegexComponent() const 
        {
            return this->preanchor == nullptr && this->postanchor == nullptr && this->re->isValidNamedRegexComponent();
        }
    };
}
