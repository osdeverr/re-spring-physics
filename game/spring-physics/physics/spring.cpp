//
// Created by Артем on 04.02.2022.
//

#include "spring.hpp"

void PhysicsSpring::force_tick() const {

    Vec3f delta = get_delta();

    float spring_length = delta.len();

    delta /= spring_length;

    float relative_speed = get_extension_speed();
    float force = (m_target_length / spring_length - 1) * m_strength - relative_speed * m_damping;

    delta *= force;

    m_vertex_a->m_force -= delta;
    m_vertex_b->m_force += delta;
}

float PhysicsSpring::get_length() const {
    return get_delta().len();
}

float PhysicsSpring::get_extension_speed() const {

    Vec3f& a_velocity = m_vertex_a->m_velocity;
    Vec3f& b_velocity = m_vertex_b->m_velocity;

    Vec3f relative_velocity = b_velocity - a_velocity;

    Vec3f distance_vector = get_delta();

    float spring_length = distance_vector.len();
    Vec3f distance_normalized = distance_vector / spring_length;

    return relative_velocity.dot(distance_normalized);
}

Vec3f PhysicsSpring::get_delta() const {
    return m_vertex_b->m_position - m_vertex_a->m_position;
}
