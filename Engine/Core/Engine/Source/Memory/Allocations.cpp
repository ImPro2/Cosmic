#include "cspch.hpp"
#include "Allocations.hpp"

CS_MODULE_LOG_INFO(Cosmic, Memory.Allocations);

namespace Cosmic
{

	void Allocations::LogAllocationStatistics()
	{
		size_t inUse = sStatistics.TotalAllocatedMemory - sStatistics.TotalFreedMemory;
		CS_LOG_INFO("Allocation Statistics:");
		CS_LOG_INFO("    Allocated Memory: {}", inUse);
		CS_LOG_INFO("    Total Allocated Memory Size: {}", sStatistics.TotalAllocatedMemory);
		CS_LOG_INFO("    Total Freed Memory Size: {}", sStatistics.TotalFreedMemory);
		CS_LOG_INFO("    Allocators ({}):", sAllocators.size());

		for (Ref<IAllocator> allocator : sAllocators)
		{
			const char* name = allocator->GetName().c_str();
			CS_LOG_INFO("        - {}", name);
		}
	}

	void Allocations::Init()
	{
		sStatistics.TotalAllocatedMemory = 0;
		sStatistics.TotalFreedMemory     = 0;

		DefaultAllocator::Init();
		FrameStackAllocator::Init();
		PersistentStackAllocator::Init();
	}

	void Allocations::Shutdown()
	{
		DefaultAllocator::Shutdown();
		FrameStackAllocator::Shutdown();
		PersistentStackAllocator::Free();
		PersistentStackAllocator::Shutdown();

		// All other allocators
		for (Ref<IAllocator> allocator : sAllocators)
			allocator->Shutdown();
	}

	void Allocations::EndFrame()
	{
		FrameStackAllocator::Free();
	}

	void Allocations::OnAllocation(size_t size)
	{
		sStatistics.TotalAllocatedMemory += size;
	}

	void Allocations::OnFree(size_t size)
	{
		sStatistics.TotalFreedMemory += size;
	}

}
