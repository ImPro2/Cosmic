#pragma once
#include "Base/Types.hpp"
#include "Memory/StrongRef.hpp"

namespace Cosmic
{

	class Allocations;

	class IAllocator : public IRefCounted
	{
	public:
		IAllocator(const String& name)
			: mName(name)
		{
		}

	protected:
		virtual void Shutdown() = 0;

	public:
		const String& GetName() const { return mName; }

	protected:
		void OnAllocation(size_t size);
		void OnFree(size_t size);

	protected:
		friend class Allocations;

		String mName;
	};

}
