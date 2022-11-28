#pragma once

class ConcurrentPhysicsEngine;

#include <unordered_set>
#include "vertex.hpp"
#include "spring.hpp"
#include "../../utils/semaphores.h"
#include "../../utils/async-task.hpp"
#include "../creatures/creature.hpp"

class PhysicsThread : public AsyncTask {

    std::unordered_set<Creature*> m_creatures {};

    ConcurrentPhysicsEngine* m_engine = nullptr;

    int m_phase = 0;

public:

    std::unordered_set<Creature*>& get_creatures() { return m_creatures; }

    explicit PhysicsThread(ConcurrentPhysicsEngine* engine);

    void async_task() override;

    void substep(float dt);

    void save_forces();
    void restore_forces();
    void clear_forces();
    void apply_forces(float dt);
    void apply_velocities(float dt);

    void process_vertex_forces(float dt, PhysicsVertex* vertex);
};

#include "physics-engine.hpp"