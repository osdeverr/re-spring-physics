#pragma once

#include "../utils/vec3.hpp"
#include "world.hpp"

class ControllableCreature {
public:
    ControllableCreature() = default;
    virtual ~ControllableCreature() = default;

    virtual void set_controls(Vec3f controls) = 0;
    virtual void set_throttle(float throttle) = 0;

    virtual Creature* get_creature() = 0;
};