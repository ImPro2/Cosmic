#pragma once
#include "Memory/Allocator/Impl/HeapAllocator.hpp"

namespace Cosmic
{

	class Allocations;

	class DefaultAllocator
	{
	public:
		template<typename T, typename... Args>
		static T* Allocate(Args&&... args)
		{
			return sAllocator->Allocate<T>(std::forward<Args>(args)...);
		}

		template<typename T>
		static void Free(T* ptr)
		{
			sAllocator->Free(ptr);
		}

	private:
		static void Init();
		static void Shutdown();

	private:
		inline static StrongRef<HeapAllocator, DefaultAllocator> sAllocator;

		friend class Allocations;
	};

}
