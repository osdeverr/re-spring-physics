//
// Created by Артем on 27.01.2022.
//

#include "general-app.hpp"

void GeneralApp::process_event(const sf::Event &event) {
    switch(event.type) {
        case sf::Event::Closed: m_window->close(); break;
        case sf::Event::KeyPressed:
            on_key_press(event.key.code);
            break;
        case sf::Event::KeyReleased:
            on_key_release(event.key.code);
            break;
        case sf::Event::MouseButtonPressed:
            on_mouse_press((double)event.mouseButton.x, (double)event.mouseButton.y);
            break;
        case sf::Event::MouseMoved:
            on_mouse_move((double)event.mouseMove.x, (double)event.mouseMove.y);
        default: break;
    }
}

void GeneralApp::loop() {
    sf::Event event = {};

    while(true) {

        while (m_window->pollEvent(event)) {
            process_event(event);
        }

        if(m_window->isOpen()) on_tick();
        else break;
    }
}
