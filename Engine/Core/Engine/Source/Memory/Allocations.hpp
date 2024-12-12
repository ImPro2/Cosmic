#pragma once
#include "Base/Types.hpp"

#include "Memory/IAllocator.hpp"
#include "Memory/SmartPtrs.hpp"

#include "Memory/DefaultAllocator.hpp"
#include "Memory/FrameStackAllocator.hpp"
#include "Memory/PersistentStackAllocator.hpp"

namespace Cosmic
{

	class Application;

	struct AllocationStatistics
	{
		size_t TotalAllocatedMemory;
		size_t TotalFreedMemory;
	};

	class Allocations
	{
	public:
		static void LogAllocationStatistics();

		// Up to user to call Init() if it exists
		template<class Allocator>
		static Ref<Allocator> AddAllocator(const String& name)
		{
			Ref<Allocator> allocator = CreateRef<Allocator>(name);
			sAllocators.push_back(allocator.As<IAllocator>());

			return allocator;
		}

		template<class Allocator>
		static void RemoveAllocator(Ref<Allocator>& allocator)
		{
			auto it = std::find(sAllocators.begin(), sAllocators.end(), allocator);
			sAllocators.erase(it);

			allocator->Shutdown();
			allocator.Release();
		}

	private:
		static void Init();
		static void Shutdown();

		static void EndFrame();

	public:
		static const AllocationStatistics& GetStatistics() { return sStatistics;  }

	private:
		static void OnAllocation(size_t size);
		static void OnFree(size_t size);

	private:
		friend class IAllocator;
		friend class Application;

		inline static AllocationStatistics    sStatistics;
		inline static Vector<Ref<IAllocator>> sAllocators;
	};

}
