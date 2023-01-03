
#include "particle-program.hpp"
#include "../../gl/vertex-array-object-factory.hpp"

Graphics::ParticleProgram::ParticleProgram():
        VertexFragmentProgram("particles/vertex", "particles/fragment"),
        m_camera_matrix_uniform(this, "u_camera_matrix") {

}

void Graphics::ParticleProgram::bind_to_vertex_buffer(Graphics::GLBuffer<float>* vertex_buffer) {
    m_vertex_buffer = vertex_buffer;

    set_vao({
        {m_vertex_buffer, {
            {"a_position", 3},
            {"a_color", 4},
        }}
    });
}

void Graphics::ParticleProgram::draw_vertices(int n) {
    glEnable(GL_BLEND);
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
    GLException::check();

    use();
    bind_vao();

    m_camera_matrix_uniform.set_camera_matrix(*m_camera);

    glDrawArrays(GL_TRIANGLES, 0, n);

    unbind_vao();

    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    GLException::check();
}