#pragma once

#include <vector>
#include <unordered_set>
#include "vertex.hpp"
#include "spring.hpp"
#include "physics-thread.hpp"

class ConcurrentPhysicsEngine {
    std::vector<PhysicsThread*> m_threads {};

    Vec3f m_gravity { 0, -9.81, 0 };
    float m_floor_level = 0.2;
    float m_dt = 1.0f / 60.0f;
    int m_substeps_count = 30;
    int m_thread_count = 8;
    bool m_clear_forces = false;

    template<typename Comparator, typename MemberFunc>
    PhysicsThread* choose_thread(Comparator comp, MemberFunc func);

public:
    ConcurrentPhysicsEngine();
    ~ConcurrentPhysicsEngine();

    void add_creature(Creature* creature);
    void remove_creature(Creature* creature);

    void tick();
    float get_dt();
    int get_substeps();
    Vec3f get_gravity();
    float get_floor_level();

    bool threads_should_clear_forces() { return m_clear_forces; }
};