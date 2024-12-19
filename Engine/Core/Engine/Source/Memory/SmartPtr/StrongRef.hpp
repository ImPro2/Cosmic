#pragma once
#include "Base/Types.hpp"
#include "Memory/SmartPtr/ReferenceCounter.hpp"
#include "Memory/Allocations.hpp"

namespace Cosmic
{

    class IRefCounted
    {
    };

    template<class T, class Allocator>
    class StrongRef
    {
    public:
        static_assert(std::is_base_of_v<IRefCounted, T>, "T must be derived from IRefCounted.");

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
            ReferenceCounter::DecRefCount<T, Allocator>(mPtr);
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

        bool IsValid() const
        {
            if (ReferenceCounter::GetRefCount(mPtr) == 0)
                mPtr = nullptr;

            return mPtr != nullptr;
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
                ReferenceCounter::IncRefCount(mPtr);
        }

        void DecRef() const
        {
            if (mPtr)
            {
                uint32 refCount = ReferenceCounter::DecRefCount<T, Allocator>(mPtr);

                if (refCount == 0)
                    mPtr = nullptr;
            }
        }

    private:
        template<class, class>
        friend class StrongRef;

        mutable T* mPtr;
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
            ReferenceCounter::ZeroRefCount<T, Allocator>(mPtr);
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

        bool IsValid() const
        {
            if (ReferenceCounter::GetRefCount(mPtr) == 0)
				mPtr = nullptr;

            return mPtr != nullptr;
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
            {
                ReferenceCounter::IncRefCount(mPtr);
            }
        }

        void DecRef() const
        {
            if (mPtr)
            {
                uint32 refCount = ReferenceCounter::DecRefCount<T, Allocator>(mPtr);

                if (refCount == 0)
                    mPtr = nullptr;
            }
        }

    private:
        template<class, class>
        friend class PersistentStrongRef;

        mutable T* mPtr;
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
