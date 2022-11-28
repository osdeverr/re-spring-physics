
#include <FreeImage.h>
#include <iostream>
#include "scene-texture.hpp"

namespace Graphics {

    bool SceneTexture::load(const std::string& path) {

        FIBITMAP* bitmap = FreeImage_Load(FreeImage_GetFileType(path.c_str(), 0), path.c_str());
        FIBITMAP* converted = FreeImage_ConvertTo32Bits(bitmap);
        FreeImage_Unload(bitmap);

        auto* image = (unsigned char*) FreeImage_GetBits(converted);
        int image_width = (int)FreeImage_GetWidth(converted);
        int image_height = (int)FreeImage_GetHeight(converted);
        int image_channels = (int)FreeImage_GetBPP(converted) / 8;

        FreeImage_Unload(converted);

        if (!image) {
            return false;
        }

        GLTextureFormat format = GLTextureFormat::rgba;
        GLTextureInternalFormat internal_format = GLTextureInternalFormat::rgba8;

        switch(image_channels) {
            case 1:
                format = GLTextureFormat::red;
                internal_format = GLTextureInternalFormat::r8;
                break;
            case 2:
                format = GLTextureFormat::rg;
                internal_format = GLTextureInternalFormat::rg8;
                break;
            case 3:
                format = GLTextureFormat::rgb;
                internal_format = GLTextureInternalFormat::rgba8;
                break;
            case 4:
                format = GLTextureFormat::rgba;
                internal_format = GLTextureInternalFormat::rgba8;
                break;
            default:
                free(image);
                return false;
        }

        m_gl_texture.set_format(format);
        m_gl_texture.set_internal_format(internal_format);
        m_gl_texture.create_texture();
        m_gl_texture.bind();
        m_gl_texture.set_parameter(GLTextureParameter::texture_min_filter, GL_LINEAR);
        m_gl_texture.set_parameter(GLTextureParameter::texture_mag_filter, GL_LINEAR);
        m_gl_texture.set_parameter(GLTextureParameter::texture_wrap_s, GL_REPEAT);
        m_gl_texture.set_parameter(GLTextureParameter::texture_wrap_t, GL_REPEAT);
        m_gl_texture.set_parameter(GLTextureParameter::texture_wrap_r, GL_REPEAT);
        m_gl_texture.set_image(image_width, image_height, image);

        return true;
    }

    void SceneTexture::make_resident(bool resident) {
        if (resident == m_resident) {
            return;
        }

        m_resident = resident;
        if (resident) {
            if(m_bindless_handle == -1) {
                m_bindless_handle = glGetTextureHandleARB(m_gl_texture.get_handle());
            }
            glMakeTextureHandleResidentARB(m_bindless_handle);
        } else {
            glMakeTextureHandleNonResidentARB(m_bindless_handle);
        }
    }
}