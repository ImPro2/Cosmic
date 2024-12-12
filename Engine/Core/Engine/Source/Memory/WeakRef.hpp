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

        WeakRef(const StrongRef<T, Allocator>& ref)
            : mPtr((T*)ref.Ptr())
        {
        }

        WeakRef(const WeakRef& other)
            : mPtr(other.mPtr)
        {
        }

        WeakRef& operator=(const StrongRef<T, Allocator>& ref)
        {
            mPtr = (T*)ref.Ptr();
            return *this;
        }

        WeakRef& operator=(const WeakRef& other)
        {
            mPtr = other.mPtr;
            return *this;
        }

        WeakRef(StrongRef<T, Allocator>&&)            = delete;
        WeakRef& operator=(StrongRef<T, Allocator>&&) = delete;

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
        StrongRef<T, Allocator> Own()
        {
			return StrongRef<T, Allocator>(mPtr);
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
