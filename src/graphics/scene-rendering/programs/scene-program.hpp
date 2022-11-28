#pragma once

namespace Graphics {
class SceneProgram;
class SceneRenderer;
}

#include "../../gl/vertex-fragment-program.hpp"
#include "../../gl/uniform.hpp"

namespace Graphics {

class SceneProgram : public VertexFragmentProgram {
protected:
    GLBuffer<float>* m_vertex_buffer = nullptr;
    SceneRenderer* m_renderer = nullptr;
    Uniform m_matrix_buffer_uniform;

public:

    SceneProgram(const std::string& vertex_shader_name, const std::string& fragment_shader_name):
        VertexFragmentProgram(vertex_shader_name, fragment_shader_name),
        m_matrix_buffer_uniform(this, "u_matrix_buffer") {}

    void bind_to_vertex_buffer(GLBuffer<float>* vertex_buffer);
    void bind_matrix_buffer(GLenum texture);

    void set_renderer(SceneRenderer* renderer) { m_renderer = renderer; }
    SceneRenderer* get_renderer() { return m_renderer; }

    void draw_vertices();
};

}