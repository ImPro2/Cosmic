#pragma once
#include "Memory/StrongRef.hpp"

namespace Cosmic
{

    template<class T, class Allocator = DefaultAllocator>
    class WeakRef
    {
    public:
        WeakRef()  = default;
        ~WeakRef() = default;

        WeakRef(std::nullptr_t)
            : mPtr(nullptr)
        {
        }

        WeakRef(const Ref<T, Allocator>& ref)
            : mPtr(ref.Ptr())
        {
        }

        WeakRef(const WeakRef& other)
            : mPtr(other.mPtr)
        {
        }

        WeakRef& operator=(const Ref<T, Allocator>& ref)
        {
            mPtr = ref.Ptr();
            return *this;
        }

        WeakRef& operator=(const WeakRef& other)
        {
            mPtr = other.mPtr;
            return *this;
        }

        WeakRef(Ref<T, Allocator>&&)            = delete;
        WeakRef& operator=(Ref<T, Allocator>&&) = delete;

        WeakRef(WeakRef&& other)
            : mPtr(other.mPtr)
        {
            other.mPtr = nullptr;
        }

        WeakRef& operator=(WeakRef&& other)
        {
            mPtr       = other.mPtr;
            other.mPtr = nullptr;

            return *this;
        }

    public:
        Ref<T, Allocator> Own()
        {
			return Ref<T, Allocator>(mPtr);
        }

    public:
        bool IsValid() const
        {
            return (mPtr != nullptr) ? mPtr->GetRefCount() > 0 : false;
        }

    public:
        operator bool()       { return IsValid(); }
        operator bool() const { return IsValid(); }

    private:
        template<class, class>
        friend class WeakRef;

        T* mPtr;
    };

}
