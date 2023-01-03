#pragma once

namespace Graphics {
class GeometryPool;
}

#include "../../utils/vec3.hpp"

namespace Graphics {

class Material {
    GeometryPool* m_geometry_pool;
    int m_buffer_index = 0;
    bool m_grid = false;
    float m_specular_factor = 10;
    Vec3f m_color = {1, 1, 1};

public:
    Material(GeometryPool* pool): m_geometry_pool(pool) {}

    Vec3f get_color();
    void set_color(const Vec3f& color);

    bool get_grid();
    void set_grid(bool value);

    int get_buffer_index();
    void set_buffer_index(int index);

    float get_specular_factor();
    void set_specular_factor(float factor);
};
}