#pragma once

#include "../../utils/vec3.hpp"
#include "geometry-object-config.hpp"
#include "material.hpp"

namespace Graphics {
class ShapeGenerator {

    std::vector<Graphics::SceneVertex> m_mesh {};

    void increase_sphere_details(std::vector<Vec3f>& sphere) {
        auto sphere_size = sphere.size();
        for(int j = 0; j < sphere_size; j += 3) {
            Vec3f v0 = sphere[j + 0];
            Vec3f v1 = sphere[j + 1];
            Vec3f v2 = sphere[j + 2];

            Vec3f d1 = (v1 - v0) *= 0.5;
            Vec3f d2 = (v2 - v0) *= 0.5;

            v1 = v0 + d1;
            v2 = v0 + d2;
            Vec3f v12n = (v1 + d2).normalize();

            Vec3f v1n = v1.normal();
            Vec3f v2n = v2.normal();

            sphere[j + 1] = v1n;
            sphere[j + 2] = v2n;

            sphere.push_back(v1n);
            sphere.push_back((v1 + d1).normalize());
            sphere.push_back(v12n);

            sphere.push_back(v2n);
            sphere.push_back(v12n);
            sphere.push_back((v2 + d2).normalize());

            sphere.push_back(v1n);
            sphere.push_back(v12n);
            sphere.push_back(v2n);
        }
    }

public:

    std::vector<Graphics::SceneVertex> &get_mesh() { return m_mesh; }

    void reset();;

    void add_triangle(const Vec3f &p1, const Vec3f &p2, const Vec3f &p3, Graphics::Material* material);

    void add_square(const Vec3f &p1, const Vec3f &p2, const Vec3f &p3, const Vec3f &p4, Material* material);

    void add_cube(const Vec3f &center, const Vec3f &size, Material* material);

    void add_cube(const Vec3f &center, const Vec3f &size, Material* materials[6]);

    void add_sphere(const Vec3f &center, float radius, Material* material, int lod = 1);

    void add_vertex(const Vec3f &position, const Vec3f &normal, Material* material);
};
}