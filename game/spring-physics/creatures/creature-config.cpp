//
// Created by Артем on 04.02.2022.
//

#include "creature-config.hpp"
#include "../objects/muscle.hpp"

void VertexConfig::apply(VertexObject* vertex) const {
    auto physical_body = vertex->get_physics_vertex();
    physical_body->m_mass = m_mass;
    physical_body->m_floor_friction = m_floor_friction;
    physical_body->m_position = m_position;
}

void SpringConfig::apply(SpringObject* spring) const {
    auto physical_body = spring->get_physics_spring();
    physical_body->m_strength = m_strength;
    physical_body->m_damping = m_damping;
}

void MuscleConfig::apply(Muscle* muscle, SpringObject* spring) const {
    muscle->m_min_length = m_min_length_coefficient * spring->get_physics_spring()->m_target_length;
    muscle->m_max_length = m_max_length_coefficient * spring->get_physics_spring()->m_target_length;
    muscle->m_spring = spring;
}
