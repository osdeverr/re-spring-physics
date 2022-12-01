#pragma once

#include "vertex.hpp"

struct PhysicsJet {
    PhysicsVertex* m_head_vertex = nullptr;
    PhysicsVertex* m_tail_vertex = nullptr;
    float m_force = 0.0f;
public:
    PhysicsJet(PhysicsVertex* head_vertex, PhysicsVertex* tail_vertex);

    void set_force(float force);

    float get_force() const;

    void force_tick(float dt) const;
};