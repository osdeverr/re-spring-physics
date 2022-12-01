//
// Created by Артем on 31.01.2022.
//

#include "user-controller.hpp"

void UserController::move_mouse_to_center() {
    auto size = app->get_window()->getSize();
    sf::Mouse::setPosition((sf::Vector2i)size / 2, *app->get_window());
}

UserController::UserController(GeneralApp *app) : app(app) {

}

void UserController::on_mouse_move(double x, double y) {
    if(!m_capture_mouse) return;

    auto size = app->get_window()->getSize();
    Vec2f center = {(float)size.x / 2, (float)size.y / 2};

    Vec2f position = {(float)x, (float)y};
    Vec2f delta = position - center;

    if(delta.len_squared() > 0) {
        m_camera_controller->on_mouse_move(delta / Vec2f{ (float)size.x, (float)size.y });
        move_mouse_to_center();
        app->get_window()->setMouseCursorVisible(false);
    } else {
        m_dummy_mouse_event_received = true;
    }
}

void UserController::on_key_press(sf::Keyboard::Key key) {
    if(key == sf::Keyboard::Escape) {
        m_capture_mouse = !m_capture_mouse;
        app->get_window()->setMouseCursorVisible(!m_capture_mouse);
        if(m_capture_mouse) {
            move_mouse_to_center();
        }
    }
    m_camera_controller->on_key_press(key);
}

void UserController::on_key_release(sf::Keyboard::Key key) {
    m_camera_controller->on_key_release(key);
}

void UserController::on_tick(float dt) {

    if(m_capture_mouse && app->get_window()->hasFocus() && !m_dummy_mouse_event_received) {
        move_mouse_to_center();
        m_dummy_mouse_event_received = false;
    }

    m_camera_controller->tick(dt);
}

void UserController::set_capture_mouse(bool capture) {
    m_capture_mouse = capture;
}

void UserController::set_camera_controller(CameraController *camera_controller) {
    m_camera_controller = camera_controller;
}
