module;
#include "cspch.hpp"
export module Cosmic.App.File;

import Cosmic.Base;

namespace Cosmic
{

    class FileSystem;

    export class File
    {
    public:
        const StringView GetName();
        const StringView GetExtension();
        const String     GetNameAndExtension();


        const StringView GetAbsolutePath();
        const StringView GetParentDirectory();

        const size_t     GetSize();

        const String     Read();
        const uint8* ReadBinary();

        void             Write(const StringView text);
        void             WriteBinary(const unsigned char* text);

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
