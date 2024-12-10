#pragma once
#include "Base/Types.hpp"

#include <cstddef>

namespace Cosmic
{

    class File;

    class Path
    {
    public:
        Path();
        Path(const char* path);
        Path(const String& path);
        Path(const StringView path);

    public:
        bool IsAbsolute() const;
        bool IsRelative() const;

        String GetBase() const; // part after parent dir
        Path   RelativeFrom(const Path& other) const;
    
    public:
        inline bool operator==(const Path& other) const
        {
            return mPath == other.mPath;
        }

        void operator/=(const Path& other);

    public:
        operator String() const { return mPath; }
        const String& GetString() const { return mPath; }

    private:
        String mPath;
    };

    Path operator /(const Path& lhs, const Path& rhs);
    Path operator /(const Path& lhs, const File& rhs);

}

template<>
struct std::hash<Cosmic::Path>
{

	size_t operator()(const Cosmic::Path& path) const
	{
		return std::hash<std::string>()(path.GetString());
	}

};
