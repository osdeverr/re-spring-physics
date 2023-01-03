#pragma once

#include <SFML/Window/Keyboard.hpp>
#include "../../utils/vec2.hpp"

class CameraController {
public:
    virtual void tick(float delta_time) = 0;
    virtual ~CameraController() = default;

    virtual void on_key_press(sf::Keyboard::Key key) = 0;
    virtual void on_key_release(sf::Keyboard::Key key) = 0;
    virtual void on_mouse_move(Vec2f delta) = 0;
};