#pragma once

#include <SFML/Window/Keyboard.hpp>
#include "../utils/vec3.hpp"
#include "../utils/vec2.hpp"
#include "../general-app.hpp"
#include "../graphics/camera.hpp"
#include "camera/camera-controller.hpp"

class UserController {
    GeneralApp* app;
    bool m_dummy_mouse_event_received = false;

    CameraController* m_camera_controller = nullptr;

    bool m_capture_mouse = true;

    void move_mouse_to_center();

public:

    UserController(GeneralApp* app);

    void on_mouse_move(double x, double y);

    void on_key_press(sf::Keyboard::Key key);

    void on_key_release(sf::Keyboard::Key key);

    void on_tick(float dt);

    void set_capture_mouse(bool capture);

    void set_camera_controller(CameraController* camera_controller);
};