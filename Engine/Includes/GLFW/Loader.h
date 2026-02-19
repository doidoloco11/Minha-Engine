#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "../AssetsHelper/AssetLoader.h"
#include "Assets.h"

class ShaderLoader : public AssetLoader {
    Asset* Load(AssetRegister* reg, const fs::path& path) override {
        if (path.extension() == ".comp") {
            Shader* shader = new Shader(path.string().c_str());
            shader->name = path.string();
            return shader;
        }
        else if (path.extension() == ".vert") {
            std::string baseName = path.stem().string();
            std::string vertPath = path.string();
            std::string fragPath = baseName + ".frag";
            std::string geomPath = baseName + ".geom";

            if (fs::exists(geomPath)) {
                Shader* shader = new Shader(vertPath.c_str(), geomPath.c_str(), fragPath.c_str());
                shader->name = baseName + ".glsl";
                return shader;
            }
            else {
                Shader* shader = new Shader(vertPath.c_str(), fragPath.c_str());
                shader->name = baseName + ".glsl";
                return shader;
            }
        }
        return nullptr;
    }
};