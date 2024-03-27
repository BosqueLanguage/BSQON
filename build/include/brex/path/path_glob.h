#pragma once

#include "../common.h"
#include "path.h"

#include "../regex/brex.h"

namespace bpath
{
    enum class GlobSimpleComponentTag
    {
        LITERAL_TAG,
        WILDCARD_TAG,
        REGEX_TAG
    };

    class GlobSimpleComponent
    {
    public:
        const GlobSimpleComponentTag type;

        GlobSimpleComponent(GlobSimpleComponentTag type) : type(type) {;}
        virtual ~GlobSimpleComponent() {;}

        virtual std::u8string toBSQONFormat() const = 0;

        static GlobSimpleComponent* jparse(json jv);
    };

    class LiteralComponent : public GlobSimpleComponent
    {
    public:
        const brex::ASCIIString value;

        LiteralComponent(brex::ASCIIString value) : GlobSimpleComponent(GlobSimpleComponentTag::LITERAL_TAG), value(value) {;}
        virtual ~LiteralComponent() {;}

        std::u8string toBSQONFormat() const override final
        {
            return std::u8string(this->value.cbegin(), this->value.cend());
        }

        static LiteralComponent* jparse(json jv);
    };

    class WildcardComponent : public GlobSimpleComponent
    {
    public:
        WildcardComponent() : GlobSimpleComponent(GlobSimpleComponentTag::WILDCARD_TAG) {;}
        virtual ~WildcardComponent() {;}

        std::u8string toBSQONFormat() const override final
        {
            return u8"*";
        }

        static WildcardComponent* jparse(json jv);
    };

    class RegexComponent : public GlobSimpleComponent
    {
    public:
        const brex::Regex* re;

        RegexComponent(const brex::Regex* re) : GlobSimpleComponent(GlobSimpleComponentTag::REGEX_TAG), re(re) {;}
        virtual ~RegexComponent() {;}

        std::u8string toBSQONFormat() const override final
        {
            return re->toBSQONGlobFormat();
        }

        static RegexComponent* jparse(json jv);
    };

    enum class GlobSegmentComponentTag
    {
        SEGMENT_LITERAL_TAG,
        SEGMENT_WILDCARD_TAG,
        SEGMENT_REGEX_TAG,
        SEGMENT_EXPANSIVE_WILDCARD_TAG,
        SEGMENT_EXPANSIVE_STAR_TAG,
        SEGMENT_EXPANSIVE_PLUS_TAG,
        SEGMENT_EXPANSIVE_RANGE_TAG,
        SEGMENT_EXPANSIVE_QUESTION_TAG
    };

    class SegmentGlobCompnent
    {
    public:
        const GlobSegmentComponentTag tag;
    
        SegmentGlobCompnent(GlobSegmentComponentTag tag) : tag(tag) {;}
        virtual ~SegmentGlobCompnent() {;}

        virtual std::u8string toBSQONFormat() const = 0;

        static SegmentGlobCompnent* jparse(json jv);
    };

    class SegmentLiteralComponent : public SegmentGlobCompnent
    {
    public:
        const brex::ASCIIString value;

        SegmentLiteralComponent(brex::ASCIIString value) : SegmentGlobCompnent(GlobSegmentComponentTag::SEGMENT_LITERAL_TAG), value(value) {;}
        virtual ~SegmentLiteralComponent() {;}

        std::u8string toBSQONFormat() const override final
        {
            return std::u8string(this->value.cbegin(), this->value.cend());
        }

        static SegmentLiteralComponent* jparse(json jv);
    };
    
    class SegmentWildcardComponent : public SegmentGlobCompnent
    {
    public:
        SegmentWildcardComponent() : SegmentGlobCompnent(GlobSegmentComponentTag::SEGMENT_WILDCARD_TAG) {;}
        virtual ~SegmentWildcardComponent() {;}

        std::u8string toBSQONFormat() const override final
        {
            return u8"*";
        }

        static SegmentWildcardComponent* jparse(json jv);
    };

    class SegmentRegexComponent : public SegmentGlobCompnent
    {
    public:
        const brex::Regex* re;

        SegmentRegexComponent(const brex::Regex* re) : SegmentGlobCompnent(GlobSegmentComponentTag::SEGMENT_REGEX_TAG), re(re) {;}
        virtual ~SegmentRegexComponent() {;}

        std::u8string toBSQONFormat() const override final
        {
            return re->toBSQONGlobFormat();
        }

        static SegmentRegexComponent* jparse(json jv);
    };

    class SegmentExpansiveComponent : public SegmentGlobCompnent
    {
    public:
        SegmentExpansiveComponent(GlobSegmentComponentTag type) : SegmentGlobCompnent(type) {;}
        virtual ~SegmentExpansiveComponent() {;}
    };

    class SegmentExpansiveWildcardComponent : public SegmentExpansiveComponent
    {
    public:
        SegmentExpansiveWildcardComponent() : SegmentExpansiveComponent(GlobSegmentComponentTag::SEGMENT_EXPANSIVE_WILDCARD_TAG) {;}
        virtual ~SegmentExpansiveWildcardComponent() {;}

        std::u8string toBSQONFormat() const override final
        {
            return u8"**";
        }

        static SegmentExpansiveWildcardComponent* jparse(json jv);
    };

    class SegmentExpansiveRegexComponent : public SegmentExpansiveComponent
    {
    public:
        const brex::Regex* re;

        SegmentExpansiveRegexComponent(GlobSegmentComponentTag type, const brex::Regex* re) : SegmentExpansiveComponent(type), re(re) {;}
        virtual ~SegmentExpansiveRegexComponent() {;} 
    };

    class SegmentExpansiveStarComponent : public SegmentExpansiveRegexComponent
    {
    public:
        SegmentExpansiveStarComponent(const brex::Regex* re) : SegmentExpansiveRegexComponent(GlobSegmentComponentTag::SEGMENT_EXPANSIVE_STAR_TAG, re) {;}
        virtual ~SegmentExpansiveStarComponent() {;}

        std::u8string toBSQONFormat() const override final
        {
            return re->toBSQONGlobFormat() + u8'*';
        }

        static SegmentExpansiveStarComponent* jparse(json jv);
    };

    class SegmentExpansivePlusComponent : public SegmentExpansiveRegexComponent
    {
    public:
        SegmentExpansivePlusComponent(const brex::Regex* re) : SegmentExpansiveRegexComponent(GlobSegmentComponentTag::SEGMENT_EXPANSIVE_PLUS_TAG, re) {;}
        virtual ~SegmentExpansivePlusComponent() {;}

        std::u8string toBSQONFormat() const override final
        {
            return re->toBSQONGlobFormat() + u8'+';
        }

        static SegmentExpansivePlusComponent* jparse(json jv);
    };

    class SegmentExpansiveRangeComponent : public SegmentExpansiveRegexComponent
    {
    public:
        const int64_t low;
        const int64_t high;

        SegmentExpansiveRangeComponent(const brex::Regex* re, int64_t low, int64_t high) : SegmentExpansiveRegexComponent(GlobSegmentComponentTag::SEGMENT_EXPANSIVE_RANGE_TAG, re), low(low), high(high) {;}
        virtual ~SegmentExpansiveRangeComponent() {;}

        std::u8string toBSQONFormat() const override final
        {
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
            
            return re->toBSQONGlobFormat() + std::u8string(iterstr.cbegin(), iterstr.cend());
        }

        static SegmentExpansiveRangeComponent* jparse(json jv);
    };

    class SegmentExpansiveQuestionComponent : public SegmentExpansiveRegexComponent
    {
    public:
        SegmentExpansiveQuestionComponent(const brex::Regex* re) : SegmentExpansiveRegexComponent(GlobSegmentComponentTag::SEGMENT_EXPANSIVE_QUESTION_TAG, re) {;}
        virtual ~SegmentExpansiveQuestionComponent() {;}

        std::u8string toBSQONFormat() const override final
        {
            return re->toBSQONGlobFormat() + u8'?';
        }

        static SegmentExpansiveQuestionComponent* jparse(json jv);
    };

    class GlobAuthorityInfo
    {
    public:
        const std::optional<GlobSimpleComponent*> userinfo;
        const GlobSimpleComponent* host;

        GlobAuthorityInfo(std::optional<GlobSimpleComponent*> userinfo, GlobSimpleComponent* host) : userinfo(userinfo), host(host) {;}

        std::u8string toBSQONFormat() const
        {
            if(this->userinfo.has_value()) {
                return this->userinfo.value()->toBSQONFormat() + u8'@' + this->host->toBSQONFormat();
            }
            else {
                return this->host->toBSQONFormat();
            }
        }

        static GlobAuthorityInfo* jparse(json jv);
    };

    class GlobElementInfo
    {
    public:
        const GlobSimpleComponent* ename;
        const std::optional<GlobSimpleComponent*> ext;

        GlobElementInfo(GlobSimpleComponent* ename, std::optional<GlobSimpleComponent*> ext) : ename(ename), ext(ext) {;}

        std::u8string toBSQONFormat() const
        {
            if(this->ext.has_value()) {
                return this->ename->toBSQONFormat() + u8'.' + this->ext.value()->toBSQONFormat();
            }
            else {
                return this->ename->toBSQONFormat();
            }
        }

        static GlobElementInfo* jparse(json jv);
    };

    class PathGlob
    {
    //
    //  /x/y/*     <-- all files in y
    //  /x/y/*/    <-- all directories in y
    //
    //  /x/y/**   <-- all files (recursively) reachable from y
    //  /x/y/**/  <-- all directories (recursively) reachable from in y
    //
    //  /x/y/*.*     <-- all files in y with an extension
    //  /x/y/**/*.*  <-- all files (recursively) reachable y with an extension
    //
    public:
        const std::optional<GlobSimpleComponent*> scheme;
        const std::optional<GlobAuthorityInfo*> authorityinfo;
        const std::vector<SegmentGlobCompnent*> segments;
        const std::optional<GlobElementInfo*> elementinfo;

        const bool tailingslash; //cannot have elementinfo and tailingSlash as false

        PathGlob(std::optional<GlobSimpleComponent*> scheme, std::optional<GlobAuthorityInfo*> authorityinfo, std::vector<SegmentGlobCompnent*> segments, std::optional<GlobElementInfo*> elementinfo, bool tailingslash) : scheme(scheme), authorityinfo(authorityinfo), segments(segments), elementinfo(elementinfo), tailingslash(tailingslash) {;}

        std::u8string toBSQONFormat() const
        {
            std::u8string res;
            if(this->scheme.has_value()) {
                res += this->scheme.value()->toBSQONFormat() + u8":";
            }

            if(this->authorityinfo.has_value()) {
                res += u8"//" + this->authorityinfo.value()->toBSQONFormat();
            }

            if(this->scheme.has_value() || this->authorityinfo.has_value()) {
                res += u8"/";
            }
            
            for(size_t i = 0; i < this->segments.size(); ++i) {
                res += this->segments[i]->toBSQONFormat();
                
                if(i != this->segments.size() - 1) {
                    res += u8"/";
                }
            }
            
            if(tailingslash) {
                res += u8"/";
            }
            if(this->elementinfo.has_value()) {
                res += u8"/" + this->elementinfo.value()->toBSQONFormat();
            }
            
            return res;
        }

        static PathGlob* jparse(json jv);
    };

    typedef PathGlob ResourceDescriptor;
}
