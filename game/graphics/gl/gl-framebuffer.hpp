#pragma once

#include <GL/glew.h>
#include "gl-texture.hpp"
#include "../../error-handling.hpp"
#include "../../utils/vec2.hpp"

namespace Graphics {

class GLFramebuffer {
    GLenum m_handle = 0;
    GLuint m_depth_buffer_handle = 0;

public:
    GLFramebuffer() {};
    ~GLFramebuffer() {
        if(m_handle) glDeleteFramebuffers(1, &m_handle);
        if(m_depth_buffer_handle) glDeleteRenderbuffers(1, &m_depth_buffer_handle);
    }

    void create_framebuffer() {
        glGenFramebuffers(1, &m_handle);
        GLException::check();
    }

    void create_depth_buffer(GLsizei width, GLsizei height) {
        if(m_depth_buffer_handle == 0) {
            bind();
            glGenRenderbuffers(1, &m_depth_buffer_handle);
            glBindRenderbuffer(GL_RENDERBUFFER, m_depth_buffer_handle);
        }
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depth_buffer_handle);
        GLException::check();
    }

    void bind() const {
        glBindFramebuffer(GL_FRAMEBUFFER, m_handle);
        GLException::check();
    }

    void make_draw_buffer() {
        GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
        glDrawBuffers(1, DrawBuffers);
        GLException::check();
    }

    static void unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void bind_depth_texture(GLTexture* depth_texture) {
        if(depth_texture) {
            glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depth_texture->get_handle(), 0);
        } else {
            glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, 0, 0);
        }
    }

    void bind_color_texture(GLTexture* texture) {
        if(texture) {
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture->get_handle(), 0);
        } else {
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 0, 0);
        }
    }

    bool is_complete() {
        return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
    }
};

}