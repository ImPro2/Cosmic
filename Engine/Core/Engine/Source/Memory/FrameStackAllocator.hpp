#pragma once
#include "Memory/StackAllocator.hpp"

namespace Cosmic
{

	class Application;

	class FrameStackAllocator
	{
	public:
		inline static constexpr size_t StackSize = 1024 * 1024;

	public:
		template<typename T, typename... Args>
		static T* Allocate(Args&&... args)
		{
			return sAllocator.Allocate<T>(std::forward<Args>(args)...);
		}

	private:
		static void Init();
		static void Shutdown();

		static void Free();

	private:
		inline static StackAllocator<StackSize> sAllocator;

		friend class Application;
	};

}
