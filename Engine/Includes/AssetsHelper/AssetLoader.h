#pragma once

#include <string>
#include <filesystem>
#include "Asset.h"

namespace fs = std::filesystem;

class AssetRegister;

class AssetLoader {
public:
	AssetLoader() = default;
	virtual ~AssetLoader() = default;
	virtual Asset* Load(AssetRegister* reg, const fs::path& path) = 0;
};