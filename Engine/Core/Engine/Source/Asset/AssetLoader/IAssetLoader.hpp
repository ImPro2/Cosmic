#pragma once
#include "Asset/IAsset.hpp"
#include "Memory/Memory.hpp"

namespace Cosmic
{

	class IAssetLoader
	{
	public:
		virtual bool LoadData(const AssetMetadata& metadata, Ref<IAsset>& asset)       = 0;
		virtual bool SaveData(const AssetMetadata& metadata, const Ref<IAsset>& asset) = 0;
	};

}
