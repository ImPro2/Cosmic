#pragma once
#include "Base/Types.hpp"

namespace Cosmic
{

	template<typename E, typename = std::enable_if_t<std::is_enum_v<E>>>
	class BitFlags
	{
	public:
		using UnderlyingType = std::underlying_type_t<E>;

	public:
		// Constructors

		BitFlags()
			: mFlags(static_cast<UnderlyingType>(0))
		{
		}

		BitFlags(const UnderlyingType& value)
			: mFlags(value)
		{
		}

		BitFlags(const E& value)
			: mFlags(static_cast<UnderlyingType>(value))
		{
		}

		BitFlags(std::initializer_list<E> flags)
		{
			for (E value : flags)
			{
				mFlags |= value;
			}
		}

		// Assignment operators

		BitFlags& operator=(const BitFlags& other)
		{
			mFlags = other.mFlags;
			return *this;
		}

		BitFlags& operator=(const E& other)
		{
			mFlags = static_cast<UnderlyingType>(other);
			return *this;
		}

	public:
		inline void Set(const E& value)   { mFlags |=  static_cast<UnderlyingType>(value); }
		inline void Unset(const E& value) { mFlags &= ~static_cast<UnderlyingType>(value); }

		inline bool IsSet(const E& value)
		{
			return (mFlags & static_cast<UnderlyingType>(value)) == static_cast<UnderlyingType>(value);
		}

		void Clear() { mFlags = static_cast<UnderlyingType>(0); }

	public:
		// Overloaded operators

		inline operator bool() const
		{
			return mFlags != static_cast<UnderlyingType>(0);
		}

		inline operator UnderlyingType() const
		{
			return mFlags;
		}

		

	public:
		void Set(const UnderlyingType& flags) { mFlags = flags; }
		UnderlyingType Get() const { return mFlags; }

	private:
		UnderlyingType mFlags;
	};

	template<typename E>
	bool operator==(const BitFlags<E>& lhs, const BitFlags<E>& rhs)
	{
		return lhs.Get() == rhs.Get();
	}

	template<typename E>
	inline bool operator!=(const BitFlags<E>& lhs, const BitFlags<E>& rhs)
	{
		return lhs.Get() != rhs.Get();
	}

	template<typename E>
	BitFlags<E> operator~(const BitFlags<E>& other)
	{
		return BitFlags<E>(~other.Get());
	}

	template<typename E>
	BitFlags<E> operator~(const E& other)
	{
		return BitFlags<E>(~static_cast<BitFlags<E>::UnderlyingType>(other));
	}

	template<typename E>
	BitFlags<E> operator|(const BitFlags<E>& lhs, const BitFlags<E>& rhs)
	{
		return BitFlags<E>(lhs.Get() | rhs.Get());
	}

	template<typename E>
	BitFlags<E> operator|(const BitFlags<E>& lhs, const E& rhs)
	{
		return BitFlags<E>(lhs.Get() | static_cast<BitFlags<E>::UnderlyingType>(rhs));
	}

	template<typename E>
	BitFlags<E>& operator|=(BitFlags<E>& lhs, const BitFlags<E>& rhs)
	{
		return lhs = lhs | rhs;
	}

	template<typename E>
	BitFlags<E>& operator|=(BitFlags<E>& lhs, const E& rhs)
	{
		return lhs = lhs | rhs;
	}

	template<typename E>
	BitFlags<E> operator&(const BitFlags<E>& lhs, const BitFlags<E>& rhs)
	{
		return BitFlags<E>(lhs.Get() & rhs.Get());
	}

	template<typename E>
	BitFlags<E> operator&(const BitFlags<E>& lhs, const E& rhs)
	{
		return BitFlags<E>(lhs.Get() & static_cast<BitFlags<E>::UnderlyingType>(rhs));
	}

	template<typename E>
	BitFlags<E>& operator&=(BitFlags<E>& lhs, const BitFlags<E>& rhs)
	{
		return lhs = lhs & rhs;
	}

	template<typename E>
	BitFlags<E>& operator&=(BitFlags<E>& lhs, const E& rhs)
	{
		return lhs = lhs & rhs;
	}

	template<typename E>
	BitFlags<E> operator^(const BitFlags<E>& lhs, const BitFlags<E>& rhs)
	{
		return BitFlags(lhs.Get() ^ rhs.Get());
	}

	template<typename E>
	BitFlags<E> operator^(const BitFlags<E>& lhs, const E& rhs)
	{
		return BitFlags(lhs.Get() ^ static_cast<BitFlags<E>::UnderlyingType>(rhs));
	}

	template<typename E>
	BitFlags<E>& operator^=(BitFlags<E>& lhs, const BitFlags<E>& rhs)
	{
		return lhs = lhs ^ rhs;
	}

	template<typename E>
	BitFlags<E>& operator^=(BitFlags<E>& lhs, const E& rhs)
	{
		return lhs = lhs ^ rhs;
	}

}

