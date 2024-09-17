#pragma once
#include <string>
#include <array>
#include <vector>
#include <unordered_map>
#include <memory>

namespace Cosmic
{

    using int8       =          __int8;
    using int16      =          __int16;
    using int32      =          __int32;
    using int64      =          __int64;

    using uint8      = unsigned __int8;
    using uint16     = unsigned __int16;
    using uint32     = unsigned __int32;
    using uint64     = unsigned __int64;

    using float32    = float;
    using float64    = float;
    
    using byte       = uint8;

    using String     = std::string;
    using StringView = std::string_view;

    template<typename T>
    using Vector            = std::vector<T>;

    template<typename T, size_t S>
    using Array             = std::array<T, S>;

    template<typename K, typename V>
    using UnorderedMap      = std::unordered_map<K, V>;

    template<typename F, typename S>
    struct Pair
    {
        F first;
        S second;
    };

    template<typename T>
    using Ref = std::shared_ptr<T>;

    template<typename T, typename ... Args>
    constexpr Ref<T> CreateRef(Args&& ... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using Scope = std::unique_ptr<T>;

    template<typename T, typename ... Args>
    constexpr Scope<T> CreateScope(Args&& ... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }
}