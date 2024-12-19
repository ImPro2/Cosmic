#pragma once
#include "Memory/Allocator/Impl/StackAllocator.hpp"
#include "Memory/Allocator/DefaultAllocator.hpp"
#include "Memory/SmartPtr/StrongRef.hpp"

namespace Cosmic
{

	class Allocations;

	class PersistentStackAllocator
	{
	public:
		inline static constexpr size_t StackSize = 1024 * 1024;

	public:
		template<typename T, typename... Args>
		static T* Allocate(Args&&... args)
		{
			return sAllocator->Allocate<T>(std::forward<Args>(args)...);
		}

		static void Free();
		static void SetMarker(void* marker);

	private:
		static void Init();
		static void Shutdown();

	private:
		inline static StrongRef<StackAllocator<StackSize>, DefaultAllocator> sAllocator;

		friend class Allocations;
	};

}
