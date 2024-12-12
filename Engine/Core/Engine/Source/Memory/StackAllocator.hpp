#pragma once
#include "Base/Types.hpp"
#include "Base/StaticBuffer.hpp"

namespace Cosmic
{

	template<size_t N>
	class StackAllocator
	{
	public:
		inline static constexpr size_t MaxSize = N;

	public:
		StackAllocator()  = default;
		~StackAllocator() = default;

	public:
		void Init(void* marker = nullptr)
		{
			mTop = mStack.GetData();
			SetMarker(marker);
		}

		void Shutdown()
		{
			mStack.~StaticBuffer();
		}

	public:
		template<typename T, typename... Args>
		T* Allocate(Args&&... args)
		{
			size_t offset = sizeof(T);
			CS_ASSERT_NOMSG(GetSize() + offset < MaxSize);

			T* ptr = new(mTop) T(std::forward<Args>(args)...);
			mTop   = static_cast<byte*>(mTop) + offset;

			return ptr;
		}

		void Free()
		{
			memset(mMarker, 0, (size_t)mTop - (size_t)mMarker);
			mTop = mMarker;
		}

		void SetMarker(void* marker)
		{
			// Marker is out of bounds
			CS_ASSERT_NOMSG(marker == nullptr || (marker > mTop && (size_t)marker - (size_t)mTop < MaxSize));

			mMarker = (marker == nullptr) ? mTop : marker;
		}
		
	public:
		void*  GetTop()        { return mTop;                                    }
		void*  GetMarker()     { return mMarker;                                 }
		size_t GetSize() const { return (size_t)mTop - (size_t)mStack.GetData(); }

	private:
		StaticBuffer<N> mStack;
		void*           mTop;
		void*           mMarker;
	};
	
}
