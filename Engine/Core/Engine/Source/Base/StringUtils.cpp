module;
#include "cspch.hpp"
module Cosmic.Base.StringUtils;

namespace Cosmic::StringUtils
{

    Vector<String> Split(const String& str, const StringView delimiters)
    {
        size_t start = 0;
        size_t end   = str.find_first_of(delimiters);

        Vector<String> result;
        result.reserve(10); // it's probably not going to exceed more than ten 'splits'

        while (end <= String::npos)
        {
            StringView token = StringView(str.c_str() + start, end - start);
            
            if (!token.empty())
                result.emplace_back(token.data());

            if (end == String::npos)
                break;

            start = end + 1;
            end   = str.find_first_of(delimiters, start);
        }

        return result;
    }

    Vector<String> Split(const String& str, char delimiter)
    {
        String delimiters = String(1, delimiter);
        return Split(str, delimiters);
    }

    Vector<String> Split(const StringView str, const StringView delimiters)
    {
        size_t start = 0;
        size_t end   = str.find_first_of(delimiters);

        Vector<String> result;
        result.reserve(10);

        while (end <= String::npos)
        {
            StringView token = StringView(str.data() + start, end - start);

            if (!token.empty())
                result.emplace_back(token.data());

            if (end == String::npos)
                break;

            start = end + 1;
            end   = str.find_first_of(delimiters, start);
        }

        return result;
    }

    Vector<String> Split(const StringView str, char delimiter)
    {
        return Split(str, StringView(&delimiter));
    }

    Vector<String> SplitInLines(const String& str)
    {
        return Split(str, "\n");
    }

    uint32 FindPosition(const String& str, const StringView search, uint32 offset)
    {
        const char* s     = str.c_str() + offset;
        const char* found = strstr(s, search.data());

        if (found == nullptr) return -1;

        return (uint32)(found - s) + offset;
    }

    uint32 FindPosition(const StringView str, const StringView search, uint32 offset)
    {
        const char* s     = str.data() + offset;
        const char* found = strstr(s, search.data());

        if (found == nullptr) return -1;

        return (uint32)(found - s) + offset;
    }

    StringView Range(const String& str, uint32 start, uint32 length)
    {
        return StringView(str.c_str() + start, length);
    }

    StringView Range(const StringView str, uint32 start, uint32 length)
    {
        return StringView(str.data() + start, length);
    }

    String RemoveRange(const String& str, uint32 start, uint32 length)
    {
        String result = str;
        return result.erase(start, length);
    }

    bool Contains(const String& str, const StringView chars)
    {
        return str.find(chars) != String::npos;
    }

    bool Contains(const StringView str, const StringView chars)
    {
        return str.find(chars) != StringView::npos;
    }

    bool StartsWith(const String& str, const StringView start)
    {
        return str.find(start) == 0;
    }

    bool StartsWith(const StringView str, const StringView start)
    {
        return str.find(start) == 0;
    }

    String Replace(String str, char ch1, char ch2)
    {
        for (int32 i = 0; i < str.size(); i++)
        {
            if (str[i] == ch1)
                str[i]  = ch2;
        }

        return str;
    }

}