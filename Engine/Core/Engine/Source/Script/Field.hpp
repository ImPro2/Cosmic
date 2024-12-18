#pragma once
#include "Base/Types.hpp"

namespace Cosmic
{

	enum class EFieldType : uint16
	{
		Unknown = 0,
		Float32, Float2, Float3, Float4,
		Int32, Int2, Int3, Int4,
		UInt32, UInt2, UInt3, UInt4,
		String
	};

	class IField
	{
	public:
		IField()  = default;
		~IField() = default;

	public:
		EFieldType GetType()      const { return mType;       }
		bool       IsRegistered() const { return mRegistered; }

	public:
		template<typename T>
		T& GetValue()
		{
			return static_cast<T&>(*GetValuePtr());
		}

		virtual void* GetValuePtr() = 0;

	protected:
		void RegisterField();

	protected:
		EFieldType mType       = EFieldType::Unknown;
		bool       mRegistered = false;
	};

	template<typename T>
	class Field : public IField
	{
	public:
		Field()
		{
			InitializeField();
		}

		Field(const T& value)
			: mValue(value)
		{
			InitializeField();
		}

		Field(T&& value)
			: mValue(std::move(value))
		{
			InitializeField();
		}

		Field& operator=(const T& value)
		{
			mValue = value;
			InitializeField();
		}

		Field& operator=(T&& value)
		{
			mValue = std::move(value);
			InitializeField();
		}

	public:
		void* GetValuePtr() override { return &mValue; }

	public:
		operator T()       { return mValue; }
		operator T() const { return mValue; }

	private:
		void InitializeField()
		{
			if (mRegistered)
				return;

			mRegistered = true;

			if constexpr (std::is_same_v<T, float32>)
				mType = EFieldType::Float32;
			else if constexpr (std::is_same_v<T, float2>)
				mType = EFieldType::Float2;
			else if constexpr (std::is_same_v<T, float3>)
				mType = EFieldType::Float3;
			else if constexpr (std::is_same_v<T, float4>)
				mType = EFieldType::Float4;
			else if constexpr (std::is_same_v<T, int32>)
				mType = EFieldType::Int32;
			else if constexpr (std::is_same_v<T, int2>)
				mType = EFieldType::Int2;
			else if constexpr (std::is_same_v<T, int3>)
				mType = EFieldType::Int3;
			else if constexpr (std::is_same_v<T, int4>)
				mType = EFieldType::Int4;
			else if constexpr (std::is_same_v<T, uint32>)
				mType = EFieldType::UInt32;
			else if constexpr (std::is_same_v<T, uint2>)
				mType = EFieldType::UInt2;
			else if constexpr (std::is_same_v<T, uint3>)
				mType = EFieldType::UInt3;
			else if constexpr (std::is_same_v<T, uint4>)
				mType = EFieldType::UInt4;
			else if constexpr (std::is_same_v<T, String>)
				mType = EFieldType::String;
			else
				static_assert(false);

			RegisterField();
		}

	private:
		T mValue;
	};

}
