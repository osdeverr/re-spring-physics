
#include "jet-object.hpp"
#include "../spring-physics-app.hpp"

JetObject::JetObject(World *world, PhysicsVertex *head_vertex, PhysicsVertex *tail_vertex) : WorldObject(world) {
    m_world = world;
    m_physics_jet = std::make_unique<PhysicsJet>(head_vertex, tail_vertex);
    m_world->add_object(this);
}

JetObject::~JetObject() {
    if(m_geometry_object) {
        auto geometry_pool = m_world->get_renderer()->get_geometry_pool();
        geometry_pool->destroy_object(m_geometry_object);
        geometry_pool->destroy_material(m_material);
    }

    m_world->remove_object(this);
}

void JetObject::create_colored_mesh(const Vec3f &color) {
    // Just a cylinder for now

    delete m_geometry_object;

    auto geometry_pool = m_world->get_renderer()->get_geometry_pool();
    if (!m_material) m_material = geometry_pool->create_material();
    m_material->set_color(color);

    Graphics::ShapeGenerator shape_generator;

    int segments = 10;
    float radius = 0.1f;

    Vec3f start = {0.1, 0, 0};
    Vec3f end = {0.9, 0, 0};
    Vec3f basis1 = {0, 1, 0};
    Vec3f basis2 = {0, 0, 1};

    for (int i = 0; i < segments; i++) {
        float angle = (float) i / (float) segments * 2.0f * M_PI;
        float next_angle = (float) (i + 1) / (float) segments * 2.0f * M_PI;

        Vec3f offset1 = (basis1 * cos(angle) + basis2 * sin(angle)) * radius;
        Vec3f offset2 = (basis1 * cos(next_angle) + basis2 * sin(next_angle)) * radius;

        shape_generator.add_triangle(start, start + offset1, start + offset2, m_material);
        shape_generator.add_triangle(start + offset1, start + offset2, end + offset1, m_material);
        shape_generator.add_triangle(start + offset2, end + offset1, end + offset2, m_material);
        shape_generator.add_triangle(end, end + offset1, end + offset2, m_material);
    }

    m_geometry_object = geometry_pool->create_object({shape_generator.get_mesh()}, nullptr);
}

void JetObject::tick(float dt) {
    // Calculate the transformation matrix

    Vec3f basis1 = (m_physics_jet->m_head_vertex->m_position - m_physics_jet->m_tail_vertex->m_position);
    Vec3f basis2 = get_orthogonal_vector(basis1);
    Vec3f basis3 = basis1.cross(basis2).normal();
    Vec3f tail = m_physics_jet->m_tail_vertex->m_position;

    Matrix4f transform({
                               basis1.x, basis1.y, basis1.z, 0,
                               basis2.x, basis2.y, basis2.z, 0,
                               basis3.x, basis3.y, basis3.z, 0,
                               tail.x, tail.y, tail.z, 1,
                       });

    float force_fraction = m_physics_jet->m_force / m_max_force;
    m_particles_accumulator += dt * force_fraction * m_particle_intensity;

    Vec3f flame_speed = -basis1.normal() * 15;

    Vec3f velocity = m_physics_jet->m_tail_vertex->m_velocity;

    while (m_particles_accumulator > 0.0f) {

        float particle_time = m_particles_accumulator / force_fraction / m_particle_intensity;

        Vec3f current_particle_speed = flame_speed + velocity;
        current_particle_speed += basis1 * (rand() / (float) RAND_MAX - 0.5f) * 0.5f;
        current_particle_speed += basis2 * (rand() / (float) RAND_MAX - 0.5f) * 1.0f;
        current_particle_speed += basis3 * (rand() / (float) RAND_MAX - 0.5f) * 1.0f;

        Vec3f particle_position = tail - current_particle_speed * particle_time;

        m_world->get_particle_host()->add_particle({
                   .m_position = particle_position,
                   .m_velocity = current_particle_speed,
                   .m_color = Vec4f(1, 1, 0.5, 1),
                   .m_lifetime = 0.1f,
           });

        m_particles_accumulator -= 1.0f;
    }

    m_geometry_object->set_transform(transform);
}
