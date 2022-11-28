//
// Created by Артем on 30.01.2022.
//

#include "spring-physics-app.hpp"
#include "camera/follow-creature-camera.hpp"
#include "camera/free-camera-controller.hpp"
#include "models/airplane-creature.hpp"

SpringPhysicsApp::SpringPhysicsApp() : GeneralApp() {
    create_window(2200, 1600);
    m_world = std::make_unique<World>();
    m_camera = std::make_unique<PerspectiveCamera>();
    m_camera->set_position({0, 3, -3});

    m_world->get_renderer()->set_camera(m_camera.get());
    auto size = m_window->getSize();
    m_world->set_screen_size({(int) size.x, (int) size.y});

    m_camera->set_aspect((float) size.x / (float) size.y);

    build_model();

    m_world->get_renderer()->add_light({{0.3, -0.8, 0.5},
                                        {0.6, 0.6,  0.6}});

    m_camera_controller = std::make_unique<FollowCreatureCamera>(m_camera.get(), m_creature->get_creature());
    m_user_controller.set_camera_controller(m_camera_controller.get());

//    m_runway_texture = m_world->get_renderer()->get_texture_manager()->load_texture("resources/textures/runway.png");
//    m_runway_texture->make_resident(true);

//    I'm going to cry.
//    Freaking OpenGL doesn't support bindless textures on macos.
//    I'm about to kick the OpenGL from this project and replace it with Vulkan.
//    Also, it's time to kick out SFML and replace it with GLFW.
//    But first it is worth to publish the OpenGL version, so it won't be lost completely.
//    Let's do some huge refactoring, baby!
}

void SpringPhysicsApp::create_window(int width, int height) {

    sf::ContextSettings settings;
    settings.majorVersion = 4;
    settings.minorVersion = 1;
    settings.depthBits = 24;
    settings.attributeFlags |= sf::ContextSettings::Core;

    m_window = std::make_unique<sf::RenderWindow>(
            sf::VideoMode(width, height, 28),
            "3D Stuff",
            sf::Style::Titlebar | sf::Style::Close,
            settings
    );

    m_window->setFramerateLimit(60);
    m_window->setVerticalSyncEnabled(true);

    glewInit();

    GLException::enable_debug();
    GLException::check();
}

void SpringPhysicsApp::on_tick() {
    reset_gl();
    clear_window();
    on_draw();

    m_window->display();
}

void SpringPhysicsApp::on_draw() {

    float dt = 1.0f / 60.0f;

    m_user_controller.on_tick(dt);

    m_real_airplane_controls = m_input_airplane_controls * 0.1 + m_real_airplane_controls * 0.9;

    m_creature->set_controls(m_real_airplane_controls);
    m_creature->set_throttle(m_throttle);

    m_world->get_physics_engine()->tick();

    m_world->tick(dt);

    m_world->draw();
}

void SpringPhysicsApp::reset_gl() {
    auto size = m_window->getSize();
    glViewport(0, 0, (GLsizei) size.x, (GLsizei) size.y);
}

void SpringPhysicsApp::clear_window() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void SpringPhysicsApp::on_mouse_move(double x, double y) {
    m_user_controller.on_mouse_move(x, y);
}

void SpringPhysicsApp::on_key_press(sf::Keyboard::Key key) {
    GeneralApp::on_key_press(key);
    m_user_controller.on_key_press(key);

    switch (key) {
        case sf::Keyboard::T: {
            m_throttle = 1.0f;
            break;
        }
        case sf::Keyboard::Up:
            m_input_airplane_controls.y = -1;
            break;
        case sf::Keyboard::Down:
            m_input_airplane_controls.y = 1;
            break;
        case sf::Keyboard::Left:
            m_input_airplane_controls.x = -1;
            break;
        case sf::Keyboard::Right:
            m_input_airplane_controls.x = 1;
            break;
        default:
            break;
    }

}

void SpringPhysicsApp::on_key_release(sf::Keyboard::Key key) {
    GeneralApp::on_key_release(key);
    m_user_controller.on_key_release(key);

    switch (key) {
        case sf::Keyboard::T: {
            m_throttle = 0.0f;
            break;
        }
        case sf::Keyboard::Up:
        case sf::Keyboard::Down:
            m_input_airplane_controls.y = 0;
            break;
        case sf::Keyboard::Left:
        case sf::Keyboard::Right:
            m_input_airplane_controls.x = 0;
            break;
        case sf::Keyboard::C:
            m_free_camera = !m_free_camera;
            if (m_free_camera) {
                m_camera_controller = std::make_unique<FreeCameraController>(m_camera.get());
            } else {
                m_camera_controller = std::make_unique<FollowCreatureCamera>(m_camera.get(), m_creature->get_creature());
            }
            m_user_controller.set_camera_controller(m_camera_controller.get());
        default:
            break;
    }
}

void SpringPhysicsApp::build_model() {
    m_creature = std::make_unique<AirplaneCreature>(m_world.get());
}
