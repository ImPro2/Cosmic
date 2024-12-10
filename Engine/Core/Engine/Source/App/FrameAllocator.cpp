#include "cspch.hpp"
#include "FrameAllocator.hpp"

namespace Cosmic
{

	void FrameAllocator::Init()
	{
		sAllocator.Init();
	}

	void FrameAllocator::Shutdown()
	{
		sAllocator.Shutdown();
	}

	void FrameAllocator::Free()
	{
		sAllocator.Free();
	}

}
