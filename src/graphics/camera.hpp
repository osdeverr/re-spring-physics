#pragma once

#include "../utils/vec3.hpp"

class Camera {
protected:

    Vec3f m_camera_position = {0, 0, 0};
    Vec3f m_camera_z = {0, 0, -1};
    Vec3f m_camera_y = {0, 1, 0};
    Vec3f m_camera_x = {1, 0, 0};
    float m_near = 0.1;
    float m_far = 1000;

    mutable bool m_needs_update = true;
    mutable Matrix4f m_camera_matrix {};

public:

    virtual void update() const = 0;

    const Matrix4f& get_matrix() const {
        update_if_needed();
        return m_camera_matrix;
    }

    void update_if_needed() const {
        if(m_needs_update) update();
    }

    Vec3f& get_position() {
        m_needs_update = true;
        return m_camera_position;
    }

    const Vec3f& get_position() const {
        return m_camera_position;
    }

    void set_position(const Vec3f& position) {
        m_camera_position = position;
        m_needs_update = true;
    }

    Vec3f get_direction() const { return -m_camera_z; }
    Vec3f get_top() const { return m_camera_y; }
    Vec3f get_right() const { return m_camera_x; }

    void set_direction(Vec3f camera_direction, Vec3f camera_top) {

        m_camera_z = -camera_direction;
        m_camera_y = camera_top;

        m_camera_z.normalize();
        m_camera_y.normalize();

        m_camera_x = m_camera_z.cross(camera_top);

        m_needs_update = true;
    }

    void set_pitch_yaw(float pitch, float yaw) {

        Matrix4f view_matrix = Matrix4f::rotation_x_matrix(pitch) * Matrix4f::rotation_y_matrix(yaw);

        m_camera_x = {view_matrix.m_data[0], view_matrix.m_data[1], view_matrix.m_data[2]  };
        m_camera_y = {view_matrix.m_data[4], view_matrix.m_data[5], view_matrix.m_data[6]  };
        m_camera_z = {-view_matrix.m_data[8], -view_matrix.m_data[9], -view_matrix.m_data[10] };

        m_needs_update = true;
    }

    float get_near() { return m_near; }

    void set_near(float near) {
        m_near = near;
        m_needs_update = true;
    }

    float get_far() { return m_far; }

    void set_far(float far) {
        m_far = far;
        m_needs_update = true;
    }
};