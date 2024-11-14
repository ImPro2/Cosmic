#pragma once
#include <array>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include <stdint.h>

namespace Cosmic {

    using int8  = int8_t;
    using int16 = int16_t;
    using int32 = int32_t;
    using int64 = int64_t;
    
    using uint8  = uint8_t;
    using uint16 = uint16_t;
    using uint32 = uint32_t;
    using uint64 = uint64_t;

    using float32 = float;
    using float64 = double;
    
    // using byte = uint8;
    
    using String = std::string;
    using StringView = std::string_view;
    
    template <typename T>
    using Vector = std::vector<T>;
    
    template <typename T, size_t S>
    using Array = std::array<T, S>;
    
    template <typename K, typename V>
    using UnorderedMap = std::unordered_map<K, V>;
    
    template <typename F, typename S>
    struct Pair
    {
        F first;
        S second;
    };
    
    template <typename T>
    using Ref = std::shared_ptr<T>;
    
    template <typename T, typename... Args>
    constexpr Ref<T> CreateRef(Args &&...args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
    
    template <typename T>
    using Scope = std::unique_ptr<T>;
    
    template <typename T, typename... Args>
    constexpr Scope<T> CreateScope(Args &&...args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

}
