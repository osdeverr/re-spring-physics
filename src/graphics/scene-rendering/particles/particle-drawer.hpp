#pragma once

#include <vector>
#include "../../gl/gl-buffer.hpp"
#include "../programs/particle-program.hpp"

namespace Graphics {

    struct GPUParticle {
        float x, y, z;
        float r, g, b, a;
    };

    struct Particle {
        Vec3f m_position;
        Vec3f m_velocity;
        Vec4f m_color;
        float m_lifetime;
    };

    class ParticleDrawer {
        ParticleProgram m_program;
        Graphics::GLBuffer<float> m_vertex_buffer;
        SceneRenderer* m_scene_renderer = nullptr;

    public:
        explicit ParticleDrawer();

        void upload_particles(const std::vector<Particle>& particles);

        void draw();

        void set_renderer(SceneRenderer *renderer);
    };

}