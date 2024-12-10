#pragma once
#include "Asset/IAsset.hpp"
#include "Asset/AssetLoader/IAssetLoader.hpp"

namespace Cosmic
{

	using AssetLoaderMap = UnorderedMap<EAssetType, Scope<IAssetLoader>>;

	class AssetLoader
	{
	public:
		void Init();
		void Shutdown();

	public:
		bool LoadData(const AssetMetadata& metadata, Ref<IAsset>& asset);
		bool SaveData(const AssetMetadata& metadata, const Ref<IAsset>& asset);

	private:
		AssetLoaderMap mLoaderMap;
	};

}
