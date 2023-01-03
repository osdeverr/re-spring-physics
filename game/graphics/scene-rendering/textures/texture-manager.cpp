
#include "texture-manager.hpp"

Graphics::SceneTexture *Graphics::TextureManager::load_texture(const std::string &path) {
    auto it = m_textures.find(path);
    if (it != m_textures.end()) {
        return it->second.get();
    }
    auto texture = std::make_unique<SceneTexture>();
    texture->load(path);
    auto* ptr = texture.get();
    m_textures[path] = std::move(texture);
    return ptr;
}
