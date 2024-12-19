#pragma once
#include "Memory/Allocator/FrameStackAllocator.hpp"

namespace Cosmic
{

	// Pointer to an object allocated in FrameStackAllocator

	template<class T>
	class FramePtr
	{
	public:
		FramePtr()               = delete;
		FramePtr(std::nullptr_t) = delete;
		
		FramePtr(T* ptr)
			: mPtr(ptr)
		{
		}

		FramePtr(const FramePtr<T>& other)
			: mPtr(other.mPtr)
		{
		}

		FramePtr(FramePtr<T>&& other)
			: mPtr(other.mPtr)
		{
		}

		template<class T2>
		FramePtr(const FramePtr<T2>& other)
			: mPtr((T*)other.Ptr())
		{
		}

		template<class T2>
		FramePtr(FramePtr<T2>&& other)
			: mPtr((T*)other.Ptr())
		{
		}

		~FramePtr() = default;
		
	public:
		FramePtr<T>& operator=(FramePtr<T>&& other)
		{
			mPtr = other.mPtr;
			return *this;
		}

	public:
		template<class T2>
		FramePtr<T2> As() const
		{
			return FramePtr<T2>(*this);
		}

	public:
		T*       Ptr()       { return mPtr; }
		const T* Ptr() const { return mPtr; }

    public:
        T*       operator->()       { return mPtr; }
        const T* operator->() const { return mPtr; }

        T&       operator*()       { return *mPtr; }
        const T& operator*() const { return *mPtr; }

        operator bool()       { return mPtr != nullptr; }
        operator bool() const { return mPtr != nullptr; }

	private:
		T* mPtr;
	};

	template<class T, typename... Args>
	constexpr FramePtr<T> CreateFramePtr(Args&&... args)
	{
		return FramePtr<T>(FrameStackAllocator::Allocate<T>(std::forward<Args>(args)...));
	}

}
