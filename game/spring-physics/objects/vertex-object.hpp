#pragma once

#include "world-object.hpp"
#include "../physics/vertex.hpp"
#include "../../graphics/scene-rendering/geometry-object.hpp"
#include "../../graphics/scene-rendering/shape-generator.hpp"

class VertexObject : public WorldObject {

    std::unique_ptr<PhysicsVertex> m_physics_vertex = nullptr;
    Graphics::GeometryObject* m_geometry_object = nullptr;
    Graphics::Material* m_material = nullptr;

public:
    explicit VertexObject(World* world);
    ~VertexObject();

    PhysicsVertex* get_physics_vertex() { return m_physics_vertex.get(); }
    Graphics::GeometryObject* get_geometry_object() { return m_geometry_object; }

    void tick(float dt) override;

    void create_colored_mesh(const Vec3f& color);
};