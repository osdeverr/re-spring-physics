#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/Event.hpp>

class GeneralApp {

protected:

    std::unique_ptr<sf::RenderWindow> m_window {};
    void process_event(const sf::Event &event);

public:

    GeneralApp() {}

    virtual void on_key_press(sf::Keyboard::Key key) {}
    virtual void on_key_release(sf::Keyboard::Key key) {}
    virtual void on_tick() {}
    virtual void on_draw() {}
    virtual void on_mouse_move(double x, double y) {}
    virtual void on_mouse_press(double x, double y) {}

    sf::RenderWindow* get_window() { return m_window.get(); }

    virtual void loop();;
};