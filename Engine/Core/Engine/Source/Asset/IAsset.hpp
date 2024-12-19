#pragma once
#include "Base/Types.hpp"
#include "App/Path.hpp"
#include "Memory/Memory.hpp"

#define CS_INVALID_ASSET_HANDLE 0

namespace Cosmic
{

	enum class EAssetType : uint16
	{
		None,
		Scene,
		Texture2D
	};

	enum class EAssetFlag : uint16
	{
		None    = 0,
		Valid   = 1,
		Missing = 2
	};

	using EAssetFlags = uint16;
	using AssetHandle = uint32;

	struct AssetMetadata
	{
		EAssetType  Type;
		AssetHandle Handle;
		bool        IsLoaded;
		Path        FilePath; // rel to assets dir
	};

	class IAsset : public IRefCounted
	{
	public:
		inline EAssetType  GetAssetType()    const     { return mAssetType;              }
		inline EAssetFlag  GetAssetFlags()   const     { return (EAssetFlag)mAssetFlags; }
		inline AssetHandle GetAssetHandle()  const     { return mAssetHandle;            }

		inline void SetAssetType(EAssetType type)      { mAssetType   = type;            }
		inline void SetAssetFlags(EAssetFlag flags)    { mAssetFlags  = (uint16)flags;   }
		inline void SetAssetHandle(AssetHandle handle) { mAssetHandle = handle;          }

		inline bool IsValid() const
		{
			return (mAssetFlags & (uint16)EAssetFlag::Valid) && !(mAssetFlags & (uint16)EAssetFlag::Missing);
		}

	public:
		bool operator==(const IAsset& other) { return mAssetHandle == other.mAssetHandle; }
		bool operator!=(const IAsset& other) { return mAssetHandle != other.mAssetHandle; }

	private:
		EAssetType  mAssetType    = EAssetType::None;
		uint16      mAssetFlags   = (uint16)EAssetFlag::None;
		AssetHandle mAssetHandle  = CS_INVALID_ASSET_HANDLE;
	};

}
