#pragma once
#include "Base/Types.hpp"
#include "Base/StaticBuffer.hpp"

namespace Cosmic
{

	template<size_t N>
	class StackAllocator
	{
	public:
		static const constexpr MaxSize = N;

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
			CS_ASSERT_NOMSG(GetSize() + offset >= MaxSize);

			T* ptr = new(mTop) T(std::forward<Args>(args)...);
			mTop += offset;
			return ptr;
		}

		void Free()
		{
			memcpy(mMarker, nullptr, mTop - mMarker);
			mTop = mMarker;
		}

		void SetMarker(void* marker)
		{ 
			CS_ASSERT_NOMSG(marker > mTop && (marker - mTop) < MaxSize); // Marker is out of bounds
			mMarker = (marker == nullptr) ? mTop : marker;
		}
		
	public:
		void*  GetTop()        { return mTop;                              }
		void*  GetMarker()     { return mMarker;                           }
		size_t GetSize() const { return (size_t)(mTop - mStack.GetData()); }

	private:
		StaticBuffer<N> mStack;
		void*           mTop;
		void*           mMarker;
	};
	
}
