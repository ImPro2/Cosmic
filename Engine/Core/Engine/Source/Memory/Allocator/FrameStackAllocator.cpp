#include "cspch.hpp"
#include "FrameStackAllocator.hpp"
#include "Memory/Allocations.hpp"

namespace Cosmic
{

	void FrameStackAllocator::Init()
	{
		sAllocator = Allocations::AddAllocator<StackAllocator<StackSize>>("Frame Stack Allocator");
		sAllocator->Init();
	}

	void FrameStackAllocator::Shutdown()
	{
		Allocations::RemoveAllocator(sAllocator);
	}

	void FrameStackAllocator::Free()
	{
		sAllocator->Free();
	}

}
