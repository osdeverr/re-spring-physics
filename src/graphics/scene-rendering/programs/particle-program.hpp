
namespace Graphics {
    class SceneRenderer;
}

#include "../../gl/vertex-fragment-program.hpp"
#include "../../gl/uniform.hpp"
#include "scene-program.hpp"

namespace Graphics {

    class ParticleProgram : public VertexFragmentProgram {

        Graphics::Uniform m_camera_matrix_uniform;
        GLBuffer<float>* m_vertex_buffer = nullptr;
        Camera* m_camera = nullptr;

    public:

        explicit ParticleProgram();

        void bind_to_vertex_buffer(GLBuffer<float> *vertex_buffer);

        void draw_vertices(int n);

        void set_camera(Camera* camera) { m_camera = camera; }
    };
}