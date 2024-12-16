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

    template<class T>
    using PersistentRef = PersistentStrongRef<T, DefaultAllocator>;

    // Create Ref with DefaultAllocator

    template<class T, typename... Args>
    constexpr Ref<T> CreateRef(Args&&... args)
    {
        return Ref<T>(DefaultAllocator::Allocate<T>(std::forward<Args>(args)...));
    }

    template<class T, typename... Args>
    constexpr PersistentRef<T> CreatePersistentRef(Args&&... args)
    {
        return PersistentRef<T>(DefaultAllocator::Allocate<T>(std::forward<Args>(args)...));
    }


}
