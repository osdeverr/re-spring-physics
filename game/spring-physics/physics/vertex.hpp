#pragma once

#include "../../utils/vec3.hpp"

struct PhysicsVertex {
    float m_mass = 1;
    float m_floor_friction = 1;
    Vec3f m_position;
    Vec3f m_velocity;
    Vec3f m_force;
    Vec3f m_step_force;
};