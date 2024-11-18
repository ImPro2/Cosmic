#include "cspch.hpp"

#ifdef CS_PLATFORM_LINUX

#include "App/Path.hpp"
#include "App/File.hpp"

#include <filesystem>

namespace Cosmic
{

    Path::Path()
        : mPath("")
    {
    }

    Path::Path(const char* path)
        : mPath(path)
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
        return (mPath[0] == '/');
    }

    bool Path::IsRelative() const
    {
        return !IsAbsolute();
    }

    String Path::GetBase() const
    {
        return std::filesystem::path(mPath).filename().string();
    }

    void Path::operator/=(const Path& other)
    {
        mPath = (mPath / other).mPath;
    }

    Path operator /(const Path& lhs, const Path& rhs)
    {
        StringView lhsStr = StringView(lhs.GetString().data(), lhs.GetString().size());
        StringView rhsStr = StringView(rhs.GetString().data(), rhs.GetString().size());

        if (lhsStr[lhsStr.size() - 1] == '/')
            lhsStr = StringView(lhs.GetString().data(), lhs.GetString().size() - 1);
        
        return Path(std::format("{}{}{}", lhsStr.data(), '/', rhsStr.data()));
    }

    Path operator /(const Path& lhs, const File& rhs)
    {
        return lhs / rhs.GetAbsolutePath();
    }

}

#endif
