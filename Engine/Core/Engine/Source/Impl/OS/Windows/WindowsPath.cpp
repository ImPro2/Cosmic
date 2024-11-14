#include "cspch.hpp"

#ifdef CS_PLATFORM_WINDOWS

#include "App/Path.hpp"
#include <Windows.h>

namespace Cosmic
{

    Path::Path()
        : mPath("")
    {
    }

    Path::Path(const String& path)
        : mPath(path)
    {
    }

    Path::Path(const StringView path)
        : mPath(path)
    {
    }

    bool Path::IsAbsolute() const
    {
        return !IsRelative();
    }

    bool Path::IsRelative() const
    {
        return ::PathIsRelativeA(mPath.c_str());
    }

    Path operator /(const Path& lhs, const Path& rhs)
    {
        StringView lhsStr = StringView(lhs.GetString().data(), lhs.GetString().size());
        StringView rhsStr = StringView(rhs.GetString().data(), rhs.GetString().size());

        if (lhsStr[lhsStr.size() - 1] == '/')
            lhsStr = StringView(lhs.GetString().data(), lhs.GetString().size() - 1);
        
        return Path(std::format("{}{}{}", lhsStr.data(), '/', rhsStr.data()));
    }

}

#endif
