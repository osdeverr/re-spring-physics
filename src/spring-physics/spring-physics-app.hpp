#pragma once

#include "../general-app.hpp"
#include "../graphics/scene-rendering/programs/scene-rendering-program.hpp"
#include "../utils/vec2.hpp"
#include "user-controller.hpp"
#include "../graphics/perspective_camera.hpp"
#include "world.hpp"
#include "objects/spring-object.hpp"
#include "objects/vertex-object.hpp"
#include "creatures/creature.hpp"
#include "camera/camera-controller.hpp"
#include "../graphics/scene-rendering/particles/particle-host.hpp"
#include <random>

class SpringPhysicsApp : public GeneralApp {

    UserController m_user_controller { (GeneralApp*) this };

    std::unique_ptr<World> m_world {};
    std::unique_ptr<PerspectiveCamera> m_camera {};
    std::unique_ptr<CameraController> m_camera_controller {};

    Graphics::GeometryObject* object = nullptr;

    float m_throttle = 0.0f;
    float m_engine_power = 10.0f;

    int m_left_aileron_spring = 0;
    int m_right_aileron_spring = 0;

    int m_left_elevator_spring = 0;
    int m_right_elevator_spring = 0;

    float m_left_aileron_muscle_length = 0;
    float m_right_aileron_muscle_length = 0;

    float m_left_elevator_muscle_length = 0;
    float m_right_elevator_muscle_length = 0;

    Vec3f m_input_airplane_controls {};
    Vec3f m_real_airplane_controls {};

    Graphics::SceneTexture* m_runway_texture = nullptr;

    std::unique_ptr<Creature> m_creature {};

    void create_window(int width, int height);

    void reset_gl();
    void clear_window();

    bool m_free_camera = false;

public:

    SpringPhysicsApp();

    void on_tick() override;

    void on_mouse_move(double x, double y) override;

    void on_draw() override;

    void on_key_press(sf::Keyboard::Key key) override;
    void on_key_release(sf::Keyboard::Key key) override;

    void build_model();
};