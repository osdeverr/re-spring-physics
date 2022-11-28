#pragma once

#include "../../utils/vec3.hpp"

namespace Graphics {

struct SceneDirectionalLight {
    Vec3f m_direction;
    Vec3f m_color;

    SceneDirectionalLight(const Vec3f &direction, const Vec3f &color) :
            m_direction(direction),
            m_color(color) {
        this->m_direction.normalize();
    }
};

}