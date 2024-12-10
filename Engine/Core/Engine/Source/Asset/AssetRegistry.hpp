#pragma once
#include "Base/Types.hpp"
#include "IAsset.hpp"

namespace Cosmic
{

	// wrapper for unordered_map<Path -> AssetMetadata>
	// Takes absolute paths and stores relative paths
	
	using AssetRegistryMap = UnorderedMap<Path, AssetMetadata>;

	class AssetRegistry
	{
	public:
		AssetRegistry() = default;
		AssetRegistry(const Path& assetDir);

	public:
		bool Contains(const Path& path) const;
		void Remove(const Path& path);

	public:
		AssetMetadata&       Get(const Path& path);
		const AssetMetadata& Get(const Path& path) const;

		AssetMetadata&       operator[](const Path& path);

	private:
		Path RelativeToAssetDir(const Path& path) const;

	public:
		AssetRegistryMap::iterator       begin()  { return mAssetMap.begin();  }
		AssetRegistryMap::iterator       end()    { return mAssetMap.end();    }
		AssetRegistryMap::const_iterator cbegin() { return mAssetMap.cbegin(); }
		AssetRegistryMap::const_iterator cend()   { return mAssetMap.cend();   }

	private:
		AssetRegistryMap mAssetMap;
		Path             mAssetDirectory;
	};

}
