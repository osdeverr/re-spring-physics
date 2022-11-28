#pragma once

#include "../../utils/vec3.hpp"
#include "material.hpp"

namespace Graphics {
struct SceneVertex {
    Vec3f m_position;
    Vec3f m_normal;
    Material* m_material;

    SceneVertex(const Vec3f &position, const Vec3f &normal, Material* material) :
            m_position(position), m_normal(normal), m_material(material) {
    }

    // 6 floats + two additional 32-bit indices
    static constexpr int length = 8;
    static constexpr int index_offset = 6;
    static constexpr int material_offset = 7;
};
}