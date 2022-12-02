
#include "follow-creature-camera.hpp"

void FollowCreatureCamera::tick(float delta_time) {
    Vec3f creature_position = m_creature->get_camera_binding_point();

    Vec3f delta = creature_position - m_old_creature_position;
    m_old_creature_position = creature_position;

    delta.set(1, 0);

    float camera_angle = old_camera_heading;

    if(delta.len() > 0.1) {
        float creature_heading = 0;
        creature_heading = -atan2(delta.x, delta.z);
        float desired_camera_angle = creature_heading + M_PI / 2;
        if(old_speed_low) {
            m_view_angle += old_camera_heading - desired_camera_angle;
        }
        camera_angle = desired_camera_angle;
        old_speed_low = false;
    } else {
        old_speed_low = true;
    }

    old_camera_heading = camera_angle;

    camera_angle += m_view_angle;

    Vec3f m_angle_vector = Vec3f(sin(camera_angle), 0.0f, -cos(camera_angle));

    Vec3f camera_position = creature_position + m_angle_vector * m_camera_distance + m_offset;

    m_camera->set_position(camera_position);

    float pitch = atan2(m_offset.y, m_camera_distance);

    m_camera->set_pitch_yaw(pitch, camera_angle);

    m_view_angle += m_camera_speed.x * delta_time * m_rotation_speed;
    m_camera_distance += m_camera_speed.z * delta_time * m_speed;
    m_offset.y += m_camera_speed.y * delta_time * m_speed;
}

void FollowCreatureCamera::on_key_press(sf::Keyboard::Key key) {
    switch (key) {
        case sf::Keyboard::W:
            m_camera_speed.z = -1.0f;
            break;
        case sf::Keyboard::S:
            m_camera_speed.z = 1.0f;
            break;
        case sf::Keyboard::A:
            m_camera_speed.x = -1.0f;
            break;
        case sf::Keyboard::D:
            m_camera_speed.x = 1.0f;
            break;
        case sf::Keyboard::Space:
            m_camera_speed.y = 1.0f;
            break;
        case sf::Keyboard::LShift:
            m_camera_speed.y = -1.0f;
            break;
        default:
            break;
    }
}

void FollowCreatureCamera::on_key_release(sf::Keyboard::Key key) {
    switch (key) {
        case sf::Keyboard::W:
        case sf::Keyboard::S:
            m_camera_speed.z = 0.0f;
            break;
        case sf::Keyboard::A:
        case sf::Keyboard::D:
            m_camera_speed.x = 0.0f;
            break;
        case sf::Keyboard::Space:
        case sf::Keyboard::LShift:
            m_camera_speed.y = 0.0f;
            break;
        default:
            break;
    }
}

void FollowCreatureCamera::on_mouse_move(Vec2f delta) {
    delta *= m_mouse_sensitive;
    m_view_angle -= delta.x * m_rotation_speed;
    m_offset.y += delta.y * m_speed;
}
