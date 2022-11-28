//
// Created by Артем on 03.10.2021.
//


#include "shadow-mapping-program.hpp"
#include "../scene-renderer.hpp"
#include "../../gl/vertex-array-object-factory.hpp"

namespace Graphics {
ShadowMappingProgram::ShadowMappingProgram():
        SceneProgram("shadow-mapping/vertex", "shadow-mapping/fragment"),
        m_camera_matrix_uniform(this, "u_camera_matrix"),
        m_light_camera_matrix_uniform(this, "u_light_camera_matrix"),
        m_light_map_uniform(this, "u_light_map"),
        m_light_color_uniform(this, "u_light_color"),
        m_light_direction_uniform(this, "u_light_direction"),
        m_previous_framebuffer_uniform(this, "u_previous_framebuffer"),
        m_should_add_previous_framebuffer(this, "u_should_add_previous_framebuffer"),
        m_material_buffer_uniform(this, "u_material_buffer"),
        m_camera_position_uniform(this, "u_camera_position") {
}

void ShadowMappingProgram::draw() {
    auto shadow_framebuffer = m_renderer->get_shadow_map_framebuffer();

    use();
    bind_vao();

    m_renderer->get_geometry_pool()->get_material_buffer()->bind_texture(GL_TEXTURE1);
    m_material_buffer_uniform.set1i(1);

    glActiveTexture(GL_TEXTURE2);
    shadow_framebuffer->m_texture.bind();

    Matrix4f bias = Matrix4f({0.5f, 0.0f, 0.0f, 0.0f,
                              0.0f, 0.5f, 0.0f, 0.0f,
                              0.0f, 0.0f, 0.5f, 0.0f,
                              0.5f, 0.5f, 0.5f, 1.0f});

    m_camera_matrix_uniform.set_camera_matrix(*m_renderer->get_camera());
    m_camera_position_uniform.set3f(m_renderer->get_camera()->get_position());
    m_light_camera_matrix_uniform.setm44f(m_renderer->get_current_light_camera()->get_matrix() * bias);
    m_light_map_uniform.set1i(2);
    m_light_color_uniform.set3f(m_renderer->get_current_light().m_color);
    m_light_direction_uniform.set3f(m_renderer->get_current_light().m_direction);

    auto* active_framebuffer = m_renderer->get_active_shadow_framebuffer();
    if(active_framebuffer) active_framebuffer->make_drawing_target();
    else GLFramebuffer::unbind();

    m_previous_framebuffer_uniform.set1i(3);

    auto* previous_framebuffer = m_renderer->get_previous_shadow_framebuffer();
    if(previous_framebuffer) {
        glActiveTexture(GL_TEXTURE3);
        previous_framebuffer->m_texture.bind();
        m_should_add_previous_framebuffer.set1i(1);
    } else {
        m_should_add_previous_framebuffer.set1i(0);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    draw_vertices();

    unbind_vao();
}

}