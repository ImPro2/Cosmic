#include "cspch.hpp"
#include "FrameStackAllocator.hpp"

namespace Cosmic
{

	void FrameStackAllocator::Init()
	{
		sAllocator.Init();
	}

	void FrameStackAllocator::Shutdown()
	{
		sAllocator.Shutdown();
	}

	void FrameStackAllocator::Free()
	{
		sAllocator.Free();
	}

}
