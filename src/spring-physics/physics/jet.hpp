#pragma once

#include "vertex.hpp"

struct PhysicsJet {
    PhysicsVertex* m_head_vertex = nullptr;
    PhysicsVertex* m_tail_vertex = nullptr;
    float m_force = 0.0f;
public:
    PhysicsJet(PhysicsVertex* head_vertex, PhysicsVertex* tail_vertex) {
        m_head_vertex = head_vertex;
        m_tail_vertex = tail_vertex;
    }

    void set_force(float force) {
        m_force = force;
    }

    float get_force() const {
        return m_force;
    }

    void force_tick(float dt) const {
        Vec3f direction = m_head_vertex->m_position - m_tail_vertex->m_position;
        direction.normalize();
        m_head_vertex->m_force += direction * m_force;
    }
};