#pragma once
#include "Base/Types.hpp"

namespace Cosmic
{

	template<size_t N, typename T = byte>
	class StaticBuffer
	{
	public:
		StaticBuffer()  = default;
		~StaticBuffer() = default;

	public:
		T*             GetData()       { return mBuffer; }
		const T* const GetData() const { return mBuffer; }

		T&       operator[](size_t i)       { return mBuffer[i]; }
		const T& operator[](size_t i) const { return mBuffer[i]; }

	private:
		T mBuffer[N];
	};

}
