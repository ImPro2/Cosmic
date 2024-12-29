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
		static T* AllocateArray(size_t count)
		{
			return sAllocator->AllocateArray<T>(count);
		}

		template<typename T>
		static void Free(T* ptr)
		{
			sAllocator->Free(ptr);
		}

		template<typename T>
		static void FreeArray(T* ptr, size_t count)
		{
			sAllocator->FreeArray(ptr, count);
		}

	private:
		static void Init();
		static void Shutdown();

	private:
		inline static StrongRef<HeapAllocator, DefaultAllocator> sAllocator;

		friend class Allocations;
	};

}
