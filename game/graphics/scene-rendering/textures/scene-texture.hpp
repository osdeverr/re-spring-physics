#pragma once

#include "texture-manager.hpp"
#include "../../gl/gl-texture.hpp"
#include <string>
#include <GL/glew.h>

namespace Graphics {
    class TextureManager;

    struct SceneTexture {
        GLTexture m_gl_texture { GLTextureTarget::texture_2d };
        GLuint64 m_bindless_handle = 0;
        bool m_resident = false;

        SceneTexture() = default;
        ~SceneTexture() {
            make_resident(false);
        }

        bool load(const std::string &path);
        void make_resident(bool resident);
    };
}