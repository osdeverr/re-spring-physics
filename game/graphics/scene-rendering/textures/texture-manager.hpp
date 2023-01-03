#pragma once

#include <vector>
#include <memory>
#include "scene-texture.hpp"
#include <unordered_map>

namespace Graphics {
    struct SceneTexture;
    // A class that allows to load textures from files
    // and toggle their residency.

    class TextureManager {
        std::unordered_map<std::string, std::unique_ptr<SceneTexture>> m_textures;
    public:
        TextureManager() = default;

        // Loads a texture from a file and returns its handle
        // If the texture is already loaded, it returns the handle of the existing texture
        // If the texture is not loaded, it loads it and returns its handle

        SceneTexture* load_texture(const std::string& path);
    };

}