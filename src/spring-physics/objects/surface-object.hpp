#pragma once

#include "world-object.hpp"
#include "../physics/vertex.hpp"
#include "../../graphics/scene-rendering/geometry-object.hpp"
#include "../../graphics/scene-rendering/shape-generator.hpp"
#include "../physics/surface.hpp"

class SurfaceObject : public WorldObject {

    static constexpr float AIR_DENSITY = 1.0;
    static constexpr float AIR_FRICTION_COEFF = 0.1;

    std::unique_ptr<PhysicsSurface> m_physics_surface = nullptr;
    Graphics::GeometryObject *m_geometry_object = nullptr;
    Graphics::Material *m_material = nullptr;

public:
    explicit SurfaceObject(World *world, PhysicsVertex *vertex_a, PhysicsVertex *vertex_b, PhysicsVertex *vertex_c);

    ~SurfaceObject();

    Graphics::GeometryObject *get_geometry_object() { return m_geometry_object; }

    void tick(float dt) override {
        if (!m_geometry_object) return;

        // The triangle points are:
        // {0, 0, 0}, {0, 1, 0}, {0, 0, 1}

        // The true triangle points are:
        // m_vertex_a->m_position, m_vertex_b->m_position, m_vertex_c->m_position

        // It's necessary to calculate the transformation matrix
        // to transform the triangle points to the true triangle points.

        // First, we need to subtract the first point from the other two points.
        // This will make the first point equal to the origin.

        Vec3f origin = m_physics_surface->m_vertex_a->m_position;
        Vec3f point_b = m_physics_surface->m_vertex_b->m_position - origin;
        Vec3f point_c = m_physics_surface->m_vertex_c->m_position - origin;

        // The matrix should transform {0, 1, 0} to point_b and {0, 0, 1} to point_c.
        // Thus, the matrix should be:

        Matrix4f transform({
                                   1, 0, 0, 0,
                                   point_b.x, point_b.y, point_b.z, 0,
                                   point_c.x, point_c.y, point_c.z, 0,
                                   origin.x, origin.y, origin.z, 1,
                           });

        m_geometry_object->set_transform(transform);
    }

    PhysicsSurface* get_physics_surface() { return m_physics_surface.get(); }

    void create_colored_mesh(const Vec3f &color);
};