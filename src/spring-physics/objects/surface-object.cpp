//
// Created by Артем on 04.02.2022.
//

#include "surface-object.hpp"
#include "../world.hpp"
#include "../../graphics/scene-rendering/shape-generator.hpp"

SurfaceObject::SurfaceObject(World *world, PhysicsVertex *vertex_a, PhysicsVertex *vertex_b,
                             PhysicsVertex *vertex_c) : WorldObject(world) {
    m_physics_surface = std::make_unique<PhysicsSurface>(vertex_a,
                                                         vertex_b,
                                                         vertex_c);
    m_world->add_object(this);
}

SurfaceObject::~SurfaceObject() {
    if (m_geometry_object) {
        auto geometry_pool = m_world->get_renderer()->get_geometry_pool();
        geometry_pool->destroy_object(m_geometry_object);
        geometry_pool->destroy_material(m_material);
    }
    m_world->remove_object(this);
}

void SurfaceObject::create_colored_mesh(const Vec3f &color) {
    delete m_geometry_object;

    auto geometry_pool = m_world->get_renderer()->get_geometry_pool();

    if (!m_material) m_material = geometry_pool->create_material();
    m_material->set_color(color);

    Graphics::ShapeGenerator generator;
    generator.add_triangle({0, 0, 0}, {0, 1, 0}, {0, 0, 1}, m_material);

    m_geometry_object = geometry_pool->create_object({generator.get_mesh()}, nullptr);
}
