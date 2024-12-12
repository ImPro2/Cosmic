#pragma once
#include "Memory/StackAllocator.hpp"

namespace Cosmic
{

	class PersistentStackAllocator
	{
	public:
		inline static constexpr size_t StackSize = 1024 * 1024;

	public:
		template<typename T, typename... Args>
		static T* Allocate(Args&&... args)
		{
			return sAllocator.Allocate<T>(std::forward<Args>(args)...);
		}

		static void Free();
		static void SetMarker(void* marker);

	private:
		static void Init();
		static void Shutdown();

	private:
		inline static StackAllocator<StackSize> sAllocator;

		friend class Application;
	};

}
