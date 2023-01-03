#pragma once


namespace Graphics {
class SceneRenderingProgram;
struct TexturedFramebuffer;
}

#include "../../gl/uniform.hpp"
#include "../../gl/gl-texture.hpp"
#include "../../gl/gl-framebuffer.hpp"
#include "../../gl/vertex-fragment-program.hpp"
#include "../../../utils/matrix4.hpp"
#include "../../../utils/vec3.hpp"
#include "../../gl/gl-texture-buffer.hpp"
#include "scene-program.hpp"

namespace Graphics {

class SceneRenderingProgram : public SceneProgram {

    Graphics::Uniform m_camera_matrix_uniform;
    Graphics::Uniform m_ambient_light_uniform;
    Graphics::Uniform m_shadow_map_uniform;
    Graphics::Uniform m_material_buffer_uniform;

    Vec3f m_ambient_light {};

public:

    SceneRenderingProgram();

    void draw();

};

}