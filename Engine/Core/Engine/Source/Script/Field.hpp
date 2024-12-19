#pragma once
#include "Base/Base.hpp"
#include "Script/FieldMacros.hpp"

namespace Cosmic
{

	enum class EFieldType : uint16
	{
		Unknown = 0,
		Float32, Float2, Float3, Float4,
		Int32, Int2, Int3, Int4,
		UInt32, UInt2, UInt3, UInt4,
		String,
		Enum
	};

	class IField
	{
	public:
		IField(const String& name, const String& typeName, EFieldType fieldType)
			: mName(name), mTypeName(typeName), mType(fieldType)
		{
		}

		~IField() = default;

	public:
		EFieldType    GetType()      const { return mType;       }
		const String& GetName()      const { return mName;       }
		const String& GetTypeName()  const { return mTypeName;   }
		bool          IsRegistered() const { return mRegistered; }

	public:
		template<typename T>
		void SetValue(const T& value)
		{
			T& data = GetValue<T>();
			data    = value;
		}

		template<typename T>
		T& GetValue()
		{
			return *static_cast<T*>(GetValuePtr());
		}

		template<typename T>
		T& GetDefaultValue()
		{
			return *static_cast<T*>(GetDefaultValuePtr());
		}

		String GetEnumToStringFunctionName();
		String GetEnumFromStringFunctionName();

	public:
		virtual void* GetValuePtr()        = 0;
		virtual void* GetDefaultValuePtr() = 0;

		virtual void Reset() = 0;

	protected:
		void RegisterField();

	protected:
		EFieldType mType       = EFieldType::Unknown;
		String     mName       = "";
		String     mTypeName   = "";
		bool       mRegistered = false;
	};

	template<typename T, char... Chars>
	class Field : public IField
	{
	public:
		Field()
			: IField(ExtractName(), ExtractTypeName(), ExtractFieldType()), mValue(), mDefaultValue()
		{
			RegisterField();
		}

		Field(const T& value)
			: IField(ExtractName(), ExtractTypeName(), ExtractFieldType()), mValue(value), mDefaultValue(value)
		{
			RegisterField();
		}

		Field(T&& value)
			: IField(ExtractName(), ExtractTypeName(), ExtractFieldType()), mValue(std::move(value)), mDefaultValue(mValue)
		{
			RegisterField();
		}

		Field& operator=(const T& value)
		{
			mValue = value;

			if (!mRegistered)
			{
				mDefaultValue = value;
				RegisterField();
			}
		}

		Field& operator=(T&& value)
		{
			mValue = std::move(value);

			if (!mRegistered)
			{
				mDefaultValue = mValue;
				RegisterField();
			}
		}

		T*       operator->()       { return &mValue; }
		const T* operator->() const { return &mValue; }

	public:
		void* GetValuePtr()        override { return &mValue;        }
		void* GetDefaultValuePtr() override { return &mDefaultValue; }

		void Reset() override
		{
			mValue = mDefaultValue;
		}

	public:
		operator T()       { return mValue; }
		operator T() const { return mValue; }

	private:
		String ExtractName()
		{
			Vector<char> vec = { Chars... };
			return String(vec.begin(), vec.end());
		}

		String ExtractTypeName()
		{
			return TypeStr<T>();
		}

		EFieldType ExtractFieldType()
		{
			if constexpr (std::is_same_v<T, float32>)
				return EFieldType::Float32;
			else if constexpr (std::is_same_v<T, float2>)
				return EFieldType::Float2;
			else if constexpr (std::is_same_v<T, float3>)
				return EFieldType::Float3;
			else if constexpr (std::is_same_v<T, float4>)
				return EFieldType::Float4;
			else if constexpr (std::is_same_v<T, int32>)
				return EFieldType::Int32;
			else if constexpr (std::is_same_v<T, int2>)
				return EFieldType::Int2;
			else if constexpr (std::is_same_v<T, int3>)
				return EFieldType::Int3;
			else if constexpr (std::is_same_v<T, int4>)
				return EFieldType::Int4;
			else if constexpr (std::is_same_v<T, uint32>)
				return EFieldType::UInt32;
			else if constexpr (std::is_same_v<T, uint2>)
				return EFieldType::UInt2;
			else if constexpr (std::is_same_v<T, uint3>)
				return EFieldType::UInt3;
			else if constexpr (std::is_same_v<T, uint4>)
				return EFieldType::UInt4;
			else if constexpr (std::is_same_v<T, String>)
				return EFieldType::String;
			else if constexpr (std::is_enum_v<T>)
				return EFieldType::Enum;
			else
				static_assert(false);

			return EFieldType::Unknown;
		}

	private:
		T mValue;
		T mDefaultValue;

		String mTypeName;
	};

}
