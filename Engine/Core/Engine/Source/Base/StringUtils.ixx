module;
#include "cspch.hpp"
export module Cosmic.Base.StringUtils;

import Cosmic.Base.Types;

namespace Cosmic::StringUtils
{

    export Vector<String> Split(const String&    str, const StringView delimiters);
    export Vector<String> Split(const String&    str, char             delimiter);
    export Vector<String> Split(const StringView str, const StringView delimiters);
    export Vector<String> Split(const StringView str, char             delimiter);

    export Vector<String> SplitInLines(const String& str);

    export uint32         FindPosition(const String&    str, const StringView search, uint32 offset);
    export uint32         FindPosition(const StringView str, const StringView search, uint32 offset);

    export StringView     Range(const String&    str, uint32 start, uint32 length);
    export StringView     Range(const StringView str, uint32 start, uint32 length);

    export String         RemoveRange(const String&    str, uint32 start, uint32 length);

    export bool           Contains(const String&    str, const StringView chars);
    export bool           Contains(const StringView str, const StringView chars);

    export bool           StartsWith(const String&    str, const StringView start);
    export bool           StartsWith(const StringView str, const StringView start);

    export String         Replace(String str, char ch1, char ch2);
}