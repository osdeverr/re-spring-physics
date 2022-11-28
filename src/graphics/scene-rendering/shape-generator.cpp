//
// Created by Артем on 03.02.2022.
//

#include "shape-generator.hpp"
#include "shapes/icosahedron.hpp"

void
Graphics::ShapeGenerator::add_triangle(const Vec3f &p1, const Vec3f &p2, const Vec3f &p3, Graphics::Material* material) {
    Vec3f a = p3 - p1;
    Vec3f b = p2 - p1;
    Vec3f normal = a.cross(b);
    normal.normalize();
    add_vertex(p1, normal, material);
    add_vertex(p2, normal, material);
    add_vertex(p3, normal, material);
}

void Graphics::ShapeGenerator::add_square(const Vec3f &p1, const Vec3f &p2, const Vec3f &p3, const Vec3f &p4,
                                          Graphics::Material* material) {
    add_triangle(p1, p2, p4, material);
    add_triangle(p1, p4, p3, material);
}

void Graphics::ShapeGenerator::add_cube(const Vec3f &center, const Vec3f &size, Graphics::Material* material) {
    Material* materials[] = {material, material, material, material, material, material};
    add_cube(center, size, materials);
}

void Graphics::ShapeGenerator::add_cube(const Vec3f &center, const Vec3f &size, Graphics::Material** materials) {

    Vec3f masks[3] = {
            {1, 0, 0},
            {0, 1, 0},
            {0, 0, 1}
    };
    for(int i = 0; i < 3; i++) {

        Vec3f basis_u = masks[(i + 1) % 3] * size;
        Vec3f basis_v = masks[(i + 2) % 3] * size;

        Vec3f offset = center + masks[i] * size * 0.5 - (basis_u + basis_v) * 0.5;

        add_square(offset,
                   offset + basis_u,
                   offset + basis_v,
                   offset + basis_u + basis_v, materials[i]);

        offset -= masks[i] * size;

        add_square(offset,
                   offset + basis_u,
                   offset + basis_v,
                   offset + basis_u + basis_v, materials[i + 3]);
    }
}

void Graphics::ShapeGenerator::add_vertex(const Vec3f &position, const Vec3f &normal, Graphics::Material* material) {
    m_mesh.emplace_back(position, normal, material);
}

void Graphics::ShapeGenerator::reset() {
    m_mesh.clear();
}

void Graphics::ShapeGenerator::add_sphere(const Vec3f &center, float radius, Graphics::Material* material, int lod) {
    std::vector<Vec3f> sphere {};

    for(auto& face : IcosahedronShape::faces) {
        Vec3f vertices[3] = {
            IcosahedronShape::vertices[face[0]],
            IcosahedronShape::vertices[face[1]],
            IcosahedronShape::vertices[face[2]],
        };

        for(auto& vertex : vertices) {
            vertex /= IcosahedronShape::icosahedron_radius;
            sphere.push_back(vertex);
        }
    }

    for(int i = 0; i < lod; i++) {
        increase_sphere_details(sphere);
    }

    for(auto& vertex : sphere) {
        add_vertex(vertex * radius += center, vertex, material);
    }
}
