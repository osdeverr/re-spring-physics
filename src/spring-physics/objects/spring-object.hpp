#pragma once

#include "world-object.hpp"
#include "../physics/spring.hpp"
#include "../../graphics/scene-rendering/geometry-object.hpp"
#include "../../graphics/scene-rendering/material.hpp"

class SpringObject : public WorldObject {
    std::unique_ptr<PhysicsSpring> m_physics_spring {};
    Graphics::GeometryObject* m_geometry_object = nullptr;
    Graphics::Material* m_material = nullptr;

public:
    explicit SpringObject(World* world);
    ~SpringObject() override;

    PhysicsSpring* get_physics_spring() { return m_physics_spring.get(); }
    Graphics::GeometryObject* get_geometry_object() { return m_geometry_object; }

    void create_colored_mesh(const Vec3f& color);

    void tick(float dt) override;
};