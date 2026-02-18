#pragma once

#include <string>

struct Asset {
	virtual ~Asset() = default;
	std::string name;
};