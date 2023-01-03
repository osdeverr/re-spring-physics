//
// Created by Артем on 01.02.2022.
//

#include "scene-renderer.hpp"

Graphics::SceneRenderer::SceneRenderer() {
    m_shadow_map_framebuffer.initialize();
    for(auto& framebuffer : m_shadow_buffers) framebuffer.initialize();

    m_shadow_map_framebuffer.set_size(m_shadow_map_resolution);

    m_shadow_projection_program.set_renderer(this);
    m_shadow_mapping_program.set_renderer(this);
    m_final_program.set_renderer(this);

    auto vertex_buffer = m_geometry_pool.get_vertex_buffer();

    m_shadow_projection_program.bind_to_vertex_buffer(vertex_buffer);
    m_shadow_mapping_program.bind_to_vertex_buffer(vertex_buffer);
    m_final_program.bind_to_vertex_buffer(vertex_buffer);

    m_particle_drawer.set_renderer(this);
}

void Graphics::SceneRenderer::draw() {

    m_geometry_pool.defragment_buffer(1);
    m_geometry_pool.update_transforms();
    m_geometry_pool.update_materials();
    m_geometry_pool.synchronize();

    for(m_current_light_index = 0; m_current_light_index < m_directional_lights.size(); m_current_light_index++) {
        auto& m_directional_light = m_directional_lights[m_current_light_index];
        prepare_light(m_directional_light);

        m_shadow_projection_program.draw();
        m_shadow_mapping_program.draw();
    }

    if(m_current_light_index == 0) {
        m_current_light_index = 1;

        get_previous_shadow_framebuffer()->make_drawing_target();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    reset_gl();
    m_final_program.draw();
    m_particle_drawer.draw();
}

void Graphics::SceneRenderer::reset_gl() const {
    GLFramebuffer::unbind();
    glViewport(0, 0, (GLsizei)m_screen_size.x, (GLsizei)m_screen_size.y);
}

void Graphics::SceneRenderer::prepare_light(const Graphics::SceneDirectionalLight &light) {
    auto light_up = get_light_up(light.m_direction);

    Vec3f light_map_position = m_main_camera->get_position() - light.m_direction * m_shadow_camera_distance;

    m_light_camera.set_position(light_map_position);
    m_light_camera.set_lb_corner(-m_shadow_map_extent);
    m_light_camera.set_rt_corner(m_shadow_map_extent);
    m_light_camera.set_direction(light.m_direction, light_up);

    m_shadow_map_framebuffer.make_drawing_target();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

Vec3f Graphics::SceneRenderer::get_light_up(const Vec3f &direction) {
    Vec3f up = {0, 1, 0};
    float projection = up.dot(direction);
    if(abs(projection) > 0.5) {
        up = {0, 0, 1};
        projection = up.dot(direction);
    }
    up -= direction * projection;
    return up;
}

void Graphics::SceneRenderer::set_screen_size(const Vec2i &size) {
    m_screen_size = size;

    for(auto & m_shadow_buffer : m_shadow_buffers) {
        m_shadow_buffer.set_size(size);
    }
}

Vec3f Graphics::SceneRenderer::get_ambient_light() {
    return m_ambient_light;
}
