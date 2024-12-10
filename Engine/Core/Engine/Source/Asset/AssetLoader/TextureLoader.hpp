#pragma once
#include "IAssetLoader.hpp"

namespace Cosmic
{

	class Texture2DLoader : public IAssetLoader
	{
	public:
		virtual bool LoadData(const AssetMetadata& metadata, Ref<IAsset>& asset)       override;
		virtual bool SaveData(const AssetMetadata& metadata, const Ref<IAsset>& asset) override;
	};

}
