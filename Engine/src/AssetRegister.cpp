#include <filesystem>
#include "../Includes/AssetsHelper/Asset.h"
#include "../Includes/AssetsHelper/AssetLoader.h"
#include "../Includes/AssetsHelper/AssetRegister.h"

namespace fs = std::filesystem;

void AssetRegister::LoadAllAssets(const std::string& assetsDirectory) {
	for (const auto& entry : fs::recursive_directory_iterator(fs::path(assetsDirectory))) {
		if (entry.is_regular_file()) {
			auto p = entry.path();

			std::string ext = p.extension().string();

			auto loader = loaders.find(ext.size() > 1 ? ext.substr(1) : ext);
			if (loader == loaders.end()) {
				printf("No loader for %s\n", ext.c_str());
				continue;
			}
			Asset* asset = loader->second->Load(this, p.string());
			if (asset) assets[asset->name] = asset;
		}
	}
}
