#pragma once

#include "../controllable-creature.hpp"
#include "../creatures/creature.hpp"

class AirplaneCreature : public ControllableCreature {
public:
    AirplaneCreature(World* world);
    virtual ~AirplaneCreature() = default;

    void set_controls(Vec3f controls) override;
    void set_throttle(float throttle) override;
    Creature* get_creature() override {
        return m_creature.get();
    };

    float m_throttle = 0.0f;
    float m_engine_power = 13.0f;

    int m_left_aileron_spring = 0;
    int m_right_aileron_spring = 0;

    int m_left_elevator_spring = 0;
    int m_right_elevator_spring = 0;

    float m_left_aileron_muscle_length = 0;
    float m_right_aileron_muscle_length = 0;

    float m_left_elevator_muscle_length = 0;
    float m_right_elevator_muscle_length = 0;

    std::unique_ptr<Creature> m_creature;
    World* m_world;
};