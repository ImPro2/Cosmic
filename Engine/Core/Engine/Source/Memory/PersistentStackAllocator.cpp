#include "cspch.hpp"
#include "PersistentStackAllocator.hpp"
#include "Memory/Allocations.hpp"

namespace Cosmic
{

	void PersistentStackAllocator::Init()
	{
		sAllocator = Allocations::AddAllocator<StackAllocator<StackSize>>("Persistent Stack Allocator");
		sAllocator->Init();
	}

	void PersistentStackAllocator::Shutdown()
	{
		Allocations::RemoveAllocator(sAllocator);
	}

	void PersistentStackAllocator::Free()
	{
		sAllocator->Free();
	}

	void PersistentStackAllocator::SetMarker(void* marker)
	{
		sAllocator->SetMarker(marker);
	}

}
