#pragma once

#include <string>
#include <unordered_map>

struct Asset;
class AssetLoader;

class AssetRegister {
public:
	static AssetRegister& getInstance() {
		static AssetRegister instance;
		return instance;
	};

	template<class Loader>
	void AddAssetLoader(const std::string& ext) {
		static_assert(std::is_base_of<AssetLoader, Loader>::value, "Loader must be derived from AssetLoader");
		loaders[ext] = new Loader();
	};
	void LoadAllAssets(const std::string& assetsDirectory);
	void Unload() {
		for (auto asset : assets) {
			delete asset.second;
		}
		assets.clear();
		for (auto loader : loaders) {
			delete loader.second;
		}
		loaders.clear();
	};
	const Asset* GetAsset(const std::string& name) {
		if (assets.find(name) != assets.end()) {
			return assets[name];
		}
		return nullptr;
	};

private:
	std::unordered_map<std::string, AssetLoader*> loaders;
	std::unordered_map<std::string, Asset*> assets;
};