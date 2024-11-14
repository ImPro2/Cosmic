#pragma once
#include "Base/Base.hpp"

namespace Cosmic
{

    class FileSystem;

    class File
    {
    public:
        const String GetName();
        const String GetExtension();
        const String GetNameAndExtension();

        const String GetAbsolutePath();
        const String GetParentDirectory();

        const size_t GetSize();

        const String Read();
        const uint8* ReadBinary();

        void Write(const StringView text);
        void WriteBinary(const unsigned char* text);

        File(const std::string_view absolutePath)
            : mAbsolutePath(absolutePath)
        {
        }

    private:
        void SetAbsolutePath(const StringView absolutePath)
        {
            mAbsolutePath = absolutePath;
        }

    private:
        String mAbsolutePath;

    private:
        friend class FileSystem;
    };

}
