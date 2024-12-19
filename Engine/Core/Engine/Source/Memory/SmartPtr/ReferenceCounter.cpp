#include "cspch.hpp"
#include "ReferenceCounter.hpp"

#include "Memory/Allocations.hpp"

namespace Cosmic
{

	ReferenceCounter* ReferenceCounter::Init()
	{
		sInstance = new ReferenceCounter();
		return sInstance;
	}

	void ReferenceCounter::Shutdown()
	{
		delete sInstance;
	}

}