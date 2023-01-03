
#include "surface-triangle-object.hpp"
#include "../world.hpp"

SurfaceTriangleObject::SurfaceTriangleObject(World *world, Vec3f vertex_a, Vec3f vertex_b, Vec3f vertex_c) : WorldObject(world) {
    m_physics_triangle = m_world->get_physics_engine()->get_terrain()->add_triangle(vertex_a, vertex_b, vertex_c);
    m_world->add_object(this);

    create_colored_mesh({0.9, 0.4, 0.4});
    update_position(vertex_a, vertex_b, vertex_c);
}

SurfaceTriangleObject::~SurfaceTriangleObject() {
    if (m_geometry_object) {
        auto geometry_pool = m_world->get_renderer()->get_geometry_pool();
        geometry_pool->destroy_object(m_geometry_object);
        geometry_pool->destroy_material(m_material);
    }

    m_world->get_physics_engine()->get_terrain()->destroy_triangle(m_physics_triangle);
    m_physics_triangle = nullptr;
    m_world->remove_object(this);
}

void SurfaceTriangleObject::create_colored_mesh(const Vec3f &color) {
    delete m_geometry_object;

    auto geometry_pool = m_world->get_renderer()->get_geometry_pool();

    if (!m_material) m_material = geometry_pool->create_material();
    m_material->set_color(color);

    Graphics::ShapeGenerator generator;
    generator.add_triangle({0, 0, 0}, {0, 1, 0}, {0, 0, 1}, m_material);

    m_geometry_object = geometry_pool->create_object({generator.get_mesh()}, nullptr);
}

void SurfaceTriangleObject::tick(float dt) {
    // Do nothing, the object is static
}

void SurfaceTriangleObject::update_position(Vec3f vertex_a, Vec3f vertex_b, Vec3f vertex_c) {
    if (!m_geometry_object) return;

    m_physics_triangle->set_points(vertex_a, vertex_b, vertex_c);

    Vec3f basis_b = vertex_b - vertex_a;
    Vec3f basis_c = vertex_c - vertex_a;
    Vec3f basis_a = basis_b.cross(basis_c).normalize();

    Matrix4f transform({
                               basis_a.x, basis_a.y, basis_a.z, 0,
                               basis_b.x, basis_b.y, basis_b.z, 0,
                               basis_c.x, basis_c.y, basis_c.z, 0,
                               vertex_a.x, vertex_a.y, vertex_a.z, 1,
                       });

    m_geometry_object->set_transform(transform);
}
