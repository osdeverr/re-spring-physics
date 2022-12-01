#pragma once

#include "vec3.hpp"

float dist_point_segment(const Vec3f& p, const Vec3f& a, const Vec3f& b);
float capsule_first_intersect(Vec3f ray_origin, Vec3f ray_direction, Vec3f cap_a, Vec3f cap_b, float cap_radius);
Vec3f capsule_normal(Vec3f position, Vec3f cap_a, Vec3f cap_b);
Vec3f capsule_project(Vec3f point, Vec3f cap_a, Vec3f cap_b, float cap_radius);