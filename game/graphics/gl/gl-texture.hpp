#pragma once

#include <GL/glew.h>
#include "gl-enums.hpp"
#include "../../error-handling.hpp"
#include <vector>

namespace Graphics {

class GLTexture {
    GLuint m_handle = 0;
    GLTextureTarget m_target;
    GLTextureInternalFormat m_internal_format;
    GLTextureFormat m_format = GLTextureFormat::rgba;
    GLTextureType m_type = GLTextureType::type_byte;
    int m_width = -1;
    int m_height = -1;

public:
    GLTexture(GLTextureTarget target, GLTextureInternalFormat internal_format = GLTextureInternalFormat::rgba8):
            m_target(target),
            m_internal_format(internal_format)
    {};
    ~GLTexture() { if(m_handle) glDeleteTextures(1, &m_handle); }

    void create_texture() {
        glGenTextures(1, &m_handle);
        GLException::check();
    }

    void set_internal_format(GLTextureInternalFormat format) { m_internal_format = format; }
    void set_format(GLTextureFormat format) { m_format = format; }
    void set_type(GLTextureType type) { m_type = type; }

    void bind() {
        glBindTexture((GLenum)m_target, (GLenum) m_handle);
        GLException::check();
    }

    void set_image(int p_width, int p_height, const void *pixels = nullptr) {
        m_width = p_width;
        m_height = p_height;
        glTexImage2D((GLenum)m_target, 0, (GLint)m_internal_format, p_width, p_height, 0, (GLenum)m_format, (GLenum)m_type, pixels);
        GLException::check();
    };

    void set_parameter(GLTextureParameter parameter, GLint value) {
        glTexParameteri((GLenum)m_target, (GLenum)parameter, value);
        GLException::check();
    }

    void set_parameter(GLTextureParameter parameter, GLfloat value) {
        glTexParameterf((GLenum)m_target, (GLenum)parameter, value);
        GLException::check();
    }

    void* download(GLTextureFormat p_format, GLTextureType p_type) {

        if(m_width < 0 || m_height < 0) return nullptr;

        void* buffer = calloc(m_width * m_height, get_texture_format_size(m_format) * get_texture_type_size(m_type));
        glGetTexImage((GLenum) m_target, 0, (GLenum) p_format, (GLenum) p_type, buffer);

        GLenum error = glGetError();
        if(error) {
            free(buffer);
            throw GLException(error);
        }

        return buffer;
    }

    GLuint get_handle() const { return m_handle; }
    GLTextureTarget get_target() const { return m_target; }

    static int get_texture_format_size(GLTextureFormat format) {
        switch(format) {
            case GLTextureFormat::red:
            case GLTextureFormat::stencil_index:
            case GLTextureFormat::depth_component:
                return 1;

            case GLTextureFormat::rg:
            case GLTextureFormat::depth_stencil:
                return 2;

            case GLTextureFormat::rgb:
            case GLTextureFormat::bgr:
            case GLTextureFormat::red_integer:
            case GLTextureFormat::rg_integer:
            case GLTextureFormat::rgb_integer:
            case GLTextureFormat::bgr_integer:
                return 3;

            case GLTextureFormat::rgba_integer:
            case GLTextureFormat::bgra_integer:
            case GLTextureFormat::rgba:
            case GLTextureFormat::bgra:
                return 4;
        }
    }

    static int get_texture_type_size(GLTextureType type) {
        switch(type) {
            case GLTextureType::type_unsigned_byte:
            case GLTextureType::type_unsigned_byte_3_3_2:
            case GLTextureType::type_unsigned_byte_2_3_3_rev:
            case GLTextureType::type_byte:
                return 1;

            case GLTextureType::type_unsigned_short:
            case GLTextureType::type_short:
            case GLTextureType::type_unsigned_short_5_6_5:
            case GLTextureType::type_unsigned_short_5_6_5_rev:
            case GLTextureType::type_unsigned_short_4_4_4_4:
            case GLTextureType::type_unsigned_short_4_4_4_4_rev:
            case GLTextureType::type_unsigned_short_5_5_5_1:
            case GLTextureType::type_unsigned_short_1_5_5_5_rev:
            case GLTextureType::type_half_float:
                return 2;

            case GLTextureType::type_unsigned_int:
            case GLTextureType::type_int:
            case GLTextureType::type_float:
            case GLTextureType::type_unsigned_int_8_8_8_8:
            case GLTextureType::type_unsigned_int_8_8_8_8_rev:
            case GLTextureType::type_unsigned_int_10_10_10_2:
            case GLTextureType::type_unsigned_int_2_10_10_10_rev:
                return 4;
        }
    }

    int get_width() { return m_width; }
    int get_height() { return m_height; }
};

}