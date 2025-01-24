#pragma once
#include "Base/Base.hpp"

namespace Cosmic
{

	using RefCountMap = UnorderedMap<void*, uint32>;

	class Allocations;

	class ReferenceCounter
	{
	private:
		static ReferenceCounter* Init();
		static void              Shutdown();

	public:
		template<class T>
		static uint32 GetRefCount(T* ptr)
		{
			if (sInstance->mRefCountMap.find(ptr) == sInstance->mRefCountMap.end())
			{
				return 0;
			}

			return sInstance->mRefCountMap.at(ptr);
		}

		template<class T>
		static void IncRefCount(T* ptr)
		{
			sInstance->mRefCountMap[ptr]++;
		}

		template<class T, class Allocator, bool Array = false>
		static uint32 DecRefCount(T* ptr, uint32 count = 0)
		{
			if (sInstance->mRefCountMap.find(ptr) == sInstance->mRefCountMap.end())
			{
				return 0;
			}

			sInstance->mRefCountMap[ptr]--;

			if (sInstance->mRefCountMap[ptr] == 0)
			{
				sInstance->mRefCountMap.erase(ptr);

				if constexpr (Array)
					Allocator::FreeArray(ptr, count);
				else
					Allocator::Free(ptr);

				ptr = nullptr;
				return 0;
			}

			return sInstance->mRefCountMap.at(ptr);
		}

		template<class T, typename Allocator>
		static void ZeroRefCount(T* ptr)
		{
			if (sInstance->mRefCountMap.find(ptr) == sInstance->mRefCountMap.end())
			{
				return;
			}

			sInstance->mRefCountMap.erase(ptr);
			Allocator::Free(ptr);
			ptr = nullptr;
		}

	private:
		RefCountMap mRefCountMap;
		inline static ReferenceCounter* sInstance;

		friend class Allocations;
	};

}
