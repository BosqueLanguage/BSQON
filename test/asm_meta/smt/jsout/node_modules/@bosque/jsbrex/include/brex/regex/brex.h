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

        virtual std::string toBSQStandard() const = 0;
        virtual std::string toSMTRegex() const = 0;
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

        virtual std::string toBSQStandard() const override final
        {
            if(this->isunicode) {
                return "\"" + processRegexCharsToBsqStandard(this->codes) + "\"";
            }
            else {
                return "'" + processRegexCharsToBsqStandard(this->codes) + "'";
            }
        }

        virtual std::string toSMTRegex() const override final
        {
            return "(str.to.re \"" + processRegexCharsToSMT(this->codes) + "\")";
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

        virtual std::string toBSQStandard() const override final
        {
            std::string rngs = "[";
            if(this->compliment) {
                rngs.push_back('^');
            }

            for(auto ii = this->ranges.cbegin(); ii != this->ranges.cend(); ++ii) {
                auto cr = *ii;

                auto lowbytes = processRegexCharToBsqStandard(cr.low);
                rngs.append(lowbytes.cbegin(), lowbytes.cend());

                if(cr.low != cr.high) {
                    rngs.push_back('-');
                    
                    auto highbytes = processRegexCharToBsqStandard(cr.high);
                    rngs.append(highbytes.cbegin(), highbytes.cend());
                }
            }
            rngs.push_back(']');

            return rngs;
        }

        virtual std::string toSMTRegex() const override final
        {
            std::vector<std::string> opts;
            for(auto ii = this->ranges.cbegin(); ii != this->ranges.cend(); ++ii) {
                auto cr = *ii;

                if(!this->compliment) {
                    auto lowbytes = "\"" + processRegexCharToSMT(cr.low) + "\"";
                    if(cr.low == cr.high) {
                        opts.push_back("(str.to.re " + lowbytes + ")");
                    }
                    else {
                        auto highbytes = "\"" + processRegexCharToSMT(cr.high) + "\"";
                        opts.push_back("(re.range " + lowbytes + " " + highbytes + ")");
                    }
                }
                else {
                    RegexChar cslow = this->isunicode ? (RegexChar)0 : (RegexChar)9;
                    std::string cslowbytes = "\"" + processRegexCharToSMT(cslow) + "\"";

                    RegexChar cshigh = this->isunicode ? (RegexChar)0x10FFFF : (RegexChar)126;
                    std::string cshighbytes = "\"" + processRegexCharToSMT(cshigh) + "\"";
                    
                    auto highbytes = "\"" + processRegexCharToSMT(cr.high + 1) + "\"";
                    auto hrstr = "(re.range " + highbytes + " " + cshighbytes + ")";

                    if(cr.low == cslow) {
                        opts.push_back(hrstr);
                    }
                    else {
                        auto lowbytes = "\"" + processRegexCharToSMT(cr.low - 1) + "\"";
                        auto lowstr = "(re.range " + cslowbytes + " " + lowbytes + ")";

                        opts.push_back("(re.union " + lowstr + " " + hrstr + ")");
                    }
                }
            }

            std::string optsstr;
            if(opts.size() == 1) {
                optsstr = opts.front();
            }
            else {
                std::string cop = this->compliment ? std::string{"re.inter"} : std::string{"re.union"};

                optsstr = "(" + cop + std::accumulate(opts.cbegin(), opts.cend(), std::string{""}, [](const std::string& acc, const std::string& opt) {
                    return acc + " " + opt;
                }) + ")";
            }

            return optsstr;
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

        virtual std::string toBSQStandard() const override final
        {
            return ".";
        }

        virtual std::string toSMTRegex() const override final
        {
            return "re.allchar";
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

        virtual std::string toBSQStandard() const override final
        {
            return "${" + this->rname + "}";
        }

        virtual std::string toSMTRegex() const override final
        {
            return "${" + this->rname + "}";
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

        virtual std::string toBSQStandard() const override final
        {
            return "env[" + this->ename + ']';
        }

        virtual std::string toSMTRegex() const override final
        {
            return "env[" + this->ename + ']';
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

        virtual std::string toBSQStandard() const override final
        {
            if(!this->repeat->needsParens()) {
                return this->repeat->toBSQStandard() + "*";
            }
            else {
                return "(" + this->repeat->toBSQStandard() + ")*";
            }
        }

        virtual std::string toSMTRegex() const override final
        {
            return "(re.* " + this->repeat->toSMTRegex() + ")";
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

        virtual std::string toBSQStandard() const override final
        {
            if (!this->repeat->needsParens()) {
                return this->repeat->toBSQStandard() + "+";
            }
            else {
                return "(" + this->repeat->toBSQStandard() + ")+";
            }
        }

        virtual std::string toSMTRegex() const override final
        {
            return "(re.+ " + this->repeat->toSMTRegex() + ")";
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

        virtual std::string toBSQStandard() const override final
        {
            std::string repeatstr;
            if(!this->repeat->needsParens()) {
                repeatstr = this->repeat->toBSQStandard();
            }
            else {
                repeatstr = '(' + this->repeat->toBSQStandard() + ')';
            }

            std::string iterstr{'{'};
            if(this->low == this->high) {
                iterstr += std::to_string(this->low) + '}';
            }
            else {
                if(this->low == 0) {
                    iterstr += ',' + std::to_string(this->high) + '}';
                }
                else if(this->high == UINT16_MAX) {
                    iterstr += std::to_string(this->low) + ",}";
                }
                else {
                    iterstr += std::to_string(this->low) + ',' + std::to_string(this->high) + '}';
                }   
            }

            return repeatstr + iterstr;
        }

        virtual std::string toSMTRegex() const override final
        {
            if(this->high == UINT16_MAX) {
                //Unbounded repeat
                std::string iterstr = std::to_string(this->low) + " " + std::to_string(this->low);
                std::string repeatstr = this->repeat->toSMTRegex();
                
                return "(re.++ ((_ re.loop " + iterstr + ") " + repeatstr + ") " + "(re.* " + repeatstr + "))";
            }
            else {
                std::string iterstr = std::to_string(this->low) + " " + std::to_string(this->high);
                std::string repeatstr = this->repeat->toSMTRegex();

                return "((_ re.loop " + iterstr + ") " + repeatstr + ")";
            }
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

        virtual std::string toBSQStandard() const override final
        {
            if (!this->opt->needsParens()) {
                return this->opt->toBSQStandard() + "?";
            }
            else {
                return "(" + this->opt->toBSQStandard() + ")?";
            }
        }

        virtual std::string toSMTRegex() const override final
        {
            return "(re.opt " + this->opt->toSMTRegex() + ")";
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

        virtual std::string toBSQStandard() const override final
        {
            std::string optstr;
            for(auto ii = this->opts.cbegin(); ii != this->opts.cend(); ++ii) {
                if(ii != this->opts.cbegin()) {
                    optstr += '|';
                }

                if(!(*ii)->needsParens()) {
                    optstr += (*ii)->toBSQStandard();
                }
                else {
                    optstr += '(' + (*ii)->toBSQStandard() + ')';
                }
            }
        
            return optstr;
        }

        virtual std::string toSMTRegex() const override final
        {
            std::string optstr = "(re.union";
            for(auto ii = this->opts.cbegin(); ii != this->opts.cend(); ++ii) {
                optstr += " " + (*ii)->toSMTRegex();
            }
            optstr += ")";

            return optstr;
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

        virtual std::string toBSQStandard() const override final
        {
            std::string regexstr;
            for(auto ii = this->regexs.cbegin(); ii != this->regexs.cend(); ++ii) {
                if(!(*ii)->needsSequenceParens()) {
                    regexstr += (*ii)->toBSQStandard();
                }
                else {
                    regexstr += '(' + (*ii)->toBSQStandard() + ')';
                }
            }
            
            return regexstr;
        }

        virtual std::string toSMTRegex() const override final
        {
            std::string regexstr = "(re.++";
            for(auto ii = this->regexs.cbegin(); ii != this->regexs.cend(); ++ii) {
                regexstr += " " + (*ii)->toSMTRegex();
            }
            regexstr += ")";

            return regexstr;
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

        std::string toBSQStandard() const
        {
            std::string fstr;
            if(this->isNegated) {
                fstr += '!';
            }

            if(this->isFrontCheck) {
                fstr += '^';
            }
            
            std::string tstr;
            if(this->isBackCheck) {
                tstr = '$';
            }

            std::string opstr;
            if((fstr.empty() && tstr.empty()) || !this->opt->needsParens()) {
                opstr = this->opt->toBSQStandard();
            }
            else {
                opstr = '(' + this->opt->toBSQStandard() + ')';
            }

            return fstr + opstr + tstr;
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
        virtual std::string toBSQStandard() const = 0;

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

        virtual std::string toBSQStandard() const override final
        {
            return this->entry.toBSQStandard();
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

        virtual std::u8string toBSQONFormat() const override final
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

        virtual std::string toBSQStandard() const override final
        {
            std::string muststr;
            for(auto ii = this->musts.cbegin(); ii != this->musts.cend(); ++ii) {
                if(ii != this->musts.cbegin()) {
                    muststr += " & ";
                }

                muststr += ii->toBSQStandard();
            }
            
            return muststr;
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

        std::string toBSQStandard() const
        {
            std::string fstr;
            if(this->preanchor != nullptr) {
                fstr += this->preanchor->toBSQStandard() + '^';
            }

            if(this->preanchor != nullptr || this->postanchor != nullptr) {
                fstr += '<';
            }
            fstr += this->re->toBSQStandard();
            if(this->preanchor != nullptr || this->postanchor != nullptr) {
                fstr += '>';
            }

            if(this->postanchor != nullptr) {
                fstr += '$' + this->postanchor->toBSQStandard();
            }

            std::string fchar = "";
            if(this->rtag == RegexKindTag::Path) {
                fchar = "p";
            }
            else {
                if(this->ctag == RegexCharInfoTag::Char) {
                    fchar = "c";
                }
            }

            return '/' + fstr + '/' + fchar;
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
