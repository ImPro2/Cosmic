#pragma once
#include "Base/Types.hpp"
#include "Memory/DefaultAllocator.hpp"

namespace Cosmic
{

    class IRefCounted
    {
    public:
        void IncRefCount()  const { mRefCount++;   }
        void DecRefCount()  const { mRefCount--;   }
        void ZeroRefCount() const { mRefCount = 0; }

    public:
        uint32 GetRefCount() const { return mRefCount; }

    private:
        mutable uint32 mRefCount = 0;
    };

    template<class T, class Allocator = DefaultAllocator>
    class Ref
    {
    public:
        // Default constructor & destructor

        Ref(T* ptr = nullptr)
            : mPtr(ptr)
        {
			IncRef();
        }

        ~Ref()
        {
            DecRef();
        }

        // Copy constructors & assignments

        Ref(const Ref& other)
            : mPtr(other.mPtr)
        {
            IncRef();
        }

        template<class T2>
        Ref(const Ref<T2>& other)
            : mPtr((T*)other.mPtr)
        {
            IncRef();
        }

		Ref& operator=(std::nullptr_t)
        {
            DecRef();
            mPtr = nullptr;

            return *this;
        }

        Ref& operator=(const Ref& other)
        {
            other.IncRef();
            DecRef();

            mPtr = other.mPtr;
            return *this;
        }

        template<class T2>
        Ref& operator=(const Ref<T2, Allocator>& other)
        {
            other.IncRef();
            DecRef();

            mPtr = (T*)other.mPtr;
            return *this;
        }

        // Move constructors & assignments

        Ref(Ref&& other)
            : mPtr(other.mPtr)
        {
            other.mPtr = nullptr;
        }

        template<class T2>
        Ref(Ref<T2, Allocator>&& other)
            : mPtr(other.mPtr)
        {
            other.mPtr = nullptr;
        }

        Ref& operator=(Ref&& other)
        {
            mPtr       = other.mPtr;
            other.mPtr = nullptr;

            return *this;
        }

        template<class T2>
        Ref& operator=(Ref<T2, Allocator>&& other)
        {
            mPtr       = (T*)other.mPtr;
            other.mPtr = nullptr;

            return *this;
        }

   public:
        T*       Ptr()       { return mPtr; }
        const T* Ptr() const { return mPtr; }

    public:
        void Release()
        {
            mPtr->ZeroRefCount();
            Allocator::Free(mPtr);
            mPtr = nullptr;
        }

        void Reset(T* ptr = nullptr)
        {
            DecRef();
            mPtr = ptr;
        }

        template<class T2>
        Ref<T2> As() const
        {
            return Ref<T2>(*this);
        }

    public:
        T* operator->()       { return mPtr; }
        T* operator->() const { return mPtr; }

        T&       operator*()       { return *mPtr; }
        const T& operator*() const { return *mPtr; }

        operator bool()       { return mPtr != nullptr; }
        operator bool() const { return mPtr != nullptr; }

    private:
        void IncRef() const
        {
            if (mPtr)
                mPtr->IncRefCount();
        }

        void DecRef() const
        {
            if (mPtr)
            {
                mPtr->DecRefCount();

                if (mPtr->GetRefCount() == 0)
                    Allocator::Free(mPtr);
            }
        }

    private:
        template<class, class>
        friend class Ref;

        T* mPtr;
    };

    // Create Ref with DefaultAllocator

    template<class T, typename... Args>
    constexpr Ref<T, DefaultAllocator> CreateRef(Args&&... args)
    {
        return Ref<T, DefaultAllocator>(DefaultAllocator::Allocate<T>(std::forward<Args>(args)...));
    }

}
