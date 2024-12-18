#pragma once
#include <random>

namespace Cosmic
{

	template<typename T>
	inline T Random(T min, T max)
	{
		CS_BREAK();
		return (T)nullptr;
	}

	template<>
	inline int32 Random<int32>(int32 min, int32 max)
	{
		static std::random_device rd;
		static std::mt19937 engine(rd());
		static std::uniform_int_distribution<int32> dist(min, max);

		return dist(engine);
	}

}
