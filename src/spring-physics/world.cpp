//
// Created by Артем on 04.02.2022.
//

#include "world.hpp"
#include "../graphics/scene-rendering/shape-generator.hpp"

void World::tick(float dt) {
    for(auto& object : m_objects) {
        object->tick(dt);
    }
    get_renderer()->get_particle_drawer()->upload_particles(m_particle_host->get_particles());
    m_particle_host->tick(dt);
}

void World::add_floor() {
    Graphics::ShapeGenerator generator;

    m_floor_material = m_renderer->get_geometry_pool()->create_material();
    m_floor_material->set_color({0.15, 0.5, 0.2});
    m_floor_material->set_grid(true);

    generator.add_cube({0, -0.05, 0}, {10000, 0.1, 10000}, m_floor_material);
    m_floor = m_renderer->get_geometry_pool()->create_object({generator.get_mesh()}, nullptr);
    generator.reset();
}
