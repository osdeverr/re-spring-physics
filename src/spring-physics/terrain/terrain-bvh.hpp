#pragma once

#include "../../utils/vec3.hpp"
#include <vector>
#include "../objects/surface-triangle-object.hpp"

struct TerrainBVH {
    struct Node {
        Vec3f min;
        Vec3f max;
        TerrainPolygon* triangle;
        Node* left = nullptr;
        Node* right = nullptr;
    };

    Node* m_root = nullptr;

    Vec3f min(TerrainPolygon& triangle);

    Vec3f max(TerrainPolygon& triangle);

    static Vec3f min(Vec3f a, Vec3f b) {
        return Vec3f {std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z)};
    }

    static Vec3f max(Vec3f a, Vec3f b) {
        return Vec3f {std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z)};
    }

    void build(std::vector<TerrainPolygon*>& surface_mesh);

    void build(Node* node, std::vector<TerrainPolygon*>& triangles, int depth);

    void destroy(Node* node);

    void maybe_query_subtree(Node* node, Vec3f min, Vec3f max, std::vector<TerrainPolygon*>& result);

    void query(Node* node, const Vec3f& min, const Vec3f& max, std::vector<TerrainPolygon*>& result);
};