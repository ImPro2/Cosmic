module;
#include "cspch.hpp"
export module Cosmic.Base.Singleton;

namespace Cosmic
{
	export template<typename T>
	class Singleton
	{
	public:
		static T& Get()
		{
			return *sInstance;
		}

	protected:
		Singleton()
		{
			sInstance = static_cast<T*>(this);
		}

		Singleton(Singleton&&) noexcept
		{
			sInstance = static_cast<T*>(this);
		}

		~Singleton()
		{
			sInstance = nullptr;
		}

		Singleton& operator=(Singleton&&) noexcept
		{
			sInstance = static_cast<T*>(this);
			return *this;
		}

	private:
		static inline T* sInstance;
	};

}