#pragma once

#include "world-object.hpp"
#include "../physics/vertex.hpp"
#include "../../graphics/scene-rendering/geometry-object.hpp"
#include "../../graphics/scene-rendering/shape-generator.hpp"
#include "../physics/surface.hpp"

class CreatureTriangleObject : public WorldObject {

    static constexpr float AIR_DENSITY = 1.0;
    static constexpr float AIR_FRICTION_COEFF = 0.1;

    std::unique_ptr<PhysicsSurface> m_physics_triangle = nullptr;
    Graphics::GeometryObject *m_geometry_object = nullptr;
    Graphics::Material *m_material = nullptr;

public:
    explicit CreatureTriangleObject(World *world, PhysicsVertex *vertex_a, PhysicsVertex *vertex_b,
                                    PhysicsVertex *vertex_c);

    ~CreatureTriangleObject();

    Graphics::GeometryObject *get_geometry_object() { return m_geometry_object; }

    void tick(float dt) override;

    PhysicsSurface *get_physics_surface() { return m_physics_triangle.get(); }

    void create_colored_mesh(const Vec3f &color);
};