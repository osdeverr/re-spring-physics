#pragma once

namespace Graphics {

struct ShadowMapFramebuffer;

}

#include "../gl/gl-texture.hpp"
#include "scene-directional-light.hpp"
#include "../../utils/vec2.hpp"
#include "../gl/gl-framebuffer.hpp"
#include "textured-framebuffer.hpp"

namespace Graphics {

struct ShadowMapFramebuffer : public TexturedFramebuffer {

public:
    ShadowMapFramebuffer() : TexturedFramebuffer() {
    }

    void configure_texture() override {
        TexturedFramebuffer::configure_texture();

        m_texture.set_parameter(GLTextureParameter::texture_compare_func, GL_LEQUAL);
        m_texture.set_parameter(GLTextureParameter::texture_compare_mode, GL_COMPARE_REF_TO_TEXTURE);
        m_texture.set_internal_format(GLTextureInternalFormat::depth_component);
        m_texture.set_format(GLTextureFormat::depth_component);
        m_texture.set_parameter(Graphics::GLTextureParameter::texture_mag_filter, GL_LINEAR);
        m_texture.set_parameter(Graphics::GLTextureParameter::texture_min_filter, GL_LINEAR);
    }

    void resize_framebuffer() override {
        // Note: not calling implementation of the superclass

        m_framebuffer.bind_depth_texture(&m_texture);
    }


};

}