#pragma once
#include "Types.hpp"

namespace Cosmic::StringUtils
{

    Vector<String> Split(const String&    str, const StringView delimiters);
    Vector<String> Split(const String&    str, char             delimiter);
    Vector<String> Split(const StringView str, const StringView delimiters);
    Vector<String> Split(const StringView str, char             delimiter);

    Vector<String> SplitInLines(const String& str);

    uint32         FindPosition(const String&    str, const StringView search, uint32 offset);
    uint32         FindPosition(const StringView str, const StringView search, uint32 offset);

    StringView     Range(const String&    str, uint32 start, uint32 length);
    StringView     Range(const StringView str, uint32 start, uint32 length);

    String         RemoveRange(const String&    str, uint32 start, uint32 length);

    bool           Contains(const String&    str, const StringView chars);
    bool           Contains(const StringView str, const StringView chars);

    bool           StartsWith(const String&    str, const StringView start);
    bool           StartsWith(const StringView str, const StringView start);

    String         Replace(String str, char ch1, char ch2);
}