//
// Created by Артем on 01.02.2022.
//

#include "shadow-projection-program.hpp"
#include "../scene-renderer.hpp"

Graphics::ShadowProjectionProgram::ShadowProjectionProgram() :
        SceneProgram("shadow-projection/vertex", "shadow-projection/fragment"),
        m_light_camera_matrix_uniform(this, "u_light_camera_matrix"){

}

void Graphics::ShadowProjectionProgram::draw() {
    use();
    bind_vao();

    m_light_camera_matrix_uniform.set_camera_matrix(*m_renderer->get_current_light_camera());

    draw_vertices();
    unbind_vao();
}
