//
// Created by Артем on 02.02.2022.
//

#include "scene-program.hpp"
#include "../scene-renderer.hpp"
#include "../../gl/vertex-array-object-factory.hpp"

void Graphics::SceneProgram::bind_to_vertex_buffer(Graphics::GLBuffer<float>* vertex_buffer) {
    m_vertex_buffer = vertex_buffer;

    set_vao({
        {m_vertex_buffer, {
            {"a_position", 3},
            {"a_normal", 3},
            {"a_transform", 1, AttributeType::integer},
            {"a_material", 1, AttributeType::integer},
        }}
    });
}

void Graphics::SceneProgram::draw_vertices() {
    bind_matrix_buffer(GL_TEXTURE0);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
    GLException::check();

    auto* geometry_pool = m_renderer->get_geometry_pool();
    glMultiDrawArrays(GL_TRIANGLES, geometry_pool->get_start_indices(), geometry_pool->get_size_array(), geometry_pool->get_object_count());

    GLException::check();

    glDisable(GL_DEPTH_TEST);
    GLException::check();
}

void Graphics::SceneProgram::bind_matrix_buffer(GLenum texture) {
    m_renderer->get_geometry_pool()->get_matrix_buffer()->bind_texture(texture);
    m_matrix_buffer_uniform.set1i((int)(texture - GL_TEXTURE0));
}
