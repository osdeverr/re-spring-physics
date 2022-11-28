#pragma once

namespace Graphics {

struct TexturedFramebuffer;

}

#include "../gl/gl-texture.hpp"
#include "scene-directional-light.hpp"
#include "../../utils/vec2.hpp"
#include "../gl/gl-framebuffer.hpp"

namespace Graphics {

struct TexturedFramebuffer {
    Graphics::GLTexture m_texture;
    Graphics::GLFramebuffer m_framebuffer;
    Vec2i m_size { -1, -1 };

    TexturedFramebuffer():
        m_texture(Graphics::GLTextureTarget::texture_2d, Graphics::GLTextureInternalFormat::rgb32f),
        m_framebuffer(){
    }

    void initialize() {
        m_texture.create_texture();
        m_texture.bind();
        configure_texture();

        m_framebuffer.create_framebuffer();
        m_framebuffer.bind();
    }

    virtual void configure_texture() {
        m_texture.set_type(Graphics::GLTextureType::type_float);
        m_texture.set_format(Graphics::GLTextureFormat::rgb);
        m_texture.set_parameter(Graphics::GLTextureParameter::texture_mag_filter, GL_NEAREST);
        m_texture.set_parameter(Graphics::GLTextureParameter::texture_min_filter, GL_NEAREST);
        m_texture.set_parameter(Graphics::GLTextureParameter::texture_wrap_s, GL_CLAMP_TO_EDGE);
        m_texture.set_parameter(Graphics::GLTextureParameter::texture_wrap_t, GL_CLAMP_TO_EDGE);
    }

    virtual void resize_framebuffer() {
        m_framebuffer.bind_color_texture(&m_texture);
        m_framebuffer.create_depth_buffer(m_size.x, m_size.y);
    }

    void set_size(const Vec2i &size) {
        m_size = size;
        m_texture.bind();
        m_texture.set_image(size.x, size.y);

        m_framebuffer.bind();
        resize_framebuffer();
    }

    virtual void make_drawing_target() {
        m_framebuffer.bind();
        glViewport(0, 0, m_size.x, m_size.y);
    }
};

}