#pragma once
#include "Memory/IAllocator.hpp"

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
		void Free(T* ptr)
		{
			OnFree(sizeof(T));
			delete ptr;
		}
	};

}
