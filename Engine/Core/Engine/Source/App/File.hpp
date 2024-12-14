#pragma once
#include "App/Path.hpp"

namespace Cosmic
{

    class FileSystem;

    class File
    {
    public:
        File() = default;
        File(const Path& absolutePath)
            : mAbsolutePath(absolutePath)
        {
        }

        File(const StringView absolutePath)
            : mAbsolutePath(absolutePath)
        {
        }

    public:
        operator Path() const { return mAbsolutePath; }

    public:
        const String GetName();
        const String GetExtension();
        const String GetNameAndExtension();

        const Path GetAbsolutePath() const;

    public:
        const size_t GetSize();

        const String Read();
        const uint8* ReadBinary();

        void Write(const StringView text);
        void WriteBinary(const uint8* data, size_t size);

    private:
        Path mAbsolutePath;

    private:
        friend class FileSystem;
    };

}
