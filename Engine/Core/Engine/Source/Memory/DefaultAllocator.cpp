#include "cspch.hpp"
#include "DefaultAllocator.hpp"
#include "Memory/Allocations.hpp"

namespace Cosmic
{

	void DefaultAllocator::Init()
	{
		sAllocator = Allocations::AddAllocator<HeapAllocator>("Default Allocator");
	}

	void DefaultAllocator::Shutdown()
	{
		Allocations::RemoveAllocator(sAllocator);
	}

}
