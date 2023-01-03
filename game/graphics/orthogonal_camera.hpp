#pragma once

#include "camera.hpp"
#include "../utils/vec2.hpp"

class OrthogonalCamera : public Camera {

    Vec2f m_lb_corner {-1, -1};
    Vec2f m_rt_corner {1, 1};

public:
    OrthogonalCamera(): Camera() {}

    void update() const override;

    Vec2f get_rt_corner() { return m_rt_corner; }
    void set_rt_corner(const Vec2f& rt_corner) { m_rt_corner = rt_corner; }

    Vec2f get_lb_corner() { return m_lb_corner; }
    void set_lb_corner(const Vec2f& lb_corner) { m_lb_corner = lb_corner; }

};