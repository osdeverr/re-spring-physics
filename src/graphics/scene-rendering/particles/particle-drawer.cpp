
#include "particle-drawer.hpp"
#include "../scene-renderer.hpp"

Graphics::ParticleDrawer::ParticleDrawer() :
        m_vertex_buffer(GLBufferType::array_buffer, GLBufferUsage::dynamic_draw) {
    m_vertex_buffer.create_buffer();
    m_program.bind_to_vertex_buffer(&m_vertex_buffer);
}

void Graphics::ParticleDrawer::upload_particles(const std::vector<Particle> &particles) {
    m_vertex_buffer.get_storage().resize(particles.size() * 7 * 6);
    auto *gpu_particles = (GPUParticle *) m_vertex_buffer.get_storage().data();

    float particle_size = 0.1f;
    Camera* camera = m_scene_renderer->get_camera();
    Vec3f camera_right = camera->get_right() * particle_size;
    Vec3f camera_up = camera->get_top() * particle_size;

    for (int i = 0; i < particles.size(); i++) {
        const Particle *particle = particles.data() + i;
        GPUParticle *gpu_particle = gpu_particles + i * 6;

        Vec3f position = particle->m_position;

        for(int j = 0; j < 6; j++) {
            memcpy(&gpu_particle[j].r, &particle->m_color, sizeof(float) * 4);
        }

        position -= camera_right;
        position -= camera_up;

        memcpy(&gpu_particle[0].x, &position, sizeof(float) * 3);
        memcpy(&gpu_particle[3].x, &position, sizeof(float) * 3);

        position += camera_right * 2;

        memcpy(&gpu_particle[1].x, &position, sizeof(float) * 3);

        position += camera_up * 2;

        memcpy(&gpu_particle[2].x, &position, sizeof(float) * 3);
        memcpy(&gpu_particle[5].x, &position, sizeof(float) * 3);

        position -= camera_right * 2;

        memcpy(&gpu_particle[4].x, &position, sizeof(float) * 3);
    }

    m_vertex_buffer.synchronize();
}

void Graphics::ParticleDrawer::draw() {
    m_program.set_camera(m_scene_renderer->get_camera());
    m_program.draw_vertices(m_vertex_buffer.get_storage().size() / 7);
}

void Graphics::ParticleDrawer::set_renderer(Graphics::SceneRenderer *renderer) {
    m_scene_renderer = renderer;
}
