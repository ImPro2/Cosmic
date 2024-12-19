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

    template<class T, class Allocator>
    class StrongRef
    {
    public:
        // Default constructor & destructor

        StrongRef(T* ptr = nullptr)
            : mPtr(ptr)
        {
			IncRef();
        }

        ~StrongRef()
        {
            DecRef();
        }

        // Copy constructors & assignments

        StrongRef(const StrongRef& other)
            : mPtr(other.mPtr)
        {
            IncRef();
        }

        template<class T2>
        StrongRef(const StrongRef<T2, Allocator>& other)
            : mPtr((T*)other.mPtr)
        {
            IncRef();
        }

		StrongRef& operator=(std::nullptr_t)
        {
            DecRef();
            mPtr = nullptr;

            return *this;
        }

        StrongRef& operator=(const StrongRef& other)
        {
            other.IncRef();
            DecRef();

            mPtr = other.mPtr;
            return *this;
        }

        template<class T2>
        StrongRef& operator=(const StrongRef<T2, Allocator>& other)
        {
            other.IncRef();
            DecRef();

            mPtr = (T*)other.mPtr;
            return *this;
        }

        // Move constructors & assignments

        StrongRef(StrongRef&& other)
            : mPtr(other.mPtr)
        {
            other.mPtr = nullptr;
        }

        template<class T2>
        StrongRef(StrongRef<T2, Allocator>&& other)
            : mPtr(other.mPtr)
        {
            other.mPtr = nullptr;
        }

        StrongRef& operator=(StrongRef&& other)
        {
            mPtr       = other.mPtr;
            other.mPtr = nullptr;

            return *this;
        }

        template<class T2>
        StrongRef& operator=(StrongRef<T2, Allocator>&& other)
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
        StrongRef<T2, Allocator> As() const
        {
            return StrongRef<T2, Allocator>(*this);
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
        friend class StrongRef;

        T* mPtr;
    };

    template<class T, class Allocator>
    class PersistentStrongRef
    {
    public:
        // Default constructor & destructor

        PersistentStrongRef(T* ptr = nullptr)
            : mPtr(ptr)
        {
			IncRef();
        }

        ~PersistentStrongRef()
        {
            // Do nothing
        }

        // Copy constructors & assignments

        PersistentStrongRef(const PersistentStrongRef& other)
            : mPtr(other.mPtr)
        {
            IncRef();
        }

        template<class T2>
        PersistentStrongRef(const PersistentStrongRef<T2, Allocator>& other)
            : mPtr((T*)other.mPtr)
        {
            IncRef();
        }

		PersistentStrongRef& operator=(std::nullptr_t)
        {
            DecRef();
            mPtr = nullptr;

            return *this;
        }

        PersistentStrongRef& operator=(const PersistentStrongRef& other)
        {
            other.IncRef();
            DecRef();

            mPtr = other.mPtr;
            return *this;
        }

        template<class T2>
        PersistentStrongRef& operator=(const PersistentStrongRef<T2, Allocator>& other)
        {
            other.IncRef();
            DecRef();

            mPtr = (T*)other.mPtr;
            return *this;
        }

        // Move constructors & assignments

        PersistentStrongRef(PersistentStrongRef&& other)
            : mPtr(other.mPtr)
        {
            other.mPtr = nullptr;
        }

        template<class T2>
        PersistentStrongRef(PersistentStrongRef<T2, Allocator>&& other)
            : mPtr(other.mPtr)
        {
            other.mPtr = nullptr;
        }

        PersistentStrongRef& operator=(PersistentStrongRef&& other)
        {
            mPtr       = other.mPtr;
            other.mPtr = nullptr;

            return *this;
        }

        template<class T2>
        PersistentStrongRef& operator=(PersistentStrongRef<T2, Allocator>&& other)
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
        PersistentStrongRef<T2, Allocator> As() const
        {
            return PersistentStrongRef<T2, Allocator>(*this);
        }

        bool IsValid()
        {
            return (mPtr != nullptr) ? mPtr->GetRefCount() > 0 : false;
        }

    public:
        T* operator->()       { return mPtr; }
        T* operator->() const { return mPtr; }

        T&       operator*()       { return *mPtr; }
        const T& operator*() const { return *mPtr; }

        operator bool()       { return IsValid(); }
        operator bool() const { return IsValid(); }

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
        friend class PersistentStrongRef;

        T* mPtr;
    };

    template<class T, class Allocator>
    inline bool operator==(const StrongRef<T, Allocator>& left, const StrongRef<T, Allocator>& right)
    {
        return left.Ptr() == right.Ptr();
    }

    template<class T, class Allocator>
    inline bool operator==(const PersistentStrongRef<T, Allocator>& left, const PersistentStrongRef<T, Allocator>& right)
    {
        return left.Ptr() == right.Ptr();
    }

}
