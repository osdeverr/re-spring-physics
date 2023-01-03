//
// Created by Артем on 03.10.2021.
//

//#include "accumulator_program.hpp"

#include "scene-rendering-program.hpp"
#include "../../gl/vertex-array-object-factory.hpp"
#include "../scene-renderer.hpp"

namespace Graphics {
SceneRenderingProgram::SceneRenderingProgram():
        SceneProgram("basic/vertex", "basic/fragment"),
        m_camera_matrix_uniform(this, "u_camera_matrix"),
        m_ambient_light_uniform(this, "u_ambient_light"),
        m_shadow_map_uniform(this, "u_shadow_map"),
        m_material_buffer_uniform(this, "u_material_buffer") {
}

void SceneRenderingProgram::draw() {
    use();
    bind_vao();

    m_renderer->get_geometry_pool()->get_material_buffer()->bind_texture(GL_TEXTURE1);
    m_material_buffer_uniform.set1i(1);

    glActiveTexture(GL_TEXTURE2);
    m_renderer->get_previous_shadow_framebuffer()->m_texture.bind();

    m_shadow_map_uniform.set1i(2);
    m_ambient_light_uniform.set3f(m_renderer->get_ambient_light());
    m_camera_matrix_uniform.set_camera_matrix(*m_renderer->get_camera());

    draw_vertices();

    unbind_vao();
}

}