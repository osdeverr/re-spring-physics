#pragma once

#include "../utils/vec3.hpp"
#include "world.hpp"

class ControllableCreature {
public:
    ControllableCreature() = default;
    virtual ~ControllableCreature() = default;

    virtual void set_controls(Vec3f controls) = 0;
    virtual void set_throttle(float throttle) = 0;

    virtual Vec3f get_camera_binding_point() {
        return get_creature()->get_center();
    }

    virtual Creature* get_creature() = 0;
};