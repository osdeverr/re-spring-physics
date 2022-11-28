//
// Created by Артем on 30.01.2022.
//

#include "spring-physics-app.hpp"
#include "camera/follow-creature-camera.hpp"
#include "camera/free-camera-controller.hpp"

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

    m_camera_controller = std::make_unique<FollowCreatureCamera>(m_camera.get(), m_creature.get());
    m_user_controller.set_camera_controller(m_camera_controller.get());

    m_runway_texture = m_world->get_renderer()->get_texture_manager()->load_texture("resources/textures/runway.png");

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

    // Update muscle inputs
    m_creature->get_springs()[m_left_aileron_spring]->get_physics_spring()->m_target_length =
            m_left_aileron_muscle_length * (1.0 + m_real_airplane_controls.x * 0.1);
    m_creature->get_springs()[m_right_aileron_spring]->get_physics_spring()->m_target_length =
            m_right_aileron_muscle_length * (1.0 - m_real_airplane_controls.x * 0.1);

    m_creature->get_springs()[m_left_elevator_spring]->get_physics_spring()->m_target_length =
            m_left_elevator_muscle_length * (1.0 + m_real_airplane_controls.y * 0.3);

    m_creature->get_springs()[m_right_elevator_spring]->get_physics_spring()->m_target_length =
            m_right_elevator_muscle_length * (1.0 + m_real_airplane_controls.y * 0.3);

    for (auto &jet: m_creature->get_jet_objects()) {
        jet->get_physics_jet()->m_force = jet->get_max_force() * m_throttle;
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
            if (m_free_camera) {
                m_camera_controller = std::make_unique<FreeCameraController>(m_camera.get());
            } else {
                m_camera_controller = std::make_unique<FollowCreatureCamera>(m_camera.get(), m_creature.get());
            }
            m_user_controller.set_camera_controller(m_camera_controller.get());
        default:
            break;
    }
}

void SpringPhysicsApp::build_model() {
    CreatureConfig config;

    std::vector<Vec3f> vertices = {};
    std::vector<std::pair<int, int>> edges = {};

    // Build a cube with 8 vertices and 12 edges

    vertices.emplace_back((6.3 + -1.0), 0.0f, -2.0f); // front, bottom, left 0
    vertices.emplace_back((8.0 + 1.0), -1.5f, -2.0f); // back, bottom, left 1
    vertices.emplace_back((2.0 + -1.0), 2.0f, -1.0f); // front, top, left 2
    vertices.emplace_back((7.0 + 1.0), 2.0f, -1.0f); // back, top, left 3
    vertices.emplace_back((6.3 + -1.0), 0.0f, 2.0f); // front, bottom, right 4
    vertices.emplace_back((8.0 + 1.0), -1.5f, 2.0f); // back, bottom, right 5
    vertices.emplace_back((2.0 + -1.0), 2.0f, 1.0f); // front, top, right 6
    vertices.emplace_back((7.0 + 1.0), 2.0f, 1.0f); // back, top, right 7

    for (int i = 0; i < 8; i++) {
        for (int j = i + 1; j < 8; j++) {
            edges.emplace_back(i, j);
        }
    }

    // A plane oriented towards x-negative direction

    // Add wings to the cube

    vertices.emplace_back(11.0f, 2.0f, -10.0f); // 8
    vertices.emplace_back(11.0f, 2.0f, 10.0f); // 9

    // Connect wings to the cube

    edges.emplace_back(0, 8);
    edges.emplace_back(1, 8);
    edges.emplace_back(2, 8);
    edges.emplace_back(3, 8);
    edges.emplace_back(4, 9);
    edges.emplace_back(5, 9);
    edges.emplace_back(6, 9);
    edges.emplace_back(7, 9);

    // Add a tail to the cube

    vertices.emplace_back(23.0f, 2.0f, 0.0f); // 10

    // Add some anchors behind the cube, on the y=2, x=11 line
    // Connect them to the nearest cube vertices and wingtips

    vertices.emplace_back(11.0f, 2.0f, -3.0f); // 11
    vertices.emplace_back(11.0f, 2.0f, 3.0f); // 12

    edges.emplace_back(1, 11);
    edges.emplace_back(5, 11);
    edges.emplace_back(3, 11);
    edges.emplace_back(7, 11);
    edges.emplace_back(8, 11);

    edges.emplace_back(1, 12);
    edges.emplace_back(5, 12);
    edges.emplace_back(3, 12);
    edges.emplace_back(7, 12);
    edges.emplace_back(9, 12);

    edges.emplace_back(11, 12);

    // Add the secondary tail mount point, connect it to those anchors and the bottom near two edges of the cube

    vertices.emplace_back(15.0f, 2.0f, 0.0f); // 13

    edges.emplace_back(1, 13);
    edges.emplace_back(5, 13);
    edges.emplace_back(11, 13);
    edges.emplace_back(12, 13);

    // Connect tail to the secondary tail mount point

    edges.emplace_back(10, 13);

    // Add four more anchors below the tail anchors to form a prolonged triangle, connect them all to each other

    vertices.emplace_back(23.0f, 0.0f, -2.0f); // 14
    vertices.emplace_back(23.0f, 0.0f, 2.0f); // 15
    vertices.emplace_back(15.0f, 0.0f, -2.0f); // 16
    vertices.emplace_back(15.0f, 0.0f, 2.0f); // 17

    edges.emplace_back(10, 14);
    edges.emplace_back(10, 15);
    edges.emplace_back(10, 16);
    edges.emplace_back(10, 17);

    edges.emplace_back(13, 14);
    edges.emplace_back(13, 15);
    edges.emplace_back(13, 16);
    edges.emplace_back(13, 17);

    edges.emplace_back(14, 15);
    edges.emplace_back(14, 16);
    edges.emplace_back(14, 17);

    edges.emplace_back(15, 16);
    edges.emplace_back(15, 17);

    edges.emplace_back(16, 17);

    // Connect the closest bottom of the cube to the bottom anchors

    edges.emplace_back(1, 16);
    edges.emplace_back(1, 17);
    edges.emplace_back(5, 16);
    edges.emplace_back(5, 17);

    // Connect bottom anchors to additional wing anchors (nodes 11 and 12)

    edges.emplace_back(11, 16);
    edges.emplace_back(11, 17);
    edges.emplace_back(12, 16);
    edges.emplace_back(12, 17);

    // Add vertical stabilizer

    vertices.emplace_back(23.0f, 6.0f, 0); // 18

    // Connect vertical stabilizer node to the tail anchors

    edges.emplace_back(10, 18);
    edges.emplace_back(13, 18);
    edges.emplace_back(14, 18);
    edges.emplace_back(15, 18);

    // Add horizontal stabilizers

    vertices.emplace_back(23.0f, 2.0f, -6.0f); // 19
    vertices.emplace_back(23.0f, 2.0f, 6.0f); // 20

    // Connect horizontal stabilizers to the tail anchors

    edges.emplace_back(10, 19);
    edges.emplace_back(10, 20);
    edges.emplace_back(13, 19);
    edges.emplace_back(13, 20);
    edges.emplace_back(14, 19);
    edges.emplace_back(14, 20);
    edges.emplace_back(15, 19);
    edges.emplace_back(15, 20);

    // Add ailerons. They should be connected to wingtips and anchors behind the cube

    vertices.emplace_back(14.0f, 2.0f, -5.0f); // 21
    vertices.emplace_back(14.0f, 2.0f, 5.0f); // 22

    edges.emplace_back(8, 21);
    edges.emplace_back(9, 22);
    edges.emplace_back(11, 21);
    edges.emplace_back(12, 22);

    // Aileron support. Located just under the aileron nodes

    vertices.emplace_back(11.0f, 0.0f, -2.0f); // 23
    vertices.emplace_back(11.0f, 0.0f, 2.0f); // 24

    edges.emplace_back(21, 23);
    edges.emplace_back(8, 23);
    edges.emplace_back(11, 23);
    edges.emplace_back(22, 24);
    edges.emplace_back(9, 24);
    edges.emplace_back(12, 24);

    // Add aileron muscles. They are connected to the aileron support nodes and the lower tail anchors

    m_left_aileron_spring = edges.size();
    edges.emplace_back(23, 16);

    m_right_aileron_spring = edges.size();
    edges.emplace_back(24, 17);

    // Add elevators

    vertices.emplace_back(26.0f, 2.0f, -3.0f); // 25
    vertices.emplace_back(26.0f, 2.0f, 3.0f); // 26

    edges.emplace_back(10, 25);
    edges.emplace_back(10, 26);

    // Elevator support. Located just under the elevator nodes

    vertices.emplace_back(26.0f, 0.0f, -2.0f); // 27
    vertices.emplace_back(26.0f, 0.0f, 2.0f); // 28

    edges.emplace_back(25, 27);
    edges.emplace_back(26, 28);
    edges.emplace_back(19, 27);
    edges.emplace_back(20, 28);
    edges.emplace_back(25, 19);
    edges.emplace_back(26, 20);
    edges.emplace_back(10, 27);
    edges.emplace_back(10, 28);

    // Add elevator muscles. They are connected to the elevator support nodes and the lower tail anchors

    m_left_elevator_spring = edges.size();
    edges.emplace_back(27, 14);

    m_right_elevator_spring = edges.size();
    edges.emplace_back(28, 15);

    // Add the wing and tail surfaces

    // Wing (node 8 for left wing, node 9 for right wing)
    config.m_surfaces.push_back(SurfaceConfig{8, 2, 3});
    config.m_surfaces.push_back(SurfaceConfig{9, 6, 7});

    // Secondary wing surfaces (With use of those anchors behind the cube, indexed 11 and 12)
    config.m_surfaces.push_back(SurfaceConfig{8, 11, 3});
    config.m_surfaces.push_back(SurfaceConfig{9, 12, 7});

    // Tail (node 10 for tail, node 13 for secondary tail mount point,
    // node 18 for vertical stabilizer, nodes 19 and 20 for horizontal stabilizers)

    config.m_surfaces.push_back(SurfaceConfig{10, 13, 18});
    config.m_surfaces.push_back(SurfaceConfig{10, 19, 13});
    config.m_surfaces.push_back(SurfaceConfig{20, 10, 13});

    // Aileron surfaces (node 22 for left aileron, node 24 for right aileron)

    config.m_surfaces.push_back(SurfaceConfig{8, 21, 11});
    config.m_surfaces.push_back(SurfaceConfig{9, 22, 12});

    // Elevator surfaces (nodes 27 and 29 for left elevator, nodes 28 and 30 for right elevator)

    config.m_surfaces.push_back(SurfaceConfig{19, 10, 25});
    config.m_surfaces.push_back(SurfaceConfig{20, 10, 26});


    // Build the creature

    for (int i = 0; i < vertices.size(); i++) {
        config.m_vertices.emplace_back(VertexConfig{0.005, 0.5, vertices[i]});
    }

    for (int i = 0; i < edges.size(); i++) {
        config.m_springs.emplace_back(SpringConfig{1000, 3, edges[i].first, edges[i].second});
    }

    // Add the wheels (build them from nodes and springs)
    // There are two wheels laying on the axle between nodes 1 and 5

    int wheel_nodes = 16;
    float wheel_radius = 1.0f;

    Vec3f node1 = vertices[1];
    Vec3f node5 = vertices[5];

    for (int i = 0; i < wheel_nodes; i++) {
        float angle = 2.0f * M_PI * i / wheel_nodes;
        float x = wheel_radius * sin(angle);
        float y = wheel_radius * cos(angle);

        int node_index = config.m_vertices.size();
        config.m_vertices.emplace_back(VertexConfig{0.005, 1, Vec3f{x, y, 0} + node1});
        config.m_vertices.emplace_back(VertexConfig{0.005, 1, Vec3f{x, y, 0} + node5});

        config.m_springs.emplace_back(SpringConfig{50, 3, 1, node_index});
        config.m_springs.emplace_back(SpringConfig{50, 3, 5, node_index});

        config.m_springs.emplace_back(SpringConfig{50, 3, 1, node_index + 1});
        config.m_springs.emplace_back(SpringConfig{50, 3, 5, node_index + 1});

        if (i > 0) {
            config.m_springs.emplace_back(SpringConfig{50, 3, node_index, node_index - 2});
            config.m_springs.emplace_back(SpringConfig{50, 3, node_index + 1, node_index - 1});
        } else {
            config.m_springs.emplace_back(SpringConfig{50, 3, node_index, node_index + 2 * (wheel_nodes - 1)});
            config.m_springs.emplace_back(SpringConfig{50, 3, node_index + 1, node_index + 1 + 2 * (wheel_nodes - 1)});
        }
    }

    // Add a low-friction node in the tail that will be used to slide the tail on the ground

    int tail_node = config.m_vertices.size();
    config.m_vertices.emplace_back(VertexConfig{0.05, 0.2, Vec3f{23.0f, -1.5f, 0.0f}});

    // Fix the tail node in place
    config.m_springs.emplace_back(SpringConfig{1000, 3, 13, tail_node});
    config.m_springs.emplace_back(SpringConfig{1000, 3, 14, tail_node});
    config.m_springs.emplace_back(SpringConfig{1000, 3, 15, tail_node});

    // Add jet engines

    float top_fraction = 0.4;
    float bottom_fraction = 1 - top_fraction;

    config.m_jets.emplace_back(JetConfig{0, 16, m_engine_power / 2 * bottom_fraction});
    config.m_jets.emplace_back(JetConfig{2, 3, m_engine_power / 2 * top_fraction});
    config.m_jets.emplace_back(JetConfig{4, 17, m_engine_power / 2 * bottom_fraction});
    config.m_jets.emplace_back(JetConfig{6, 7, m_engine_power / 2 * top_fraction});

    // Scale the model

    for (int i = 0; i < config.m_vertices.size(); i++) {
        config.m_vertices[i].m_position = config.m_vertices[i].m_position * 0.3 + Vec3f{0.0f, 1.0f, 0.0f};
    }

    // Calculate weight of nodes based on count of springs connected to them

    float m_frame_weight = 0.0000066f; // Per one strength unit per one meter

    for (int i = 0; i < config.m_springs.size(); i++) {
        auto &spring = config.m_springs[i];
        float frame_length = (config.m_vertices[spring.m_vertex_a_index].m_position -
                              config.m_vertices[spring.m_vertex_b_index].m_position).len();
        float frame_weight = frame_length * m_frame_weight * spring.m_strength;
        config.m_vertices[spring.m_vertex_a_index].m_mass += frame_weight / 2;
        config.m_vertices[spring.m_vertex_b_index].m_mass += frame_weight / 2;
    }

    m_creature = std::make_unique<Creature>(m_world.get(), config);

    // Print the center of mass

    std::cout << "Center of mass: x = " << m_creature->get_center()[0] << ", y = " << m_creature->get_center()[1]
              << ", z = " << m_creature->get_center()[2] << std::endl;

    m_left_aileron_muscle_length = (
            m_creature->get_vertices()[23]->get_physics_vertex()->m_position -
            m_creature->get_vertices()[16]->get_physics_vertex()->m_position).len();
    m_right_aileron_muscle_length = (
            m_creature->get_vertices()[24]->get_physics_vertex()->m_position -
            m_creature->get_vertices()[17]->get_physics_vertex()->m_position).len();

    m_left_elevator_muscle_length = (
            m_creature->get_vertices()[27]->get_physics_vertex()->m_position -
            m_creature->get_vertices()[14]->get_physics_vertex()->m_position).len();

    m_right_elevator_muscle_length = (
            m_creature->get_vertices()[28]->get_physics_vertex()->m_position -
            m_creature->get_vertices()[15]->get_physics_vertex()->m_position).len();


    m_creature->make_visible();
}
