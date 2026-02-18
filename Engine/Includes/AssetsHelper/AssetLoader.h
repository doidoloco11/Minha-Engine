#pragma once

#include <string>
#include "Asset.h"

class AssetRegister;

class AssetLoader {
public:
	AssetLoader() = default;
	virtual ~AssetLoader() = default;
	virtual Asset* Load(AssetRegister* reg, const std::string& path) = 0;
};