#pragma once

#include "vertex.hpp"

struct PhysicsSurface {
    PhysicsVertex* m_vertex_a = nullptr;
    PhysicsVertex* m_vertex_b = nullptr;
    PhysicsVertex* m_vertex_c = nullptr;

    float m_air_friction = 0.01;
    float m_air_density = 0.3;

    PhysicsSurface(PhysicsVertex* vertex_a, PhysicsVertex* vertex_b, PhysicsVertex* vertex_c);

    void force_tick() const;
};