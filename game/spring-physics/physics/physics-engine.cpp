//
// Created by Артем on 04.02.2022.
//

#include "physics-engine.hpp"
#include "../../utils/vec2.hpp"

void ConcurrentPhysicsEngine::tick() {
    for(int i = 0; i < m_substeps_count; i++) {
        m_clear_forces = i == m_substeps_count - 1;
        m_terrain.ensure_valid();
        for(auto &thread: m_threads) thread->run_async();
        for(auto &thread: m_threads) thread->wait();
    }
}

ConcurrentPhysicsEngine::ConcurrentPhysicsEngine() {
    for(int i = 0; i < m_thread_count; i++) {
        m_threads.push_back(new PhysicsThread(this));
    }
}

float ConcurrentPhysicsEngine::get_dt() {
    return m_dt;
}

int ConcurrentPhysicsEngine::get_substeps() {
    return m_substeps_count;
}

Vec3f ConcurrentPhysicsEngine::get_gravity() {
    return m_gravity;
}

float ConcurrentPhysicsEngine::get_floor_level() {
    return m_floor_level;
}

template<typename Comparator, typename MemberFunc>
PhysicsThread* ConcurrentPhysicsEngine::choose_thread(Comparator comp, MemberFunc func) {
    PhysicsThread* thread = m_threads[0];

    for(int i = 1; i < m_threads.size(); i++) {
        if(comp((m_threads[i]->*func)().size(), (thread->*func)().size())) thread = m_threads[i];
    }

    return thread;
}


ConcurrentPhysicsEngine::~ConcurrentPhysicsEngine() {
    for(auto thread : m_threads) { delete thread; }
    m_threads.clear();
}

void ConcurrentPhysicsEngine::remove_creature(Creature* creature) {
    for(auto thread : m_threads) {
        if(thread->get_creatures().erase(creature) == 1) return;
    }
}

void ConcurrentPhysicsEngine::add_creature(Creature* creature) {
    PhysicsThread* least_loaded_thread = choose_thread(std::less<>(), &PhysicsThread::get_creatures);

    least_loaded_thread->get_creatures().insert(creature);
}
