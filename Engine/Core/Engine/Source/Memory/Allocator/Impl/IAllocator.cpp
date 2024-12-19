#include "cspch.hpp"
#include "IAllocator.hpp"

#include "Memory/Allocations.hpp"

namespace Cosmic
{

	void IAllocator::OnAllocation(size_t size)
	{
		Allocations::OnAllocation(size);
	}

	void IAllocator::OnFree(size_t size)
	{
		Allocations::OnFree(size);
	}

}
