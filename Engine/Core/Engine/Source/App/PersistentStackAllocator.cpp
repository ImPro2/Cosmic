#include "cspch.hpp"
#include "PersistentStackAllocator.hpp"

namespace Cosmic
{

	void PersistentStackAllocator::Init()
	{
		sAllocator.Init();
	}

	void PersistentStackAllocator::Shutdown()
	{
		sAllocator.Shutdown();
	}

	void PersistentStackAllocator::Free()
	{
		sAllocator.Free();
	}

	void PersistentStackAllocator::SetMarker(void* marker)
	{
		sAllocator.SetMarker(marker);
	}

}
