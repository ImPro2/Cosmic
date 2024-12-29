#pragma once
#include "Memory/Allocator/Impl/IAllocator.hpp"

namespace Cosmic
{

	class HeapAllocator : public IAllocator
	{
	public:
		HeapAllocator(const String& name)
			: IAllocator(name)
		{
		}

	public:
		void Shutdown() override
		{
		}

	public:
		template<typename T, typename... Args>
		T* Allocate(Args&&... args)
		{
			OnAllocation(sizeof(T));
			return new T(std::forward<Args>(args)...);
		}

		template<typename T>
		T* AllocateArray(size_t count)
		{
			OnAllocation(sizeof(T) * count);
			return new T[count];
		}

		template<typename T>
		void Free(T* ptr)
		{
			OnFree(sizeof(T));
			delete ptr;
		}

		template<typename T>
		void FreeArray(T* ptr, size_t count)
		{
			OnFree(sizeof(T) * count);
			delete[] ptr;
		}
	};

}
