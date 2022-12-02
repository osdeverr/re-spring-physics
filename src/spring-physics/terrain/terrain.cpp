
#include "terrain.hpp"

TerrainPolygon *Terrain::add_triangle(Vec3f vertex_a, Vec3f vertex_b, Vec3f vertex_c) {
    auto triangle = std::make_unique<TerrainPolygon>(vertex_a, vertex_b, vertex_c);
    auto triangle_ptr = triangle.get();
    m_surface_mesh.emplace_back(std::move(triangle));
    bvh_valid = false;
    return triangle_ptr;
}

void Terrain::destroy_triangle(TerrainPolygon *triangle) {
    auto it = std::find_if(m_surface_mesh.begin(), m_surface_mesh.end(), [triangle](const auto& t) {
        return t.get() == triangle;
    });
    if (it != m_surface_mesh.end()) {
        m_surface_mesh.erase(it);
        bvh_valid = false;
    }
}

std::vector<std::unique_ptr<TerrainPolygon>> &Terrain::get_surface_mesh() {
    return m_surface_mesh;
}

void Terrain::ensure_valid() {
    if (!bvh_valid) {
        std::vector<TerrainPolygon*> triangles;
        for (auto& triangle : m_surface_mesh) {
            triangles.push_back(triangle.get());
        }
        m_bvh.build(triangles);
        bvh_valid = true;
    }
}

void Terrain::query(const Vec3f &from, const Vec3f &to, std::vector<TerrainPolygon *> &result) {
    assert(bvh_valid);
    m_bvh.query(m_bvh.m_root, m_bvh.get_min(from, to), m_bvh.get_max(from, to), result);
}
