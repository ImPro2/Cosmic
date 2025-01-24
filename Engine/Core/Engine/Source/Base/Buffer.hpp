#pragma once
#include "Types.hpp"
#include "Memory/Memory.hpp"

namespace Cosmic
{

	class Buffer
	{
	public:
		Buffer() = default;

		Buffer(uint8* data, size_t size)
		{
			Copy(data, size);

			IncRef();
		}

		Buffer(size_t size)
			: mData(DefaultAllocator::AllocateArray<uint8>(size)), mSize(size)
		{
			IncRef();
		}

		~Buffer()
		{
			DecRef();
		}

		Buffer& operator=(const uint8* data) = delete;

		Buffer& operator=(const Buffer& other)
		{
			Copy(other);
			IncRef();
			return *this;
		}

		uint8& operator[](size_t i)       { return mData[i]; }
		uint8  operator[](size_t i) const { return mData[i]; }

		uint8*       GetData()       { return mData; }
		const uint8* GetData() const { return mData; }

		size_t GetSize() const { return mSize; }

	public:
		void Copy(const uint8* data, size_t size)
		{
			if (mData)
				DefaultAllocator::FreeArray<uint8>(mData, mSize);

			mData = DefaultAllocator::AllocateArray<uint8>(size);

			memcpy(mData, data, size);
			mSize = size;
		}

		void Copy(const Buffer& other)
		{
			Copy(other.mData, other.mSize);
		}

		void Resize(size_t size)
		{
			Copy(mData, size);
		}

		void Clear()
		{
			memcpy(mData, 0, mSize);
		}

		void Free()
		{
			if (mData)
				DefaultAllocator::FreeArray<uint8>(mData, mSize);

			mData = nullptr;
			mSize = 0;
		}

	private:
		void IncRef() const
		{
			if (mData)
				ReferenceCounter::IncRefCount(mData);
		}

		void DecRef() const
		{
            if (mData)
            {
                uint32 refCount = ReferenceCounter::DecRefCount<uint8, DefaultAllocator, true>(mData, mSize);

				if (refCount == 0)
				{
					mData = nullptr;
					mSize = 0;
				}
            }
		}

	private:
		mutable uint8* mData = nullptr;
		mutable size_t mSize = 0;
	};

}
