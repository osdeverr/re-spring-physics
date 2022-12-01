
#include "jet.hpp"

PhysicsJet::PhysicsJet(PhysicsVertex *head_vertex, PhysicsVertex *tail_vertex) {
    m_head_vertex = head_vertex;
    m_tail_vertex = tail_vertex;
}

void PhysicsJet::set_force(float force) {
    m_force = force;
}

float PhysicsJet::get_force() const {
    return m_force;
}

void PhysicsJet::force_tick(float dt) const {
    Vec3f direction = m_head_vertex->m_position - m_tail_vertex->m_position;
    direction.normalize();
    m_head_vertex->m_force += direction * m_force;
}
