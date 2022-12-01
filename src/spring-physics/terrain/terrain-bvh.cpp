
#include "terrain-bvh.hpp"

Vec3f TerrainBVH::min(TerrainPolygon &triangle) {
    Vec3f result = min(triangle.get_point_a(), triangle.get_point_b());
    result = min(result, triangle.get_point_c());
    result -= Vec3f(triangle.get_thickness(), triangle.get_thickness(), triangle.get_thickness());
    return result;
}

Vec3f TerrainBVH::max(TerrainPolygon &triangle) {
    Vec3f result = max(triangle.get_point_a(), triangle.get_point_b());
    result = max(result, triangle.get_point_c());
    result += Vec3f(triangle.get_thickness(), triangle.get_thickness(), triangle.get_thickness());
    return result;
}

void TerrainBVH::build(std::vector<TerrainPolygon *> &surface_mesh) {
    destroy(m_root);
    m_root = new Node();
    build(m_root, surface_mesh, 0);
}

void TerrainBVH::build(TerrainBVH::Node *node, std::vector<TerrainPolygon *> &triangles, int depth) {
    if (triangles.empty()) return;

    Vec3f min = this->min(*triangles[0]);
    Vec3f max = this->max(*triangles[0]);

    for (auto triangle : triangles) {
        min = this->min(min, this->min(*triangle));
        max = this->max(max, this->max(*triangle));
    }

    node->min = min;
    node->max = max;

    if (triangles.size() == 1) {
        node->triangle = triangles[0];
        return;
    } else {
        node->triangle = nullptr;
    }

    int axis = depth % 3;

    std::vector<TerrainPolygon*> left_triangles;
    std::vector<TerrainPolygon*> right_triangles;

    // Partition

    std::sort(triangles.begin(), triangles.end(), [axis](TerrainPolygon* a, TerrainPolygon* b) {
        float a_axis = a->get_point_a()[axis] + a->get_point_b()[axis] + a->get_point_c()[axis];
        float b_axis = b->get_point_a()[axis] + b->get_point_b()[axis] + b->get_point_c()[axis];
        return a_axis < b_axis;
    });

    for (int i = 0; i < triangles.size(); i++) {
        if (i < triangles.size() / 2) {
            left_triangles.push_back(triangles[i]);
        } else {
            right_triangles.push_back(triangles[i]);
        }
    }

    if (left_triangles.empty()) {
        left_triangles = right_triangles;
        right_triangles.clear();
    }

    if (right_triangles.empty()) {
        right_triangles = left_triangles;
        left_triangles.clear();
    }

    node->left = new Node();
    node->right = new Node();

    build(node->left, left_triangles, depth + 1);
    build(node->right, right_triangles, depth + 1);
}

void TerrainBVH::destroy(TerrainBVH::Node *node) {
    if(node == nullptr) return;

    if (node->left != nullptr) {
        destroy(node->left);
    }

    if (node->right != nullptr) {
        destroy(node->right);
    }

    delete node;
}

void
TerrainBVH::maybe_query_subtree(TerrainBVH::Node *node, Vec3f min, Vec3f max, std::vector<TerrainPolygon *> &result) {
    if (node &&
        node->max.x >= min.x && node->min.x <= max.x &&
        node->max.y >= min.y && node->min.y <= max.y &&
        node->max.z >= min.z && node->min.z <= max.z) {
        query(node, min, max, result);
    }
}

void
TerrainBVH::query(TerrainBVH::Node *node, const Vec3f &min, const Vec3f &max, std::vector<TerrainPolygon *> &result) {
    if(node->triangle) {
        result.push_back(node->triangle);
    }
    maybe_query_subtree(node->left, min, max, result);
    maybe_query_subtree(node->right, min, max, result);
}
