#pragma once

#include "camera-controller.hpp"
#include "../../graphics/camera.hpp"

class FreeCameraController : public CameraController {
public:
    explicit FreeCameraController(Camera *camera) : m_camera(camera) {};

    void tick(float delta_time) override;

    void on_key_press(sf::Keyboard::Key key) override;

    void on_key_release(sf::Keyboard::Key key) override;

    void on_mouse_move(Vec2f delta) override;

private:
    Camera *m_camera;
    float m_speed = 0.1;
    float m_rotation_speed = 0.1;

    Vec3f m_camera_speed_vector = {0, 0, 0};

    float m_pitch = 0;
    float m_camera_speed = 10.0f;
    float yaw = 0;
};