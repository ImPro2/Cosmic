#pragma once
#include "Memory/Allocator/DefaultAllocator.hpp"

namespace Cosmic
{

    template<class T, class Allocator = DefaultAllocator>
    class Scope
    {
    public:
        Scope(T* ptr = nullptr)
            : mPtr(ptr)
        {
        }

        Scope(Scope&& other)
            : mPtr(other.mPtr)
        {
            other.mPtr = nullptr;
        }

        Scope& operator=(Scope&& other)
        {
            if (mPtr != other.mPtr)
            {
                delete mPtr;
                mPtr = other.mPtr;
                other.mPtr = nullptr;
            }

            return *this;
        }

        template<class T2>
        Scope(Scope<T2>&& other)
            : mPtr((T*)other.mPtr)
        {
            other.mPtr = nullptr;
        }

        template<class T2>
        Scope& operator=(Scope<T2, Allocator>&& other)
        {
            if (mPtr != (T*)other.mPtr)
            {
                delete mPtr;
                mPtr = (T*)other.mPtr;
                other.mPtr = nullptr;
            }

            return *this;
        }

        Scope(const Scope&)                              = delete;
        Scope& operator=(const Scope&)                   = delete;

        template<class T2>
        Scope(const Scope<T2, Allocator>&)               = delete;

        template<class T2>
        Scope<T>& operator=(const Scope<T2, Allocator>&) = delete;

        ~Scope()
        {
            if (mPtr)
            {
                Allocator::Free(mPtr);
                mPtr = nullptr;
            }
        }

    public:
        T*       Ptr()       { return mPtr; }
        const T* Ptr() const { return mPtr; }

    public:
        T* Release()
        {
            T* tmp = mPtr;
            mPtr   = nullptr;

            return tmp;
        }

        void Reset(T* ptr = nullptr)
        {
            if (!ptr)
                Allocator::Free(mPtr);

            mPtr = ptr;
        }

        template<class T2>
        T2* As() const
        {
            return (T2*)mPtr;
        }

#if 0
        template<class T2>
        Scope<T2> As() const
        {
            //return Scope<T2>(*this);
            //return Scope<T2>((T2*)mPtr);
            return (T2*)mPtr;
        }
#endif

    public:
        T*       operator->()       { return mPtr; }
        const T* operator->() const { return mPtr; }

        T&       operator*()       { return *mPtr; }
        const T& operator*() const { return *mPtr; }

        operator bool()       { return mPtr != nullptr; }
        operator bool() const { return mPtr != nullptr; }

    private:
        template<class, class>
        friend class Scope;

        T* mPtr;
    };

    // Create Scope with DefaultAllocator

    template<class T, typename... Args>
    constexpr Scope<T, DefaultAllocator> CreateScope(Args&&... args)
    {
        return Scope<T, DefaultAllocator>(DefaultAllocator::Allocate<T>(std::forward<Args>(args)...));
    }

}
