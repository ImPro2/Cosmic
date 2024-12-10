#pragma once
#include "IAsset.hpp"
#include "AssetRegistry.hpp"
#include "AssetLoader/AssetLoader.hpp"

namespace Cosmic
{

	using AssetHandleMap = UnorderedMap<AssetHandle, Ref<IAsset>>;

	class AssetManager
	{
	public:
		static void Init();
		static void Shutdown();

	public:


	private:
		inline static AssetHandleMap sLoadedAssets;
		inline static AssetRegistry  sAssetRegistry;
		inline static AssetLoader    sAssetLoader;
	};

}
