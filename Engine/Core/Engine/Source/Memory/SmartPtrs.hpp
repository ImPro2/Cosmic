#pragma once
#include "Memory/ScopedPtr.hpp"
#include "Memory/StrongRef.hpp"
#include "Memory/WeakRef.hpp"
#include "Memory/FramePtr.hpp"
#include "Memory/DefaultAllocator.hpp"

namespace Cosmic
{

    template<class T>
    using Ref = StrongRef<T, DefaultAllocator>;

    // Create Ref with DefaultAllocator

    template<class T, typename... Args>
    constexpr Ref<T> CreateRef(Args&&... args)
    {
        return Ref<T>(DefaultAllocator::Allocate<T>(std::forward<Args>(args)...));
    }

}
