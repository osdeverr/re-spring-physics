#pragma once

#include <vector>
#include "particle-drawer.hpp"

namespace Graphics {

    class ParticleHost {
        std::vector<Particle> m_particles;
        Vec3f m_gravity = Vec3f(0, -9.81f, 0);

    public:
        void tick(float dt) {
            for (int i = 0; i < m_particles.size();) {
                Particle& particle = m_particles[i];
                if (particle.m_lifetime > 0) {
                    particle.m_color.set(3, particle.m_color[3] * (1 - dt / particle.m_lifetime));
                    particle.m_lifetime -= dt;
                    particle.m_position += particle.m_velocity * dt;
                    particle.m_velocity += m_gravity * dt;

                    if (particle.m_lifetime < 0) {
                        std::swap(m_particles[i], m_particles.back());
                        m_particles.pop_back();
                        continue;
                    }
                }
                i++;
            }
        }

        void add_particle(const Particle &particle) {
            if(particle.m_lifetime > 0) {
                m_particles.push_back(particle);
            }
        }

        const std::vector<Particle>& get_particles() {
            return m_particles;
        }
    };

}