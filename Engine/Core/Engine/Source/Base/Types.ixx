module;
#include "cspch.hpp"
export module Cosmic.Base.Types;

namespace Cosmic
{

    export using int8       =          __int8;
    export using int16      =          __int16;
    export using int32      =          __int32;
    export using int64      =          __int64;

    export using uint8      = unsigned __int8;
    export using uint16     = unsigned __int16;
    export using uint32     = unsigned __int32;
    export using uint64     = unsigned __int64;

    export using float32    = float;
    export using float64    = float;
    
    export using byte       = uint8;

    export using String     = std::string;
    export using StringView = std::string_view;

    export template<typename T>
    using Vector            = std::vector<T>;

    export template<typename T, size_t S>
    using Array             = std::array<T, S>;

    export template<typename K, typename V>
    using UnorderedMap      = std::unordered_map<K, V>;

    export template<typename F, typename S>
    struct Pair
    {
        F first;
        S second;
    };

    export template<typename T>
    using Ref = std::shared_ptr<T>;

    export template<typename T, typename ... Args>
    constexpr Ref<T> CreateRef(Args&& ... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    export template<typename T>
    using Scope = std::unique_ptr<T>;

    export template<typename T, typename ... Args>
    constexpr Scope<T> CreateScope(Args&& ... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }
}