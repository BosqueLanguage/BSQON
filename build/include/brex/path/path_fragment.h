#pragma once

#include "../common.h"
#include "path.h"

namespace bpath
{
    class PathFragment
    {
    public:
        const std::optional<brex::CString> scheme;
        const std::optional<AuthorityInfo> authorityinfo;
        const std::optional<std::vector<brex::CString>> segments;
        const std::optional<ElementInfo> elementinfo;

        static PathFragment* jparse(json jv)
        {
            return nullptr;
        }

        static bool test(const std::string& path)
        {
            return true;
        }
    };
}
