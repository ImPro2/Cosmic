#pragma once

namespace Cosmic
{

	class DefaultAllocator
	{
	public:
		template<class T, typename... Args>
		static T* Allocate(Args&&... args)
		{
			return new T(std::forward<Args>(args)...);
		}

		static void Free(void* ptr)
		{
			delete ptr;
		}
	};

}
