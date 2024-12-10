#include "cspch.hpp"
#include "AssetRegistry.hpp"
#include "App/FileSystem.hpp"

namespace Cosmic
{

	AssetRegistry::AssetRegistry(const Path& assetDir)
		: mAssetDirectory(assetDir)
	{
	}

	bool AssetRegistry::Contains(const Path& path) const
	{
		Path rel = RelativeToAssetDir(path);
		return mAssetMap.contains(rel);
	}

	void AssetRegistry::Remove(const Path& path)
	{
		Path rel = RelativeToAssetDir(path);
		mAssetMap.erase(rel);
	}

	AssetMetadata& AssetRegistry::Get(const Path& path)
	{
		Path rel = RelativeToAssetDir(path);
		return mAssetMap.at(rel);
	}

	const AssetMetadata& AssetRegistry::Get(const Path& path) const
	{
		Path rel = RelativeToAssetDir(path);
		return mAssetMap.at(path);
	}

	AssetMetadata& AssetRegistry::operator[](const Path& path)
	{
		Path rel = RelativeToAssetDir(path);
		return mAssetMap[path];
	}

	Path AssetRegistry::RelativeToAssetDir(const Path& path) const
	{
		if (path.IsAbsolute())
			return path.RelativeFrom(mAssetDirectory);

		return path;
	}

}
