#pragma once

#include <vector>
#include "world-object.hpp"
#include "../physics/terrain-polygon.hpp"
#include "../../graphics/scene-rendering/geometry-object.hpp"
#include "../../graphics/scene-rendering/material.hpp"

class MapObject : public WorldObject {

    std::vector<TerrainPolygon*> m_physics_triangles {};
    Graphics::GeometryObject *m_geometry_object = nullptr;
    Graphics::Material* m_material = nullptr;

public:
    explicit MapObject(World *world, const std::string &path, const Matrix4f &transform);

    ~MapObject() override;

    Graphics::GeometryObject *get_geometry_object() { return m_geometry_object; }

    void tick(float dt) override;

    void create_mesh(const std::string &path, const Matrix4f& transform);
};