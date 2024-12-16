#include "cspch.hpp"
#include "Allocations.hpp"

CS_MODULE_LOG_INFO(Cosmic, Memory.Allocations);

namespace Cosmic
{

	void Allocations::LogAllocationStatistics()
	{
		size_t inUse = sInstance->mStatistics.TotalAllocatedMemory - sInstance->mStatistics.TotalFreedMemory;
		CS_LOG_INFO("Allocation Statistics:");
		CS_LOG_INFO("    Allocated Memory: {}", inUse);
		CS_LOG_INFO("    Total Allocated Memory Size: {}", sInstance->mStatistics.TotalAllocatedMemory);
		CS_LOG_INFO("    Total Freed Memory Size: {}", sInstance->mStatistics.TotalFreedMemory);
		CS_LOG_INFO("    Allocators ({}):", sInstance->mAllocators.size());

		for (Ref<IAllocator> allocator : sInstance->mAllocators)
		{
			const char* name = allocator->GetName().c_str();
			CS_LOG_INFO("        - {}", name);
		}
	}

	PersistentRef<Allocations> Allocations::Init()
	{
		sInstance = PersistentRef<Allocations>(new Allocations());

		sInstance->mStatistics.TotalAllocatedMemory = 0;
		sInstance->mStatistics.TotalFreedMemory     = 0;

		DefaultAllocator::Init();
		FrameStackAllocator::Init();
		PersistentStackAllocator::Init();

		return sInstance;
	}

	void Allocations::Shutdown()
	{
		DefaultAllocator::Shutdown();
		FrameStackAllocator::Shutdown();
		PersistentStackAllocator::Free();
		PersistentStackAllocator::Shutdown();

		// All other allocators
		for (Ref<IAllocator> allocator : sInstance->mAllocators)
			allocator->Shutdown();
	}

	void Allocations::EndFrame()
	{
		FrameStackAllocator::Free();
	}

	void Allocations::SetInstance(const PersistentRef<Allocations>& instance)
	{
		sInstance = instance;

		DefaultAllocator::sAllocator         = sInstance->mAllocators[0];
		FrameStackAllocator::sAllocator      = sInstance->mAllocators[1];
		PersistentStackAllocator::sAllocator = sInstance->mAllocators[2];
	}

	void Allocations::OnAllocation(size_t size)
	{
		sInstance->mStatistics.TotalAllocatedMemory += size;
	}

	void Allocations::OnFree(size_t size)
	{
		sInstance->mStatistics.TotalFreedMemory += size;
	}

}
