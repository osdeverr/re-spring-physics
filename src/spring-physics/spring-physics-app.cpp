//
// Created by Артем on 30.01.2022.
//

#include "spring-physics-app.hpp"
#include "camera/follow-creature-camera.hpp"
#include "camera/free-camera-controller.hpp"
#include "models/airplane-creature.hpp"
#include "models/car/car-creature.hpp"
#include "models/builder/model-builder.hpp"
#include "models/n-wheeler/n-wheeler-creature.hpp"

SpringPhysicsApp::SpringPhysicsApp() : GeneralApp() {
    create_window(2200, 1600);
    m_world = std::make_unique<World>();
    m_camera = std::make_unique<PerspectiveCamera>();
    m_camera->set_position({0, 3, -3});

    m_world->get_renderer()->set_camera(m_camera.get());
    auto size = m_window->getSize();
    m_world->set_screen_size({(int) size.x, (int) size.y});

    m_camera->set_aspect((float) size.x / (float) size.y);

    m_world->get_renderer()->add_light({{0.3, -0.8, 0.5},
                                        {0.6, 0.6,  0.6}});

//    m_world->get_surface_mesh().push_back(std::make_unique<SurfaceTriangleObject>(m_world.get(), Vec3f {-20, 0, -5}, Vec3f {-20, 0, 5}, Vec3f {-100, 5, -5}));
//    m_world->get_surface_mesh().push_back(std::make_unique<SurfaceTriangleObject>(m_world.get(), Vec3f {-20, 0, 5}, Vec3f {-100, 5, -5}, Vec3f {-100, 5, 5}));

    // Map with car
//    {
//        m_map_object = std::make_unique<MapObject>(m_world.get(), "resources/maps/map.obj", Matrix4f::scale_matrix(0.01, 0.01, 0.01));
        // On the border (for car & physics testing)
//        build_model(Matrix4f::translation_matrix(0, 22, 0.35f) * Matrix4f::rotation_x_matrix(0.1) * Matrix4f::rotation_y_matrix(M_PI / 2));

        // On the road (for airplane)
//        build_model(Matrix4f::translation_matrix(0, 25, -3.0f) * Matrix4f::rotation_x_matrix(0.1) * Matrix4f::rotation_y_matrix(M_PI / 2));
//    }

    // Map2 with car
    {
        m_map_object = std::make_unique<MapObject>(m_world.get(), "resources/maps/map2.obj", Matrix4f::scale_matrix(0.03, 0.03, 0.03) * Matrix4f::translation_matrix(0, 5, 0));
        // On the border (for car & physics testing)
        build_model({});
    }

    // Collision test
//    {
//        m_world->get_surface_mesh().push_back(
//                std::make_unique<SurfaceTriangleObject>(m_world.get(), Vec3f{0, 2.9, -1}, Vec3f{1, 2.9, -1},
//                                                        Vec3f{0, 2.9, 1}));
//        m_world->get_surface_mesh().push_back(
//                std::make_unique<SurfaceTriangleObject>(m_world.get(), Vec3f{1, 2.9, -1}, Vec3f{0, 2.9, 1},
//                                                        Vec3f{1, 2.9, 1}));
//
//        for (auto &triangle: m_world->get_surface_mesh()) {
//            triangle->get_physics_surface()->set_thickness(0.5);
//        }
//
//        ModelBuilder::Builder builder;
//        builder.get_state().m_vertex_weight = 0.3f;
//        builder.get_state().m_spring_damping = 10.0f;
//        builder.get_state().m_spring_strength = 3000.0f;
//        for (int i = 0; i < 10; i++) {
//            float x = (float) i * 0.2 + -0.5;
//            builder.vertex({x, 3.6, 0});
//            if (i > 0) {
//                builder.spring(i, i - 1);
//            }
//        }
//
//        m_world->get_physics_engine()->set_substeps(1);
//        m_world->get_physics_engine()->set_dt(1.0f / 60.0f / 30.0f);
//
//        (new Creature(m_world.get(), builder.get_config()))->make_visible();
//    }

// Collision test 2
//    {
//        ModelBuilder::Builder builder{};
//        builder.get_state().m_vertex_weight = 1.0f;
//        builder.vertex({0, 23, 0});
//        builder.vertex({0, 23, 1});
//        builder.spring(0, 1);
//
//        m_world->get_physics_engine()->set_substeps(1);
//        m_world->get_physics_engine()->set_dt(1.0f / 60.0f / 30.0f);
//
//        (new Creature(m_world.get(), builder.get_config()))->make_visible();
//        m_camera->set_position({0, 23, -3});
//        m_camera->set_direction({0, 0, 1}, {0, 1, 0});
//    }

//    load_obj("resources/maps/map2.obj", m_world.get(), Matrix4f::scale_matrix(0.03, 0.03, 0.03));
//    build_model({});

//    m_runway_texture = m_world->get_renderer()->get_texture_manager()->load_texture("resources/textures/runway.png");
//    m_runway_texture->make_resident(true);

//    I'm going to cry.
//    Freaking OpenGL doesn't support bindless textures on macos.
//    I'm about to kick the OpenGL from this project and replace it with Vulkan.
//    Also, it's time to kick out SFML and replace it with GLFW/GLUT.
//    But first it is worth to publish the OpenGL version, so it won't be lost completely.
//    Let's do some huge refactoring, baby!

    if(m_creature) {
        m_camera_controller = std::make_unique<FollowCreatureCamera>(m_camera.get(), m_creature->get_creature());
    } else {
        m_camera_controller = std::make_unique<FreeCameraController>(m_camera.get());
    }
    m_user_controller.set_camera_controller(m_camera_controller.get());
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

    if(m_creature) {
        m_creature->set_controls(m_real_airplane_controls);
        m_creature->set_throttle(m_throttle);
    }

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
            if (m_free_camera || !m_creature) {
                m_camera_controller = std::make_unique<FreeCameraController>(m_camera.get());
            } else {
                m_camera_controller = std::make_unique<FollowCreatureCamera>(m_camera.get(), m_creature->get_creature());
            }
            m_user_controller.set_camera_controller(m_camera_controller.get());
        default:
            break;
    }
}

void SpringPhysicsApp::build_model(const Matrix4f& transform) {
//    m_creature = std::make_unique<AirplaneCreature>(m_world.get(), transform);
//    m_creature = std::make_unique<CarCreature>(m_world.get(), transform);
    m_creature = std::make_unique<NWheelerCreature>(m_world.get(), transform);
}
