#pragma once

#include "camera-controller.hpp"
#include "../../graphics/camera.hpp"
#include "../creatures/creature.hpp"

class FollowCreatureCamera : public CameraController {
public:
    explicit FollowCreatureCamera(Camera *camera, Creature *creature) : m_camera(camera), m_creature(creature) {};

    void tick(float delta_time) override;

    void on_key_press(sf::Keyboard::Key key) override;

    void on_key_release(sf::Keyboard::Key key) override;

    void on_mouse_move(Vec2f delta) override;

private:
    Camera *m_camera;
    Creature *m_creature;
    float m_speed = 4.0;
    float m_rotation_speed = 2.0;

    float m_view_angle = 0.0f;
    Vec3f m_offset = Vec3f(0.0f, 1.0f, 0.0f);
    float m_camera_distance = 10.0f;
    float m_mouse_sensitive = 0.5f;

    bool old_speed_low = false;
    float old_camera_heading = 0.0f;
    Vec3f m_old_creature_position = Vec3f(0.0f, 0.0f, 0.0f);

    Vec3f m_camera_speed = {0, 0, 0};
};