#pragma once

#include "../../controllable-creature.hpp"
#include "../builder/model-builder.hpp"

class TrainCreature: public ControllableCreature {

public:
    TrainCreature(World* world, const Matrix4f& transform);
    ~TrainCreature() override = default;

    void set_controls(Vec3f controls) override;
    void set_throttle(float throttle) override;

    Creature* get_creature() override {
        return m_creature.get();
    };

    Vec3f get_camera_binding_point() override {
        Vec3f position {};
        for(int i = m_locomotive_start; i < m_locomotive_end; i++) {
            position += m_creature->get_vertices()[i]->get_physics_vertex()->m_position;
        }
        position /= (float)(m_locomotive_end - m_locomotive_start);
        return position;
    }

    float m_throttle = 0.0f;
    float m_engine_power = 10.0f;

    int m_wagons = 20;
    int m_locomotive_axles = 4;

    float m_frame_width = 2.0f;
    float m_wheel_width = 0.5f;
    float m_wagon_length = 4.0f;
    float m_connection_length = 2.0f;

    int m_locomotive_start = 0;
    int m_locomotive_end = 0;

    std::vector<int> m_steering_springs;

    float m_steering_spring_length = 0;

    std::unique_ptr<Creature> m_creature;
    World* m_world;

    int build_wagon(ModelBuilder::Builder &builder, int connection);
    int build_locomotive(ModelBuilder::Builder &builder);
    void connect_axles(ModelBuilder::Builder &builder, int frame_a, int frame_b);

    int build_axis(float x, ModelBuilder::Builder &builder);
};