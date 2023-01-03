

#include "free-camera-controller.hpp"

void FreeCameraController::tick(float delta_time) {
    Vec3f camera_space_speed = m_camera_speed_vector * m_camera_speed * delta_time;
    Vec3f camera_speed = {0, 0, 0};

    Vec3f camera_front_direction = m_camera->get_direction();
    camera_front_direction.set(1, 0);
    camera_front_direction.normalize();

    Vec3f camera_right_direction = Vec3f {camera_front_direction.z, 0, -camera_front_direction.x};

    camera_speed += camera_front_direction * camera_space_speed.z;
    camera_speed += camera_right_direction * camera_space_speed.x;
    camera_speed += Vec3f {0, 1, 0} * camera_space_speed.y;

    m_camera->set_position(camera_speed + m_camera->get_position());
}

void FreeCameraController::on_key_press(sf::Keyboard::Key key) {
    switch(key) {
        case sf::Keyboard::W:
            m_camera_speed_vector.z = 1;
            break;
        case sf::Keyboard::A:
            m_camera_speed_vector.x = -1;
            break;
        case sf::Keyboard::S:
            m_camera_speed_vector.z = -1;
            break;
        case sf::Keyboard::D:
            m_camera_speed_vector.x = 1;
            break;
        case sf::Keyboard::Space:
            m_camera_speed_vector.y = 1;
            break;
        case sf::Keyboard::LShift:
            m_camera_speed_vector.y = -1;
            break;
        case sf::Keyboard::LControl:
            m_camera_speed = 50;
            break;
        default:
            break;
    }
}

void FreeCameraController::on_key_release(sf::Keyboard::Key key) {
    switch(key) {
        case sf::Keyboard::W:
        case sf::Keyboard::S:
            m_camera_speed_vector.z = 0;
            break;
        case sf::Keyboard::A:
        case sf::Keyboard::D:
            m_camera_speed_vector.x = 0;
            break;
        case sf::Keyboard::Space:
        case sf::Keyboard::LShift:
            m_camera_speed_vector.y = 0;
            break;
        case sf::Keyboard::LControl:
            m_camera_speed = 10;
            break;
        default:
            break;
    }
}

void FreeCameraController::on_mouse_move(Vec2f delta) {
    m_pitch += delta.y;
    yaw -= delta.x;
    m_camera->set_pitch_yaw(m_pitch, yaw);
}

