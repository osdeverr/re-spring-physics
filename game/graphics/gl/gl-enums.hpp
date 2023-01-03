#pragma once

#include <GL/glew.h>

namespace Graphics {
/// @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBindTexture.xhtml

enum class GLTextureTarget {
    texture_1d                   = GL_TEXTURE_1D,
    texture_2d                   = GL_TEXTURE_2D,
    texture_3d                   = GL_TEXTURE_3D,
    texture_1d_array             = GL_TEXTURE_1D_ARRAY,
    texture_2d_array             = GL_TEXTURE_2D_ARRAY,
    texture_rectangle            = GL_TEXTURE_RECTANGLE,
    texture_cube_map             = GL_TEXTURE_CUBE_MAP,
    texture_cube_map_array       = GL_TEXTURE_CUBE_MAP_ARRAY,
    texture_buffer               = GL_TEXTURE_BUFFER,
    texture_2d_multisample       = GL_TEXTURE_2D_MULTISAMPLE,
    texture_2d_multisample_array = GL_TEXTURE_2D_MULTISAMPLE_ARRAY,
};

/// @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexImage2D.xhtml

enum class GLTextureInternalFormat {
    r8           = GL_R8,
    r16          = GL_R16,
    r16f         = GL_R16F,
    r32f         = GL_R32F,
    r8i          = GL_R8I,
    r16i         = GL_R16I,
    r32i         = GL_R32I,
    r8ui         = GL_R8UI,
    r16ui        = GL_R16UI,
    r32ui        = GL_R32UI,
    rg8          = GL_RG8,
    rg16         = GL_RG16,
    rg16f        = GL_RG16F,
    rg32f        = GL_RG32F,
    rg8i         = GL_RG8I,
    rg16i        = GL_RG16I,
    rg32i        = GL_RG32I,
    rg8ui        = GL_RG8UI,
    rg16ui       = GL_RG16UI,
    rg32ui       = GL_RG32UI,
    rgb32f       = GL_RGB32F,
    rgb32i       = GL_RGB32I,
    rgb32ui      = GL_RGB32UI,
    rgba8        = GL_RGBA8,
    rgba16       = GL_RGBA16,
    rgba16f      = GL_RGBA16F,
    rgba32f      = GL_RGBA32F,
    rgba8i       = GL_RGBA8I,
    rgba16i      = GL_RGBA16I,
    rgba32i      = GL_RGBA32I,
    rgba8ui      = GL_RGBA8UI,
    rgba16ui     = GL_RGBA16UI,
    rgba32ui     = GL_RGBA32UI,
    depth_component    = GL_DEPTH_COMPONENT,
    depth_component16  = GL_DEPTH_COMPONENT16,
    depth_component24  = GL_DEPTH_COMPONENT24,
    depth_component32f = GL_DEPTH_COMPONENT32F
};

/// @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexImage2D.xhtml

enum class GLTextureFormat {
    red             = GL_RED,
    rg              = GL_RG,
    rgb             = GL_RGB,
    bgr             = GL_BGR,
    rgba            = GL_RGBA,
    bgra            = GL_BGRA,
    red_integer     = GL_RED_INTEGER,
    rg_integer      = GL_RG_INTEGER,
    rgb_integer     = GL_RGB_INTEGER,
    bgr_integer     = GL_BGR_INTEGER,
    rgba_integer    = GL_RGBA_INTEGER,
    bgra_integer    = GL_BGRA_INTEGER,
    stencil_index   = GL_STENCIL_INDEX,
    depth_component = GL_DEPTH_COMPONENT,
    depth_stencil   = GL_DEPTH_STENCIL,
};

/// @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexImage2D.xhtml

enum class GLTextureType {
    type_unsigned_byte                = GL_UNSIGNED_BYTE,
    type_byte                         = GL_BYTE,
    type_unsigned_short               = GL_UNSIGNED_SHORT,
    type_short                        = GL_SHORT,
    type_unsigned_int                 = GL_UNSIGNED_INT,
    type_int                          = GL_INT,
    type_half_float                   = GL_HALF_FLOAT,
    type_float                        = GL_FLOAT,
    type_unsigned_byte_3_3_2          = GL_UNSIGNED_BYTE_3_3_2,
    type_unsigned_byte_2_3_3_rev      = GL_UNSIGNED_BYTE_2_3_3_REV,
    type_unsigned_short_5_6_5         = GL_UNSIGNED_SHORT_5_6_5,
    type_unsigned_short_5_6_5_rev     = GL_UNSIGNED_SHORT_5_6_5_REV,
    type_unsigned_short_4_4_4_4       = GL_UNSIGNED_SHORT_4_4_4_4,
    type_unsigned_short_4_4_4_4_rev   = GL_UNSIGNED_SHORT_4_4_4_4_REV,
    type_unsigned_short_5_5_5_1       = GL_UNSIGNED_SHORT_5_5_5_1,
    type_unsigned_short_1_5_5_5_rev   = GL_UNSIGNED_SHORT_1_5_5_5_REV,
    type_unsigned_int_8_8_8_8         = GL_UNSIGNED_INT_8_8_8_8,
    type_unsigned_int_8_8_8_8_rev     = GL_UNSIGNED_INT_8_8_8_8_REV,
    type_unsigned_int_10_10_10_2      = GL_UNSIGNED_INT_10_10_10_2,
    type_unsigned_int_2_10_10_10_rev  = GL_UNSIGNED_INT_2_10_10_10_REV
};

/// @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexParameter.xhtml

enum class GLTextureParameter {
    depth_stencil_texture_mode = GL_DEPTH_STENCIL_TEXTURE_MODE,
    texture_base_level         = GL_TEXTURE_BASE_LEVEL,
    texture_compare_func       = GL_TEXTURE_COMPARE_FUNC,
    texture_compare_mode       = GL_TEXTURE_COMPARE_MODE,
    texture_lod_bias           = GL_TEXTURE_LOD_BIAS,
    texture_min_filter         = GL_TEXTURE_MIN_FILTER,
    texture_mag_filter         = GL_TEXTURE_MAG_FILTER,
    texture_min_lod            = GL_TEXTURE_MIN_LOD,
    texture_max_lod            = GL_TEXTURE_MAX_LOD,
    texture_max_level          = GL_TEXTURE_MAX_LEVEL,
    texture_swizzle_r          = GL_TEXTURE_SWIZZLE_R,
    texture_swizzle_g          = GL_TEXTURE_SWIZZLE_G,
    texture_swizzle_b          = GL_TEXTURE_SWIZZLE_B,
    texture_swizzle_a          = GL_TEXTURE_SWIZZLE_A,
    texture_wrap_s             = GL_TEXTURE_WRAP_S,
    texture_wrap_t             = GL_TEXTURE_WRAP_T,
    texture_wrap_r             = GL_TEXTURE_WRAP_R,

};

/// @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBindBuffer.xhtml

enum class GLBufferType {
    array_buffer              = GL_ARRAY_BUFFER,
    atomic_counter_buffer     = GL_ATOMIC_COUNTER_BUFFER,
    copy_read_buffer          = GL_COPY_READ_BUFFER,
    copy_write_buffer         = GL_COPY_WRITE_BUFFER,
    dispatch_indirect_buffer  = GL_DISPATCH_INDIRECT_BUFFER,
    draw_indirect_buffer      = GL_DRAW_INDIRECT_BUFFER,
    element_array_buffer      = GL_ELEMENT_ARRAY_BUFFER,
    pixel_pack_buffer         = GL_PIXEL_PACK_BUFFER,
    pixel_unpack_buffer       = GL_PIXEL_UNPACK_BUFFER,
    query_buffer              = GL_QUERY_BUFFER,
    shader_storage_buffer     = GL_SHADER_STORAGE_BUFFER,
    texture_buffer            = GL_TEXTURE_BUFFER,
    transform_feedback_buffer = GL_TRANSFORM_FEEDBACK_BUFFER,
    uniform_buffer            = GL_UNIFORM_BUFFER,
};

/// @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBufferData.xhtml

enum class GLBufferUsage {
    static_draw  = GL_STATIC_DRAW,
    dynamic_draw = GL_DYNAMIC_DRAW
};
}