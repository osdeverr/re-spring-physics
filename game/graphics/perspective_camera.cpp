//
// Created by Артем on 31.01.2022.
//

#include "perspective_camera.hpp"

void PerspectiveCamera::update() const {

    Matrix4f view_matrix = Matrix4f({
        m_camera_x.x, m_camera_y.x, m_camera_z.x, 0,
        m_camera_x.y, m_camera_y.y, m_camera_z.y, 0,
        m_camera_x.z, m_camera_y.z, m_camera_z.z, 0,
        -m_camera_x.dot(m_camera_position), -m_camera_y.dot(m_camera_position), -m_camera_z.dot(m_camera_position), 1
    });

    m_camera_matrix = view_matrix * Matrix4f::projection_matrix(m_fov, m_aspect_ratio, m_near, m_far);

    m_needs_update = false;
}