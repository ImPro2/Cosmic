#pragma once
#include "Base/Types.hpp"

#include "Memory/Allocator/Impl/IAllocator.hpp"
#include "Memory/SmartPtr/SmartPtrs.hpp"

#include "Memory/Allocator/DefaultAllocator.hpp"
#include "Memory/Allocator/FrameStackAllocator.hpp"
#include "Memory/Allocator/PersistentStackAllocator.hpp"

namespace Cosmic
{

	class Application;
	class VulkanApp; // temporary

	struct AllocationStatistics
	{
		size_t TotalAllocatedMemory;
		size_t TotalFreedMemory;
	};

	class Allocations : public IRefCounted
	{
	public:
		static void LogAllocationStatistics();

		// Up to user to call Init() if it exists
		template<class Allocator>
		static Ref<Allocator> AddAllocator(const String& name)
		{
			Ref<Allocator> allocator = CreateRef<Allocator>(name);
			sInstance->mAllocators.push_back(allocator.As<IAllocator>());

			return allocator;
		}

		template<class Allocator>
		static void RemoveAllocator(Ref<Allocator>& allocator)
		{
			auto it = std::find(sInstance->mAllocators.begin(), sInstance->mAllocators.end(), allocator);
			sInstance->mAllocators.erase(it);

			allocator->Shutdown();
			allocator.Release();
		}

	private:
		static PersistentRef<Allocations> Init();
		static void                       Shutdown();

		static void EndFrame();

		static void SetInstance(const PersistentRef<Allocations>& instance);

	public:
		static const AllocationStatistics& GetStatistics()       { return sInstance->mStatistics;       }

	private:
		static void OnAllocation(size_t size);
		static void OnFree(size_t size);

	private:
		AllocationStatistics    mStatistics;
		Vector<Ref<IAllocator>> mAllocators;
		ReferenceCounter*       mReferenceCounter;

		inline static PersistentRef<Allocations> sInstance;

		friend class IAllocator;
		friend class Application;
		friend class VulkanApp;
	};

}
