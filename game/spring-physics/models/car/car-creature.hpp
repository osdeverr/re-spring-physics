#pragma once

#include "../../controllable-creature.hpp"

class CarCreature : public ControllableCreature {

public:
    CarCreature(World* world, const Matrix4f& transform);
    ~CarCreature() override = default;

    void set_controls(Vec3f controls) override;
    void set_throttle(float throttle) override;

    Creature* get_creature() override {
        return m_creature.get();
    };

    float m_throttle = 0.0f;
    float m_engine_power = 10.0f;

    int m_front_steering_spring = 0;
    int m_back_steering_spring = 0;

    float m_front_steering_spring_length = 0;
    float m_back_steering_spring_length = 0;

    std::unique_ptr<Creature> m_creature;
    World* m_world;
};