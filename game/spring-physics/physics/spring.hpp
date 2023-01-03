#pragma once

#include "vertex.hpp"

struct PhysicsSpring {
    PhysicsVertex* m_vertex_a = nullptr;
    PhysicsVertex* m_vertex_b = nullptr;
    float m_target_length = 1;
    float m_strength = 1;
    float m_damping = 1;

    void force_tick() const;

    Vec3f get_delta() const;
    float get_length() const;
    float get_extension_speed() const;
};