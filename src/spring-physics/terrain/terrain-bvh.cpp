
#include "terrain-bvh.hpp"

Vec3f TerrainBVH::get_min(TerrainPolygon &triangle) {
    Vec3f result = get_min(triangle.get_point_a(), triangle.get_point_b());
    result = get_min(result, triangle.get_point_c());
    result -= Vec3f(triangle.get_thickness(), triangle.get_thickness(), triangle.get_thickness());
    return result;
}

Vec3f TerrainBVH::get_max(TerrainPolygon &triangle) {
    Vec3f result = get_max(triangle.get_point_a(), triangle.get_point_b());
    result = get_max(result, triangle.get_point_c());
    result += Vec3f(triangle.get_thickness(), triangle.get_thickness(), triangle.get_thickness());
    return result;
}

Vec3f TerrainBVH::get_min(std::vector<TerrainPolygon *>::iterator begin, std::vector<TerrainPolygon *>::iterator end) {
    Vec3f result = get_max(**begin);
    for (auto it = begin; it != end; ++it) {
        result = get_min(result, get_min(**it));
    }
    return result;
}

Vec3f TerrainBVH::get_max(std::vector<TerrainPolygon *>::iterator begin, std::vector<TerrainPolygon *>::iterator end) {
    Vec3f result = get_min(**begin);
    for (auto it = begin; it != end; ++it) {
        result = get_max(result, get_max(**it));
    }
    return result;
}

float TerrainBVH::get_min(TerrainPolygon &triangle, int axis) {
    return std::min(std::min(triangle.get_point_a()[axis], triangle.get_point_b()[axis]), triangle.get_point_c()[axis]);
}

float TerrainBVH::get_max(TerrainPolygon &triangle, int axis) {
    return std::max(std::max(triangle.get_point_a()[axis], triangle.get_point_b()[axis]), triangle.get_point_c()[axis]);
}

float TerrainBVH::get_min(std::vector<TerrainPolygon *>::iterator begin, std::vector<TerrainPolygon *>::iterator end, int axis) {
    float result = get_min(**begin, axis);
    for (auto it = begin; it != end; ++it) {
        result = std::min(result, get_min(**it, axis));
    }
    return result;
}

float TerrainBVH::get_max(std::vector<TerrainPolygon *>::iterator begin, std::vector<TerrainPolygon *>::iterator end, int axis) {
    float result = get_max(**begin, axis);
    for (auto it = begin; it != end; ++it) {
        result = std::max(result, get_max(**it, axis));
    }
    return result;
}

void TerrainBVH::build(std::vector<TerrainPolygon *> &surface_mesh) {
    destroy(m_root);
    m_root = new Node();
    build(m_root, surface_mesh, 0);
}

void TerrainBVH::build(TerrainBVH::Node *node, std::vector<TerrainPolygon *> &triangles, int depth) {
    if (triangles.empty()) return;

    Vec3f min = get_min(triangles.begin(), triangles.end());
    Vec3f max = get_max(triangles.begin(), triangles.end());

    node->min = min;
    node->max = max;

    if (triangles.size() == 1) {
        node->triangle = triangles[0];
        return;
    } else {
        node->triangle = nullptr;
    }

    int best_axis = -1;
    float best_volume = std::numeric_limits<float>::infinity();

    for(int i = 0; i < 3; i++) {
        float volume = get_split_volume(triangles, i);
        if (volume < best_volume) {
            best_volume = volume;
            best_axis = i;
        }
    }

    std::vector<TerrainPolygon*> left_triangles;
    std::vector<TerrainPolygon*> right_triangles;

    // Partition

    std::sort(triangles.begin(), triangles.end(), [best_axis](TerrainPolygon* a, TerrainPolygon* b) {
        float a_axis = a->get_point_a()[best_axis] + a->get_point_b()[best_axis] + a->get_point_c()[best_axis];
        float b_axis = b->get_point_a()[best_axis] + b->get_point_b()[best_axis] + b->get_point_c()[best_axis];
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

float TerrainBVH::get_split_volume(std::vector<TerrainPolygon *> &triangles, int axis) {
    std::sort(triangles.begin(), triangles.end(), [axis](TerrainPolygon *a, TerrainPolygon *b) {
        float a_axis = a->get_point_a()[axis] + a->get_point_b()[axis] + a->get_point_c()[axis];
        float b_axis = b->get_point_a()[axis] + b->get_point_b()[axis] + b->get_point_c()[axis];
        return a_axis < b_axis;
    });

    float median = triangles.size() / 2;

    Vec3f left_min = get_min(triangles.begin(), triangles.begin() + median);
    Vec3f left_max = get_max(triangles.begin(), triangles.begin() + median);

    Vec3f right_min = get_min(triangles.begin() + median, triangles.end());
    Vec3f right_max = get_max(triangles.begin() + median, triangles.end());

    Vec3f left_size = left_max - left_min;
    Vec3f right_size = right_max - right_min;

    float left_volume = left_size[0] * left_size[1] * left_size[2];
    float right_volume = right_size[0] * right_size[1] * right_size[2];

    return left_volume + right_volume;
}