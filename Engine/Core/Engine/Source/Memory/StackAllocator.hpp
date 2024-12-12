#pragma once
#include "Base/Types.hpp"
#include "Base/StaticBuffer.hpp"
#include "Memory/IAllocator.hpp"

namespace Cosmic
{

	template<size_t N>
	class StackAllocator : public IAllocator
	{
	public:
		inline static constexpr size_t MaxSize = N;

	public:
		StackAllocator(const String& name)
			: IAllocator(name)
		{
		}

		~StackAllocator() = default;

	public:
		void Init(void* marker = nullptr)
		{
			mTop = mStack.GetData();
			SetMarker(marker);
		}

		void Shutdown() override
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

			OnAllocation(sizeof(T));

			return ptr;
		}

		void Free()
		{
			size_t size = (size_t)mTop - (size_t)mMarker;
			memset(mMarker, 0, size);
			mTop = mMarker;

			OnFree(size);
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
