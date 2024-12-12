#include "cspch.hpp"
#include "AssetLoader.hpp"

#include "SceneLoader.hpp"
#include "TextureLoader.hpp"

namespace Cosmic
{

	void AssetLoader::Init()
	{
		mLoaderMap[EAssetType::Scene]     = CreateScope<SceneLoader>();
		mLoaderMap[EAssetType::Texture2D] = std::move(CreateScope<Texture2DLoader>());
	}

	void AssetLoader::Shutdown()
	{
		mLoaderMap.clear();
	}

	bool AssetLoader::LoadData(const AssetMetadata& metadata, Ref<IAsset>& asset)
	{
		return mLoaderMap[metadata.Type]->LoadData(metadata, asset);
	}

	bool AssetLoader::SaveData(const AssetMetadata& metadata, const Ref<IAsset>& asset)
	{
		return mLoaderMap[metadata.Type]->SaveData(metadata, asset);
	}

}
