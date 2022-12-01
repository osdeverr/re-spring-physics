
#include "surface.hpp"

void PhysicsSurface::force_tick() const {
    // Simulate the aerodynamics of the surface.
    // The surface is a triangle, so the aerodynamics is calculated by the triangle's normal.

    Vec3f origin = m_vertex_a->m_position;
    Vec3f point_b = m_vertex_b->m_position - origin;
    Vec3f point_c = m_vertex_c->m_position - origin;
    Vec3f normal = point_b.cross(point_c).normal();

    // The normal is the direction of the force.
    // The force is proportional to the surface area.
    // The surface area is proportional to the triangle's area.
    // The triangle's area is half of the cross product of the triangle's sides.

    float area = point_b.cross(point_c).len() / 2;

    // The force is proportional to the projected velocity.
    Vec3f velocity = (m_vertex_a->m_velocity + m_vertex_b->m_velocity + m_vertex_c->m_velocity) / 3;
    Vec3f projection = normal * velocity.dot(normal);

    // The force is applied to the center of the triangle.
    Vec3f normal_force = -projection * area;
    Vec3f air_friction = -(velocity - projection) * area * m_air_friction;

    Vec3f force = (normal_force + air_friction) * (m_air_density / 3);

    // Apply the force to the vertices.

    m_vertex_a->m_force += force;
    m_vertex_b->m_force += force;
    m_vertex_c->m_force += force;
}

PhysicsSurface::PhysicsSurface(PhysicsVertex *vertex_a, PhysicsVertex *vertex_b, PhysicsVertex *vertex_c) {
    m_vertex_a = vertex_a;
    m_vertex_b = vertex_b;
    m_vertex_c = vertex_c;
}
