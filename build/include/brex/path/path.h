#pragma once

#include "../common.h"

namespace bpath
{
    class AuthorityInfo
    {
    public:
        const std::optional<brex::CString> userinfo;
        const brex::CString host;

        AuthorityInfo(std::optional<brex::CString> userinfo, brex::CString host) : userinfo(userinfo), host(host) {;}

        std::u8string toBSQONFormat() const
        {
            std::u8string res = u8"//";
            if(this->userinfo.has_value()) {
                res += std::u8string(this->userinfo.value().cbegin(), this->userinfo.value().cend());
                res += u8"@";
            }
            res += std::u8string(this->host.cbegin(), this->host.cend());
            return res;
        }
    };

    class ElementInfo
    {
    public:
        const brex::CString ename;
        const std::optional<brex::CString> ext;

        ElementInfo(brex::CString ename, std::optional<brex::CString> ext) : ename(ename), ext(ext) {;}

        std::u8string toBSQONFormat() const
        {
            std::u8string res = std::u8string(this->ename.cbegin(), this->ename.cend());
            if(this->ext.has_value()) {
                res += u8".";
                res += std::u8string(this->ext.value().cbegin(), this->ext.value().cend());
            }
            return res;
        }
    };

    class Path
    {
    public:
        const brex::CString scheme;
        const std::optional<AuthorityInfo> authorityinfo;
        const std::vector<brex::CString> segments;
        const std::optional<ElementInfo> elementinfo;

        const bool tailingslash; //cannot have elementinfo and tailingSlash as false

        Path(brex::CString scheme, std::optional<AuthorityInfo> authorityinfo, std::vector<brex::CString> segments, std::optional<ElementInfo> elementinfo, bool tailingslash) : scheme(scheme), authorityinfo(authorityinfo), segments(segments), elementinfo(elementinfo), tailingslash(tailingslash) {;}
        ~Path() {;}

        std::u8string toBSQONFormat() const
        {
            std::u8string res = std::u8string(this->scheme.cbegin(), this->scheme.cend());
            res += u8":";
            if(this->authorityinfo.has_value()) {
                res += this->authorityinfo.value().toBSQONFormat();
            }
            res += u8"/";
            for(size_t i = 0; i < this->segments.size(); ++i) {
                res += std::u8string(this->segments[i].cbegin(), this->segments[i].cend());
                
                if(i != this->segments.size() - 1) {
                    res += u8"/";
                }
            }
            
            if(tailingslash) {
                res += u8"/";
            }
            if(this->elementinfo.has_value()) {
                res += u8"/" + this->elementinfo.value().toBSQONFormat();
            }
            
            return res;
        }
    };
}
