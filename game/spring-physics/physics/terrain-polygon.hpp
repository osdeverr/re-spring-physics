#pragma once

#include "../../utils/vec3.hpp"

class PhysicsThread;

class TerrainPolygon {
    Vec3f m_points[3];

    Vec3f m_normal;

    // The triangle is not actually flat in the physics world, because otherwise it would be impossible to
    // calculate the collision properly.
    float m_thickness = 0.1f;
public:
    TerrainPolygon(Vec3f point_a, Vec3f point_b, Vec3f point_c);

    Vec3f get_point_a() const { return m_points[0]; }
    Vec3f get_point_b() const { return m_points[1]; }
    Vec3f get_point_c() const { return m_points[2]; }

    Vec3f get_normal() const { return m_normal; }

    float get_thickness() const { return m_thickness; }
    void set_thickness(float thickness) { m_thickness = thickness; }

    void set_points(Vec3f point_a, Vec3f point_b, Vec3f point_c);;

    bool check_hit(const Vec3f &point, const Vec3f &direction, float* distance = nullptr) const;

    bool collides(Vec3f ray_origin, Vec3f ray_direction, float distance, float& collision_distance, Vec3f& normal) const;
};