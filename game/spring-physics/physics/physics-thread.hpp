#pragma once

class ConcurrentPhysicsEngine;

#include <unordered_set>
#include "vertex.hpp"
#include "spring.hpp"
#include "../../utils/semaphores.h"
#include "../../utils/async-task.hpp"
#include "../creatures/creature.hpp"
#include "terrain-polygon.hpp"

class PhysicsThread : public AsyncTask {

    std::unordered_set<Creature*> m_creatures {};
    ConcurrentPhysicsEngine* m_engine = nullptr;
    std::vector<TerrainPolygon *> m_triangle_buffer {};

    int m_max_vertex_collisions = 4;

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

    void handle_terrain_collision(float dt);

    bool handle_vertex_terrain_collisions(float dt, PhysicsVertex *vertex, Vec3f& from, Vec3f& to);
};

#include "physics-engine.hpp"