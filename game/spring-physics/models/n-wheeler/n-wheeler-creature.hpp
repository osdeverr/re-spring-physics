#pragma once

#include "../../controllable-creature.hpp"

class NWheelerCreature: public ControllableCreature {

public:
    NWheelerCreature(World* world, const Matrix4f& transform);
    ~NWheelerCreature() override = default;

    void set_controls(Vec3f controls) override;
    void set_throttle(float throttle) override;

    Creature* get_creature() override {
        return m_creature.get();
    };

    float m_throttle = 0.0f;
    float m_engine_power = 10.0f;

    int m_axles = 40;

    std::vector<int> m_steering_springs;

    float m_steering_spring_length = 0;

    std::unique_ptr<Creature> m_creature;
    World* m_world;
};