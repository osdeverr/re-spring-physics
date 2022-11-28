#pragma once

#include "camera.hpp"
#include "../utils/vec2.hpp"

class PerspectiveCamera : public Camera {
    float m_fov = 60.0f / 180.0f * M_PI;
    float m_aspect_ratio = 1.0;

public:
    PerspectiveCamera() : Camera() {}

    void update() const override;

    float get_fov() { return m_fov; }

    void set_fov(float fov) {
        m_fov = fov;
        m_needs_update = true;
    }

    float get_aspect() { return m_aspect_ratio; }

    void set_aspect(float aspect) {
        m_aspect_ratio = aspect;
        m_needs_update = true;
    }
};