#pragma once

#include "../../gl/gl-buffer.hpp"
#include "../../gl/uniform.hpp"
#include "../../gl/vertex-fragment-program.hpp"
#include "scene-program.hpp"


namespace Graphics {

class ShadowMappingProgram : public SceneProgram {

    Graphics::Uniform m_camera_matrix_uniform;
    Graphics::Uniform m_light_camera_matrix_uniform;
    Graphics::Uniform m_light_map_uniform;
    Graphics::Uniform m_light_color_uniform;
    Graphics::Uniform m_light_direction_uniform;
    Graphics::Uniform m_previous_framebuffer_uniform;
    Graphics::Uniform m_should_add_previous_framebuffer;
    Graphics::Uniform m_material_buffer_uniform;
    Graphics::Uniform m_camera_position_uniform;

public:

    ShadowMappingProgram();
    void draw();
};

}