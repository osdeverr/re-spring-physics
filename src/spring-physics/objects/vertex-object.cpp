//
// Created by Артем on 04.02.2022.
//

#include "vertex-object.hpp"
#include "../world.hpp"
#include "../../graphics/scene-rendering/shape-generator.hpp"

VertexObject::VertexObject(World* world) : WorldObject(world) {
    m_physics_vertex = std::make_unique<PhysicsVertex>();
    m_world->add_object(this);
}

VertexObject::~VertexObject() {
    if(m_geometry_object) {
        auto geometry_pool = m_world->get_renderer()->get_geometry_pool();
        geometry_pool->destroy_object(m_geometry_object);
        geometry_pool->destroy_material(m_material);
    }
    m_world->remove_object(this);
}

void VertexObject::create_colored_mesh(const Vec3f& color) {
    delete m_geometry_object;

    auto geometry_pool = m_world->get_renderer()->get_geometry_pool();

    if(!m_material) m_material = geometry_pool->create_material();
    m_material->set_color(color);

    Graphics::ShapeGenerator generator;
    generator.add_sphere({}, 0.1, m_material, 1);

    m_geometry_object = geometry_pool->create_object({ generator.get_mesh() }, nullptr);
}

void VertexObject::tick(float dt) {
    if(!m_geometry_object) return;

    Vec3f& position = m_physics_vertex->m_position;
    m_geometry_object->set_transform(Matrix4f::translation_matrix(position.x, position.y, position.z));
}
